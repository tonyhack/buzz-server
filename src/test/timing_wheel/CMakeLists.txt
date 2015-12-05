
set(TIMING_WHEEL_TEST_SOURCES
    timing_wheel_test.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/timing_wheel_test)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/timing_wheel_test)

add_executable(timing_wheel_test ${TIMING_WHEEL_TEST_SOURCES} ${META_SOURCE})
target_link_libraries(timing_wheel_test ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift)

