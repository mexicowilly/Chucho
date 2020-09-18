#
# Copyright 2013-2020 Will Mason
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
    SET_DIRECTORY_PROPERTIES(PROPERTIES COMPILE_OPTIONS "/wd4996")
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

IF(PROTOBUF_FOUND)
    LIST(APPEND CHUCHO_TEST_SERIALIZER_SOURCES protobuf_serializer_test.cpp)
ENDIF()

IF(CHUCHO_HAVE_CAPN_PROTO)
    LIST(APPEND CHUCHO_TEST_SERIALIZER_SOURCES capn_proto_serializer_test.cpp)
ENDIF()

IF(CHUCHO_HAVE_FLATBUFFERS)
    LIST(APPEND CHUCHO_TEST_SERIALIZER_SOURCES flatbuffers_serializer_test.cpp)
ENDIF()

IF(CHUCHO_HAVE_ZEROMQ)
    LIST(APPEND CHUCHO_TEST_MQ_SOURCES zeromq_writer_test.cpp)
ENDIF()

IF(CHUCHO_HAVE_ACTIVEMQ)
    LIST(APPEND CHUCHO_TEST_MQ_SOURCES activemq_writer_test.cpp)
ENDIF()

IF(CHUCHO_HAVE_RABBITMQ)
    LIST(APPEND CHUCHO_TEST_MQ_SOURCES rabbitmq_writer_test.cpp)
ENDIF()

IF(CHUCHO_HAVE_RDKAFKA)
    LIST(APPEND CHUCHO_TEST_MQ_SOURCES kafka_writer_test.cpp)
ENDIF()

IF(ZLIB_FOUND)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES
         gzip_file_compressor_test.cpp
         zlib_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_ZLIB)
ENDIF()

IF(BZIP2_FOUND)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES
         bzip2_compressor_test.cpp
         bzip2_file_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_BZIP2)
ENDIF()

IF(LibArchive_FOUND)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES zip_file_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_LIBARCHIVE)
ENDIF()

IF(LIBLZMA_FOUND)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES
         lzma_file_compressor_test.cpp
         lzma_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_LZMA)
ENDIF()

IF(CHUCHO_HAVE_LZ4)
    LIST(APPEND CHUCHO_TEST_COMPRESSION_SOURCES
         lz4_compressor_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_LZ4)
ENDIF()

IF(YAML_CONFIG)
    LIST(APPEND CHUCHO_CONFIGURATOR_TESTS yaml_configurator_test.cpp utf8_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_YAML_CONFIG)
ENDIF()

IF(JSON_CONFIG)
    LIST(APPEND CHUCHO_CONFIGURATOR_TESTS json_configurator_test.cpp utf8_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_JSON_CONFIG)
ENDIF()

IF(CONFIG_FILE_CONFIG OR LOG4CPLUS_CONFIG)
    LIST(APPEND CHUCHO_CONFIGURATOR_TESTS properties_test.cpp)
    IF(CONFIG_FILE_CONFIG)
        LIST(APPEND CHUCHO_CONFIGURATOR_TESTS chucho_config_file_configurator_test.cpp)
        ADD_DEFINITIONS(-DCHUCHO_CONFIG_FILE_CONFIG)
    ENDIF()
    IF(LOG4CPLUS_CONFIG)
        LIST(APPEND CHUCHO_CONFIGURATOR_TESTS log4cplus_config_file_configurator_test.cpp)
        ADD_DEFINITIONS(-DCHUCHO_LOG4CPLUS_CONFIG)
    ENDIF()
ENDIF()

IF(CHUCHO_HAVE_SOCI)
    LIST(APPEND CHUCHO_TEST_DB_SOURCES database_writer_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_SOCI)
ENDIF()

IF(RUBY_FOUND)
    LIST(APPEND CHUCHO_TEST_EVALUATOR_SOURCES ruby_evaluator_filter_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_RUBY)
ENDIF()

IF(CURL_FOUND)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_CURL)
ENDIF()

IF(CHUCHO_HAVE_ZEROMQ)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_ZEROMQ)
ENDIF()

IF(PROTOBUF_FOUND)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_PROTOBUF)
ENDIF()

IF(CHUCHO_HAVE_CAPN_PROTO)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_CAPN_PROTO)
ENDIF()

IF(CHUCHO_HAVE_FLATBUFFERS)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_FLATBUFFERS)
ENDIF()

IF(CHUCHO_HAVE_ACTIVEMQ)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_ACTIVEMQ)
ENDIF()

IF(CHUCHO_HAVE_RABBITMQ)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_RABBITMQ)
ENDIF()

IF(CHUCHO_HAVE_RDKAFKA)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_RDKAFKA)
ENDIF()

IF(CHUCHO_HAVE_DOORS)
    LIST(APPEND CHUCHO_TEST_DOOR_SOURCES door_writer_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_DOORS)
ENDIF()

IF(CURL_FOUND)
    LIST(APPEND CHUCHO_TEST_CURL_SOURCES loggly_writer_test.cpp)
ENDIF()

IF(AWSSDK_FOUND)
    LIST(APPEND CHUCHO_TEST_AWS_SOURCES cloudwatch_writer_test.cpp)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_AWSSDK)
ENDIF()

IF(CHUCHO_PRIV_EXPORT)
    SET(CHUCHO_TEST_EMBEDDED_SOURCES ../embedded/cJSON/cJSON.c)
    CHUCHO_SET_YAML_SOURCES(CHUCHO_TEST_EMBEDDED_SOURCES)
ENDIF()

ADD_EXECUTABLE(unit-test EXCLUDE_FROM_ALL
               async_writer_test.cpp
               c_test.cpp
               cache_and_release_filter_test.cpp
               calendar_test.cpp
               configuration_test.cpp
               configurator_test.cpp
               configurator_test.hpp
               diagnostic_context_test.cpp
               duplicate_message_filter_test.cpp
               event_cache_test.cpp
               file_test.cpp
               file_descriptor_writer_test.cpp
               file_writer_test.cpp
               formatted_message_serializer_test.cpp
               harness.cpp
               interval_file_roll_trigger_test.cpp
               json_formatter_test.cpp
               level_test.cpp
               level_filter_test.cpp
               level_threshold_filter_test.cpp
               log_macro_test.cpp
               log_stream_test.cpp
               loggable_test.cpp
               logger_test.cpp
               marker_test.cpp
               multithread_test.cpp
               named_pipe_writer_test.cpp
               noop_compressor_test.cpp
               on_start_file_roll_trigger_test.cpp
               optional_test.cpp
               optional_features_test.cpp
               pattern_formatter_test.cpp
               pipe_writer_test.cpp
               regex_test.cpp
               rolling_file_writer_test.cpp
               security_policy_test.cpp
               size_file_roll_trigger_test.cpp
               streamable_test.cpp
               text_util_test.cpp
               utf8_test.cpp
               yaml_formatter_test.cpp
               ${CHUCHO_TEST_EMBEDDED_SOURCES}
               ${CHUCHO_TEST_COMPRESSION_SOURCES}
               ${CHUCHO_CONFIGURATOR_TESTS}
               ${CHUCHO_TEST_EVALUATOR_SOURCES}
               ${CHUCHO_TEST_SERIALIZER_SOURCES}
               ${CHUCHO_TEST_MQ_SOURCES}
               ${CHUCHO_TEST_DOOR_SOURCES}
               ${CHUCHO_TEST_CURL_SOURCES}
               ${CHUCHO_TEST_AWS_SOURCES}
               ${CHUCHO_TEST_DB_SOURCES})
TARGET_LINK_LIBRARIES(unit-test chucho gtest)

IF(MSVC)
    SET_SOURCE_FILES_PROPERTIES(pattern_formatter_test.cpp PROPERTIES
                                COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    SET_SOURCE_FILES_PROPERTIES(rolling_file_writer_test.cpp PROPERTIES
                                COMPILE_FLAGS /wd4018)
ENDIF()

IF(CHUCHO_HAVE_C_GENERIC)
    SET_TARGET_PROPERTIES(unit-test PROPERTIES
                          COMPILE_DEFINITIONS CHUCHO_HAVE_C_GENERIC)
ENDIF()

ADD_EXECUTABLE(named-pipe-writer-test-helper EXCLUDE_FROM_ALL
               named_pipe_writer_test_helper.cpp)
TARGET_LINK_LIBRARIES(named-pipe-writer-test-helper chucho)
ADD_DEPENDENCIES(unit-test named-pipe-writer-test-helper)

ADD_EXECUTABLE(c-test-helper EXCLUDE_FROM_ALL
               c_test_helper.c)
TARGET_LINK_LIBRARIES(c-test-helper chucho ${CHUCHO_STD_CXX_LIBS})
ADD_DEPENDENCIES(unit-test c-test-helper)
SET_TARGET_PROPERTIES(c-test-helper PROPERTIES LINKER_LANGUAGE C)

IF(CHUCHO_HAVE_ZEROMQ)
    ADD_EXECUTABLE(zeromq-writer-test-helper EXCLUDE_FROM_ALL
                   zeromq_writer_test_helper.cpp)
    TARGET_LINK_LIBRARIES(zeromq-writer-test-helper chucho)
    ADD_DEPENDENCIES(unit-test zeromq-writer-test-helper)
ENDIF()

IF(CHUCHO_HAVE_ACTIVEMQ)
    ADD_EXECUTABLE(activemq-writer-test-helper EXCLUDE_FROM_ALL
                   activemq_writer_test_helper.cpp)
    TARGET_LINK_LIBRARIES(activemq-writer-test-helper chucho)
    ADD_DEPENDENCIES(unit-test activemq-writer-test-helper)
ENDIF()

IF(CHUCHO_HAVE_RABBITMQ)
    ADD_EXECUTABLE(rabbitmq-writer-test-helper EXCLUDE_FROM_ALL
                   rabbitmq_writer_test_helper.cpp)
    TARGET_LINK_LIBRARIES(rabbitmq-writer-test-helper chucho)
    ADD_DEPENDENCIES(unit-test rabbitmq-writer-test-helper)
ENDIF()

IF(CHUCHO_HAVE_RDKAFKA)
    ADD_EXECUTABLE(kafka-writer-test-helper EXCLUDE_FROM_ALL
                   kafka_writer_test_helper.cpp)
    TARGET_LINK_LIBRARIES(kafka-writer-test-helper chucho)
    ADD_DEPENDENCIES(unit-test kafka-writer-test-helper)
ENDIF()

IF(CHUCHO_HAVE_DOORS)
    ADD_EXECUTABLE(door-writer-test-helper EXCLUDE_FROM_ALL
                   door_writer_test_helper.cpp)
    TARGET_LINK_LIBRARIES(door-writer-test-helper chucho)
    ADD_DEPENDENCIES(unit-test door-writer-test-helper)
ENDIF()

ADD_CUSTOM_TARGET(check
                  COMMAND unit-test
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

IF(CHUCHO_WINDOWS)
    FILE(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}" CHUCHO_EVENT_LOG_DLL)
    SET(CHUCHO_EVENT_LOG_DLL "${CHUCHO_EVENT_LOG_DLL}\\event_log.dll")
    STRING(REPLACE "\\" "\\\\" CHUCHO_EVENT_LOG_DLL "${CHUCHO_EVENT_LOG_DLL}")
ENDIF()

ADD_EXECUTABLE(time-rolling-file-test EXCLUDE_FROM_ALL
               harness.cpp
               time_rolling_file_test.cpp)
TARGET_LINK_LIBRARIES(time-rolling-file-test chucho gtest)

ADD_EXECUTABLE(syslog-writer-test EXCLUDE_FROM_ALL
               harness.cpp
               syslog_writer_test.cpp)
TARGET_LINK_LIBRARIES(syslog-writer-test chucho gtest)

IF(CHUCHO_WINDOWS)
    ADD_EXECUTABLE(windows-event-log-writer-test EXCLUDE_FROM_ALL
                   harness.cpp
                   windows_event_log_writer_test.cpp)
    SET_SOURCE_FILES_PROPERTIES(windows_event_log_writer_test.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_EVENT_LOG_DLL="${CHUCHO_EVENT_LOG_DLL}")
    TARGET_LINK_LIBRARIES(windows-event-log-writer-test chucho gtest)
ENDIF()

IF(CURL_FOUND)
    ADD_EXECUTABLE(email-writer-test EXCLUDE_FROM_ALL
                   harness.cpp
                   email_writer_test.cpp)
    TARGET_LINK_LIBRARIES(email-writer-test chucho gtest)
ENDIF()

ADD_EXECUTABLE(configuration-off EXCLUDE_FROM_ALL
               configuration_off.cpp)
TARGET_LINK_LIBRARIES(configuration-off chucho)

ADD_CUSTOM_TARGET(check-config
                  COMMAND "${CMAKE_COMMAND}" "-DCHUCHO_SOURCE_DIR=${CMAKE_SOURCE_DIR}" -DCLANG_LIBSTDCXX=${CLANG_LIBSTDCXX} -P "${CMAKE_CURRENT_SOURCE_DIR}/CheckAllConfigs.cmake")

ADD_CUSTOM_TARGET(all-test
                  DEPENDS unit-test time-rolling-file-test syslog-writer-test windows-event-log-writer-test)
