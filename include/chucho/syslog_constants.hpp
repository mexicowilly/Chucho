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

#if !defined(CHUCHO_SYSLOG_CONSTANTS_HPP__)
#define CHUCHO_SYSLOG_CONSTANTS_HPP__

#include <chucho/export.hpp>
#include <chucho/syslog_constants.h>
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
CHUCHO_EXPORT extern const std::uint16_t DEFAULT_PORT;

/**
 * All standard syslog facilities. Defined in the file
 * <chucho/syslog_constants.hpp>. 
 */
enum class facility
{
    KERN        = ::CHUCHO_SYSLOG_FACILITY_KERN, /**< Kernel messages */
    USER        = ::CHUCHO_SYSLOG_FACILITY_USER, /**< User-level messages */
    MAIL        = ::CHUCHO_SYSLOG_FACILITY_MAIL, /**< Mail system */
    DAEMON      = ::CHUCHO_SYSLOG_FACILITY_DAEMON, /**< System daemons */
    AUTH        = ::CHUCHO_SYSLOG_FACILITY_AUTH, /**< Security/authorization messages */
    SYSLOG      = ::CHUCHO_SYSLOG_FACILITY_SYSLOG, /**< Message generated interally by syslogd */
    LPR         = ::CHUCHO_SYSLOG_FACILITY_LPR, /**< Line printer subsystem */
    NEWS        = ::CHUCHO_SYSLOG_FACILITY_NEWS, /**< Network news subsystem */
    UUCP        = ::CHUCHO_SYSLOG_FACILITY_UUCP, /**< UUCP subsystem */
    CRON        = ::CHUCHO_SYSLOG_FACILITY_CRON, /**< Clock/scheduling daemon */
    AUTHPRIV    = ::CHUCHO_SYSLOG_FACILITY_AUTHPRIV, /**< Security/authorization messages */
    FTP         = ::CHUCHO_SYSLOG_FACILITY_FTP, /**< FTP daemon */
    LOCAL0      = ::CHUCHO_SYSLOG_FACILITY_LOCAL0, /**< Local use 0 */
    LOCAL1      = ::CHUCHO_SYSLOG_FACILITY_LOCAL1, /**< Local use 1 */
    LOCAL2      = ::CHUCHO_SYSLOG_FACILITY_LOCAL2, /**< Local use 2 */
    LOCAL3      = ::CHUCHO_SYSLOG_FACILITY_LOCAL3, /**< Local use 3 */
    LOCAL4      = ::CHUCHO_SYSLOG_FACILITY_LOCAL4, /**< Local use 4 */
    LOCAL5      = ::CHUCHO_SYSLOG_FACILITY_LOCAL5, /**< Local use 5 */
    LOCAL6      = ::CHUCHO_SYSLOG_FACILITY_LOCAL6, /**< Local use 6 */
    LOCAL7      = ::CHUCHO_SYSLOG_FACILITY_LOCAL7 /**< Local use 7 */
};

/**
 * All standard syslog severities. Defined in the file 
 * <chucho/syslog_constants.hpp>.
 */
enum class severity
{
    EMERGENCY       = ::CHUCHO_SYSLOG_SEVERITY_EMERGENCY, /**< The system is unusable */
    ALERT           = ::CHUCHO_SYSLOG_SEVERITY_ALERT, /**< Action must be taken immediately */
    CRITICAL        = ::CHUCHO_SYSLOG_SEVERITY_CRITICAL, /**< Critical conditions */
    ERROR           = ::CHUCHO_SYSLOG_SEVERITY_ERROR, /**< Error conditions */
    WARNING         = ::CHUCHO_SYSLOG_SEVERITY_WARNING, /**< Warning conditions */
    NOTICE          = ::CHUCHO_SYSLOG_SEVERITY_NOTICE, /**< Normal but significant conditions */
    INFORMATIONAL   = ::CHUCHO_SYSLOG_SEVERITY_INFORMATIONAL, /**< Information messages */
    DEBUG_          = ::CHUCHO_SYSLOG_SEVERITY_DEBUG_/**< Debug-level messages. This constant has a trailing underscore so that it will not clash with a possible macro named DEBUG. */
};
//@}

}

}

#endif
