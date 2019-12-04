/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/cache_filter_memento.hpp>
#include <chucho/cache_filter.hpp>
#include <chucho/text_util.hpp>

namespace chucho
{

cache_filter_memento::cache_filter_memento(configurator &cfg)
    : filter_memento(cfg)
{
    set_status_origin("cache_filter_memento");
    set_default_name(typeid(cache_filter));
    cfg.get_security_policy().set_integer("cache_filter::chunk_size", 1024, 100 * 1024 * 1024);
    cfg.get_security_policy().set_integer("cache_filter::max_chunks", 2, 1000000);
    cfg.get_security_policy().set_text("cache_filter::chunk_size(text)", 9);
    cfg.get_security_policy().set_text("cache_filter::max_chunks(text)", 7);
    set_handler("threshold", [this](const std::string &name) { threshold_ = level::from_text(validate("cache_filter::level", name)); });
    set_handler("chunk_size", [this] (const std::string& s) { chunk_size_ = static_cast<std::size_t>(validate("cache_filter::chunk_size",
        text_util::parse_byte_size(validate("cache_filter::chunk_size(text)", s)))); });
    set_handler("max_chunks", [this] (const std::string& cap) { max_chunks_ = validate("cache_filter::max_chunks", std::stoul(validate("cache_filter::max_chunks(text)", cap))); });
}

}
