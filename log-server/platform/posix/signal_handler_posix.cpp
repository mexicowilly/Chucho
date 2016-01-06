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

#include "signal_handler.hpp"
#include "is_shut_down.hpp"
#include <chucho/log.hpp>
#include <signal.h>
#include <array>
#include <map>
#include <thread>
#include <cstring>
#include <algorithm>

namespace
{

const std::array<int, 11> ASYNC =
{
    SIGHUP,
    SIGINT,
    SIGQUIT,
    SIGPIPE,
    SIGTERM,
    SIGUSR1,
    SIGUSR2,
    SIGCHLD,
    SIGVTALRM,
    SIGXCPU,
    SIGXFSZ
};

const std::array<int, 6> DEADLY_ASYNC =
{
    SIGINT,
    SIGQUIT,
    SIGPIPE,
    SIGTERM,
    SIGXCPU,
    SIGXFSZ
};

const std::array<int, 7> SYNC =
{
    SIGILL,
    SIGABRT,
    SIGFPE,
    SIGSEGV,
    SIGTRAP,
    SIGBUS,
    SIGSYS
};

std::pair<const char*, const char*> get_info(int sig)
{
    static std::map<int, std::pair<const char*, const char*>> INFO =
    {
        { SIGHUP, { "SIGHUP", "hang up" } },
        { SIGINT, { "SIGINT", "interrupt" } },
        { SIGQUIT, { "SIGQUIT", "quit" } },
        { SIGPIPE, { "SIGPIPE", "broken pipe" } },
        { SIGTERM, { "SIGTERM", "termination" } },
        { SIGUSR1, { "SIGUSR1", "user-defined signal 1" } },
        { SIGUSR2, { "SIGUSR2", "user-defined signal 2" } },
        { SIGCHLD, { "SIGCHLD", "child process exited" } },
        { SIGVTALRM, { "SIGVTALRM", "virtual alarm clock" } },
        { SIGILL, { "SIGILL", "illegal instruction" } },
        { SIGABRT, { "SIGABRT", "abort" } },
        { SIGFPE, { "SIGFPE", "floating point exception" } },
        { SIGSEGV, { "SIGSEGV", "invalid memory reference" } },
        { SIGTRAP, { "SIGTRAP", "trace/breakpoint trap" } },
        { SIGBUS, { "SIGBUS", "bus error" } },
        { SIGSYS, { "SIGSYS", "bad argument to system call" } },
        { SIGXCPU, { "SIGXCPU", "CPU time limit exceeded" } },
        { SIGXFSZ, { "SIGXFSZ", "file size limit exceeded" } }
    };
    auto found = INFO.find(sig);
    if (found == INFO.end())
        return std::make_pair("unknown", "");
    return found->second;
}

void log_signal(int sig)
{
    auto inf = get_info(sig);
    CHUCHO_WARN(chucho::logger::get("chuchod.signal_handler"), "Signal received: " << inf.second << " (" <<
        inf.first << ") (" << sig << ")");
}

void abort_handler(int sig)
{
    log_signal(sig);
    raise(sig);
}

void sigwait_main(std::function<void()> hup_handler)
{
    sigset_t signals;
    sigemptyset(&signals);
    for (int i : ASYNC)
        sigaddset(&signals, i);
    int rec;
    while (true)
    {
        sigwait(&signals, &rec);
        log_signal(rec);
        if (std::find(DEADLY_ASYNC.begin(), DEADLY_ASYNC.end(), rec) != DEADLY_ASYNC.end())
            chucho::server::is_shut_down = true;
        else if (rec == SIGHUP && hup_handler)
            hup_handler();
        sigaddset(&signals, rec);
    }
}

}

namespace chucho
{

namespace server
{

namespace signal_handler
{

void install(std::function<void()> hup_handler)
{
    sigset_t blocked;
    sigemptyset(&blocked);
    for (int i : ASYNC)
        sigaddset(&blocked, i);
    pthread_sigmask(SIG_BLOCK, &blocked, 0);
    std::thread wait(sigwait_main, hup_handler);
    wait.detach();
    struct sigaction action;
    std::memset(&action, 0, sizeof(action));
    sigemptyset(&action.sa_mask);
    action.sa_handler = abort_handler;
    action.sa_flags = SA_RESETHAND;
    for (int i : SYNC)
    {
        if (sigaction(i, &action, 0) != 0)
        {
            auto inf = get_info(i);
            CHUCHO_ERROR(chucho::logger::get("chuchod.signal_handler"), "Could not install signal handler for " <<
                inf.first << " (" << i << ") (" << inf.second << ")");
        }
    }
}

}

}

}
