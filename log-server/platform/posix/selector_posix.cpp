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
#include <vector>
#include <poll.h>

namespace chucho
{

namespace server
{

void selector::main()
{
    std::vector<struct pollfd> fds;
    while (true)
    {
        std::unique_lock<std::mutex> lock(guard_);
        condition_.wait(lock, [this] () { return stop_ || !readers_.empty(); });
        if (stop_)
            return;
        fds.resize(readers_.size());
        std::size_t i = 0;
        for (const std::pair<int, std::shared_ptr<socket_reader>>& r : readers_)
        {
            fds[i].fd = r.first;
            fds[i].events = POLLIN;
            fds[i].revents = 0;
            i++;
        }
        lock.unlock();
        CHUCHO_DEBUG(logger_, "Polling " << fds.size() << " sockets");
        int num = poll(&fds[0], fds.size(), 500);
        if (num > 0)
        {
            for (const struct pollfd& fd : fds)
            {
                if (fd.revents != 0)
                {
                    guard_.lock();
                    auto found = readers_.find(fd.fd);
                    if (found == readers_.end())
                    {
                        CHUCHO_WARN(logger_, "An unknown socket_reader was selected for input, socket " << fd.fd);
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
