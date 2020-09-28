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

#include <chucho/door_writer.hpp>
#include <chucho/file_exception.hpp>
#include <chucho/logger.hpp>
#include <chucho/door_event.h>
#include <thread>
#include <cstring>
#include <sstream>
#include <door.h>

namespace chucho
{

door_writer::door_writer(const std::string& name,
                         std::unique_ptr<formatter>&& fmt,
                         const std::string& file_name)
    : file_writer(name, std::move(fmt), on_start::APPEND, true)
{
    set_status_origin("door_writer");
    set_allow_creation(false);
    open(file_name);
}

std::unique_ptr<char> door_writer::create_door_event(const event& evt) const
{
    std::string fm = formatter_->format(evt);
    auto fnlen = std::strlen(evt.get_file_name());
    auto funclen = std::strlen(evt.get_function_name());
    auto lvllen = std::strlen(evt.get_level()->get_name());
    std::ostringstream thr_stream;
    thr_stream << std::this_thread::get_id();
    auto thr = thr_stream.str();
    std::string mrk;
    if (evt.get_marker())
    {
        std::ostringstream mrk_stream;
        mrk_stream << *evt.get_marker();
        mrk = mrk_stream.str();
    }
    std::size_t sz = sizeof(chucho_door_event) + fm.length() + 1 + fnlen + 1 +
        funclen + 1 + evt.get_logger()->get_name().length() + 1 + lvllen + 1 +
        thr.length() + 1;
    if (evt.get_marker())
        sz += mrk.length() + 1;
    std::unique_ptr<char> result(new char[sz]);
    std::memset(result.get(), 0, sz);
    chucho_door_event* de = reinterpret_cast<chucho_door_event*>(result.get());
    de->size = sz;
    unsigned off = sizeof(chucho_door_event);
    de->formatted_message_offset = off;
    de->formatted_message_len = fm.length() + 1;
    std::strcpy(result.get() + off, fm.c_str());
    de->timestamp = event::clock_type::to_time_t(evt.get_time());
    off += de->formatted_message_len;
    de->file_name_offset = off;
    de->file_name_len = fnlen + 1;
    std::strcpy(result.get() + off, evt.get_file_name());
    de->line_number = evt.get_line_number();
    off += de->file_name_len;
    de->function_name_offset = off;
    de->function_name_len = funclen + 1;
    std::strcpy(result.get() + off, evt.get_function_name());
    off += de->function_name_len;
    de->logger_name_offset = off;
    de->logger_name_len = evt.get_logger()->get_name().length() + 1;
    std::strcpy(result.get() + off, evt.get_logger()->get_name().c_str());
    off += de->logger_name_len;
    de->level_name_offset = off;
    de->level_name_len = lvllen + 1;
    std::strcpy(result.get() + off, evt.get_level()->get_name());
    off += de->level_name_len;
    if (evt.get_marker())
    {
        de->marker_offset = off;
        de->marker_len = mrk.length() + 1;
        std::strcpy(result.get() + off, mrk.c_str());
        off += de->marker_len;
    }
    de->thread_id_offset = off;
    de->thread_id_len = thr.length() + 1;
    std::strcpy(result.get() + off, thr.c_str());
    return result;
}

void door_writer::flush()
{
}

void door_writer::write_impl(const event& evt)
{
    try
    {
        ensure_access();
        if (is_open())
        {
            auto devt = create_door_event(evt);
            door_arg_t darg;
            std::memset(&darg, 0, sizeof(darg));
            darg.data_ptr = devt.get();
            darg.data_size = reinterpret_cast<chucho_door_event*>(devt.get())->size;
            if (door_call(get_file_descriptor(), &darg) != 0)
            {
                int err = errno;
                if (err == EBADF)
                {
                    report_error("The file " + get_file_name() + " is not a door. Retrying in 3 seconds.");
                    close();
                }
                else if (err == ENOBUFS)
                {
                    std::size_t data_max;
                    std::ostringstream stream;
                    stream << "The event size " << darg.data_size << " is greater than the maximum size ";
                    if (door_getparam(get_file_descriptor(), DOOR_PARAM_DATA_MAX, &data_max) == 0)
                        stream << data_max << ' ';
                    stream << "allowed by the door attached to " << get_file_name();
                    report_error(stream.str());
                }
                else
                {
                    report_error("An error occurred writing to the door attached to " + get_file_name() + ": " + std::strerror(err));
                }
            }
        }
        else
        {
            report_error("Cannot write to door attached to " + get_file_name() + " because it is not open");
        }
    }
    catch (exception& e)
    {
#if defined(CHUCHO_HAVE_NESTED_EXCEPTIONS)
        std::throw_with_nested(file_exception("Could not write to door attached to " + get_file_name()));
#else
        throw file_exception("Could not write to door attached to " + get_file_name() + ": " + e.what());
#endif
    }
}

}
