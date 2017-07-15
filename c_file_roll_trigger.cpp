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

#include <chucho/c_file_roll_trigger.hpp>
#include <chucho/file_roll_trigger.h>

extern "C"
{

chucho_rc chucho_release_file_roll_trigger(chucho_file_roll_trigger* trg)
{
    delete trg;
    return CHUCHO_NO_ERROR;
}

}
