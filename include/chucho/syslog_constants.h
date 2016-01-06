/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_SYSLOG_CONSTANTS_H__)
#define CHUCHO_SYSLOG_CONSTANTS_H__

/**
 * @file
 * Constants used in writing to syslog. 
 * @ingroup c_syslog
 */

#if defined(__cplusplus)
extern "C"
{
#endif

extern unsigned CHUCHO_DEFAULT_SYSLOG_PORT; /**< The default syslog port, which is 514. */

/**
 * The syslog facility.
 */
typedef enum
{
    CHUCHO_SYSLOG_FACILITY_KERN     = 0 * 8, /**< Kernel messages */
    CHUCHO_SYSLOG_FACILITY_USER     = 1 * 8, /**< User-level messages */
    CHUCHO_SYSLOG_FACILITY_MAIL     = 2 * 8, /**< Mail system */
    CHUCHO_SYSLOG_FACILITY_DAEMON   = 3 * 8, /**< System daemons */
    CHUCHO_SYSLOG_FACILITY_AUTH     = 4 * 8, /**< Security/authorization messages */
    CHUCHO_SYSLOG_FACILITY_SYSLOG   = 5 * 8, /**< Message generated interally by syslogd */
    CHUCHO_SYSLOG_FACILITY_LPR      = 6 * 8, /**< Line printer subsystem */
    CHUCHO_SYSLOG_FACILITY_NEWS     = 7 * 8, /**< Network news subsystem */
    CHUCHO_SYSLOG_FACILITY_UUCP     = 8 * 8, /**< UUCP subsystem */
    CHUCHO_SYSLOG_FACILITY_CRON     = 9 * 8, /**< Clock/scheduling daemon */
    CHUCHO_SYSLOG_FACILITY_AUTHPRIV = 10 * 8, /**< Security/authorization messages */
    CHUCHO_SYSLOG_FACILITY_FTP      = 11 * 8, /**< FTP daemon */
    CHUCHO_SYSLOG_FACILITY_LOCAL0   = 16 * 8, /**< Local use 0 */
    CHUCHO_SYSLOG_FACILITY_LOCAL1   = 17 * 8, /**< Local use 1 */
    CHUCHO_SYSLOG_FACILITY_LOCAL2   = 18 * 8, /**< Local use 2 */
    CHUCHO_SYSLOG_FACILITY_LOCAL3   = 19 * 8, /**< Local use 3 */
    CHUCHO_SYSLOG_FACILITY_LOCAL4   = 20 * 8, /**< Local use 4 */
    CHUCHO_SYSLOG_FACILITY_LOCAL5   = 21 * 8, /**< Local use 5 */
    CHUCHO_SYSLOG_FACILITY_LOCAL6   = 22 * 8, /**< Local use 6 */
    CHUCHO_SYSLOG_FACILITY_LOCAL7   = 23 * 8 /**< Local use 7 */
} chucho_syslog_facility;

/**
 * The syslog severity.
 */
typedef enum
{
    CHUCHO_SYSLOG_SEVERITY_EMERGENCY, /**< The system is unusable */
    CHUCHO_SYSLOG_SEVERITY_ALERT, /**< Action must be taken immediately */
    CHUCHO_SYSLOG_SEVERITY_CRITICAL, /**< Critical conditions */
    CHUCHO_SYSLOG_SEVERITY_ERROR, /**< Error conditions */
    CHUCHO_SYSLOG_SEVERITY_WARNING, /**< Warning conditions */
    CHUCHO_SYSLOG_SEVERITY_NOTICE, /**< Normal but significant conditions */
    CHUCHO_SYSLOG_SEVERITY_INFORMATIONAL, /**< Information messages */
    CHUCHO_SYSLOG_SEVERITY_DEBUG_ /**< Debug-level messages. This constant has a trailing underscore so that it will not clash with a possible macro named DEBUG. */
} chucho_syslog_severity;

#if defined(__cplusplus)
}
#endif

#endif
