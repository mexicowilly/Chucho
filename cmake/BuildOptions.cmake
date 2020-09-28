#
# Copyright 2013-2020 Will Mason
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

# Whether to use libstdc++ when compiling with Clang.
IF(CMAKE_SYSTEM_NAME STREQUAL Linux AND CMAKE_CXX_COMPILER_ID MATCHES Clang)
    OPTION(CLANG_LIBSTDCXX "Whether to use libstdc++ with the Clang compiler" ON)
ELSE()
    OPTION(CLANG_LIBSTDCXX "Whether to use libstdc++ with the Clang compiler" OFF)
ENDIF()

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
# awssdk: cloudwatch_writer
# bzip2: bzip2_compressor, bzip2_file_compressor
# capn_proto: capn_proto_serializer
# curl: email_writer, email_trigger, level_threshold_email_trigger, loggly_writer
# flatbuffers: flatbuffers_serializer
# libarchive: zip_file_compressor
# liblzma: lzma_compressor, lzma_file_compressor
# lz4: lz4_compressor
# protobuf: protobuf_serializer
# rabbitmq: rabbitmq_writer
# rdkafka: kafka_writer
# ruby: ruby_evaluator_filter
# soci: database_writer
# zeromq: zeromq_writer
# zlib: zlib_compressor, gzip_file_compressor
#

# SET(WITH_LIBS <Your libraries, semi-colon separated>)
