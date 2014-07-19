#
# Copyright 2013-2014 Will Mason
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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)
PROJECT(chucho CXX C)

SET(CHUCHO_VERSION 1.5)
SET(CHUCHO_SO_VERSION 1)
# This is used for release candidate distros
#SET(CHUCHO_DIST_VERSION try-1)

INCLUDE(Configure.cmake)

# Include CMAKE_BINARY_DIR because that's where the generated file
# export.hpp lives.
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

IF(CHUCHO_POSIX)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/posix/calendar_posix.cpp
        platform/posix/diagnostic_context_posix.cpp
        platform/posix/environment_posix.cpp
        platform/posix/file_posix.cpp
        platform/posix/host_posix.cpp
        platform/posix/line_ending_posix.cpp
        platform/posix/pattern_formatter_posix.cpp
        platform/posix/socket_connector_posix.cpp
        platform/posix/socket_exception_posix.cpp
        platform/posix/syslog_writer_posix.cpp
        platform/posix/text_util_posix.cpp)

    IF(CHUCHO_HAVE_GMTIME_R)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/calendar_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS CHUCHO_HAVE_GMTIME_R)
    ENDIF()
    IF(CHUCHO_HAVE_LOCALTIME_R)
        GET_SOURCE_FILE_PROPERTY(CHUCHO_CALENDAR_DEFS platform/posix/calendar_posix.cpp COMPILE_DEFINITIONS)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/calendar_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS "${CHUCHO_CALENDAR_DEFS};CHUCHO_HAVE_LOCALTIME_R")
    ENDIF()
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/file_posix.cpp PROPERTIES
                                    COMPILE_FLAGS "-Wno-unused-result")
    ENDIF()
    IF(CHUCHO_NO_FTS)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/file_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS CHUCHO_NO_FTS)
    ENDIF()
    IF(CHUCHO_DIRENT_NEEDS_NAME)
        GET_SOURCE_FILE_PROPERTY(CHUCHO_FILE_DEFS platform/posix/file_posix.cpp COMPILE_DEFINITIONS)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/file_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS "${CHUCHO_FILE_DEFS};CHUCHO_DIRENT_NEEDS_NAME")
    ENDIF()
ELSEIF(CHUCHO_WINDOWS)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/windows/calendar_windows.cpp
        platform/windows/diagnostic_context_windows.cpp
        platform/windows/environment_windows.cpp
        platform/windows/error_util.cpp
        platform/windows/error_util.hpp
        platform/windows/file_windows.cpp
        platform/windows/host_windows.cpp
        platform/windows/line_ending_windows.cpp
        platform/windows/pattern_formatter_windows.cpp
        platform/windows/socket_connector_windows.cpp
        platform/windows/socket_exception_windows.cpp
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
    include/chucho/configurable.hpp
    include/chucho/configurable_factory.hpp
    include/chucho/configuration.hpp
    include/chucho/configurator.hpp
    include/chucho/console_writer.hpp
    include/chucho/cout_writer.hpp
    include/chucho/database_writer.hpp
    include/chucho/diagnostic_context.hpp
    include/chucho/duplicate_message_filter.hpp
    include/chucho/evaluator_filter.hpp
    include/chucho/event.hpp
    include/chucho/exception.hpp
    "${CMAKE_BINARY_DIR}/chucho/export.hpp"
    include/chucho/file_compressor.hpp
    include/chucho/file_compressor_factory.hpp
    include/chucho/file_compressor_memento.hpp
    include/chucho/file_roller.hpp
    include/chucho/file_roller_memento.hpp
    include/chucho/file_roll_trigger.hpp
    include/chucho/file_writer.hpp
    include/chucho/filter.hpp
    include/chucho/finalize.hpp
    include/chucho/formatter.hpp
    include/chucho/level.hpp
    include/chucho/level_filter.hpp
    include/chucho/level_threshold_filter.hpp
    include/chucho/log.hpp
    include/chucho/loggable.hpp
    include/chucho/logger.hpp
    include/chucho/marker.hpp
    include/chucho/memento.hpp
    include/chucho/non_copyable.hpp
    include/chucho/noop_file_compressor.hpp
    include/chucho/numbered_file_roller.hpp
    include/chucho/optional.hpp
    include/chucho/pattern_formatter.hpp
    include/chucho/remote_writer.hpp
    include/chucho/rolling_file_writer.hpp
    include/chucho/ruby_evaluator_filter.hpp
    include/chucho/size_file_roll_trigger.hpp
    include/chucho/sliding_numbered_file_roller.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/syslog_constants.hpp
    include/chucho/syslog_writer.hpp
    include/chucho/time_file_roller.hpp
    "${CMAKE_BINARY_DIR}/chucho/version.hpp"
    include/chucho/writer.hpp
    include/chucho/writer_factory.hpp
    include/chucho/writer_memento.hpp
    include/chucho/yaml_formatter.hpp)

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS ${CHUCHO_PUBLIC_HEADERS})

SET(CHUCHO_C_HEADERS
    include/chucho/async_writer.h
    include/chucho/cerr_writer.h
    include/chucho/cout_writer.h
    include/chucho/configuration.h
    include/chucho/diagnostic_context.h
    include/chucho/duplicate_message_filter.h
    "${CMAKE_BINARY_DIR}/chucho/export.h"
    include/chucho/file_compressor.h
    include/chucho/file_roll_trigger.h
    include/chucho/file_roller.h
    include/chucho/file_writer.h
    include/chucho/filter.h
    include/chucho/finalize.h
    include/chucho/formatter.h
    include/chucho/level.h
    include/chucho/level_filter.h
    include/chucho/level_threshold_filter.h
    include/chucho/log.h
    include/chucho/logger.h
    include/chucho/numbered_file_roller.h
    include/chucho/pattern_formatter.h
    include/chucho/remote_writer.h
    include/chucho/return_code.h
    include/chucho/rolling_file_writer.h
    include/chucho/size_file_roll_trigger.h
    include/chucho/sliding_numbered_file_roller.h
    include/chucho/status.h
    include/chucho/status_manager.h
    include/chucho/syslog_constants.h
    include/chucho/syslog_writer.h
    include/chucho/time_file_roller.h
    include/chucho/writer.h)
IF(CHUCHO_WINDOWS)
    LIST(APPEND CHUCHO_C_HEADERS
         include/chucho/windows_event_log_writer.h)
ELSE()
    LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
         include/chucho/windows_event_log_writer.h)
ENDIF()

IF(C_API)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS ${CHUCHO_C_HEADERS})
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS ${CHUCHO_C_HEADERS})

IF(CHUCHO_WINDOWS)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS
         include/chucho/windows_event_log_writer.hpp)
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/windows_event_log_writer.hpp)

IF(CHUCHO_HAVE_ZLIB)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/gzip_file_compressor.hpp)
    IF(C_API)
        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/gzip_file_compressor.h)
    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/gzip_file_compressor.hpp
     include/chucho/gzip_file_compressor.h)

IF(CHUCHO_HAVE_BZIP2)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/bzip2_file_compressor.hpp)
    IF(C_API)
        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/bzip2_file_compressor.h)
    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/bzip2_file_compressor.hpp
     include/chucho/bzip2_file_compressor.h)

IF(CHUCHO_HAVE_MINIZIP)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/zip_file_compressor.hpp)
    IF(C_API)
        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/zip_file_compressor.h)
    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/zip_file_compressor.hpp
     include/chucho/zip_file_compressor.h)

IF(CHUCHO_HAVE_ORACLE)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/oracle_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         oracle_writer.cpp
         oracle_writer_factory.cpp
         oracle_writer_memento.cpp
         include/chucho/oracle_writer_factory.hpp
         include/chucho/oracle_writer_memento.hpp)
    IF(C_API)
        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/oracle_writer.h)
        LIST(APPEND CHUCHO_DATABASE_SOURCES c_oracle_writer.cpp)
    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/oracle_writer.hpp
     include/chucho/oracle_writer.h)

IF(CHUCHO_HAVE_MYSQL)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/mysql_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         mysql_writer.cpp
         mysql_writer_factory.cpp
         mysql_writer_memento.cpp
         include/chucho/mysql_writer_factory.hpp
         include/chucho/mysql_writer_memento.hpp)
    IF(C_API)
        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/mysql_writer.h)
        LIST(APPEND CHUCHO_DATABASE_SOURCES c_mysql_writer.cpp)
    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/mysql_writer.hpp
     include/chucho/mysql_writer.h)

IF(CHUCHO_HAVE_SQLITE)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/sqlite_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         sqlite_writer.cpp
         sqlite_writer_factory.cpp
         sqlite_writer_memento.cpp
         include/chucho/sqlite_writer_factory.hpp
         include/chucho/sqlite_writer_memento.hpp)
    IF(C_API)
        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/sqlite_writer.h)
        LIST(APPEND CHUCHO_DATABASE_SOURCES c_sqlite_writer.cpp)
    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/sqlite_writer.hpp
     include/chucho/sqlite_writer.h)

IF(CHUCHO_HAVE_POSTGRES)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/postgres_writer.hpp)
    LIST(APPEND CHUCHO_DATABASE_SOURCES
         postgres_writer.cpp
         postgres_writer_factory.cpp
         postgres_writer_memento.cpp
         include/chucho/postgres_writer_factory.hpp
         include/chucho/postgres_writer_memento.hpp)
#    IF(C_API)
#        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/postgres_writer.h)
#        LIST(APPEND CHUCHO_DATABASE_SOURCES c_postgres_writer.cpp)
#    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/postgres_writer.hpp)
#     include/chucho/postgres_writer.h)

IF(CHUCHO_HAVE_RUBY)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/ruby_evaluator_filter.hpp)
    LIST(APPEND CHUCHO_EVALUATOR_SOURCES
         ruby_evaluator_filter.cpp
         ruby_evaluator_filter_factory.cpp
         ruby_evaluator_filter_memento.cpp
         include/chucho/ruby_evaluator_filter_factory.hpp
         include/chucho/ruby_evaluator_filter_memento.hpp)
    IF(C_API)
        LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/ruby_evaluator_filter.h)
        LIST(APPEND CHUCHO_EVALUATOR_SOURCES c_ruby_evaluator_filter.cpp)
    ENDIF()
    IF(RUBY_FRAMEWORK)
        SET_SOURCE_FILES_PROPERTIES(ruby_evaluator_filter.cpp PROPERTIES
                                    COMPILE_DEFINITIONS CHUCHO_RUBY_FRAMEWORK)
    ELSEIF(RUBY_INCLUDE_DIR)
        SET_SOURCE_FILES_PROPERTIES(ruby_evaluator_filter.cpp PROPERTIES
                                    COMPILE_FLAGS "-I\"${RUBY_INCLUDE_DIR}\" -I\"${CHUCHO_RUBY_CONFIG_DIR}\"")
    ENDIF()
ENDIF()

LIST(APPEND CHUCHO_DOCUMENTABLE_HEADERS
     include/chucho/ruby_evaluator_filter.hpp
     include/chucho/ruby_evaluator_filter.h)

# The libyaml sources are required by chuchod
MACRO(CHUCHO_SET_YAML_SOURCES CHUCHO_YAML_SOURCE_VAR)
    # Update this when you rev libyaml
    SET(CHUCHO_LIBYAML_VERSION_MAJOR 0)
    SET(CHUCHO_LIBYAML_VERSION_MINOR 1)
    SET(CHUCHO_LIBYAML_VERSION_PATCH 4)

    SET(${CHUCHO_YAML_SOURCE_VAR}
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
        # Microsoft doesn't want libyaml to use strdup
        SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/embedded/libyaml/api.c" PROPERTIES
                                    COMPILE_FLAGS /wd4996)
    ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
        SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/embedded/libyaml/api.c" PROPERTIES
                                    COMPILE_FLAGS "-Wno-implicit-function-declaration -Wno-int-to-pointer-cast")
    ENDIF()
ENDMACRO()

# Configurator settings
IF(YAML_CONFIG)
    CHUCHO_SET_YAML_SOURCES(CHUCHO_EMBEDDED_SOURCES)
    SET(CHUCHO_CONFIGURATOR_SOURCES
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
    SET_SOURCE_FILES_PROPERTIES(config_file_configurator.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_CONFIGURATOR_DEFS}")
    SET_SOURCE_FILES_PROPERTIES(memento_key_set.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_CONFIG_FILE_CONFIG)
ENDIF()

IF(C_API)
    SET(CHUCHO_C_SOURCES
        c_async_writer.cpp
        c_cerr_writer.cpp
        c_cout_writer.cpp
        c_configuration.cpp
        c_diagnostic_context.cpp
        c_duplicate_message_filter.cpp
        c_file_compressor.cpp
        c_file_roll_trigger.cpp
        c_file_roller.cpp
        c_file_writer.cpp
        c_filter.cpp
        c_finalize.cpp
        c_formatter.cpp
        c_level.cpp
        c_level_filter.cpp
        c_level_threshold_filter.cpp
        c_log.cpp
        c_logger.cpp
        c_numbered_file_roller.cpp
        c_pattern_formatter.cpp
        c_remote_writer.cpp
        c_return_code.cpp
        c_rolling_file_writer.cpp
        c_size_file_roll_trigger.cpp
        c_sliding_numbered_file_roller.cpp
        c_status_manager.cpp
        c_syslog_constants.cpp
        c_syslog_writer.cpp
        c_time_file_roller.cpp
        c_util.cpp
        c_writer.cpp
        include/chucho/c_file_compressor.hpp
        include/chucho/c_file_roll_trigger.hpp
        include/chucho/c_file_roller.hpp
        include/chucho/c_filter.hpp
        include/chucho/c_formatter.hpp
        include/chucho/c_level.hpp
        include/chucho/c_logger.hpp
        include/chucho/c_util.hpp
        include/chucho/c_writer.hpp)
    IF(CHUCHO_WINDOWS)
        LIST(APPEND CHUCHO_C_SOURCES
             platform/windows/c_windows_event_log_writer.cpp)
    ENDIF()
ENDIF()

IF(MSVC)
    SET_SOURCE_FILES_PROPERTIES(c_diagnostic_context.cpp PROPERTIES
                                COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    SET_SOURCE_FILES_PROPERTIES(c_file_roller.cpp PROPERTIES
                                COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
ENDIF()

SET(CHUCHO_SOURCES
    async_writer.cpp
    async_writer_factory.cpp
    async_writer_memento.cpp
    bzip2_file_compressor_factory.cpp
    calendar.cpp
    cerr_writer.cpp
    cerr_writer_factory.cpp
    cerr_writer_memento.cpp
    clock_util.cpp
    configurable.cpp
    configurable_factory.cpp
    configuration.cpp
    configurator.cpp
    console_writer.cpp
    cout_writer.cpp
    cout_writer_factory.cpp
    cout_writer_memento.cpp
    demangle.cpp
    database_writer.cpp
    diagnostic_context.cpp
    duplicate_message_filter.cpp
    duplicate_message_filter_factory.cpp
    duplicate_message_filter_memento.cpp
    event.cpp
    exception.cpp
    file_compressor_factory.cpp
    file_compressor_memento.cpp
    file_exception.cpp
    file_roller.cpp
    file_roller_memento.cpp
    file_writer.cpp
    file_writer_factory.cpp
    file_writer_memento.cpp
    finalize.cpp
    garbage_cleaner.cpp
    gzip_file_compressor_factory.cpp
    host.cpp
    level.cpp
    level_filter.cpp
    level_filter_factory.cpp
    level_filter_memento.cpp
    level_threshold_filter.cpp
    level_threshold_filter_factory.cpp
    level_threshold_filter_memento.cpp
    logger.cpp
    logger_factory.cpp
    logger_memento.cpp
    marker.cpp
    memento.cpp
    memento_key_set.cpp
    noop_file_compressor.cpp
    numbered_file_roller.cpp
    numbered_file_roller_factory.cpp
    numbered_file_roller_memento.cpp
    pattern_formatter.cpp
    pattern_formatter_factory.cpp
    pattern_formatter_memento.cpp
    regex.cpp
    regex_exception.cpp
    remote_writer.cpp
    remote_writer_factory.cpp
    remote_writer_memento.cpp
    rolling_file_writer.cpp
    rolling_file_writer_factory.cpp
    rolling_file_writer_memento.cpp
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
    yaml_formatter.cpp
    zip_file_compressor_factory.cpp
    include/chucho/async_writer_factory.hpp
    include/chucho/async_writer_memento.hpp
    include/chucho/bzip2_file_compressor_factory.hpp
    include/chucho/calendar.hpp
    include/chucho/cerr_writer_factory.hpp
    include/chucho/cerr_writer_memento.hpp
    include/chucho/clock_util.hpp
    include/chucho/configurator.hpp
    include/chucho/cout_writer_factory.hpp
    include/chucho/cout_writer_memento.hpp
    include/chucho/demangle.hpp
    include/chucho/duplicate_message_filter_factory.hpp
    include/chucho/duplicate_message_filter_memento.hpp
    include/chucho/environment.hpp
    include/chucho/exception.hpp
    include/chucho/file.hpp
    include/chucho/file_exception.hpp
    include/chucho/file_writer_factory.hpp
    include/chucho/file_writer_memento.hpp
    include/chucho/garbage_cleaner.hpp
    include/chucho/gzip_file_compressor_factory.hpp
    include/chucho/host.hpp
    include/chucho/level_filter_factory.hpp
    include/chucho/level_filter_memento.hpp
    include/chucho/level_threshold_filter_factory.hpp
    include/chucho/level_threshold_filter_memento.hpp
    include/chucho/line_ending.hpp
    include/chucho/logger_factory.hpp
    include/chucho/logger_memento.hpp
    include/chucho/memento_key_set.hpp
    include/chucho/numbered_file_roller_factory.hpp
    include/chucho/numbered_file_roller_memento.hpp
    include/chucho/pattern_formatter_factory.hpp
    include/chucho/pattern_formatter_memento.hpp
    include/chucho/regex.hpp
    include/chucho/regex_exception.hpp
    include/chucho/remote_writer_factory.hpp
    include/chucho/remote_writer_memento.hpp
    include/chucho/rolling_file_writer_factory.hpp
    include/chucho/rolling_file_writer_memento.hpp
    include/chucho/size_file_roll_trigger_factory.hpp
    include/chucho/size_file_roll_trigger_memento.hpp
    include/chucho/sliding_numbered_file_roller_factory.hpp
    include/chucho/sliding_numbered_file_roller_memento.hpp
    include/chucho/socket_connector.hpp
    include/chucho/socket_exception.hpp
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
    ${CHUCHO_EVALUATOR_SOURCES})

IF(CHUCHO_HAVE_PUT_TIME)
    SET_SOURCE_FILES_PROPERTIES(calendar.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_PUT_TIME)
ENDIF()

IF(CHUCHO_WINDOWS)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_WINDOWS)
    SET_SOURCE_FILES_PROPERTIES(configurator.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_WINDOWS)
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

IF(CHUCHO_HAVE_RUBY)
    LIST(APPEND CHUCHO_CONFIGURATOR_DEFS CHUCHO_HAVE_RUBY)
ENDIF()

IF(DEFINED CHUCHO_CONFIGURATOR_DEFS)
    SET_SOURCE_FILES_PROPERTIES(configurator.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_CONFIGURATOR_DEFS}")
ENDIF()

SET_SOURCE_FILES_PROPERTIES(yaml_configurator.cpp PROPERTIES
                            COMPILE_DEFINITIONS YAML_DECLARE_STATIC)

IF(CHUCHO_HAVE_ARPA_INET_H)
    SET_SOURCE_FILES_PROPERTIES(remote_writer.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_ARPA_INET_H)
ENDIF()

IF(CHUCHO_HAVE_WINSOCK2_H)
    GET_SOURCE_FILE_PROPERTY(CHUCHO_REMOTE_WRITER_DEFS remote_writer.cpp COMPILE_DEFINITIONS)
    SET_SOURCE_FILES_PROPERTIES(remote_writer.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_REMOTE_WRITER_DEFS};CHUCHO_HAVE_WINSOCK2_H")
ENDIF()

IF(CHUCHO_HAVE_STD_REGEX)
    SET_SOURCE_FILES_PROPERTIES(regex.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_STD_REGEX)
ELSEIF(CHUCHO_HAVE_POSIX_REGEX)
    SET_SOURCE_FILES_PROPERTIES(regex.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_POSIX_REGEX)
ELSE()
    MESSAGE(FATAL_ERROR "You don't have regular expressions")
ENDIF()

IF(CHUCHO_HAVE_ZLIB)
    LIST(APPEND CHUCHO_SOURCES
         gzip_file_compressor.cpp
         ${CHUCHO_ZLIB_SOURCES})
    IF(C_API)
        LIST(APPEND CHUCHO_SOURCES c_gzip_file_compressor.cpp)
    ENDIF()
    IF(MSVC)
        SET_SOURCE_FILES_PROPERTIES(gzip_file_compressor.cpp PROPERTIES
                                    COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    ENDIF()
    SET_SOURCE_FILES_PROPERTIES(gzip_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_ZLIB)
    IF(ZLIB_SOURCE)
        INCLUDE_DIRECTORIES("${ZLIB_SOURCE}")
    ELSEIF(ZLIB_INCLUDE_DIR)
        INCLUDE_DIRECTORIES("${ZLIB_INCLUDE_DIR}")
    ENDIF()
    IF(CHUCHO_ZLIB_SOURCES)
        IF(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_COMPILER_IS_GNUCXX)
            SET_SOURCE_FILES_PROPERTIES("${ZLIB_SOURCE}/gzlib.c"
                                        "${ZLIB_SOURCE}/gzread.c"
                                        "${ZLIB_SOURCE}/gzwrite.c"
                                        PROPERTIES
                                        COMPILE_FLAGS -Wno-implicit-function-declaration)
        ELSEIF(MSVC)
            SET_SOURCE_FILES_PROPERTIES("${ZLIB_SOURCE}/gzlib.c"
                                        "${ZLIB_SOURCE}/gzread.c"
                                        "${ZLIB_SOURCE}/gzwrite.c"
                                        PROPERTIES
                                        COMPILE_FLAGS /wd4996)
        ENDIF()
    ENDIF()
ENDIF()

IF(CHUCHO_HAVE_BZIP2)
    LIST(APPEND CHUCHO_SOURCES
         bzip2_file_compressor.cpp
         ${CHUCHO_BZIP2_SOURCES})
    IF(C_API)
        LIST(APPEND CHUCHO_SOURCES c_bzip2_file_compressor.cpp)
    ENDIF()
    SET_SOURCE_FILES_PROPERTIES(bzip2_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_BZIP2)
    IF(BZIP2_SOURCE)
        INCLUDE_DIRECTORIES("${BZIP2_SOURCE}")
    ELSEIF(BZIP2_INCLUDE_DIR)
        INCLUDE_DIRECTORIES("${BZIP2_INCLUDE_DIR}")
    ENDIF()
    IF(MSVC)
        SET_SOURCE_FILES_PROPERTIES(bzip2_file_compressor.cpp PROPERTIES
                                    COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
        IF(CHUCHO_BZIP2_SOURCES)
            SET_SOURCE_FILES_PROPERTIES("${BZIP2_SOURCE}/bzlib.c" PROPERTIES
                                        COMPILE_FLAGS /wd4996)
        ENDIF()
    ELSEIF(CMAKE_COMPILER_IS_GNUCXX AND CHUCHO_BZIP2_SOURCES)
        SET_SOURCE_FILES_PROPERTIES("${BZIP2_SOURCE}/bzlib.c" PROPERTIES
                                    COMPILE_FLAGS -Wno-implicit-function-declaration)
    ENDIF()
ENDIF()

IF(CHUCHO_HAVE_MINIZIP)
    LIST(APPEND CHUCHO_SOURCES
         zip_file_compressor.cpp
         ${CHUCHO_MINIZIP_SOURCES})
    IF(C_API)
        LIST(APPEND CHUCHO_SOURCES c_zip_file_compressor.cpp)
    ENDIF()
    SET_SOURCE_FILES_PROPERTIES(zip_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_MINIZIP)
    INCLUDE_DIRECTORIES("${MINIZIP_SOURCE}")
    IF(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        SET_SOURCE_FILES_PROPERTIES("${MINIZIP_SOURCE}/zip.c"
                                    PROPERTIES
                                    COMPILE_FLAGS "-Wno-parentheses-equality -Wno-incompatible-pointer-types")
    ENDIF()
    IF(CHUCHO_SOLARIS)
        SET_SOURCE_FILES_PROPERTIES(${CHUCHO_MINIZIP_SOURCES}
                                    PROPERTIES
                                    COMPILE_DEFINITIONS "_LARGEFILE_SOURCE;_FILE_OFFSET_BITS=64")
    ELSEIF(NOT CHUCHO_HAVE_FOPEN64)
        SET_SOURCE_FILES_PROPERTIES(${CHUCHO_MINIZIP_SOURCES}
                                    PROPERTIES
                                    COMPILE_DEFINITIONS USE_FILE32API)
    ENDIF()
ENDIF()

IF(ENABLE_SHARED)
    ADD_LIBRARY(chucho SHARED ${CHUCHO_SOURCES})
    SET_TARGET_PROPERTIES(chucho PROPERTIES
                          VERSION ${CHUCHO_VERSION}
                          SOVERSION ${CHUCHO_SO_VERSION}
                          PUBLIC_HEADER "${CHUCHO_PUBLIC_HEADERS}"
                          FRAMEWORK ${ENABLE_FRAMEWORK})
    IF(CHUCHO_HAVE_ORACLE)
        TARGET_LINK_LIBRARIES(chucho "${ORACLE_CLIENT_LIB}")
    ENDIF()
    IF(CHUCHO_HAVE_POSTGRES)
        TARGET_LINK_LIBRARIES(chucho "${POSTGRES_CLIENT_LIB}")
    ENDIF()
    IF(CHUCHO_HAVE_RUBY)
        TARGET_LINK_LIBRARIES(chucho "${RUBY_LIB}")
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

IF(CHUCHO_WINDOWS)
    TARGET_LINK_LIBRARIES(chucho ws2_32)
ENDIF()

IF(ZLIB_LIB_DIR)
    LINK_DIRECTORIES("${ZLIB_LIB_DIR}")
    TARGET_LINK_LIBRARIES(chucho z)
ENDIF()

IF(BZIP2_LIB_DIR)
    LINK_DIRECTORIES("${BZIP2_LIB_DIR}")
    TARGET_LINK_LIBRARIES(chucho bz2)
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
ADD_SUBDIRECTORY(log-server)

# Documentation
IF(DOXYGEN_FOUND)
    SET(CHUCHO_DOXYGEN_INPUT main_page.hpp)
    IF(CHUCHO_DOCUMENT_ALL)
        FOREACH(HEAD ${CHUCHO_SOURCES})
            IF(HEAD MATCHES "^include/chucho/.+\\.hpp$")
                SET(CHUCHO_DOXYGEN_INPUT "${CHUCHO_DOXYGEN_INPUT} ${HEAD}")
            ENDIF()
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
    ADD_CUSTOM_TARGET(doc
                      COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_BINARY_DIR}/doxygen.conf"
                      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
ELSE()
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

# Distro
FILE(GLOB_RECURSE CHUCHO_ALL_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/*")
FILE(RELATIVE_PATH CHUCHO_REL_BINARY_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
UNSET(CHUCHO_TO_REMOVE)
FOREACH(SRC ${CHUCHO_ALL_FILES})
    IF(SRC MATCHES "^${CHUCHO_REL_BINARY_DIR}/" OR SRC MATCHES "\\.git" OR SRC MATCHES ".DS_Store$")
        LIST(APPEND CHUCHO_TO_REMOVE "${SRC}")
    ENDIF()
ENDFOREACH()
LIST(LENGTH CHUCHO_TO_REMOVE CHUCHO_TO_REMOVE_LENGTH)
IF(CHUCHO_TO_REMOVE_LENGTH GREATER 0)
    LIST(REMOVE_ITEM CHUCHO_ALL_FILES ${CHUCHO_TO_REMOVE})
ENDIF()
STRING(TOLOWER ${CMAKE_PROJECT_NAME} CHUCHO_LOWER_PROJECT_NAME)
SET(CHUCHO_DIST_NAME_BASE "${CMAKE_BINARY_DIR}/${CHUCHO_LOWER_PROJECT_NAME}-${CHUCHO_VERSION}")
IF(DEFINED CHUCHO_DIST_VERSION)
    SET(CHUCHO_DIST_NAME_BASE "${CHUCHO_DIST_NAME_BASE}-${CHUCHO_DIST_VERSION}")
ENDIF()
FOREACH(SRC ${CHUCHO_ALL_FILES})
    SET(CHUCHO_SRC_OUT "${CHUCHO_DIST_NAME_BASE}/${SRC}")
    LIST(APPEND CHUCHO_DIST_OUTPUT "${CHUCHO_SRC_OUT}")
    ADD_CUSTOM_COMMAND(OUTPUT "${CHUCHO_SRC_OUT}"
                       COMMAND "${CMAKE_COMMAND}" -E copy "${SRC}" "${CHUCHO_SRC_OUT}"
                       DEPENDS "${SRC}"
                       WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
ENDFOREACH()
ADD_CUSTOM_TARGET(dist
                  COMMAND "${CMAKE_COMMAND}" -E tar czvf ${CHUCHO_DIST_NAME_BASE}.tar.gz ${CHUCHO_DIST_NAME_BASE}
                  DEPENDS ${CHUCHO_DIST_OUTPUT})
IF(CHUCHO_ZIP)
    FILE(RELATIVE_PATH CHUCHO_ZDIST_NAME_BASE "${CMAKE_BINARY_DIR}" "${CHUCHO_DIST_NAME_BASE}")
    ADD_CUSTOM_TARGET(zdist
                      COMMAND "${CHUCHO_ZIP}" --recurse-paths --to-crlf ${CHUCHO_DIST_NAME_BASE}.zip ${CHUCHO_ZDIST_NAME_BASE}
                      DEPENDS ${CHUCHO_DIST_OUTPUT})
ENDIF()
