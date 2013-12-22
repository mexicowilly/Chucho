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

#include <chucho/socket_connector.hpp>
#include <chucho/socket_exception.hpp>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

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

namespace
{

#if defined(CHUCHO_MANUAL_SUPPRESS_SIGPIPE)

class sigpipe_suppressor
{
public:
    sigpipe_suppressor();
    ~sigpipe_suppressor();

private:
    sigset_t blocked_;
    sigset_t pending_;
    sigset_t pipe_only_;
};

sigpipe_suppressor::sigpipe_suppressor()
{
    sigemptyset(&blocked_);
    sigemptyset(&pipe_only_);
    sigaddset(&pipe_only_, SIGPIPE);
    sigemptyset(&pending_);
    sigpending(&pending_);
    if (!sigismember(&pending_, SIGPIPE))
        pthread_sigmask(SIG_BLOCK, &pipe_only_, &blocked_);
}

sigpipe_suppressor::~sigpipe_suppressor()
{
    if (!sigismember(&pending_, SIGPIPE))
    {
        sigemptyset(&pending_);
        sigpending(&pending_);
        if (sigismember(&pending_, SIGPIPE))
        {
            int sig;
            sigwait(&pipe_only_, &sig);
        }
        if (!sigismember(&blocked_, SIGPIPE))
            pthread_sigmask(SIG_UNBLOCK, &pipe_only_, nullptr);
    }
}

#else

class sigpipe_suppressor
{
};

#endif


}

namespace chucho
{

socket_connector::socket_connector(const std::string& host, std::uint16_t port)
    : socket_(-1),
      host_(host),
      port_(port),
      is_blocking_mode_(true)
{
    set_status_origin("socket_connector");
    try
    {
        open_socket();
    }
    catch (std::exception&)
    {
        report_warning("Unable to open socket", std::current_exception());
    }
}

socket_connector::~socket_connector()
{
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
    }
}

bool socket_connector::can_write()
{
    bool result = false;
    if (socket_ == -1)
    {
        try
        {
            open_socket();
        }
        catch (std::exception&)
        {
            report_warning("Unable to open socket", std::current_exception());
            return result;
        }
    }
    struct pollfd pfd;
    pfd.fd = socket_;
    pfd.events = POLLOUT;
    pfd.revents = 0;
    int num = poll(&pfd, 1, 0);
    if (num > 0)
    {
        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL))
        {
            report_info("The connection to " + host_ + ":" + std::to_string(port_) + " was lost. Reconnecting...");
            open_socket();
        }
        else
        {
            if (!is_blocking_mode_)
            {
                int flags = fcntl(socket_, F_GETFL);
                fcntl(socket_, F_SETFL, (flags & ~O_NONBLOCK));
                is_blocking_mode_ = true;
                report_info("The connection to " + host_ + ":" + std::to_string(port_) + " was established");
            }
            result = true;
        }
    }
    return result;
}

void socket_connector::open_socket()
{
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
        socket_ = -1;
    }
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* addrs;
    int rc = getaddrinfo(host_.c_str(), std::to_string(port_).c_str(), &hints, &addrs);
    if (rc != 0)
        throw exception("Could not resolve address of " + host_ + ": " + gai_strerror(rc));
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
            int flags = fcntl(socket_, F_GETFL);
            fcntl(socket_, F_SETFL, (flags | O_NONBLOCK));
            if (connect(socket_, addrs->ai_addr, addrs->ai_addrlen) == 0 ||
                errno == EINPROGRESS)
            {
                is_blocking_mode_ = false;
                return;
            }
            close(socket_);
        }
        addrs = addrs->ai_next;
    } while (addrs != nullptr);
    if (socket_ != -1)
        close(socket_);
    int err = errno;
    std::string msg;
    if (socket_ == -1)
        msg = "Could not create socket";
    else
        msg = "Could not connect to " + host_;
    throw socket_exception(msg, err);
}

void socket_connector::write(const std::uint8_t* buf, std::size_t length)
{
    if (socket_ != -1)
    {
        sigpipe_suppressor sup;
        std::size_t remaining = length;
        while (remaining > 0)
        {
            ssize_t sent = send(socket_, buf + length - remaining, remaining, MSG_NOSIGNAL);
            if (sent == -1)
                throw socket_exception("Error sending data over socket", errno);
            remaining -= sent;
        }
    }
}

}
