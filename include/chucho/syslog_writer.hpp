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

#if !defined(CHUCHO_SYSLOG_WRITER_HPP__)
#define CHUCHO_SYSLOG_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class syslog_transport_handle;

/**
 * @class syslog_writer syslog_writer.hpp chucho/syslog_writer.hpp
 * A @ref writer that writes to syslog. If a syslog_writer is 
 * given a host name, then it will use UDP to write to the 
 * syslog. Otherwise, if the platform supports it, then the 
 * writer will use the native syslog interface and allow the 
 * system to handle the IPC. 
 * 
 * @ingroup writers syslog
 */
class CHUCHO_EXPORT syslog_writer : public writer
{
public:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a syslog_writer. If this constructor is used, then 
     * the messages will be written to the local host using the 
     * syslog interface, which allows the system to manage the IPC. 
     * UDP over a socket is not used in this case. 
     *  
     * If the system does not support the syslog C interface, then 
     * UDP over a socket is used to connect to the local host. 
     * 
     * @param fmt the formatter
     * @param fcl the syslog facility
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw exception if the connection to the syslog cannot be 
     *        established
     */
    syslog_writer(std::shared_ptr<formatter> fmt,
                  syslog::facility fcl);
    /**
     * Construct a syslog_writer. When this constructor is used, 
     * then syslog messages are sent over a UDP socket. 
     * 
     * @param fmt the formatter
     * @param fcl the syslog facility
     * @param host the syslog host 
     * @param port the port on which syslogd is listening 
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw exception if the connection to the syslog cannot be 
     *        established
     */
    syslog_writer(std::shared_ptr<formatter> fmt,
                  syslog::facility fcl,
                  const std::string& host,
                  std::uint16_t port = syslog::DEFAULT_PORT);
    //@}

    /**
     * Return the syslog facility.
     * 
     * @return the facility
     */
    syslog::facility get_facility() const;
    /**
     * Return the host name. This text will be empty if no host name 
     * was provided in the contructor. 
     * 
     * @return the syslog host name
     */
    const std::string& get_host_name() const;
    /**
     * Return the host name. This optional will not be set if no 
     * host name was provided in the contructor. 
     * 
     * @return the syslog host name
     */
    const optional<std::uint16_t>& get_port() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    class CHUCHO_NO_EXPORT transport
    {
    public:
        transport();
        transport(const std::string& host, std::uint16_t port);
        ~transport();

        std::string format(syslog::facility fcl,
                           syslog::severity sev,
                           const event::time_type& when,
                           const std::string& message);
        void send(syslog::facility fcl,
                  syslog::severity sev,
                  const std::string& message);

    private:
        syslog_transport_handle* handle_;
    };

    CHUCHO_NO_EXPORT std::string remote_message(syslog::facility fcl,
                                                syslog::severity sev,
                                                const event::time_type& when,
                                                const std::string& local_message);

    transport transport_;
    syslog::facility facility_;
    std::string host_name_;
    optional<std::uint16_t> port_;
};

inline syslog::facility syslog_writer::get_facility() const
{
    return facility_;
}

inline const std::string& syslog_writer::get_host_name() const
{
    return host_name_;
}

inline const optional<std::uint16_t>& syslog_writer::get_port() const
{
    return port_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
