/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/optional_features.hpp>
#include <mutex>
#include <functional>

namespace
{

void init_feature_set(chucho::optional_features::feature_set& fs)
{
#if defined(CHUCHO_HAVE_ACTIVEMQ_WRITER)
    fs.set(chucho::optional_features::ACTIVEMQ_WRITER);
#endif
#if defined(CHUCHO_HAVE_BZIP2_COMPRESSION)
    fs.set(chucho::optional_features::BZIP2_COMPRESSION);
#endif
    // We always have C API now
    fs.set(chucho::optional_features::C_API);
#if defined(CHUCHO_HAVE_CAPN_PROTO_SERIALIZER)
    fs.set(chucho::optional_features::CAPN_PROTO_SERIALIZER);
#endif
#if defined(CHUCHO_HAVE_CONFIG_FILE_CONFIG)
    fs.set(chucho::optional_features::CONFIG_FILE_CONFIGURATION);
#endif
#if defined(CHUCHO_HAVE_CURL)
    fs.set(chucho::optional_features::EMAIL_WRITER);
    fs.set(chucho::optional_features::LOGGLY_WRITER);
#endif
#if defined(CHUCHO_HAVE_LOG4CPLUS_CONFIG)
    fs.set(chucho::optional_features::LOG4CPLUS_CONFIGURATION);
#endif
#if defined(CHUCHO_HAVE_MINIZIP_COMPRESSION)
    fs.set(chucho::optional_features::MINIZIP_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_PROTOBUF_SERIALIZER)
    fs.set(chucho::optional_features::PROTOBUF_SERIALIZER);
#endif
#if defined(CHUCHO_HAVE_RABBITMQ_WRITER)
    fs.set(chucho::optional_features::RABBITMQ_WRITER);
#endif
#if defined(CHUCHO_HAVE_RUBY_EVALUATOR_FILTER)
    fs.set(chucho::optional_features::RUBY_EVALUATOR_FILTER);
#endif
#if defined(CHUCHO_HAVE_YAML_CONFIG)
    fs.set(chucho::optional_features::YAML_CONFIGURATION);
#endif
#if defined(CHUCHO_HAVE_ZEROMQ_WRITER)
    fs.set(chucho::optional_features::ZEROMQ_WRITER);
#endif
#if defined(CHUCHO_HAVE_ZLIB_COMPRESSION)
    fs.set(chucho::optional_features::ZLIB_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_LIBARCHIVE_COMPRESSION)
    fs.set(chucho::optional_features::LIBARCHIVE_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_LZMA_COMPRESSION)
    fs.set(chucho::optional_features::LZMA_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_LZ4_COMPRESSION)
    fs.set(chucho::optional_features::LZ4_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_FLATBUFFERS)
    fs.set(chucho::optional_features::FLATBUFFERS_SERIALIZER);
#endif
#if defined(CHUCHO_HAVE_AWSSDK)
    fs.set(chucho::optional_features::CLOUDWATCH_WRITER);
#endif
#if defined(CHUCHO_HAVE_SOCI)
    fs.set(chucho::optional_features::DATABASE_WRITER);
#endif
}

}

namespace chucho
{

namespace optional_features
{

const feature_set& get()
{
    static std::once_flag once;
    static feature_set fs;

    std::call_once(once, init_feature_set, std::ref(fs));
    return fs;
}

}

}
