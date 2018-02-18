#
# Copyright 2013-2017 Will Mason
# 
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.3)
IF(POLICY CMP0042)
    CMAKE_POLICY(SET CMP0042 OLD)
ENDIF()

PROJECT(chucho CXX C)

SET(CHUCHO_MAJOR_VERSION 1)
SET(CHUCHO_MINOR_VERSION 12)
SET(CHUCHO_VERSION ${CHUCHO_MAJOR_VERSION}.${CHUCHO_MINOR_VERSION})

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

INCLUDE(BuildOptions)
INCLUDE(Configure)

# We can release stuff
SET(CPACK_PACKAGE_VERSION_MAJOR ${CHUCHO_MAJOR_VERSION})
SET(CPACK_PACKAGE_VERSION_MINOR ${CHUCHO_MINOR_VERSION})
SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CHUCHO_VERSION})
SET(CPACK_SOURCE_IGNORE_FILES "build;\.git;\.idea;^.*\.swp$;${CPACK_SOURCE_IGNORE_FILES}")
SET(CPACK_SOURCE_GENERATOR TGZ ZIP)
INCLUDE(CPack)

# Include CMAKE_BINARY_DIR because that's where the generated file
# export.h lives.
INCLUDE_DIRECTORIES(include
                    embedded/include
                    "${CMAKE_BINARY_DIR}")
# Don't just add these even they are empty, because
# if you put the directory in quotes, CMake gets mad
# if the value is empty.
IF(CHUCHO_HAVE_ORACLE)
    INCLUDE_DIRECTORIES("${ORACLE_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_MYSQL)
    INCLUDE_DIRECTORIES("${MYSQL_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_SQLITE)
    INCLUDE_DIRECTORIES("${SQLITE_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_POSTGRES)
    INCLUDE_DIRECTORIES("${POSTGRES_INCLUDE_DIR}")
ENDIF()
IF(CURL_FOUND)
    INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})
ENDIF()
IF(PROTOBUF_FOUND)
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
ENDIF()
IF(CHUCHO_HAVE_CAPN_PROTO)
    INCLUDE_DIRECTORIES("${CAPN_PROTO_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_FLATBUFFERS)
    INCLUDE_DIRECTORIES("${FLATBUFFERS_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_ZEROMQ)
    INCLUDE_DIRECTORIES("${ZEROMQ_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_ACTIVEMQ)
    INCLUDE_DIRECTORIES("${ACTIVEMQ_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_DB2)
    INCLUDE_DIRECTORIES("${DB2_INCLUDE_DIR}")
ENDIF()
IF(CHUCHO_HAVE_RABBITMQ)
    INCLUDE_DIRECTORIES("${RABBITMQ_INCLUDE_DIR}")
ENDIF()
IF(RUBY_FOUND)
    INCLUDE_DIRECTORIES("${RUBY_INCLUDE_DIRS}")
ENDIF()
IF(ZLIB_FOUND)
    INCLUDE_DIRECTORIES("${ZLIB_INCLUDE_DIRS}")
ENDIF()
IF(BZIP2_FOUND)
    INCLUDE_DIRECTORIES("${BZIP2_INCLUDE_DIR}")
ENDIF()
IF(LIBLZMA_FOUND)
    INCLUDE_DIRECTORIES("${LIBLZMA_INCLUDE_DIRS}")
ENDIF()
IF(LibArchive_FOUND)
    INCLUDE_DIRECTORIES("${LibArchive_INCLUDE_DIRS}")
ENDIF()
IF(CHUCHO_HAVE_LZ4)
    INCLUDE_DIRECTORIES("${LZ4_INCLUDE_DIR}")
ENDIF()

IF(CHUCHO_POSIX)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/posix/calendar_posix.cpp
        platform/posix/cerr_writer_posix.cpp
        platform/posix/cout_writer_posix.cpp
        platform/posix/diagnostic_context_posix.cpp
        platform/posix/environment_posix.cpp
        platform/posix/file_descriptor_writer_posix.cpp
        platform/posix/file_posix.cpp
        platform/posix/file_writer_posix.cpp
        platform/posix/host_posix.cpp
        platform/posix/line_ending_posix.cpp
        platform/posix/named_pipe_writer_posix.cpp
        platform/posix/pipe_writer_posix.cpp
        platform/posix/process_posix.cpp
        platform/posix/syslog_writer_posix.cpp
        platform/posix/text_util_posix.cpp)

    IF(CHUCHO_HAVE_GMTIME_R)
        SET(CHUCHO_CALENDAR_DEFS CHUCHO_HAVE_GMTIME_R)
    ENDIF()
    IF(CHUCHO_HAVE_LOCALTIME_R)
        LIST(APPEND CHUCHO_CALENDAR_DEFS CHUCHO_HAVE_LOCALTIME_R)
    ENDIF()
    IF(CHUCHO_HAVE_TIMEGM)
        LIST(APPEND CHUCHO_CALENDAR_DEFS CHUCHO_HAVE_TIMEGM)
    ENDIF()
    IF(CHUCHO_HAVE_TM_GMTOFF)
        LIST(APPEND CHUCHO_CALENDAR_DEFS CHUCHO_HAVE_TM_GMTOFF)
    ENDIF()
    IF(CHUCHO_HAVE_TIMEZONE)
        LIST(APPEND CHUCHO_CALENDAR_DEFS CHUCHO_HAVE_TIMEZONE)
    ENDIF()
    IF(CHUCHO_CALENDAR_DEFS)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/calendar_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS "${CHUCHO_CALENDAR_DEFS}")
    ENDIF()
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/file_posix.cpp PROPERTIES
                                    COMPILE_FLAGS "-Wno-unused-result")
    ENDIF()
    # CHUCHO_POSIX_FILE_DEFS might already be set from the check for large
    # file support flags in Configure.cmake
    IF(CHUCHO_NO_FTS)
        LIST(APPEND CHUCHO_POSIX_FILE_DEFS CHUCHO_NO_FTS)
    ENDIF()
    IF(CHUCHO_DIRENT_NEEDS_NAME)
        LIST(APPEND CHUCHO_POSIX_FILE_DEFS CHUCHO_DIRENT_NEEDS_NAME)
    ENDIF()
    IF(CHUCHO_POSIX_FILE_DEFS)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/file_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS "${CHUCHO_POSIX_FILE_DEFS}")
    ENDIF()
    IF(CHUCHO_HAVE_O_LARGEFILE)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/file_writer_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS CHUCHO_HAVE_O_LARGEFILE)
    ENDIF()
ELSEIF(CHUCHO_WINDOWS)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/windows/calendar_windows.cpp
        platform/windows/cerr_writer_windows.cpp
        platform/windows/cout_writer_windows.cpp
        platform/windows/diagnostic_context_windows.cpp
        platform/windows/environment_windows.cpp
        platform/windows/error_util.cpp
        platform/windows/error_util.hpp
        platform/windows/file_descriptor_writer_windows.cpp
        platform/windows/file_windows.cpp
        platform/windows/file_writer_windows.cpp
        platform/windows/host_windows.cpp
        platform/windows/line_ending_windows.cpp
        platform/windows/named_pipe_writer_windows.cpp
        platform/windows/pipe_writer_windows.cpp
        platform/windows/syslog_writer_windows.cpp
        platform/windows/text_util_windows.cpp
        platform/windows/windows_event_log_writer.cpp
        platform/windows/windows_event_log_writer_factory.cpp
        include/chucho/windows_event_log_writer_factory.hpp
        platform/windows/windows_event_log_writer_memento.cpp
        include/chucho/windows_event_log_writer_memento.hpp
        platform/windows/winsock_startup.cpp
        platform/windows/winsock_startup.hpp)
    SET_SOURCE_FILES_PROPERTIES(platform/windows/windows_event_log_writer.cpp PROPERTIES
                                COMPILE_DEFINITIONS CMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
ENDIF()

SET(CHUCHO_PUBLIC_HEADERS
    include/chucho/async_writer.hpp
    include/chucho/cerr_writer.hpp
    include/chucho/compressor.hpp
    include/chucho/configurable.hpp
    include/chucho/configurable_factory.hpp
    include/chucho/configuration.hpp
    include/chucho/configuration.h
    include/chucho/configurator.hpp
    include/chucho/cout_writer.hpp
    include/chucho/database_writer.hpp
    include/chucho/diagnostic_context.hpp
    include/chucho/duplicate_message_filter.hpp
    include/chucho/evaluator_filter.hpp
    include/chucho/event.hpp
    include/chucho/event_cache_stats.hpp
    include/chucho/exception.hpp
    "${CMAKE_BINARY_DIR}/chucho/export.h"
    include/chucho/file_compressor.hpp
    include/chucho/file_compressor_factory.hpp
    include/chucho/file_compressor_memento.hpp
    include/chucho/file_descriptor_writer.hpp
    include/chucho/file_roller.hpp
    include/chucho/file_roller_memento.hpp
    include/chucho/file_roll_trigger.hpp
    include/chucho/file_writer.hpp
    include/chucho/filter.hpp
    include/chucho/filter_memento.hpp
    include/chucho/finalize.hpp
    include/chucho/finalize.h
    include/chucho/formatted_message_serializer.hpp
    include/chucho/formatter.hpp
    include/chucho/function_name.hpp
    include/chucho/interval_file_roll_trigger.hpp
    include/chucho/level.hpp
    include/chucho/level_filter.hpp
    include/chucho/level_threshold_filter.hpp
    include/chucho/log_stream.hpp
    include/chucho/log_streambuf.hpp
    include/chucho/log.hpp
    include/chucho/log.h
    include/chucho/loggable.hpp
    include/chucho/logger.hpp
    include/chucho/marker.hpp
    include/chucho/memento.hpp
    include/chucho/message_queue_writer.hpp
    include/chucho/nameable_memento.hpp
    include/chucho/named_pipe_writer.hpp
    include/chucho/non_copyable.hpp
    include/chucho/noop_compressor.hpp
    include/chucho/noop_file_compressor.hpp
    include/chucho/numbered_file_roller.hpp
    include/chucho/optional.hpp
    include/chucho/optional_features.hpp
    include/chucho/pattern_formatter.hpp
    include/chucho/pipe_writer.hpp
    include/chucho/rolling_file_writer.hpp
    include/chucho/ruby_evaluator_filter.hpp
    include/chucho/security_policy.hpp
    include/chucho/serializer.hpp
    include/chucho/size_file_roll_trigger.hpp
    include/chucho/sliding_numbered_file_roller.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/streamable.hpp
    include/chucho/syslog_constants.hpp
    include/chucho/syslog_writer.hpp
    include/chucho/time_file_roller.hpp
    "${CMAKE_BINARY_DIR}/chucho/version.hpp"
    include/chucho/writer.hpp
    include/chucho/writer_factory.hpp
    include/chucho/writer_memento.hpp)

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS ${CHUCHO_PUBLIC_HEADERS})

IF(CHUCHO_WINDOWS)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/windows_event_log_writer.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/windows_event_log_writer.hpp)

IF(ZLIB_FOUND)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/gzip_file_compressor.hpp
         include/chucho/zlib_compressor.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/gzip_file_compressor.hpp
     include/chucho/zlib_compressor.hpp)

IF(BZIP2_FOUND)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/bzip2_compressor.hpp
         include/chucho/bzip2_file_compressor.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/bzip2_compressor.hpp
     include/chucho/bzip2_file_compressor.hpp)

IF(LibArchive_FOUND)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/zip_file_compressor.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/zip_file_compressor.hpp)

IF(LIBLZMA_FOUND)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/lzma_compressor.hpp
         include/chucho/lzma_file_compressor.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/lzma_compressor.hpp
     include/chucho/lzma_file_compressor.hpp)

IF(CHUCHO_HAVE_LZ4)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/lz4_compressor.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/lz4_compressor.hpp)

IF(CHUCHO_HAVE_ORACLE)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/oracle_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         oracle_writer.cpp
         oracle_writer_factory.cpp
         oracle_writer_memento.cpp
         include/chucho/oracle_writer_factory.hpp
         include/chucho/oracle_writer_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/oracle_writer.hpp)

IF(CHUCHO_HAVE_MYSQL)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/mysql_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         mysql_writer.cpp
         mysql_writer_factory.cpp
         mysql_writer_memento.cpp
         include/chucho/mysql_writer_factory.hpp
         include/chucho/mysql_writer_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/mysql_writer.hpp)

IF(CHUCHO_HAVE_SQLITE)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/sqlite_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         sqlite_writer.cpp
         sqlite_writer_factory.cpp
         sqlite_writer_memento.cpp
         include/chucho/sqlite_writer_factory.hpp
         include/chucho/sqlite_writer_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/sqlite_writer.hpp)

IF(CHUCHO_HAVE_POSTGRES)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/postgres_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         postgres_writer.cpp
         postgres_writer_factory.cpp
         postgres_writer_memento.cpp
         include/chucho/postgres_writer_factory.hpp
         include/chucho/postgres_writer_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/postgres_writer.hpp)

IF(CHUCHO_HAVE_DB2)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/db2_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         db2_writer.cpp
         db2_writer_factory.cpp
         db2_writer_memento.cpp
         include/chucho/db2_writer_factory.hpp
         include/chucho/db2_writer_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/db2_writer.hpp)

IF(RUBY_FOUND)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/ruby_evaluator_filter.hpp)
    LIST(APPEND CHUCHO_EVALUATOR_SOURCES
         ruby_evaluator_filter.cpp
         ruby_evaluator_filter_factory.cpp
         ruby_evaluator_filter_memento.cpp
         include/chucho/ruby_evaluator_filter_factory.hpp
         include/chucho/ruby_evaluator_filter_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/ruby_evaluator_filter.hpp)

IF(CURL_FOUND)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/email_trigger.hpp
         include/chucho/email_writer.hpp
         include/chucho/level_threshold_email_trigger.hpp)
    LIST(APPEND CHUCHO_EMBEDDED_SOURCES
         "${CMAKE_SOURCE_DIR}/embedded/fnv/hash_64a.c")
    LIST(APPEND CHUCHO_EMAIL_SOURCES
         email_writer.cpp
         email_writer_factory.cpp
         email_writer_memento.cpp
         level_threshold_email_trigger.cpp
         level_threshold_email_trigger_factory.cpp
         level_threshold_email_trigger_memento.cpp
         include/chucho/email_writer_factory.hpp
         include/chucho/email_writer_memento.hpp
         include/chucho/level_threshold_email_trigger_factory.hpp
         include/chucho/level_threshold_email_trigger_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/email_trigger.hpp
     include/chucho/email_writer.hpp
     include/chucho/level_threshold_email_trigger.hpp)

IF(PROTOBUF_FOUND)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/protobuf_serializer.hpp)
    LIST(APPEND CHUCHO_SERIALIZER_SOURCES
         protobuf_serializer.cpp
         protobuf_serializer_factory.cpp
         include/chucho/protobuf_serializer_factory.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/protobuf_serializer.hpp)

IF(CHUCHO_HAVE_CAPN_PROTO)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/capn_proto_serializer.hpp)
    LIST(APPEND CHUCHO_SERIALIZER_SOURCES
         capn_proto_serializer.cpp
         capn_proto_serializer_factory.cpp
         include/chucho/capn_proto_serializer_factory.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/capn_proto_serializer.hpp)

IF(CHUCHO_HAVE_FLATBUFFERS)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/flatbuffers_serializer.hpp)
    LIST(APPEND CHUCHO_SERIALIZER_SOURCES
         flatbuffers_serializer.cpp
         flatbuffers_serializer_factory.cpp
         include/chucho/flatbuffers_serializer_factory.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/flatbuffers_serializer.hpp)

IF(CHUCHO_HAVE_ZEROMQ)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/zeromq_writer.hpp)
    LIST(APPEND CHUCHO_MESSAGE_QUEUE_SOURCES
         zeromq_writer.cpp
         zeromq_writer_factory.cpp
         include/chucho/zeromq_writer_factory.hpp
         zeromq_writer_memento.cpp
         include/chucho/zeromq_writer_memento.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/zeromq_writer.hpp)

IF(CHUCHO_HAVE_ACTIVEMQ)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/activemq_writer.hpp
         include/chucho/activemq_writer_factory.hpp
         include/chucho/activemq_writer_memento.hpp)
    LIST(APPEND CHUCHO_MESSAGE_QUEUE_SOURCES
         activemq_writer.cpp
         activemq_writer_factory.cpp
         activemq_writer_memento.cpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/activemq_writer.hpp)

IF(CHUCHO_HAVE_RABBITMQ)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/rabbitmq_writer.hpp
         include/chucho/rabbitmq_writer_factory.hpp
         include/chucho/rabbitmq_writer_memento.hpp)
    LIST(APPEND CHUCHO_MESSAGE_QUEUE_SOURCES
         rabbitmq_writer.cpp
         rabbitmq_writer_factory.cpp
         rabbitmq_writer_memento.cpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/rabbitmq_writer.hpp)

IF(CHUCHO_HAVE_DOORS)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/door_event.h
         include/chucho/door_writer.hpp
         include/chucho/door_writer_factory.hpp
         include/chucho/door_writer_memento.hpp)
    LIST(APPEND CHUCHO_DOOR_SOURCES
         platform/solaris/door_writer.cpp
         platform/solaris/door_writer_factory.cpp
         platform/solaris/door_writer_memento.cpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/door_event.h
     include/chucho/door_writer.hpp)

# The libyaml sources are required by chuchod
MACRO(CHUCHO_SET_YAML_SOURCES CHUCHO_YAML_SOURCE_VAR)
    # Update this when you rev libyaml
    SET(CHUCHO_LIBYAML_VERSION_MAJOR 0)
    SET(CHUCHO_LIBYAML_VERSION_MINOR 1)
    SET(CHUCHO_LIBYAML_VERSION_PATCH 7)

    LIST(APPEND ${CHUCHO_YAML_SOURCE_VAR}
        "${CMAKE_SOURCE_DIR}/embedded/libyaml/api.c"
        "${CMAKE_SOURCE_DIR}/embedded/libyaml/loader.c"
        "${CMAKE_SOURCE_DIR}/embedded/libyaml/parser.c"
        "${CMAKE_SOURCE_DIR}/embedded/libyaml/reader.c"
        "${CMAKE_SOURCE_DIR}/embedded/libyaml/scanner.c")

    SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/embedded/libyaml/api.c" PROPERTIES
                                COMPILE_DEFINITIONS "YAML_VERSION_MAJOR=${CHUCHO_LIBYAML_VERSION_MAJOR};YAML_VERSION_MINOR=${CHUCHO_LIBYAML_VERSION_MINOR};YAML_VERSION_PATCH=${CHUCHO_LIBYAML_VERSION_PATCH};YAML_VERSION_STRING=\"${CHUCHO_LIB_YAML_VERSION_MAJOR}.${CHUCHO_LIBYAML_VERSION_MINOR}.${CHUCHO_LIBYAML_VERSION_PATCH}\";YAML_DECLARE_STATIC")
    SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/embedded/libyaml/loader.c"
                                "${CMAKE_SOURCE_DIR}/embedded/libyaml/parser.c"
                                "${CMAKE_SOURCE_DIR}/embedded/libyaml/reader.c"
                                "${CMAKE_SOURCE_DIR}/embedded/libyaml/scanner.c"
                                PROPERTIES
                                COMPILE_DEFINITIONS YAML_DECLARE_STATIC)
    IF(MSVC)
        SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/embedded/libyaml/loader.c"
                                    "${CMAKE_SOURCE_DIR}/embedded/libyaml/scanner.c"
                                    "${CMAKE_SOURCE_DIR}/embedded/libyaml/api.c"
                                    PROPERTIES
                                    COMPILE_FLAGS "/wd4996 /wd4267 /wd4244")
    ELSEIF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER_ID STREQUAL Clang)
        SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/embedded/libyaml/api.c" PROPERTIES
                                    COMPILE_FLAGS "-Wno-implicit-function-declaration -Wno-int-to-pointer-cast")
    ENDIF()
ENDMACRO()

# Configurator settings
IF(YAML_CONFIG)
    CHUCHO_SET_YAML_SOURCES(CHUCHO_EMBEDDED_SOURCES)
    LIST(APPEND CHUCHO_CONFIGURATOR_SOURCES
         yaml_configurator.cpp
         yaml_parser.cpp
         include/chucho/utf8.hpp
         include/chucho/yaml_configurator.hpp
         include/chucho/yaml_parser.hpp)
    SET_SOURCE_FILES_PROPERTIES(configuration.cpp PROPERTIES
                                COMPILE_DEFINITIONS "CHUCHO_YAML_CONFIG;YAML_DECLARE_STATIC")
    SET_SOURCE_FILES_PROPERTIES(yaml_parser.cpp PROPERTIES
                                COMPILE_DEFINITIONS YAML_DECLARE_STATIC)
    MESSAGE(STATUS "Chucho has YAML configuration parsing enabled")
ENDIF()

IF(CONFIG_FILE_CONFIG OR LOG4CPLUS_CONFIG)
    LIST(APPEND CHUCHO_CONFIGURATOR_SOURCES
         config_file_configurator.cpp
         properties.cpp
         include/chucho/config_file_configurator.hpp
         include/chucho/properties.hpp)
    GET_SOURCE_FILE_PROPERTY(CHUCHO_CONFIGURATION_DEFS configuration.cpp COMPILE_DEFINITIONS)
    SET_SOURCE_FILES_PROPERTIES(configuration.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_CONFIGURATION_DEFS};CHUCHO_CONFIG_FILE_CONFIG")
    IF(CONFIG_FILE_CONFIG)
        SET(CHUCHO_CONFIGURATOR_DEFS CHUCHO_CONFIG_FILE)
        MESSAGE(STATUS "Chucho has config file configuration parsing enabled")
    ENDIF()
    IF(LOG4CPLUS_CONFIG)
        LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_LOG4CPLUS_FILE)
        MESSAGE(STATUS "Chucho has log4cplus configuration parsing enabled")
    ENDIF()
    IF(CURL_FOUND)
        LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_EMAIL_WRITER)
    ENDIF()
    SET_SOURCE_FILES_PROPERTIES(config_file_configurator.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_CONFIGURATOR_DEFS}")
    SET_SOURCE_FILES_PROPERTIES(memento_key_set.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_CONFIG_FILE_CONFIG)
ENDIF()

IF(JSON_CONFIG)
    LIST(APPEND CHUCHO_EMBEDDED_SOURCES
         "${CMAKE_SOURCE_DIR}/embedded/cJSON/cJSON.c"
         "${CMAKE_SOURCE_DIR}/embedded/include/cJSON.h")
    LIST(APPEND CHUCHO_CONFIGURATOR_SOURCES
        json_configurator.cpp
        include/chucho/json_configurator.hpp)
    GET_SOURCE_FILE_PROPERTY(CHUCHO_CONFIGURATION_DEFS configuration.cpp COMPILE_DEFINITIONS)
    SET_SOURCE_FILES_PROPERTIES(configuration.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_CONFIGURATION_DEFS};CHUCHO_JSON_CONFIG")
    MESSAGE(STATUS "Chucho has JSON configuration parsing enabled")
ENDIF()

IF(MSVC)
    SET_SOURCE_FILES_PROPERTIES(c_diagnostic_context.cpp PROPERTIES
                                COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    SET_SOURCE_FILES_PROPERTIES(c_file_roller.cpp PROPERTIES
                                COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
ENDIF()

IF(PROTOBUF_FOUND)
    PROTOBUF_GENERATE_CPP(CHUCHO_PROTO_SOURCE CHUCHO_PROTO_HEADER serialization/chucho.proto)
    LIST(APPEND CHUCHO_MQ_GEN_SOURCES "${CHUCHO_PROTO_HEADER}" "${CHUCHO_PROTO_SOURCE}")
ENDIF()

IF(CHUCHO_HAVE_CAPN_PROTO)
    SET(CHUCHO_CAPN_PROTO_SOURCES "${CMAKE_BINARY_DIR}/chucho.capnp.c++" "${CMAKE_BINARY_DIR}/chucho.capnp.h")
    ADD_CUSTOM_COMMAND(OUTPUT ${CHUCHO_CAPN_PROTO_SOURCES}
                       COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/serialization/chucho.capnp" "${CMAKE_BINARY_DIR}"
                       COMMAND "${CHUCHO_CAPNP}" compile "--output=${CHUCHO_CAPNPC_CXX}" "${CMAKE_BINARY_DIR}/chucho.capnp"
                       DEPENDS "${CMAKE_SOURCE_DIR}/serialization/chucho.capnp")
    LIST(APPEND CHUCHO_MQ_GEN_SOURCES ${CHUCHO_CAPN_PROTO_SOURCES})
    SET_SOURCE_FILES_PROPERTIES(${CHUCHO_CAPN_PROTO_SOURCES} PROPERTIES GENERATED TRUE)
ENDIF()

IF(CHUCHO_HAVE_FLATBUFFERS)
    ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_BINARY_DIR}/chucho_generated.h"
                       COMMAND "${CHUCHO_FLATC}" --cpp -o "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/serialization/chucho.fbs"
                       DEPENDS "${CMAKE_SOURCE_DIR}/serialization/chucho.fbs")
    LIST(APPEND CHUCHO_MQ_GEN_SOURCES "${CMAKE_BINARY_DIR}/chucho_generated.h")
    SET_SOURCE_FILES_PROPERTIES("${CMAKE_BINARY_DIR}/chucho_generated.h" PROPERTIES GENERATED TRUE)
ENDIF()

SET(CHUCHO_SOURCES
    async_writer.cpp
    async_writer_factory.cpp
    async_writer_memento.cpp
    bzip2_file_compressor_factory.cpp
    calendar.cpp
    cerr_writer_factory.cpp
    configurable.cpp
    configurable_factory.cpp
    configuration.cpp
    c_configuration.cpp
    c_finalize.cpp
    c_log.cpp
    configurator.cpp
    cout_writer_factory.cpp
    demangle.cpp
    database_writer.cpp
    diagnostic_context.cpp
    duplicate_message_filter.cpp
    duplicate_message_filter_factory.cpp
    event.cpp
    event_cache.cpp
    exception.cpp
    file_compressor_factory.cpp
    file_compressor_memento.cpp
    file_descriptor_writer.cpp
    file_exception.cpp
    file_roller.cpp
    file_roller_memento.cpp
    file_writer.cpp
    file_writer_factory.cpp
    file_writer_memento.cpp
    filter_memento.cpp
    finalize.cpp
    formatted_message_serializer.cpp
    formatted_message_serializer_factory.cpp
    garbage_cleaner.cpp
    gzip_file_compressor_factory.cpp
    host.cpp
    interval_file_roll_trigger.cpp
    interval_file_roll_trigger_factory.cpp
    interval_file_roll_trigger_memento.cpp
    level.cpp
    level_filter.cpp
    level_filter_factory.cpp
    level_filter_memento.cpp
    level_threshold_filter.cpp
    level_threshold_filter_factory.cpp
    level_threshold_filter_memento.cpp
    log_stream.cpp
    log_streambuf.cpp
    logger.cpp
    logger_factory.cpp
    logger_memento.cpp
    lzma_file_compressor_factory.cpp
    marker.cpp
    memento.cpp
    memento_key_set.cpp
    message_queue_writer.cpp
    message_queue_writer_memento.cpp
    nameable_memento.cpp
    named_pipe_writer.cpp
    named_pipe_writer_factory.cpp
    named_pipe_writer_memento.cpp
    noop_compressor.cpp
    noop_compressor_factory.cpp
    noop_file_compressor.cpp
    numbered_file_roller.cpp
    numbered_file_roller_factory.cpp
    numbered_file_roller_memento.cpp
    optional_features.cpp
    pattern_formatter.cpp
    pattern_formatter_factory.cpp
    pattern_formatter_memento.cpp
    pipe_writer_factory.cpp
    pipe_writer_memento.cpp
    regex.cpp
    regex_exception.cpp
    rolling_file_writer.cpp
    rolling_file_writer_factory.cpp
    rolling_file_writer_memento.cpp
    security_policy.cpp
    size_file_roll_trigger.cpp
    size_file_roll_trigger_factory.cpp
    size_file_roll_trigger_memento.cpp
    sliding_numbered_file_roller.cpp
    sliding_numbered_file_roller_factory.cpp
    sliding_numbered_file_roller_memento.cpp
    status.cpp
    status_manager.cpp
    status_observer.cpp
    status_reporter.cpp
    syslog_constants.cpp
    syslog_writer.cpp
    syslog_writer_factory.cpp
    syslog_writer_memento.cpp
    text_util.cpp
    time_file_roller.cpp
    time_file_roller_factory.cpp
    time_file_roller_memento.cpp
    time_util.cpp
    utf8.cpp
    writer.cpp
    writer_factory.cpp
    writer_memento.cpp
    zip_file_compressor_factory.cpp
    include/chucho/async_writer_factory.hpp
    include/chucho/async_writer_memento.hpp
    include/chucho/bzip2_file_compressor_factory.hpp
    include/chucho/calendar.hpp
    include/chucho/cerr_writer_factory.hpp
    include/chucho/configurator.hpp
    include/chucho/cout_writer_factory.hpp
    include/chucho/demangle.hpp
    include/chucho/duplicate_message_filter_factory.hpp
    include/chucho/environment.hpp
    include/chucho/event_cache.hpp
    include/chucho/exception.hpp
    include/chucho/file.hpp
    include/chucho/file_exception.hpp
    include/chucho/file_writer_factory.hpp
    include/chucho/file_writer_memento.hpp
    include/chucho/formatted_message_serializer_factory.hpp
    include/chucho/garbage_cleaner.hpp
    include/chucho/gzip_file_compressor_factory.hpp
    include/chucho/host.hpp
    include/chucho/interval_file_roll_trigger_factory.hpp
    include/chucho/interval_file_roll_trigger_memento.hpp
    include/chucho/level_filter_factory.hpp
    include/chucho/level_filter_memento.hpp
    include/chucho/level_threshold_filter_factory.hpp
    include/chucho/level_threshold_filter_memento.hpp
    include/chucho/line_ending.hpp
    include/chucho/logger_factory.hpp
    include/chucho/logger_memento.hpp
    include/chucho/lzma_file_compressor_factory.hpp
    include/chucho/memento_key_set.hpp
    include/chucho/message_queue_writer_memento.hpp
    include/chucho/move_util.hpp
    include/chucho/named_pipe_writer_factory.hpp
    include/chucho/named_pipe_writer_memento.hpp
    include/chucho/noop_compressor_factory.hpp
    include/chucho/numbered_file_roller_factory.hpp
    include/chucho/numbered_file_roller_memento.hpp
    include/chucho/pattern_formatter_factory.hpp
    include/chucho/pattern_formatter_memento.hpp
    include/chucho/pipe_writer_factory.hpp
    include/chucho/pipe_writer_memento.hpp
    include/chucho/process.hpp
    include/chucho/regex.hpp
    include/chucho/regex_exception.hpp
    include/chucho/rolling_file_writer_factory.hpp
    include/chucho/rolling_file_writer_memento.hpp
    include/chucho/size_file_roll_trigger_factory.hpp
    include/chucho/size_file_roll_trigger_memento.hpp
    include/chucho/sliding_numbered_file_roller_factory.hpp
    include/chucho/sliding_numbered_file_roller_memento.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/syslog_writer_factory.hpp
    include/chucho/syslog_writer_memento.hpp
    include/chucho/text_util.hpp
    include/chucho/time_file_roller_factory.hpp
    include/chucho/time_file_roller_memento.hpp
    include/chucho/time_util.hpp
    include/chucho/utf8.hpp
    include/chucho/zip_file_compressor_factory.hpp
    ${CHUCHO_CONFIGURATOR_SOURCES}
    ${CHUCHO_PLATFORM_SOURCES}
    ${CHUCHO_EMBEDDED_SOURCES}
    ${CHUCHO_PUBLIC_HEADERS}
    ${CHUCHO_C_SOURCES}
    ${CHUCHO_DATABASE_SOURCES}
    ${CHUCHO_EVALUATOR_SOURCES}
    ${CHUCHO_EMAIL_SOURCES}
    ${CHUCHO_SERIALIZER_SOURCES}
    ${CHUCHO_MESSAGE_QUEUE_SOURCES}
    ${CHUCHO_DOOR_SOURCES}
    ${CHUCHO_MQ_GEN_SOURCES})

IF(CHUCHO_HAVE_PUT_TIME)
    SET_SOURCE_FILES_PROPERTIES(calendar.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_PUT_TIME)
ENDIF()

IF(CHUCHO_WINDOWS)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_WINDOWS)
    SET_SOURCE_FILES_PROPERTIES(configurator.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_WINDOWS)
    SET_SOURCE_FILES_PROPERTIES(file_descriptor_writer.cpp PROPERTIES
                                COMPILE_DEFINITIONS "NOMINMAX;_SCL_SECURE_NO_WARNINGS")
    SET_SOURCE_FILES_PROPERTIES(sliding_numbered_file_roller.cpp
                                time_file_roller.cpp
                                PROPERTIES
                                COMPILE_DEFINITIONS NOMINMAX)
ENDIF()

IF(CHUCHO_HAVE_MYSQL)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_MYSQL)
ENDIF()

IF(CHUCHO_HAVE_ORACLE)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_ORACLE)
ENDIF()

IF(CHUCHO_HAVE_SQLITE)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_SQLITE)
ENDIF()

IF(CHUCHO_HAVE_POSTGRES)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_POSTGRES)
ENDIF()

IF(CHUCHO_HAVE_DB2)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_DB2)
ENDIF()

IF(RUBY_FOUND)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_RUBY)
ENDIF()

IF(CURL_FOUND)
    LIST(FIND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_EMAIL_WRITER CHUCHO_HEW_INDEX)
    IF(CHUCHO_HEW_INDEX EQUAL -1)
        LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_EMAIL_WRITER)
    ENDIF()
ENDIF()

IF(PROTOBUF_FOUND)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_PROTOBUF)
ENDIF()

IF(CHUCHO_HAVE_CAPN_PROTO)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_CAPN_PROTO)
ENDIF()

IF(CHUCHO_HAVE_FLATBUFFERS)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_FLATBUFFERS)
ENDIF()

IF(CHUCHO_HAVE_ZEROMQ)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_ZEROMQ)
ENDIF()

IF(CHUCHO_HAVE_ACTIVEMQ)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_ACTIVEMQ)
ENDIF()

IF(CHUCHO_HAVE_RABBITMQ)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_RABBITMQ)
ENDIF()

IF(ZLIB_FOUND)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_ZLIB)
ENDIF()

IF(BZIP2_FOUND)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_BZIP2)
ENDIF()

IF(LIBLZMA_FOUND)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_LZMA)
ENDIF()

IF(CHUCHO_HAVE_LZ4)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_LZ4)
ENDIF()

IF(CHUCHO_HAVE_DOORS)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_DOORS)
ENDIF()

IF(DEFINED CHUCHO_CONFIGURATOR_DEFS)
    SET_SOURCE_FILES_PROPERTIES(configurator.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_CONFIGURATOR_DEFS}")
ENDIF()

# Feature definitions
IF(YAML_CONFIG)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_YAML_CONFIG)
ENDIF()
IF(CONFIG_FILE_CONFIG)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_CONFIG_FILE_CONFIG)
ENDIF()
IF(LOG4CPLUS_CONFIG)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_LOG4CPLUS_CONFIG)
ENDIF()
IF(CURL_FOUND)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_EMAIL_WRITER)
ENDIF()
IF(CHUCHO_HAVE_DB2)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_DB2_WRITER)
ENDIF()
IF(CHUCHO_HAVE_MYSQL)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_MYSQL_WRITER)
ENDIF()
IF(CHUCHO_HAVE_ORACLE)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_ORACLE_WRITER)
ENDIF()
IF(CHUCHO_HAVE_POSTGRES)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_POSTGRESQL_WRITER)
ENDIF()
IF(CHUCHO_HAVE_SQLITE)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_SQLITE_WRITER)
ENDIF()
IF(RUBY_FOUND)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_RUBY_EVALUATOR_FILTER)
ENDIF()
IF(CHUCHO_HAVE_ACTIVEMQ)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_ACTIVEMQ_WRITER)
ENDIF()
IF(CHUCHO_HAVE_ZEROMQ)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_ZEROMQ_WRITER)
ENDIF()
IF(CHUCHO_HAVE_RABBITMQ)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_RABBITMQ_WRITER)
ENDIF()
IF(PROTOBUF_FOUND)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_PROTOBUF_SERIALIZER)
ENDIF()
IF(CHUCHO_HAVE_CAPN_PROTO)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_CAPN_PROTO_SERIALIZER)
ENDIF()
IF(CHUCHO_HAVE_FLATBUFFERS)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_FLATBUFFERS)
ENDIF()
IF(ZLIB_FOUND)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_ZLIB_COMPRESSION)
ENDIF()
IF(BZIP2_FOUND)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_BZIP2_COMPRESSION)
ENDIF()
IF(LibArchive_FOUND)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_LIBARCHIVE_COMPRESSION)
ENDIF()
IF(LIBLZMA_FOUND)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_LZMA_COMPRESSION)
ENDIF()
IF(CHUCHO_HAVE_LZ4)
    LIST(APPEND CHUCHO_FEATURE_DEFS CHUCHO_HAVE_LZ4_COMPRESSION)
ENDIF()
IF(DEFINED CHUCHO_FEATURE_DEFS)
    SET_SOURCE_FILES_PROPERTIES(optional_features.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_FEATURE_DEFS}")
ENDIF()

SET_SOURCE_FILES_PROPERTIES(yaml_configurator.cpp PROPERTIES
                            COMPILE_DEFINITIONS YAML_DECLARE_STATIC)

IF(CHUCHO_HAVE_STD_REGEX)
    SET_SOURCE_FILES_PROPERTIES(regex.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_STD_REGEX)
ELSEIF(CHUCHO_HAVE_POSIX_REGEX)
    SET_SOURCE_FILES_PROPERTIES(regex.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_POSIX_REGEX)
ELSE()
    MESSAGE(FATAL_ERROR "You don't have regular expressions")
ENDIF()

IF(ZLIB_FOUND)
    LIST(APPEND CHUCHO_SOURCES
         gzip_file_compressor.cpp
         zlib_compressor.cpp
         zlib_compressor_factory.cpp
         zlib_compressor_memento.cpp
         include/chucho/zlib_compressor_factory.hpp
         include/chucho/zlib_compressor_memento.hpp)
    IF(MSVC)
        SET_SOURCE_FILES_PROPERTIES(gzip_file_compressor.cpp PROPERTIES
                                    COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    ENDIF()
    SET_SOURCE_FILES_PROPERTIES(gzip_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_ZLIB)
ENDIF()

IF(BZIP2_FOUND)
    LIST(APPEND CHUCHO_SOURCES
         bzip2_compressor.cpp
         bzip2_compressor_factory.cpp
         bzip2_file_compressor.cpp
         include/chucho/bzip2_compressor_factory.hpp)
    SET_SOURCE_FILES_PROPERTIES(bzip2_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_BZIP2)
ENDIF()

IF(LibArchive_FOUND)
    LIST(APPEND CHUCHO_SOURCES
         zip_file_compressor.cpp)
    SET_SOURCE_FILES_PROPERTIES(zip_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_LIBARCHIVE)
ENDIF()

IF(LIBLZMA_FOUND)
    LIST(APPEND CHUCHO_SOURCES
         lzma_file_compressor.cpp
         lzma_compressor.cpp
         lzma_compressor_factory.cpp
         include/chucho/lzma_compressor_factory.hpp)
    SET_SOURCE_FILES_PROPERTIES(lzma_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_LZMA)
ENDIF()

IF(CHUCHO_HAVE_LZ4)
    LIST(APPEND CHUCHO_SOURCES
         lz4_compressor.cpp
         lz4_compressor_factory.cpp
         include/chucho/lz4_compressor_factory.hpp)
ENDIF()

IF(ENABLE_SHARED)
    ADD_LIBRARY(chucho SHARED ${CHUCHO_SOURCES})
    SET_TARGET_PROPERTIES(chucho PROPERTIES
                          VERSION ${CHUCHO_VERSION}
                          SOVERSION ${CHUCHO_MAJOR_VERSION}
                          PUBLIC_HEADER "${CHUCHO_PUBLIC_HEADERS}"
                          FRAMEWORK ${ENABLE_FRAMEWORK})
    IF(DEFINED CHUCHO_SO_FLAGS)
        SET_TARGET_PROPERTIES(chucho PROPERTIES
                              COMPILE_FLAGS "${CHUCHO_SO_FLAGS}")
    ENDIF()
    IF(CMAKE_GENERATOR STREQUAL Xcode AND CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        SET_TARGET_PROPERTIES(chucho PROPERTIES
                              LINK_FLAGS "-std=c++11 -stdlib=libc++")
    ENDIF()
    IF(CHUCHO_WINDOWS)
        SET(CHUCHO_RT_DEST bin)
    ELSE()
        SET(CHUCHO_RT_DEST lib)
    ENDIF()
    INSTALL(TARGETS chucho
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION ${CHUCHO_RT_DEST}
            FRAMEWORK DESTINATION /Library/Frameworks
            PUBLIC_HEADER DESTINATION include/chucho)
ELSE()
    ADD_LIBRARY(chucho STATIC ${CHUCHO_SOURCES})
    SET_TARGET_PROPERTIES(chucho PROPERTIES
                          PUBLIC_HEADER "${CHUCHO_PUBLIC_HEADERS}")
    INSTALL(TARGETS chucho
            ARCHIVE DESTINATION lib
            PUBLIC_HEADER DESTINATION include/chucho)
ENDIF()

IF(CHUCHO_HAVE_MYSQL)
    TARGET_LINK_LIBRARIES(chucho ${MYSQL_LIBS})
ENDIF()

IF(CHUCHO_HAVE_ORACLE)
    TARGET_LINK_LIBRARIES(chucho ${ORACLE_LIBS})
ENDIF()

IF(CHUCHO_HAVE_SQLITE)
    TARGET_LINK_LIBRARIES(chucho ${SQLITE_LIBS})
ENDIF()

IF(CHUCHO_HAVE_POSTGRES)
    TARGET_LINK_LIBRARIES(chucho ${POSTGRES_LIBS})
ENDIF()

IF(CHUCHO_HAVE_DB2)
    TARGET_LINK_LIBRARIES(chucho ${DB2_LIBS})
ENDIF()

IF(RUBY_FOUND)
    TARGET_LINK_LIBRARIES(chucho ${RUBY_LIBRARY})
ENDIF()

IF(CURL_FOUND)
    TARGET_LINK_LIBRARIES(chucho ${CURL_LIBRARIES})
ENDIF()

IF(PROTOBUF_FOUND)
    TARGET_LINK_LIBRARIES(chucho ${PROTOBUF_LIBRARIES})
ENDIF()

IF(CHUCHO_HAVE_CAPN_PROTO)
    TARGET_LINK_LIBRARIES(chucho ${CAPN_PROTO_LIBS})
ENDIF()

IF(CHUCHO_HAVE_FLATBUFFERS)
    TARGET_LINK_LIBRARIES(chucho ${FLATBUFFERS_LIBS})
ENDIF()

IF(CHUCHO_HAVE_ZEROMQ)
    TARGET_LINK_LIBRARIES(chucho ${ZEROMQ_LIBS})
ENDIF()

IF(CHUCHO_HAVE_ACTIVEMQ)
    TARGET_LINK_LIBRARIES(chucho ${ACTIVEMQ_LIBS})
ENDIF()

IF(CHUCHO_HAVE_RABBITMQ)
    TARGET_LINK_LIBRARIES(chucho ${RABBITMQ_LIBS})
ENDIF()

IF(LibArchive_FOUND)
    TARGET_LINK_LIBRARIES(chucho ${LibArchive_LIBRARIES})
ENDIF()

IF(ZLIB_FOUND)
    TARGET_LINK_LIBRARIES(chucho ${ZLIB_LIBRARIES})
ENDIF()

IF(BZIP2_FOUND)
    TARGET_LINK_LIBRARIES(chucho ${BZIP2_LIBRARIES})
ENDIF()

IF(LIBLZMA_FOUND)
    TARGET_LINK_LIBRARIES(chucho ${LIBLZMA_LIBRARIES})
ENDIF()

IF(CHUCHO_HAVE_LZ4)
    TARGET_LINK_LIBRARIES(chucho ${LZ4_LIBS})
ENDIF()

TARGET_LINK_LIBRARIES(chucho ${CMAKE_THREAD_LIBS_INIT})

IF(CHUCHO_SOLARIS)
    TARGET_LINK_LIBRARIES(chucho socket nsl)
ENDIF()

IF(CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
    TARGET_LINK_LIBRARIES(chucho demangle)
ENDIF()

IF(CHUCHO_WINDOWS)
    TARGET_LINK_LIBRARIES(chucho ws2_32)
ENDIF()

IF(CHUCHO_WINDOWS)
    ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_BINARY_DIR}/event_log.rc"
                              "${CMAKE_BINARY_DIR}/event_log.hpp"
                       COMMAND mc -e hpp -h "${CMAKE_BINARY_DIR}" -r "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/platform/windows/event_log.mc"
                       DEPENDS platform/windows/event_log.mc)
    ADD_LIBRARY(event_log SHARED "${CMAKE_BINARY_DIR}/event_log.rc")
    SET_TARGET_PROPERTIES(event_log PROPERTIES
                          LINKER_LANGUAGE CXX
                          LINK_FLAGS /NOENTRY)
    ADD_DEPENDENCIES(chucho event_log)
    INSTALL(TARGETS event_log
            RUNTIME DESTINATION bin)
ENDIF()

ADD_SUBDIRECTORY(test)

# Documentation
IF(DOXYGEN_FOUND)
    SET(CHUCHO_DOXYGEN_INPUT main_page.hpp)
    IF(CHUCHO_DOCUMENT_ALL)
        FILE(GLOB CHUCHO_ALL_HEADERS RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/include/chucho/*")
        FOREACH(HEAD ${CHUCHO_ALL_HEADERS})
            SET(CHUCHO_DOXYGEN_INPUT "${CHUCHO_DOXYGEN_INPUT} ${HEAD}")
        ENDFOREACH()
        FILE(GLOB CHUCHO_GENERATED_HEADERS RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/chucho/*.hpp")
        FOREACH(HEAD ${CHUCHO_GENERATED_HEADERS})
            SET(CHUCHO_DOXYGEN_INPUT "${CHUCHO_DOXYGEN_INPUT} ${HEAD}")
        ENDFOREACH()
        SET(CHUCHO_DOXYGEN_EXTRACT_ALL YES)
    ELSE()
        FOREACH(HEAD ${CHUCHO_DOCUMENTABLE_HEADERS})
            SET(CHUCHO_DOXYGEN_INPUT "${CHUCHO_DOXYGEN_INPUT} ${HEAD}")
        ENDFOREACH()
        SET(CHUCHO_DOXYGEN_EXTRACT_ALL NO)
    ENDIF()
    CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/doxygen.conf.in" "${CMAKE_BINARY_DIR}/doxygen.conf")
    MESSAGE(STATUS "Generating HTML header and footer for API documentation")
    EXECUTE_PROCESS(COMMAND "${DOXYGEN_EXECUTABLE}" -w html "${CMAKE_BINARY_DIR}/doc_header.html" "${CMAKE_BINARY_DIR}/doc_footer.html" "${CMAKE_BINARY_DIR}/doc_stylesheet.html" "${CMAKE_BINARY_DIR}/doxygen.conf"
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    FILE(STRINGS "${CMAKE_BINARY_DIR}/doc_header.html" CHUCHO_HTML_HEADER)
    FILE(STRINGS "${CMAKE_BINARY_DIR}/doc_footer.html" CHUCHO_HTML_FOOTER)
    FILE(REMOVE "${CMAKE_BINARY_DIR}/doc_header.html" "${CMAKE_BINARY_DIR}/doc_footer.html" "${CMAKE_BINARY_DIR}/doc_stylesheet.html")
    LIST(FIND CHUCHO_HTML_HEADER <head> CHUCHO_HEAD_INDEX)
    IF(CHUCHO_HEAD_INDEX EQUAL -1)
        MESSAGE(FATAL_ERROR "The <head> section of the HTML header could not be found")
    ENDIF()
    MATH(EXPR CHUCHO_FAVICON_INDEX "${CHUCHO_HEAD_INDEX} + 1")
    LIST(INSERT CHUCHO_HTML_HEADER ${CHUCHO_FAVICON_INDEX} "<link rel=\"shortcut icon\" href=\"$relpath^itzcuintli.ico\" type=\"image/x-icon\"/>")
    FOREACH(LINE ${CHUCHO_HTML_HEADER})
        FILE(APPEND "${CMAKE_BINARY_DIR}/doc_header.html" ${LINE})
        FILE(APPEND "${CMAKE_BINARY_DIR}/doc_header.html" "\n")
    ENDFOREACH()
    LIST(FIND CHUCHO_HTML_FOOTER </body> CHUCHO_BODY_INDEX)
    IF(CHUCHO_BODY_INDEX EQUAL -1)
        MESSAGE(FATAL_ERROR "The </body> section of the HTML footer could not be found")
    ENDIF()
    LIST(INSERT CHUCHO_HTML_FOOTER ${CHUCHO_BODY_INDEX} "<script async src=\"//radar.cedexis.com/1/23762/radar.js\"></script>")
    FOREACH(LINE ${CHUCHO_HTML_FOOTER})
        FILE(APPEND "${CMAKE_BINARY_DIR}/doc_footer.html" ${LINE})
        FILE(APPEND "${CMAKE_BINARY_DIR}/doc_footer.html" "\n")
    ENDFOREACH()
    ADD_CUSTOM_TARGET(doc
                      COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_BINARY_DIR}/doxygen.conf"
                      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
ELSE()
    MESSAGE(STATUS "Doxygen must be present in order to generate Doxygen documentation")
    ADD_CUSTOM_TARGET(doc
                      COMMAND "${CMAKE_COMMAND}" -E echo "Doxygen was not found on this system, so the documentation cannot be generated")
ENDIF()

# Static analysis
IF(CHUCHO_CPPCHECK)
    SET(CHUCHO_CPPCHECK_SOURCES ${CHUCHO_SOURCES})
    SET(CHUCHO_TO_REMOVE ${CHUCHO_EMBEDDED_SOURCES})
    FOREACH(SRC ${CHUCHO_SOURCES})
        GET_FILENAME_COMPONENT(CHUCHO_SRC_EXT "${SRC}" EXT)    
        IF(CHUCHO_SRC_EXT STREQUAL .hpp)
            LIST(APPEND CHUCHO_TO_REMOVE "${SRC}")
        ENDIF()
    ENDFOREACH()
    LIST(REMOVE_ITEM CHUCHO_CPPCHECK_SOURCES ${CHUCHO_TO_REMOVE})
    ADD_CUSTOM_TARGET(cppcheck
                      COMMAND "${CHUCHO_CPPCHECK}" --enable=all -I include -I embedded/include -U_MSC_VER -U__clang__ -U__GNUC__ -U__SUNPRO_CC ${CHUCHO_CPPCHECK_SOURCES}
                      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
ENDIF()
