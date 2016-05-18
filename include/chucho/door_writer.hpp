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

#if !defined(CHUCHO_DOOR_WRITER_HPP__)
#define CHUCHO_DOOR_WRITER_HPP__

#include <chucho/file_writer.hpp>

namespace chucho
{

class CHUCHO_EXPORT door_writer : public file_writer
{
public:
    door_writer(std::shared_ptr<formatter> fmt,
                const std::string& name);

protected:
    virtual void flush() override;
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT std::unique_ptr<char> create_door_event(const event& evt) const;
};

}

#endif