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

ADD_DEFINITIONS(-DGTEST_HAS_TR1_TUPLE=0 -Dchucho_EXPORTS)
INCLUDE_DIRECTORIES("${CHUCHO_EXTERNAL_PREFIX}/include")

ADD_EXECUTABLE(unit-test EXCLUDE_FROM_ALL
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
               yaml_configurator_test.cpp)
TARGET_LINK_LIBRARIES(unit-test chucho gtest ${CHUCHO_THREAD_LIB})

ADD_CUSTOM_TARGET(check
                  COMMAND unit-test)

ADD_EXECUTABLE(rolling-file-test EXCLUDE_FROM_ALL
               harness.cpp
               time_rolling_file_test.cpp)
TARGET_LINK_LIBRARIES(rolling-file-test chucho gtest)

ADD_EXECUTABLE(syslog-writer-test EXCLUDE_FROM_ALL
               harness.cpp
               syslog_writer_test.cpp)
TARGET_LINK_LIBRARIES(syslog-writer-test chucho gtest)

ADD_EXECUTABLE(remote-writer-test EXCLUDE_FROM_ALL
               remote_writer_test.cpp)
TARGET_LINK_LIBRARIES(remote-writer-test chucho)
