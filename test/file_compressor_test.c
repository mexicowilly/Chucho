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
#include <stdio.h>
#include <chucho/error.h>
#if defined(CHUCHO_HAVE_ZLIB)
#include <chucho/gzip_file_compressor.h>
#endif
#if defined(CHUCHO_HAVE_BZIP2)
#include <chucho/bzip2_file_compressor.h>
#endif
#if defined(CHUCHO_HAVE_MINIZIP)
#include <chucho/zip_file_compressor.h>
#endif

static const char* create_file()
{
    FILE* f = fopen("file_compressor_test_file", "w");
    if (f == NULL) 
        return NULL;
    char* chars = malloc(1024 * 100);
    memset(chars, 'W', 1024 * 100);
    fwrite(chars, 1, 1024 * 100, f);
    free(chars);
    fclose(f);
    return "file_compressor_test_file";
}

static void remove_file(const char* name, const char* ext)
{
    char* cmp = malloc(strlen(name) + strlen(ext) + 1);
    strcpy(cmp, name);
    strcat(cmp, ext);
    sput_fail_unless(remove(cmp) == 0, "remove compressed file");
    free(cmp);
}

#if defined(CHUCHO_HAVE_ZLIB)
static void gzip_compressor_test(void)
{
    chucho_file_compressor* cmp;
    int rc = chucho_create_gzip_file_compressor(&cmp, 3);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create gzip compressor");
    const char* ext;
    rc = chucho_cmp_get_extension(cmp, &ext);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get extension");
    sput_fail_unless(strcmp(ext, ".gz") == 0, "extension is .gz");
    unsigned idx;
    rc = chucho_cmp_get_min_index(cmp, &idx);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get min index");
    sput_fail_unless(idx == 3, "min index is 3");
    const char* fn = create_file();
    rc = chucho_cmp_compress(cmp, fn);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "compress file");
    remove_file(fn, ext);
    rc = chucho_release_file_compressor(cmp);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release compressor");
}
#endif

#if defined(CHUCHO_HAVE_BZIP2)
static void bzip2_compressor_test(void)
{
    chucho_file_compressor* cmp;
    int rc = chucho_create_bzip2_file_compressor(&cmp, 4);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create bzip2 compressor");
    const char* ext;
    rc = chucho_cmp_get_extension(cmp, &ext);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get extension");
    sput_fail_unless(strcmp(ext, ".bz2") == 0, "extension is .bz2");
    unsigned idx;
    rc = chucho_cmp_get_min_index(cmp, &idx);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get min index");
    sput_fail_unless(idx == 4, "min index is 4");
    const char* fn = create_file();
    rc = chucho_cmp_compress(cmp, fn);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "compress file");
    remove_file(fn, ext);
    rc = chucho_release_file_compressor(cmp);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release compressor");
}
#endif

#if defined(CHUCHO_HAVE_MINIZIP)
static void zip_compressor_test(void)
{
    chucho_file_compressor* cmp;
    int rc = chucho_create_zip_file_compressor(&cmp, 5);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create zip compressor");
    const char* ext;
    rc = chucho_cmp_get_extension(cmp, &ext);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get extension");
    sput_fail_unless(strcmp(ext, ".zip") == 0, "extension is .zip");
    unsigned idx;
    rc = chucho_cmp_get_min_index(cmp, &idx);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get min index");
    sput_fail_unless(idx == 5, "min index is 5");
    const char* fn = create_file();
    rc = chucho_cmp_compress(cmp, fn);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "compress file");
    remove_file(fn, ext);
    rc = chucho_release_file_compressor(cmp);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release compressor");
}
#endif

void run_file_compressor_test(void)
{
    sput_enter_suite("file_compressor");
    #if defined(CHUCHO_HAVE_ZLIB)
    sput_run_test(gzip_compressor_test);
    #endif
    #if defined(CHUCHO_HAVE_BZIP2)
    sput_run_test(bzip2_compressor_test);
    #endif
    #if defined(CHUCHO_HAVE_MINIZIP)
    sput_run_test(zip_compressor_test);
    #endif
    sput_leave_suite();
}
