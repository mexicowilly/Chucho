/*
 * Copyright 2013-2015 Will Mason
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

#include <chucho/rolling_file_writer.hpp>
#include <stdexcept>

namespace chucho
{

// TODO: Support fowarded constructors when Microsoft supports it
rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         std::shared_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : file_writer(fmt, file_writer::on_start::APPEND, true),
      roller_(roller),
      trigger_(trigger)
{
    init();
    std::string fn = roller_->get_active_file_name();
    if (fn.empty())
        throw std::invalid_argument("The file_roller does not provide an initial file name, so a rolling_file_writer constructor without file name cannot be used");
    open(fn);
}

// TODO: Support fowarded constructors when Microsoft supports it
rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         const std::string& file_name,
                                         std::shared_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : file_writer(fmt, file_name, file_writer::on_start::APPEND, true),
      roller_(roller),
      trigger_(trigger)
{
    init();
}

rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         const std::string& file_name,
                                         on_start start,
                                         bool flush,
                                         std::shared_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : file_writer(fmt, file_name, start, flush),
      roller_(roller),
      trigger_(trigger)
{
    init();
}

rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         on_start start,
                                         bool flush,
                                         std::shared_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : file_writer(fmt, start, flush),
      roller_(roller),
      trigger_(trigger)
{
    init();
    std::string fn = roller_->get_active_file_name();
    if (fn.empty())
        throw std::invalid_argument("The file_roller does not provide an iniital file name, so a rolling_file_writer constructor without file name cannot be used");
    open(fn);
}

void rolling_file_writer::init()
{
    if (!roller_)
        throw std::invalid_argument("The file_roller cannot be an unintialized std::shared_ptr");
    set_status_origin("rolling_file_writer");
    if (!trigger_)
        trigger_ = std::dynamic_pointer_cast<file_roll_trigger>(roller_);
    if (!trigger_)
        throw std::invalid_argument("The rolling_file_writer has no file_roll_trigger");
    roller_->set_file_writer(*this);
}

void rolling_file_writer::write_impl(const event& evt)
{
    if (trigger_->is_triggered(get_file_name(), evt))
    {
        close();
        roller_->roll();
        open(roller_->get_active_file_name());
    }
    file_writer::write_impl(evt);
}

}
