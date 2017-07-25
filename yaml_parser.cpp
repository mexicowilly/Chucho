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

#include <chucho/yaml_parser.hpp>

namespace
{

int istream_reader(void* raw,
                   unsigned char* buf,
                   std::size_t size,
                   std::size_t* size_read)
{
    // You don't own this pointer
    std::istream* stream = reinterpret_cast<std::istream*>(raw);
    if (stream->eof())
    {
        *size_read = 0;
        return 1;
    }
    if (!stream->good())
        return 0;
    stream->read(reinterpret_cast<char*>(buf), size);
    *size_read = static_cast<std::size_t>(stream->gcount());
    return 1;
}

}

namespace chucho
{

yaml_parser::yaml_parser(std::istream& stream)
{
    yaml_parser_initialize(&parser_);
    yaml_parser_set_input(&parser_, istream_reader, &stream);
}

yaml_parser::~yaml_parser()
{
    yaml_parser_delete(&parser_);
}

}
