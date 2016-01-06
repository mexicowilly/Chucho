/*
 * Copyright 2013-2016 Will Mason
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

#include "log.hpp"
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>

namespace chucho
{

namespace server
{

std::string log::get_config_file_name()
{
    return "/etc/chuchod.yaml";
}

std::string log::get_file_name()
{
    struct stat stat_buf;
    std::string cand("/var/log");
    if (!(stat(cand.c_str(), &stat_buf) == 0 &&
          S_ISDIR(stat_buf.st_mode) &&
          access(cand.c_str(), W_OK) == 0))
    {
        struct passwd* entry = getpwuid(getuid());
        if (entry != nullptr)
        {
            cand = entry->pw_dir;
            if (!(stat(cand.c_str(), &stat_buf) == 0 &&
                  S_ISDIR(stat_buf.st_mode) &&
                  access(cand.c_str(), W_OK) == 0))
            {
                cand.clear();
                char* tmpdir = getenv("TMPDIR");
                if (tmpdir != nullptr)
                {
                    if (stat(tmpdir, &stat_buf) == 0 &&
                        S_ISDIR(stat_buf.st_mode) &&
                        access(tmpdir, W_OK) == 0)
                    {
                        cand = tmpdir;
                    }
                }
                if (cand.empty())
                {
                    if (stat(P_tmpdir, &stat_buf) == 0 &&
                        S_ISDIR(stat_buf.st_mode) &&
                        access(P_tmpdir, W_OK) == 0)
                    {
                        cand = P_tmpdir;
                    }
                    else if (stat("/tmp", &stat_buf) == 0 &&
                             S_ISDIR(stat_buf.st_mode) &&
                             access("/tmp", W_OK))
                    {
                        cand = "/tmp";
                    }
                }
            }
        }
    }
    if (cand.empty())
    {
        cand = "chuchod.log";
    }
    else
    {
        if (cand.back() != '/')
            cand += '/';
        cand += "chuchod.log";
    }
    return cand;
}

}

}
