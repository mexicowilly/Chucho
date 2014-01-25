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

namespace
{

int create_node(chucho_dgc_node** node,
                const char* const key,
                const char* const value)
{
    if (node == nullptr || key == nullptr || value == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *node = new chucho_dgc_node;
        (*node)->key = new char [std::strlen(key) + 1];
        std::strcpy((*node)->key, key);
        (*node)->value = new char [std::strlen(value) + 1];
        std::strcpy((*node)->value, value);
    }
    catch (...)
    {
        if (*node != nullptr)
        {
            delete [] (*node)->key;
            delete [] (*node)->value;
            delete *node;
            *node = nullptr;
        }
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

int create_nodes(chucho_dgc_node*** arr, size_t count)
{
    if (arr == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *arr = new chucho_dgc_node*[count + 1];
        std::memset(*arr, 0, sizeof(chucho_dgc_node*) * (count + 1));
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

int release_node(chucho_dgc_node* node)
{
    delete [] node->key;
    delete [] node->value;
    delete node; 
    return CHUCHO_NO_ERROR;
}

}

extern "C"
{

int chucho_dgc_release_nodes(chucho_dgc_node** arr)
{
    std::size_t i = 0;
    while (arr[i] != nullptr) 
        release_node(arr[i++]);
    delete [] arr;
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

int chucho_dgc_get(chucho_dgc_node*** nodes)
{
    if (nodes == nullptr)
        return CHUCHO_NULL_POINTER; 
    chucho_dgc_node** loc;
    try
    {
        auto cpp = chucho::diagnostic_context::get();
        int rc = create_nodes(&loc, cpp.size());
        if (rc != CHUCHO_NO_ERROR) 
            return rc;
        int i = 0;
        for (auto kv : cpp) 
        {
            int rc = create_node(&loc[i],
                                 kv.first.c_str(),
                                 kv.second.c_str());
            if (rc != CHUCHO_NO_ERROR) 
            {
                while (--i >= 0) 
                    release_node((*nodes)[i]);
                // Don't use chucho_dgc_release_nodes here
                delete [] loc;
                return CHUCHO_OUT_OF_MEMORY;
            }
            i++;
        }
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    *nodes = loc;
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

int chucho_dgc_set_all(chucho_dgc_node** nodes)
{
    try
    {
        std::map<std::string, std::string> ctx;
        size_t i = 0;
        while (nodes[i] != nullptr) 
        {
            ctx[nodes[i]->key] = nodes[i]->value;
            i++;
        }
        chucho_dgc_release_nodes(nodes);
        chucho::diagnostic_context::set(ctx);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
