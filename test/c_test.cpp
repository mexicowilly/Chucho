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

#include <gtest/gtest.h>
#include <chucho/file.hpp>
#include <chucho/logger.h>
#include <fstream>

TEST(c, simple)
{
#if defined(CHUCHO_YAML_CONFIG)
    std::string helper_name(std::string("test") + chucho::file::dir_sep + "c-test-helper");
    #if defined(CHUCHO_WINDOWS)
    helper_name += ".exe";
    #endif
    ASSERT_TRUE(chucho::file::exists(helper_name));
    ASSERT_EQ(0, std::system((helper_name + " c-test.log").c_str()));
    ASSERT_TRUE(chucho::file::exists("c-test.log"));
    std::ifstream stream("c-test.log");
    ASSERT_TRUE(stream.is_open());
    std::vector<std::string> lines;
    while (stream)
    {
        lines.emplace_back("");
        std::getline(stream, lines.back());
    }
    stream.close();
    lines.pop_back();
#if defined(CHUCHO_HAVE_C_GENERIC)
    std::cout << "Expecting tests using _Generic" << std::endl;
    ASSERT_EQ(64, lines.size());
    EXPECT_STREQ("TRACE c 1", lines[0].c_str());
    EXPECT_STREQ("TRACE c_by_logger 2", lines[1].c_str());
    EXPECT_STREQ("TRACE c_by_logger 3", lines[2].c_str());
    EXPECT_STREQ("TRACE mark c 4", lines[3].c_str());
    EXPECT_STREQ("TRACE mark c_by_logger 5", lines[4].c_str());
    EXPECT_STREQ("TRACE mark c_by_logger 6", lines[5].c_str());
    EXPECT_STREQ("DEBUG c 7", lines[6].c_str());
    EXPECT_STREQ("DEBUG c_by_logger 8", lines[7].c_str());
    EXPECT_STREQ("DEBUG c_by_logger 9", lines[8].c_str());
    EXPECT_STREQ("DEBUG mark c 10", lines[9].c_str());
    EXPECT_STREQ("DEBUG mark c_by_logger 11", lines[10].c_str());
    EXPECT_STREQ("DEBUG mark c_by_logger 12", lines[11].c_str());
    EXPECT_STREQ("INFO c 13", lines[12].c_str());
    EXPECT_STREQ("INFO c_by_logger 14", lines[13].c_str());
    EXPECT_STREQ("INFO c_by_logger 15", lines[14].c_str());
    EXPECT_STREQ("INFO mark c 16", lines[15].c_str());
    EXPECT_STREQ("INFO mark c_by_logger 17", lines[16].c_str());
    EXPECT_STREQ("INFO mark c_by_logger 18", lines[17].c_str());
    EXPECT_STREQ("WARN c 19", lines[18].c_str());
    EXPECT_STREQ("WARN c_by_logger 20", lines[19].c_str());
    EXPECT_STREQ("WARN c_by_logger 21", lines[20].c_str());
    EXPECT_STREQ("WARN mark c 22", lines[21].c_str());
    EXPECT_STREQ("WARN mark c_by_logger 23", lines[22].c_str());
    EXPECT_STREQ("WARN mark c_by_logger 24", lines[23].c_str());
    EXPECT_STREQ("ERROR c 25", lines[24].c_str());
    EXPECT_STREQ("ERROR c_by_logger 26", lines[25].c_str());
    EXPECT_STREQ("ERROR c_by_logger 27", lines[26].c_str());
    EXPECT_STREQ("ERROR mark c 28", lines[27].c_str());
    EXPECT_STREQ("ERROR mark c_by_logger 29", lines[28].c_str());
    EXPECT_STREQ("ERROR mark c_by_logger 30", lines[29].c_str());
    EXPECT_STREQ("FATAL c 31", lines[30].c_str());
    EXPECT_STREQ("FATAL c_by_logger 32", lines[31].c_str());
    EXPECT_STREQ("FATAL c_by_logger 33", lines[32].c_str());
    EXPECT_STREQ("FATAL mark c 34", lines[33].c_str());
    EXPECT_STREQ("FATAL mark c_by_logger 35", lines[34].c_str());
    EXPECT_STREQ("FATAL mark c_by_logger 36", lines[35].c_str());

    EXPECT_STREQ("INFO c every n 0", lines[36].c_str());

    EXPECT_STREQ("INFO c_by_logger every n 12000", lines[37].c_str());
    EXPECT_STREQ("INFO c_by_logger every n 18000", lines[38].c_str());

    EXPECT_STREQ("INFO c_by_logger every n 24000", lines[39].c_str());
    EXPECT_STREQ("INFO c_by_logger every n 28000", lines[40].c_str());
    EXPECT_STREQ("INFO c_by_logger every n 32000", lines[41].c_str());

    EXPECT_STREQ("INFO mark c every n 36000", lines[42].c_str());
    EXPECT_STREQ("INFO mark c every n 39000", lines[43].c_str());
    EXPECT_STREQ("INFO mark c every n 42000", lines[44].c_str());
    EXPECT_STREQ("INFO mark c every n 45000", lines[45].c_str());

    EXPECT_STREQ("INFO mark c_by_logger every n 48000", lines[46].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 50000", lines[47].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 52000", lines[48].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 54000", lines[49].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 56000", lines[50].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 58000", lines[51].c_str());

    EXPECT_STREQ("INFO mark c_by_logger every n 60000", lines[52].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 61000", lines[53].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 62000", lines[54].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 63000", lines[55].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 64000", lines[56].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 65000", lines[57].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 66000", lines[58].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 67000", lines[59].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 68000", lines[60].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 69000", lines[61].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 70000", lines[62].c_str());
    EXPECT_STREQ("INFO mark c_by_logger every n 71000", lines[63].c_str());
#else
    std::cout << "Expecting tests without _Generic" << std::endl;
    ASSERT_EQ(44, lines.size());
    EXPECT_STREQ("TRACE c 1", lines[0].c_str());
    EXPECT_STREQ("TRACE c_by_logger 2", lines[1].c_str());
    EXPECT_STREQ("TRACE mark c 3", lines[2].c_str());
    EXPECT_STREQ("TRACE mark c_by_logger 4", lines[3].c_str());
    EXPECT_STREQ("DEBUG c 5", lines[4].c_str());
    EXPECT_STREQ("DEBUG c_by_logger 6", lines[5].c_str());
    EXPECT_STREQ("DEBUG mark c 7", lines[6].c_str());
    EXPECT_STREQ("DEBUG mark c_by_logger 8", lines[7].c_str());
    EXPECT_STREQ("INFO c 9", lines[8].c_str());
    EXPECT_STREQ("INFO c_by_logger 10", lines[9].c_str());
    EXPECT_STREQ("INFO mark c 11", lines[10].c_str());
    EXPECT_STREQ("INFO mark c_by_logger 12", lines[11].c_str());
    EXPECT_STREQ("WARN c 13", lines[12].c_str());
    EXPECT_STREQ("WARN c_by_logger 14", lines[13].c_str());
    EXPECT_STREQ("WARN mark c 15", lines[14].c_str());
    EXPECT_STREQ("WARN mark c_by_logger 16", lines[15].c_str());
    EXPECT_STREQ("ERROR c 17", lines[16].c_str());
    EXPECT_STREQ("ERROR c_by_logger 18", lines[17].c_str());
    EXPECT_STREQ("ERROR mark c 19", lines[18].c_str());
    EXPECT_STREQ("ERROR mark c_by_logger 20", lines[19].c_str());
    EXPECT_STREQ("FATAL c 21", lines[20].c_str());
    EXPECT_STREQ("FATAL c_by_logger 22", lines[21].c_str());
    EXPECT_STREQ("FATAL mark c 23", lines[22].c_str());
    EXPECT_STREQ("FATAL mark c_by_logger 24", lines[23].c_str());

    EXPECT_STREQ("INFO c every n 0", lines[24].c_str());
#endif
    chucho::file::remove("c-test.log");
#endif
}

TEST(c, permits)
{
    chucho_logger_t* lgr;

    lgr = chucho_get_logger("c.permits");
    chucho_logger_set_level(lgr, CHUCHO_WARN);
    EXPECT_FALSE(chucho_logger_permits(lgr, CHUCHO_TRACE));
    EXPECT_FALSE(chucho_logger_permits(lgr, CHUCHO_DEBUG));
    EXPECT_FALSE(chucho_logger_permits(lgr, CHUCHO_INFO));
    EXPECT_TRUE(chucho_logger_permits(lgr, CHUCHO_WARN));
    EXPECT_TRUE(chucho_logger_permits(lgr, CHUCHO_ERROR));
    EXPECT_TRUE(chucho_logger_permits(lgr, CHUCHO_FATAL));
    chucho_release_logger(lgr);
}
