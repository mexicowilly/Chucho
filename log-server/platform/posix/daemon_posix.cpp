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

#include "daemon.hpp"
#include <chucho/log.hpp>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

namespace chucho
{

namespace server
{

namespace daemon
{

void possess(std::function<void()> func)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        int err = errno;
        CHUCHO_FATAL(chucho::logger::get("chuchod"), "Unable to fork: " << std::strerror(err));
        std::exit(EXIT_FAILURE);
    }
    else if (pid != 0)
    {
        _exit(EXIT_SUCCESS);
    }
    setsid();
    // don't close all fds because the log file is one of them
    chdir("/");
    int null = open("/dev/null", O_RDWR);
    if (null != -1)
    {
        dup2(null, STDIN_FILENO);
        dup2(null, STDOUT_FILENO);
        dup2(null, STDERR_FILENO);
        close(null);
    }
    func();
}

}

}

}
