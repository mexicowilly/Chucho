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

#include "sput.h"
#include <chucho/configuration.h>
#include <chucho/finalize.h>
#include "log_macro_test.h"
#include "file_writer_test.h"
#include "configuration_test.h"
#include "async_writer_test.h"
#if defined(CHUCHO_HAVE_ZLIB) || defined(CHUCHO_HAVE_BZIP2) || defined(CHUCHO_HAVE_MINIZIP)
#include "file_compressor_test.h"
#endif

sput_struct __sput;

int main()
{
    chucho_cnf_set_style(CHUCHO_CONFIGURATION_STYLE_OFF);
    sput_start_testing();
    run_log_macro_test();
    run_file_writer_test();
    run_configuration_test();
    run_async_writer_test();
    #if defined(CHUCHO_HAVE_ZLIB) || defined(CHUCHO_HAVE_BZIP2) || defined(CHUCHO_HAVE_MINIZIP)
    run_file_compressor_test();
    #endif
    sput_finish_testing();
    chucho_finalize();
    return sput_get_return_value();
}
