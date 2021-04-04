#
# Copyright 2013-2021 Will Mason
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

IF(EXISTS "${FILE_NAME}")
    SET(OUT_NAME "${FILE_NAME}.mod")
    FILE(REMOVE "${OUT_NAME}")
    FILE(STRINGS "${FILE_NAME}" LINES)
    FOREACH(LINE ${LINES})
        IF(LINE MATCHES "-WX")
            STRING(REPLACE "-WX" "/wd4996" LINE_MOD ${LINE})
        ELSE()
            SET(LINE_MOD ${LINE})
        ENDIF()
        FILE(APPEND "${OUT_NAME}" "${LINE_MOD}\n")
    ENDFOREACH()
    FILE(RENAME "${OUT_NAME}" "${FILE_NAME}")
ENDIF()
