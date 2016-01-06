/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/email_writer.hpp>
#include <chucho/email_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/c_email_trigger.hpp>
#include <chucho/exception.hpp>

#include <iostream>

extern "C"
{

unsigned CHUCHO_DEFAULT_EMAIL_PORT = chucho::email_writer::DEFAULT_PORT;
size_t CHUCHO_DEFAULT_EMAIL_BUFFER_CAPACITY = chucho::email_writer::DEFAULT_BUFFER_CAPACITY;

chucho_rc chucho_create_email_writer(chucho_writer** wrt,
                                     chucho_formatter* fmt,
                                     const char* const host,
                                     chucho_email_connection_type connect,
                                     const char* const* to,
                                     const char* const from,
                                     const char* const subject,
                                     chucho_email_trigger* trg,
                                     const char* const user,
                                     const char* const password,
                                     unsigned port,
                                     size_t buffer_capacity)
{
    if (wrt == nullptr || fmt == nullptr || host == nullptr || to == nullptr ||
        from == nullptr || subject == nullptr || trg == nullptr ||
        (user == nullptr && password != nullptr) || (user != nullptr && password == nullptr))
    {
        return CHUCHO_NULL_POINTER;
    }
    try
    {
        std::vector<std::string> tov;
        for (unsigned i = 0; to[i] != nullptr; i++)
            tov.push_back(to[i]);
        chucho::email_writer::connection_type conn;
        if (connect == CHUCHO_EMAIL_CONNECTION_CLEAR)
            conn = chucho::email_writer::connection_type::CLEAR;
        else if (connect == CHUCHO_EMAIL_CONNECTION_SSL)
            conn = chucho::email_writer::connection_type::SSL;
        else if (connect == CHUCHO_EMAIL_CONNECTION_STARTTLS)
            conn = chucho::email_writer::connection_type::STARTTLS;
        *wrt = new chucho_writer();
        if (user == nullptr && password == nullptr)
        {
            (*wrt)->writer_ = std::make_shared<chucho::email_writer>(fmt->fmt_,
                                                                     host,
                                                                     conn,
                                                                     tov,
                                                                     from,
                                                                     subject,
                                                                     trg->trg_,
                                                                     port,
                                                                     buffer_capacity);
        }
        else
        {
            (*wrt)->writer_ = std::make_shared<chucho::email_writer>(fmt->fmt_,
                                                                     host,
                                                                     conn,
                                                                     tov,
                                                                     from,
                                                                     subject,
                                                                     trg->trg_,
                                                                     user,
                                                                     password,
                                                                     port,
                                                                     buffer_capacity);
        }
    }
    catch (chucho::exception&)
    {
        delete *wrt;
        *wrt = nullptr;
        return CHUCHO_INVALID_ARGUMENT;
    }
    catch (...)
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    chucho_release_formatter(fmt);
    chucho_release_email_trigger(trg);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_buffer_capacity(const chucho_writer* wrt, size_t* cap)
{
    if (wrt == nullptr || cap == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *cap = ewrt->get_buffer_capacity();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_buffer_size(const chucho_writer* wrt, size_t* sz)
{
    if (wrt == nullptr || sz == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *sz = ewrt->get_buffer_size();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_connection_type(const chucho_writer* wrt,
                                          chucho_email_connection_type* tp)
{
    if (wrt == nullptr || tp == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    chucho::email_writer::connection_type ltp = ewrt->get_connection_type();
    if (ltp == chucho::email_writer::connection_type::CLEAR)
        *tp = CHUCHO_EMAIL_CONNECTION_CLEAR;
    else if (ltp == chucho::email_writer::connection_type::SSL)
        *tp = CHUCHO_EMAIL_CONNECTION_SSL;
    else if (ltp == chucho::email_writer::connection_type::STARTTLS)
        *tp = CHUCHO_EMAIL_CONNECTION_STARTTLS;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_from(const chucho_writer* wrt, const char** frm)
{
    if (wrt == nullptr || frm == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *frm = ewrt->get_from().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_host(const chucho_writer* wrt, const char** hst)
{
    if (wrt == nullptr || hst == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *hst = ewrt->get_host().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_password(const chucho_writer* wrt, const char** pwd)
{
    if (wrt == nullptr || pwd == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *pwd = ewrt->get_password() ? ewrt->get_password()->c_str() : nullptr;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_port(const chucho_writer* wrt, unsigned* prt)
{
    if (wrt == nullptr || prt == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *prt = ewrt->get_port();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_subject(const chucho_writer* wrt, const char** sub)
{
    if (wrt == nullptr || sub == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *sub = ewrt->get_subject().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_to(const chucho_writer* wrt,
                             const char*** rcpts,
                             size_t sz,
                             size_t* num)
{
    if (wrt == nullptr || num == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    try
    {
        const auto& tos = ewrt->get_to();
        *num = tos.size();
        if (sz < tos.size()) 
            return CHUCHO_INSUFFICIENT_BUFFER;
        if (rcpts == nullptr) 
            return CHUCHO_NULL_POINTER;
        for (unsigned i = 0; i < *num; i++)
            (*rcpts)[i] = tos[i].c_str();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_trigger(const chucho_writer* wrt, chucho_email_trigger** trg)
{
    if (wrt == nullptr || trg == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    try
    {
        *trg = new chucho_email_trigger();
        (*trg)->trg_ = ewrt->get_trigger();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_user(const chucho_writer* wrt, const char** usr)
{
    if (wrt == nullptr || usr == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *usr = ewrt->get_user() ? ewrt->get_user()->c_str() : nullptr;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_get_verbose(const chucho_writer* wrt, int* vb)
{
    if (wrt == nullptr || vb == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    *vb = ewrt->get_verbose() ? 1 : 0;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ewrt_set_verbose(const chucho_writer* wrt, int vb)
{
    if (wrt == nullptr)
        return CHUCHO_NULL_POINTER;
    auto ewrt = std::dynamic_pointer_cast<chucho::email_writer>(wrt->writer_);
    if (!ewrt) 
        return CHUCHO_TYPE_MISMATCH;
    try
    {
        ewrt->set_verbose(vb != 0);
    }
    catch (...)
    {
        return CHUCHO_INVALID_ARGUMENT;
    }
    return CHUCHO_NO_ERROR;
}

}
