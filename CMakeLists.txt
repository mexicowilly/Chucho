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
PROJECT(chuhco CXX C)

INCLUDE(Configure.cmake)

INCLUDE_DIRECTORIES(include embedded/include)

SET(CHUCHO_VERSION 1.1)
SET(CHUCHO_SO_VERSION 1)
SET(CHUCHO_DIST_VERSION 1.1-try-1)

IF(CHUCHO_POSIX)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/posix/calendar_posix.cpp
        platform/posix/diagnostic_context_posix.cpp
        platform/posix/file_posix.cpp
        platform/posix/host_posix.cpp
        platform/posix/line_ending_posix.cpp
        platform/posix/pattern_formatter_posix.cpp
        platform/posix/syslog_writer_posix.cpp)
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
    include/chucho/export.hpp
    include/chucho/file_roller.hpp
    include/chucho/file_roll_trigger.hpp
    include/chucho/file_writer.hpp
    include/chucho/filter.hpp
    include/chucho/formatter.hpp
    include/chucho/level.hpp
    include/chucho/level_filter.hpp
    include/chucho/level_threshold_filter.hpp
    include/chucho/log.hpp
    include/chucho/logger.hpp
    include/chucho/marker.hpp
    include/chucho/memento.hpp
    include/chucho/numbered_file_roller.hpp
	include/chucho/optional.hpp
    include/chucho/pattern_formatter.hpp
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
    include/chucho/writer_memento.hpp)

SET(CHUCHO_EMBEDDED_SOURCES
    embedded/trex/trex.c
    embedded/yaml-cpp/aliasmanager.cpp
    embedded/yaml-cpp/binary.cpp
    embedded/yaml-cpp/conversion.cpp
    embedded/yaml-cpp/directives.cpp
    embedded/yaml-cpp/emitfromevents.cpp
    embedded/yaml-cpp/emitter.cpp
    embedded/yaml-cpp/emitterstate.cpp
    embedded/yaml-cpp/emitterutils.cpp
    embedded/yaml-cpp/exp.cpp
    embedded/yaml-cpp/iterator.cpp
    embedded/yaml-cpp/node.cpp
    embedded/yaml-cpp/null.cpp
    embedded/yaml-cpp/nodebuilder.cpp
    embedded/yaml-cpp/nodeownership.cpp
    embedded/yaml-cpp/ostream.cpp
    embedded/yaml-cpp/parser.cpp
    embedded/yaml-cpp/regex.cpp
    embedded/yaml-cpp/scanner.cpp
    embedded/yaml-cpp/scanscalar.cpp
    embedded/yaml-cpp/scantag.cpp
    embedded/yaml-cpp/scantoken.cpp
    embedded/yaml-cpp/simplekey.cpp
    embedded/yaml-cpp/singledocparser.cpp
    embedded/yaml-cpp/stream.cpp
    embedded/yaml-cpp/tag.cpp)

IF(CMAKE_C_COMPILER_ID STREQUAL Clang)
    SET_SOURCE_FILES_PROPERTIES(embedded/trex/trex.c PROPERTIES
                                COMPILE_FLAGS "-Wno-logical-op-parentheses -Wno-unused-value")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET_SOURCE_FILES_PROPERTIES(embedded/yaml-cpp/emitter.cpp
                                embedded/yaml-cpp/emitterstate.cpp
                                embedded/yaml-cpp/iterator.cpp
                                embedded/yaml-cpp/node.cpp
                                embedded/yaml-cpp/scanner.cpp
                                PROPERTIES
                                COMPILE_FLAGS "-Wno-deprecated-declarations")
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
    syslog_writer.cpp
    syslog_writer_factory.cpp
    syslog_writer_memento.cpp
    time_file_roller.cpp
    time_file_roller_factory.cpp
    time_file_roller_memento.cpp
    utf8_validator.cpp
    writer.cpp
    writer_factory.cpp
    writer_memento.cpp
    yaml_configurator.cpp
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
    include/chucho/printf_util.hpp
    include/chucho/regex.hpp
    include/chucho/regex_exception.hpp
    include/chucho/rolling_file_writer_factory.hpp
    include/chucho/rolling_file_writer_memento.hpp
    include/chucho/size_file_roll_trigger_factory.hpp
    include/chucho/size_file_roll_trigger_memento.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/syslog_writer_factory.hpp
    include/chucho/syslog_writer_memento.hpp
    include/chucho/time_file_roller_factory.hpp
    include/chucho/time_file_roller_memento.hpp
    include/chucho/utf8_validator.hpp
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
    ADD_DEFINITIONS(-Dchucho_EXPORTS)
    INSTALL(TARGETS chucho
            ARCHIVE DESTINATION lib
            PUBLIC_HEADER DESTINATION include/chucho)
ENDIF()

ADD_SUBDIRECTORY(test)

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

# Distro
FILE(GLOB_RECURSE CHUCHO_SOURCES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/*")
FILE(RELATIVE_PATH CHUCHO_REL_BINARY_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
FOREACH(SRC ${CHUCHO_SOURCES})
    IF(SRC MATCHES "^${CHUCHO_REL_BINARY_DIR}" OR SRC MATCHES ".git" OR SRC MATCHES ".DS_Store")
        LIST(APPEND CHUCHO_TO_REMOVE "${SRC}")
    ENDIF()
ENDFOREACH()
LIST(REMOVE_ITEM CHUCHO_SOURCES ${CHUCHO_TO_REMOVE})
SET(CHUCHO_DIST_NAME_BASE "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}-${CHUCHO_DIST_VERSION}")
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
