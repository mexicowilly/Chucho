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

#include "selector.hpp"
#include "error_message.hpp"
#include "is_shut_down.hpp"
#define FD_SETSIZE 10000
#include <winsock2.h>

namespace chucho
{

namespace server
{

void selector::main()
{
    fd_set fds;
    timeval wait;
    std::vector<int> socks;
    while (true)
    {
        std::unique_lock<std::mutex> lock(guard_);
        while (!stop_ && readers_.empty())
            condition_.wait(lock);
        if (stop_)
            return;
        FD_ZERO(&fds);
        socks.clear();
        for (const std::pair<int, std::shared_ptr<socket_reader>>& r : readers_)
        {
            FD_SET(r.first, &fds);
            socks.push_back(r.first);
        }
        lock.unlock();
        wait.tv_sec = 0;
        wait.tv_usec = 500000;
        CHUCHO_DEBUG(logger_, "Polling " << socks.size() << " sockets");
        int rc = select(readers_.size(), &fds, nullptr, nullptr, &wait);
        if (rc == SOCKET_ERROR)
        {
            DWORD err = WSAGetLastError();
            if (err != WSAENOTSOCK)
            {
                CHUCHO_FATAL(logger_, "Unrecoverable error occurred in the socket library: " << windows::error_message(err));
                is_shut_down = true;
                return;
            }
            continue;
        }
        if (rc > 0)
        {
            for (int sock : socks)
            {
                if (FD_ISSET(sock, &fds))
                {
                    guard_.lock();
                    auto found = readers_.find(sock);
                    if (found == readers_.end())
                    {
                        CHUCHO_WARN(logger_, "An unknown socket_reader was selected for input, socket " << sock);
                    }
                    else
                    {
                        std::string peer(found->second->get_full_host());
                        was_selected_(found->second);
                        readers_.erase(found);
                        CHUCHO_DEBUG(logger_, "Connection for peer " << peer <<
                            " was selected for input and removed from the selector");
                    }
                    guard_.unlock();
                }
            }
        }
    }
}

}

}
