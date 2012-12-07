CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(chuhco CXX)

INCLUDE(Configure.cmake)

INCLUDE_DIRECTORIES(include)

SET(CHUCHO_VERSION 1.1)
SET(CHUCHO_SOVERSION 1)

IF(CHUCHO_POSIX)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/posix/calendar_posix.cpp
        platform/posix/file_posix.cpp
        platform/posix/pattern_formatter_posix.cpp)
ENDIF()

ADD_LIBRARY(chucho SHARED
            console_writer.cpp
            event.cpp
            exception.cpp
            file_roller.cpp
            file_writer.cpp
            level.cpp
            level_threshold_filter.cpp
            logger.cpp
            numbered_file_roller.cpp
            pattern_formatter.cpp
            rolling_file_writer.cpp
            size_file_roll_trigger.cpp
            status.cpp
            status_manager.cpp
            status_observer.cpp
            status_reporter.cpp
            time_file_roller.cpp
            writer.cpp
            include/chucho/calendar.hpp
            include/chucho/console_writer.hpp
            include/chucho/exception.hpp
            include/chucho/event.hpp
            include/chucho/export.hpp
            include/chucho/file.hpp
            include/chucho/file_roller.hpp
            include/chucho/file_roll_trigger.hpp
            include/chucho/file_writer.hpp
            include/chucho/filter.hpp
            include/chucho/formatter.hpp
            include/chucho/level.hpp
            include/chucho/level_threshold_filter.hpp
            include/chucho/logger.hpp
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
            ${CHUCHO_PLATFORM_SOURCES})

SET_TARGET_PROPERTIES(chucho PROPERTIES
                      COMPILE_FLAGS "${CHUCHO_CXX_SO_FLAGS}"
                      FRAMEWORK TRUE
                      VERSION ${CHUCHO_VERSION}
                      SOVERSION ${CHUCHO_SOVERSION})

ADD_SUBDIRECTORY(test)
