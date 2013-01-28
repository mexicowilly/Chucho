CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(chuhco CXX)

INCLUDE(Configure.cmake)

INCLUDE_DIRECTORIES(include "${CHUCHO_EXTERNAL_PREFIX}/include")
LINK_DIRECTORIES("${CHUCHO_EXTERNAL_PREFIX}/lib")

SET(CHUCHO_VERSION 1.1)
SET(CHUCHO_SOVERSION 1)

IF(CHUCHO_POSIX)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/posix/calendar_posix.cpp
        platform/posix/diagnostic_context_posix.cpp
        platform/posix/file_posix.cpp
        platform/posix/line_ending_posix.cpp
        platform/posix/pattern_formatter_posix.cpp)
ENDIF()

SET(CHUCHO_PUBLIC_HEADERS
    include/chucho/cerr_writer.hpp
    include/chucho/configurable.hpp
    include/chucho/configurable_factory.hpp
    include/chucho/configuration.hpp
    include/chucho/console_writer.hpp
    include/chucho/cout_writer.hpp
    include/chucho/diagnostic_context.hpp
    include/chucho/event.hpp
    include/chucho/export.hpp
    include/chucho/file_roller.hpp
    include/chucho/file_roll_trigger.hpp
    include/chucho/file_writer.hpp
    include/chucho/filter.hpp
    include/chucho/formatter.hpp
    include/chucho/level.hpp
    include/chucho/level_threshold_filter.hpp
    include/chucho/logger.hpp
    include/chucho/marker.hpp
    include/chucho/memento.hpp
    include/chucho/numbered_file_roller.hpp
    include/chucho/pattern_formatter.hpp
    include/chucho/rolling_file_writer.hpp
    include/chucho/size_file_roll_trigger.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/time_file_roller.hpp
    include/chucho/writer.hpp
    include/chucho/writer_factory.hpp
    include/chucho/writer_memento.hpp)

SET(CHUCHO_SOURCES
    cerr_writer.cpp
    cerr_writer_factory.cpp
    cerr_writer_memento.cpp
    configurable.cpp
    configurable_factory.cpp
    configuration.cpp
    configurator.cpp
    console_writer.cpp
    cout_writer.cpp
    cout_writer_factory.cpp
    cout_writer_memento.cpp
    demangle.cpp
    event.cpp
    exception.cpp
    file_exception.cpp
    file_roller.cpp
    file_writer.cpp
    file_writer_factory.cpp
    file_writer_memento.cpp
    level.cpp
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
    clock_util.cpp
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
    include/chucho/exception.hpp
    include/chucho/file.hpp
    include/chucho/file_exception.hpp
    include/chucho/level_threshold_filter_factory.hpp
    include/chucho/level_threshold_filter_memento.hpp
    include/chucho/line_ending.hpp
    include/chucho/logger_factory.hpp
    include/chucho/logger_memento.hpp
    include/chucho/numbered_file_roller_factory.hpp
    include/chucho/numbered_file_roller_memento.hpp
    include/chucho/pattern_formatter_factory.hpp
    include/chucho/pattern_formatter_memento.hpp
    include/chucho/rolling_file_writer_factory.hpp
    include/chucho/rolling_file_writer_memento.hpp
    include/chucho/size_file_roll_trigger_factory.hpp
    include/chucho/size_file_roll_trigger_memento.hpp
    include/chucho/status.hpp
    include/chucho/status_manager.hpp
    include/chucho/status_observer.hpp
    include/chucho/status_reporter.hpp
    include/chucho/time_file_roller_factory.hpp
    include/chucho/time_file_roller_memento.hpp
    include/chucho/utf8_validator.hpp
    include/chucho/yaml_configurator.hpp
    ${CHUCHO_PLATFORM_SOURCES}
    ${CHUCHO_PUBLIC_HEADERS})

ADD_CUSTOM_TARGET(chucho)

SET(CHUCHO_EXTERNAL_DEPS yaml-cpp-external utf8-cpp-external)

IF(ENABLE_SHARED)
    ADD_LIBRARY(chucho-shared SHARED ${CHUCHO_SOURCES})
    TARGET_LINK_LIBRARIES(chucho-shared yaml-cpp)
    ADD_DEPENDENCIES(chucho-shared ${CHUCHO_EXTERNAL_DEPS})
    SET_TARGET_PROPERTIES(chucho-shared PROPERTIES
                          OUTPUT_NAME chucho
                          COMPILE_FLAGS "${CHUCHO_CXX_SO_FLAGS}"
                          FRAMEWORK TRUE
                          VERSION ${CHUCHO_VERSION}
                          SOVERSION ${CHUCHO_SOVERSION}
                          PUBLIC_HEADER ${CHUCHO_PUBLIC_HEADERS})
    ADD_DEPENDENCIES(chucho chucho-shared)
    INSTALL(TARGETS chucho-shared
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION lib
            PUBLIC_HEADER DESTINATION include/chucho)
ENDIF()
IF(ENABLE_STATIC)
    ADD_LIBRARY(chucho-static STATIC ${CHUCHO_SOURCES})
    ADD_DEPENDENCIES(chucho-static ${CHUCHO_EXTERNAL_DEPS})
    SET_TARGET_PROPERTIES(chucho-static PROPERTIES
                          OUTPUT_NAME chucho)
    ADD_DEFINITIONS(-Dchucho_shared_EXPORTS)
    ADD_DEPENDENCIES(chucho chucho-static)
    INSTALL(TARGETS chucho-static
            ARCHIVE DESTINATION lib
            PUBLIC_HEADER DESTINATION include/chucho)
ENDIF()

ADD_SUBDIRECTORY(test)
