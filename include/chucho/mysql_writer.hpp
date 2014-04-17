/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/database_writer.hpp>
#include <chucho/async_writer.hpp>

namespace chucho
{

/**
 * @class oracle_writer oracle_writer.hpp chucho/oracle_writer.hpp 
 * A writer that sends log events to an Oracle database. The 
 * table to which Chucho will write must exist in the database. 
 * Please refer to the file sql/oracle.sql, which is a script 
 * that can be exected to create the table, for information 
 * about what columns must exist. 
 * 
 * @ingroup writers
 */
class mysql_writer : public database_writer
{
public:
    static std::uint16_t DEFAULT_PORT;
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a MySQL writer.
     * 
     * @param fmt the formatter
     * @param user the user name for the database
     * @param password the password of the user for the database
     * @param database the database name, which is a TNS name, such 
     *                 as myoracleserver.com/dbname
     */
    mysql_writer(std::shared_ptr<formatter> fmt,
                 const std::string& host,
                 const std::string& user,
                 const std::string& password,
                 const std::string& database,
                 std::uint16_t port = DEFAULT_PORT,
                 std::size_t capacity = async_writer::DEFAULT_QUEUE_CAPACITY,
                 std::shared_ptr<level> discard_threshold = level::INFO_(),
                 bool flush_on_destruct = true);
    //@}

    async_writer& get_async_writer() const;
    const std::string& get_database() const;
    const std::string& get_host() const;
    const std::string& get_password() const;
    const std::string& get_user() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::unique_ptr<async_writer> async_;
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;
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

inline const std::string& mysql_writer::get_user() const
{
    return user_;
}

}

#endif
