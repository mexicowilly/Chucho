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

#if !defined(CHUCHO_DUPLICATE_MESSAGE_FILTER_EMITTER_HPP__)
#define CHUCHO_DUPLICATE_MESSAGE_FILTER_EMITTER_HPP__

#include "filter_emitter.hpp"

namespace chucho
{

namespace config_tool
{

class duplicate_message_filter_emitter : public filter_emitter
{
public:
    duplicate_message_filter_emitter(const properties& props);

    virtual void emit(std::ostream& stream, std::size_t shifts) override;
};

}

}

#endif
