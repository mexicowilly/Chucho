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

#if !defined(CHUCHO_POSTGRES_WRITER_HPP__)
#define CHUCHO_POSTGRES_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/database_writer.hpp>
#include <libpq-fe.h>

namespace chucho
{

/**
 * @class postgres_writer postgres_writer.hpp chucho/postgres_writer.hpp 
 * A writer that sends log events to an PostgreSQL database. The
 * table to which Chucho will write must exist in the database. 
 * Please refer to the file sql/postgres.sql, which is a script 
 * that can be exected to create the table, for information 
 * about what columns must exist. 
 * 
 * @ingroup writers database
 */
class postgres_writer : public database_writer
{
public:
    /**
     * @name Constructor and Destructor
     * @{
     */
    /**
     * Construct a PostgreSQL writer.
     * 
     * @param fmt the formatter
     * @param uri the <a 
 *       href="http://www.postgresql.org/docs/9.3/static/libpq-connect.html#LIBPQ-CONNSTRING">connection
 *       URI</a>
     */
    postgres_writer(std::shared_ptr<formatter> fmt, const std::string& uri);
    /**
     * Destroy a PostgreSQL writer.
     */
    ~postgres_writer();
    /** @} */

    /**
     * Return the URI of the database connection.
     * 
     * @return the URI
     */
    const std::string& get_uri() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::string uri_;
    PGconn* connection_;
};

inline const std::string& postgres_writer::get_uri() const
{
    return uri_;
}

}

#endif
