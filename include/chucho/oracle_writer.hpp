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

#if !defined(CHUCHO_ORACLE_WRITER_HPP__)
#define CHUCHO_ORACLE_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/database_writer.hpp>
#include <oci.h>

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
 * @ingroup writers database
 */
class CHUCHO_EXPORT oracle_writer : public database_writer
{
public:
    /**
     * @name Constructor and Destructor
     */
    //@{
    /**
     * Construct an Oracle writer.
     *
     * @param name the name of the writer
     * @param fmt the formatter
     * @param user the user name for the database
     * @param password the password of the user for the database
     * @param database the database name, which is a TNS name, such 
     *                 as myoracleserver.com/dbname
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw exception if there is a problem connecting to the 
     *        database
     */
    oracle_writer(const std::string& name,
                  std::unique_ptr<formatter>&& fmt,
                  const std::string& user,
                  const std::string& password,
                  const std::string& database);
    /**
     * Destroy an Oracle writer.
     */
    ~oracle_writer();
    //@}

    /**
     * Return the TNS name of the database.
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
    void finalize();
    void react(sword code, const std::string& msg, bool final);

    std::string user_;
    std::string password_;
    std::string database_;
    OCIEnv* env_;
    OCIError* err_;
    OCISvcCtx* ctx_;
    OCIStmt* insert_evt_;
    OCIBind* formatted_msg_;
    OCIBind* timestamp_;
    OCIBind* file_name_;
    OCIBind* line_number_;
    OCIBind* func_;
    OCIBind* logger_;
    OCIBind* level_;
    OCIBind* marker_;
    OCIBind* thread_name_;
    OCIBind* host_name_;
};

inline const std::string& oracle_writer::get_database() const
{
    return database_;
}

inline const std::string& oracle_writer::get_password() const
{
    return password_;
}

inline const std::string& oracle_writer::get_user() const
{
    return user_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
