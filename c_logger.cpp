/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/logger.h>
#include <chucho/logger.hpp>
#include <chucho/c_logger.hpp>

extern "C"
{

chucho_logger_t* chucho_get_logger(const char* const name)
{
    auto result = new chucho_logger_t();
    result->logger = chucho::logger::get(name);
    return result;
}

void chucho_release_logger(chucho_logger_t* lgr)
{
    delete lgr;
}

int chucho_logger_permits(const chucho_logger_t* lgr, chucho_level_t lvl)
{
    return lgr->logger->permits(chucho::c_to_level(lvl));
}

void chucho_logger_set_level(chucho_logger_t* lgr, chucho_level_t lvl)
{
    lgr->logger->set_level(chucho::c_to_level(lvl));
}

}
