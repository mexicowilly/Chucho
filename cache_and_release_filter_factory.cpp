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

#include <chucho/cache_and_release_filter_factory.hpp>
#include <chucho/cache_and_release_filter_memento.hpp>
#include <chucho/cache_and_release_filter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

cache_and_release_filter_factory::cache_and_release_filter_factory()
{
    set_status_origin("cache_and_release_filter_factory");
}

std::unique_ptr<configurable> cache_and_release_filter_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto cfm = dynamic_cast<cache_and_release_filter_memento *>(mnto.get());
    assert(cfm != nullptr);
    if (cfm->get_name().empty())
        throw exception("cache_and_release_filter_factory: The name must be set");
    if (!cfm->get_chunk_size())
        throw exception("cache_and_release_filter_factory: The chunk size must be set");
    if (!cfm->get_max_chunks())
        throw exception("cache_and_release_filter_factory: The max chunks must be set");
    if (!cfm->get_cache_threshold())
        throw exception("cache_and_release_filter_factory: The cache threshold must be set");
    if (!cfm->get_release_threshold())
        throw exception("cache_and_release_filter_factory: The release threshold must be set");
    auto cnf = std::make_unique<cache_and_release_filter>(cfm->get_name(),
                                                          cfm->get_cache_threshold(),
                                                          cfm->get_release_threshold(),
                                                          *cfm->get_chunk_size(),
                                                          *cfm->get_max_chunks());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> cache_and_release_filter_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<cache_and_release_filter_memento>(cfg);
    return std::move(mnto);
}

}
