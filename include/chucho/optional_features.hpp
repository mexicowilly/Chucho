/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_OPTIONAL_FEATURES_HPP__)
#define CHUCHO_OPTIONAL_FEATURES_HPP__

/**
 * @file optional_features.hpp
 * Functions for querying the optional features that were
 * enabled for this build of Chucho.
 * 
 * @ingroup miscellaneous
 */

#include <chucho/export.h>
#include <bitset>

namespace chucho
{

/**
 * The optional feature namespace
 */
namespace optional_features
{

/**
 * Features that may or may not be present. Please refer to the file
 * @c cmake/BuildOptions.cmake for information about how to enable or disable
 * optional features.
 */
enum feature
{
    ACTIVEMQ_WRITER,            /**< ActiveMQ */
    BZIP2_COMPRESSION,          /**< Bzip2 */
    C_API,                      /**< C API */
    CAPN_PROTO_SERIALIZER,      /**< Cap'n Proto serialization */
    DB2_WRITER,                 /**< DB2 */
    CONFIG_FILE_CONFIGURATION,  /**< Config file format configuration */
    EMAIL_WRITER,               /**< Whether CURL is present */
    LOG4CPLUS_CONFIGURATION,    /**< Log4cplus config file format configuration */
    MINIZIP_COMPRESSION,        /**< Minizip, which is used for zip file format compression. Version 1.11 and later will never have this option. */
    MYSQL_WRITER,               /**< MySQL */
    ORACLE_WRITER,              /**< Oracle */
    POSTGRESQL_WRITER,          /**< PostgreSQL */
    PROTOBUF_SERIALIZER,        /**< Protobuf serialization */
    RABBITMQ_WRITER,            /**< RabbitMQ */
    RUBY_EVALUATOR_FILTER,      /**< Ruby */
    SQLITE_WRITER,              /**< SQLite */
    YAML_CONFIGURATION,         /**< YAML config file format configuration */
    ZEROMQ_WRITER,              /**< ZeroMQ */
    ZLIB_COMPRESSION,           /**< Zlib */
    LIBARCHIVE_COMPRESSION,     /**< Libarchive, which is used for zip file format compression after Version 1.11 */
    LZMA_COMPRESSION,           /**< LZMA compression */
    LZ4_COMPRESSION,            /**< LZ4 compression */
    FLATBUFFERS_SERIALIZER,     /**< Flatbuffers serialization */
    LOGGLY_WRITER,              /**< Loggly writer */
    CLOUDWATCH_WRITER,          /**< AWS Cloudwatch writer */
    DATABASE_WRITER,            /**< Database writer */
    KAFKA_WRITER,               /**< Kafka writer */
    FEATURE_COUNT               /**< Do not use. This is just so I can know how many there are. */
};

/**
 * A set of optional Chucho features.
 */
typedef std::bitset<FEATURE_COUNT> feature_set;

/**
 * Return the current feature set. Bits may be queried using the @c test
 * method with a value from the @ref feature enum as an argument.
 * 
 * @return the current set of features
 */
CHUCHO_EXPORT const feature_set& get();
/**
 * Test whether a single feature is present.
 * @param feat the feature to test
 * @return true if that feature is present, false otherwise
 */
inline bool have(feature feat)
{
    return get().test(feat);
}

}

}

#endif
