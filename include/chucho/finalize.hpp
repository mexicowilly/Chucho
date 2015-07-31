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

#if !defined(CHUCHO_FINALIZE_HPP__)
#define CHUCHO_FINALIZE_HPP__

#include <chucho/export.hpp>

/**
 * @file 
 * Chucho's finalization functionality 
 */

/**
 * The Chucho namespace
 */
namespace chucho
{

/**
 * Finalize the Chucho library. This function should be called 
 * if you believe your memory leak analysis tools when they tell 
 * you that static objects that live the life of the application 
 * are memory leaks. You should also use this function if you 
 * are using Chucho as a shared object or within an enclosing 
 * shared object. 
 *  
 * Included by @p <chucho/finalize.hpp>. 
 *  
 * @note No Chucho APIs may be used after calling this function. 
 *       The result is extremely undefined.
 * @ingroup miscellaneous
 */
CHUCHO_EXPORT void finalize();

}

#endif
