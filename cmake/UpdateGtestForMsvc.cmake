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
