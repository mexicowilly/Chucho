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

#if !defined(CHUCHO_NON_COPYABLE_HPP__)
#define CHUCHO_NON_COPYABLE_HPP__

#include <chucho/export.hpp>

namespace chucho
{

/**
 * @class non_copyable non_copyable.hpp chucho/non_copyable.hpp
 * An object that cannot be copied. This class is unfortunately 
 * necessary as long as some compilers refuse to support "= 
 * delete". Specifically, this class exists in order to support 
 * Visual Studio 2012. 
 *  
 * @ingroup miscellaneous 
 */
class CHUCHO_EXPORT non_copyable
{
public:
    non_copyable() { }

private:
    non_copyable(const non_copyable&);
    non_copyable& operator= (const non_copyable&);
};

}

#endif
