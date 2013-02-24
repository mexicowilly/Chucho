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

INCLUDE(CheckCXXCompilerFlag)
INCLUDE(CheckCXXSymbolExists)
INCLUDE(CheckCXXSourceRuns)
INCLUDE(ExternalProject)

# static and shared
OPTION(ENABLE_SHARED "Whether to build a shared object" FALSE)

# framework or not
OPTION(ENABLE_FRAMEWORK "Whether to build as a framework on Macintosh" TRUE)
IF(ENABLE_FRAMEWORK)
    SET(CHUCHO_NEED_TO_USE_THE_FRAMEWORK_VARIABLE_OR_CMAKE_COMPLAINS ${ENABLE_FRAMEWORK})
ENDIF()

# Set consistent platform names
IF(CMAKE_SYSTEM_NAME STREQUAL Windows)
    SET(CHUCHO_WINDOWS TRUE)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL Linux)
    SET(CHUCHO_LINUX TRUE)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL SunOS)
    SET(CHUCHO_SOLARIS TRUE)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
    SET(CHUCHO_FREEBSD TRUE)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL NetBSD)
    SET(CHUCHO_NETBSD TRUE)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL OpenBSD)
    SET(CHUCHO_OPENBSD TRUE)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    SET(CHUCHO_MACINTOSH TRUE)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL AIX)
    SET(CHUCHO_AIX TRUE)
ENDIF()
IF(NOT CHUCHO_WINDOWS)
    SET(CHUCHO_POSIX TRUE)
ENDIF()

# Set default build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Build type, one of: Release, Debug, RelWithDebInfo, or MinSizeRel" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
MESSAGE(STATUS "Build type -- ${CMAKE_BUILD_TYPE}")

# Compiler flags
IF(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    CHECK_CXX_COMPILER_FLAG(-stdlib=libc++ CHUCHO_LIBCXX_FLAG)
    IF(NOT CHUCHO_LIBCXX_FLAG)
        MESSAGE(FATAL_ERROR "libc++ is required")
    ENDIF()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    CHECK_CXX_COMPILER_FLAG(-std=c++11 CHUCHO_CXX11_FLAG)
    IF(CHUCHO_CXX11_FLAG)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ELSEIF(CHUCHO_MACINTOSH)
        MESSAGE(FATAL_ERROR "C++11 compatibility is required")
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden CHUCHO_VIS_FLAG)
    IF(CHUCHO_VIS_FLAG)
        SET(CHUCHO_CXX_SO_FLAGS -fvisibility=hidden)
    ENDIF()
	IF(CMAKE_GENERATOR STREQUAL Xcode)
		SET(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -stdlib=libc++")
	ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
        MESSAGE(FATAL_ERROR "g++ version 4.7 or later is required")
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-std=c++11 CHUCHO_HAVE_STD)
    IF(CHUCHO_HAVE_STD)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ELSE()
        MESSAGE(FATAL_ERROR "-std=c++11 is required")
    ENDIF()
ENDIF()

# rpath
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_RPATH FALSE)

IF(CHUCHO_POSIX)
    # host name functions
    CHECK_CXX_SYMBOL_EXISTS(uname sys/utsname.h CHUCHO_HAVE_UNAME)
    IF(NOT CHUCHO_HAVE_UNAME)
        MESSAGE(FATAL_ERROR "uname is required")
    ENDIF()

    # resource usage
    CHECK_CXX_SYMBOL_EXISTS(getrusage sys/resource.h CHUCHO_HAVE_GETRUSAGE)
    IF(NOT CHUCHO_HAVE_GETRUSAGE)
        MESSAGE(FATAL_ERROR "getrusage is required")
    ENDIF()

    # getpid
    CHECK_CXX_SYMBOL_EXISTS(getpid unistd.h CHUCHO_HAVE_GETPID)
    IF(NOT CHUCHO_HAVE_GETPID)
        MESSAGE(FATAL_ERROR "getpid is required")
    ENDIF()

    # stat
    CHECK_CXX_SYMBOL_EXISTS(stat sys/stat.h CHUCHO_HAVE_STAT)
    IF(NOT CHUCHO_HAVE_STAT)
        MESSAGE(FATAL_ERROR "stat is required")
    ENDIF()

    # gmtime/localtime
    FOREACH(SYM gmtime localtime)
        CHECK_CXX_SYMBOL_EXISTS(${SYM}_r time.h CHUCHO_HAVE_${SYM}_R)
        IF(CHUCHO_HAVE_${SYM}_R)
            STRING(TOUPPER ${SYM} CHUCHO_UPPER_SYM)
            ADD_DEFINITIONS(-DCHUCHO_HAVE_${CHUCHO_UPPER_SYM}_R)
        ENDIF()
    ENDFOREACH()

    # fts
    FOREACH(SYM fts_open fts_read fts_close)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} fts.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "FTS functions are required")
        ENDIF()
    ENDFOREACH()

    # remove
    CHECK_CXX_SYMBOL_EXISTS(remove stdio.h CHUCHO_HAVE_REMOVE)
    IF(NOT CHUCHO_HAVE_REMOVE)
        MESSAGE(FATAL_ERROR "remove is required")
    ENDIF()

    # realpath
    CHECK_CXX_SYMBOL_EXISTS(realpath stdlib.h CHUCHO_HAVE_REALPATH)
    IF(NOT CHUCHO_HAVE_REALPATH)
        MESSAGE(FATAL_ERROR "realpath is required")
    ENDIF()

    # whether linking to libpthread is required
    SET(CHUCHO_PTHREAD_SOURCE "#include <pthread.h>\npthread_key_t k; void d(void*) { }; int main() { pthread_key_create(&k, d); return 0; }")
    CHECK_CXX_SOURCE_RUNS("${CHUCHO_PTHREAD_SOURCE}" CHUCHO_PTHREAD_LINK)
    IF(NOT CHUCHO_PTHREAD_LINK)
        UNSET(CHUCHO_PTHREAD_LINK)
        UNSET(CHUCHO_PTHREAD_LINK CACHE)
        SET(CMAKE_REQUIRED_LIBRARIES pthread)
        CHECK_CXX_SOURCE_RUNS("${CHUCHO_PTHREAD_SOURCE}" CHUCHO_PTHREAD_LINK)
        IF(CHUCHO_PTHREAD_LINK)
            MESSAGE(STATUS "Linking with libpthread is required")
        ELSE()
            MESSAGE(FATAL_ERROR "can't build a program with threads at all")
        ENDIF()
        SET(CHUCHO_THREAD_LIB pthread)
    ENDIF()
ENDIF()

#
# CHECK_CXX_SYMBOL_EXISTS does not work for the following
#

# std::put_time
CHECK_CXX_SOURCE_COMPILES("#include <iomanip>\nint main() { std::tm t; std::put_time(&t, \\\"%Y\\\"); return 0; }"
                          CHUCHO_HAVE_PUT_TIME)
IF(CHUCHO_HAVE_PUT_TIME)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_PUT_TIME)
ENDIF()

# doxygen
FIND_PACKAGE(Doxygen)

#
# External projects
#
ADD_CUSTOM_TARGET(external)
SET(CHUCHO_EXTERNAL_PREFIX "${CMAKE_BINARY_DIR}")
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX "${CHUCHO_EXTERNAL_PREFIX}")

# Gtest
IF(CHUCHO_WINDOWS)
    SET(CHUCHO_GTEST_GENERATOR "NMake Makefiles")
ELSE()
    SET(CHUCHO_GTEST_GENERATOR "Unix Makefiles")
ENDIF()

ExternalProject_Add(gtest-external
                    URL http://googletest.googlecode.com/files/gtest-1.6.0.zip
                    URL_MD5 4577b49f2973c90bf9ba69aa8166b786
                    CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=Debug -Dgtest_force_shared_crt:BOOL=ON "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DGTEST_HAS_TR1_TUPLE=0"
                    CMAKE_GENERATOR "${CHUCHO_GTEST_GENERATOR}"
                    INSTALL_COMMAND "")
ExternalProject_Add_Step(gtest-external
                         install-headers
                         COMMAND "${CMAKE_COMMAND}" -E make_directory <INSTALL_DIR>/include
                         COMMAND "${CMAKE_COMMAND}" -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
                         DEPENDEES build)
IF(CHUCHO_WINDOWS)
    ExternalProject_Add_Step(gtest-external
                             install-libs
                             COMMAND "${CMAKE_COMMAND}" -E make_directory <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest.lib <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest.pdb <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest_main.lib <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest_main.pdb <INSTALL_DIR>/lib
                             DEPENDEES install-headers)
ELSE()
    ExternalProject_Add_Step(gtest-external
                             install-libs
                             COMMAND "${CMAKE_COMMAND}" -E make_directory <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/libgtest.a <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/libgtest_main.a <INSTALL_DIR>/lib
                             DEPENDEES install-headers)
    ADD_LIBRARY(gtest STATIC IMPORTED)
    SET_TARGET_PROPERTIES(gtest PROPERTIES
                          IMPORTED_LOCATION "${CHUCHO_EXTERNAL_PREFIX}/lib/libgtest.a")
ENDIF()
ADD_DEPENDENCIES(external gtest-external)
