/*
 * Copyright 2013-2021 Will Mason
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

#include <gtest/gtest.h>
#include <chucho/optional_features.hpp>
#include <iostream>

#define EXPECT_FEATURE(feat) \
    std::cout << "Testing " << #feat << std::endl; \
    EXPECT_TRUE(fs.test( feat )); \
    EXPECT_TRUE(chucho::optional_features::have( feat )); \
    fs.reset( feat )

TEST(optional_features, simple)
{
    chucho::optional_features::feature_set fs = chucho::optional_features::get();
#if defined(CHUCHO_HAVE_ACTIVEMQ)
    EXPECT_FEATURE(chucho::optional_features::ACTIVEMQ_WRITER);
#endif
#if defined(CHUCHO_HAVE_BZIP2)
    EXPECT_FEATURE(chucho::optional_features::BZIP2_COMPRESSION);
#endif
    EXPECT_FEATURE(chucho::optional_features::C_API);
#if defined(CHUCHO_HAVE_CAPN_PROTO)
    EXPECT_FEATURE(chucho::optional_features::CAPN_PROTO_SERIALIZER);
#endif
#if defined(CHUCHO_HAVE_DB2)
    EXPECT_FEATURE(chucho::optional_features::DB2_WRITER);
#endif
    EXPECT_FEATURE(chucho::optional_features::CONFIG_FILE_CONFIGURATION);
#if defined(CHUCHO_HAVE_CURL)
    EXPECT_FEATURE(chucho::optional_features::EMAIL_WRITER);
    EXPECT_FEATURE(chucho::optional_features::LOGGLY_WRITER);
#endif
    EXPECT_FEATURE(chucho::optional_features::LOG4CPLUS_CONFIGURATION);
#if defined(CHUCHO_HAVE_LIBARCHIVE)
    EXPECT_FEATURE(chucho::optional_features::LIBARCHIVE_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_LZMA)
    EXPECT_FEATURE(chucho::optional_features::LZMA_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_MYSQL)
    EXPECT_FEATURE(chucho::optional_features::MYSQL_WRITER);
#endif
#if defined(CHUCHO_HAVE_ORACLE)
    EXPECT_FEATURE(chucho::optional_features::ORACLE_WRITER);
#endif
#if defined(CHUCHO_HAVE_POSTGRES)
    EXPECT_FEATURE(chucho::optional_features::POSTGRESQL_WRITER);
#endif
#if defined(CHUCHO_HAVE_PROTOBUF)
    EXPECT_FEATURE(chucho::optional_features::PROTOBUF_SERIALIZER);
#endif
#if defined(CHUCHO_HAVE_RABBITMQ)
    EXPECT_FEATURE(chucho::optional_features::RABBITMQ_WRITER);
#endif
#if defined(CHUCHO_HAVE_RUBY)
    EXPECT_FEATURE(chucho::optional_features::RUBY_EVALUATOR_FILTER);
#endif
#if defined(CHUCHO_HAVE_SQLITE)
    EXPECT_FEATURE(chucho::optional_features::SQLITE_WRITER);
#endif
    EXPECT_FEATURE(chucho::optional_features::YAML_CONFIGURATION);
#if defined(CHUCHO_HAVE_ZEROMQ)
    EXPECT_FEATURE(chucho::optional_features::ZEROMQ_WRITER);
#endif
#if defined(CHUCHO_HAVE_ZLIB)
    EXPECT_FEATURE(chucho::optional_features::ZLIB_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_LZ4)
    EXPECT_FEATURE(chucho::optional_features::LZ4_COMPRESSION);
#endif
#if defined(CHUCHO_HAVE_FLATBUFFERS)
    EXPECT_FEATURE(chucho::optional_features::FLATBUFFERS_SERIALIZER);
#endif
#if defined(CHUCHO_HAVE_AWSSDK)
    EXPECT_FEATURE(chucho::optional_features::CLOUDWATCH_WRITER);
#endif
#if defined(CHUCHO_HAVE_SOCI)
    EXPECT_FEATURE(chucho::optional_features::DATABASE_WRITER);
#endif
#if defined(CHUCHO_HAVE_RDKAFKA)
    EXPECT_FEATURE(chucho::optional_features::KAFKA_WRITER);
#endif
    EXPECT_TRUE(fs.none());
}
