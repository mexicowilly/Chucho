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

#include <chucho/memento_key_set.hpp>
#if defined(CHUCHO_CONFIG_FILE_CONFIG)
#include <chucho/config_file_configurator.hpp>
#endif

namespace chucho
{

memento_key_set get_memento_key_set(configurator& cfg)
{
    memento_key_set ks = memento_key_set::CHUCHO;

    #if defined(CHUCHO_CONFIG_FILE_CONFIG)

    auto ccfg = dynamic_cast<const config_file_configurator*>(&cfg);
    if (ccfg != nullptr)
        ks = ccfg->get_memento_key_set();

    #endif

    return ks;
}

}
