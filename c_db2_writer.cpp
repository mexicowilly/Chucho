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

#include <chucho/db2_writer.hpp>
#include <chucho/db2_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/exception.hpp>

extern "C"
{

chucho_rc chucho_create_db2_writer(chucho_writer** wrt,
                                   chucho_formatter* fmt,
                                   const char* const database,
                                   const char* const user,
                                   const char* const password)
{
    if (wrt == nullptr || fmt == nullptr || user == nullptr || password == nullptr || database == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        (*wrt)->writer_ = std::make_shared<chucho::db2_writer>(fmt->fmt_,
                                                               database,
                                                               user,
                                                               password);
    }
    catch (chucho::exception&) 
    {
        delete *wrt;
        *wrt = nullptr;
        return CHUCHO_CONNECTION_ERROR;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    chucho_release_formatter(fmt);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_db2wrt_get_database(const chucho_writer* wrt, const char** database)
{
    if (wrt == nullptr || database == nullptr)
        return CHUCHO_NULL_POINTER;
    auto db2wrt = std::dynamic_pointer_cast<chucho::db2_writer>(wrt->writer_);
    if (!db2wrt) 
        return CHUCHO_TYPE_MISMATCH;
    *database = db2wrt->get_database().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_db2wrt_get_password(const chucho_writer* wrt, const char** password)
{
    if (wrt == nullptr || password == nullptr)
        return CHUCHO_NULL_POINTER;
    auto db2wrt = std::dynamic_pointer_cast<chucho::db2_writer>(wrt->writer_);
    if (!db2wrt) 
        return CHUCHO_TYPE_MISMATCH;
    *password = db2wrt->get_password().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_db2wrt_get_user(const chucho_writer* wrt, const char** user)
{
    if (wrt == nullptr || user == nullptr)
        return CHUCHO_NULL_POINTER;
    auto db2wrt = std::dynamic_pointer_cast<chucho::db2_writer>(wrt->writer_);
    if (!db2wrt) 
        return CHUCHO_TYPE_MISMATCH;
    *user = db2wrt->get_user().c_str();
    return CHUCHO_NO_ERROR;
}

}