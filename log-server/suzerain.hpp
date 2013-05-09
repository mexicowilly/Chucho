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

#if !defined(CHUCHO_SUZERAIN_HPP__)
#define CHUCHO_SUZERAIN_HPP__

#include "socket_reader.hpp"
#include "properties.hpp"
#include <chucho/logger.hpp>

namespace chucho
{

namespace server
{

class suzerain
{
public:
    suzerain();
    suzerain(const suzerain&) = delete;

    suzerain& operator= (const suzerain&) = delete;

    void run(const properties& props);

private:
    void process_events(std::shared_ptr<socket_reader> reader);

    std::shared_ptr<chucho::logger> logger_;
};

}

}

#endif
