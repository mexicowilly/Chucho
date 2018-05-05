/*
 * Copyright 2013-2018 Will Mason
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

#include <gtest/gtest.h>
#include <chucho/json_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/diagnostic_context.hpp>
#include <chucho/process.hpp>
#include <cJSON.h>
#include <thread>

namespace
{

#if defined(CHUCHO_WINDOWS)
const char* file_name = "\\one\\two";
#else
const char* file_name = "/one/two";
#endif

class json_formatter_test : public ::testing::Test
{
public:
    json_formatter_test()
        : evt_(chucho::logger::get("json logger"),
               chucho::level::INFO_(),
               "hi",
               file_name,
               10,
               "dowdy",
               "chucho")
    {
        chucho::diagnostic_context::clear();
    }

protected:
    chucho::event evt_;
};

}

TEST_F(json_formatter_test, all)
{
    chucho::json_formatter fmt;
    auto s = fmt.format(evt_);
    std::cout << s << std::endl;
    auto json = cJSON_Parse(s.c_str());
    ASSERT_TRUE(json != nullptr);
    auto item = cJSON_GetObjectItem(json, "logger");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("json logger", item->valuestring);
    item = cJSON_GetObjectItem(json, "level");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("INFO", item->valuestring);
    item = cJSON_GetObjectItem(json, "message");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("hi", item->valuestring);
    item = cJSON_GetObjectItem(json, "file_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ(file_name, item->valuestring);
    item = cJSON_GetObjectItem(json, "line_number");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(10, item->valuedouble);
    item = cJSON_GetObjectItem(json, "function_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("dowdy", item->valuestring);
    item = cJSON_GetObjectItem(json, "marker");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("chucho", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "diagnostic_context"));
    item = cJSON_GetObjectItem(json, "process_id");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(chucho::process::id(), item->valuedouble);
    item = cJSON_GetObjectItem(json, "thread");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), item->valuestring);
    stream.str("");
    item = cJSON_GetObjectItem(json, "host_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    item = cJSON_GetObjectItem(json, "timestamp");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_EQ(15, std::strlen(item->valuestring));
    cJSON_Delete(json);

    chucho::json_formatter fmt2(chucho::json_formatter::style::PRETTY);
    s = fmt2.format(evt_);
    std::cout << s << std::endl;
    json = cJSON_Parse(s.c_str());
    ASSERT_TRUE(json != nullptr);
    item = cJSON_GetObjectItem(json, "logger");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("json logger", item->valuestring);
    item = cJSON_GetObjectItem(json, "level");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("INFO", item->valuestring);
    item = cJSON_GetObjectItem(json, "message");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("hi", item->valuestring);
    item = cJSON_GetObjectItem(json, "file_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ(file_name, item->valuestring);
    item = cJSON_GetObjectItem(json, "line_number");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(10, item->valuedouble);
    item = cJSON_GetObjectItem(json, "function_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("dowdy", item->valuestring);
    item = cJSON_GetObjectItem(json, "marker");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("chucho", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "diagnostic_context"));
    item = cJSON_GetObjectItem(json, "process_id");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(chucho::process::id(), item->valuedouble);
    item = cJSON_GetObjectItem(json, "thread");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), item->valuestring);
    stream.str("");
    item = cJSON_GetObjectItem(json, "host_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    item = cJSON_GetObjectItem(json, "timestamp");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_EQ(15, std::strlen(item->valuestring));
    cJSON_Delete(json);
}

TEST_F(json_formatter_test, diagnostic_context)
{
    chucho::json_formatter fmt(chucho::json_formatter::field_disposition::INCLUDED,
                               {chucho::json_formatter::field::DIAGNOSTIC_CONTEXT});
    chucho::diagnostic_context::at("my dog") = "has fleas";
    chucho::diagnostic_context::at("and so") = "do I";
    chucho::event evt(chucho::logger::get("json logger"),
                      chucho::level::INFO_(),
                      "hi",
                      file_name,
                      10,
                      "dowdy",
                      "chucho");
    auto s = fmt.format(evt);
    auto json = cJSON_Parse(s.c_str());
    ASSERT_TRUE(json != nullptr);
    auto item = cJSON_GetObjectItem(json, "diagnostic_context");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsObject(item));
    auto val = cJSON_GetObjectItem(item, "my dog");
    ASSERT_TRUE(cJSON_IsString(val));
    EXPECT_STREQ("has fleas", val->valuestring);
    val = cJSON_GetObjectItem(item, "and so");
    ASSERT_TRUE(cJSON_IsString(val));
    EXPECT_STREQ("do I", val->valuestring);
    cJSON_Delete(json);
    chucho::diagnostic_context::clear();
}

TEST_F(json_formatter_test, exclusion)
{
    chucho::json_formatter fmt(chucho::json_formatter::field_disposition::EXCLUDED,
                               {chucho::json_formatter::field::LOGGER, chucho::json_formatter::field::MESSAGE});
    auto s = fmt.format(evt_);
    auto json = cJSON_Parse(s.c_str());
    ASSERT_TRUE(json != nullptr);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "logger"));
    auto item = cJSON_GetObjectItem(json, "level");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("INFO", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "message"));
    item = cJSON_GetObjectItem(json, "file_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ(file_name, item->valuestring);
    item = cJSON_GetObjectItem(json, "line_number");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(10, item->valuedouble);
    item = cJSON_GetObjectItem(json, "function_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("dowdy", item->valuestring);
    item = cJSON_GetObjectItem(json, "marker");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("chucho", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "diagnostic_context"));
    item = cJSON_GetObjectItem(json, "process_id");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(chucho::process::id(), item->valuedouble);
    item = cJSON_GetObjectItem(json, "thread");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), item->valuestring);
    stream.str("");
    cJSON_Delete(json);

    chucho::json_formatter fmt2(chucho::json_formatter::field_disposition::EXCLUDED,
                                {chucho::json_formatter::field::LOGGER, chucho::json_formatter::field::MESSAGE},
                                chucho::json_formatter::style::PRETTY);
    s = fmt2.format(evt_);
    json = cJSON_Parse(s.c_str());
    ASSERT_TRUE(json != nullptr);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "logger"));
    item = cJSON_GetObjectItem(json, "level");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("INFO", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "message"));
    item = cJSON_GetObjectItem(json, "file_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ(file_name, item->valuestring);
    item = cJSON_GetObjectItem(json, "line_number");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(10, item->valuedouble);
    item = cJSON_GetObjectItem(json, "function_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("dowdy", item->valuestring);
    item = cJSON_GetObjectItem(json, "marker");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("chucho", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "diagnostic_context"));
    item = cJSON_GetObjectItem(json, "process_id");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(chucho::process::id(), item->valuedouble);
    item = cJSON_GetObjectItem(json, "thread");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), item->valuestring);
    stream.str("");
    item = cJSON_GetObjectItem(json, "host_name");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    item = cJSON_GetObjectItem(json, "timestamp");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_EQ(15, std::strlen(item->valuestring));
    cJSON_Delete(json);
}

TEST_F(json_formatter_test, inclusion)
{
    chucho::json_formatter fmt(chucho::json_formatter::field_disposition::INCLUDED,
                               {chucho::json_formatter::field::LOGGER, chucho::json_formatter::field::MESSAGE});
    auto s = fmt.format(evt_);
    auto json = cJSON_Parse(s.c_str());
    ASSERT_TRUE(json != nullptr);
    auto item = cJSON_GetObjectItem(json, "logger");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("json logger", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "level"));
    item = cJSON_GetObjectItem(json, "message");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("hi", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "file_name"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "line_number"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "function_name"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "marker"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "diagnostic_context"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "process_id"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "thread"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "host_name"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "timestamp"));
    cJSON_Delete(json);

    chucho::json_formatter fmt2(chucho::json_formatter::field_disposition::INCLUDED,
                                {chucho::json_formatter::field::LOGGER, chucho::json_formatter::field::MESSAGE},
                                chucho::json_formatter::style::PRETTY);
    s = fmt2.format(evt_);
    json = cJSON_Parse(s.c_str());
    ASSERT_TRUE(json != nullptr);
    item = cJSON_GetObjectItem(json, "logger");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("json logger", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "level"));
    item = cJSON_GetObjectItem(json, "message");
    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(cJSON_IsString(item));
    EXPECT_STREQ("hi", item->valuestring);
    EXPECT_FALSE(cJSON_HasObjectItem(json, "file_name"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "line_number"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "function_name"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "marker"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "diagnostic_context"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "process_id"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "thread"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "host_name"));
    EXPECT_FALSE(cJSON_HasObjectItem(json, "timestamp"));
    cJSON_Delete(json);
}
