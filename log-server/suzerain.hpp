/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_SUZERAIN_HPP__)
#define CHUCHO_SUZERAIN_HPP__

#include "socket_listener.hpp"
#include "properties.hpp"
#include "vassals.hpp"
#include "selector.hpp"
#include <chucho/logger.hpp>
#include <chucho/non_copyable.hpp>
#if defined(CHUCHOD_VELOCITY)
#include <chrono>
#include <cstdint>
#endif

namespace chucho
{

namespace server
{

class suzerain : non_copyable
{
public:
    suzerain(properties& props);
    ~suzerain();

    void run();

private:
    void process_events(std::shared_ptr<socket_reader> reader);
    void sighup_handler();
    void was_selected(std::shared_ptr<socket_reader> reader);

    std::shared_ptr<chucho::logger> logger_;
    std::unique_ptr<vassals> vassals_;
    std::unique_ptr<selector> selector_;
    std::unique_ptr<socket_listener> listener_;
    properties& props_;
    std::mutex listener_guard_;
    #if defined(CHUCHOD_VELOCITY)
    std::chrono::steady_clock::time_point first_seen_;
    std::chrono::steady_clock::time_point last_processed_;
    std::mutex velocity_guard_;
    std::uintmax_t total_event_count_;
    #endif
};

}

}

#endif
