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

#if !defined(CHUCHO_YAML_PARSER_HPP_)
#define CHUCHO_YAML_PARSER_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/non_copyable.hpp>
#include <yaml.h>
#include <istream>

namespace chucho
{

class yaml_parser : non_copyable
{
public:
    yaml_parser(std::istream& stream);
    ~yaml_parser();

    operator yaml_parser_t* ();

    yaml_mark_t problem_mark() const;
    std::string problem_message() const;

private:
    yaml_parser_t parser_;
};

inline yaml_parser::operator yaml_parser_t* ()
{
    return &parser_;
}

inline yaml_mark_t yaml_parser::problem_mark() const
{
    return parser_.problem_mark;
}

inline std::string yaml_parser::problem_message() const
{
    return parser_.problem;
}

}

#endif
