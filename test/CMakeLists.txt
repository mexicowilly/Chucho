#
# Copyright 2013 Will Mason
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

ADD_DEFINITIONS(-DGTEST_HAS_TR1_TUPLE=0)
IF(CHUCHO_POSIX)
    ADD_DEFINITIONS(-DCHUCHO_POSIX)
ELSEIF(CHUCHO_WINDOWS)
    ADD_DEFINITIONS(-DCHUCHO_WINDOWS)
ENDIF()
INCLUDE_DIRECTORIES("${CHUCHO_EXTERNAL_PREFIX}/include")

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

ADD_EXECUTABLE(unit-test EXCLUDE_FROM_ALL
               async_writer_test.cpp
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
               logger_test.cpp
               log_macro_test.cpp
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

IF(MSVC)
    SET_SOURCE_FILES_PROPERTIES(pattern_formatter_test.cpp PROPERTIES
                                COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    SET_SOURCE_FILES_PROPERTIES(rolling_file_writer_test.cpp PROPERTIES
                                COMPILE_FLAGS /wd4018)
ENDIF()

ADD_CUSTOM_TARGET(check
                  COMMAND unit-test)

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

ADD_CUSTOM_TARGET(all-test
                  DEPENDS unit-test rolling-file-test syslog-writer-test remote-writer-test)
