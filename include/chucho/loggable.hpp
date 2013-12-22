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

#if !defined(CHUCHO_LOGGABLE_HPP__)
#define CHUCHO_LOGGABLE_HPP__

#include <chucho/logger.hpp>
#include <type_traits>

namespace chucho
{

template <typename type>
class loggable
{
protected:
    loggable();
    loggable(const std::string& name);

    std::shared_ptr<logger> get_logger() const;
    void rename_logger(const std::type_info& new_type);
    void rename_logger(const std::string& name);

private:
    std::shared_ptr<logger> logger_;
};

template <typename type>
inline loggable<type>::loggable()
    : logger_(logger::get(logger::type_to_logger_name(typeid(type))))
{
}

template <typename type>
inline loggable<type>::loggable(const std::string& name)
    : logger_(logger::get(name))
{
}

template <typename type>
inline std::shared_ptr<logger> loggable<type>::get_logger() const
{
    return logger_;
}

template <typename type>
inline void loggable<type>::rename_logger(const std::type_info& new_type)
{
    logger_ = logger::get(logger::type_to_logger_name(new_type));
}

template <typename type>
inline void loggable<type>::rename_logger(const std::string& name)
{
    logger_ = logger::get(name);
}

}

#endif
