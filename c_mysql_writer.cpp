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

#include <chucho/mysql_writer.hpp>
#include <chucho/mysql_writer.h>
#include <chucho/async_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/c_util.hpp>
#include <chucho/exception.hpp>

extern "C"
{

unsigned CHUCHO_DEFAULT_MYSQL_PORT = chucho::mysql_writer::DEFAULT_PORT;

chucho_rc chucho_create_mysql_writer(chucho_writer** wrt,
                                     chucho_formatter* fmt,
                                     const char* const host,
                                     const char* const user,
                                     const char* const password,
                                     const char* const database,
                                     unsigned port,
                                     size_t capacity,
                                     const chucho_level* discard_threshold,
                                     int flush_on_destruct)
{
    if (wrt == nullptr || fmt == nullptr || host == nullptr || user == nullptr ||
        password == nullptr || database == nullptr || discard_threshold == nullptr)
    {
        return CHUCHO_NULL_POINTER;
    }
    try
    {
        *wrt = new chucho_writer();
        (*wrt)->writer_ = std::make_shared<chucho::mysql_writer>(fmt->fmt_,
                                                                 host,
                                                                 user,
                                                                 password,
                                                                 database,
                                                                 port,
                                                                 capacity,
                                                                 discard_threshold->level_,
                                                                 flush_on_destruct);
    }
    catch (chucho::exception&) 
    {
        delete *wrt;
        return CHUCHO_CONNECTION_ERROR;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    chucho_release_formatter(fmt);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_mwrt_get_async_writer(const chucho_writer* wrt, chucho_writer** async)
{
    if (wrt == nullptr || async == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto mwrt = std::dynamic_pointer_cast<chucho::mysql_writer>(wrt->writer_);
    try
    {
        *async = new chucho_writer;
        (*async)->writer_ = mwrt->get_async_writer();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_mwrt_get_database(const chucho_writer* wrt, const char** database)
{
    if (wrt == nullptr || database == nullptr)
        return CHUCHO_NULL_POINTER;
    auto mwrt = std::dynamic_pointer_cast<chucho::mysql_writer>(wrt->writer_);
    if (!mwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *database = mwrt->get_database().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_mwrt_get_host(const chucho_writer* wrt, const char** host)
{
    if (wrt == nullptr || host == nullptr)
        return CHUCHO_NULL_POINTER;
    auto mwrt = std::dynamic_pointer_cast<chucho::mysql_writer>(wrt->writer_);
    if (!mwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *host = mwrt->get_host().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_mwrt_get_password(const chucho_writer* wrt, const char** password)
{
    if (wrt == nullptr || password == nullptr)
        return CHUCHO_NULL_POINTER;
    auto mwrt = std::dynamic_pointer_cast<chucho::mysql_writer>(wrt->writer_);
    if (!mwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *password = mwrt->get_password().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_mwrt_get_port(const chucho_writer* wrt, unsigned* port)
{
    if (wrt == nullptr || port == nullptr)
        return CHUCHO_NULL_POINTER;
    auto mwrt = std::dynamic_pointer_cast<chucho::mysql_writer>(wrt->writer_);
    if (!mwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *port = mwrt->get_port();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_mwrt_get_user(const chucho_writer* wrt, const char** user)
{
    if (wrt == nullptr || user == nullptr)
        return CHUCHO_NULL_POINTER;
    auto mwrt = std::dynamic_pointer_cast<chucho::mysql_writer>(wrt->writer_);
    if (!mwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *user = mwrt->get_user().c_str();
    return CHUCHO_NO_ERROR;
}

}
