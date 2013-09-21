/*
 * Copyright 2013 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "suzerain.hpp"
#include "socket_listener.hpp"
#include "is_shut_down.hpp"
#include "eof_exception.hpp"
#include "single_instance.hpp"
#include "signal_handler.hpp"
#include <chucho/log.hpp>
#include <chucho/event.hpp>
#include <chucho/optional.hpp>
#include <chucho/configuration.hpp>
#include <chucho/non_copyable.hpp>
#include <yaml.h>
#include <sstream>
#if defined(CHUCHO_HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#elif defined(CHUCHO_HAVE_WINSOCK2_H)
#include <winsock2.h>
#endif
#include <chrono>
#include <bitset>
#include <cstring>
#include <stdlib.h>

namespace
{

chucho::marker reconstruct_marker(const std::string& text)
{
    std::size_t pos = text.find('[');
    if (pos == std::string::npos)
        return chucho::marker(text);
    chucho::marker result("");
    std::size_t pos2;
    if (pos > 0)
    {
        pos2 = text.find_last_not_of(' ', pos - 1);
        if (pos2 != std::string::npos)
            pos2++;
        result = text.substr(0, pos2);
    }
    pos++;
    while (text[pos] == ' ')
        pos++;
    unsigned depth = 1;
    std::size_t lpos = pos;
    while (++lpos < text.length())
    {
        bool at_end = false;
        if (text[lpos] == '[')
        {
            depth++;
        }
        else if (text[lpos] == ']')
        {
            if (--depth == 0)
                at_end = true;
        }
        else if (text[lpos] == ',')
        {
            if (depth == 1)
                at_end = true;
        }
        if (at_end)
        {
            std::string child = text.substr(pos, lpos - pos);
            pos = lpos + 1;
            while (text[pos] == ' ')
                pos++;
            lpos = pos + 1;
            pos2 = child.find_last_not_of(' ');
            if (pos2 != std::string::npos)
                child.erase(pos2 + 1);
            chucho::marker mchild(reconstruct_marker(child));
            result.insert(mchild);
        }
    }
    return result;
}

std::uint64_t str_to_64(const char* str)
{
#if defined(CHUCHO_WINDOWS)
    return _strtoui64(str, nullptr, 10);
#else
    return std::strtoull(str, nullptr, 10);
#endif
}

class wire_event : chucho::non_copyable
{
public:
    wire_event(yaml_document_t& doc,
               const yaml_node_t& node,
               std::shared_ptr<chucho::server::socket_reader> reader);

    const chucho::event& get_event() const;
    unsigned get_version() const;

private:
    std::unique_ptr<chucho::event> event_;
    std::string file_name_storage_;
    std::string function_name_storage_;
    unsigned version_;
};

wire_event::wire_event(yaml_document_t& doc,
                       const yaml_node_t& node,
                       std::shared_ptr<chucho::server::socket_reader> reader)
{
    enum requireds
    {
        VERSION,
        MILLISECONDS_SINCE_EPOCH,
        FILE_NAME,
        LINE_NUMBER,
        FUNCTION_NAME,
        LOGGER,
        LEVEL,
        THREAD,
        MESSAGE,
        COUNT
    };
    std::bitset<COUNT> found;
    chucho::event::time_type when;
    const char* file_name;
    unsigned line_number;
    const char* function_name;
    std::shared_ptr<chucho::logger> logger;
    std::shared_ptr<chucho::level> level;
    chucho::optional<chucho::marker> mark;
    const char* thread;
    std::string msg;
    for (yaml_node_pair_t* p = node.data.mapping.pairs.start;
         p < node.data.mapping.pairs.top;
         p++)
    {
        yaml_node_t& key_node(*yaml_document_get_node(&doc, p->key));
        yaml_node_t& value_node(*yaml_document_get_node(&doc, p->value));
        if (key_node.type == YAML_SCALAR_NODE && value_node.type == YAML_SCALAR_NODE)
        {
            const char* key = reinterpret_cast<char*>(key_node.data.scalar.value);
            const char* value = reinterpret_cast<char*>(value_node.data.scalar.value);
            if (std::strcmp(key, "version") == 0)
            {
                version_ = std::atoi(value);
                found.set(VERSION);
            }
            else if (std::strcmp(key, "milliseconds_since_epoch") == 0)
            {
                when = chucho::event::clock_type::from_time_t(0) +
                    std::chrono::milliseconds(str_to_64(value));
                found.set(MILLISECONDS_SINCE_EPOCH);
            }
            else if (std::strcmp(key, "file_name") == 0)
            {
                file_name = value;
                found.set(FILE_NAME);
            }
            else if (std::strcmp(key, "line_number") == 0)
            {
                line_number = std::atoi(value);
                found.set(LINE_NUMBER);
            }
            else if (std::strcmp(key, "function_name") == 0)
            {
                function_name = value;
                found.set(FUNCTION_NAME);
            }
            else if (std::strcmp(key, "logger") == 0)
            {
                logger = chucho::logger::get(value);
                found.set(LOGGER);
            }
            else if (std::strcmp(key, "level") == 0)
            {
                level = chucho::level::from_text(value);
                found.set(LEVEL);
            }
            else if (std::strcmp(key, "marker") == 0)
            {
                mark = reconstruct_marker(value);
            }
            else if (std::strcmp(key, "thread") == 0)
            {
                thread = value;
                found.set(THREAD);
            }
            else if (std::strcmp(key, "message") == 0)
            {
                msg = value;
                // If the message comes in with YAML literal syntax ('|'), then it might
                // have a trailing newline (if there is more than one event in the YAML).
                if (!msg.empty() && msg.back() == '\n')
                {
                    msg.pop_back();
                    if (!msg.empty() && msg.back() == '\r')
                        msg.pop_back();
                }
                found.set(MESSAGE);
            }
            else
            {
                throw chucho::exception(std::string("Unknown key found in YAML event: ") +
                    key);
            }
        }
        else
        {
            throw chucho::exception("Invalid node type: key is " + std::to_string(key_node.type) + " and value is " + std::to_string(value_node.type));
        }
    }
    if (!found.all())
    {
        throw chucho::exception("Not all required keys were found in the event node: " + found.to_string());
    }
    event_.reset(new chucho::event(logger,
                                   level,
                                   msg,
                                   file_name,
                                   line_number,
                                   function_name,
                                   reader->get_base_host(),
                                   reader->get_full_host(),
                                   thread,
                                   mark));
}

inline const chucho::event& wire_event::get_event() const
{
    return *event_;
}

inline unsigned wire_event::get_version() const
{
    return version_;
}

}

namespace chucho
{

namespace server
{

suzerain::suzerain(properties& props)
    : logger_(chucho::logger::get("chuchod.suzerain")),
      props_(props)
      #if defined(CHUCHOD_VELOCITY)
      ,
      first_seen_(std::chrono::steady_clock::time_point::min()),
      last_processed_(std::chrono::steady_clock::time_point::min()),
      total_event_count_(0)
      #endif
{
}

suzerain::~suzerain()
{
    #if defined(CHUCHOD_VELOCITY)
    if (total_event_count_ > 0)
    {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(last_processed_ - first_seen_).count();
        CHUCHO_INFO(logger_, "Saw " << total_event_count_ << " events in " << seconds << " seconds");
        CHUCHO_INFO(logger_, "Velocity " << total_event_count_ / seconds << " events per second");
    }
    else
    {
        CHUCHO_INFO_STR(logger_, "No events were seen");
    }
    #endif
}

void suzerain::process_events(std::shared_ptr<socket_reader> reader)
{
    std::size_t event_count = 0;
    try
    {
        std::uint32_t size;
        reader->read(reinterpret_cast<std::uint8_t*>(&size), sizeof(size));
        size = ntohl(size);
        std::string yaml(size, 0);
        reader->read(reinterpret_cast<std::uint8_t*>(const_cast<char*>(yaml.data())), size);
        CHUCHO_DEBUG(logger_, yaml);
        yaml_parser_t prs;
        yaml_parser_initialize(&prs);
        struct prs_sentry
        {
            prs_sentry(yaml_parser_t& prs) : prs_(prs) { }
            ~prs_sentry() { yaml_parser_delete(&prs_); }
            yaml_parser_t& prs_;
        } ps(prs);
        yaml_parser_set_input_string(&prs, reinterpret_cast<const yaml_char_t*>(yaml.data()), yaml.length());
        yaml_document_t doc;
        while (true)
        {
            if (!yaml_parser_load(&prs, &doc))
            {
                throw chucho::exception("Error parsing YAML at line " +
                    std::to_string(prs.problem_mark.line) + ", column " +
                    std::to_string(prs.problem_mark.column) + ": " +
                    std::string(prs.problem) + ": " + yaml);
            }
            struct doc_sentry
            {
                doc_sentry(yaml_document_t& doc) : doc_(doc) { }
                ~doc_sentry() { yaml_document_delete(&doc_); }
                yaml_document_t& doc_;
            } ds(doc);
            yaml_node_t* node = yaml_document_get_root_node(&doc);
            if (node == nullptr)
                break;
            if (node->type == YAML_SEQUENCE_NODE)
            {
                for (yaml_node_item_t* i = node->data.sequence.items.start;
                     i < node->data.sequence.items.top;
                     i++)
                {
                    yaml_node_t& evt_node(*yaml_document_get_node(&doc, *i));
                    if (evt_node.type == YAML_MAPPING_NODE)
                    {
                        const yaml_node_t& key =
                            *yaml_document_get_node(&doc, evt_node.data.mapping.pairs.start->key);
                        if (key.type == YAML_SCALAR_NODE &&
                            std::strcmp(reinterpret_cast<const char*>(key.data.scalar.value),
                                        "event") == 0)
                        {
                            const yaml_node_t& maps =
                                *yaml_document_get_node(&doc, evt_node.data.mapping.pairs.start->value);
                            if (maps.type == YAML_MAPPING_NODE)
                            {
                                wire_event wevt(doc, maps, reader);
                                std::shared_ptr<chucho::logger> lgr(wevt.get_event().get_logger());
                                if (lgr->permits(wevt.get_event().get_level()))
                                    lgr->write(wevt.get_event());
                                event_count++;
                            }
                            else
                            {
                                throw chucho::exception("The value of the event mapping must be a mapping");
                            }
                        }
                        else
                        {
                            throw chucho::exception("The sequence may only consist of mappings with a key of \"event\"");
                        }
                    }
                    else
                    {
                        throw chucho::exception("The event node must be a mapping node");
                    }
                }
            }
            else
            {
                throw chucho::exception("The YAML events must be expressed as a sequence of event nodes");
            }
        }
    }
    catch (eof_exception&)
    {
        CHUCHO_INFO(logger_, "The connection on socket " << reader->get_full_host() << " was disconnected by the peer");
        return;
    }
    catch (std::exception& e)
    {
        CHUCHO_ERROR(logger_, "Error processing events: " << e.what());
    }
    CHUCHO_DEBUG(logger_, "Processed " << event_count << " events");
    #if defined(CHUCHOD_VELOCITY)
    velocity_guard_.lock();
    last_processed_ = std::chrono::steady_clock::now();
    total_event_count_ += event_count;
    velocity_guard_.unlock();
    #endif
    selector_->add(reader);
}

void suzerain::run()
{
    if (!props_.is_service())
        single_instance::ensure();
    signal_handler::install(std::bind(&suzerain::sighup_handler, this));
    vassals_.reset(new vassals(props_.vassal_count(),
                               std::bind(&suzerain::process_events, this, std::placeholders::_1)));
    selector_.reset(new selector(std::bind(&suzerain::was_selected, this, std::placeholders::_1)));
    try
    {
        listener_.reset(new socket_listener(props_.port()));
    }
    catch (std::exception& e)
    {
        CHUCHO_ERROR(logger_, "Unable to create a listener socket: " << e.what());
        is_shut_down = true;
        if (!props_.is_service())
            single_instance::release();
        return;
    }
    try
    {
        while (!is_shut_down)
        {
            try
            {
                std::shared_ptr<socket_reader> reader = listener_->accept();
                selector_->add(reader);
                CHUCHO_INFO(logger_, "Accepted new connection from " << reader->get_full_host());
            }
            catch (eof_exception&)
            {
                std::lock_guard<std::mutex> lg(listener_guard_);
                CHUCHO_INFO_STR(logger_, "Resuming connection acceptance");
            }
        }
        CHUCHO_INFO_STR(logger_, "The listener has been shut down");
    }
    catch (shutdown_exception&)
    {
        CHUCHO_INFO_STR(logger_, "The listener has been shut down");
    }
    catch (std::exception& e)
    {
        CHUCHO_ERROR(logger_, "An unexpected exception has occurred: " << e.what());
    }
    // Defect #86.
    // Order is important here. When selector has a hit, then it passes a reader
    // to the vassals for processing. When a vassal is done processing then it
    // passes the reader back to selector. They are interlocking depenendcies.
    // The fix is to stop the vasslas without destroying the object, then to destroy
    // the selector, then to destory the vassals. This cleanly breaks the
    // dependency chain.
    listener_.reset();
    vassals_->stop();
    selector_.reset();
    vassals_.reset();
    if (!props_.is_service())
        single_instance::release();
}

void suzerain::sighup_handler()
{
    properties old_props(props_);
    vassals_->stop();
    selector_->stop();
    if (chucho::configuration::reconfigure())
    {
        try
        {
            std::lock_guard<std::mutex> lg(listener_guard_);
            if (old_props.port() != props_.port())
            {
                listener_.reset(new socket_listener(props_.port()));
                CHUCHO_INFO(logger_, "The listener socket has been reset to listen on port " << props_.port());
            }
        }
        catch (std::exception& e)
        {
            CHUCHO_ERROR(logger_, "Unable to create a listener socket: " << e.what());
            is_shut_down = true;
            return;
        }
        if (!chucho::configuration::get_loaded_file_name().empty())
        {
            CHUCHO_INFO(logger_, "Reread the configuration from " + chucho::configuration::get_loaded_file_name());
            CHUCHO_INFO(logger_, props_);
        }
    }
    else
    {
        props_ = old_props;
    }
    selector_->start();
    vassals_->start(props_.vassal_count());
}

void suzerain::was_selected(std::shared_ptr<socket_reader> reader)
{
    #if defined(CHUCHOD_VELOCITY)
    velocity_guard_.lock();
    if (first_seen_ == std::chrono::steady_clock::time_point::min())
        first_seen_ = std::chrono::steady_clock::now();
    velocity_guard_.unlock();
    #endif
    vassals_->submit(reader);
    CHUCHO_DEBUG(logger_, "Submitted " << reader->get_full_host() << " for work");
}

}

}
