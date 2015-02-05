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

#if !defined(CHUCHO_CANCELLABLE_HPP__)
#define CHUCHO_CANCELLABLE_HPP__

#include "scrollable.hpp"

namespace chucho
{

namespace config_tool
{

class cancellable : public scrollable
{
public:
    cancellable(const std::string& title,
                unsigned x,
                unsigned y,
                std::size_t width,
                std::size_t height);
    cancellable(unsigned x,
                unsigned y,
                std::size_t width,
                std::size_t height);

    bool cancelled() const;

protected:
    virtual exit_status unknown(chtype ch) override;

private:
    bool cancelled_;
};

inline bool cancellable::cancelled() const
{
    return cancelled_;
}

}

}

#endif
