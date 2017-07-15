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
#include <chucho/rolling_file_writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_formatter.hpp>
#include <chucho/c_file_roller.hpp>
#include <chucho/c_file_roll_trigger.hpp>

extern "C"
{

chucho_rc chucho_create_rolling_file_writer(chucho_writer** wrt,
                                            chucho_formatter* fmt,
                                            chucho_file_roller* rlr,
                                            chucho_file_roll_trigger* trg,
                                            const char* const name,
                                            chucho_on_start on_start,
                                            int flush)
{
    if (wrt == nullptr || fmt == nullptr || rlr == nullptr) 
        return CHUCHO_NULL_POINTER;
    chucho_writer* loc = nullptr;
    try
    {
        *wrt = new chucho_writer();
        chucho::file_writer::on_start ons = on_start == CHUCHO_ON_START_APPEND ?
            chucho::file_writer::on_start::APPEND : chucho::file_writer::on_start::TRUNCATE;
        std::shared_ptr<chucho::file_roll_trigger> cpptrg = trg == nullptr ?
            std::shared_ptr<chucho::file_roll_trigger>() : trg->trg_;
        if (name == nullptr) 
        {
            (*wrt)->writer_ = std::shared_ptr<chucho::rolling_file_writer>(
                new chucho::rolling_file_writer(fmt->fmt_,
                                                ons,
                                                flush == 0 ? false : true,
                                                rlr->rlr_,
                                                cpptrg));
        }
        else
        {
            // Visual Studio 2012 will not allow std::make_shared here
            (*wrt)->writer_ = std::shared_ptr<chucho::rolling_file_writer>(
                new chucho::rolling_file_writer(fmt->fmt_,
                                                name,
                                                ons,
                                                flush == 0 ? false : true,
                                                rlr->rlr_,
                                                cpptrg));
        }
        chucho_release_formatter(fmt);
        chucho_release_file_roller(rlr);
        chucho_release_file_roll_trigger(trg);
    }
    catch (std::invalid_argument&) 
    {
        delete *wrt;
        *wrt = nullptr;
        return CHUCHO_INVALID_ARGUMENT;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rfwrt_get_file_roller(const chucho_writer* wrt, chucho_file_roller** rlr)
{
    if (wrt == nullptr || rlr == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto rfw = std::dynamic_pointer_cast<chucho::rolling_file_writer>(wrt->writer_);
    if (!rfw) 
        return CHUCHO_TYPE_MISMATCH;
    try
    {
        *rlr = new chucho_file_roller;
        (*rlr)->rlr_ = rfw->get_file_roller();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rfwrt_get_file_roll_trigger(const chucho_writer* wrt, chucho_file_roll_trigger** trg)
{
    if (wrt == nullptr || trg == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto rfw = std::dynamic_pointer_cast<chucho::rolling_file_writer>(wrt->writer_);
    if (!rfw) 
        return CHUCHO_TYPE_MISMATCH;
    try
    {
        auto cpptrg = rfw->get_file_roll_trigger();
        if (cpptrg) 
        {
            *trg = new chucho_file_roll_trigger;
            (*trg)->trg_ = cpptrg;
        }
        else
        {
            *trg = nullptr;
        }
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
