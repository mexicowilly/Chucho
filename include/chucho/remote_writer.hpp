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

#if !defined(CHUCHO_REMOTE_WRITER_HPP__)
#define CHUCHO_REMOTE_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <cstdint>
#include <queue>

namespace chucho
{

class socket_connector;

/**
 * @class remote_writer remote_writer.hpp chucho/remote_writer.hpp
 * A writer that writes to a (possibly) remote chucho server. 
 * The chucho server running on the (possibly) remote host 
 * accepts any events written to this writer. The chucho 
 * server acts as a logging proxy: it must be configured with 
 * any loggers that appear in the events written to this logger. 
 * The logging settings for those loggers on the chucho server 
 * itself then serve as the settings used for events written to 
 * this remote_writer.
 *  
 * This writer will never block the writing thread while a 
 * connection to the chucho server is being made. However, after
 * the connection has been established, then the socket is 
 * placed in blocking mode for subsequent writes. This is so 
 * that a slow network connection can appropriately push back on 
 * the process writing over the network. 
 *  
 * When a connection is being established, or after a connection 
 * has been lost and must be re-established, then events are 
 * cached in a ring buffer. So, it is possible to lose events,
 * but by adjusting the size of the event buffer, the 
 * possibility of event loss may be minimized. 
 *  
 * This writer internally uses a yaml_formatter for 
 * serialization of the events. That is why no formatter can be 
 * set on this writer. The events are serialized and 
 * reconstituted intact on the server side. 
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT remote_writer : public writer
{
public:
    /**
     * The default port on which the chucho server is listening, 
     * which is 21012. 
     */
    static const std::uint16_t DEFAULT_PORT;
    /**
     * The default unsent cache size, which is 1000. Events are 
     * added to the unsent cache when the connection to the server 
     * is down. 
     */
    static const std::size_t DEFAULT_UNSENT_CACHE_MAX;

    /**
     * @name Constructors and destructor
     */
    //@{
    /**
     * Construct a remote_writer.
     *
     * @param name the name of this writer
     * @param host the host on which the chucho server is running
     * @param port the port on which the chucho server is listening
     * @param unsent_cache_max the maximum size of the unsent event 
     *                         cache
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the host name is empty
     * @throw std::invalid_argument if the port is zero
     */
    remote_writer(const std::string& name,
                  const std::string& host,
                  std::uint16_t port = DEFAULT_PORT,
                  std::size_t unsent_cache_max = DEFAULT_UNSENT_CACHE_MAX);
    /**
     * Destroy the remote_writer.
     */
    ~remote_writer();
    //@}

    /**
     * Return the host to which the remote_writer is meant to 
     * connect. 
     * 
     * @return the name of the host
     */
    const std::string& get_host() const;
    /**
     * Return the port number to which the remote_writer is meant to 
     * connect. 
     * 
     * @return the port number
     */
    std::uint16_t get_port() const;
    /**
     * Return the maximum size of the unsent cache. The unsent cache
     * is a ring buffer of events that is used to store events that 
     * arrive when there is no connection to the chucho server. 
     * 
     * @return the maximum size
     */
    std::size_t get_unsent_cache_max() const;
    /**
     * Return the current number of events in the unsent cache. The 
     * unsent cache is a ring buffer of events that is used to store 
     * events that arrive when there is no connection to the chucho 
     * server. 
     * 
     * @return the current size
     */
    std::size_t get_unsent_cache_size() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::unique_ptr<socket_connector> connector_;
    std::string host_;
    std::uint16_t port_;
    std::deque<event> unsent_events_;
    std::size_t unsent_cache_max_;
};

inline const std::string& remote_writer::get_host() const
{
    return host_;
}

inline std::uint16_t remote_writer::get_port() const
{
    return port_;
}

inline std::size_t remote_writer::get_unsent_cache_max() const
{
    return unsent_cache_max_;
}

inline std::size_t remote_writer::get_unsent_cache_size() const
{
    return unsent_events_.size();
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
