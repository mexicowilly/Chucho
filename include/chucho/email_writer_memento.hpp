/*
 * Copyright 2013-2021 Will Mason
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

#if !defined(CHUCHO_EMAIL_WRITER_MEMENTO_HPP_)
#define CHUCHO_EMAIL_WRITER_MEMENTO_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/email_writer.hpp>

namespace chucho
{

class email_writer_memento : public writer_memento
{
public:
    email_writer_memento(configurator& cfg);

    const optional<std::size_t>& get_buffer_size() const;
    const optional<email_writer::connection_type>& get_connection_type() const;
    std::unique_ptr<email_trigger> get_email_trigger();
    const std::string& get_from() const;
    const std::string& get_host() const;
    const std::string& get_password() const;
    const optional<std::uint16_t>& get_port() const;
    const std::string& get_subject() const;
    const std::vector<std::string>& get_to() const;
    const std::string& get_user() const;
    const optional<bool>& get_verbose() const;
    virtual void handle(std::unique_ptr<configurable>&& cnf) override;

private:
    void set_connection_type(const std::string& connect);
    void set_to(const std::string& to);

    std::unique_ptr<email_trigger> trigger_;
    std::string from_;
    std::vector<std::string> to_;
    std::string host_;
    optional<std::uint16_t> port_;
    std::string subject_;
    std::string user_;
    std::string password_;
    optional<email_writer::connection_type> connection_type_;
    optional<std::size_t> buffer_size_;
    optional<bool> verbose_;
};

inline const optional<std::size_t>& email_writer_memento::get_buffer_size() const
{
    return buffer_size_;
}

inline const optional<email_writer::connection_type>& email_writer_memento::get_connection_type() const
{
    return connection_type_;
}

inline std::unique_ptr<email_trigger> email_writer_memento::get_email_trigger()
{
    return std::move(trigger_);
}

inline const std::string& email_writer_memento::get_from() const
{
    return from_;
}

inline const std::string& email_writer_memento::get_host() const
{
    return host_;
}

inline const std::string& email_writer_memento::get_password() const
{
    return password_;
}

inline const optional<std::uint16_t>& email_writer_memento::get_port() const
{
    return port_;
}

inline const std::string& email_writer_memento::get_subject() const
{
    return subject_;
}

inline const std::vector<std::string>& email_writer_memento::get_to() const
{
    return to_;
}

inline const std::string& email_writer_memento::get_user() const
{
    return user_;
}

inline const optional<bool>& email_writer_memento::get_verbose() const
{
    return verbose_;
}

}

#endif
