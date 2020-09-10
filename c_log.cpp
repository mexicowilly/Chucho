/*
 * Copyright 2013-2020 Will Mason
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
#include <chucho/text_util.hpp>
#include <chucho/logger.hpp>
#include <chucho/c_logger.hpp>
#include <cstdarg>

namespace chucho
{

std::shared_ptr<chucho::level> c_to_level(chucho_level_t lvl)
{
    switch (lvl)
    {
    case CHUCHO_TRACE:
        return chucho::level::TRACE_();
    case CHUCHO_DEBUG:
        return chucho::level::DEBUG_();
    case CHUCHO_INFO:
        return chucho::level::INFO_();
    case CHUCHO_WARN:
        return chucho::level::WARN_();
    case CHUCHO_ERROR:
        return chucho::level::ERROR_();
    case CHUCHO_FATAL:
        return chucho::level::FATAL_();
    }
    return chucho::level::INFO_();
}

}

namespace
{

void log_impl(chucho_level_t lvl,
              std::shared_ptr<chucho::logger> lgr,
              const char* const file,
              int line,
              const char* const func,
              const char* const format,
              va_list args)
{
    auto lv = chucho::c_to_level(lvl);
    if (lgr->permits(lv))
    {
        auto msg = chucho::text_util::format(format, args);
        lgr->write(chucho::event(lgr,
                                 lv,
                                 msg,
                                 file,
                                 line,
                                 func));
    }
}

void log_impl_mark(chucho_level_t lvl,
                   std::shared_ptr<chucho::logger> lgr,
                   const char* const file,
                   int line,
                   const char* const func,
                   const char* const mark,
                   const char* const format,
                   va_list args)
{
    auto lv = chucho::c_to_level(lvl);
    if (lgr->permits(lv))
    {
        auto msg = chucho::text_util::format(format, args);
        lgr->write(chucho::event(lgr,
                                 lv,
                                 msg,
                                 file,
                                 line,
                                 func,
                                 mark));
    }
}

}

extern "C"
{

void chucho_log(chucho_level_t lvl,
                const char* const lgr,
                const char* const file,
                int line,
                const char* const func,
                const char* const format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(lvl, chucho::logger::get(lgr), file, line, func, format, args);
    va_end(args);
}

void chucho_log_mark(chucho_level_t lvl,
                     const char* const lgr,
                     const char* const file,
                     int line,
                     const char* const func,
                     const char* const mark,
                     const char* const format, ...) {
    va_list args;
    va_start(args, format);
    log_impl_mark(lvl, chucho::logger::get(lgr), file, line, func, mark, format, args);
    va_end(args);
}

void chucho_log_logger(chucho_level_t lvl,
                       chucho_logger_t* lgr,
                       const char* const file,
                       int line,
                       const char* const func,
                       const char* const format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(lvl, lgr->logger, file, line, func, format, args);
    va_end(args);
}

void chucho_log_mark_logger(chucho_level_t lvl,
                            chucho_logger_t* lgr,
                            const char* const file,
                            int line,
                            const char* const func,
                            const char* const mark,
                            const char* const format, ...) {
    va_list args;
    va_start(args, format);
    log_impl_mark(lvl, lgr->logger, file, line, func, mark, format, args);
    va_end(args);
}

}
