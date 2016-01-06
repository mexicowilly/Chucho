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

#if !defined(CHUCHO_POSTGRES_WRITER_MEMENTO_HPP__)
#define CHUCHO_POSTGRES_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>

namespace chucho
{

class postgres_writer_memento : public writer_memento
{
public:
    postgres_writer_memento(configurator& cfg);

    const std::string& get_uri() const;

private:
    std::string uri_;
};

inline const std::string& postgres_writer_memento::get_uri() const
{
    return uri_;
}

}

#endif
