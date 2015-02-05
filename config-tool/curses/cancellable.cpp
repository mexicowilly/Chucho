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

#include "cancellable.hpp"
#include "chucho_keys.hpp"

namespace chucho
{

namespace config_tool
{

cancellable::cancellable(const std::string& title,
                         unsigned x,
                         unsigned y,
                         std::size_t width,
                         std::size_t height)
    : scrollable(title, x, y, width, height),
      cancelled_(false)
{
}

cancellable::cancellable(unsigned x,
                         unsigned y,
                         std::size_t width,
                         std::size_t height)
    : scrollable(x, y, width, height),
      cancelled_(false)
{
}

cancellable::exit_status cancellable::unknown(chtype ch)
{
    exit_status st = exit_status::should_not_exit;
    switch (ch)
    {
    case static_cast<chtype>(key::ESC):
        st = exit_status::should_exit;
        cancelled_ = true;
        break;
    }
    return st;
}

}

}
