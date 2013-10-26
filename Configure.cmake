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

# We'll want this later
MACRO(CHUCHO_FIND_PROGRAM CHUCHO_FIND_VAR CHUCHO_PROGRAM)
    MESSAGE(STATUS "Looking for ${CHUCHO_PROGRAM}")
    FIND_PROGRAM(${CHUCHO_FIND_VAR} "${CHUCHO_PROGRAM}")
    MESSAGE(STATUS "Looking for ${CHUCHO_PROGRAM} - ${${CHUCHO_FIND_VAR}}")
ENDMACRO()

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
    CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden CHUCHO_VIS_FLAG)
    IF(CHUCHO_VIS_FLAG)
        SET(CHUCHO_CXX_SO_FLAGS -fvisibility=hidden)
    ENDIF()
ELSEIF(MSVC)
    IF(MSVC_VERSION LESS 1700)
        MESSAGE(FATAL_ERROR "Microsoft compiler version 17 or later is required (the compiler that ships with Visual Studio 2012)")
    ENDIF()
    IF(ENABLE_SHARED)
        SET(CMAKE_CXX_FLAGS "/wd4275 /EHsc")
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

# Configure our version header
STRING(REGEX REPLACE "^([0-9]+)\\..+$" "\\1" CHUCHO_VERSION_MAJOR ${CHUCHO_VERSION})
STRING(REGEX REPLACE "^.+\\.([0-9]+)$" "\\1" CHUCHO_VERSION_MINOR ${CHUCHO_VERSION})
CONFIGURE_FILE(include/chucho/version.hpp.in "${CMAKE_BINARY_DIR}/chucho/version.hpp")

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
    FOREACH(HEAD arpa/inet.h assert.h fcntl.h limits.h netdb.h poll.h pthread.h pwd.h signal.h
                 sys/socket.h sys/stat.h sys/utsname.h syslog.h time.h unistd.h)
        STRING(REPLACE . _ CHUCHO_HEAD_VAR_NAME CHUCHO_HAVE_${HEAD})
        STRING(REPLACE / _ CHUCHO_HEAD_VAR_NAME ${CHUCHO_HEAD_VAR_NAME})
        STRING(TOUPPER ${CHUCHO_HEAD_VAR_NAME} CHUCHO_HEAD_VAR_NAME)
        CHECK_INCLUDE_FILE_CXX(${HEAD} ${CHUCHO_HEAD_VAR_NAME})
        IF(NOT ${CHUCHO_HEAD_VAR_NAME})
            MESSAGE(FATAL_ERROR "The header ${HEAD} is required")
        ENDIF()
    ENDFOREACH()

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
        STRING(TOUPPER ${SYM} CHUCHO_UPPER_${SYM})
        CHECK_CXX_SYMBOL_EXISTS(${SYM} time.h CHUCHO_HAVE_${CHUCHO_UPPER_${SYM}})
    ENDFOREACH()

    # fts
    CHECK_INCLUDE_FILE_CXX(fts.h CHUCHO_HAVE_FTS_H)
    IF(CHUCHO_HAVE_FTS_H)
        FOREACH(SYM fts_open fts_read fts_close)
            CHECK_CXX_SYMBOL_EXISTS(${SYM} fts.h CHUCHO_HAVE_${SYM})
            IF(NOT CHUCHO_HAVE_${SYM})
                SET(CHUCHO_NO_FTS TRUE)
                BREAK()
            ENDIF()
        ENDFOREACH()
    ELSE()
        SET(CHUCHO_NO_FTS TRUE)
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

    # htonl
    CHECK_CXX_SYMBOL_EXISTS(htonl arpa/inet.h CHUCHO_HAVE_HTONL)
    IF(NOT CHUCHO_HAVE_HTONL)
        MESSAGE(FATAL_ERROR "htonl is required")
    ENDIF()
ELSEIF(CHUCHO_WINDOWS)
    FOREACH(HEAD windows.h winsock2.h io.h process.h ws2tcpip.h time.h assert.h)
        STRING(REPLACE . _ CHUCHO_HEAD_VAR_NAME CHUCHO_HAVE_${HEAD})
        STRING(TOUPPER ${CHUCHO_HEAD_VAR_NAME} CHUCHO_HEAD_VAR_NAME)
        CHECK_INCLUDE_FILE_CXX(${HEAD} ${CHUCHO_HEAD_VAR_NAME})
        IF(NOT ${CHUCHO_HEAD_VAR_NAME})
            MESSAGE(FATAL_ERROR "The header ${HEAD} is required")
        ENDIF()
    ENDFOREACH()
    # Shellapi.h has to have windows.h included first
    CHECK_CXX_SOURCE_COMPILES("#include <windows.h>\n#include <shellapi.h>\nint main() { return 0; }" CHUCHO_HAVE_SHELLAPI_H)
    IF(NOT CHUCHO_HAVE_SHELLAPI_H)
        MESSAGE(FATAL_ERROR "shellapi.h is required")
    ENDIF()

    # sc
    CHUCHO_FIND_PROGRAM(CHUCHO_SC sc)
    IF(NOT CHUCHO_SC)
        MESSAGE(FATAL_ERROR "sc is required")
    ENDIF()
ENDIF()

# Nested exceptions
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

# Regular expressions
CHECK_INCLUDE_FILE_CXX(regex CHUCHO_HAVE_STD_REGEX_HEADER)
IF(CHUCHO_HAVE_STD_REGEX_HEADER)
    # Clang's 4.2.1 std::regex library has a bug where it doesn't match properly
    # when you make a regex ungreedy by matching against everything but the
    # start of the regex.
    CHECK_CXX_SOURCE_RUNS("
#include <regex>
#include <fstream>
#include <iostream>
int main()
{
    std::ofstream out(\"std-regex-result\");
    std::regex re(\"\\\\\\\\$([Ee][Nn][Vv])?\\\\\\\\{([^{]+)\\\\\\\\}\", std::regex_constants::extended);
    std::smatch m;
    std::string s(\"I \\\${HAVE} one and \\\$ENV{it} looks good.\");
    if (std::regex_search(s, m, re) &&
        m.size() == 3 &&
        m.position(0) == 2 &&
        m.length(0) == 7 &&
        !m[1].matched &&
        m.position(2) == 4 &&
        m.length(2) == 4)
    {
        out << \"good\";
    }
    else
    {
        out << \"buggy\";
    }
    return EXIT_SUCCESS;
}" CHUCHO_HAVE_STD_REGEX_RUNS)
    IF(CHUCHO_HAVE_STD_REGEX_RUNS)
        FILE(READ "${CMAKE_BINARY_DIR}/std-regex-result" CHUCHO_STD_REGEX_RESULT)
        IF(CHUCHO_STD_REGEX_RESULT STREQUAL good)
            SET(CHUCHO_HAVE_STD_REGEX TRUE)
        ELSE()
            MESSAGE(STATUS "Although std::regex is available, it has bugs")
        ENDIF()
    ENDIF()
ENDIF()
# Do not else this
IF(CHUCHO_HAVE_STD_REGEX)
    MESSAGE(STATUS "Using regular expressions - std::regex")
ELSE()
    CHECK_INCLUDE_FILE_CXX(regex.h CHUCHO_HAVE_POSIX_REGEX)
    IF(NOT CHUCHO_HAVE_POSIX_REGEX)
        MESSAGE(FATAL_ERROR "You must have either std::regex or POSIX regex from the C library")
    ENDIF()
    FOREACH(SYM regcomp regerror regexec regfree REG_EXTENDED)
        CHECK_CXX_SYMBOL_EXISTS(${SYM} regex.h CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "${SYM} is required")
        ENDIF()
    ENDFOREACH()
    MESSAGE(STATUS "Using regular expressions - POSIX")
ENDIF()

# htonl
CHECK_CXX_SYMBOL_EXISTS(assert assert.h CHUCHO_HAVE_ASSERT)
IF(NOT CHUCHO_HAVE_ASSERT)
    MESSAGE(FATAL_ERROR "assert is required")
ENDIF()

# doxygen
FIND_PACKAGE(Doxygen)

# cppcheck
CHUCHO_FIND_PROGRAM(CHUCHO_CPPCHECK cppcheck)

# Solaris service stuff
IF(CHUCHO_SOLARIS)
    CHUCHO_FIND_PROGRAM(CHUCHO_SVCCFG svccfg)
    IF(NOT CHUCHO_SVCCFG)
        MESSAGE(FATAL_ERROR "svccfg is required")
    ENDIF()
    CHUCHO_FIND_PROGRAM(CHUCHO_SVCADM svcadm)
    IF(NOT CHUCHO_SVCADM)
        MESSAGE(FATAL_ERROR "svcadm is required")
    ENDIF()
ENDIF()

# zip
CHUCHO_FIND_PROGRAM(CHUCHO_ZIP zip)
IF(NOT CHUCHO_ZIP)
    MESSAGE(STATUS "No zdist target will be available, since you don't have zip")
ENDIF()

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

MACRO(CHUCHO_DOWNLOAD CHUCHO_DL_URL CHUCHO_DL_PKG_VAR)
    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")
    GET_FILENAME_COMPONENT(CHUCHO_DL_BASE_FILE "${CHUCHO_DL_URL}" NAME)
    SET(CHUCHO_DL_PKG_NAME "${CMAKE_BINARY_DIR}/packages/${CHUCHO_DL_BASE_FILE}")
    MESSAGE(STATUS "Attempting to download ${CHUCHO_DL_URL}")
    FILE(DOWNLOAD "${CHUCHO_DL_URL}" "${CHUCHO_DL_PKG_NAME}"
         INACTIVITY_TIMEOUT 60
         STATUS CHUCHO_DL_STATUS
         SHOW_PROGRESS)
    LIST(GET CHUCHO_DL_STATUS 0 CHUCHO_DL_STATUS_0)
    IF(CHUCHO_DL_STATUS_0 EQUAL 0)
        SET(${CHUCHO_DL_PKG_VAR} "${CHUCHO_DL_PKG_NAME}")
    ELSE()
        LIST(GET CHUCHO_DL_STATUS 1 CHUCHO_DL_STATUS_1)
        MESSAGE(WARNING "An error occurred downloading ${CHUCHO_DL_URL}: ${CHUCHO_DL_STATUS_1}")
    ENDIF()
ENDMACRO()

# zlib
#
# You may enable zlib support by doing one of several things:
#
# In order to have zlib embedded into Chucho and not require external linkage later,
# do one of these three things:
#
#   * Set ZLIB_URL to the URL from which to retrieve the zlib tarball
#   * Set ZLIB_PACKAGE to the name of the zlib tarball
#   * Set ZLIB_SOURCE to the name of the directory where an unpacked zlib tarball lives
#
# In order to use zlib as an external library that your executable must subsequently
# also be linked to, do one of the following two things:
#
#   * Set both ZLIB_INCLUDE_DIR and ZLIB_LIB_DIR
#   * Set nothing and let CMake try to find the library and headers
#
# To completely disable zlib support, do the following:
#
#   * Set DISABLE_ZLIB
#
IF(NOT CHUCHO_HAVE_ZLIB)
    IF(ZLIB_URL OR ZLIB_PACKAGE OR ZLIB_SOURCE)
        IF(ZLIB_URL)
            CHUCHO_DOWNLOAD("${ZLIB_URL}" ZLIB_PACKAGE)
        ENDIF()
        IF(ZLIB_PACKAGE)
            IF(EXISTS "${ZLIB_PACKAGE}")
                MESSAGE(STATUS "Unpacking the zlib package ${ZLIB_PACKAGE}")
                EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -E tar xf "${ZLIB_PACKAGE}"
                                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                                RESULT_VARIABLE CHUCHO_RESULT)
                IF(CHUCHO_RESULT EQUAL 0)
                    FILE(GLOB ZLIB_SOURCE "${CMAKE_BINARY_DIR}/zlib-*")
                    IF(NOT ZLIB_SOURCE)
                        MESSAGE(WARNING "Zlib compression is disabled because after unpacking ${ZLIB_PACKAGE} no directory named \"zlib-*\" could be found in ${CMAKE_BINARY_DIR}")
                    ENDIF()
                ELSE()
                    MESSAGE(WARNING "Zlib compression is disabled because ${ZLIB_PACKAGE} could not be unpacked")
                ENDIF()
            ELSE()
                MESSAGE(WARNING "Zlib compression is disabled because ZLIB_PACKAGE does not reference an existing file: ${ZLIB_PACKAGE}")
            ENDIF()
        ENDIF()
        IF(ZLIB_SOURCE)
            SET(ZLIB_SOURCE "${ZLIB_SOURCE}" CACHE INTERNAL "The location of zlib sources")
            MESSAGE(STATUS "Using zlib sources at ${ZLIB_SOURCE}")
            FOREACH(SRC adler32 compress crc32 deflate gzclose gzread gzlib gzwrite inffast inflate inftrees trees zutil)
                LIST(APPEND CHUCHO_ZLIB_SOURCES "${ZLIB_SOURCE}/${SRC}.c")
            ENDFOREACH()
            SET(CHUCHO_ZLIB_SOURCES ${CHUCHO_ZLIB_SOURCES} CACHE INTERNAL "The zlib sources to build")
        ENDIF()
    ELSEIF(ZLIB_INCLUDE_DIR OR ZLIB_LIB_DIR)
        IF(NOT ZLIB_INCLUDE_DIR)
            MESSAGE(WARNING "Zlib compression is disabled because If ZLIB_LIB_DIR is set, then ZLIB_INCLUDE_DIR must also be set")
            UNSET(ZLIB_LIB_DIR)
        ELSEIF(NOT ZLIB_LIB_DIR)
            MESSAGE(WARNING "Zlib compression is disabled because If ZLIB_INCLUDE_DIR is set, then ZLIB_LIB_DIR must also be set")
            UNSET(ZLIB_INCLUDE_DIR)
        ELSE()
            SET(ZLIB_INCLUDE_DIR "${ZLIB_INCLUDE_DIR}" CACHE INTERNAL "The location of zlib.h")
            SET(ZLIB_LIB_DIR "${ZLIB_LIB_DIR}" CACHE INTERNAL "The location of libz")
        ENDIF()
    ELSEIF(NOT DISABLE_ZLIB)
        FIND_PACKAGE(ZLIB)
        IF(ZLIB_FOUND)
            SET(ZLIB_INCLUDE_DIR "${ZLIB_INCLUDE_DIRS}" CACHE STRING "The location of zlib.h")
            LIST(GET ZLIB_LIBRARIES 0 ZLIB_LIB_DIR)
            GET_FILENAME_COMPONENT(ZLIB_LIB_DIR "${ZLIB_LIB_DIR}" PATH)
            SET(ZLIB_LIB_DIR "${ZLIB_LIB_DIR}" CACHE STRING "The location of libz")
        ELSE()
            MESSAGE(WARNING "Zlib compression is disabled because the library could not be found. Set DISABLE_ZLIB to silence this warning.")
        ENDIF()
    ENDIF()
    IF(ZLIB_SOURCE OR ZLIB_INCLUDE_DIR)
        SET(CHUCHO_HAVE_ZLIB TRUE CACHE INTERNAL "Whether we have found zlib or not")
    ENDIF()
ENDIF()

# bzip2
#
# You may enable bzip2 support by doing one of several things:
#
# In order to have bzip2 embedded into Chucho and not require external linkage later,
# do one of these two things:
#
#   * Set BZIP2_URL to the URL from which to retrieve the bzip2 tarball
#   * Set BZIP2_PACKAGE to the name of the bzip2 tarball
#   * Set BZIP2_SOURCE to the name of the directory where an unpacked bzip2 tarball lives
#
# In order to use bzip2 as an external library that your executable must subsequently
# also be linked to, do one of the following two things:
#
#   * Set both BZIP2_INCLUDE_DIR and BZIP2_LIB_DIR
#   * Set nothing and let CMake try to find the library and headers
#
# To completely disable zlib support, do the following:
#
#   * Set DISABLE_BZIP2
#
IF(NOT CHUCHO_HAVE_BZIP2)
    IF(BZIP2_URL OR BZIP2_PACKAGE OR BZIP2_SOURCE)
        IF(BZIP2_URL)
            CHUCHO_DOWNLOAD("${BZIP2_URL}" BZIP2_PACKAGE)
        ENDIF()
        IF(BZIP2_PACKAGE)
            IF(EXISTS "${BZIP2_PACKAGE}")
                MESSAGE(STATUS "Unpacking the bzip2 package ${BZIP2_PACKAGE}")
                EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -E tar xf "${BZIP2_PACKAGE}"
                                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                                RESULT_VARIABLE CHUCHO_RESULT)
                IF(CHUCHO_RESULT EQUAL 0)
                    FILE(GLOB BZIP2_SOURCE "${CMAKE_BINARY_DIR}/bzip2-*")
                    IF(NOT BZIP2_SOURCE)
                        MESSAGE(WARNING "Bzip2 compression is disabled because after unpacking ${BZIP2_PACKAGE} no directory named \"bzip2-*\" could be found in ${CMAKE_BINARY_DIR}")
                    ENDIF()
                ELSE()
                    MESSAGE(WARNING "Bzip2 compression is disabled because ${BZIP2_PACKAGE} could not be unpacked")
                ENDIF()
            ELSE()
                MESSAGE(WARNING "Bzip2 compression is disabled because BZIP2_PACKAGE does not reference an existing file: ${BZIP2_PACKAGE}")
            ENDIF()
        ENDIF()
        IF(BZIP2_SOURCE)
            SET(BZIP2_SOURCE "${BZIP2_SOURCE}" CACHE INTERNAL "The location of the bzip2 sources")
            MESSAGE(STATUS "Using bzip2 sources at ${BZIP2_SOURCE}")
            FOREACH(SRC blocksort bzlib compress crctable decompress huffman randtable)
                LIST(APPEND CHUCHO_BZIP2_SOURCES "${BZIP2_SOURCE}/${SRC}.c")
            ENDFOREACH()
            SET(CHUCHO_BZIP2_SOURCES ${CHUCHO_BZIP2_SOURCES} CACHE INTERNAL "The bzip2 sources to build")
        ENDIF()
    ELSEIF(BZIP2_INCLUDE_DIR OR BZIP2_LIB_DIR)
        IF(NOT BZIP2_INCLUDE_DIR)
            MESSAGE(WARNING "Bzip2 compression is disabled because If BZIP2_LIB_DIR is set, then BZIP2_INCLUDE_DIR must also be set")
            UNSET(BZIP2_LIB_DIR)
        ELSEIF(NOT BZIP2_LIB_DIR)
            MESSAGE(WARNING "Bzip2 compression is disabled because If BZIP2_INCLUDE_DIR is set, then BZIP2_LIB_DIR must also be set")
            UNSET(BZIP2_INCLUDE_DIR)
        ELSE()
            SET(BZIP2_INCLUDE_DIR "${BZIP2_INCLUDE_DIR}" CACHE INTERNAL "The location of bzlib.h")
            SET(BZIP2_LIB_DIR "${BZIP2_LIB_DIR}" CACHE INTERNAL "The location of libbz2")
        ENDIF()
    ELSEIF(NOT DISABLE_BZIP2)
        FIND_PACKAGE(BZip2)
        IF(BZIP2_FOUND)
            LIST(GET BZIP2_LIBRARIES 0 BZIP2_LIB_DIR)
            GET_FILENAME_COMPONENT(BZIP2_LIB_DIR "${BZIP2_LIB_DIR}" PATH)
            SET(BZIP2_LIB_DIR "${BZIP2_LIB_DIR}" CACHE INTERNAL "The location of libbz2")
        ELSE()
            MESSAGE(WARNING "Bzip2 compression is disabled because the library could not be found. Set DISABLE_BZIP2 to silence this warning.")
        ENDIF()
    ENDIF()
    IF(BZIP2_SOURCE OR BZIP2_INCLUDE_DIR)
        SET(CHUCHO_HAVE_BZIP2 TRUE CACHE INTERNAL "Whether we have found bzip2")
    ENDIF()
ENDIF()

# minizip (for compressing to a zip archive)
#
# You may enable minizip support for inclusion in the Chucho library by doing
# one of these three things:
#
#   * Set MINIZIP_URL to the URL from which to retrieve the minizip source archive
#   * Set MINIZIP_PACKAGE to the name of the minizip source zip archive
#   * Set MINIZIP_SOURCE to the name of the directory where an unpacked minizip sources live
#
IF(NOT CHUCHO_HAVE_MINIZIP)
    IF(MMINIZIP_URL OR MINIZIP_PACKAGE OR MINIZIP_SOURCE)
        IF(CHUCHO_HAVE_ZLIB)
            IF(MINIZIP_URL)
                CHUCHO_DOWNLOAD("${MINIZIP_URL}" MINIZIP_PACKAGE)
            ENDIF()
            IF(MINIZIP_PACKAGE)
                IF(EXISTS "${MINIZIP_PACKAGE}")
                    MESSAGE(STATUS "Unpacking the minizip package ${MINIZIP_PACKAGE}")
                    SET(MINIZIP_SOURCE "${CMAKE_BINARY_DIR}/minizip")
                    FILE(MAKE_DIRECTORY "${MINIZIP_SOURCE}")
                    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -E tar xzf "${MINIZIP_PACKAGE}"
                                    WORKING_DIRECTORY "${MINIZIP_SOURCE}"
                                    RESULT_VARIABLE CHUCHO_RESULT)
                    IF(NOT CHUCHO_RESULT EQUAL 0)
                        MESSAGE(WARNING "Zip archive compression is disabled because ${MINIZIP_PACKAGE} could not be unpacked")
                        UNSET(MINIZIP_SOURCE)
                    ENDIF()
                ELSE()
                    MESSAGE(WARNING "Zip archive compression is disabled because MINIZIP_PACKAGE does not reference an existing file: ${MINIZIP_PACKAGE}")
                ENDIF()
            ENDIF()
            IF(MINIZIP_SOURCE)
                SET(MINIZIP_SOURCE "${MINIZIP_SOURCE}" CACHE INTERNAL "The location of the minizip sources")
                MESSAGE(STATUS "Using minizip sources at ${MINIZIP_SOURCE}")
                FOREACH(SRC zip ioapi)
                    LIST(APPEND CHUCHO_MINIZIP_SOURCES "${MINIZIP_SOURCE}/${SRC}.c")
                ENDFOREACH()
                SET(CHUCHO_MINIZIP_SOURCES ${CHUCHO_MINIZIP_SOURCES} CACHE INTERNAL "The minizip sources to build")
                SET(CHUCHO_HAVE_MINIZIP TRUE CACHE INTERNAL "Whether we have found minizip or not")
                CHECK_SYMBOL_EXISTS(fopen64 stdio.h CHUCHO_HAVE_FOPEN64)
            ENDIF()
        ELSE()
            MESSAGE(WARNING "Minizip requires that zlib also be included")
        ENDIF()
    ENDIF()
ENDIF()
