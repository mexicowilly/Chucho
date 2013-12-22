/*
 * Copyright 2013-2014 Will Mason
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

#include "single_instance.hpp"
#include <chucho/log.hpp>
#include <fstream>
#include <signal.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <unistd.h>

namespace
{

const char* PID_FILE_NAME = "/var/run/chuchod.pid";

}

namespace chucho
{

namespace server
{

namespace single_instance
{

void ensure()
{
    std::shared_ptr<chucho::logger> lgr(chucho::logger::get("chuchod"));
    int rc = access(PID_FILE_NAME, W_OK | R_OK);
    if (rc == 0 || errno == ENOENT)
    {
        if (rc == 0)
        {
            std::ifstream pid_file(PID_FILE_NAME);
            pid_t pid;
            pid_file >> pid;
            if (!pid_file.eof())
            {
                CHUCHO_ERROR(lgr, "There was an error reading the file " << PID_FILE_NAME);
                std::exit(EXIT_FAILURE);
            }
            if (kill(pid, 0) == 0 || errno != ESRCH)
            {
                CHUCHO_ERROR(lgr, "An instance of chuchod is already running on pid " << pid);
                std::exit(EXIT_FAILURE);
            }
            pid_file.close();
        }
        std::ofstream pid_file(PID_FILE_NAME);
        if (pid_file.is_open())
        {
            pid_file << getpid();
        }
        else
        {
            CHUCHO_ERROR(lgr, "Could not open the file " << PID_FILE_NAME << " for writing");
            std::exit(EXIT_FAILURE);
        }
    }
    else
    {
        int err = errno;
        CHUCHO_ERROR(lgr, "There is a problem with access to the file " <<
            PID_FILE_NAME << ": " << std::strerror(err));
        std::exit(EXIT_FAILURE);
    }
}

void release()
{
    std::remove(PID_FILE_NAME);
}

}

}

}
