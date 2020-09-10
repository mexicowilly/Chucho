/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_DATABASE_WRITER_HPP__)
#define CHUCHO_DATABASE_WRITER_HPP__

#include <chucho/writer.hpp>
#include <soci/soci.h>

namespace chucho
{

/**
 * @class database_writer database_writer.hpp chucho/database_writer.hpp
 * A writer that writes to a database. Supported databases include any
 * that the <a href="http://soci.sourceforge.net">SOCI</a> library
 * supports. As of this writing these are Oracle, MySQL, DB2, PostgreSQL,
 * sqlite3, ODBC and Firebird. You must have the back-end client libraries
 * installed, as well as a SOCI library in order to use this writer.
 *
 * No compile-time linkage is necessary with the back-end SOCI library or
 * the database client libraries, but at run-time, these libraries must
 * be available.
 *
 * The text of the connection parameter depends on which back-end is being
 * requested. But, generally, it takes the form:
 * @code
 * <back-end type>://<space-separated parameters>
 * @endcode
 * For PostgreSQL, this might look like:
 * @code
 * postgresql://host=lotsofdoggies.com port=5432 dbname=chucho user=scrumpy password=dumpy
 * @endcode
 * And, for Oracle, it might be something like:
 * @code
 * oracle://service=//host.stuff.com:1521/CHUCHO user=scrumpy password=dumpy
 * @endcode
 * Please consult the SOCI documentation for the details of the desired
 * back-end.
 *
 * @ingroup writers database
 */
class CHUCHO_EXPORT database_writer : public writer
{
public:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct a writer.
     *
     * The text of the connection parameter depends on which back-end is being
     * requested. But, generally, it takes the form:
     * @code
     * <back-end type>://<space-separated parameters>
     * @endcode
     * For PostgreSQL, this might look like:
     * @code
     * postgresql://host=lotsofdoggies.com port=5432 dbname=chucho user=scrumpy password=dumpy
     * @endcode
     * And, for Oracle, it might be something like:
     * @code
     * oracle://service=//host.stuff.com:1521/CHUCHO user=scrumpy password=dumpy
     * @endcode
     * Please consult the SOCI documentation for the details of the desired
     * back-end.
     *
     * @param name the name
     * @param fmt the formatter
     * @param connection the connection information
     */
    database_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    const std::string& connection);
    /**
     * @}
     */

protected:
    virtual void write_impl(const event& evt) override;

private:
    soci::session sql_;
    soci::statement stmt_;
    std::string formatted_message_;
    std::tm timestamp_;
    std::string file_name_;
    int line_number_;
    std::string function_name_;
    std::string logger_name_;
    std::string level_name_;
    std::string marker_;
    soci::indicator marker_ind_;
    std::string thread_;
    std::string host_name_;
    int process_id_;
};

}

#endif
