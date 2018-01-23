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

#if !defined(CHUCHO_SQLITE_WRITER_HPP__)
#define CHUCHO_SQLITE_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/database_writer.hpp>
#include <sqlite3.h>

namespace chucho
{

/**
 * @class sqlite_writer sqlite_writer.hpp chucho/sqlite_writer.hpp 
 * A writer that writes log events to a SQLite database. Please 
 * refer to the sql/sqlite.sql for information about the name of 
 * the table involved and what columns exist. Unlike with the 
 * MySQL and Orcale databases, the table for SQLite does not 
 * have to exist before the database is used. It will be created 
 * automatically if it does not yet exist. 
 * 
 * @ingroup writers database
 */
class CHUCHO_EXPORT sqlite_writer : public database_writer
{
public:
    /**
     * @name Constructor and Destructor
     */
    //@{
    /**
     * Create a SQLite writer.
     *
     * @param name the name of the writer
     * @param fmt the formatter to use
     * @param file_name the name of the file that does or will hold 
     *                  the database
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::unique_ptr
     * @throw exception if the SQLite database cannot be opened
     */
    sqlite_writer(const std::string& name, std::unique_ptr<formatter>&& fmt, const std::string& file_name);
    /**
     * Destroy a SQLite writer.
     */
    ~sqlite_writer();
    //@}

    /**
     * Return the name of the file that holds the database.
     * 
     * @return the file name
     */
    const std::string& get_file_name() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::string file_name_;
    sqlite3* db_;
    sqlite3_stmt* insert_;
};

inline const std::string& sqlite_writer::get_file_name() const
{
    return file_name_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
