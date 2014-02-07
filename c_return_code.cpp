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

#include <chucho/return_code.h>
#include <chucho/garbage_cleaner.hpp>
#include <mutex>
#include <map>

namespace
{

struct static_data
{
    static_data();

    std::map<chucho_rc, const char*> messages_;
};

static_data::static_data()
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
    messages_[CHUCHO_NO_ERROR] = "No error";
    messages_[CHUCHO_NO_SUCH_LEVEL] = "No such level";
    messages_[CHUCHO_NULL_POINTER] = "Null pointer";
    messages_[CHUCHO_INSUFFICIENT_BUFFER] = "Insufficient buffer";
    messages_[CHUCHO_FORMAT_ERROR] = "Format error";
    messages_[CHUCHO_OUT_OF_MEMORY] = "Out of memory";
    messages_[CHUCHO_TYPE_MISMATCH] = "Type mismatch";
    messages_[CHUCHO_INVALID_PATTERN] = "Invalid pattern";
    messages_[CHUCHO_INVALID_ARGUMENT] = "Invalid argument";
    messages_[CHUCHO_NO_SUCH_VALUE] = "No such value";
    messages_[CHUCHO_CONNECTION_ERROR] = "Connection error";
}

std::once_flag once;

static_data& data()
{
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

}

extern "C"
{

const char* chucho_rc_message(chucho_rc rc)
{
    static_data& sd(data());
    auto found = sd.messages_.find(rc);
    return found == sd.messages_.end() ? "<unknown error code>" : found->second;
}

}
