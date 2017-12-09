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

#if !defined(CHUCHO_MYSQL_WRITER_HPP__)
#define CHUCHO_MYSQL_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/database_writer.hpp>
#include <chucho/async_writer.hpp>

namespace chucho
{

/**
 * @class mysql_writer mysql_writer.hpp chucho/mysql_writer.hpp 
 * A writer that sends log events to a MySQL database. The table
 * to which Chucho will write must exist in the database. Please
 * refer to the file sql/mysql.sql, which is a script that can 
 * be exected to create the table, for information about what 
 * columns must exist. 
 *  
 * In order for the MySQL client to work properly in a 
 * multi-threaded program, each thread has to be specifically 
 * initialized for MySQL before any MySQL calls are actually 
 * made. For this reason, Chucho uses an @ref async_writer under
 * the covers, which has the effect of channeling all MySQL 
 * writes through a single thread. This thread is guaranteed to 
 * be safe for MySQL calls. Therefore, the user does not have to 
 * prepare any threads before use with Chucho and mysql_writer. 
 * 
 * @ingroup writers database
 */
class CHUCHO_EXPORT mysql_writer : public database_writer
{
public:
    /**
     * The default MySQL port, which is 3306.
     */
    static const std::uint16_t DEFAULT_PORT;
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a MySQL writer.
     *
     * @param name the name of the writer
     * @param fmt the formatter
     * @param host the host on which MySQL is running
     * @param user the user name for the database
     * @param password the password of the user for the database
     * @param database the database name, which is a TNS name, such 
     *                 as myoracleserver.com/dbname
     * @param port the port to which to connect
     * @param capacity the capacity of the blocking queue in the 
     *                 @ref async_writer underlying the MySQL writer
     * @param discard_threshold the level at which to discard events 
     *                          when the queue capacity of the
     *                          underlying @ref async_writer has
     *                          reached 80% capacity
     * @param flush_on_destruct whether to flush pending events in 
     *                          the underlying @ref async_writer
     *                          when this writer is destroyed
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw exception if there is a problem connecting to the 
     *        MySQL database
     *  
     * @sa async_writer 
     */
    mysql_writer(const std::string& name,
                 std::unique_ptr<formatter>&& fmt,
                 const std::string& host,
                 const std::string& user,
                 const std::string& password,
                 const std::string& database,
                 std::uint16_t port = DEFAULT_PORT,
                 std::size_t capacity = async_writer::DEFAULT_QUEUE_CAPACITY,
                 std::shared_ptr<level> discard_threshold = level::INFO_(),
                 bool flush_on_destruct = true);
    //@}

    /**
     * Return the underlying @ref async_writer. The MySQL writer 
     * channels all writes through the underlying @ref async_writer. 
     * This is to ensure that the thread to which the events are 
     * written is prepared for interaction with the MySQL client. 
     * 
     * @return the @ref async_writer
     */
    async_writer& get_async_writer() const;
    /**
     * Return the name of the database.
     * 
     * @return the database name
     */
    const std::string& get_database() const;
    /**
     * Return the host on which MySQL is running.
     * 
     * @return the host name
     */
    const std::string& get_host() const;
    /**
     * Return the password used to connect to the database.
     * 
     * @return the password
     */
    const std::string& get_password() const;
    /**
     * Return the port used to connect to the database.
     * 
     * @return the port
     */
    std::uint16_t get_port() const;
    /**
     * Return the databaes user name.
     * 
     * @return the user name
     */
    const std::string& get_user() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::unique_ptr<async_writer> async_;
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;
    std::uint16_t port_;
};

inline async_writer& mysql_writer::get_async_writer() const
{
    return *async_;
}

inline const std::string& mysql_writer::get_database() const
{
    return database_;
}

inline const std::string& mysql_writer::get_host() const
{
    return host_;
}

inline const std::string& mysql_writer::get_password() const
{
    return password_;
}

inline std::uint16_t mysql_writer::get_port() const
{
    return port_;
}

inline const std::string& mysql_writer::get_user() const
{
    return user_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
