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
    IF(INSTALL_SERVICE)
        CONFIGURE_FILE(platform/windows/InstallService.cmake.in
                       "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
    ENDIF()
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
        platform/posix/socket_life_cycle.cpp
        platform/posix/socket_listener_posix.cpp
        platform/posix/socket_reader_posix.cpp)
ELSEIF(CHUCHO_WINDOWS)
    SET(CHUCHOD_PLATFORM_SOURCES
        platform/windows/daemon_windows.cpp
        platform/windows/error_message.cpp
        platform/windows/error_message.hpp
        platform/windows/log_windows.cpp
        platform/windows/selector_windows.cpp
        platform/windows/signal_handler_windows.cpp
        platform/windows/single_instance_windows.cpp
        platform/windows/socket_life_cycle.cpp
        platform/windows/socket_listener_windows.cpp
        platform/windows/socket_reader_windows.cpp)
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
               socket_life_cycle.hpp
               socket_listener.hpp
               socket_reader.cpp
               socket_reader.hpp
               suzerain.cpp
               suzerain.hpp
               vassals.cpp
               vassals.hpp
               ${CHUCHOD_PLATFORM_SOURCES})

SET_SOURCE_FILES_PROPERTIES(suzerain.cpp PROPERTIES
                            COMPILE_DEFINITIONS YAML_DECLARE_STATIC)

TARGET_LINK_LIBRARIES(chuchod chucho ${CHUCHO_THREAD_LIB})
IF(CHUCHO_SOLARIS)
    TARGET_LINK_LIBRARIES(chuchod socket nsl)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_SOURCE_FILES_PROPERTIES(platform/posix/daemon_posix.cpp PROPERTIES
                                COMPILE_FLAGS "-Wno-unused-result")
ENDIF()

INSTALL(TARGETS chuchod
        RUNTIME DESTINATION bin)
INSTALL(FILES chuchod.yaml.sample
        DESTINATION etc)
IF(CHUCHO_WINDOWS AND INSTALL_SERVICE)
    INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
ENDIF()
