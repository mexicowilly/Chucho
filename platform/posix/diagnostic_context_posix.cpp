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

#include <chucho/diagnostic_context.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <mutex>
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

std::once_flag once;

key_manager& kmgr()
{
    // This will be cleaned at finalize time
    static key_manager* km;

    std::call_once(once, [&] () { km = new key_manager(); });
    return *km;
}

key_manager::key_manager()
{
    pthread_key_create(&key_, destructor);
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

key_manager::~key_manager()
{
    // Go ahead and delete manually here, because when the thread
    // that is running the garbage_cleaner is the same thread that
    // created the key, then the key will be deleted before the
    // destructor function gets a chance to be called, and the map
    // will leak. This has been observed.
    delete reinterpret_cast<std::map<std::string, std::string>*>(pthread_getspecific(key_));
    pthread_key_delete(key_);
}

pthread_key_t key_manager::get_key()
{
    return key_;
}

}

namespace chucho
{

std::map<std::string, std::string>& diagnostic_context::get_map()
{
    void* p = pthread_getspecific(kmgr().get_key());
    if (p == nullptr)
    {
        p = new std::map<std::string, std::string>();
        pthread_setspecific(kmgr().get_key(), p);
    }
    return *reinterpret_cast<std::map<std::string, std::string>*>(p);
}

}
