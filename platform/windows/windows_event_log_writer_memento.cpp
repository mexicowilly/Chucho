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

#include <chucho/windows_event_log_writer_memento.hpp>
#include <chucho/windows_event_log_writer.hpp>

namespace chucho
{

windows_event_log_writer_memento::windows_event_log_writer_memento(configurator& cfg, memento_key_set ks)
    : writer_memento(cfg)
{
    set_status_origin("windows_event_log_writer_memento");
    set_default_name(typeid(windows_event_log_writer));
    set_handler("log", [this] (const std::string& val) { log_ = val; });
    set_handler("source", [this] (const std::string& val) { source_ = val; });
    if (ks == memento_key_set::CHUCHO)
        set_handler("host", [this] (const std::string& val) { host_ = val; });
    else if (ks == memento_key_set::LOG4CPLUS)
        set_handler("server", [this] (const std::string& val) { host_ = val; });
}

}
