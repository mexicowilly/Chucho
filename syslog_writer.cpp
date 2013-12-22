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

#include <chucho/syslog_writer.hpp>
#include <chucho/calendar.hpp>

namespace chucho
{

syslog_writer::syslog_writer(std::shared_ptr<formatter> fmt,
                             syslog::facility fcl)
    : writer(fmt),
      facility_(fcl)
{
    set_status_origin("syslog_writer");
}

syslog_writer::syslog_writer(std::shared_ptr<formatter> fmt,
                             syslog::facility fcl,
                             const std::string& host,
                             std::uint16_t port)
    : writer(fmt),
      transport_(host, port),
      facility_(fcl),
      host_name_(host),
      port_(port)
{
    set_status_origin("syslog_writer");
}

void syslog_writer::write_impl(const event& evt)
{
    syslog::severity sev = evt.get_level()->get_syslog_severity();
    std::string msg = transport_.format(facility_,
                                        sev,
                                        evt.get_time(),
                                        formatter_->format(evt));
    transport_.send(facility_, sev, msg);
}

}
