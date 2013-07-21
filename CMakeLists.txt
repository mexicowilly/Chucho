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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(CHUCHO CXX C)

INCLUDE(Configure.cmake)

# Include CMAKE_BINARY_DIR because that's where the generated file
# export.hpp lives.
INCLUDE_DIRECTORIES(include embedded/include "${CMAKE_BINARY_DIR}")

SET(CHUCHO_VERSION 1.2)
SET(CHUCHO_SO_VERSION 1)
# This is used for release candidate distros
SET(CHUCHO_DIST_VERSION try-2)

IF(CHUCHO_POSIX)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/posix/calendar_posix.cpp
        platform/posix/diagnostic_context_posix.cpp
        platform/posix/file_posix.cpp
        platform/posix/host_posix.cpp
        platform/posix/line_ending_posix.cpp
        platform/posix/pattern_formatter_posix.cpp
        platform/posix/socket_connector_posix.cpp
        platform/posix/socket_exception_posix.cpp
        platform/posix/syslog_writer_posix.cpp)
ELSEIF(CHUCHO_WINDOWS)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/windows/calendar_windows.cpp
        platform/windows/error_util.cpp
        platform/windows/error_util.hpp
        platform/windows/file_windows.cpp
        platform/windows/host_windows.cpp
        platform/windows/line_ending_windows.cpp
        platform/windows/pattern_formatter_windows.cpp
        platform/windows/socket_connector_windows.cpp
        platform/windows/socket_exception_windows.cpp
        platform/windows/syslog_writer_windows.cpp
        platform/windows/winsock_startup.cpp
        platform/windows/winsock_startup.hpp)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_SOURCE_FILES_PROPERTIES(platform/posix/file_posix.cpp PROPERTIES
                                COMPILE_FLAGS "-Wno-unused-result")
ENDIF()

SET(CHUCHO_PUBLIC_HEADERS
    include/chucho/cerr_writer.hpp
    include/chucho/configurable.hpp
    include/chucho/configurable_factory.hpp
    include/chucho/configuration.hpp
    include/chucho/configurator.hpp
    include/chucho/console_writer.hpp
    include/chucho/cout_writer.hpp
    include/chucho/diagnostic_context.hpp
    include/chucho/duplicate_message_filter.hpp
    include/chucho/event.hpp
    include/chucho/exception.hpp
    "${CMAKE_BINARY_DIR}/chucho/export.hpp"
    include/chucho/file_roller.hpp
    include/chucho/file_roll_trigger.hpp
    include/chucho/file_writer.hpp
    include/chucho/filter.hpp
    include/chucho/finalize.hpp
    include/chucho/formatter.hpp
    include/chucho/level.hpp
    include/chucho/level_filter.hpp
    include/chucho/level_threshold_filter.hpp
    include/chucho/log.hpp
    include/chucho/logger.hpp
    include/chucho/marker.hpp
    include/chucho/memento.hpp
    include/chucho/non_copyable.hpp
    include/chucho/numbered_file_roller.hpp
	include/chucho/optional.hpp
    include/chucho/pattern_formatter.hpp
    include/chucho/remote_writer.hpp
    include/chucho/rolling_file_writer.hpp
    include/chucho/size_file_roll_trigger.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/syslog_constants.hpp
    include/chucho/syslog_writer.hpp
    include/chucho/time_file_roller.hpp
    include/chucho/writer.hpp
    include/chucho/writer_factory.hpp
    include/chucho/writer_memento.hpp
    include/chucho/yaml_formatter.hpp)

# Update this when you rev libyaml
SET(CHUCHO_LIBYAML_VERSION_MAJOR 0)
SET(CHUCHO_LIBYAML_VERSION_MINOR 1)
SET(CHUCHO_LIBYAML_VERSION_PATCH 4)

SET(CHUCHO_EMBEDDED_SOURCES
    embedded/trex/trex.c
    embedded/libyaml/api.c
    embedded/libyaml/loader.c
    embedded/libyaml/parser.c
    embedded/libyaml/reader.c
    embedded/libyaml/scanner.c)

SET_SOURCE_FILES_PROPERTIES(embedded/libyaml/api.c PROPERTIES
                            COMPILE_DEFINITIONS "YAML_VERSION_MAJOR=${CHUCHO_LIBYAML_VERSION_MAJOR};YAML_VERSION_MINOR=${CHUCHO_LIBYAML_VERSION_MINOR};YAML_VERSION_PATCH=${CHUCHO_LIBYAML_VERSION_PATCH};YAML_VERSION_STRING=\"${CHUCHO_LIB_YAML_VERSION_MAJOR}.${CHUCHO_LIBYAML_VERSION_MINOR}.${CHUCHO_LIBYAML_VERSION_PATCH}\";YAML_DECLARE_STATIC")
SET_SOURCE_FILES_PROPERTIES(embedded/libyaml/loader.c
                            embedded/libyaml/parser.c
                            embedded/libyaml/reader.c
                            embedded/libyaml/scanner.c
                            PROPERTIES
                            COMPILE_DEFINITIONS YAML_DECLARE_STATIC)
IF(CMAKE_C_COMPILER_ID STREQUAL Clang)
    SET_SOURCE_FILES_PROPERTIES(embedded/trex/trex.c PROPERTIES
                                COMPILE_FLAGS "-Wno-logical-op-parentheses -Wno-unused-value")
ENDIF()

SET(CHUCHO_SOURCES
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
    duplicate_message_filter.cpp
    duplicate_message_filter_factory.cpp
    duplicate_message_filter_memento.cpp
    event.cpp
    exception.cpp
    file_exception.cpp
    file_roller.cpp
    file_writer.cpp
    file_writer_factory.cpp
    file_writer_memento.cpp
    finalize.cpp
    garbage_cleaner.cpp
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
    status.cpp
    status_manager.cpp
    status_observer.cpp
    status_reporter.cpp
    syslog_constants.cpp
    syslog_writer.cpp
    syslog_writer_factory.cpp
    syslog_writer_memento.cpp
    time_file_roller.cpp
    time_file_roller_factory.cpp
    time_file_roller_memento.cpp
    utf8.cpp
    writer.cpp
    writer_factory.cpp
    writer_memento.cpp
    yaml_configurator.cpp
    yaml_formatter.cpp
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
    include/chucho/exception.hpp
    include/chucho/file.hpp
    include/chucho/file_exception.hpp
    include/chucho/garbage_cleaner.hpp
    include/chucho/host.hpp
    include/chucho/level_filter_factory.hpp
    include/chucho/level_filter_memento.hpp
    include/chucho/level_threshold_filter_factory.hpp
    include/chucho/level_threshold_filter_memento.hpp
    include/chucho/line_ending.hpp
    include/chucho/logger_factory.hpp
    include/chucho/logger_memento.hpp
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
    include/chucho/socket_connector.hpp
    include/chucho/socket_exception.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/syslog_writer_factory.hpp
    include/chucho/syslog_writer_memento.hpp
    include/chucho/time_file_roller_factory.hpp
    include/chucho/time_file_roller_memento.hpp
    include/chucho/utf8.hpp
    include/chucho/yaml_configurator.hpp
    ${CHUCHO_PLATFORM_SOURCES}
    ${CHUCHO_EMBEDDED_SOURCES}
    ${CHUCHO_PUBLIC_HEADERS})

IF(ENABLE_SHARED)
    ADD_LIBRARY(chucho SHARED ${CHUCHO_SOURCES})
    SET_TARGET_PROPERTIES(chucho PROPERTIES
                          COMPILE_FLAGS "${CHUCHO_CXX_SO_FLAGS}"
                          VERSION ${CHUCHO_VERSION}
                          SOVERSION ${CHUCHO_SO_VERSION}
                          PUBLIC_HEADER "${CHUCHO_PUBLIC_HEADERS}"
                          FRAMEWORK ${ENABLE_FRAMEWORK})
    IF(CMAKE_GENERATOR STREQUAL Xcode AND CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        SET_TARGET_PROPERTIES(chucho PROPERTIES
                              LINK_FLAGS "-std=c++11 -stdlib=libc++")
    ENDIF()
    INSTALL(TARGETS chucho
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION lib
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

ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(log-server)

# Documentation
IF(DOXYGEN_FOUND)
    SET(CHUCHO_DOXYGEN_INPUT \"${CMAKE_SOURCE_DIR}/main_page.hpp\")
	FOREACH(HEAD ${CHUCHO_PUBLIC_HEADERS})
		SET(CHUCHO_DOXYGEN_INPUT "${CHUCHO_DOXYGEN_INPUT} \"${CMAKE_SOURCE_DIR}/${HEAD}\"")
	ENDFOREACH()
	CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/doxygen.conf" "${CMAKE_BINARY_DIR}/doxygen_mod.conf")
	ADD_CUSTOM_TARGET(doc
					  COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_BINARY_DIR}/doxygen_mod.conf"
					  WORKING_DIRECTORY "${CHUCHO_SOURCE_DIR}")
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
FILE(GLOB_RECURSE CHUCHO_SOURCES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/*")
FILE(RELATIVE_PATH CHUCHO_REL_BINARY_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
FOREACH(SRC ${CHUCHO_SOURCES})
    IF(SRC MATCHES "^${CHUCHO_REL_BINARY_DIR}" OR SRC MATCHES ".git" OR SRC MATCHES ".DS_Store")
        LIST(APPEND CHUCHO_TO_REMOVE "${SRC}")
    ENDIF()
ENDFOREACH()
LIST(REMOVE_ITEM CHUCHO_SOURCES ${CHUCHO_TO_REMOVE})
STRING(TOLOWER ${CMAKE_PROJECT_NAME} CHUCHO_LOWER_PROJECT_NAME)
SET(CHUCHO_DIST_NAME_BASE "${CMAKE_BINARY_DIR}/${CHUCHO_LOWER_PROJECT_NAME}-${CHUCHO_VERSION}")
IF(DEFINED CHUCHO_DIST_VERSION)
    SET(CHUCHO_DIST_NAME_BASE "${CHUCHO_DIST_NAME_BASE}-${CHUCHO_DIST_VERSION}")
ENDIF()
FOREACH(SRC ${CHUCHO_SOURCES})
    SET(CHUCHO_SRC_OUT "${CHUCHO_DIST_NAME_BASE}/${SRC}")
    LIST(APPEND CHUCHO_DIST_OUTPUT "${CHUCHO_SRC_OUT}")
    ADD_CUSTOM_COMMAND(OUTPUT "${CHUCHO_SRC_OUT}"
                       COMMAND "${CMAKE_COMMAND}" -E copy "${SRC}" "${CHUCHO_SRC_OUT}"
                       DEPENDS "${SRC}"
                       WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
ENDFOREACH()
ADD_CUSTOM_TARGET(dist
                  COMMAND "${CMAKE_COMMAND}" -E tar czvf "${CHUCHO_DIST_NAME_BASE}.tar.gz" "${CHUCHO_DIST_NAME_BASE}"
                  DEPENDS ${CHUCHO_DIST_OUTPUT})
