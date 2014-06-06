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

IF(MSVC)
    ADD_DEFINITIONS(-D_VARIADIC_MAX=10)
ENDIF()

IF(C_API)
    IF(CMAKE_COMPILER_IS_GNUC OR CMAKE_C_COMPILER_ID STREQUAL Clang)
        INCLUDE(CheckCCompilerFlag)
        CHECK_C_COMPILER_FLAG(-std=c99 CHUCHO_HAVE_STDC99)
        IF(NOT CHUCHO_HAVE_STDC99)
            MESSAGE(FATAL_ERROR "-std=c99 is required")
        ENDIF()
        SET(CMAKE_C_FLAGS -std=c99)
    ENDIF()
ENDIF()

IF(CHUCHO_POSIX)
    ADD_DEFINITIONS(-DCHUCHO_POSIX)
ELSEIF(CHUCHO_WINDOWS)
    ADD_DEFINITIONS(-DCHUCHO_WINDOWS)
ENDIF()
INCLUDE_DIRECTORIES("${CHUCHO_EXTERNAL_PREFIX}/include")

IF(CHUCHO_SOLARIS)
    ADD_DEFINITIONS(-DCHUCHO_SOLARIS)
ENDIF()

IF(CHUCHO_HAVE_ZLIB)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES gzip_file_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_ZLIB)
ENDIF()

IF(CHUCHO_HAVE_BZIP2)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES bzip2_file_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_BZIP2)
ENDIF()

IF(CHUCHO_HAVE_MINIZIP)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES zip_file_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_MINIZIP)
ENDIF()

IF(YAML_CONFIG)
    SET(CHUCHO_CONFIGURATOR_TESTS yaml_configurator_test.cpp utf8_test.cpp)
ENDIF()

IF(CONFIG_FILE_CONFIG OR LOG4CPLUS_CONFIG)
    LIST(APPEND CHUCHO_CONFIGURATOR_TESTS properties_test.cpp)
    IF(CONFIG_FILE_CONFIG)
        LIST(APPEND CHUCHO_CONFIGURATOR_TESTS chucho_config_file_configurator_test.cpp)
    ENDIF()
    IF(LOG4CPLUS_CONFIG)
        LIST(APPEND CHUCHO_CONFIGURATOR_TESTS log4cplus_config_file_configurator_test.cpp)
    ENDIF()
ENDIF()

IF(CHUCHO_HAVE_MYSQL)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_MYSQL)
ENDIF()

IF(CHUCHO_HAVE_ORACLE)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_ORACLE)
ENDIF()

IF(CHUCHO_HAVE_SQLITE)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_SQLITE)
ENDIF()

ADD_EXECUTABLE(unit-test EXCLUDE_FROM_ALL
               async_writer_test.cpp
               calendar_test.cpp
               configurator_test.cpp
               configurator_test.hpp
               diagnostic_context_test.cpp
               duplicate_message_filter_test.cpp
               file_test.cpp
               file_writer_test.cpp
               harness.cpp
               level_test.cpp
               level_filter_test.cpp
               level_threshold_filter_test.cpp
               log_macro_test.cpp
               loggable_test.cpp
               logger_test.cpp
               marker_test.cpp
               multithread_test.cpp
               optional_test.cpp
               pattern_formatter_test.cpp
               regex_test.cpp
               rolling_file_writer_test.cpp
               size_file_roll_trigger_test.cpp
               utf8_test.cpp
               ${CHUCHO_TEST_COMPRESSION_SOURCES}
               ${CHUCHO_CONFIGURATOR_TESTS})
TARGET_LINK_LIBRARIES(unit-test chucho gtest ${CHUCHO_THREAD_LIB})
IF(CHUCHO_SOLARIS)
    TARGET_LINK_LIBRARIES(unit-test socket nsl)
ENDIF()

IF(CHUCHO_HAVE_MYSQL)
    TARGET_LINK_LIBRARIES(unit-test "${MYSQL_CLIENT_LIB}")
ENDIF()

IF(CHUCHO_HAVE_ORACLE)
    TARGET_LINK_LIBRARIES(unit-test "${ORACLE_CLIENT_LIB}")
ENDIF()

IF(CHUCHO_HAVE_SQLITE)
    TARGET_LINK_LIBRARIES(unit-test "${SQLITE_CLIENT_LIB}")
ENDIF()

IF(MSVC)
    SET_SOURCE_FILES_PROPERTIES(pattern_formatter_test.cpp PROPERTIES
                                COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    SET_SOURCE_FILES_PROPERTIES(rolling_file_writer_test.cpp PROPERTIES
                                COMPILE_FLAGS /wd4018)
ENDIF()

ADD_CUSTOM_TARGET(check
                  COMMAND unit-test)

IF(C_API)
    IF(CHUCHO_HAVE_ZLIB OR CHUCHO_HAVE_BZIP2 OR CHUCHO_HAVE_MINIZIP)
        SET(CHUCHO_TEST_COMPRESSION_C_SOURCES
            file_compressor_test.c
            file_compressor_test.h)
    ENDIF()

    IF(CHUCHO_HAVE_ORACLE)
        LIST(APPEND CHUCHO_DB_C_SOURCES
             oracle_writer_test.c
             oracle_writer_test.h)
    ENDIF()

    IF(CHUCHO_HAVE_MYSQL)
        LIST(APPEND CHUCHO_DB_C_SOURCES
             mysql_writer_test.c
             mysql_writer_test.h)
    ENDIF()

    IF(CHUCHO_HAVE_SQLITE)
        LIST(APPEND CHUCHO_DB_C_SOURCES
             sqlite_writer_test.c
             sqlite_writer_test.h)
    ENDIF()

    ADD_EXECUTABLE(c-unit-test EXCLUDE_FROM_ALL
                   async_writer_test.c
                   async_writer_test.h
                   cerr_writer_test.c
                   cerr_writer_test.h
                   cout_writer_test.c
                   cout_writer_test.h
                   configuration_test.c
                   configuration_test.h
                   diagnostic_context_test.c
                   diagnostic_context_test.h
                   duplicate_message_filter_test.c
                   duplicate_message_filter_test.h
                   file_roller_test.c
                   file_roller_test.h
                   file_writer_test.c
                   file_writer_test.h
                   harness.c
                   level_filter_test.c
                   level_filter_test.h
                   level_test.c
                   level_test.h
                   level_threshold_filter_test.c
                   level_threshold_filter_test.h
                   log_macro_test.c
                   log_macro_test.h
                   logger_test.c
                   logger_test.h
                   numbered_file_roller_test.c
                   numbered_file_roller_test.h
                   pattern_formatter_test.c
                   pattern_formatter_test.h
                   remote_writer_test.c
                   remote_writer_test.h
                   rolling_file_writer_test.c
                   rolling_file_writer_test.h
                   size_file_roll_trigger_test.c
                   size_file_roll_trigger_test.h
                   sliding_numbered_file_roller_test.c
                   sliding_numbered_file_roller_test.h
                   syslog_writer_test.c
                   syslog_writer_test.h
                   test_util.c
                   test_util.h
                   time_file_roller_test.c
                   time_file_roller_test.h
                   writer_test.c
                   writer_test.h
                   ${CHUCHO_TEST_COMPRESSION_C_SOURCES}
                   ${CHUCHO_DB_C_SOURCES})
    TARGET_LINK_LIBRARIES(c-unit-test chucho ${CHUCHO_THREAD_LIB})

    IF(CHUCHO_HAVE_ORACLE)
        TARGET_LINK_LIBRARIES(c-unit-test "${ORACLE_CLIENT_LIB}")
    ENDIF()

    IF(CHUCHO_HAVE_MYSQL)
        TARGET_LINK_LIBRARIES(c-unit-test "${MYSQL_CLIENT_LIB}")
    ENDIF()

    IF(CHUCHO_HAVE_SQLITE)
        TARGET_LINK_LIBRARIES(c-unit-test "${SQLITE_CLIENT_LIB}")
    ENDIF()

    IF(CHUCHO_SOLARIS)
        TARGET_LINK_LIBRARIES(c-unit-test socket nsl)
    ENDIF()

    ADD_CUSTOM_TARGET(c-check
                      COMMAND c-unit-test)
ENDIF()

ADD_EXECUTABLE(time-rolling-file-test EXCLUDE_FROM_ALL
               harness.cpp
               time_rolling_file_test.cpp)
TARGET_LINK_LIBRARIES(time-rolling-file-test chucho gtest ${CHUCHO_THREAD_LIB})
IF(CHUCHO_SOLARIS)
    TARGET_LINK_LIBRARIES(time-rolling-file-test socket nsl)
ENDIF()

ADD_EXECUTABLE(syslog-writer-test EXCLUDE_FROM_ALL
               harness.cpp
               syslog_writer_test.cpp)
TARGET_LINK_LIBRARIES(syslog-writer-test chucho gtest ${CHUCHO_THREAD_LIB})
IF(CHUCHO_SOLARIS)
    TARGET_LINK_LIBRARIES(syslog-writer-test socket nsl)
ENDIF()

ADD_EXECUTABLE(remote-writer-test EXCLUDE_FROM_ALL
               remote_writer_test.cpp)
TARGET_LINK_LIBRARIES(remote-writer-test chucho ${CHUCHO_THREAD_LIB})
IF(CHUCHO_SOLARIS)
    TARGET_LINK_LIBRARIES(remote-writer-test socket nsl)
ENDIF()

IF(CHUCHO_WINDOWS)
    ADD_EXECUTABLE(windows-event-log-writer-test EXCLUDE_FROM_ALL
                   harness.cpp
                   windows_event_log_writer_test.cpp)
    FILE(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}" CHUCHO_EVENT_LOG_DLL)
    SET(CHUCHO_EVENT_LOG_DLL "${CHUCHO_EVENT_LOG_DLL}\\event_log.dll")
    STRING(REPLACE "\\" "\\\\" CHUCHO_EVENT_LOG_DLL "${CHUCHO_EVENT_LOG_DLL}")
    SET_SOURCE_FILES_PROPERTIES(windows_event_log_writer_test PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_EVENT_LOG_DLL="${CHUCHO_EVENT_LOG_DLL}")
    TARGET_LINK_LIBRARIES(windows-event-log-writer-test chucho gtest ${CHUCHO_THREAD_LIB})
ENDIF()

IF(CHUCHO_HAVE_ORACLE)
    ADD_EXECUTABLE(oracle-writer-test EXCLUDE_FROM_ALL
                   harness.cpp
                   oracle_writer_test.cpp)
    TARGET_LINK_LIBRARIES(oracle-writer-test chucho ${ORACLE_CLIENT_LIB} gtest ${CHUCHO_THREAD_LIB})
ENDIF()

IF(CHUCHO_HAVE_MYSQL)
    ADD_EXECUTABLE(mysql-writer-test EXCLUDE_FROM_ALL
                   harness.cpp
                   mysql_writer_test.cpp)
    TARGET_LINK_LIBRARIES(mysql-writer-test chucho "${MYSQL_CLIENT_LIB}" gtest ${CHUCHO_THREAD_LIB})
ENDIF()

IF(CHUCHO_HAVE_SQLITE)
    ADD_EXECUTABLE(sqlite-writer-test EXCLUDE_FROM_ALL
                   harness.cpp
                   sqlite_writer_test.cpp)
    TARGET_LINK_LIBRARIES(sqlite-writer-test chucho "${SQLITE_CLIENT_LIB}" gtest ${CHUCHO_THREAD_LIB})
ENDIF()

ADD_EXECUTABLE(configuration-off EXCLUDE_FROM_ALL
               configuration_off.cpp)
TARGET_LINK_LIBRARIES(configuration-off chucho ${CHUCHO_THREAD_LIB})

ADD_CUSTOM_TARGET(check-config
                  COMMAND "${CMAKE_COMMAND}" "-DCHUCHO_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/CheckAllConfigs.cmake")

ADD_CUSTOM_TARGET(all-test
                  DEPENDS unit-test rolling-file-test syslog-writer-test remote-writer-test windows-event-log-writer-test)
