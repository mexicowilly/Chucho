/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_BZIP2_FILE_COMPRESSOR_FACTORY_HPP__)
#define CHUCHO_BZIP2_FILE_COMPRESSOR_FACTORY_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/file_compressor_factory.hpp>

namespace chucho
{

class bzip2_file_compressor_factory : public file_compressor_factory
{
public:
    bzip2_file_compressor_factory();

    virtual std::unique_ptr<configurable> create_configurable(std::unique_ptr<memento>& mnto) override;
};

}

#endif
