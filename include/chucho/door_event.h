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

#if !defined(CHUCHO_DOOR_EVENT_H__)
#define CHUCHO_DOOR_EVENT_H__

#include <time.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct
{
    size_t size;
    unsigned int formatted_message_offset;
    unsigned int formatted_message_len;
    time_t timestamp;
    unsigned int file_name_offset;
    unsigned int file_name_len;
    unsigned int line_number;
    unsigned int function_name_offset;
    unsigned int function_name_len;
    unsigned int logger_name_offset;
    unsigned int logger_name_len;
    unsigned int level_name_offset;
    unsigned int level_name_len;
    unsigned int marker_offset;
    unsigned int marker_len;
    unsigned int thread_id_offset;
    unsigned int thread_id_len;
} chucho_door_event;

#define CHUCHO_DOOR_FORMATTED_MESSAGE(dep) ((const char*)(dep) + (dep)->formatted_message_offset)
#define CHUCHO_DOOR_FILE_NAME(dep) ((const char*)(dep) + (dep)->file_name_offset)
#define CHUCHO_DOOR_FUNCTION_NAME(dep) ((const char*)(dep) + (dep)->function_name_offset)
#define CHUCHO_DOOR_LOGGER_NAME(dep) ((const char*)(dep) + (dep)->logger_name_offset)
#define CHUCHO_DOOR_LEVEL_NAME(dep) ((const char*)(dep) + (dep)->level_name_offset)
#define CHUCHO_DOOR_MARKER(dep) ((const char*)(dep) + (dep)->marker_offset)
#define CHUCHO_DOOR_THREAD_ID(dep) ((const char*)(dep) + (dep)->thread_id_offset)

#if defined(__cplusplus)
}
#endif

#endif