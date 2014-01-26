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

#include <chucho/bzip2_file_compressor_factory.hpp>
#include <chucho/file_compressor_memento.hpp>

namespace chucho
{

file_compressor_factory::file_compressor_factory()
{
    set_status_origin("file_compressor_factory");
}

std::shared_ptr<memento> file_compressor_factory::create_memento(const configurator& cfg)
{
    auto mnto = std::make_shared<file_compressor_memento>(cfg);
    return mnto;
}

}
