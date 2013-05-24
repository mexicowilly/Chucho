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
#include <yaml-cpp/yaml.h>
#include <sstream>
#if defined(CHUCHO_HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#endif
#include <chrono>

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

class wire_event
{
public:
    wire_event(const YAML::Node& node,
               std::shared_ptr<chucho::server::socket_reader> reader);
    wire_event(const wire_event&) = delete;

    wire_event& operator= (const wire_event&) = delete;

    const chucho::event& get_event() const;
    unsigned get_version() const;

private:
    std::unique_ptr<chucho::event> event_;
    std::string file_name_storage_;
    std::string function_name_storage_;
    unsigned version_;
};

wire_event::wire_event(const YAML::Node& node,
                       std::shared_ptr<chucho::server::socket_reader> reader)
{
    const YAML::Node* val = node.FindValue("version");
    if (val == nullptr)
        throw chucho::exception("The event is missing version");
    version_ = std::stoul(val->to<std::string>());
    val = node.FindValue("milliseconds_since_epoch");
    if (val == nullptr)
        throw chucho::exception("The event is missing milliseconds_since_epoch");
    chucho::event::time_type when = chucho::event::clock_type::from_time_t(0) +
        std::chrono::milliseconds(std::stoull(val->to<std::string>()));
    val = node.FindValue("file_name");
    if (val == nullptr)
        throw chucho::exception("The event is missing file_name");
    file_name_storage_ = val->to<std::string>();
    val = node.FindValue("line_number");
    if (val == nullptr)
        throw chucho::exception("The event is missing line_number");
    unsigned line = std::stoul(val->to<std::string>());
    val = node.FindValue("function_name");
    if (val == nullptr)
        throw chucho::exception("The event is missing function_name");
    function_name_storage_ = val->to<std::string>();
    val = node.FindValue("logger");
    if (val == nullptr)
        throw chucho::exception("The event is missing logger");
    std::shared_ptr<chucho::logger> log = chucho::logger::get(val->to<std::string>());
    val = node.FindValue("level");
    if (val == nullptr)
        throw chucho::exception("The event is missing level");
    std::shared_ptr<chucho::level> lvl = chucho::level::from_text(val->to<std::string>());
    val = node.FindValue("marker");
    chucho::optional<chucho::marker> mark;
    if (val != nullptr)
        mark = reconstruct_marker(val->to<std::string>());
    val = node.FindValue("message");
    if (val == nullptr)
        throw chucho::exception("The event is missing message");
    event_.reset(new chucho::event(log,
                                   lvl,
                                   val->to<std::string>(),
                                   file_name_storage_.c_str(),
                                   line,
                                   function_name_storage_.c_str(),
                                   reader->get_base_host(),
                                   reader->get_full_host(),
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

suzerain::suzerain(const properties& props)
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
        //CHUCHO_INFO(logger_, yaml);
        std::istringstream stream(yaml);
        yaml.clear();
        yaml.shrink_to_fit();
        YAML::Parser prs(stream);
        YAML::Node doc;
        while (prs.GetNextDocument(doc))
        {
            for (YAML::Iterator itor = doc.begin(); itor != doc.end(); itor++)
            {
                try
                {
                    if (itor->Type() != YAML::NodeType::Map)
                        throw chucho::exception("Malformed YAML was encountered while processing an event: The top-level element is not a map");
                }
                catch (YAML::Exception& ye)
                {
                    throw chucho::exception("Malformed YAML was encountered while processing an event: The top-level element is not part of a sequence");
                }
                if (itor->begin().first().to<std::string>() == "event")
                {
                    wire_event wevt(itor->begin().second(), reader);
                    std::shared_ptr<chucho::logger> lgr(wevt.get_event().get_logger());
                    if (lgr->permits(wevt.get_event().get_level()))
                        lgr->write(wevt.get_event());
                    event_count++;
                }
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
        CHUCHO_ERROR(logger_, std::string("Error processing events: ") + e.what());
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
    single_instance::ensure();
    signal_handler::install();
    vassals_.reset(new vassals(props_.vassal_count(),
                               std::bind(&suzerain::process_events, this, std::placeholders::_1)));
    selector_.reset(new selector(std::bind(&suzerain::was_selected, this, std::placeholders::_1)));
    std::unique_ptr<socket_listener> lstn;
    try
    {
        lstn.reset(new socket_listener(props_.port()));
    }
    catch (std::exception& e)
    {
        CHUCHO_ERROR(logger_, "Unable to create a listener socket: " << e.what());
        return;
    }
    try
    {
        while (!is_shut_down)
        {
            std::shared_ptr<socket_reader> reader = lstn->accept();
            selector_->add(reader);
            CHUCHO_INFO(logger_, "Accepted new connection from " << reader->get_full_host());
        }
        CHUCHO_INFO_STR(logger_, "The listener has been shut down");
    }
    catch (shutdown_exception& se)
    {
        CHUCHO_INFO_STR(logger_, "The listener has been shut down");
    }
    catch (eof_exception&)
    {
        CHUCHO_ERROR_STR(logger_, "The listener socket has unexpectedly closed");
    }
    catch (std::exception& e)
    {
        CHUCHO_ERROR(logger_, "An unexpected exception has occurred: " << e.what());

    }
    selector_.reset();
    vassals_.reset();
    single_instance::release();
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
