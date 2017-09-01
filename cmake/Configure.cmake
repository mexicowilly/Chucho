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
INCLUDE(CheckCXXSourceRuns)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(ExternalProject)
INCLUDE(CheckTypeSize)
INCLUDE(CheckStructHasMember)
INCLUDE(ChuchoFindProgram)
INCLUDE(ChuchoFindPackage)

# framework or not
SET(CHUCHO_NEEDS_TO_USE_THE_FRAMEWORK_VARIABLE_OR_CMAKE_COMPLAINS ${ENABLE_FRAMEWORK})

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

# Now do platform checks
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

    # Doors
    IF(CHUCHO_SOLARIS)
        CHECK_INCLUDE_FILE_CXX(door.h CHUCHO_HAVE_DOOR_H)
        IF(CHUCHO_HAVE_DOOR_H)
            CHUCHO_REQUIRE_SYMBOLS(door.h door_call door_getparam door_create door_return)
            CHUCHO_REQUIRE_SYMBOLS(stropts.h fattach)
            SET(CHUCHO_HAVE_DOORS TRUE CACHE INTERNAL "Whether we have doors")
        ENDIF()
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

CHUCHO_FIND_PACKAGE(CURL)
CHUCHO_FIND_PACKAGE(Ruby)
CHUCHO_FIND_PACKAGE(Protobuf)
# Do not use CHUCHO_FIND_PACKAGE for CapnProto
FIND_PACKAGE(CapnProto)
CHUCHO_FIND_PACKAGE(ZLIB)
CHUCHO_FIND_PACKAGE(BZip2)
CHUCHO_FIND_PACKAGE(SQLITE INCLUDE sqlite3.h LIBS sqlite3 PKG_CONFIG_NAME sqlite3 SYMBOLS
    sqlite3_threadsafe sqlite3_open sqlite3_prepare_v2 sqlite3_reset sqlite3_bind_text
    sqlite3_bind_int64 sqlite3_bind_int sqlite3_step sqlite3_finalize sqlite3_close
    sqlite3_extended_result_codes sqlite3_extended_errcode sqlite3_errmsg)
CHUCHO_FIND_PACKAGE(ORACLE INCLUDE oci.h LIBS occi clntsh SYMBOLS
    OCIEnvCreate OCIHandleAlloc OCILogon2 OCIStmtPrepare OCIBindByName
    OCIStmtExecute OCIHandleFree OCIErrorGet OCIDescriptorAlloc OCIDescriptorFree)
CHUCHO_FIND_PACKAGE(MYSQL INCLUDE mysql.h LIBS mysqlclient PKG_CONFIG_NAME mysqlclient SYMBOLS
    mysql_init mysql_real_connect mysql_stmt_init mysql_stmt_prepare mysql_autocommit
    mysql_stmt_close mysql_close mysql_stmt_bind_param mysql_stmt_execute)
CHUCHO_FIND_PACKAGE(POSTGRES INCLUDE libpq-fe.h LIBS pq PKG_CONFIG_NAME libpq SYMBOLS
    PQconnectdb PQprepare PQexecPrepared PQresultStatus PQresultErrorMessage
    PQclear PQfinish PQstatus PQerrorMessage)
CHUCHO_FIND_PACKAGE(DB2 INCLUDE sqlcli1.h LIBS db2 SYMBOLS
    SQLAllocHandle SQLFreeHandle SQLSetEnvAttr SQLConnect SQLBindParameter
    SQLPrepare SQLExecute SQLDisconnect SQLGetDiagRec)
CHUCHO_FIND_PACKAGE(ZEROMQ INCLUDE zmq.h LIBS zmq PKG_CONFIG_NAME libzmq SYMBOLS
    zmq_ctx_new zmq_ctx_destroy zmq_socket zmq_close zmq_connect zmq_msg_send
    zmq_msg_init_size zmq_msg_data zmq_strerror zmq_msg_close zmq_bind)
CHUCHO_FIND_PACKAGE(ACTIVEMQ INCLUDE cms/ConnectionFactory.h LIBS activemq-cpp apr-1)
CHUCHO_FIND_PACKAGE(RABBITMQ INCLUDE amqp.h LIBS rabbitmq PKG_CONFIG_NAME librabbitmq SYMBOLS
    amqp_new_connection amqp_socket_open amqp_login amqp_get_rpc_reply
    amqp_channel_close amqp_connection_close amqp_basic_publish amqp_channel_open
    amqp_destroy_connection amqp_error_string2 amqp_cstring_bytes amqp_queue_declare
    amqp_basic_consume amqp_consume_message amqp_empty_bytes amqp_empty_table
    amqp_destroy_envelope)

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