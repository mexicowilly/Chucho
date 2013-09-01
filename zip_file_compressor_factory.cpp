/*
 * Copyright 2013 Will Mason
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

#include <chucho/zip_file_compressor_factory.hpp>
#include <chucho/file_compressor_memento.hpp>
#include <chucho/zip_file_compressor.hpp>
#include <chucho/noop_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

zip_file_compressor_factory::zip_file_compressor_factory()
{
    set_status_origin("zip_file_compressor_factory");
}

std::shared_ptr<configurable> zip_file_compressor_factory::create_configurable(std::shared_ptr<memento> mnto)
{
#if defined(CHUCHO_HAVE_MINIZIP)
    auto fcm = std::dynamic_pointer_cast<file_compressor_memento>(mnto);
    assert(fcm);
    if (!fcm->get_min_index())
        throw exception("gzip_file_compressor_factory: The min_index field must be set");
    auto zfc = std::make_shared<zip_file_compressor>(*fcm->get_min_index());
#else
    report_warning("A zip_file_compressor was requested in the configuration, but this Chucho library was built without zip archive support.");
    auto zfc = std::make_shared<noop_file_compressor>();
#endif
    report_info("Created a " + demangle::get_demangled_name(typeid(*zfc)));
    return zfc;
}

}


