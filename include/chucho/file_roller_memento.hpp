/*
 * Copyright 2013 Will Mason
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

#if !defined(CHUCHO_FILE_ROLLER_MEMENTO_HPP__)
#define CHUCHO_FILE_ROLLER_MEMENTO_HPP__

#include <chucho/memento.hpp>
#include <chucho/file_compressor.hpp>

namespace chucho
{

class CHUCHO_EXPORT file_roller_memento : public memento
{
public:
    file_roller_memento(const configurator& cfg);

    std::shared_ptr<file_compressor> get_compressor() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    std::shared_ptr<file_compressor> compressor_;
};

inline std::shared_ptr<file_compressor> file_roller_memento::get_compressor() const
{
    return compressor_;
}

}

#endif
