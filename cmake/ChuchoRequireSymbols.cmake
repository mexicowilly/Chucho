#
# Copyright 2013-2018 Will Mason
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

INCLUDE(CheckCXXSymbolExists)
INCLUDE(CheckSymbolExists)

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
