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
#include <chucho/event.hpp>
#include <chucho/c_logger.hpp>
#include <chucho/c_level.hpp>
#include <chucho/optional.hpp>
#include <chucho/text_util.hpp>
#include <cstdarg>

extern "C"
{

chucho_rc chucho_log(const chucho_level* lvl,
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
            auto msg = chucho::text_util::format(format, args);
            va_end(args);
            lgr->logger_->write(chucho::event(lgr->logger_,
                                              lvl->level_,
                                              msg,
                                              file,
                                              line,
                                              func));
        }
    }
    catch (...)
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_log_mark(const chucho_level* lvl,
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
            auto msg = chucho::text_util::format(format, args);
            va_end(args);
            lgr->logger_->write(chucho::event(lgr->logger_,
                                              lvl->level_,
                                              msg,
                                              file,
                                              line,
                                              func,
                                              mark));
        }
    }
    catch (...)
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}