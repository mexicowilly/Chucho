/*
 * Copyright 2013 Will Mason
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
#include <pthread.h>

namespace
{

void destructor(void* p)
{
    delete reinterpret_cast<std::map<std::string, std::string>*>(p);
}

class key_manager
{
public:
    key_manager();
    ~key_manager();

    pthread_key_t get_key();

private:
    pthread_key_t key_;
};

key_manager kmgr;

std::map<std::string, std::string>& get_map()
{
    void* p = pthread_getspecific(kmgr.get_key());
    if (p == nullptr)
    {
        p = new std::map<std::string, std::string>();
        pthread_setspecific(kmgr.get_key(), p);
    }
    return *reinterpret_cast<std::map<std::string, std::string>*>(p);
}

key_manager::key_manager()
{
    pthread_key_create(&key_, destructor);
}

key_manager::~key_manager()
{
    pthread_key_delete(key_);
}

pthread_key_t key_manager::get_key()
{
    return key_;
}

}

namespace chucho
{

std::string& diagnostic_context::at(const std::string& key)
{
    return get_map()[key];
}

void diagnostic_context::clear()
{
    get_map().clear();
}

bool diagnostic_context::empty()
{
    return get_map().empty();
}

void diagnostic_context::erase(const std::string& key)
{
    get_map().erase(key);
}

std::map<std::string, std::string> diagnostic_context::get()
{
    return get_map();
}

void diagnostic_context::set(const std::map<std::string, std::string>& ctx)
{
    get_map() = ctx;
}

}
