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

#if !defined(CHUCHO_DIAGNOSTIC_CONTEXT_H__)
#define CHUCHO_DIAGNOSTIC_CONTEXT_H__

/**
 * @file 
 * @copydoc chucho::diagnostic_context 
 *  
 * @ingroup c_miscellaneous 
 */

#include <chucho/return_code.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * A key-value pair from the diagnostic context. This type is 
 * returned from the @ref chucho_dgc_get function. 
 */
typedef struct
{
    /**
     * The key.
     */
    char* key;
    /**
     * The value.
     */
    char* value;
} chucho_dgc_node;

/**
 * Release a diagnostic context node. This function must be 
 * called for any nodes returned from the @ref chucho_dgc_get 
 * function. 
 *  
 * @post Ownership of the node parameter is transferred to the 
 *       callee.
 * 
 * @param[in] node 
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_dgc_release_node(chucho_dgc_node* node);
/**
 * Return the value for a given key. If there is no value for 
 * the key, then a string of length zero is returned. 
 * 
 * @param[in] key the key
 * @return the value
 */
CHUCHO_EXPORT const char* chucho_dgc_at(const char* const key);
/**
 * Clear the diagnostice context.
 *
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_dgc_clear(void);
/**
 * Return a non-zero value if the diagnostic context is empty.
 * 
 * @return non-zero if the context is empty
 */
CHUCHO_EXPORT int chucho_dgc_empty(void);
/**
 * Erase a given key and its value from the context.
 * 
 * @param[in] key the key to erase
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_dgc_erase(const char* const key);
/**
 * Return all nodes from the diagnostic context. The passed in 
 * array of node pointers is filled. The count paramters is 
 * always set to the actual number of nodes in the context. 
 *  
 * @post Ownership of the returned nodes is transferred to the 
 *       callee, and they must be released with the @ref
 *       chucho_dgc_release_node() function.
 * 
 * @param[out] buf the array into which to place the nodes
 * @param[in] buf_size the size of the array
 * @param[out] count the number of nodes in the diagnostic 
 *       context
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_dgc_get(chucho_dgc_node** buf, size_t buf_size, size_t* count);
/**
 * Set a key-value pair into the diagnostic context.
 * 
 * @param[in] key the key
 * @param[in] value the value
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_dgc_set(const char* const key, const char* const value);

#if defined(__cplusplus)
}
#endif

#endif
