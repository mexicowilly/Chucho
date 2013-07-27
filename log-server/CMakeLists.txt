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
ADD_DEFINITIONS(-DCHUCHO_VERSION=${CHUCHO_VERSION})
IF(CHUCHOD_VELOCITY)
    ADD_DEFINITIONS(-DCHUCHOD_VELOCITY)
ENDIF()

IF(CHUCHO_WINDOWS)
    ADD_DEFINITIONS(-DCHUCHO_WINDOWS)
ELSEIF(CHUCHO_POSIX)
    ADD_DEFINITIONS(-DCHUCHO_POSIX)
ENDIF()

GET_DIRECTORY_PROPERTY(CHUCHO_DIR_DEFS COMPILE_DEFINITIONS)
LIST(REMOVE_ITEM CHUCHO_DIR_DEFS CHUCHO_BUILD)
SET_DIRECTORY_PROPERTIES(PROPERTIES COMPILE_DEFINITIONS "${CHUCHO_DIR_DEFS}")

IF(CHUCHO_POSIX)
    SET(CHUCHOD_PLATFORM_SOURCES
        platform/posix/daemon_posix.cpp
        platform/posix/log_posix.cpp
        platform/posix/selector_posix.cpp
        platform/posix/signal_handler_posix.cpp
        platform/posix/single_instance_posix.cpp
        platform/posix/socket_reader_posix.cpp
        platform/posix/socket_listener_posix.cpp)
ELSEIF(CHUCHO_WINDOWS)
    SET(CHUCHOD_PLATFORM_SOURCES
        platform/windows/daemon_windows.cpp
        platform/windows/log_windows.cpp)
ENDIF()

ADD_EXECUTABLE(chuchod
               daemon.hpp
               eof_exception.cpp
               eof_exception.hpp
               is_shut_down.cpp
               is_shut_down.hpp
               log.cpp
               log.hpp
               main.cpp
               properties.cpp
               properties.hpp
               selector.cpp
               selector.hpp
               signal_handler.hpp
               single_instance.hpp
               socket_listener.hpp
               socket_reader.cpp
               socket_reader.hpp
               suzerain.cpp
               suzerain.hpp
               vassals.cpp
               vassals.hpp
               ${CHUCHOD_PLATFORM_SOURCES})
TARGET_LINK_LIBRARIES(chuchod chucho ${CHUCHO_THREAD_LIB})
INSTALL(TARGETS chuchod
        RUNTIME DESTINATION bin)
INSTALL(FILES chuchod.yaml.sample
        DESTINATION etc)
