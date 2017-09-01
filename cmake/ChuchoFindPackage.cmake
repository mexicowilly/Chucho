#
# Copyright 2013-2017 Will Mason
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

INCLUDE(ChuchoRequireSymbols)

FIND_PACKAGE(PkgConfig QUIET)

LIST(APPEND WITH_LIBS $ENV{CHUCHO_WITH_LIBS})
STRING(TOLOWER "${WITH_LIBS}" LOWER_WITH_LIBS)
SET(CHUCHO_BUILT_INS curl protobuf ruby zlib bzip2)

# This cannot be a function, because then FIND_PACKAGE will fail
# to put its variables in the right scope
MACRO(CHUCHO_FIND_PACKAGE CHUCHO_PKG_NAME)
    STRING(TOLOWER ${CHUCHO_PKG_NAME} LOWER_PKG_NAME)
    LIST(FIND LOWER_WITH_LIBS ${LOWER_PKG_NAME} SHOULD_FIND_${CHUCHO_PKG_NAME})
    IF(${SHOULD_FIND_${CHUCHO_PKG_NAME}} EQUAL -1)
        RETURN()
    ENDIF()
    LIST(FIND CHUCHO_BUILT_INS ${LOWER_PKG_NAME} BUILT_IN_PKG)
    IF (${BUILT_IN_PKG} EQUAL -1)
        UNSET(INCLUDE_ARGS)
        UNSET(LIB_ARGS)
        UNSET(SYMBOL_ARGS)
        UNSET(PKG_CONFIG_ARGS)
        FOREACH(ARG ${ARGN})
            IF(${ARG} STREQUAL INCLUDE)
                SET(CURSET INCLUDE_ARGS)
            ELSEIF(${ARG} STREQUAL LIBS)
                SET(CURSET LIB_ARGS)
            ELSEIF(${ARG} STREQUAL SYMBOLS)
                SET(CURSET SYMBOL_ARGS)
            ELSEIF(${ARG} STREQUAL PKG_CONFIG_NAME)
                SET(CURSET PKG_CONFIG_ARGS)
            ELSE()
                LIST(APPEND ${CURSET} ${ARG})
            ENDIF()
        ENDFOREACH()
        IF (PKG_CONFIG_FOUND)
            IF(PKG_CONFIG_ARGS)
                SET(PKG_CONFIG_NAME ${PKG_CONFIG_ARGS})
            ELSE()
                STRING(TOLOWER ${CHUCHO_PKG_NAME} PKG_CONFIG_NAME)
            ENDIF()
            PKG_CHECK_MODULES(${CHUCHO_PKG_NAME}_PKGCNF QUIET ${PKG_CONFIG_NAME})
        ENDIF()
        IF(INCLUDE_ARGS)
            MESSAGE(STATUS "Looking for ${INCLUDE_ARGS}")
            FIND_PATH(${CHUCHO_PKG_NAME}_INCLUDE_DIR NAMES ${INCLUDE_ARGS} HINTS ${${CHUCHO_PKG_NAME}_PKGCNF_INCLUDE_DIRS} ${${CHUCHO_PKG_NAME}_INCLUDE_DIR})
            IF(${CHUCHO_PKG_NAME}_INCLUDE_DIR)
                MESSAGE(STATUS "Looking for ${INCLUDE_ARGS} - ${${CHUCHO_PKG_NAME}_INCLUDE_DIR}")
            ELSE()
                MESSAGE(STATUS "Looking for ${INCLUDE_ARGS} - not found")
                MESSAGE(FATAL_ERROR "${CHUCHO_PKG_NAME} is required")
            ENDIF()
        ENDIF()
        IF(LIB_ARGS)
            FOREACH(LIB ${LIB_ARGS})
                MESSAGE(STATUS "Looking for ${LIB}")
                FIND_LIBRARY(${CHUCHO_PKG_NAME}_LIB_${LIB} NAMES ${LIB} HINTS ${${CHUCHO_PKG_NAME}_PKGCNF_LIBRARY_DIRS} ${${CHUCHO_PKG_NAME}_LIB})
                IF(${CHUCHO_PKG_NAME}_LIB_${LIB})
                    MESSAGE(STATUS "Looking for ${LIB} - ${${CHUCHO_PKG_NAME}_LIB_${LIB}}")
                ELSE()
                    MESSAGE(STATUS "Looking for ${LIB_ARGS} - not found")
                    MESSAGE(FATAL_ERROR "${CHUCHO_PKG_NAME} is required")
                ENDIF()
                LIST(APPEND ${CHUCHO_PKG_NAME}_LIBS ${${CHUCHO_PKG_NAME}_LIB_${LIB}})
            ENDFOREACH()
        ENDIF()
        SET(CMAKE_REQUIRED_INCLUDES ${${CHUCHO_PKG_NAME}_INCLUDE_DIR})
        SET(CMAKE_REQUIRED_LIBRARIES ${${CHUCHO_PKG_NAME}_LIBS})
        CHUCHO_REQUIRE_SYMBOLS(${INCLUDE_ARGS} ${SYMBOL_ARGS})
        UNSET(CMAKE_REQUIRED_INCLUDES)
        UNSET(CMAKE_REQUIRED_LIBRARIES)
        SET(${CHUCHO_PKG_NAME}_INCLUDE_DIR ${${CHUCHO_PKG_NAME}_INCLUDE_DIR})
        SET(${CHUCHO_PKG_NAME}_LIBS ${${CHUCHO_PKG_NAME}_LIBS})
        SET(CHUCHO_HAVE_${CHUCHO_PKG_NAME} TRUE)
        MARK_AS_ADVANCED(${CHUCHO_PKG_NAME}_INCLUDE_DIR ${CHUCHO_PKG_NAME}_LIBS CHUCHO_HAVE_${CHUCHO_PKG_NAME})
    ELSE()
        FIND_PACKAGE(${CHUCHO_PKG_NAME} REQUIRED)
    ENDIF()
ENDMACRO()
