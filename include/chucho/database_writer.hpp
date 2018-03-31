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

#if !defined(CHUCHO_DATABASE_WRITER_HPP__)
#define CHUCHO_DATABASE_WRITER_HPP__

#include <chucho/writer.hpp>
#include <soci/soci.h>

namespace chucho
{

/**
 * @class database_writer database_writer.hpp chucho/database_writer.hpp
 * A writer that writes to a database.
 * 
 * @ingroup writers database
 */
class CHUCHO_EXPORT database_writer : public writer
{
public:
    database_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    const std::string& connection);

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
};

}

#endif
