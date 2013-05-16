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

#include "log.hpp"
#include "suzerain.hpp"
#include "daemon.hpp"
#include <cstdlib>

using namespace chucho::server;

int main()
{
    properties props;
    log::configure(props);
    suzerain lord(props);
    std::shared_ptr<chucho::logger> lgr(chucho::logger::get("chuchod"));
    CHUCHO_INFO_STR(lgr, "chuchod has started");
    CHUCHO_INFO(lgr, props);
    if (props.console_mode())
        lord.run();
    else
        daemon::possess(std::bind(&suzerain::run, std::ref(lord)));
    CHUCHO_INFO(lgr, "chuchod is shutting down");
    return EXIT_SUCCESS;
}