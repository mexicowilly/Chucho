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

INCLUDE(CheckCXXCompilerFlag)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXSymbolExists)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckCXXSourceRuns)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(ExternalProject)
INCLUDE(CheckTypeSize)
INCLUDE(FindCURL)
INCLUDE(CheckStructHasMember)

# framework or not
SET(CHUCHO_NEEDS_TO_USE_THE_FRAMEWORK_VARIABLE_OR_CMAKE_COMPLAINS ${ENABLE_FRAMEWORK})

# We'll want this later
MACRO(CHUCHO_FIND_PROGRAM CHUCHO_FIND_VAR CHUCHO_PROGRAM)
    MESSAGE(STATUS "Looking for ${CHUCHO_PROGRAM}")
    FIND_PROGRAM(${CHUCHO_FIND_VAR} "${CHUCHO_PROGRAM}")
    MESSAGE(STATUS "Looking for ${CHUCHO_PROGRAM} - ${${CHUCHO_FIND_VAR}}")
ENDMACRO()

# Require symbols
MACRO(CHUCHO_REQUIRE_SYMBOLS CHUCHO_HEADER)
    FOREACH(SYM ${ARGN})
        CHECK_CXX_SYMBOL_EXISTS(${SYM} ${CHUCHO_HEADER} CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "The symbol ${SYM} could not be found in ${CHUCHO_HEADER}")
        ENDIF()
    ENDFOREACH()
ENDMACRO()

# Require C symbols
MACRO(CHUCHO_REQUIRE_C_SYMBOLS CHUCHO_HEADER)
    FOREACH(SYM ${ARGN})
        CHECK_SYMBOL_EXISTS(${SYM} ${CHUCHO_HEADER} CHUCHO_HAVE_${SYM})
        IF(NOT CHUCHO_HAVE_${SYM})
            MESSAGE(FATAL_ERROR "The symbol ${SYM} could not be found in ${CHUCHO_HEADER}")
        ENDIF()
    ENDFOREACH()
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
ELSEIF(CYGWIN)
    SET(CHUCHO_CYGWIN TRUE)
ENDIF()
IF(CMAKE_SYSTEM_NAME MATCHES "^.+BSD$")
    SET(CHUCHO_BSD TRUE)
ENDIF()
IF(NOT CHUCHO_WINDOWS)
    SET(CHUCHO_POSIX TRUE)
ENDIF()

# Set default build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Build type, one of: Release, Debug, RelWithDebInfo, or MinSizeRel" FORCE)
ENDIF()
MESSAGE(STATUS "Build type -- ${CMAKE_BUILD_TYPE}")

# Standards
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED TRUE)

# Compiler flags
IF(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    IF(CLANG_LIBSTDCXX)
        SET(CHUCHO_LIBCXX_FLAG -stdlib=libstdc++)
    ELSE()
        SET(CHUCHO_LIBCXX_FLAG -stdlib=libc++)
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(${CHUCHO_LIBCXX_FLAG} CHUCHO_HAS_LIBCXX_FLAG)
    IF(CHUCHO_HAS_LIBCXX_FLAG)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CHUCHO_LIBCXX_FLAG}")
    ELSE()
        MESSAGE(FATAL_ERROR "${CHUCHO_LIBCXX_FLAG} is required")
    ENDIF()
    IF(ENABLE_SHARED)
        CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden CHUCHO_VIS_FLAG)
        IF(CHUCHO_VIS_FLAG)
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fvisibility=hidden")
            SET(CHUCHO_SO_FLAGS "-fvisibility=hidden")
        ENDIF()
    ENDIF()
    IF(CMAKE_GENERATOR STREQUAL Xcode)
        SET(CMAKE_EXE_LINKER_FLAGS "-std=c++11 ${CHUCHO_LIBCXX_FLAG}")
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-Wno-potentially-evaluated-expression CHUCHO_HAVE_NO_POT_EVAL_EXP)
    IF(CHUCHO_HAVE_NO_POT_EVAL_EXP)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-potentially-evaluated-expression")
    ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
        MESSAGE(FATAL_ERROR "g++ version 4.7 or later is required")
    ENDIF()
    IF(ENABLE_SHARED)
        CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden CHUCHO_VIS_FLAG)
        IF(CHUCHO_VIS_FLAG)
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fvisibility=hidden")
            SET(CHUCHO_SO_FLAGS "-fvisibility=hidden")
        ENDIF()
    ENDIF()
ELSEIF(MSVC)
    IF(MSVC_VERSION LESS 1700)
        MESSAGE(FATAL_ERROR "Microsoft compiler version 17 or later is required (the compiler that ships with Visual Studio 2012)")
    ENDIF()
    IF(ENABLE_SHARED)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275 /EHsc")
    ENDIF()
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
    IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.14.0)
        MESSAGE(FATAL_ERROR "CC version 5.14.0 or later is required (the compiler that ships with Solaris Studio 12.5)")
    ENDIF()
    # CMake is supposed to do this with the standards flags. Jul 15 2017
    CHECK_CXX_COMPILER_FLAG(-std=c++11 CHUCHO_HAVE_CXX11)
    IF(CHUCHO_HAVE_CXX11)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ELSE()
        MESSAGE(FATAL_ERROR "-std=c++11 is required")
    ENDIF()
    CHECK_CXX_COMPILER_FLAG(-errtags=yes CHUCHO_HAVE_ERRTAGS)
    IF(CHUCHO_HAVE_ERRTAGS)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -errtags=yes")
    ENDIF()
    CHECK_C_COMPILER_FLAG(-errtags=yes CHUCHO_HAVE_C_ERRTAGS)
    IF(CHUCHO_HAVE_ERRTAGS)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -errtags=yes")
    ENDIF()
    SET(CHUCHO_SUNPRO_DISABLED_WARNINGS nonewline,wbadlkginit)
    CHECK_CXX_COMPILER_FLAG(-erroff=${CHUCHO_SUNPRO_DISABLED_WARNINGS} CHUCHO_HAVE_ERROFF)
    IF(CHUCHO_HAVE_ERROFF)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -erroff=${CHUCHO_SUNPRO_DISABLED_WARNINGS}")
    ELSE()
        MESSAGE(FATAL_ERROR "-erroff=${CHUCHO_SUNPRO_DISABLED_WARNINGS} is required")
    ENDIF()
    SET(CHUCHO_SUNPRO_C_DISABLED_WARNINGS E_NEWLINE_NOT_LAST)
    CHECK_C_COMPILER_FLAG(-erroff=${CHUCHO_SUNPRO_C_DISABLED_WARNINGS} CHUCHO_HAVE_C_ERROFF)
    IF(CHUCHO_HAVE_C_ERROFF)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -erroff=${CHUCHO_SUNPRO_C_DISABLED_WARNINGS}")
    ELSE()
        MESSAGE(FATAL_ERROR "-erroff=${CHUCHO_SUNPRO_DISABLED_WARNINGS} is required")
    ENDIF()
ENDIF()

# We are building Chucho
ADD_DEFINITIONS(-DCHUCHO_BUILD)

IF(CHUCHO_CYGWIN)
    ADD_DEFINITIONS(-D_BSD_SOURCE)
    SET(CMAKE_REQUIRED_DEFINITIONS -D_BSD_SOURCE)
ENDIF()

# Configure our export definitions
IF(ENABLE_SHARED)
    # Do we want to export private symbols for testing?
    IF(CHUCHO_PRIV_EXPORT)
        ADD_DEFINITIONS(-DCHUCHO_PRIV_EXPORT=CHUCHO_EXPORT)
    ENDIF()
ELSE()
    SET(CHUCHO_STATIC TRUE)
ENDIF()
MAKE_DIRECTORY("${CMAKE_BINARY_DIR}/chucho")
CONFIGURE_FILE(include/chucho/export.hpp.in "${CMAKE_BINARY_DIR}/chucho/export.hpp")
IF(C_API)
    CONFIGURE_FILE(include/chucho/export.hpp.in "${CMAKE_BINARY_DIR}/chucho/export.h")
ENDIF()

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

# Figure out the threads
FIND_PACKAGE(Threads REQUIRED)

IF(CHUCHO_POSIX)
    # headers
    FOREACH(HEAD arpa/inet.h assert.h fcntl.h limits.h netdb.h poll.h pthread.h signal.h
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
    CHUCHO_REQUIRE_SYMBOLS(sys/utsname.h uname)

    # getpid/access/close/write/pipe/read
    CHUCHO_REQUIRE_SYMBOLS(unistd.h getpid access close write pipe read)

    # stat/mkdir
    CHUCHO_REQUIRE_SYMBOLS(sys/stat.h stat mkdir mkfifo)

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
        CHUCHO_REQUIRE_SYMBOLS(dirent.h opendir readdir_r closedir)
        CHUCHO_REQUIRE_SYMBOLS(unistd.h pathconf)
        # The variable setting is flipped here. So, 1 is success, meaing the
        # program returned 0 for the exit code.
        CHECK_CXX_SOURCE_RUNS("#include <dirent.h>\n#include <unistd.h>\nint main() { return sizeof(struct dirent) >= pathconf(\"/\", _PC_NAME_MAX); }"
                              CHUCHO_DIRENT_NEEDS_NAME)
    ENDIF()

    # realpath
    CHUCHO_REQUIRE_SYMBOLS(stdlib.h realpath)

    # getaddrinfo/freeaddrinfo/gai_strerror/getnameinfo
    IF(CHUCHO_SOLARIS)
        SET(CMAKE_REQUIRED_LIBRARIES socket nsl)
    ENDIF()
    CHUCHO_REQUIRE_SYMBOLS(netdb.h getaddrinfo freeaddrinfo gai_strerror getnameinfo)
    IF(CHUCHO_SOLARIS)
        UNSET(CMAKE_REQUIRED_LIBRARIES)
    ENDIF()

    # syslog
    CHUCHO_REQUIRE_SYMBOLS(syslog.h syslog)

    # socket/sendto/connect/shutdown/send
    IF(CHUCHO_SOLARIS)
        SET(CMAKE_REQUIRED_LIBRARIES socket)
    ENDIF()
    CHUCHO_REQUIRE_SYMBOLS(sys/socket.h socket sendto connect shutdown send)
    IF(CHUCHO_SOLARIS)
        UNSET(CMAKE_REQUIRED_LIBRARIES)
    ENDIF()

    # poll
    CHUCHO_REQUIRE_SYMBOLS(poll.h poll)

    # signal stuff
    SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    CHUCHO_REQUIRE_SYMBOLS(signal.h sigemptyset sigwait sigpending sigismember pthread_sigmask)
    UNSET(CMAKE_REQUIRED_LIBRARIES)

    # open/fcntl
    CHUCHO_REQUIRE_SYMBOLS(fcntl.h open fcntl)

    # htonl
    CHUCHO_REQUIRE_SYMBOLS(arpa/inet.h htonl)

    # timegm
    CHECK_CXX_SYMBOL_EXISTS(timegm time.h CHUCHO_HAVE_TIMEGM)
    IF(NOT CHUCHO_HAVE_TIMEGM)
        CHUCHO_REQUIRE_SYMBOLS(stdlib.h setenv unsetenv)
        CHUCHO_REQUIRE_SYMBOLS(time.h tzset)
    ENDIF()

    # timezone stuff
    CHECK_CXX_SYMBOL_EXISTS(timezone time.h CHUCHO_HAVE_TIMEZONE)
    IF(CHUCHO_HAVE_TIMEZONE)
        CHECK_CXX_SOURCE_COMPILES("#include <time.h>\nint main() { long doggies = timezone / 60; return 0; }" CHUCHO_TIMEZONE_IS_INTEGRAL)
        IF(NOT CHUCHO_TIMEZONE_IS_INTEGRAL)
            UNSET(CHUCHO_HAVE_TIMEZONE)
	    UNSET(CHUCHO_HAVE_TIMEZONE CACHE)
        ENDIF()
    ENDIF()
    IF(NOT CHUCHO_HAVE_TIMEZONE)
        CHECK_STRUCT_HAS_MEMBER("struct tm" tm_gmtoff time.h CHUCHO_HAVE_TM_GMTOFF)
    ENDIF()

    # large file support for platform/posix/file_posix.cpp and platform/posix/file_writer_posix.cpp
    CHUCHO_FIND_PROGRAM(CHUCHO_GETCONF getconf)
    IF(CHUCHO_GETCONF)
        EXECUTE_PROCESS(COMMAND "${CHUCHO_GETCONF}" LFS_CFLAGS
                        RESULT_VARIABLE CHUCHO_GETCONF_RESULT
                        OUTPUT_VARIABLE CHUCHO_GETCONF_OUTPUT
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET)
        IF(CHUCHO_GETCONF_RESULT EQUAL 0)
            STRING(REPLACE -D "" CHUCHO_POSIX_FILE_DEFS "${CHUCHO_GETCONF_OUTPUT}")
            SEPARATE_ARGUMENTS(CHUCHO_POSIX_FILE_DEFS)
            MESSAGE(STATUS "Using large file flags ${CHUCHO_POSIX_FILE_DEFS}")
        ENDIF()
    ENDIF()
    CHECK_CXX_SYMBOL_EXISTS(O_LARGEFILE fcntl.h CHUCHO_HAVE_O_LARGEFILE)

    IF(CHUCHOD)
        CHECK_INCLUDE_FILE_CXX(pwd.h CHUCHO_HAVE_PWD_H)
        IF(NOT ${CHUCHO_HAVE_PWD_H})
            MESSAGE(FATAL_ERROR "The header pwd.h is required")
        ENDIF()
        CHUCHO_REQUIRE_SYMBOLS(unistd.h getuid fork setsid dup2 chdir _exit)
        IF(CHUCHO_SOLARIS)
            SET(CMAKE_REQUIRED_LIBRARIES socket)
        ENDIF()
        CHUCHO_REQUIRE_SYMBOLS(sys/socket.h recv bind listen accept)
        IF(CHUCHO_SOLARIS)
            UNSET(CMAKE_REQUIRED_LIBRARIES)
        ENDIF()
        CHUCHO_REQUIRE_SYMBOLS(pwd.h getpwuid)
        SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
        CHUCHO_REQUIRE_SYMBOLS(signal.h raise sigaddset sigaction kill)
        UNSET(CMAKE_REQUIRED_LIBRARIES)
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

# See if we can do the email writer
IF(ENABLE_CURL)
    FIND_PACKAGE(CURL)
    IF(CURL_FOUND)
        CHECK_INCLUDE_FILE_CXX(curl/curl.h CHUCHO_HAVE_CURL_INCLUDE)
        IF(NOT CHUCHO_HAVE_CURL_INCLUDE)
            SET(CHUCHO_CURL_INCLUDE_DIR ${CURL_INCLUDE_DIRS} CACHE INTERNAL "Checked include dirs for curl.h")
        ENDIF()
        SET(CMAKE_REQUIRED_INCLUDES ${CURL_INCLUDE_DIRS})
        SET(CMAKE_REQUIRED_LIBRARIES ${CURL_LIBRARIES})
        CHUCHO_REQUIRE_SYMBOLS(curl/curl.h curl_global_init curl_easy_init curl_easy_cleanup
                               curl_easy_setopt curl_easy_perform curl_version_info curl_easy_strerror
                               curl_slist_free_all)
        UNSET(CMAKE_REQUIRED_INCLUDES)
        UNSET(CMAKE_REQUIRED_LIBRARIES)
    ENDIF()
ENDIF()

# Doors
IF(CHUCHO_SOLARIS)
    CHECK_INCLUDE_FILE_CXX(door.h CHUCHO_HAVE_DOOR_H)
    IF(CHUCHO_HAVE_DOOR_H)
        CHUCHO_REQUIRE_SYMBOLS(door.h door_call door_getparam door_create door_return)
        CHUCHO_REQUIRE_SYMBOLS(stropts.h fattach)
        SET(CHUCHO_HAVE_DOORS TRUE CACHE INTERNAL "Whether we have doors")
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
# We have to check whether it exists and whether it is buggy. put_time
# on VS2012 is broken when it tries to format time zones. It just crashes.
CHECK_CXX_SOURCE_COMPILES("#include <iomanip>\nint main() { std::tm t; std::put_time(&t, \\\"%Y\\\"); return 0; }"
                          CHUCHO_HAVE_PUT_TIME_EXISTENCE)
IF(CHUCHO_HAVE_PUT_TIME_EXISTENCE)
    IF(CHUCHO_WINDOWS)
        SET(CMAKE_REQUIRED_DEFINITIONS -DCHUCHO_WINDOWS)
    ENDIF()
    CHECK_CXX_SOURCE_RUNS("
#include <iomanip>
#include <iostream>
#include <time.h>
int main()
{
#if defined(CHUCHO_WINDOWS)
    __try
    {
#endif
    time_t t = time(nullptr);
    struct tm* cal = localtime(&t);
    std::cout << std::put_time(cal, \\\"%z\\\");
#if defined(CHUCHO_WINDOWS)
    }
    __except()
    {
    return EXIT_FAILURE;
    }
#endif
    return EXIT_SUCCESS;
}" CHUCHO_HAVE_PUT_TIME_RUNS)
    IF(CHUCHO_HAVE_PUT_TIME_RUNS)
        SET(CHUCHO_HAVE_PUT_TIME TRUE CACHE INTERNAL "Whether std::put_time runs")
    ENDIF()
    IF(CHUCHO_WINDOWS)
        UNSET(CMAKE_REQUIRED_DEFINITIONS)
    ENDIF()
ENDIF()

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
            SET(CHUCHO_HAVE_STD_REGEX TRUE CACHE INTERNAL "Whether std::regex actually works")
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
    CHUCHO_REQUIRE_SYMBOLS(regex.h regcomp regerror regexec regfree REG_EXTENDED)
    MESSAGE(STATUS "Using regular expressions - POSIX")
ENDIF()

# assert
CHUCHO_REQUIRE_SYMBOLS(assert.h assert)

# C API required stuff
IF(C_API)
    CHECK_C_SOURCE_COMPILES("
#include <stdio.h>
#define VA_CHK(...) printf(__VA_ARGS__)
int main()
{
    VA_CHK(\"%s\", \"hello\");
    return 0;
}" CHUCHO_HAVE_VA_MACRO)
    IF(NOT CHUCHO_HAVE_VA_MACRO)
        MESSAGE(FATAL_ERROR "C macros with variadic arguments are required (standard C99)")
    ENDIF()

    # C headers
    # (we already checked for stdlib.h)
    FOREACH(HEAD stdio.h string.h)
        CHECK_INCLUDE_FILE(${HEAD} HAVE_${HEAD})
        IF (NOT HAVE_${HEAD})
            MESSAGE(FATAL_ERROR "${HEAD} is required")
        ENDIF()
    ENDFOREACH()

    # fopen/fgets/fclose/remove
    CHUCHO_REQUIRE_C_SYMBOLS(stdio.h fopen fgets fclose remove fwrite)

    # strdup/strstr/strcmp
    CHUCHO_REQUIRE_C_SYMBOLS(string.h strstr strcmp strlen strcpy strcat)

    # calloc/free/malloc
    CHUCHO_REQUIRE_C_SYMBOLS(stdlib.h calloc free malloc)

    # On Solaris we don't get the transitive linkage
    # to the C++ runtime when linking a C program against a Chucho
    # shared object. So, we need to figure out where the C++
    # runtime is and add it to the target link libraries of the
    # C unit test app.
    IF(C_API AND ENABLE_SHARED AND CHUCHO_SOLARIS AND NOT DEFINED CHUCHO_STD_CXX_LIBS)
        CHUCHO_FIND_PROGRAM(CHUCHO_LDD ldd)
        IF(NOT CHUCHO_LDD)
            MESSAGE(FATAL_ERROR "Could not find ldd")
        ENDIF()
        MESSAGE(STATUS "Looking for standard C++ libraries")
        FILE(WRITE "${CMAKE_BINARY_DIR}/libstdc++-check.cpp"
             "#include <string>\nint main() { std::string s; return 0; }")
        TRY_COMPILE(CHUCHO_CXX_RESULT
                    "${CMAKE_BINARY_DIR}/libstdc++-check.out"
                    "${CMAKE_BINARY_DIR}/libstdc++-check.cpp"
                    COPY_FILE "${CMAKE_BINARY_DIR}/libstdc++-check")
        IF(NOT CHUCHO_CXX_RESULT)
            MESSAGE(FATAL_ERROR "Could not compile the program to find libstdc++")
        ENDIF()
        EXECUTE_PROCESS(COMMAND "${CHUCHO_LDD}" "${CMAKE_BINARY_DIR}/libstdc++-check"
                        RESULT_VARIABLE CHUCHO_LDD_RESULT
                        OUTPUT_VARIABLE CHUCHO_LDD_OUTPUT)
        IF(NOT CHUCHO_LDD_RESULT EQUAL 0)
            MESSAGE(FATAL_ERROR "Error running ldd to find libstdc++")
        ENDIF()
        FILE(WRITE "${CMAKE_BINARY_DIR}/libstdc++-check-ldd.out"
             "${CHUCHO_LDD_OUTPUT}")
        FILE(STRINGS "${CMAKE_BINARY_DIR}/libstdc++-check-ldd.out" CHUCHO_LDD_OUTPUT)
        FOREACH(LINE ${CHUCHO_LDD_OUTPUT})
            IF(LINE MATCHES libstdc)
                STRING(REGEX REPLACE
                       "^.+=>[ \\\t]*(.+libstdc.+)$"
                       "\\1"
                       CHUCHO_LIBSTDCXX
                       "${LINE}")
            ENDIF()
            IF(LINE MATCHES libCrun)
                STRING(REGEX REPLACE
                       "^.+=>[ \\\t]*(.+libCrun.+)$"
                       "\\1"
                       CHUCHO_LIBCRUN
                       "${LINE}")
            ENDIF()
        ENDFOREACH()
        SET(CHUCHO_STD_CXX_LIBS ${CHUCHO_LIBSTDCXX} ${CHUCHO_LIBCRUN} CACHE INTERNAL "The location of the stdandard C++ runtime library")
        IF(NOT CHUCHO_STD_CXX_LIBS)
            MESSAGE(WARNING "Could not determine the location of stdandard C++ runtime libraries. The C unit tests cannot be built.")
        ENDIF()
        MESSAGE(STATUS "Looking for standard C++ libraries - ${CHUCHO_STD_CXX_LIBS}")
    ENDIF()
ENDIF()

# Oracle
IF(ORACLE_INCLUDE_DIR)
    SET(CMAKE_REQUIRED_INCLUDES "${ORACLE_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(oci.h CHUCHO_OCI_H)
    IF(CHUCHO_OCI_H)
        IF(ORACLE_CLIENT_LIB)
            SET(CMAKE_REQUIRED_LIBRARIES "${ORACLE_CLIENT_LIB}")
            CHUCHO_REQUIRE_SYMBOLS(oci.h OCIEnvCreate OCIHandleAlloc OCILogon2 OCIStmtPrepare OCIBindByName
                                   OCIStmtExecute OCIHandleFree OCIErrorGet OCIDescriptorAlloc OCIDescriptorFree)
        ELSEIF(ENABLE_SHARED)
            MESSAGE(FATAL_ERROR "You must specify ORACLE_CLIENT_LIB to build a shared library")
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "oci.h was not found in ORACLE_INCLUDE_DIR=${ORACLE_INCLUDE_DIR}")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_ORACLE TRUE CACHE INTERNAL "Whether we have Oracle client software")
ENDIF()

# MySQL
IF(MYSQL_INCLUDE_DIR)
    SET(CMAKE_REQUIRED_INCLUDES "${MYSQL_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(mysql.h CHUCHO_MYSQL_H)
    IF(CHUCHO_MYSQL_H)
        IF(MYSQL_CLIENT_LIB)
            SET(CMAKE_REQUIRED_LIBRARIES "${MYSQL_CLIENT_LIB}")
            CHUCHO_REQUIRE_SYMBOLS(mysql.h mysql_init mysql_real_connect mysql_stmt_init mysql_stmt_prepare mysql_autocommit
                                   mysql_stmt_close mysql_close mysql_stmt_bind_param mysql_stmt_execute)
        ELSEIF(ENABLE_SHARED)
            MESSAGE(FATAL_ERROR "You must specify MYSQL_CLIENT_LIB to build a shared library")
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "mysql.h was not found in MYSQL_INCLUDE_DIR=${MYSQL_INCLUDE_DIR}")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_MYSQL TRUE CACHE INTERNAL "Whether we have MySQL client software")
ENDIF()

# SQLite
IF(SQLITE_INCLUDE_DIR)
    SET(CMAKE_REQUIRED_INCLUDES "${SQLITE_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(sqlite3.h CHUCHO_SQLITE_H)
    IF(CHUCHO_SQLITE_H)
        IF(SQLITE_CLIENT_LIB)
            SET(CMAKE_REQUIRED_LIBRARIES "${SQLITE_CLIENT_LIB}")
            CHUCHO_REQUIRE_SYMBOLS(sqlite3.h sqlite3_threadsafe sqlite3_open sqlite3_prepare_v2 sqlite3_reset sqlite3_bind_text
                                   sqlite3_bind_int64 sqlite3_bind_int sqlite3_step sqlite3_finalize sqlite3_close
                                   sqlite3_extended_result_codes sqlite3_extended_errcode sqlite3_errmsg)
        ELSEIF(ENABLE_SHARED)
            MESSAGE(FATAL_ERROR "You must specify SQLITE_CLIENT_LIB to build a shared library")
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "sqlite3.h was not found in SQLITE_INCLUDE_DIR=${SQLITE_INCLUDE_DIR}")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_SQLITE TRUE CACHE INTERNAL "Whether we have SQLite client software")
ENDIF()

# PostgreSQL
IF(POSTGRES_INCLUDE_DIR)
    SET(CMAKE_REQUIRED_INCLUDES "${POSTGRES_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(libpq-fe.h CHUCHO_POSTGRES_H)
    IF(CHUCHO_POSTGRES_H)
        IF(POSTGRES_CLIENT_LIB)
            SET(CMAKE_REQUIRED_LIBRARIES "${POSTGRES_CLIENT_LIB}")
            CHUCHO_REQUIRE_SYMBOLS(libpq-fe.h PQconnectdb PQprepare PQexecPrepared PQresultStatus PQresultErrorMessage
                                   PQclear PQfinish PQstatus PQerrorMessage)
        ELSEIF(ENABLE_SHARED)
            MESSAGE(FATAL_ERROR "You must specify POSTGRES_CLIENT_LIB to build a shared library")
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "libpq-fe.h was not found in POSTGRES_INCLUDE_DIR=${POSTGRES_INCLUDE_DIR}")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_POSTGRES TRUE CACHE INTERNAL "Whether we have PostgreSQL client software")
ENDIF()

# DB2
IF(DB2_INCLUDE_DIR)
    SET(CMAKE_REQUIRED_INCLUDES "${DB2_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(sqlcli1.h CHUCHO_DB2_H)
    IF(CHUCHO_DB2_H)
        IF(DB2_CLIENT_LIB)
            SET(CMAKE_REQUIRED_LIBRARIES "${DB2_CLIENT_LIB}")
            CHUCHO_REQUIRE_SYMBOLS(sqlcli1.h SQLAllocHandle SQLFreeHandle SQLSetEnvAttr
                                   SQLConnect SQLBindParameter SQLPrepare SQLExecute
                                   SQLDisconnect SQLGetDiagRec)
        ELSEIF(ENABLE_SHARED)
            MESSAGE(FATAL_ERROR "You must specify DB2_CLIENT_LIB to build a shared library")
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "sqlci1.h was not found in DB2_INCLUDE_DIR=${DB2_INCLUDE_DIR}")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_DB2 TRUE CACHE INTERNAL "Whether we have DB2 client software")
ENDIF()

# Ruby
MACRO(CHUCHO_CHECK_RUBY_SYMBOLS CHUCHO_RUBY_HEADER)
    CHUCHO_REQUIRE_SYMBOLS(${CHUCHO_RUBY_HEADER} ruby_init ruby_finalize rb_funcall rb_str_new2
                           rb_define_variable rb_path2class rb_intern rb_rescue rb_eval_string
                           Qtrue Qfalse Qnil INT2NUM StringValueCStr)
ENDMACRO()

IF(RUBY_INCLUDE_DIR)
    FILE(GLOB_RECURSE CHUCHO_RUBY_CONFIG_DIR "${RUBY_INCLUDE_DIR}/config.h")
    IF(NOT CHUCHO_RUBY_CONFIG_DIR)
        MESSAGE(FATAL_ERROR "Could not find the location of Ruby platform-specific headers")
    ENDIF()
    STRING(FIND "${CHUCHO_RUBY_CONFIG_DIR}" ruby CHUCHO_RUBY_POS REVERSE)
    MATH(EXPR CHUCHO_RUBY_POS "${CHUCHO_RUBY_POS} - 1")
    STRING(SUBSTRING "${CHUCHO_RUBY_CONFIG_DIR}" 0 ${CHUCHO_RUBY_POS} CHUCHO_RUBY_CONFIG_DIR)
    SET(CMAKE_REQUIRED_INCLUDES "${RUBY_INCLUDE_DIR}" "${CHUCHO_RUBY_CONFIG_DIR}")
    CHECK_INCLUDE_FILE_CXX(ruby.h CHUCHO_RUBY_H)
    IF(CHUCHO_RUBY_H)
        IF(RUBY_LIB)
            SET(CMAKE_REQUIRED_LIBRARIES "${RUBY_LIB}")
            CHUCHO_CHECK_RUBY_SYMBOLS(ruby.h)
        ELSEIF(ENABLE_SHARED)
            MESSAGE(FATAL_ERROR "You must specify RUBY_LIB to build a shared library with Ruby support") 
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "ruby.h was not found in RUBY_INCLUDE_DIR=${RUBY_INCLUDE_DIR}")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_RUBY TRUE CACHE INTERNAL "Whether we have Ruby client software")
ELSEIF(RUBY_FRAMEWORK)
    IF(CHUCHO_MACINTOSH)
        SET(CMAKE_REQUIRED_FLAGS "-framework Ruby")
        CHUCHO_CHECK_RUBY_SYMBOLS(Ruby/ruby.h)
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_REQUIRED_FLAGS}")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_REQUIRED_FLAGS}")
        UNSET(CMAKE_REQUIRED_FLAGS)
        SET(CHUCHO_HAVE_RUBY TRUE CACHE INTERNAL "Whether we have a Ruby framework")
    ELSE()
        MESSAGE(WARNING "RUBY_FRAMEWORK can only take effect on Macintosh")
    ENDIF()
ENDIF()

# protobuf
IF(PROTOBUF_INCLUDE_DIR AND PROTOBUF_LIB AND PROTOC_DIR)
    FIND_PROGRAM(CHUCHO_PROTOC
                 protoc
                 PATHS "${PROTOC_DIR}"
                 NO_DEFAULT_PATH)
    IF(NOT CHUCHO_PROTOC)
        MESSAGE(FATAL_ERROR "The variable PROTOC_DIR was provided as ${PROTOC_DIR}, but it does not contain the protoc program")
    ENDIF()
    SET(CMAKE_REQUIRED_INCLUDES "${PROTOBUF_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(google/protobuf/message.h CHUCHO_GOOGLE_PROTOBUF_MESSAGE_H)
    IF(NOT CHUCHO_GOOGLE_PROTOBUF_MESSAGE_H)
        MESSAGE(FATAL_ERROR "The variable PROTOBUF_INCLUDE_DIR was provided as ${PROTOBUF_INCLUDE_DIR}, but it does not contain the protobuf headers")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    IF(NOT EXISTS "${PROTOBUF_LIB}")
        MESSAGE(FATAL_ERROR "The variable PROTOBUF_LIB was provided as ${PROTOBUF_LIB}, but it does not refer to an existing file")
    ENDIF()
    LIST(APPEND CHUCHO_SERIALIZER_SOURCES
        "${CMAKE_BINARY_DIR}/chucho.pb.cc"
        "${CMAKE_BINARY_DIR}/chucho.pb.h")
    SET(CHUCHO_HAVE_PROTOBUF TRUE CACHE INTERNAL "Whether we have protobuf")
ELSEIF(PROTOBUF_INCLUDE_DIR OR PROTOBUF_LIB OR PROTOC_DIR)
    MESSAGE(WARNING "If any of the variables PROTOBUF_INCLUDE_DIR, PROTOBUF_LIB or PROTOC_DIR have been set, then they must all be set for protobuf support to be included")
ENDIF()

# Cap'n Proto
IF(CAPN_PROTO_INCLUDE_DIR AND CAPN_PROTO_LIB AND CAPN_PROTO_KJ_LIB AND CAPNP_DIR)
    FIND_PROGRAM(CHUCHO_CAPNP
                 capnp
                 PATHS "${CAPNP_DIR}"
                 NO_DEFAULT_PATH)
    IF(NOT CHUCHO_CAPNP)
        MESSAGE(FATAL_ERROR "The variable CAPNP_DIR was provided as ${CAPNP_DIR}, but it does not contain the capnp program")
    ENDIF()
    FIND_PROGRAM(CHUCHO_CAPNPC_CXX
                 capnpc-c++
                 PATHS "${CAPNP_DIR}"
                 NO_DEFAULT_PATH)
    IF(NOT CHUCHO_CAPNPC_CXX)
        MESSAGE(FATAL_ERROR "The variable CAPNP_DIR was provided as ${CAPNP_DIR}, but it does not contain the capnpc-c++ program")
    ENDIF()
    SET(CMAKE_REQUIRED_INCLUDES "${CAPN_PROTO_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(capnp/serialize.h CHUCHO_CAPN_PROTO_SERIALIZE_PACKED_H)
    IF(NOT CHUCHO_CAPN_PROTO_MESSAGE_H OR NOT CHUCHO_CAPN_PROTO_SERIALIZE_PACKED_H OR NOT CHUCHO_CAPN_PROTO_VECTOR_H)
        MESSAGE(FATAL_ERROR "The variable CAPN_PROTO_INCLUDE_DIR was provided as ${CAPN_PROTO_INCLUDE_DIR}, but it does not contain the Cap'n Proto headers")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    IF(NOT EXISTS "${CAPN_PROTO_LIB}")
        MESSAGE(FATAL_ERROR "The variable CAPN_PROTO_LIB was provided as ${CAPN_PROTO_LIB}, but it does not refer to an existing file")
    ENDIF()
    IF(NOT EXISTS "${CAPN_PROTO_KJ_LIB}")
        MESSAGE(FATAL_ERROR "The variable CAPN_PROTO_KJ_LIB was provided as ${CAPN_PROTO_KJ_LIB}, but it does not refer to an existing file")
    ENDIF()
    LIST(APPEND CHUCHO_SERIALIZER_SOURCES
        "${CMAKE_BINARY_DIR}/chucho.capnp.c++"
        "${CMAKE_BINARY_DIR}/chucho.capnp.h")
    SET(CHUCHO_HAVE_CAPN_PROTO TRUE CACHE INTERNAL "Whether we have Cap'n Proto")
ELSEIF(CAPN_PROTO_INCLUDE_DIR OR CAPN_PROTO_LIB OR CAPN_PROTO_KJ_LIB OR CAPNP_DIR)
    MESSAGE(WARNING "If any of the variables CAPN_PROTO_INCLUDE_DIR, CAPN_PROTO_LIB, CAPN_PROTO_KJ_LIB or CAPNP_DIR have been set, then they must all be set for Cap'n Proto support to be included")
ENDIF()

# zeromq
IF(ZEROMQ_INCLUDE_DIR AND ZEROMQ_LIB)
    SET(CMAKE_REQUIRED_INCLUDES "${ZEROMQ_INCLUDE_DIR}")
    CHECK_INCLUDE_FILE_CXX(zmq.h CHUCHO_ZMQ_H)
    IF(NOT CHUCHO_ZMQ_H)
        MESSAGE(FATAL_ERROR "The variable ZEROMQ_INCLUDE_DIR was provided as ${ZEROMQ_INCLUDE_DIR}, but it does not contain the zeromq headers")
    ENDIF()
    IF(NOT EXISTS "${ZEROMQ_LIB}")
        MESSAGE(FATAL_ERROR "The variable ZEROMQ_LIB was provided as ${ZEROMQ_LIB}, but it does not refer to an existing file")
    ENDIF()
    SET(CMAKE_REQUIRED_LIBRARIES "${ZEROMQ_LIB}" ${CMAKE_THREAD_LIBS_INIT})
    IF(CHUCHO_SOLARIS)
        SET(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES};socket;nsl")
    ENDIF()
    CHUCHO_REQUIRE_SYMBOLS(zmq.h zmq_ctx_new zmq_ctx_destroy zmq_socket zmq_close zmq_connect zmq_msg_send
                           zmq_msg_init_size zmq_msg_data zmq_strerror zmq_msg_close zmq_bind)
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_ZEROMQ TRUE CACHE INTERNAL "Whether we have zeromq")
ELSEIF(ZEROMQ_INCLUDE_DIR OR ZEROMQ_LIB)
    MESSAGE(WARNING "If either of the variables ZEROMQ_INCLUDE_DIR or ZEROMQ_LIB has been set, then they must both be set for zeromq support to be included")
ENDIF()

# ActiveMQ
IF(ACTIVEMQ_INCLUDE_DIR AND ACTIVEMQ_LIB)
    SET(CMAKE_REQUIRED_INCLUDES "${ACTIVEMQ_INCLUDE_DIR}")
    FOREACH(HEAD
            cms/ConnectionFactory.h
            cms/Connection.h
            cms/Session.h
            activemq/library/ActiveMQCPP.h
            cms/ExceptionListener.h)
        STRING(REPLACE . _ CHUCHO_HEAD_VAR_NAME CHUCHO_HAVE_${HEAD})
        STRING(REPLACE / _ CHUCHO_HEAD_VAR_NAME ${CHUCHO_HEAD_VAR_NAME})
        STRING(TOUPPER ${CHUCHO_HEAD_VAR_NAME} CHUCHO_HEAD_VAR_NAME)
        CHECK_INCLUDE_FILE_CXX(${HEAD} ${CHUCHO_HEAD_VAR_NAME})
        IF(NOT ${CHUCHO_HEAD_VAR_NAME})
            MESSAGE(FATAL_ERROR "The variable ACTIVEMQ_INCLUDE_DIR was provided as ${ACTIVEMQ_INCLUDE_DIR}, but it does not contain the ActiveMQ header ${HEAD}")
        ENDIF()
    ENDFOREACH()
    IF(NOT EXISTS "${ACTIVEMQ_LIB}")
        MESSAGE(FATAL_ERROR "The variable ACTIVEMQ_LIB was provided as ${ACTIVEMQ_LIB}, but it does not refer to an existing file")
    ENDIF()
    IF(APR_LIB)
        IF(NOT EXISTS "${APR_LIB}")
            MESSAGE(WARNING "The variable APR_LIB was provided as ${APR_LIB}, but it does not refer to an existing file. The unit tests cannot be built.")
            UNSET(APR_LIB)
            UNSET(APR_LIB CACHE)
        ELSE()
            SET(CMAKE_REQUIRED_LIBRARIES "${ACTIVEMQ_LIB}" "${APR_LIB}")
            IF(CHUCHO_SOLARIS)
                SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} socket nsl)
            ENDIF()
            SET(CHUCHO_LINK_ACTIVEMQ_SOURCE "#include <cms/ConnectionFactory.h>\nint main() { cms::ConnectionFactory::createCMSConnectionFactory(\"127.0.0.1:6131\"); return 0; }")
            CHECK_CXX_SOURCE_COMPILES("${CHUCHO_LINK_ACTIVEMQ_SOURCE}" CHUCHO_LINK_ACTIVEMQ)
            IF(NOT CHUCHO_LINK_ACTIVEMQ)
                SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} sendfile)
                CHECK_CXX_SOURCE_COMPILES("${CHUCHO_LINK_ACTIVEMQ_SOURCE}" CHUCHO_LINK_ACTIVEMQ_WITH_SENDFILE)
                IF(CHUCHO_LINK_ACTIVEMQ_WITH_SENDFILE)
                    SET(CHUCHO_NEEDS_SENDFILE TRUE)
                ELSE()
                    MESSAGE(WARNING "There was a failure linking to the ActiveMQ library. The unit tests cannot be built.")
                ENDIF()
            ENDIF()
        ENDIF()
    ELSE()
        MESSAGE(STATUS "Without the APR_LIB variable being set along with the ActiveMQ variables, the unit tests cannot be built.")
    ENDIF()
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_ACTIVEMQ TRUE CACHE INTERNAL "Whether we have ActiveMQ")
ELSEIF(ACTIVEMQ_INCLUDE_DIR OR ACTIVEMQ_LIB OR APR_LIB)
    MESSAGE(WARNING "If any of the variables ACTIVEMQ_INCLUDE_DIR, ACTIVEMQ_LIB or APR_LIB has been set, then they must all be set for ActiveMQ support to be included")
ENDIF()

# RabbitMQ
IF(RABBITMQ_INCLUDE_DIR AND RABBITMQ_LIB)
    SET(CMAKE_REQUIRED_INCLUDES "${RABBITMQ_INCLUDE_DIR}")
    FOREACH(HEAD amqp_tcp_socket amqp)
        CHECK_INCLUDE_FILE_CXX(${HEAD}.h CHUCHO_HAVE_${HEAD})
        IF(NOT CHUCHO_HAVE_${HEAD})
            MESSAGE(FATAL_ERROR "The variable RABBITMQ_INCLUDE_DIR was provided as ${RABBITMQ_INCLUDE_DIR}, but it does not contain the RabbitMQ headers")
        ENDIF()
    ENDFOREACH()
    IF(NOT EXISTS "${RABBITMQ_LIB}")
        MESSAGE(FATAL_ERROR "The variable RABBITMQ_LIB was provided as ${RABBITMQ_LIB}, but it does not refer to an existing file")
    ENDIF()
    SET(CMAKE_REQUIRED_LIBRARIES "${RABBITMQ_LIB}" ${CMAKE_THREAD_LIBS_INIT})
    IF(CHUCHO_SOLARIS)
        SET(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES};socket;nsl")
    ENDIF()
    CHUCHO_REQUIRE_SYMBOLS(amqp.h amqp_new_connection amqp_socket_open amqp_login amqp_get_rpc_reply
                           amqp_channel_close amqp_connection_close amqp_basic_publish amqp_channel_open
                           amqp_destroy_connection amqp_error_string2 amqp_cstring_bytes amqp_queue_declare
                           amqp_basic_consume amqp_consume_message amqp_empty_bytes amqp_empty_table
                           amqp_destroy_envelope)
    CHUCHO_REQUIRE_SYMBOLS(amqp_tcp_socket.h amqp_tcp_socket_new amqp_parse_url)
    UNSET(CMAKE_REQUIRED_INCLUDES)
    UNSET(CMAKE_REQUIRED_LIBRARIES)
    SET(CHUCHO_HAVE_RABBITMQ TRUE CACHE INTERNAL "Whether we have RabbitMQ")
ELSEIF(RABBITMQ_INCLUDE_DIR OR RABBITMQ_LIB)
    MESSAGE(WARNING "If either of the variables RABBITMQ_INCLUDE_DIR or RABBITMQ_LIB has been set, then they must both be set for RabbitMQ support to be included")
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

# Macintosh launchd stuff
IF(CHUCHO_MACINTOSH)
    CHUCHO_FIND_PROGRAM(CHUCHO_LAUNCHCTL launchctl)
    IF(NOT CHUCHO_LAUNCHCTL)
        MESSAGE(FATAL_ERROR "launchctl is required")
    ENDIF()
    IF(INSTALL_SERVICE)
        CHUCHO_REQUIRE_SYMBOLS(sys/event.h kqueue kevent EV_SET EVFILT_READ EV_ADD)
        CHUCHO_REQUIRE_SYMBOLS(launch.h launch_data_new_string LAUNCH_KEY_CHECKIN
                               launch_msg launch_data_free launch_data_get_type launch_data_get_errno
                               launch_data_dict_lookup LAUNCH_JOBKEY_SOCKETS launch_data_array_get_count
                               launch_data_array_get_index launch_data_get_fd)
        SET(CMAKE_EXTRA_INCLUDE_FILES launch.h)
        CHECK_TYPE_SIZE(launch_data_t CHUCHO_LAUNCH_DATA_T_SIZE)
        IF(CHUCHO_LAUNCH_DATA_T_SIZE STREQUAL "")
            MESSAGE(FATAL_ERROR "The launch_data_t type could not be found")
        ENDIF()
        UNSET(CMAKE_EXTRA_INCLUDE_FILES)
    ENDIF()
ENDIF()

# Linux service stuff
IF(CHUCHO_LINUX)
    CHUCHO_FIND_PROGRAM(CHUCHO_INIT init)
    IF(CHUCHO_INIT)
        EXECUTE_PROCESS(COMMAND "${CHUCHO_INIT}" --version
                        OUTPUT_VARIABLE CHUCHO_INIT_OUT
                        ERROR_QUIET)
    ENDIF()
    IF(CHUCHO_INIT_OUT AND CHUCHO_INIT_OUT MATCHES upstart)
        CHUCHO_FIND_PROGRAM(CHUCHO_INITCTL initctl)
        IF(CHUCHO_INITCTL)
            MESSAGE(STATUS "This Linux is using the Upstart init system")
            SET(CHUCHO_UPSTART_INIT TRUE)
        ELSE()
            MESSAGE(WARNING "initctl is required when using the Upstart init system. The chuchod service will not be installed.")
        ENDIF()
    ELSE()
        CHUCHO_FIND_PROGRAM(CHUCHO_SYSTEMCTL systemctl)
        IF(CHUCHO_SYSTEMCTL)
            EXECUTE_PROCESS(COMMAND "${CHUCHO_SYSTEMCTL}"
                            OUTPUT_VARIABLE CHUCHO_SYSTEMCTL_OUT
                            ERROR_QUIET)
            IF(CHUCHO_SYSTEMCTL_OUT AND CHUCHO_SYSTEMCTL_OUT MATCHES "-\\.mount")
                MESSAGE(STATUS "This Linux is using the systemd init system")
                SET(CHUCHO_SYSTEMD_INIT TRUE)
            ELSE()
                MESSAGE(WARNING "systemctl is required when using the systemd init system. The chuchod service will not be installed.")
            ENDIF()
        ELSE()
            MESSAGE(STATUS "Chucho only supports the Upstart and systemd init systems on Linux.")
        ENDIF()
    ENDIF()
ENDIF()

# BSD service stuff
IF(CHUCHO_BSD AND EXISTS /etc/rc AND EXISTS /etc/rc.conf AND EXISTS /etc/rc.subr AND IS_DIRECTORY /etc/rc.d)
    CHUCHO_FIND_PROGRAM(CHUCHO_SED sed)
    IF(NOT CHUCHO_SED)
        MESSAGE(FATAL_ERROR "sed is required when using the rc.d init system")
    ENDIF()
    MESSAGE(STATUS "This ${CMAKE_SYSTEM_NAME} is using the rc.d init system")
    SET(CHUCHO_RC_INIT TRUE)
ENDIF()

#
# External projects
#
ADD_CUSTOM_TARGET(external)
SET(CHUCHO_EXTERNAL_PREFIX "${CMAKE_BINARY_DIR}" CACHE STRING "The path to Chucho external projects")
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX "${CHUCHO_EXTERNAL_PREFIX}")

# Gtest
IF(CHUCHO_WINDOWS)
    SET(CHUCHO_GTEST_GENERATOR "NMake Makefiles")
ELSE()
    SET(CHUCHO_GTEST_GENERATOR "Unix Makefiles")
ENDIF()

IF(MSVC)
    SET(CHUCHO_ADDL_GTEST_CXX_FLAGS "-D_VARIADIC_MAX=10")
ELSEIF(CHUCHO_CYGWIN)
    SET(CHUCHO_ADDL_GTEST_CXX_FLAGS "-D_BSD_SOURCE")
ENDIF()

IF(CHUCHO_WINDOWS)
    SET(CHUCHO_GTEST_CMAKE_FLAGS -Dgtest_force_shared_crt:BOOL=ON)
ENDIF()

# Not all cmakes can download URLs of the https variety. Therefore, if
# there is no specific package, we just clone the repository.
IF(GTEST_PACKAGE AND EXISTS "${GTEST_PACKAGE}")
    FILE(SHA1 "${GTEST_PACKAGE}" CHUCHO_GTEST_SHA1)
    FILE(TO_CMAKE_PATH "${GTEST_PACKAGE}" GTEST_PACKAGE_CMAKE_PATH)
    SET(CHUCHO_GTEST_PACKAGE_ARGS
        URL "${GTEST_PACKAGE_CMAKE_PATH}"
        URL_HASH SHA1=${CHUCHO_GTEST_SHA1})
ELSE()
    SET(CHUCHO_GTEST_PACKAGE_ARGS
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.0)
ENDIF()
ExternalProject_Add(gtest-external
                    ${CHUCHO_GTEST_PACKAGE_ARGS}
                    CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE} "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} ${CHUCHO_ADDL_GTEST_CXX_FLAGS}" -DBUILD_GTEST=ON -DBUILD_GMOCK=OFF "-DCMAKE_INSTALL_PREFIX=${CHUCHO_EXTERNAL_PREFIX}" ${CHUCHO_GTEST_CMAKE_FLAGS}
                    CMAKE_GENERATOR "${CHUCHO_GTEST_GENERATOR}")
ADD_LIBRARY(gtest STATIC IMPORTED)
IF(CHUCHO_WINDOWS)
    SET_TARGET_PROPERTIES(gtest PROPERTIES
                          IMPORTED_LOCATION "${CHUCHO_EXTERNAL_PREFIX}/lib/gtest.lib")
ELSE()
    SET_TARGET_PROPERTIES(gtest PROPERTIES
                          IMPORTED_LOCATION "${CHUCHO_EXTERNAL_PREFIX}/lib/libgtest.a")
ENDIF()
ADD_DEPENDENCIES(gtest gtest-external)
SET_TARGET_PROPERTIES(gtest-external PROPERTIES
                      EXCLUDE_FROM_ALL TRUE)
ADD_DEPENDENCIES(external gtest-external)

# This macro is used to download the possbile compression libraries
# specified by the _URL settings.
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
