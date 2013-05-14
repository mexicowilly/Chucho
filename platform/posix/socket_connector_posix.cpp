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

#include <chucho/socket_connector.hpp>
#include <chucho/socket_exception.hpp>
#include <cerrno>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>

// Some operating systems allow you to suppress SIGPIPE on a socket
// with SO_NOSIGPIPE. Others allow you to suppress it when sending
// data with MSG_NOSIGNAL. Others allow neither, so you have to do it
// manually.
#if !defined(MSG_NOSIGNAL) && !defined(SO_NOSIGPIPE)
#define CHUCHO_MANUAL_SUPPRESS_SIGPIPE
#endif

// We'll use this flag, if the OS provides it.
#if !defined(MSG_NOSIGNAL)
#define MSG_NOSIGNAL 0
#endif

namespace chucho
{

socket_connector::socket_connector(const std::string& host, std::uint16_t port)
{
    std::string ptext(std::to_string(port));
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* addrs;
    int rc = getaddrinfo(host.c_str(), ptext.c_str(), &hints, &addrs);
    if (rc != 0)
        throw exception("Could not resolve address of " + host + ": " + gai_strerror(rc));
    struct sentry
    {
        sentry(addrinfo* addrs) : addrs_(addrs) { }
        ~sentry() { freeaddrinfo(addrs_); }
        addrinfo* addrs_;
    } sntry(addrs);
    do
    {
        socket_ = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
        if (socket_ != -1)
        {
            #if defined(SO_NOSIGPIPE)
            int yes = 1;
            setsockopt(socket_, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof(yes));
            #endif
            if (connect(socket_, addrs->ai_addr, addrs->ai_addrlen) == 0)
                return;
        }
        addrs = addrs->ai_next;
    } while (addrs != nullptr);
    if (socket_  != -1)
        close(socket_);
    int err = errno;
    std::string msg;
    if (socket_ == -1)
        msg = "Could not create socket";
    else
        msg = "Could not connect to " + host;
    throw socket_exception(msg, err);
}

socket_connector::~socket_connector()
{
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
    }
}

void socket_connector::write(const std::uint8_t* buf, std::size_t length)
{
    if (socket_ != -1)
    {
        std::size_t remaining = length;
        while (remaining > 0)
        {
            #if defined(CHUCHO_MANUAL_SUPPRESS_SIGPIPE)
            sigset_t blocked;
            sigset_t pending;
            sigset_t pipe_only;
            sigemptyset(&pipe_only);
            sigaddset(&pipe_only, SIGPIPE);
            sigemptyset(&pending);
            sigpending(&pending);
            if (!sigismember(&pending, SIGPIPE))
            {
                sigemptyset(&blocked);
                pthread_sigmask(SIG_BLOCK, &pipe_only, &blocked);
            }
            #endif
            ssize_t sent = send(socket_, buf + length - remaining, remaining, MSG_NOSIGNAL);
            #if defined(CHUCHO_MANUAL_SUPPRESS_SIGPIPE)
            if (!sigismember(&pending, SIGPIPE))
            {
                sigemptyset(&pending);
                sigpending(&pending);
                if (sigismember(&pending, SIGPIPE))
                {
                    int sig;
                    sigwait(&pipe_only, &sig);
                }
                if (!sigismember(&blocked, SIGPIPE))
                    pthread_sigmask(SIG_UNBLOCK, &pipe_only, nullptr);
            }
            #endif
            if (sent == -1)
                throw socket_exception("Error sending data over socket", errno);
            remaining -= sent;
        }
    }
}

}
