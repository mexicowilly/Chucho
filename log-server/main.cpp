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
#include "signal_handler.hpp"

using namespace chucho::server;

int main()
{
    properties props;
    log::configure(props);
    signal_handler::install();
    suzerain lord;
    lord.run(props);
    CHUCHO_INFO(chucho::logger::get("chuchod"), "chuchod is shutting down");
    return 0;
}
