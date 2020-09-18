/*
 * Copyright 2013-2020 Will Mason
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

#include <chucho/yaml_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/calendar.hpp>
#include <chucho/host.hpp>
#include <chucho/diagnostic_context.hpp>
#include <chucho/process.hpp>
#include <sstream>
#include <thread>
#define YAML_DECLARE_STATIC
#include <yaml.h>

namespace
{

void append_mapping(yaml_document_t& doc, int node, const char* const key, const char* const value)
{
    auto k = yaml_document_add_scalar(&doc,
                                      nullptr,
                                      reinterpret_cast<yaml_char_t*>(const_cast<char*>(key)),
                                      strlen(key),
                                      YAML_PLAIN_SCALAR_STYLE);
    auto v = yaml_document_add_scalar(&doc,
                                      nullptr,
                                      reinterpret_cast<yaml_char_t*>(const_cast<char*>(value)),
                                      strlen(value),
                                      YAML_PLAIN_SCALAR_STYLE);
    yaml_document_append_mapping_pair(&doc, node, k, v);
}

int write_handler(void* udata, unsigned char* buf, std::size_t sz)
{
    auto str = reinterpret_cast<std::string*>(udata);
    str->append(reinterpret_cast<char*>(buf), sz);
    return 1;
}

}

namespace chucho
{

yaml_formatter::yaml_formatter(style styl, time_zone tz, const std::string& time_format)
  : serialization_formatter(styl, tz, time_format)
{
}

yaml_formatter::yaml_formatter(field_disposition dis,
                               const std::vector<field>& fields,
                               style styl,
                               time_zone tz,
                               const std::string& time_format)
  : serialization_formatter(dis, fields, styl, tz, time_format)
{
}

std::string yaml_formatter::format(const event& evt)
{
    yaml_document_t doc;
    yaml_document_initialize(&doc, nullptr, nullptr, nullptr, 1, 1);
    auto node = yaml_document_add_mapping(&doc,
                                          nullptr,
                                          (style_ == style::COMPACT) ? YAML_FLOW_MAPPING_STYLE : YAML_BLOCK_MAPPING_STYLE);
    if (fields_.test(static_cast<std::size_t>(field::LOGGER)))
        append_mapping(doc, node, "logger", evt.get_logger()->get_name().c_str());
    if (fields_.test(static_cast<std::size_t>(field::LEVEL)))
        append_mapping(doc, node, "level", evt.get_level()->get_name());
    if (fields_.test(static_cast<std::size_t>(field::MESSAGE)))
        append_mapping(doc, node, "message", evt.get_message().c_str());
    if (fields_.test(static_cast<std::size_t>(field::FILE_NAME)))
        append_mapping(doc, node, "file_name", evt.get_file_name());
    if (fields_.test(static_cast<std::size_t>(field::LINE_NUMBER)))
        append_mapping(doc, node, "line_number", std::to_string(evt.get_line_number()).c_str());
    if (fields_.test(static_cast<std::size_t>(field::FUNCTION_NAME)))
        append_mapping(doc, node, "function_name", evt.get_function_name());
    if (evt.get_marker() && fields_.test(static_cast<std::size_t>(field::MARKER)))
    {
        std::ostringstream stream;
        stream << *evt.get_marker();
        append_mapping(doc, node, "marker", stream.str().c_str());
    }
    if (fields_.test(static_cast<std::size_t>(field::THREAD)))
    {
        std::ostringstream stream;
        stream << std::this_thread::get_id();
        append_mapping(doc, node, "thread", stream.str().c_str());
    }
    if (fields_.test(static_cast<std::size_t>(field::TIMESTAMP)))
        append_mapping(doc, node, "timestamp", cal_fmt_->format(evt.get_time().time_since_epoch()).c_str());
    if (fields_.test(static_cast<std::size_t>(field::HOST_NAME)))
        append_mapping(doc, node, "host_name", host::get_full_name().c_str());
    if (!diagnostic_context::empty() && fields_.test(static_cast<std::size_t>(field::DIAGNOSTIC_CONTEXT)))
    {
        auto dc = diagnostic_context::get();
        auto dc_node = yaml_document_add_mapping(&doc,
                                                 nullptr,
                                                 (style_ == style::COMPACT) ? YAML_FLOW_MAPPING_STYLE : YAML_BLOCK_MAPPING_STYLE);
        for (const auto& kv : dc)
            append_mapping(doc, dc_node, kv.first.c_str(), kv.second.c_str());
        const char* dc_name = "diagnostic_context";
        auto k = yaml_document_add_scalar(&doc,
                                          nullptr,
                                          reinterpret_cast<yaml_char_t*>(const_cast<char*>(dc_name)),
                                          strlen(dc_name),
                                          YAML_PLAIN_SCALAR_STYLE);
        yaml_document_append_mapping_pair(&doc, node, k, dc_node);
    }
    if (fields_.test(static_cast<std::size_t>(field::PROCESS_ID)))
        append_mapping(doc, node, "process_id", std::to_string(process::id()).c_str());
    yaml_emitter_t emitter;
    yaml_emitter_initialize(&emitter);
    std::string result;
    yaml_emitter_set_output(&emitter, write_handler, &result);
    yaml_emitter_set_encoding(&emitter, YAML_UTF8_ENCODING);
    yaml_emitter_set_indent(&emitter, 2);
    yaml_emitter_set_unicode(&emitter, 1);
    yaml_emitter_open(&emitter);
    yaml_emitter_dump(&emitter, &doc);
    yaml_emitter_close(&emitter);
    yaml_emitter_delete(&emitter);
    return result;
}

}
