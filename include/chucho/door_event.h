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

/**
 * @file
 * Data type passed to a door.
 */

#include <time.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @struct chucho_door_event door_event.h chucho/door_event.h
 * A structure used by the @ref chucho::door_writer
 * to pass data to the door server process. The fields of variable
 * length, such as formatted message, are appened to the end of the data
 * structure and their offsets and lengths are stored in the structure.
 * All textual fields' lengths include the trailing zero.
 */
typedef struct
{
    size_t size;                            /**< The size of this data structure */
    unsigned int formatted_message_offset;  /**< The offset from the start of this structure to the formatted message */
    unsigned int formatted_message_len;     /**< The length of the formatted message including the trailing zero */
    time_t timestamp;                       /**< The timestamp of the event */
    unsigned int file_name_offset;          /**< The offset from the start of this structure to the file name */
    unsigned int file_name_len;             /**< The length of the file named including the trailing zero */
    unsigned int line_number;               /**< The line number of the event */
    unsigned int function_name_offset;      /**< The offset from the start of this structure to the function name */
    unsigned int function_name_len;         /**< The length of the function name including the trailing zero */
    unsigned int logger_name_offset;        /**< The offset from the start of this structure to the logger name */
    unsigned int logger_name_len;           /**< The length of the logger name including the trailing zero */
    unsigned int level_name_offset;         /**< The offset from the start of this structure to the level name */
    unsigned int level_name_len;            /**< The length of the level name including the trailing zero */
    unsigned int marker_offset;             /**< The offset from the start of this structure to the marker */
    unsigned int marker_len;                /**< The length of the marker including the trailing zero */
    unsigned int thread_id_offset;          /**< The offset from the start of this structure to the thread ID */
    unsigned int thread_id_len;             /**< The length of the thread ID including the trailing zero */
} chucho_door_event;

/**
 * Retrieve the formatted messasge from a @ref chucho_door_event.
 *
 * @param dep a pointer to the chucho_door_event
 */
#define CHUCHO_DOOR_FORMATTED_MESSAGE(dep) ((const char*)(dep) + (dep)->formatted_message_offset)
/**
 * Retrieve the file name from a @ref chucho_door_event.
 *
 * @param dep a pointer to the chucho_door_event
 */
#define CHUCHO_DOOR_FILE_NAME(dep) ((const char*)(dep) + (dep)->file_name_offset)
/**
 * Retrieve the function name from a @ref chucho_door_event.
 *
 * @param dep a pointer to the chucho_door_event
 */
#define CHUCHO_DOOR_FUNCTION_NAME(dep) ((const char*)(dep) + (dep)->function_name_offset)
/**
 * Retrieve the logger name from a @ref chucho_door_event.
 *
 * @param dep a pointer to the chucho_door_event
 */
#define CHUCHO_DOOR_LOGGER_NAME(dep) ((const char*)(dep) + (dep)->logger_name_offset)
/**
 * Retrieve the level name from a @ref chucho_door_event.
 *
 * @param dep a pointer to the chucho_door_event
 */
#define CHUCHO_DOOR_LEVEL_NAME(dep) ((const char*)(dep) + (dep)->level_name_offset)
/**
 * Retrieve the marker from a @ref chucho_door_event.
 *
 * @param dep a pointer to the chucho_door_event
 */
#define CHUCHO_DOOR_MARKER(dep) ((const char*)(dep) + (dep)->marker_offset)
/**
 * Retrieve the thread ID from a @ref chucho_door_event.
 *
 * @param dep a pointer to the chucho_door_event
 */
#define CHUCHO_DOOR_THREAD_ID(dep) ((const char*)(dep) + (dep)->thread_id_offset)

#if defined(__cplusplus)
}
#endif

#endif