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

#include <chucho/json_formatter_factory.hpp>
#include <chucho/serialization_formatter_memento.hpp>
#include <chucho/json_formatter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

json_formatter_factory::json_formatter_factory()
{
    set_status_origin("json_formatter_factory");
}

std::unique_ptr<configurable> json_formatter_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto sfm = dynamic_cast<serialization_formatter_memento*>(mnto.get());
    assert(sfm != nullptr);
    if (!sfm->get_excluded_fields().empty() && !sfm->get_included_fields().empty())
        throw exception("[json_formatter_factory] Either excluded_fields or included_fields may be set, not both");
    json_formatter::style style = sfm->get_style() ? *sfm->get_style() : json_formatter::DEFAULT_STYLE;
    json_formatter::time_zone tz = sfm->get_time_zone() ? *sfm->get_time_zone() : json_formatter::DEFAULT_TIME_ZONE;
    std::string t_fmt = sfm->get_time_format().empty() ? json_formatter::DEFAULT_TIME_FORMAT : sfm->get_time_format();
    std::unique_ptr<json_formatter> cnf;
    if (sfm->get_excluded_fields().empty() && sfm->get_included_fields().empty())
    {
        cnf = std::make_unique<json_formatter>(style, tz, t_fmt);
    }
    else if (sfm->get_excluded_fields().empty())
    {
        cnf = std::make_unique<json_formatter>(json_formatter::field_disposition::INCLUDED,
                                               sfm->get_included_fields(),
                                               style,
                                               tz,
                                               t_fmt);
    }
    else
    {
        cnf = std::make_unique<json_formatter>(json_formatter::field_disposition::EXCLUDED,
                                               sfm->get_excluded_fields(),
                                               style,
                                               tz,
                                               t_fmt);
    }
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> json_formatter_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<serialization_formatter_memento>(cfg);
    return std::move(mnto);
}

}
