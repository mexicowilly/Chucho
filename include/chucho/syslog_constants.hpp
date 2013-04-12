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

#if !defined(CHUCHO_SYSLOG_CONSTANTS_HPP__)
#define CHUCHO_SYSLOG_CONSTANTS_HPP__

#include <cstdint>

namespace chucho
{

namespace syslog
{

const std::uint16_t DEFAULT_PORT = 514;

enum class facility
{
    KERN        = 0 * 8,
    USER        = 1 * 8,
    MAIL        = 2 * 8,
    DAEMON      = 3 * 8,
    AUTH        = 4 * 8,
    SYSLOG      = 5 * 8,
    LPR         = 6 * 8,
    NEWS        = 7 * 8,
    UUCP        = 8 * 8,
    CRON        = 9 * 8,
    AUTHPRIV    = 10 * 8,
    FTP         = 11 * 8,
    LOCAL0      = 16 * 8,
    LOCAL1      = 17 * 8,
    LOCAL2      = 18 * 8,
    LOCAL3      = 19 * 8,
    LOCAL4      = 20 * 8,
    LOCAL5      = 21 * 8,
    LOCAL6      = 22 * 8,
    LOCAL7      = 23 * 8
};

enum class severity
{
    EMERGENCY,
    ALERT,
    CRITICAL,
    ERROR,
    WARNING,
    NOTICE,
    INFORMATIONAL,
    DEBUG
};

}

}

#endif
