#
# Copyright 2013-2015 Will Mason
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
IF(CHUCHOD_VELOCITY)
    ADD_DEFINITIONS(-DCHUCHOD_VELOCITY)
ENDIF()

IF(CHUCHO_WINDOWS)
    ADD_DEFINITIONS(-DCHUCHO_WINDOWS)
    IF(INSTALL_SERVICE)
        CONFIGURE_FILE(platform/windows/InstallService.cmake.in
                       "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
    ENDIF()
    CONFIGURE_FILE(platform/windows/UninstallService.cmake.in
                   "${CMAKE_CURRENT_BINARY_DIR}/UninstallService.cmake")
ELSEIF(CHUCHO_SOLARIS)
    IF(INSTALL_SERVICE)
        CONFIGURE_FILE(platform/solaris/smf-manifest.xml.in
                       "${CMAKE_CURRENT_BINARY_DIR}/chucho.xml")
        EXECUTE_PROCESS(COMMAND "${CHUCHO_SVCCFG}" validate "${CMAKE_CURRENT_BINARY_DIR}/chucho.xml"
                        RESULT_VARIABLE CHUCHO_SVCCFG_RESULT)
        IF(NOT CHUCHO_SVCCFG_RESULT EQUAL 0)
            MESSAGE(FATAL_ERROR "Error validating the SMF manifest for the Chucho service")
        ENDIF()
        CONFIGURE_FILE(platform/solaris/InstallService.cmake.in
                       "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
    ENDIF()
    CONFIGURE_FILE(platform/solaris/UninstallService.cmake.in
                   "${CMAKE_CURRENT_BINARY_DIR}/UninstallService.cmake")
ELSEIF(CHUCHO_MACINTOSH)
    IF(INSTALL_SERVICE)
        CONFIGURE_FILE(platform/macintosh/org.chucho-log.chuchod.plist.in
                       "${CMAKE_CURRENT_BINARY_DIR}/org.chucho-log.chuchod.plist")
        CONFIGURE_FILE(platform/macintosh/InstallService.cmake.in
                       "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
    ENDIF()
    CONFIGURE_FILE(platform/macintosh/UninstallService.cmake.in
                   "${CMAKE_CURRENT_BINARY_DIR}/UninstallService.cmake")
ELSEIF(CHUCHO_UPSTART_INIT)
    IF(INSTALL_SERVICE)
        CONFIGURE_FILE(platform/linux/upstart.conf.in
                       "${CMAKE_CURRENT_BINARY_DIR}/chuchod.conf")
        CONFIGURE_FILE(platform/linux/InstallUpstartService.cmake.in
                       "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
    ENDIF()
    CONFIGURE_FILE(platform/linux/UninstallUpstartService.cmake.in
                   "${CMAKE_CURRENT_BINARY_DIR}/UninstallService.cmake")
ENDIF()

IF(CHUCHO_POSIX)
    ADD_DEFINITIONS(-DCHUCHO_POSIX)
ENDIF()

GET_DIRECTORY_PROPERTY(CHUCHO_DIR_DEFS COMPILE_DEFINITIONS)
LIST(REMOVE_ITEM CHUCHO_DIR_DEFS CHUCHO_BUILD)
SET_DIRECTORY_PROPERTIES(PROPERTIES COMPILE_DEFINITIONS "${CHUCHO_DIR_DEFS}")

IF(CHUCHO_POSIX)
    IF(CHUCHO_MACINTOSH AND INSTALL_SERVICE)
        SET(CHUCHOD_POSIX_DAEMON platform/macintosh/daemon_macintosh.cpp)
        SET(CHUCHOD_POSIX_LISTENER platform/macintosh/socket_listener_macintosh.cpp)
        IF(CMAKE_SYSTEM_VERSION VERSION_GREATER 13)
            SET_SOURCE_FILES_PROPERTIES(platform/macintosh/socket_listener_macintosh.cpp
                                        PROPERTIES
                                        COMPILE_FLAGS -Wno-deprecated-declarations)
        ENDIF()
    ELSE()
        SET(CHUCHOD_POSIX_DAEMON platform/posix/daemon_posix.cpp)
        SET(CHUCHOD_POSIX_LISTENER platform/posix/socket_listener_posix.cpp)
    ENDIF()
    SET(CHUCHOD_PLATFORM_SOURCES
        platform/posix/log_posix.cpp
        platform/posix/selector_posix.cpp
        platform/posix/signal_handler_posix.cpp
        platform/posix/single_instance_posix.cpp
        platform/posix/socket_life_cycle.cpp
        platform/posix/socket_reader_posix.cpp
        ${CHUCHOD_POSIX_DAEMON}
        ${CHUCHOD_POSIX_LISTENER})
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

CHUCHO_SET_YAML_SOURCES(CHUCHOD_YAML_SOURCES)

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
               ${CHUCHOD_PLATFORM_SOURCES}
               ${CHUCHOD_YAML_SOURCES})

IF(CHUCHO_HAVE_ARPA_INET_H)
    SET_SOURCE_FILES_PROPERTIES(suzerain.cpp PROPERTIES
                                COMPILE_DEFINITIONS "YAML_DECLARE_STATIC;CHUCHO_HAVE_ARPA_INET_H")
ELSE()
    SET_SOURCE_FILES_PROPERTIES(suzerain.cpp PROPERTIES
                                COMPILE_DEFINITIONS YAML_DECLARE_STATIC)
ENDIF()

IF(CHUCHO_HAVE_WINSOCK2_H)
    GET_SOURCE_FILE_PROPERTY(CHUCHO_SUZERAIN_DEFS suzerain.cpp COMPILE_DEFINITIONS)
    SET_SOURCE_FILES_PROPERTIES(suzerain.cpp PROPERTIES
                                COMPILE_DEFINITIONS "${CHUCHO_SUZERAIN_DEFS};CHUCHO_HAVE_WINSOCK2_H")
ENDIF()

TARGET_LINK_LIBRARIES(chuchod chucho ${CMAKE_THREAD_LIBS_INIT})
IF(CURL_FOUND)
    TARGET_LINK_LIBRARIES(chuchod ${CURL_LIBRARIES})
ENDIF()
IF(CHUCHO_SOLARIS)
    TARGET_LINK_LIBRARIES(chuchod socket nsl)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_SOURCE_FILES_PROPERTIES(platform/posix/daemon_posix.cpp PROPERTIES
                                COMPILE_FLAGS "-Wno-unused-result")
ENDIF()

IF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/UninstallService.cmake")
    INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/UninstallService.cmake")
ENDIF()
INSTALL(TARGETS chuchod
        RUNTIME DESTINATION bin)
INSTALL(FILES chuchod.yaml.sample
        DESTINATION etc)
IF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
    IF(CHUCHO_SOLARIS)
        INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/chucho.xml"
                DESTINATION /var/svc/manifest/application)
    ELSEIF(CHUCHO_MACINTOSH)
        INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/org.chucho-log.chuchod.plist"
                DESTINATION /Library/LaunchDaemons)
    ELSEIF(CHUCHO_UPSTART_INIT)
        INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/chuchod.conf"
                DESTINATION /etc/init)
    ENDIF()
    INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/InstallService.cmake")
ENDIF()