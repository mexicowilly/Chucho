/*
 * Copyright 2013-2021 Will Mason
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
#include <chucho/writeable_filter.hpp>
#include <assert.h>

namespace chucho
{

void writer_factory::set_filters(configurable& cnf, writer_memento& mnto)
{
    auto wrt = dynamic_cast<writer*>(&cnf);
    assert(wrt != nullptr);
    for (auto& f : mnto.get_filters())
    {
        auto wf = dynamic_cast<writeable_filter*>(f.get());
        if (wf != nullptr)
            wf->set_writer(*wrt);
        wrt->add_filter(std::move(f));
    }
}

}
