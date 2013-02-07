INCLUDE(CheckCXXCompilerFlag)
INCLUDE(CheckCXXSymbolExists)
INCLUDE(ExternalProject)

# static and shared
OPTION(ENABLE_SHARED "Whether to build a shared object" FALSE)
OPTION(ENABLE_STATIC "Whether to build a static library" TRUE)

IF(NOT ENABLE_SHARED AND NOT ENABLE_STATIC)
    MESSAGE(FATAL_ERROR "Either ENABLE_SHARED or ENABLE_STATIC must be TRUE")
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
    IF(NOT CHUCHO_CXX11_FLAG)
        MESSAGE(FATAL_ERROR "C++11 compatibility is required")
    ENDIF()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden CHUCHO_VIS_FLAG)
    IF(CHUCHO_VIS_FLAG)
        SET(CHUCHO_CXX_SO_FLAGS -fvisibility=hidden)
    ENDIF()
	IF(CMAKE_GENERATOR STREQUAL Xcode)
		SET(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -stdlib=libc++")
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
ELSE(CHUCHO_WINDOWS)
    ExternalProject_Add_Step(gtest-external
                             install-libs
                             COMMAND "${CMAKE_COMMAND}" -E make_directory <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/libgtest.a <INSTALL_DIR>/lib
                             COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/libgtest_main.a <INSTALL_DIR>/lib
                             DEPENDEES install-headers)
    ADD_LIBRARY(gtest STATIC IMPORTED)
    SET_TARGET_PROPERTIES(gtest
                          PROPERTIES
                          IMPORTED_LOCATION "${CHUCHO_EXTERNAL_PREFIX}/lib/libgtest.a")
ENDIF()
ADD_DEPENDENCIES(external gtest-external)

# yaml-cpp
IF(CHUCHO_WINDOWS)
    SET(CHUCHO_YAML_CPP_GENERATOR "NMake Makefiles")
ELSE()
    SET(CHUCHO_YAML_CPP_GENERATOR "Unix Makefiles")
ENDIF()

ExternalProject_Add(yaml-cpp-external
                    URL http://yaml-cpp.googlecode.com/files/yaml-cpp-0.3.0.tar.gz
                    URL_MD5 9aa519205a543f9372bf4179071c8ac6
                    CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}" -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
					CMAKE_GENERATOR ${CHUCHO_YAML_CPP_GENERATOR})
ADD_LIBRARY(yaml-cpp STATIC IMPORTED)
SET_TARGET_PROPERTIES(yaml-cpp
                      PROPERTIES
                      IMPORTED_LOCATION "${CHUCHO_EXTERNAL_PREFIX}/lib/libyaml-cpp.a")
ADD_DEPENDENCIES(external yaml-cpp-external)

# utf8-cpp
ExternalProject_Add(utf8-cpp-external
                    URL http://sourceforge.net/projects/utfcpp/files/utf8cpp_2x/Release%202.3.2/utf8_v2_3_2.zip/download
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")
ExternalProject_Add_Step(utf8-cpp-external
                         install-headers
                         COMMAND "${CMAKE_COMMAND}" -E make_directory <INSTALL_DIR>/include
                         COMMAND "${CMAKE_COMMAND}" -E copy_directory <SOURCE_DIR>/source <INSTALL_DIR>/include
                         DEPENDEES install)
ADD_DEPENDENCIES(external utf8-cpp-external)
