/*
 * Copyright 2013-2016 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this zlib except in compliance with the License.
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

#include <chucho/zlib_compressor_memento.hpp>

namespace chucho
{

zlib_compressor_memento::zlib_compressor_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("zlib_compressor_memento");
    cfg.get_security_policy().set_integer("zlib_compressor::compression_level", 1, 9);
    cfg.get_security_policy().set_text("zlib_compressor::compression_level(text)", 1);
    set_handler("compression_level", [this] (const std::string& lvl) { compression_level_ = validate("zlib_compressor::compression_level", std::stoi(validate("zlib_compressor::compression_level(text)", lvl))); });
}

}


