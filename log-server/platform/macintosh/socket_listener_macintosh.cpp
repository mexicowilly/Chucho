/*
 * Copyright 2013-2017 Will Mason
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

#include "socket_listener.hpp"
#include "eof_exception.hpp"
#include "is_shut_down.hpp"
#include <chucho/log.hpp>
#include <cstring>
#include <sys/event.h>
#include <sys/socket.h>
#include <launch.h>
#include <unistd.h>
#include <time.h>
#include <netdb.h>

namespace chucho
{

namespace server
{

struct socket_listener_handle
{
    socket_listener_handle() : kq_(-1) { }
    int kq_;
};

socket_listener::socket_listener(std::uint16_t port)
    : socket_(-1),
      logger_(chucho::logger::get("chuchod.socket_listener")),
      handle_(new socket_listener_handle)
{
    handle_->kq_ = kqueue();
    if (handle_->kq_ == -1) 
        throw chucho::exception(std::string("Unable to create kernel queue: ") + std::strerror(errno));
    launch_data_t checkin = launch_data_new_string(LAUNCH_KEY_CHECKIN);
    if (checkin == nullptr) 
    {
        int err = errno;
        close(handle_->kq_);
        throw chucho::exception(std::string("Could not create launch data string: ") + std::strerror(err));
    }
    launch_data_t resp = launch_msg(checkin);
    if (resp == nullptr) 
    {
        int err = errno;
        launch_data_free(checkin);
        close(handle_->kq_);
        throw chucho::exception(std::string("Could not check in with launchd: ") + std::strerror(err));
    }
    launch_data_free(checkin);
    struct sentry
    {
        sentry(launch_data_t resp) : resp_(resp) { }
        ~sentry() { launch_data_free(resp_); }
        launch_data_t resp_;
    } s(resp);
    if (launch_data_get_type(resp) == LAUNCH_DATA_ERRNO) 
    {
        int err = launch_data_get_errno(resp);
        close(handle_->kq_);
        throw chucho::exception(std::string("Error on launchd check in: ") + std::strerror(err));
    }
    launch_data_t scks = launch_data_dict_lookup(resp, LAUNCH_JOBKEY_SOCKETS);
    if (scks == nullptr) 
    {
        int err = errno;
        close(handle_->kq_);
        throw chucho::exception(std::string("Could not look up job key sockets: ") + std::strerror(err));
    }
    launch_data_t fds = launch_data_dict_lookup(scks, "Listeners");
    if (fds == nullptr) 
    {
        int err = errno;
        close(handle_->kq_);
        throw chucho::exception(std::string("Could not look up job key sockets: ") + std::strerror(err));
    }
    struct kevent kev;
    for (int i = 0; i < launch_data_array_get_count(fds); i++) 
    {
        launch_data_t fd = launch_data_array_get_index(fds, i);
        EV_SET(&kev, launch_data_get_fd(fd), EVFILT_READ, EV_ADD, 0, 0, nullptr);
        if (kevent(handle_->kq_, &kev, 1, nullptr, 0, nullptr) == -1)
        {
            int err = errno;
            close(handle_->kq_);
            throw chucho::exception(std::string("Could not add kernel event: ") + std::strerror(err));
        }
    }
}

socket_listener::~socket_listener()
{
    // This can throw when the listener is reset during SIGHUP (Really? How?)
    try
    {
        if (handle_->kq_ != -1) 
            close(handle_->kq_);
    }
    catch (exception& e)
    {
        CHUCHO_ERROR(logger_, "Error shutting down the listener: " << e.what());
    }
}

std::shared_ptr<socket_reader> socket_listener::accept()
{
    struct timespec wait;
    wait.tv_sec = 0;
    wait.tv_nsec = 500000000;
    struct kevent kev;
    int num;
    struct sockaddr_storage addr;
    do
    {
        num = kevent(handle_->kq_, nullptr, 0, &kev, 1, &wait);
        if (is_shut_down) 
            throw shutdown_exception();
        if (num > 0)
        {
            socklen_t length = sizeof(addr);
            int sock = ::accept(kev.ident, reinterpret_cast<struct sockaddr*>(&addr), &length);
            if (sock == -1)
            {
                int err = errno;
                if (err == EBADF)
                    throw eof_exception();
                throw exception(std::string("Unable to accept a connection") + std::strerror(err));
            }
            char host_name[NI_MAXHOST + 1];
            if (getnameinfo(reinterpret_cast<struct sockaddr*>(&addr), length,
                            host_name, NI_MAXHOST,
                            nullptr, 0,
                            0) != 0)
            {
                host_name[0] = 0;
            }
            std::string full_host(host_name);
            if (getnameinfo(reinterpret_cast<struct sockaddr*>(&addr), length,
                            host_name, NI_MAXHOST,
                            nullptr, 0,
                            NI_NOFQDN) != 0)
            {
                host_name[0] = 0;
            }
            return std::make_shared<socket_reader>(sock, host_name, full_host);
        }
    } while (num == 0);
    int err = errno;
    CHUCHO_ERROR(logger_, "Waiting for kernel events has been halted: " << std::strerror(err));
    throw eof_exception();
}

}

}
