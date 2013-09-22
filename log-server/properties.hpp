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

#if !defined(CHUCHO_PROPERTIES_HPP__)
#define CHUCHO_PROPERTIES_HPP__

#include <string>
#include <cstdint>
#include <ostream>

namespace chucho
{

namespace server
{

class properties
{
public:
    properties();

    bool handle_config_value(const std::string& key, const std::string& value);

    bool console_mode() const;
    std::uint16_t port() const;
    bool is_service() const;
    std::size_t vassal_count() const;

private:
    std::size_t vassal_count_;
    std::uint16_t port_;
    bool console_mode_;
    bool is_service_;
};

std::ostream& operator<< (std::ostream& stream, properties& props);

inline bool properties::console_mode() const
{
    return console_mode_;
}

inline bool properties::is_service() const
{
    return is_service_;
}

inline std::uint16_t properties::port() const
{
    return port_;
}

inline std::size_t properties::vassal_count() const
{
    return vassal_count_;
}

}

}

#endif
