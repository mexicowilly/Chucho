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
#         * OPTIONAL FEATURES
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

# Whether to build the log server, chuchod. This is only necessary if you are planning
# on installing the server to this host. If you only need to use remote_writer to
# forward event to another host, then there is no need to build chuchod.

OPTION(CHUCHOD "Whether the log server, chuchod, should be built into this Chucho" OFF)

# Whether to use libstdc++ when compiling with Clang.
OPTION(CLANG_LIBSTDCXX "Whether to use libstdc++ with the Clang compiler" OFF)

# CONFIGURATION FORMATS
# ================================================================================
#     Chucho is able to parse configuration files using three different formats,
# YAML, config file and Log4cplus. Any or all may be included in the build, and
# Chucho will figure out the format of the file being used at run time. For
# examples showing how each of these formats works, please refer to the files:
#     test/yaml_configurator_test.cpp
#     test/chucho_config_file_configurator_test.cpp
#     test/log4cplus_config_file_configurator_test.cpp

# YAML configuration.

OPTION(YAML_CONFIG "Whether to include the YAML configuration parser" ON)

# Config file format using Chucho keys.

OPTION(CONFIG_FILE_CONFIG "Whether to include the config file configuration parser that uses Chucho keys" OFF)

# Config file format using Log4cplus keys.

OPTION(LOG4CPLUS_CONFIG "Whether to support reading log4cplus configuration files" OFF)

# OPTIONAL FEATURES
# ================================================================================
#     Optional features can be enabled by linking to third-party libraries. If any
# of these libraries is found, corresponding elements of Chucho will be built. In
# order to help CMake find these features, you may need to set the variables
# CMAKE_INCLUDE_PATH, CMAKE_LIBRARY_PATH and CMAKE_PROGRAM_PATH.
#
# The feature names and what they provide are:
#
# activemq: activemq_writer
# bzip2: bzip2_compressor, bzip2_file_compressor
# capnproto: capn_proto_serializer
# curl: email_writer, email_trigger, level_threshold_email_trigger
# db2: db2_writer
# flatbuffers: flatbuffers_serializer
# libarchive: zip_file_compressor
# liblzma: lzma_compressor, lzma_file_compressor
# mysql: mysql_writer
# oracle: oracle_writer
# postgres: postgres_writer
# protobuf: protobuf_serializer
# rabbitmq: rabbitmq_writer
# ruby: ruby_evaluator_filter
# sqlite: sqlite_writer
# zeromq: zeromq_writer
# zlib: zlib_compressor, gzip_file_compressor
#

# SET(WITH_LIBS <Your libraries, semi-colon separated>)
