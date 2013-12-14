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

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/prefix.hpp>
#include <chucho/optional.hpp>
#include <string>
#include <map>
#include <istream>
#include <utility>

namespace chucho
{

class CHUCHO_PRIV_EXPORT properties
{
public:
    typedef std::multimap<std::string, std::string>::const_iterator const_iterator;

    properties(std::istream& in);

    const_iterator begin() const;
    bool empty() const;
    const_iterator end() const;
    std::pair<const_iterator, const_iterator> get(const std::string& key) const;
    optional<std::string> get_one(const std::string& key) const;
    properties get_subset(const std::string prefix) const;
    std::size_t size() const;

private:
    CHUCHO_NO_EXPORT properties();

    std::multimap<std::string, std::string> props_;
};

inline properties::const_iterator properties::begin() const
{
    return props_.begin();
}

inline bool properties::empty() const
{
    return props_.empty();
}

inline properties::const_iterator properties::end() const
{
    return props_.end();
}

inline std::pair<properties::const_iterator, properties::const_iterator> properties::get(const std::string& key) const
{
    return props_.equal_range(key);
}

inline std::size_t properties::size() const
{
    return props_.size();
}

}

#include <chucho/suffix.hpp>

#endif
