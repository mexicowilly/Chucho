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

/**
 * @addtogroup syslog
 */
//@{

/**
 * The default syslog port, which is 514.
 */
extern const std::uint16_t DEFAULT_PORT;

/**
 * All standard syslog facilities. Defined in the file
 * <chucho/syslog_constants.hpp>. 
 */
enum class facility
{
    KERN        = 0 * 8, /**< Kernel messages */
    USER        = 1 * 8, /**< User-level messages */
    MAIL        = 2 * 8, /**< Mail system */
    DAEMON      = 3 * 8, /**< System daemons */
    AUTH        = 4 * 8, /**< Security/authorization messages */
    SYSLOG      = 5 * 8, /**< Message generated interally by syslogd */
    LPR         = 6 * 8, /**< Line printer subsystem */
    NEWS        = 7 * 8, /**< Network news subsystem */
    UUCP        = 8 * 8, /**< UUCP subsystem */
    CRON        = 9 * 8, /**< Clock/scheduling daemon */
    AUTHPRIV    = 10 * 8, /**< Security/authorization messages */
    FTP         = 11 * 8, /**< FTP daemon */
    LOCAL0      = 16 * 8, /**< Local use 0 */
    LOCAL1      = 17 * 8, /**< Local use 1 */
    LOCAL2      = 18 * 8, /**< Local use 2 */
    LOCAL3      = 19 * 8, /**< Local use 3 */
    LOCAL4      = 20 * 8, /**< Local use 4 */
    LOCAL5      = 21 * 8, /**< Local use 5 */
    LOCAL6      = 22 * 8, /**< Local use 6 */
    LOCAL7      = 23 * 8 /**< Local use 7 */
};

/**
 * All standard syslog severities. Defined in the file 
 * <chucho/syslog_constants.hpp>.
 */
enum class severity
{
    EMERGENCY, /**< The system is unusable */
    ALERT, /**< Action must be taken immediately */
    CRITICAL, /**< Critical conditions */
    ERROR, /**< Error conditions */
    WARNING, /**< Warning conditions */
    NOTICE, /**< Normal but significant conditions */
    INFORMATIONAL, /**< Information messages */
    DEBUG_ /**< Debug-level messages. This constant has a trailing underscore so that it will not clash with a possible macro named DEBUG. */
};
//@}

}

}

#endif
