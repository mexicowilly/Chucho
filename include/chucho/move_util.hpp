/*
 * Copyright 2013-2020 Will Mason
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

#ifndef CHUCHO_MOVE_UTIL_HPP_
#define CHUCHO_MOVE_UTIL_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <memory>

namespace chucho
{

template <typename cast_to_type, typename cast_from_type>
std::unique_ptr<cast_to_type> dynamic_move(std::unique_ptr<cast_from_type>&& p)
{
    std::unique_ptr<cast_to_type> result;
    auto conv = dynamic_cast<cast_to_type*>(p.get());
    if (conv != nullptr)
    {
        p.release();
        result.reset(conv);
    }
    return std::move(result);
};

}

#endif
