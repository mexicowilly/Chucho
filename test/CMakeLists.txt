ADD_DEFINITIONS(-DGTEST_HAS_TR1_TUPLE=0 -Dchucho_EXPORTS)

ADD_EXECUTABLE(unit-test EXCLUDE_FROM_ALL
               diagnostic_context_test.cpp
               file_test.cpp
               file_writer_test.cpp
               harness.cpp
               level_test.cpp
               level_threshold_filter_test.cpp
               logger_test.cpp
               marker_test.cpp
               optional_test.cpp
               pattern_formatter_test.cpp
               rolling_file_writer_test.cpp
               size_file_roll_trigger_test.cpp)
TARGET_LINK_LIBRARIES(unit-test chucho ${CHUCHO_GTEST_LIB})
ADD_DEPENDENCIES(unit-test gtest-external)

ADD_CUSTOM_TARGET(check
                  COMMAND unit-test)
