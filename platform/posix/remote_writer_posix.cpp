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

#include <chucho/remote_writer.hpp>

namespace chucho
{

void remote_writer::lower_connector_thread_priority()
{
    int policy;
    struct sched_param param;
    pthread_getschedparam(connector_thread_.native_handle(), &policy, &param);
    param.sched_priority = PTHREAD_MIN_PRIORITY;
    pthread_setschedparam(connector_thread_.native_handle(), policy, &param);
}

}
