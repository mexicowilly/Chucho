#
# Copyright 2013-2016 Will Mason
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

#     _______   .---.  .---.   ___    _     _______   .---.  .---.     ,-----.     
#    /   __  \  |   |  |_ _| .'   |  | |   /   __  \  |   |  |_ _|   .'  .-,  '.   
#   | ,_/  \__) |   |  ( ' ) |   .'  | |  | ,_/  \__) |   |  ( ' )  / ,-.|  \ _ \  
# ,-./  )       |   '-(_{;}_).'  '_  | |,-./  )       |   '-(_{;}_);  \  '_ /  | : 
# \  '_ '`)     |      (_,_) '   ( \.-.|\  '_ '`)     |      (_,_) |  _`,/ \ _/  | 
#  > (_)  )  __ | _ _--.   | ' (`. _` /| > (_)  )  __ | _ _--.   | : (  '\_/ \   ; 
# (  .  .-'_/  )|( ' ) |   | | (_ (_) _)(  .  .-'_/  )|( ' ) |   |  \ `"/  \  ) /  
#  `-'`-'     / (_{;}_)|   |  \ /  . \ / `-'`-'     / (_{;}_)|   |   '. \_/``".'   
#    `._____.'  '(_,_) '---'   ``-'`-''    `._____.'  '(_,_) '---'     '-----'     
#                                                                                  

# OVERVIEW
# ================================================================================
#     Herein you will find all the build options for Chucho. Just edit the options
# you wish to change and then run CMake. The textual variables that affect the
# build are commented out. To change one to something operable, just uncoment the
# line by removing the initial '#' and then edit the second parameter, or field,
# of the SET command to make it the value you wish to use. The boolean variables
# are expressed as OPTION commands with the default value set. Just change the
# last parameter of the OPTION command to the value you wish. Valid values for
# OPTION are ON and OFF.
#
#     Sections include:
#         * GENERAL
#         * CONFIGURATION FORMATS
#         * EMAIL WRITER
#         * DATABASE
#         * RUBY EVALUATOR FILTER
#         * MESSAGE QUEUE
#         * COMPRESSION

# GENERAL
# ================================================================================

# The installation location. This is the top-level installation directory.
# Components of Chucho will be placed into subdirectories, such as include, lib,
# bin, etc.... This is a CMake variable that has a default value of /usr/local on
# POSIX systems and C:\Program Files (x86)\chucho on Windows. On Macintosh, if the
# option ENABLE_FRAMEWORK is ON and ENABLE_SHARED is ON, then this variable is
# ignored.

# SET(CMAKE_INSTALL_PREFIX "<My Installation Directory>" CACHE STRING "The installation directory" FORCE)

# Whether to build a shared object (known as DLL on Windows).

OPTION(ENABLE_SHARED "Whether to build a shared object" OFF)

# Whether to build Chucho as a framework on Macintosh. This variable only takes
# effect if ENABLE_SHARED is ON. If a framework is being built, then it will be
# installed to /Library/Frameworks, and the variable CMAKE_INSTALL_PREFIX is
# ignored.

OPTION(ENABLE_FRAMEWORK "Whether to build as a framework on Macintosh" ON)

# Whether to install the Chucho server, chuchod, as a service on systems that
# support system services. The operating systems on which the build system knows
# how to install a service include Windows, Linux, Macintosh and FreeBSD. The
# service installation may succeed on other systems, but they are untested.

OPTION(INSTALL_SERVICE "Whether to install chuchod as a system service" OFF)

# Whether to build the C API. Chucho supports a C99 interface if your program is
# written in C or you are using a compiler that cannot understand C++11. This is
# normally disabled, as it will increase the size of the Chucho library
# significantly.

OPTION(C_API "Whether the C API should be built into this Chucho" OFF)

# CONFIGURATION FORMATS
# ================================================================================
# Chucho is able to parse configuration files using three different formats, YAML,
# config file and Log4cplus. Any or all may be included in the build, and Chucho
# will figure out the format of the file being used at run time. For examples
# showing how each of these formats works, please refer to the files:
#     test/yaml_configurator_test.cpp
#     test/chucho_config_file_configurator_test.cpp
#     test/log4cplus_config_file_configurator_test.cpp

# YAML configuration.

OPTION(YAML_CONFIG "Whether to include the YAML configuration parser" ON)

# Config file format using Chucho keys.

OPTION(CONFIG_FILE_CONFIG "Whether to include the config file configuration parser that uses Chucho keys" OFF)

# Config file format using Log4cplus keys.

OPTION(LOG4CPLUS_CONFIG "Whether to support reading log4cplus configuration files" OFF)

# EMAIL WRITER
# ================================================================================

# Whether to search for the curl library. Libcurl is used to support the Chucho
# email_writer. You may explicitly disable this search if you wish.

OPTION(ENABLE_CURL "Whether libcurl should be checked so that email_writer will be enabled" ON)

# DATABASE
# ================================================================================
#     Chucho supports sending log events to databases. You must have the database
# client library available on your system at build time in order for database
# support to be included. Please refer to the files in the sql directory for exact
# descriptions of how the databases will be utilized.

# The location of the Oracle OCI header files.

# SET(ORACLE_INCLUDE_DIR "<My Oracle Include Directory>" CACHE STRING "Include directory of the Oracle client")

# The name of the Oracle OCI library. This variable need not be set for the Chucho
# library to build successfully with Oracle support. In the context of the Chucho
# build, this variable is only used for unit test linkage. However, you must link
# your own application to the OCI library when linking to Chucho.

# SET(ORACLE_CLIENT_LIB "<My Oracle Library>" CACHE STRING "Oracle client library")

# The location of the MySQL header files.

# SET(MYSQL_INCLUDE_DIR "<My MySQL Include Directory>" CACHE STRING "Include directory of the MySQL client")

# The name of the MySQL library. This variable need not be set for the Chucho
# library to build successfully with MySQL support. In the context of the Chucho
# build, this variable is only used for unit test linkage. However, you must link
# your own application to the MySQL library when linking to Chucho.

# SET(MYSQL_CLIENT_LIB "<My MySQL Library>" CACHE STRING "MySQL client library")

# The location of the SQLite header files.

# SET(SQLITE_INCLUDE_DIR "<My SQLite Include Directory>" CACHE STRING "Include directory of the SQLite client")

# The name of the SQLite library. This variable need not be set for the Chucho
# library to build successfully with SQLite support. In the context of the Chucho
# build, this variable is only used for unit test linkage. However, you must link
# your own application to the SQLite library when linking to Chucho.

# SET(SQLITE_CLIENT_LIB "<My SQLite Library>" CACHE STRING "SQLite client library")

# The location of the PostgresSQL header files.

# SET(POSTGRES_INCLUDE_DIR "<My PostgresSQL Include Directory>" CACHE STRING "Include directory of the PostgresSQL client")

# The name of the PostgresSQL library. This variable need not be set for the Chucho
# library to build successfully with PostgresSQL support. In the context of the
# Chucho build, this variable is only used for unit test linkage. However, you must
# link your own application to the PostgresSQL library when linking to Chucho.

# SET(POSTGRES_CLIENT_LIB "<My PostgresSQL Library>" CACHE STRING "PostgresSQL client library")

# RUBY EVALUATOR FILTER
# ================================================================================
#     Chucho has a filter that can evaluate any Ruby expression to determine the
# filtration disposition. Chucho must have access to the Ruby library when Chucho
# is built for this filter to be available.

# The location of the Ruby header files.

# SET(RUBY_INCLUDE_DIR "<My Ruby Include Directory>" CACHE STRING "Include directory of the Ruby client")

# The name of the Ruby library.

# SET(RUBY_LIB "<My Ruby Library>" CACHE STRING "Ruby library")

# Whether to use the system's Ruby framework on Macintosh. If you set this option
# to ON, then the RUBY_INCLUDE_DIR and RUBY_LIB variables are ignored.

OPTION(RUBY_FRAMEWORK "Whether Ruby should be included as a framework (Macintosh only)" OFF)

# MESSAGE QUEUE
# ================================================================================
#     Chucho can write log events to message queueus. The message queue client
# library must be available to Chucho at build time. Additionally, message queues
# tend to treat message as blobs. Chucho must therefore have a way to blobify a
# log event using a serializer, which is an abstract class. There is currently a
# simple formatted_message_serializer that just puts the event's formatted
# message into a blob. The possibility of more advanced serialization exists with
# the protobuf_serializer. In order to make use of this, please enable Protobuf
# support using the settings below. Please refer to the file protobuf/chucho.proto
# for precise information about how Protobuf serialization of Chucho events is
# accomplished.

# The location of the ZeroMQ header files.

# SET(ZEROMQ_INCLUDE_DIR "<My ZeroMQ Include Directory>" CACHE STRING "Include directory of the ZeroMQ library")

# The name of the ZeroMQ library.

# SET(ZEROMQ_LIB "<My ZeroMQ Library>" CACHE STRING "ZeroMQ library")

# The location of the Protobuf header files.

# SET(PROTOBUF_INCLUDE_DIR "<My Protobuf Include Directory>" CACHE STRING "Include directory of the Protobuf library")

# The name of the Protobuf library.

# SET(PROTOBUF_LIB "<My Protobuf Library>" CACHE STRING "Protobuf library")

# The location of the Protobuf compiler.

# SET(PROTOC_DIR "<Location of Protoc>" CACHE STRING "Protoc location")

# COMPRESSION
# ================================================================================
#     When rolling files, Chucho may compress them to gzip, bzip2 or zip file
# formats. With gzip and bzip2, the compression may be utilized by Chucho in one
# of two ways: embedded in the Chucho library or linked to externally by your
# application. With minizip, the library used for zip file format, the library may
# not be linked externally, only embedded into the Chucho library itself.
#
#     There are three ways that zlib (for gzip format), bzip2 (for bzip2 format)
# or minizip (for zip file format) can be embedded in the Chucho library:
#
#     * Specifying an URL from which to download the compression tarball
#     * Specifying a location in the file system where the tarball resides
#     * Specifying the location of an already unpacked tarball
#
#     There is one way that zlib or bzip2 can be linked to externally:
#
#     * Specifying the directory where the header files reside and the directory
#       where the compression library resides
#
#     Note that minizip cannot be linked externally by your application because it
# doesn't even have a build system, and thus no consistent way to deploy it on its
# own. Also note that in order to use minizip for zip file format compression, you
# must also include zlib support.

# Whether to disable zlib. If this option is turned OFF and no other zlib
# variables are used, then the system will be searched for zlib, which, if it is
# found, will result in zlib being enabled with external linkage.

OPTION(DISABLE_ZLIB "Whether zlib compression should be disabled" ON)

# The URL from which to download zlib, resulting in it being embedded in the
# Chucho library. If this is set, then no other zlib variables should also be set.

# SET(ZLIB_URL "<My Zlib URL>" CACHE STRING "URL for downloading zlib")

# The location on the file system of the zlib tarball. This will cause zlib to be
# embedded in the Chucho library. If this is set,then no other zlib variables
# should also be set.

# SET(ZLIB_PACKAGE "<My Zlib Package>" CACHE STRING "Zlib tarball")

# The location on the file system of an unpacked zlib tarball. This will cause
# zlib to be embedded in the Chucho library. If this is set, then no other zlib
# variables should also be set.

# SET(ZLIB_SOURCE "<My Zlib Source>" CACHE STRING "Zlib source")

# The location of the directory that contains the zlib headers. This will cause
# zlib to require external linkage to your application later. If this is set,
# then ZLIB_LIB_DIR must also be set and no other zlib variables should also be
# set.

# SET(ZLIB_INCLUDE_DIR "<My Zlib Include Directory>" CACHE STRING "Include directory of the zlib library")

# The directory containing the zlib library. This will cause zlib to require
# external linkage to your application later. If this is set, then
# ZLIB_INCLUDE_DIR must also be set and no other zlib variables should also be
# set.

# SET(ZLIB_LIB_DIR "<My Zlib Library Directory>" CACHE STRING "Zlib library directory")

# Whether to disable bzip2. If this option is turned OFF and no other bzip2
# variables are used, then the system will be searched for bzip2, which, if it is
# found, will result in bzip2 being enabled with external linkage.

OPTION(DISABLE_BZIP2 "Whether bzip2 compression should be disabled" ON)

# The URL from which to download bzip2, resulting in it being embedded in the
# Chucho library. If this is set, then no other bzip2 variables should also be set.

# SET(BZIP2_URL "<My Bzip2 URL>" CACHE STRING "URL for downloading bzip2")

# The location on the file system of the bzip2 tarball. This will cause bzip2 to be
# embedded in the Chucho library. If this is set,then no other bzip2 variables
# should also be set.

# SET(BZIP2_PACKAGE "<My Bzip2 Package>" CACHE STRING "bzip2 tarball")

# The location on the file system of an unpacked bzip2 tarball. This will cause
# bzip2 to be embedded in the Chucho library. If this is set, then no other bzip2
# variables should also be set.

# SET(BZIP2_SOURCE "<My Bzip2 Source>" CACHE STRING "Bzip2 source")

# The location of the directory that contains the bzip2 headers. This will cause
# bzip2 to require external linkage to your application later. If this is set,
# then BZIP2_LIB_DIR must also be set and no other bzip2 variables should also be
# set.

# SET(BZIP2_INCLUDE_DIR "<My Bzip2 Include Directory>" CACHE STRING "Include directory of the bzip2 library")

# The directory containing the bzip2 library. This will cause bzip2 to require
# external linkage to your application later. If this is set, then
# BZIP2_INCLUDE_DIR must also be set and no other bzip2 variables should also be
# set.

# SET(BZIP2_LIB_DIR "<My Bzip2 Library Directory>" CACHE STRING "bzip2 library directory")

# The URL from which to download minizip, resulting in it being embedded in the
# Chucho library. If this is set, then no other minizip variables should also be set.

# SET(MINIZIP_URL "<My Minizip URL>" CACHE STRING "URL for downloading minizip")

# The location on the file system of the minizip tarball. This will cause minizip to be
# embedded in the Chucho library. If this is set,then no other minizip variables
# should also be set.

# SET(MINIZIP_PACKAGE "<My Minizip Package>" CACHE STRING "minizip tarball")

# The location on the file system of an unpacked minizip tarball. This will cause
# minizip to be embedded in the Chucho library. If this is set, then no other minizip
# variables should also be set.

# SET(MINIZIP_SOURCE "<My Minizip Source>" CACHE STRING "Minizip source")
