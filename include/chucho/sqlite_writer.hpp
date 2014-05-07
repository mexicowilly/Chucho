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

#if !defined(CHUCHO_SQLITE_WRITER_HPP__)
#define CHUCHO_SQLITE_WRITER_HPP__

#include <chucho/database_writer.hpp>
#include <sqlite3.h>

namespace chucho
{

class sqlite_writer : public database_writer
{
public:
    sqlite_writer(std::shared_ptr<formatter> fmt, const std::string& file_name);
    ~sqlite_writer();

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

#endif
