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

#include <chucho/rolling_file_writer.hpp>
#include <stdexcept>

namespace chucho
{

rolling_file_writer::rolling_file_writer(const std::string& name,
                                         std::unique_ptr<formatter>&& fmt,
                                         std::unique_ptr<file_roller>&& roller,
                                         std::unique_ptr<file_roll_trigger>&& trigger)
    : file_writer(name, std::move(fmt), file_writer::on_start::APPEND, true),
      roller_(std::move(roller)),
      trigger_(std::move(trigger))
{
    init();
    std::string fn = roller_->get_active_file_name();
    if (fn.empty())
        throw std::invalid_argument("The file_roller does not provide an initial file name, so a rolling_file_writer constructor without file name cannot be used");
    open(fn);
}

rolling_file_writer::rolling_file_writer(const std::string& name,
                                         std::unique_ptr<formatter>&& fmt,
                                         const std::string& file_name,
                                         std::unique_ptr<file_roller>&& roller,
                                         std::unique_ptr<file_roll_trigger>&& trigger)
    : file_writer(name, std::move(fmt), file_name, file_writer::on_start::APPEND, true),
      roller_(std::move(roller)),
      trigger_(std::move(trigger))
{
    init();
}

rolling_file_writer::rolling_file_writer(const std::string& name,
                                         std::unique_ptr<formatter>&& fmt,
                                         const std::string& file_name,
                                         on_start start,
                                         bool flush,
                                         std::unique_ptr<file_roller>&& roller,
                                         std::unique_ptr<file_roll_trigger>&& trigger)
    : file_writer(name, std::move(fmt), file_name, start, flush),
      roller_(std::move(roller)),
      trigger_(std::move(trigger))
{
    init();
}

rolling_file_writer::rolling_file_writer(const std::string& name,
                                         std::unique_ptr<formatter>&& fmt,
                                         on_start start,
                                         bool flush,
                                         std::unique_ptr<file_roller>&& roller,
                                         std::unique_ptr<file_roll_trigger>&& trigger)
    : file_writer(name, std::move(fmt), start, flush),
      roller_(std::move(roller)),
      trigger_(std::move(trigger))
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
        throw std::invalid_argument("The file_roller cannot be a unintialized");
    set_status_origin("rolling_file_writer");
    if (trigger_)
    {
        effective_trigger_ = trigger_.get();
    }
    else
    {
        effective_trigger_ = dynamic_cast<file_roll_trigger*>(roller_.get());
        if (!effective_trigger_)
            throw std::invalid_argument("The rolling_file_writer has no file_roll_trigger");
    }
    roller_->set_file_writer(*this);
}

void rolling_file_writer::write_impl(const event& evt)
{
    if (effective_trigger_->is_triggered(get_file_name(), evt))
    {
        close();
        roller_->roll();
        open(roller_->get_active_file_name());
    }
    file_writer::write_impl(evt);
}

}
