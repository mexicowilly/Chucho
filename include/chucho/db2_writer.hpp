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

#if !defined(CHUCHO_DB2_WRITER_HPP__)
#define CHUCHO_DB2_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/database_writer.hpp>
#include <sqlcli1.h>

namespace chucho
{

/**
 * @class db2_writer db2_writer.hpp chucho/db2_writer.hpp 
 * 
 * @ingroup writers database
 */
class CHUCHO_EXPORT db2_writer : public database_writer
{
public:
    /**
     * @name Constructor and Destructor
     */
    //@{
    /**
     * Construct a DB2 writer.
     * 
     * @param fmt the formatter
     * @param database the database to which to connect
     * @param user the user name for the database
     * @param password the password of the user for the database
     * 
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw exception if there is a problem connecting to the 
     *        database
     */
    db2_writer(std::shared_ptr<formatter> fmt,
               const std::string& database,
               const std::string& user,
               const std::string& password);
    /**
     * Destroy a DB2 writer.
     */
    ~db2_writer();
    //@}

    /**
     * Return the name of the database.
     * 
     * @return the database name
     */
    const std::string& get_database() const;
    /**
     * Return the password.
     * 
     * @return the password
     */
    const std::string& get_password() const;
    /**
     * Return the Oracle user name.
     * 
     * @return the user
     */
    const std::string& get_user() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT void allocate_environment() const;
    CHUCHO_NO_EXPORT std::string get_error_message(SQLSMALLINT handle_type) const;

    std::string database_;
    std::string user_;
    std::string password_;
    SQLHDBC dbc_;
    SQLHSTMT stmt_;
};

inline const std::string& db2_writer::get_database() const
{
    return database_;
}

inline const std::string& db2_writer::get_password() const
{
    return password_;
}

inline const std::string& db2_writer::get_user() const
{
    return user_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif