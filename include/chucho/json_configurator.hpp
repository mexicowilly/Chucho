/*
 * Copyright 2013-2017 Will Mason
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

#ifndef CHUCHO_JSON_CONFIGURATOR_HPP
#define CHUCHO_JSON_CONFIGURATOR_HPP

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/configurator.hpp>

namespace chucho
{

class CHUCHO_PRIV_EXPORT json_configurator : public configurator
{
public:
    json_configurator(const security_policy& sec_pol);

    virtual void configure(std::istream &in) override;
};

}

#endif
