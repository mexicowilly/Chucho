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

#include <chucho/logger.h>
#include <chucho/c_logger.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/c_util.hpp>
#include <chucho/level.h>
#include <chucho/writer.h>

extern "C"
{

chucho_rc chucho_create_logger(chucho_logger** lgr, const char* const name)
{
    try
    {
        *lgr = new chucho_logger();
        (*lgr)->logger_ = chucho::logger::get(name);
    }
    catch (...)
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_add_writer(chucho_logger* lgr, chucho_writer* wrt)
{
    if (lgr == nullptr || wrt == nullptr)
        return CHUCHO_NULL_POINTER; 
    try
    {
        lgr->logger_->add_writer(wrt->writer_);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return chucho_release_writer(wrt);
}

chucho_rc chucho_lgr_get_effective_level(const chucho_logger* lgr, const chucho_level** lvl)
{
    if (lgr == nullptr || lvl == nullptr)
        return CHUCHO_NULL_POINTER;
    const chucho_level* clvl = chucho::c_util::cpp_level_to_c(lgr->logger_->get_effective_level());
    if (clvl == nullptr)
        return CHUCHO_NO_SUCH_LEVEL;
    *lvl = clvl;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_get_level(const chucho_logger* lgr, const chucho_level** lvl)
{
    if (lgr == nullptr || lvl == nullptr)
        return CHUCHO_NULL_POINTER;
    const chucho_level* clvl = chucho::c_util::cpp_level_to_c(lgr->logger_->get_level());
    if (clvl == nullptr) 
        return CHUCHO_NO_SUCH_LEVEL;
    *lvl = clvl;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_get_name(const chucho_logger* lgr, const char** nm)
{
    if (lgr == nullptr || nm == nullptr)
        return CHUCHO_NULL_POINTER;
    *nm = lgr->logger_->get_name().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_get_writers(const chucho_logger* lgr, chucho_writer** buf, size_t buf_size, size_t* count)
{
    if (lgr == nullptr || count == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        auto wrts = lgr->logger_->get_writers();
        *count = wrts.size();
        if (buf_size < wrts.size())
            return CHUCHO_INSUFFICIENT_BUFFER;
        if (buf == nullptr)
            return CHUCHO_NULL_POINTER; 
        unsigned idx = 0;
        for (auto wrt : wrts) 
        {
            buf[idx] = new chucho_writer();
            buf[idx]->writer_ = wrt;
            idx++;
        }
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_permits(const chucho_logger* lgr, const chucho_level* lvl, int* state)
{
    if (lgr == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    *state = lgr->logger_->permits(lvl->level_) ? 1 : 0;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_release_logger(chucho_logger* lgr)
{
    delete lgr;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_remove_all_writers(chucho_logger* lgr)
{
    if (lgr == nullptr) 
        return CHUCHO_NULL_POINTER;
    lgr->logger_->remove_all_writers();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_remove_writer(chucho_logger* lgr, chucho_writer* wrt)
{
    if (lgr == nullptr || wrt == nullptr)
        return CHUCHO_NULL_POINTER; 
    lgr->logger_->remove_writer(wrt->writer_);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_set_level(chucho_logger* lgr, const chucho_level* lvl)
{
    if (lgr == nullptr || lvl == nullptr)
        return CHUCHO_NULL_POINTER; 
    lgr->logger_->set_level(lvl->level_);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_set_writes_to_ancestors(chucho_logger* lgr, int state)
{
    if (lgr == nullptr)
        return CHUCHO_NULL_POINTER;
    lgr->logger_->set_writes_to_ancestors(state == 0 ? false : true); 
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lgr_writes_to_ancestors(const chucho_logger* lgr, int* state)
{
    if (lgr == nullptr)
        return CHUCHO_NULL_POINTER;
    *state = lgr->logger_->writes_to_ancestors() ? 1 : 0;
    return CHUCHO_NO_ERROR; 
}

chucho_rc chucho_remove_unused_loggers(void)
{
    chucho::logger::remove_unused_loggers();
    return CHUCHO_NO_ERROR;
}

}
