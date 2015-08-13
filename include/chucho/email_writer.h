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

#if !defined(CHUCHO_EMAIL_WRITER_H__)
#define CHUCHO_EMAIL_WRITER_H__

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/email_trigger.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef enum
{
    CHUCHO_EMAIL_CONNECTION_CLEAR,
    CHUCHO_EMAIL_CONNECTION_SSL,
    CHUCHO_EMAIL_CONNECTION_STARTTLS
} chucho_email_connection_type;

extern unsigned CHUCHO_DEFAULT_EMAIL_PORT;
extern size_t CHUCHO_DEFAULT_EMAIL_BUFFER_CAPACITY;

CHUCHO_EXPORT chucho_rc chucho_create_email_writer(chucho_writer** wrt,
                                                   chucho_formatter* fmt,
                                                   const char* const host,
                                                   chucho_email_connection_type connect,
                                                   const char* const* to,
                                                   const char* const from,
                                                   const char* const subject,
                                                   chucho_email_trigger* trg,
                                                   const char* const user,
                                                   const char* const password,
                                                   unsigned port,
                                                   size_t buffer_capacity);

CHUCHO_EXPORT chucho_rc chucho_ewrt_get_buffer_capacity(const chucho_writer* wrt, size_t* cap);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_buffer_size(const chucho_writer* wrt, size_t* sz);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_connection_type(const chucho_writer* wrt,
                                                        chucho_email_connection_type* tp);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_from(const chucho_writer* wrt, const char** frm);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_host(const chucho_writer* wrt, const char** hst);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_password(const chucho_writer* wrt, const char** pwd);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_port(const chucho_writer* wrt, unsigned* prt);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_subject(const chucho_writer* wrt, const char** sub);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_to(const chucho_writer* wrt,
                                           const char*** rcpts,
                                           size_t sz,
                                           size_t* num);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_trigger(const chucho_writer* wrt, chucho_email_trigger** trg);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_user(const chucho_writer* wrt, const char** usr);
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_verbose(const chucho_writer* wrt, int* vb);
CHUCHO_EXPORT chucho_rc chucho_ewrt_set_verbose(const chucho_writer* wrt, int vb);

#if defined(__cplusplus)
}
#endif

#endif