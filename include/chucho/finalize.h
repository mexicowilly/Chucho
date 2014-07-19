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

#if !defined(CHUCHO_FINALIZE_H__)
#define CHUCHO_FINALIZE_H__

/**
 * @file 
 * Finalize the Chucho library. 
 *  
 * @ingroup c_miscellaneous 
 */

#include <chucho/export.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Finalize the Chucho library. This cleans up all memory held 
 * by all Chucho objects and puts the library in an undefined 
 * state. 
 * 
 * @post No Chucho functions may be called after this one is.
 */
CHUCHO_EXPORT void chucho_finalize(void);

#if defined(__cplusplus)
}
#endif

#endif