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

#if !defined(CHUCHO_EMAIL_WRITER_H__)
#define CHUCHO_EMAIL_WRITER_H__

/**
 * @file email_writer.h
 * Functions for creating and querying email writers.
 * @copydoc email_writer
 * 
 * @ingroup c_email c_writers
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/email_trigger.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @copydoc chucho::email_writer::connection_type
 */
typedef enum
{
    CHUCHO_EMAIL_CONNECTION_CLEAR,      /**< @copydoc chucho::email_writer::connection_type::CLEAR */
    CHUCHO_EMAIL_CONNECTION_SSL,        /**< @copydoc chucho::email_writer::connection_type::SSL */
    CHUCHO_EMAIL_CONNECTION_STARTTLS    /**< @copydoc chucho::email_writer::connection_type::STARTTLS */
} chucho_email_connection_type;

/**
 * @copydoc chucho::email_writer::DEFAULT_PORT
 */
extern unsigned CHUCHO_DEFAULT_EMAIL_PORT;
/**
 * @copydoc chucho::email_writer::DEFAULT_BUFFER_CAPACITY
 */
extern size_t CHUCHO_DEFAULT_EMAIL_BUFFER_CAPACITY;

/**
 * @name Creation
 * @{
 */
/**
 * Create an email writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @note Both user and password must either be NULL or not
 *       NULL.
 * 
 * @param[out] wrt the email writer to create
 * @param[in] fmt the formatter
 * @param[in] host the host name of the SMTP server
 * @param[in] connect the connection type
 * @param[in] to a null-terminated array of email recipients
 * @param[in] from the email address of the sender
 * @param[in] subject a pattern-formatter formatted pattern
 *            which is run against the event that triggers
 *            the sending of email
 * @param[in] trg the email trigger
 * @param[in] user the user name, if user/password authentication
 *            should be used. This can be NULL if no user is
 *            needed.
 * @param[in] password the password, if user/password authentication
 *            should be used. This can be NULL if no password is
 *            needed.
 * @param[in] port the port to which to connect
 * @param[in] buffer_capacity the capacity of the buffer used to store
 *            events while waiting for the email trigger to fire
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
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
/**
 * @}
 */

/**
 * Return the capacity of the buffer used to hold events until the email
 * trigger fires.
 * 
 * @param[in] wrt the email writer
 * @param[out] cap the capacity of the buffer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_buffer_capacity(const chucho_writer* wrt, size_t* cap);
/**
 * Return the number of events currently in the buffer used to
 * hold events until the email trigger fires.
 * 
 * @param[in] wrt the email writer
 * @param[out] sz the size of the buffer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_buffer_size(const chucho_writer* wrt, size_t* sz);
/**
 * Return the connection type.
 * 
 * @param[in] wrt the email writer
 * @param[out] tp the connection type
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_connection_type(const chucho_writer* wrt,
                                                        chucho_email_connection_type* tp);
/**
 * Return the address of the email sender.
 * 
 * @param[in] wrt the email writer
 * @param[out] frm the sender's address
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_from(const chucho_writer* wrt, const char** frm);
/**
 * Return the host name of the SMTP server for sending email.
 * 
 * @param[in] wrt the email writer
 * @param[out] hst the host name
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_host(const chucho_writer* wrt, const char** hst);
/**
 * Return the password used for user/password authentication.
 * 
 * @param[in] wrt the email writer
 * @param[out] pwd the password, which can be NULL if no password is required
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_password(const chucho_writer* wrt, const char** pwd);
/**
 * Return the pot to which to connect for sending email.
 * 
 * @param[in] wrt the email writer
 * @param[out] prt the port
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_port(const chucho_writer* wrt, unsigned* prt);
/**
 * Return the subject for sending email. The subject is a pattern-formatter
 * pattern that is run against the last event that triggers the sending of
 * email.
 * 
 * @param[in] wrt the email writer
 * @param[out] sub the subject
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_subject(const chucho_writer* wrt, const char** sub);
/**
 * Return the addresses to which email will be sent. The array rcpts must
 * be large enough to hold all the recipient addresses. The number of
 * actual addresses returned can be determined by pre-flighting with a
 * NULL array and allocating a large enough one using the value returned
 * in the num parameter.
 * 
 * @param[in] wrt the email writer
 * @param[out] rcpts the array to which to copy addresses. This must be
 *             large enough to hold all addresses, but it can be NULL
 *             for pre-flighting to determine the required size.
 * @param[in] sz the number of elements in the rcpts array
 * @param[out] num the number of addresses that actually exist in this
 *             email writer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_to(const chucho_writer* wrt,
                                           const char*** rcpts,
                                           size_t sz,
                                           size_t* num);
/**
 * Return the email trigger that causes email to be sent.
 * 
 * @post Ownership of the trg parameter is transferred to the caller, and
 *       trg must be released with the @ref chucho_release_email_trigger
 *       function.
 * 
 * @param[in] wrt the email writer
 * @param[out] trg the email trigger
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_trigger(const chucho_writer* wrt, chucho_email_trigger** trg);
/**
 * Return the user used for user/password authentication.
 * 
 * @param[in] wrt the email writer
 * @param[out] usr the user name, which can be NULL if no user is required
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_user(const chucho_writer* wrt, const char** usr);
/**
 * Return whether the given email writer logs its activity verbosely. By
 * default, this is non-zero only if the Chucho library was built with debug
 * symbols turned on.
 * 
 * @param[in] wrt the email writer
 * @param[out] vb non-zero if the email writer is verbose, zero otherwise
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_get_verbose(const chucho_writer* wrt, int* vb);
/**
 * Set whether the given email writer should report its activity verbosely.
 * 
 * @param[in] wrt the email writer
 * @param[in] vb non-zero if verbose activity is desired, zero otherwise
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ewrt_set_verbose(const chucho_writer* wrt, int vb);

#if defined(__cplusplus)
}
#endif

#endif
