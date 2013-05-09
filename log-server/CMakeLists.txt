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

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

IF(CHUCHO_POSIX)
    SET(CHUCHOD_PLATFORM_SOURCES
        platform/posix/log_posix.cpp
        platform/posix/socket_reader_posix.cpp
        platform/posix/socket_listener_posix.cpp)
ENDIF()

ADD_EXECUTABLE(chuchod
               eof_exception.cpp
               eof_exception.hpp
               is_shut_down.cpp
               is_shut_down.hpp
               log.cpp
               log.hpp
               main.cpp
               properties.cpp
               properties.hpp
               socket_listener.hpp
               socket_reader.cpp
               socket_reader.hpp
               suzerain.cpp
               suzerain.hpp
               vassals.cpp
               vassals.hpp
               ${CHUCHOD_PLATFORM_SOURCES})
TARGET_LINK_LIBRARIES(chuchod chucho)
