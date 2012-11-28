CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(chuhco CXX)

INCLUDE(Configure.cmake)

INCLUDE_DIRECTORIES(include)

SET(CHUCHO_VERSION 1.1)
SET(CHUCHO_SOVERSION 1)

IF(CHUCHO_POSIX)
    SET(CHUCHO_PLATFORM_SOURCES
        platform/posix/pattern_formatter_posix.cpp)
ENDIF()

ADD_LIBRARY(chucho SHARED
            event.cpp
            exception.cpp
            filter.cpp
            formatter.cpp
            level.cpp
            logger.cpp
            writer.cpp
            level_threshold_filter.cpp
            pattern_formatter.cpp
            include/chucho/exception.hpp
            include/chucho/event.hpp
            include/chucho/export.hpp
            include/chucho/filter.hpp
            include/chucho/formatter.hpp
            include/chucho/level.hpp
            include/chucho/level_threshold_filter.hpp
            include/chucho/logger.hpp
            include/chucho/pattern_formatter.hpp
            include/chucho/writer.hpp
            ${CHUCHO_PLATFORM_SOURCES})

SET_TARGET_PROPERTIES(chucho PROPERTIES
                      COMPILE_FLAGS "${CHUCHO_CXX_SO_FLAGS}"
                      FRAMEWORK TRUE
                      VERSION ${CHUCHO_VERSION}
                      SOVERSION ${CHUCHO_SOVERSION})

ADD_SUBDIRECTORY(test)
