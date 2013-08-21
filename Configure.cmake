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
INCLUDE(CheckIncludeFileCXX)
INCLUDE(ExternalProject)

# static and shared
OPTION(ENABLE_SHARED "Whether to build a shared object" FALSE)

# framework or not
OPTION(ENABLE_FRAMEWORK "Whether to build as a framework on Macintosh" TRUE)
SET(CHUCHO_NEEDS_TO_USE_THE_FRAMEWORK_VARIABLE_OR_CMAKE_COMPLAINS ${ENABLE_FRAMEWORK})

# whether to install chucod as a service or not on platforms that have
# services.
OPTION(INSTALL_SERVICE "Whether to install chuchod as a system service" TRUE)

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
ENDIF()
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
ELSEIF(MSVC)
    IF(MSVC_VERSION LESS 1700)
        MESSAGE(FATAL_ERROR "Microsoft compiler version 17 or later is required (the compiler that ships with Visual Studio 2012)")
    ENDIF()
ENDIF()

# We are building Chucho
ADD_DEFINITIONS(-DCHUCHO_BUILD)

# Configure our export definitions
IF(NOT ENABLE_SHARED)
    SET(CHUCHO_STATIC TRUE)
ENDIF()
MAKE_DIRECTORY("${CMAKE_BINARY_DIR}/chucho")
CONFIGURE_FILE(include/chucho/export.hpp.in "${CMAKE_BINARY_DIR}/chucho/export.hpp")

# rpath
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_RPATH FALSE)

# stdlib.h is required on all platforms
CHECK_INCLUDE_FILE_CXX(stdlib.h CHUCHO_HAVE_STDLIB_H)
IF(NOT CHUCHO_HAVE_STDLIB_H)
    MESSAGE(FATAL_ERROR "The header stdlib.h is required")
ENDIF()

IF(CHUCHO_POSIX)
    # headers
    FOREACH(HEAD arpa/inet.h fcntl.h limits.h netdb.h poll.h pthread.h pwd.h signal.h
                 sys/socket.h sys/stat.h sys/utsname.h syslog.h time.h unistd.h)
        STRING(REPLACE . _ CHUCHO_HEAD_VAR_NAME CHUCHO_HAVE_${HEAD})
        STRING(REPLACE / _ CHUCHO_HEAD_VAR_NAME ${CHUCHO_HEAD_VAR_NAME})
        STRING(TOUPPER ${CHUCHO_HEAD_VAR_NAME} CHUCHO_HEAD_VAR_NAME)
        CHECK_INCLUDE_FILE_CXX(${HEAD} ${CHUCHO_HEAD_VAR_NAME})
        IF(NOT ${CHUCHO_HEAD_VAR_NAME})
            MESSAGE(FATAL_ERROR "The header ${HEAD} is required")
        ENDIF()
    ENDFOREACH()
    # We need to know this in the source
    ADD_DEFINITIONS(-DCHUCHO_HAVE_ARPA_INET_H)

    # host name functions
    CHECK_CXX_SYMBOL_EXISTS(uname sys/utsname.h CHUCHO_HAVE_UNAME)
    IF(NOT CHUCHO_HAVE_UNAME)
        MESSAGE(FATAL_ERROR "uname is required")
    ENDIF()

    # resource usage
    CHECK_CXX_SYMBOL_EXISTS(clock time.h CHUCHO_HAVE_CLOCK)
    IF(NOT CHUCHO_HAVE_CLOCK)
        MESSAGE(FATAL_ERROR "clock is required")
    ENDIF()

    # getpid/access/getuid/fork/close/setsid/dup2/chdir/_exit
    FOREACH(SYM getpid access getuid fork close setsid dup2 chdir _exit)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} unistd.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "${SYM} is required")
        ENDIF()
    ENDFOREACH()

    # stat/mkdir
    FOREACH(SYM stat mkdir)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} sys/stat.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "${SYM} is required")
        ENDIF()
    ENDFOREACH()

    # gmtime_r/localtime_r
    FOREACH(SYM gmtime_r localtime_r)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} time.h CHUCHO_HAVE_${SYM})
        IF(CHUCHO_HAVE_${SYM})
            STRING(TOUPPER ${SYM} CHUCHO_UPPER_${SYM})
            ADD_DEFINITIONS(-DCHUCHO_HAVE_${CHUCHO_UPPER_${SYM}})
        ENDIF()
    ENDFOREACH()

    # fts
    CHECK_INCLUDE_FILE_CXX(fts.h CHUCHO_HAVE_FTS_H)
    IF(CHUCHO_HAVE_FTS_H)
        FOREACH(SYM fts_open fts_read fts_close)
            CHECK_CXX_SYMBOL_EXISTS(${SYM} fts.h CHUCHO_HAVE_${SYM})
            IF(NOT CHUCHO_HAVE_${SYM})
                SET(CHUCHO_NO_FTS TRUE)
                ADD_DEFINITIONS(-DCHUCHO_NO_FTS)
                BREAK()
            ENDIF()
        ENDFOREACH()
    ELSE()
        SET(CHUCHO_NO_FTS TRUE)
        ADD_DEFINITIONS(-DCHUCHO_NO_FTS)
    ENDIF()

    IF(CHUCHO_NO_FTS)
        CHECK_INCLUDE_FILE_CXX(dirent.h CHUCHO_HAVE_DIRENT_H)
        IF(NOT CHUCHO_HAVE_DIRENT_H)
            MESSAGE(FATAL_ERROR "Either fts.h or dirent.h is required")
        ENDIF()
        # opendir/readdir_r/closedir
        FOREACH(SYM opendir readdir_r closedir)
            CHECK_CXX_SYMBOL_EXISTS(${SYM} dirent.h CHUCHO_HAVE_${SYM})
            IF(NOT CHUCHO_HAVE_${SYM})
                MESSAGE(FATAL_ERROR "${SYM} is required")
            ENDIF()
        ENDFOREACH()
        CHECK_CXX_SYMBOL_EXISTS(pathconf unistd.h CHUCHO_HAVE_PATHCONF)
        IF(NOT CHUCHO_HAVE_PATHCONF)
            MESSAGE(FATAL_ERROR "pathconf is required")
        ENDIF()
        # The variable setting is flipped here. So, 1 is success, meaing the
        # program returned 0 for the exit code.
        CHECK_CXX_SOURCE_RUNS("#include <dirent.h>\n#include <unistd.h>\nint main() { return sizeof(struct dirent) >= pathconf(\"/\", _PC_NAME_MAX); }"
                              CHUCHO_DIRENT_NEEDS_NAME)
        IF(CHUCHO_DIRENT_NEEDS_NAME)
            ADD_DEFINITIONS(-DCHUCHO_DIRENT_NEEDS_NAME)
        ENDIF()
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
            MESSAGE(STATUS "linking with libpthread is required")
        ELSE()
            MESSAGE(FATAL_ERROR "can't build a program with threads at all")
        ENDIF()
        SET(CHUCHO_THREAD_LIB pthread CACHE STRING "The threading library")
    ENDIF()

    # getaddrinfo/freeaddrinfo/gai_strerror/getnameinfo
    IF(CHUCHO_SOLARIS)
        SET(CMAKE_REQUIRED_LIBRARIES socket nsl)
    ENDIF()
    FOREACH(SYM getaddrinfo freeaddrinfo gai_strerror getnameinfo)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} netdb.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "${SYM} is required")
        ENDIF()
    ENDFOREACH()
    IF(CHUCHO_SOLARIS)
        UNSET(CMAKE_REQUIRED_LIBRARIES)
    ENDIF()

    # syslog
    CHECK_CXX_SYMBOL_EXISTS(syslog syslog.h CHUCHO_HAVE_SYSLOG)
    IF(NOT CHUCHO_HAVE_SYSLOG)
        MESSSAGE(FATAL_ERROR "syslog is required")
    ENDIF()

    # socket/sendto/connect/shutdown/send/recv/bind/listen
    IF(CHUCHO_SOLARIS)
        SET(CMAKE_REQUIRED_LIBRARIES socket)
    ENDIF()
    FOREACH(SYM socket sendto connect shutdown send recv bind listen accept)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} sys/socket.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "${SYM} is required")
        ENDIF()
    ENDFOREACH()
    IF(CHUCHO_SOLARIS)
        UNSET(CMAKE_REQUIRED_LIBRARIES)
    ENDIF()

    # poll
    CHECK_CXX_SYMBOL_EXISTS(poll poll.h CHUCHO_HAVE_POLL)
    IF(NOT CHUCHO_HAVE_POLL)
        MESSAGE(FATAL_ERROR "poll is required")
    ENDIF()

    # getpwuid
    CHECK_CXX_SYMBOL_EXISTS(getpwuid pwd.h CHUCHO_HAVE_GETPWUID)
    IF(NOT CHUCHO_HAVE_GETPWUID)
        MESSAGE(FATAL_ERROR "getpwuid is required")
    ENDIF()

    # signal stuff
    FOREACH(SYM raise sigemptyset sigaddset sigwait sigaction kill sigpending sigismember pthread_sigmask)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} signal.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "${SYM} is required")
        ENDIF()
    ENDFOREACH()

    # open/fcntl
    FOREACH(SYM open fcntl)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} fcntl.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "${SYM} is required")
        ENDIF()
    ENDFOREACH()
ELSEIF(CHUCHO_WINDOWS)
    FOREACH(HEAD windows.h winsock2.h io.h process.h ws2tcpip.h time.h)
        STRING(REPLACE . _ CHUCHO_HEAD_VAR_NAME CHUCHO_HAVE_${HEAD})
        STRING(TOUPPER ${CHUCHO_HEAD_VAR_NAME} CHUCHO_HEAD_VAR_NAME)
        CHECK_INCLUDE_FILE_CXX(${HEAD} ${CHUCHO_HEAD_VAR_NAME})
        IF(NOT ${CHUCHO_HEAD_VAR_NAME})
            MESSAGE(FATAL_ERROR "The header ${HEAD} is required")
        ENDIF()
    ENDFOREACH()
    ADD_DEFINITIONS(-DCHUCHO_HAVE_WINSOCK2_H)
    # Shellapi.h has to have windows.h included first
    CHECK_CXX_SOURCE_COMPILES("#include <windows.h>\n#include <shellapi.h>\nint main() { return 0; }" CHUCHO_HAVE_SHELLAPI_H)
    IF(NOT CHUCHO_HAVE_SHELLAPI_H)
        MESSAGE(FATAL_ERROR "shellapi.h is required")
    ENDIF()

    # sc
    IF(INSTALL_SERVICE)
        MESSAGE(STATUS "Looking for sc")
        FIND_PROGRAM(CHUCHO_SC sc)
        IF(NOT CHUCHO_SC)
            MESSAGE(FATAL_ERROR "sc is required in order to install the Chucho service")
        ENDIF()
        MESSAGE(STATUS "Looking for sc - ${CHUCHO_SC}")
    ENDIF()
ENDIF()

CHECK_CXX_SOURCE_COMPILES("#include <exception>\nint main() { std::exception e; std::throw_with_nested(e); std::rethrow_if_nested(e); return 0; }"
                          CHUCHO_HAVE_NESTED_EXCEPTIONS)
IF(CHUCHO_HAVE_NESTED_EXCEPTIONS)
    ADD_DEFINITIONS(-DCHUCHO_HAVE_NESTED_EXCEPTIONS)
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

# cppcheck
FIND_PROGRAM(CHUCHO_CPPCHECK cppcheck)

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
ADD_LIBRARY(gtest STATIC IMPORTED)
IF(CHUCHO_WINDOWS)
    ExternalProject_Add_Step(gtest-external
                             install-libs
                             COMMAND "${CMAKE_COMMAND}" -E make_directory <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest.lib <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest.pdb <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest_main.lib <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/gtest_main.pdb <INSTALL_DIR>/lib
                             DEPENDEES install-headers)
    SET_TARGET_PROPERTIES(gtest PROPERTIES
                          IMPORTED_LOCATION "${CHUCHO_EXTERNAL_PREFIX}/lib/gtest.lib")
ELSE()
    ExternalProject_Add_Step(gtest-external
                             install-libs
                             COMMAND "${CMAKE_COMMAND}" -E make_directory <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/libgtest.a <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/libgtest_main.a <INSTALL_DIR>/lib
                             DEPENDEES install-headers)
    SET_TARGET_PROPERTIES(gtest PROPERTIES
                          IMPORTED_LOCATION "${CHUCHO_EXTERNAL_PREFIX}/lib/libgtest.a")
ENDIF()
ADD_DEPENDENCIES(gtest gtest-external)
SET_TARGET_PROPERTIES(gtest-external PROPERTIES
                      EXCLUDE_FROM_ALL TRUE)
ADD_DEPENDENCIES(external gtest-external)
