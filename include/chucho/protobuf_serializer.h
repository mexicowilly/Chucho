/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_PROTOBUF_SERIALIZER_H__)
#define CHUCHO_PROTOBUF_SERIALIZER_H__

/**
 * @file 
 * @copydoc chucho::protobuf_serializer 
 *  
 * @ingroup c_mq 
 */

#include <chucho/serializer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create a protobuf serializer.
 * 
 * @post Ownership of the ser paramter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_serializer() function.
 * 
 * @param ser the serializer to create
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_protobuf_serializer(chucho_serializer** ser);

#if defined(__cplusplus)
}
#endif

#endif
