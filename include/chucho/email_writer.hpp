/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_EMAIL_WRITER_HPP__)
#define CHUCHO_EMAIL_WRITER_HPP__

#include <chucho/writer.hpp>
#include <chucho/email_trigger.hpp>
#include <chucho/optional.hpp>
#include <queue>

namespace chucho
{

class curl;

/**
 * @class email_writer email_writer.hpp chucho/email_writer.hpp
 * A writer that sends log events via email. The writer
 * holds events in a cache until its @ref email_trigger
 * indicates that it is time to send them. By default the
 * cache holds 256 log events. When it is time to send email
 * the log events are formatted into the email message body
 * oldest to newest, one line at a time.
 * 
 * Chucho uses [libcurl](http://curl.haxx.se/libcurl/) for
 * implementation of the SMTP protocol. When it comes to
 * SSL-based implementations, the libcurl that is present
 * at run time must have SSL support. If it doesn't, then
 * neither the @ref connection_type::SSL nor the @ref
 * connection_type::STARTTLS connection types will be
 * supported. Support for SSL can be detected before an
 * @ref email_writer is created with the static method
 * @ref get_ssl_supported.
 * 
 * @ingroup writers email
 */
class CHUCHO_EXPORT email_writer : public writer
{
public:
    /**
     * The type of connection that will be attempted with
     * the email server.
     */
    enum class connection_type
    {
        /**
         * No encryption will be used.
         */
        CLEAR,
        /**
         * A [STARTTLS](https://en.wikipedia.org/wiki/STARTTLS)
         * connection will be used.
         * 
         * @note This connection type is only supported if @ref
         * get_ssl_supported returns true.
         */
        STARTTLS,
        /**
         * A pure SSL connection will be used.
         * 
         * @note This connection type is only supported if @ref
         * get_ssl_supported returns true.
         */
        SSL
    };

    /**
     * The default email port, which is 25.
     */
    static const std::uint16_t DEFAULT_PORT;
    /**
     * The default buffer capacity, which is 256.
     */
    static const std::size_t DEFAULT_BUFFER_CAPACITY;

    /**
     * Return whether SSL-based connections are supported.
     * 
     * This method only returns true if the
     * [libcurl](http://curl.haxx.se/libcurl/) library used
     * at run time was built with SSL support.
     * 
     * @return true if SSL is supported
     */
    static bool get_ssl_supported();

    /**
     * @name Constructors and Destructor
     * @{
     */
    /**
     * Construct an email writer.
     *
     * @param name the name of the writer
     * @param fmt the formatter
     * @param host the SMTP host
     * @param connect the connection type
     * @param to the list of addressees
     * @param from the email address of the sender
     * @param subject a @ref pattern_formatter pattern that will
     *        be formatted with the last @ref event that triggers
     *        the sending of email
     * @param trigger the email trigger
     * @param port the port to which to connect
     * @param buffer_capacity the capacity of the buffer that holds
     *        events until the @ref email_trigger is triggered
     */
    email_writer(const std::string& name,
                 std::unique_ptr<formatter>&& fmt,
                 const std::string& host,
                 connection_type connect,
                 const std::vector<std::string>& to,
                 const std::string& from,
                 const std::string& subject,
                 std::unique_ptr<email_trigger>&& trigger,
                 std::uint16_t port = DEFAULT_PORT,
                 std::size_t buffer_capacity = DEFAULT_BUFFER_CAPACITY);
    /**
     * Construct an email writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param host the SMTP host
     * @param connect the connection type
     * @param to the list of addressees
     * @param from the email address of the sender
     * @param subject a @ref pattern_formatter pattern that will
     *        be formatted with the last @ref event that triggers
     *        the sending of email
     * @param trigger the email trigger
     * @param user the user name for user/password authentication
     * @param password the password for user/password authentication
     * @param port the port to which to connect
     * @param buffer_capacity the capacity of the buffer that holds
     *        events until the @ref email_trigger is triggered
     */
    email_writer(const std::string& name,
                 std::unique_ptr<formatter>&& fmt,
                 const std::string& host,
                 connection_type connect,
                 const std::vector<std::string>& to,
                 const std::string& from,
                 const std::string& subject,
                 std::unique_ptr<email_trigger>&& trigger,
                 const std::string& user,
                 const std::string& password,
                 std::uint16_t port = DEFAULT_PORT,
                 std::size_t buffer_capacity = DEFAULT_BUFFER_CAPACITY);
    /**
     * Destroy an email writer.
     */
    virtual ~email_writer();
    /**
     * @}
     */

    /**
     * Return the capacity of the buffer that holds events until
     * the @ref email_trigger is triggered.
     * 
     * @return the capacity of the buffer
     */
    std::size_t get_buffer_capacity() const;
    /**
     * Return the current size of the buffer that holds events until
     * the @ref email_trigger is triggered.
     * 
     * @return the size of the buffer
     */
    std::size_t get_buffer_size() const;
    /**
     * Return the connection type for sending email.
     * 
     * @return the type
     */
    connection_type get_connection_type() const;
    /**
     * Return the email address of the sender.
     * 
     * @return the sender
     */
    const std::string& get_from() const;
    /**
     * Return the host name of the SMTP server.
     * 
     * @return the host name
     */
    const std::string& get_host() const;
    /**
     * Return the password used for user/password
     * authentication, if there is one.
     * 
     * @return the password
     */
    const optional<std::string>& get_password() const;
    /**
     * Return the port to which to connect on the SMTP
     * host.
     * 
     * @return the port
     */
    std::uint16_t get_port() const;
    /**
     * Return the @ref pattern_formatter pattern that will
     * be formatted with the last event that triggers
     * the @ref email_trigger.
     * 
     * @return the subject
     */
    const std::string& get_subject() const;
    /**
     * Return the list of addressees to which the emails will
     * be sent.
     * 
     * @return the addressees
     */
    const std::vector<std::string>& get_to() const;
    /**
     * Return the email trigger that tells this email writer
     * when email should be sent.
     * 
     * @return the trigger
     */
    email_trigger& get_trigger() const;
    /**
     * Return the user used for user/password
     * authentication, if there is one.
     * 
     * @return the user
     */
    const optional<std::string>& get_user() const;
    /**
     * Return whether this email writer reports its
     * activity verbosely. By default, this is true
     * if Chucho was built with debug information
     * enabled.
     * 
     * @return true if verbose reporting is on
     */
    bool get_verbose() const;
    /**
     * Set whether this email writer reports its activity
     * verbosely. By default, this is true if Chucho
     * was built with debug information enabled.
     * 
     * @param state whether to report verbosely
     */
    void set_verbose(bool state);

protected:
    virtual void write_impl(const event& evt) override;

private:
    class CHUCHO_NO_EXPORT fixed_size_queue
    {
    public:
        fixed_size_queue(std::size_t max);

        std::size_t capacity() const;
        bool empty() const;
        const event& front() const;
        void pop();
        void push(const event& evt);
        std::size_t size() const;

    private:
        std::queue<event> q_;
        std::size_t max_;
    };

    CHUCHO_NO_EXPORT std::string format_date() const;
    CHUCHO_NO_EXPORT std::string format_message(const event& evt);
    CHUCHO_NO_EXPORT void init();

    fixed_size_queue evts_;
    std::unique_ptr<curl> curl_;
    std::unique_ptr<email_trigger> trigger_;
    std::string from_;
    std::vector<std::string> to_;
    std::string host_;
    std::uint16_t port_;
    std::string subject_;
    optional<std::string> user_;
    optional<std::string> password_;
    connection_type connection_type_;
};

inline std::size_t email_writer::get_buffer_capacity() const
{
    return evts_.capacity();
}

inline std::size_t email_writer::get_buffer_size() const
{
    return evts_.size();
}

inline email_writer::connection_type email_writer::get_connection_type() const
{
    return connection_type_;
}

inline const std::string& email_writer::get_from() const
{
    return from_;
}

inline const std::string& email_writer::get_host() const
{
    return host_;
}

inline const optional<std::string>& email_writer::get_password() const
{
    return password_;
}

inline std::uint16_t email_writer::get_port() const
{
    return port_;
}

inline const std::vector<std::string>& email_writer::get_to() const
{
    return to_;
}

inline const std::string& email_writer::get_subject() const
{
    return subject_;
}

inline email_trigger& email_writer::get_trigger() const
{
    return *trigger_;
}

inline const optional<std::string>& email_writer::get_user() const
{
    return user_;
}

inline std::size_t email_writer::fixed_size_queue::capacity() const
{
    return max_;
}

inline bool email_writer::fixed_size_queue::empty() const
{
    return q_.empty();
}

inline const event& email_writer::fixed_size_queue::front() const
{
    return q_.front();
}

inline void email_writer::fixed_size_queue::pop()
{
    q_.pop();
}

inline std::size_t email_writer::fixed_size_queue::size() const
{
    return q_.size();
}

}

#endif
