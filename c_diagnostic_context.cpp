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

#include <chucho/diagnostic_context.hpp>
#include <chucho/diagnostic_context.h>
#include <chucho/error.h>
#include <cstring>

extern "C"
{

int chucho_dgc_release_node(chucho_dgc_node* node)
{
    if (node == nullptr) 
        return CHUCHO_NULL_POINTER;
    delete [] node->key;
    delete [] node->value;
    delete node;
    return CHUCHO_NO_ERROR;
}

const char* chucho_dgc_at(const char* const key)
{
    try
    {
        return chucho::diagnostic_context::at(key).c_str();
    }
    catch (...) 
    {
        return NULL;
    }
}

int chucho_dgc_clear(void)
{
    try
    {
        chucho::diagnostic_context::clear();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

int chucho_dgc_empty(void)
{
    try
    {
        return chucho::diagnostic_context::empty() ? 1 : 0;
    }
    catch (...) 
    {
        return 1;
    }
}

int chucho_dgc_erase(const char* const key)
{
    try
    {
        chucho::diagnostic_context::erase(key);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

int chucho_dgc_get(chucho_dgc_node** buf, size_t buf_size, size_t* count)
{
    if (count == nullptr)
        return CHUCHO_NULL_POINTER; 
    try
    {
        auto cpp = chucho::diagnostic_context::get();
        *count = cpp.size();
        if (buf_size < cpp.size()) 
            return CHUCHO_INSUFFICIENT_BUFFER;
        if (buf == nullptr) 
            return CHUCHO_NULL_POINTER;
        unsigned idx = 0;
        for (auto kv : cpp) 
        {
            buf[idx] = new chucho_dgc_node;
            buf[idx]->key = new char[kv.first.length() + 1];
            std::strcpy(buf[idx]->key, kv.first.c_str());
            buf[idx]->value = new char[kv.second.length() + 1];
            std::strcpy(buf[idx]->value, kv.second.c_str());
            idx++;
        }
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

int chucho_dgc_set(const char* const key, const char* const value)
{
    if (key == nullptr || value == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho::diagnostic_context::at(key) = value;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
