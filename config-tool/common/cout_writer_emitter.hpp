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

#if !defined(CHUCHO_COUT_WRITER_EMITTER_HPP__)
#define CHUCHO_COUT_WRITER_EMITTER_HPP__

#include "writer_emitter.hpp"

namespace chucho
{

namespace config_tool
{

class cout_writer_emitter : public writer_emitter
{
public:
    cout_writer_emitter(const properties& props,
                        std::shared_ptr<formatter_emitter> fmt,
                        const std::vector<std::shared_ptr<filter_emitter>>& flts);

    virtual void emit(std::ostream& stream, std::size_t shifts) override;
};

}

}

#endif
