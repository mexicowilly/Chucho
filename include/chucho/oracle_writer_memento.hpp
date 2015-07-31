/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_ORACLE_WRITER_MEMENTO_HPP__)
#define CHUCHO_ORACLE_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>

namespace chucho
{

class oracle_writer_memento : public writer_memento
{
public:
    oracle_writer_memento(configurator& cfg);

    const std::string& get_database() const;
    const std::string& get_password() const;
    const std::string& get_user() const;

private:
    std::string user_;
    std::string password_;
    std::string database_;
};

inline const std::string& oracle_writer_memento::get_database() const
{
    return database_;
}

inline const std::string& oracle_writer_memento::get_password() const
{
    return password_;
}

inline const std::string& oracle_writer_memento::get_user() const
{
    return user_;
}

}

#endif
