/*
 * Copyright 2013-2021 Will Mason
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

#define YAML_DECLARE_STATIC

#include <gtest/gtest.h>
#include <chucho/yaml_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/diagnostic_context.hpp>
#include <chucho/process.hpp>
#include <chucho/optional.hpp>
#include <yaml.h>
#include <thread>
#include <cstring>

namespace
{

#if defined(CHUCHO_WINDOWS)
const char* file_name = "\\one\\two";
#else
const char* file_name = "/one/two";
#endif

class yaml_formatter_test : public ::testing::Test
{
public:
    yaml_formatter_test()
      : evt_(chucho::logger::get("yaml logger"),
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
    void parse(const std::string& in)
    {
        parsed_.clear();
        parsed_dc_ = chucho::optional<std::map<std::string, std::string>>();
        yaml_parser_t prsr;
        yaml_parser_initialize(&prsr);
        yaml_parser_set_input_string(&prsr, reinterpret_cast<const unsigned char*>(in.c_str()), in.length());
        yaml_document_t doc;
        if (yaml_parser_load(&prsr, &doc))
        {
            auto root = yaml_document_get_root_node(&doc);
            EXPECT_NE(nullptr, root);
            if (root != nullptr)
            {
                EXPECT_EQ(YAML_MAPPING_NODE, root->type);
                if (root->type == YAML_MAPPING_NODE)
                {
                    for (auto* p = root->data.mapping.pairs.start;
                         p < root->data.mapping.pairs.top;
                         p++)
                    {
                        auto k = yaml_document_get_node(&doc, p->key);
                        std::string kstr;
                        EXPECT_EQ(YAML_SCALAR_NODE, k->type);
                        if (k->type == YAML_SCALAR_NODE)
                            kstr = reinterpret_cast<const char*>(k->data.scalar.value);
                        auto v = yaml_document_get_node(&doc, p->value);
                        if (kstr == "diagnostic_context")
                        {
                            EXPECT_EQ(YAML_MAPPING_NODE, v->type);
                            if (v->type == YAML_MAPPING_NODE)
                            {
                                parsed_dc_ = std::map<std::string, std::string>();
                                for (auto* dc_p = v->data.mapping.pairs.start;
                                     dc_p < v->data.mapping.pairs.top;
                                     dc_p++)
                                {
                                    auto dc_k = yaml_document_get_node(&doc, dc_p->key);
                                    EXPECT_EQ(YAML_SCALAR_NODE, dc_k->type);
                                    std::string dc_kstr;
                                    if (dc_k->type == YAML_SCALAR_NODE)
                                        dc_kstr = reinterpret_cast<const char*>(dc_k->data.scalar.value);
                                    auto dc_v = yaml_document_get_node(&doc, dc_p->value);
                                    EXPECT_EQ(YAML_SCALAR_NODE, dc_v->type);
                                    std::string dc_vstr;
                                    if (dc_v->type == YAML_SCALAR_NODE)
                                        dc_vstr = reinterpret_cast<const char*>(dc_v->data.scalar.value);
                                    parsed_dc_->emplace(dc_kstr, dc_vstr);
                                }
                            }
                        }
                        else
                        {
                            EXPECT_EQ(YAML_SCALAR_NODE, v->type);
                            std::string vstr;
                            if (v->type == YAML_SCALAR_NODE)
                                vstr = reinterpret_cast<const char*>(v->data.scalar.value);
                            parsed_.emplace(kstr, vstr);
                        }
                    }
                }
            }
            yaml_document_delete(&doc);
        }
        else
        {
            EXPECT_TRUE(false) << "Could not load document";
        }
        yaml_parser_delete(&prsr);
    }

    chucho::event evt_;
    std::map<std::string, std::string> parsed_;
    chucho::optional<std::map<std::string, std::string>> parsed_dc_;
};

}

TEST_F(yaml_formatter_test, all)
{
    chucho::yaml_formatter fmt;
    auto s = fmt.format(evt_);
    std::cout << s << std::endl;
    parse(s);
    ASSERT_FALSE(parsed_.empty());
    auto found = parsed_.find("logger");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("yaml logger", found->second.c_str());
    found = parsed_.find("level");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("INFO", found->second.c_str());
    found = parsed_.find("message");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("hi", found->second.c_str());
    found = parsed_.find("file_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ(file_name, found->second.c_str());
    found = parsed_.find("line_number");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("10", found->second.c_str());
    found = parsed_.find("function_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("dowdy", found->second.c_str());
    found = parsed_.find("marker");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("chucho", found->second.c_str());
    EXPECT_FALSE(parsed_dc_);
    found = parsed_.find("process_id");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(chucho::process::id(), std::stoi(found->second));
    found = parsed_.find("thread");
    ASSERT_FALSE(found == parsed_.end());
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), found->second.c_str());
    stream.str("");
    found = parsed_.find("host_name");
    ASSERT_FALSE(found == parsed_.end());
    found = parsed_.find("timestamp");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(15, found->second.length());

    chucho::yaml_formatter fmt2(chucho::yaml_formatter::style::PRETTY);
    s = fmt2.format(evt_);
    std::cout << s << std::endl;
    parse(s);
    ASSERT_FALSE(parsed_.empty());
    found = parsed_.find("logger");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("yaml logger", found->second.c_str());
    found = parsed_.find("level");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("INFO", found->second.c_str());
    found = parsed_.find("message");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("hi", found->second.c_str());
    found = parsed_.find("file_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ(file_name, found->second.c_str());
    found = parsed_.find("line_number");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("10", found->second.c_str());
    found = parsed_.find("function_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("dowdy", found->second.c_str());
    found = parsed_.find("marker");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("chucho", found->second.c_str());
    EXPECT_FALSE(parsed_dc_);
    found = parsed_.find("process_id");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(chucho::process::id(), std::stoi(found->second));
    found = parsed_.find("thread");
    ASSERT_FALSE(found == parsed_.end());
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), found->second.c_str());
    found = parsed_.find("host_name");
    ASSERT_FALSE(found == parsed_.end());
    found = parsed_.find("timestamp");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(15, found->second.length());
}

TEST_F(yaml_formatter_test, diagnostic_context)
{
    chucho::yaml_formatter fmt(chucho::yaml_formatter::field_disposition::INCLUDED,
                               {chucho::yaml_formatter::field::DIAGNOSTIC_CONTEXT});
    chucho::diagnostic_context::at("my dog") = "has fleas";
    chucho::diagnostic_context::at("and so") = "do I";
    chucho::event evt(chucho::logger::get("yaml logger"),
                      chucho::level::INFO_(),
                      "hi",
                      file_name,
                      10,
                      "dowdy",
                      "chucho");
    auto s = fmt.format(evt);
    std::cout << s << std::endl;
    parse(s);
    ASSERT_TRUE(parsed_dc_);
    auto found = parsed_dc_->find("my dog");
    ASSERT_FALSE(found == parsed_dc_->end());
    EXPECT_STREQ("has fleas", found->second.c_str());
    found = parsed_dc_->find("and so");
    ASSERT_FALSE(found == parsed_dc_->end());
    EXPECT_STREQ("do I", found->second.c_str());
    chucho::diagnostic_context::clear();
}

TEST_F(yaml_formatter_test, exclusion)
{
    chucho::yaml_formatter fmt(chucho::yaml_formatter::field_disposition::EXCLUDED,
                               {chucho::yaml_formatter::field::LOGGER, chucho::yaml_formatter::field::MESSAGE});
    auto s = fmt.format(evt_);
    std::cout << s << std::endl;
    parse(s);
    ASSERT_FALSE(parsed_.empty());
    auto found = parsed_.find("logger");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("level");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("INFO", found->second.c_str());
    found = parsed_.find("message");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("file_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ(file_name, found->second.c_str());
    found = parsed_.find("line_number");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("10", found->second.c_str());
    found = parsed_.find("function_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("dowdy", found->second.c_str());
    found = parsed_.find("marker");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("chucho", found->second.c_str());
    EXPECT_FALSE(parsed_dc_);
    found = parsed_.find("process_id");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(chucho::process::id(), std::stoi(found->second));
    found = parsed_.find("thread");
    ASSERT_FALSE(found == parsed_.end());
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), found->second.c_str());
    stream.str("");
    found = parsed_.find("host_name");
    ASSERT_FALSE(found == parsed_.end());
    found = parsed_.find("timestamp");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(15, found->second.length());

    chucho::yaml_formatter fmt2(chucho::yaml_formatter::field_disposition::EXCLUDED,
                                {chucho::yaml_formatter::field::LOGGER, chucho::yaml_formatter::field::MESSAGE},
                                chucho::yaml_formatter::style::PRETTY);
    s = fmt2.format(evt_);
    std::cout << s << std::endl;
    parse(s);
    ASSERT_FALSE(parsed_.empty());
    found = parsed_.find("logger");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("level");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("INFO", found->second.c_str());
    found = parsed_.find("message");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("file_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ(file_name, found->second.c_str());
    found = parsed_.find("line_number");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("10", found->second.c_str());
    found = parsed_.find("function_name");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("dowdy", found->second.c_str());
    found = parsed_.find("marker");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("chucho", found->second.c_str());
    EXPECT_FALSE(parsed_dc_);
    found = parsed_.find("process_id");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(chucho::process::id(), std::stoi(found->second));
    found = parsed_.find("thread");
    ASSERT_FALSE(found == parsed_.end());
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), found->second.c_str());
    found = parsed_.find("host_name");
    ASSERT_FALSE(found == parsed_.end());
    found = parsed_.find("timestamp");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_EQ(15, found->second.length());
}

TEST_F(yaml_formatter_test, inclusion)
{
    chucho::yaml_formatter fmt(chucho::yaml_formatter::field_disposition::INCLUDED,
                               {chucho::yaml_formatter::field::LOGGER, chucho::yaml_formatter::field::MESSAGE});
    auto s = fmt.format(evt_);
    std::cout << s << std::endl;
    parse(s);
    ASSERT_FALSE(parsed_.empty());
    auto found = parsed_.find("logger");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("yaml logger", found->second.c_str());
    found = parsed_.find("level");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("message");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("hi", found->second.c_str());
    found = parsed_.find("file_name");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("line_number");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("function_name");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("marker");
    ASSERT_TRUE(found == parsed_.end());
    EXPECT_FALSE(parsed_dc_);
    found = parsed_.find("process_id");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("thread");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("host_name");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("timestamp");
    ASSERT_TRUE(found == parsed_.end());

    chucho::yaml_formatter fmt2(chucho::yaml_formatter::field_disposition::INCLUDED,
                                {chucho::yaml_formatter::field::LOGGER, chucho::yaml_formatter::field::MESSAGE},
                                chucho::yaml_formatter::style::PRETTY);
    s = fmt2.format(evt_);
    std::cout << s << std::endl;
    parse(s);
    ASSERT_FALSE(parsed_.empty());
    found = parsed_.find("logger");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("yaml logger", found->second.c_str());
    found = parsed_.find("level");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("message");
    ASSERT_FALSE(found == parsed_.end());
    EXPECT_STREQ("hi", found->second.c_str());
    found = parsed_.find("file_name");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("line_number");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("function_name");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("marker");
    ASSERT_TRUE(found == parsed_.end());
    EXPECT_FALSE(parsed_dc_);
    found = parsed_.find("process_id");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("thread");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("host_name");
    ASSERT_TRUE(found == parsed_.end());
    found = parsed_.find("timestamp");
    ASSERT_TRUE(found == parsed_.end());
}
