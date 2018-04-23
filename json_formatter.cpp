#include <chucho/json_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/calendar.hpp>
#include <chucho/host.hpp>
#include <chucho/diagnostic_context.hpp>
#include <cJSON.h>
#include <sstream>
#include <thread>

namespace chucho
{

json_formatter::json_formatter(style styl, time_zone tz, const std::string& time_format)
    : style_(styl), fmt_(std::make_unique<calendar::formatter>(time_format,
                                                               (tz == time_zone::LOCAL) ?
                                                                 calendar::formatter::location::LOCAL :
                                                                 calendar::formatter::location::UTC))
{
    fields_.set();
}

json_formatter::json_formatter(field_disposition dis,
                               const std::vector<field>& fields,
                               style styl,
                               time_zone tz,
                               const std::string& time_format)
    : json_formatter(styl, tz, time_format)
{
    if (dis == field_disposition::INCLUDED)
    {
        for (auto f : fields)
            fields_.set(static_cast<std::size_t>(f));
    }
    else
    {
        fields_.set();
        for (auto f : fields)
            fields_.reset(static_cast<std::size_t>(f));
    }
}

json_formatter::~json_formatter()
{
}

std::string json_formatter::format(const event& evt)
{
    std::string result;
    auto json = cJSON_CreateObject();
    if (fields_.test(static_cast<std::size_t>(field::LOGGER)))
        cJSON_AddStringToObject(json, "logger", evt.get_logger()->get_name().c_str());
    if (fields_.test(static_cast<std::size_t>(field::LEVEL)))
        cJSON_AddStringToObject(json, "level", evt.get_level()->get_name());
    if (fields_.test(static_cast<std::size_t>(field::MESSAGE)))
        cJSON_AddStringToObject(json, "message", evt.get_message().c_str());
    if (fields_.test(static_cast<std::size_t>(field::FILE_NAME)))
        cJSON_AddStringToObject(json, "file_name", evt.get_file_name());
    if (fields_.test(static_cast<std::size_t>(field::LINE_NUMBER)))
        cJSON_AddNumberToObject(json, "line_number", evt.get_line_number());
    if (fields_.test(static_cast<std::size_t>(field::FUNCTION_NAME)))
        cJSON_AddStringToObject(json, "function_name", evt.get_function_name());
    if (evt.get_marker() && fields_.test(static_cast<std::size_t>(field::MARKER)))
    {
        std::ostringstream stream;
        stream << *evt.get_marker();
        cJSON_AddStringToObject(json, "marker", stream.str().c_str());
    }
    if (fields_.test(static_cast<std::size_t>(field::THREAD)))
    {
        std::ostringstream stream;
        stream << std::this_thread::get_id();
        cJSON_AddStringToObject(json, "thread", stream.str().c_str());
    }
    if (fields_.test(static_cast<std::size_t>(field::TIMESTAMP)))
    {
        cJSON_AddStringToObject(json, "timestamp", fmt_->format(evt.get_time().time_since_epoch()).c_str());
    }
    if (fields_.test(static_cast<std::size_t>(field::HOST_NAME)))
        cJSON_AddStringToObject(json, "host_name", host::get_full_name().c_str());
    if (!diagnostic_context::empty() && fields_.test(static_cast<std::size_t>(field::DIAGNOSTIC_CONTEXT)))
    {
        auto dc = diagnostic_context::get();
        auto jdc = cJSON_CreateObject();
        for (const auto& kv : dc)
            cJSON_AddStringToObject(jdc, kv.first.c_str(), kv.second.c_str());
        cJSON_AddItemToObject(json, "diagnostic_context", jdc);
    }
    char* raw = (style_ == style::PRETTY) ? cJSON_Print(json) : cJSON_PrintUnformatted(json);
    result = raw;
    free(raw);
    cJSON_Delete(json);
    return result;
}

}