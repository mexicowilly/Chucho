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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)
PROJECT(chucho CXX C)

SET(CHUCHO_VERSION 1.4)
SET(CHUCHO_SO_VERSION 1)
# This is used for release candidate distros
# SET(CHUCHO_DIST_VERSION try-1)

INCLUDE(Configure.cmake)

# Include CMAKE_BINARY_DIR because that's where the generated file
# export.hpp lives.
INCLUDE_DIRECTORIES(include embedded/include "${CMAKE_BINARY_DIR}")

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
        platform/posix/syslog_writer_posix.cpp)

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
    IF(CHUCHO_HAVE_PUT_TIME)
        GET_SOURCE_FILE_PROPERTY(CHUCHO_CALENDAR_DEFS platform/posix/calendar_posix.cpp COMPILE_DEFINITIONS)
        SET_SOURCE_FILES_PROPERTIES(platform/posix/calendar_posix.cpp PROPERTIES
                                    COMPILE_DEFINITIONS "${CHUCHO_CALENDAR_DEFS};CHUCHO_HAVE_PUT_TIME")
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
        platform/windows/winsock_startup.cpp
        platform/windows/winsock_startup.hpp)
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
    include/chucho/diagnostic_context.hpp
    include/chucho/duplicate_message_filter.hpp
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

IF(CHUCHO_HAVE_ZLIB)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/gzip_file_compressor.hpp)
ENDIF()

IF(CHUCHO_HAVE_BZIP2)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/bzip2_file_compressor.hpp)
ENDIF()

IF(CHUCHO_HAVE_MINIZIP)
    LIST(APPEND CHUCHO_PUBLIC_HEADERS include/chucho/zip_file_compressor.hpp)
ENDIF()

# Configurator settings
IF(YAML_CONFIG)
    # Update this when you rev libyaml
    SET(CHUCHO_LIBYAML_VERSION_MAJOR 0)
    SET(CHUCHO_LIBYAML_VERSION_MINOR 1)
    SET(CHUCHO_LIBYAML_VERSION_PATCH 4)

    SET(CHUCHO_EMBEDDED_SOURCES
        embedded/libyaml/api.c
        embedded/libyaml/loader.c
        embedded/libyaml/parser.c
        embedded/libyaml/reader.c
        embedded/libyaml/scanner.c)

    IF(ENABLE_SHARED)
        SET(CHUCHO_DECLARE_YAML YAML_DECLARE_EXPORT)
    ELSE()
        SET(CHUCHO_DECLARE_YAML YAML_DECLARE_STATIC)
    ENDIF()
    SET_SOURCE_FILES_PROPERTIES(embedded/libyaml/api.c PROPERTIES
                                COMPILE_DEFINITIONS "YAML_VERSION_MAJOR=${CHUCHO_LIBYAML_VERSION_MAJOR};YAML_VERSION_MINOR=${CHUCHO_LIBYAML_VERSION_MINOR};YAML_VERSION_PATCH=${CHUCHO_LIBYAML_VERSION_PATCH};YAML_VERSION_STRING=\"${CHUCHO_LIB_YAML_VERSION_MAJOR}.${CHUCHO_LIBYAML_VERSION_MINOR}.${CHUCHO_LIBYAML_VERSION_PATCH}\";${CHUCHO_DECLARE_YAML}")
    SET_SOURCE_FILES_PROPERTIES(embedded/libyaml/loader.c
                                embedded/libyaml/parser.c
                                embedded/libyaml/reader.c
                                embedded/libyaml/scanner.c
                                PROPERTIES
                                COMPILE_DEFINITIONS ${CHUCHO_DECLARE_YAML})
    IF(MSVC)
        # Microsoft doesn't want libyaml to use strdup
        SET_SOURCE_FILES_PROPERTIES(embedded/libyaml/api.c PROPERTIES
                                    COMPILE_FLAGS /wd4996)
    ENDIF()

    SET(CHUCHO_CONFIGURATOR_SOURCES
        yaml_configurator.cpp
        yaml_parser.cpp
        include/chucho/utf8.hpp
        include/chucho/yaml_configurator.hpp
        include/chucho/yaml_parser.hpp)
    SET_SOURCE_FILES_PROPERTIES(configuration.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_YAML_CONFIG)
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
    include/chucho/utf8.hpp
    include/chucho/zip_file_compressor_factory.hpp
    ${CHUCHO_CONFIGURATOR_SOURCES}
    ${CHUCHO_PLATFORM_SOURCES}
    ${CHUCHO_EMBEDDED_SOURCES}
    ${CHUCHO_PUBLIC_HEADERS})

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
    SET_SOURCE_FILES_PROPERTIES(gzip_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_ZLIB)
    IF(ZLIB_SOURCE)
        INCLUDE_DIRECTORIES("${ZLIB_SOURCE}")
    ELSEIF(ZLIB_INCLUDE_DIR)
        INCLUDE_DIRECTORIES("${ZLIB_INCLUDE_DIR}")
    ENDIF()
    IF(CHUCHO_ZLIB_SOURCES)
        IF(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
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
    SET_SOURCE_FILES_PROPERTIES(bzip2_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_BZIP2)
    IF(BZIP2_SOURCE)
        INCLUDE_DIRECTORIES("${BZIP2_SOURCE}")
    ELSEIF(BZIP2_INCLUDE_DIR)
        INCLUDE_DIRECTORIES("${BZIP2_INCLUDE_DIR}")
    ENDIF()
    IF(MSVC)
        SET_SOURCE_FILES_PROPERTIES(bzip2_file_compressor PROPERTIES
                                    COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
        IF(CHUCHO_BZIP2_SOURCES)
            SET_SOURCE_FILES_PROPERTIES("${BZIP2_SOURCE}/bzlib.c" PROPERTIES
                                        COMPILE_FLAGS /wd4996)
        ENDIF()
    ENDIF()
ENDIF()

IF(CHUCHO_HAVE_MINIZIP)
    LIST(APPEND CHUCHO_SOURCES
         zip_file_compressor.cpp
         ${CHUCHO_MINIZIP_SOURCES})
    SET_SOURCE_FILES_PROPERTIES(zip_file_compressor_factory.cpp PROPERTIES
                                COMPILE_DEFINITIONS CHUCHO_HAVE_MINIZIP)
    INCLUDE_DIRECTORIES("${MINIZIP_SOURCE}")
    IF(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        SET_SOURCE_FILES_PROPERTIES("${MINIZIP_SOURCE}/zip.c"
                                    PROPERTIES
                                    COMPILE_FLAGS "-Wno-parentheses-equality -Wno-incompatible-pointer-types")
        IF(NOT CHUCHO_HAVE_FOPEN64)
            SET_SOURCE_FILES_PROPERTIES(${CHUCHO_MINIZIP_SOURCES}
                                        PROPERTIES
                                        COMPILE_DEFINITIONS USE_FILE32API)
        ENDIF()
    ENDIF()
ENDIF()

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

ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(log-server)

# Documentation
IF(DOXYGEN_FOUND)
    SET(CHUCHO_DOXYGEN_INPUT "\"${CMAKE_SOURCE_DIR}/main_page.hpp\"")
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
        FOREACH(HEAD ${CHUCHO_PUBLIC_HEADERS})
            SET(CHUCHO_DOXYGEN_INPUT "${CHUCHO_DOXYGEN_INPUT} ${HEAD}")
        ENDFOREACH()
        SET(CHUCHO_DOXYGEN_EXTRACT_ALL NO)
    ENDIF()
    CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/doxygen.conf" "${CMAKE_BINARY_DIR}/doxygen_mod.conf")
    ADD_CUSTOM_TARGET(doc
                      COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_BINARY_DIR}/doxygen_mod.conf"
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
FILE(GLOB_RECURSE CHUCHO_SOURCES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/*")
FILE(RELATIVE_PATH CHUCHO_REL_BINARY_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
UNSET(CHUCHO_TO_REMOVE)
FOREACH(SRC ${CHUCHO_SOURCES})
    IF(SRC MATCHES "^${CHUCHO_REL_BINARY_DIR}/" OR SRC MATCHES "\\.git" OR SRC MATCHES ".DS_Store$")
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
                  COMMAND "${CMAKE_COMMAND}" -E tar czvf ${CHUCHO_DIST_NAME_BASE}.tar.gz ${CHUCHO_DIST_NAME_BASE}
                  DEPENDS ${CHUCHO_DIST_OUTPUT})
IF(CHUCHO_ZIP)
    ADD_CUSTOM_TARGET(zdist
                      COMMAND "${CHUCHO_ZIP}" --recurse-paths --to-crlf ${CHUCHO_DIST_NAME_BASE}.zip ${CHUCHO_DIST_NAME_BASE}
                      DEPENDS ${CHUCHO_DIST_OUTPUT})
ENDIF()
