#
# Copyright 2013-2019 Will Mason
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

# These tuples are (YAML, config, log4cplus, JSON)
SET(TESTS
    ON ON ON ON
    ON ON ON OFF
    ON ON OFF ON
    ON ON OFF OFF
    ON OFF ON ON
    ON OFF ON OFF
    ON OFF OFF ON
    ON OFF OFF OFF
    OFF ON ON ON
    OFF ON ON OFF
    OFF ON OFF ON
    OFF ON OFF OFF
    OFF OFF ON ON
    OFF OFF ON OFF
    OFF OFF OFF ON
    OFF OFF OFF OFF
    END)
IF(WIN32)
    SET(GENERATOR "NMake Makefiles")
    FIND_PROGRAM(CHUCHO_BUILD_TOOL nmake)
ELSE()
    SET(GENERATOR "Unix Makefiles")
    FIND_PROGRAM(CHUCHO_BUILD_TOOL make)
ENDIF()
IF(NOT CHUCHO_BUILD_TOOL)
    MESSAGE(FATAL_ERROR "Could not find the make tool")
ENDIF()
SET(IDX 0)
LIST(GET TESTS ${IDX} YAML_VAL)
WHILE(NOT YAML_VAL STREQUAL END)
    IF(EXISTS check_config_build/src)
        FILE(RENAME check_config_build/src check_config_build_src)
    ENDIF()
    FILE(REMOVE_RECURSE check_config_build)
    FILE(MAKE_DIRECTORY check_config_build)
    IF(EXISTS check_config_build_src)
        FILE(RENAME check_config_build_src check_config_build/src)
    ENDIF()
    MATH(EXPR IDX "${IDX} + 1")
    LIST(GET TESTS ${IDX} CONFIG_VAL)
    MATH(EXPR IDX "${IDX} + 1")
    LIST(GET TESTS ${IDX} LOG4CPLUS_VAL)
    MATH(EXPR IDX "${IDX} + 1")
    LIST(GET TESTS ${IDX} JSON_VAL)
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${GENERATOR}" -DCLANG_LIBSTDCXX=${CLANG_LIBSTDCXX} -DYAML_CONFIG=${YAML_VAL} -DCONFIG_FILE_CONFIG=${CONFIG_VAL} -DLOG4CPLUS_CONFIG=${LOG4CPLUS_VAL} -DJSON_CONFIG=${JSON_VAL} "${CHUCHO_SOURCE_DIR}"
                    WORKING_DIRECTORY check_config_build
                    RESULT_VARIABLE CMD_RESULT)
    IF(NOT CMD_RESULT EQUAL 0)
        SET(ERR_MSG "CMake failed")
        BREAK()
    ENDIF()
    EXECUTE_PROCESS(COMMAND "${CHUCHO_BUILD_TOOL}" unit-test
                    WORKING_DIRECTORY check_config_build
                    RESULT_VARIABLE CMD_RESULT)
    IF(NOT CMD_RESULT EQUAL 0)
        SET(ERR_MSG "make unit-test failed")
        BREAK()
    ENDIF()
    EXECUTE_PROCESS(COMMAND test/unit-test --gtest_filter=-event_cache.*
                    WORKING_DIRECTORY check_config_build
                    RESULT_VARIABLE CMD_RESULT)
    IF(NOT CMD_RESULT EQUAL 0)
        SET(ERR_MSG "run unit-test failed")
        BREAK()
    ENDIF()
    MATH(EXPR IDX "${IDX} + 1")
    LIST(GET TESTS ${IDX} YAML_VAL)
ENDWHILE()
FILE(REMOVE_RECURSE check_config_build)
IF(DEFINED ERR_MSG)
    MESSAGE(FATAL_ERROR "[YAML=${YAML_VAL},JSON=${JSON_VAL},CONFIG=${CONFIG_VAL},LOG4CPLUS=${LOG4CPLUS_VAL}] ${ERR_MSG}")
ENDIF()
