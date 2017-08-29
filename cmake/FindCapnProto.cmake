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

CHUCHO_FIND_PACKAGE(CAPN_PROTO INCLUDE capnp/serialize.h LIBS capnp kj PKG_CONFIG_NAME capnp)
IF (CHUCHO_HAVE_CAPN_PROTO)
    CHUCHO_FIND_PROGRAM(CHUCHO_CAPNP capnp)
    IF(NOT CHUCHO_CAPNP)
        MESSAGE(FATAL_ERROR "capnp was not found")
    ENDIF()
    CHUCHO_FIND_PROGRAM(CHUCHO_CAPNP capnpc-c++)
    IF(NOT CHUCHO_CAPNP)
        MESSAGE(FATAL_ERROR "capnpc-c++ was not found")
    ENDIF()
ENDIF()
