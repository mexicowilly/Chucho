INCLUDE_DIRECTORIES("${CHUCHO_EXTERNAL_PREFIX}/include")
LINK_DIRECTORIES("${CHUCHO_EXTERNAL_PREFIX}/lib")
ADD_DEFINITIONS(-DGTEST_HAS_TR1_TUPLE=0)

ADD_EXECUTABLE(unit-test EXCLUDE_FROM_ALL
               level_test.cpp
               logger_test.cpp
               pattern_formatter_test.cpp)
TARGET_LINK_LIBRARIES(unit-test chucho ${CHUCHO_GTEST_MAIN_LIB} ${CHUCHO_GTEST_LIB})
ADD_DEPENDENCIES(unit-test gtest-external)

ADD_CUSTOM_TARGET(check
                  COMMAND unit-test)
