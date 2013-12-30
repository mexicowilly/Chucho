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

#include <chucho/log.h>
#include <chucho/error.h>
#include <chucho/event.hpp>
#include <chucho/c_logger.hpp>
#include <chucho/c_level.hpp>
#include <chucho/optional.hpp>
#include <cstdarg>
#include <cstdio>
#include <vector>

namespace
{

chucho::optional<std::string> format_message(const char* const fmt, va_list args)
{
    std::vector<char> chars(1024);
    int rc = std::vsnprintf(&chars[0], chars.size(), fmt, args);
    if (rc > chars.size() - 1) 
    {
        chars.resize(rc + 1);
        rc = std::vsnprintf(&chars[0], chars.size(), fmt, args);
        return chucho::optional<std::string>();
    }
    else if (rc < 0) 
    {
        return chucho::optional<std::string>();
    }
    return std::string(&chars[0]);
}

}

int chucho_log(const chucho_level* lvl,
               chucho_logger* lgr,
               const char* const file,
               int line,
               const char* const func,
               const char* const format, ...)
{
    try
    {
        if (lvl == nullptr || lgr == nullptr)
            return CHUCHO_NULL_POINTER; 
        if (lgr->logger_->permits(lvl->level_))
        {
            va_list args;
            va_start(args, format);
            auto msg = format_message(format, args);
            va_end(args);
            if (msg) 
            {
                lgr->logger_->write(chucho::event(lgr->logger_,
                                                  lvl->level_,
                                                  *msg,
                                                  file,
                                                  line,
                                                  func));
            }
            else
            {
                return CHUCHO_FORMAT_ERROR;
            }
        }
    }
    catch (std::bad_alloc&) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

int chucho_log_mark(const chucho_level* lvl,
                    chucho_logger* lgr,
                    const char* const file,
                    int line,
                    const char* const func,
                    const char* const mark,
                    const char* const format, ...)
{
    try
    {
        if (lvl == nullptr || lgr == nullptr || mark == nullptr)
            return CHUCHO_NULL_POINTER; 
        if (lgr->logger_->permits(lvl->level_))
        {
            va_list args;
            va_start(args, format);
            auto msg = format_message(format, args);
            va_end(args);
            if (msg) 
            {
                lgr->logger_->write(chucho::event(lgr->logger_,
                                                  lvl->level_,
                                                  *msg,
                                                  file,
                                                  line,
                                                  func,
                                                  mark));
            }
            else
            {
                return CHUCHO_FORMAT_ERROR;
            }
        }
    }
    catch (std::bad_alloc&) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}
