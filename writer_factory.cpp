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

#include <chucho/writer_factory.hpp>
#include <chucho/writer.hpp>
#include <algorithm>

namespace chucho
{

void writer_factory::set_filters(configurable& cnf, const writer_memento& mnto)
{
    auto wrt = dynamic_cast<writer&>(cnf);
    std::for_each(mnto.get_filters().begin(),
                  mnto.get_filters().end(),
                  [&] (std::unique_ptr<filter>& f) { wrt.add_filter(std::move(f)); });
}

}
