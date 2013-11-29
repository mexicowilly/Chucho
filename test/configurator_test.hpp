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

#include <gtest/gtest.h>
#include <chucho/configurator.hpp>

namespace chucho
{

namespace test
{

class configurator : public ::testing::Test
{
public:
    configurator();
    ~configurator();

protected:
    void async_writer_body();
    void async_writer_with_opts_body();
    void bzip2_file_compressor_body();
    void cerr_writer_body();
    virtual chucho::configurator& get_configurator() = 0;
    void configure(const char* const cnf);
    void cout_writer_body();
    void duplicate_message_filter_body();
    void file_writer_body();
    void gzip_file_compressor_body();
    void level_filter_body(const std::string& tmpl);
    void level_threshold_filter_body();
    void logger_body();
    void multiple_writer_body();
    void numbered_file_roller_body();
    void remote_writer_body();
    void rolling_file_writer_body();
    void size_file_roll_trigger_body(const std::string& tmpl);
    void sliding_numbered_file_roller_body();
    void syslog_writer_body();
    void syslog_writer_facility_body(const std::string& tmpl);
    void time_file_roller_body();
    void zip_file_compressor_body();
};

}

}
