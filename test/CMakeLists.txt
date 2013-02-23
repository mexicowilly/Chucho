ADD_DEFINITIONS(-DGTEST_HAS_TR1_TUPLE=0 -Dchucho_EXPORTS)

ADD_EXECUTABLE(unit-test EXCLUDE_FROM_ALL
               diagnostic_context_test.cpp
               file_test.cpp
               file_writer_test.cpp
               harness.cpp
               level_test.cpp
               level_filter_test.cpp
               level_threshold_filter_test.cpp
               logger_test.cpp
               log_macro_test.cpp
               marker_test.cpp
               optional_test.cpp
               pattern_formatter_test.cpp
               regex_test.cpp
               rolling_file_writer_test.cpp
               size_file_roll_trigger_test.cpp
               yaml_configurator_test.cpp)
TARGET_LINK_LIBRARIES(unit-test chucho yaml-cpp gtest ${CHUCHO_THREAD_LIB})
ADD_DEPENDENCIES(unit-test gtest-external)

IF(ENABLE_SHARED)
    ADD_EXECUTABLE(whereami whereami.cpp)
    GET_TARGET_PROPERTY(CHUCHO_YAML_CPP_LOCATION yaml-cpp IMPORTED_LOCATION)
    GET_FILENAME_COMPONENT(CHUCHO_YAML_CPP_DIR "${CHUCHO_YAML_CPP_LOCATION}" PATH)
    ADD_CUSTOM_TARGET(check
                      COMMAND whereami --file "${CMAKE_CURRENT_BINARY_DIR}/unit-test-location"
                      COMMAND "${CMAKE_COMMAND}" -DCHUCHO_LD_ENV_VAR=${CHUCHO_LD_ENV_VAR} "-DCHUCHO_LD_ENV_VALUE=${CHUCHO_YAML_CPP_DIR}" "-DCHUCHO_UNIT_TEST_LOCATION=${CMAKE_CURRENT_BINARY_DIR}/unit-test-location" -P "${CMAKE_CURRENT_SOURCE_DIR}/Check.cmake"
                      COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_CURRENT_BINARY_DIR}/unit-test-location")
ELSE()
    ADD_CUSTOM_TARGET(check
                      COMMAND unit-test)
ENDIF()

ADD_EXECUTABLE(rolling-file-test EXCLUDE_FROM_ALL
               harness.cpp
               time_rolling_file_test.cpp)
TARGET_LINK_LIBRARIES(rolling-file-test chucho yaml-cpp gtest)
ADD_DEPENDENCIES(unit-test gtest-external)
