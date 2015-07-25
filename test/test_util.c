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

#include "test_util.h"
#include <string.h>
#include <stdlib.h>

char* read_line(FILE* f)
{
    char* result = NULL;
    if (!feof(f)) 
    {
        char buf[1024];
        char* rc = fgets(buf, sizeof(buf), f);
        if (rc != NULL) 
        {
            char* nl = strpbrk(buf, "\r\n");
            if (nl != NULL)
                *nl = 0;
            result = malloc(strlen(buf) + 1);
            strcpy(result, buf);
        }
    }
    return result;
}
