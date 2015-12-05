
set(SIMPLE_ENCRYPT_TEST_SOURCES
    ../simple_encrypt.cc
    simple_encrypt_test.cc)

set(DIRECTION_CALCULATOR_TEST_SOURCES
    direction_calculator_test.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/global)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/global)

add_executable(simple_encrypt_test ${SIMPLE_ENCRYPT_TEST_SOURCES})
target_link_libraries(simple_encrypt_test)

add_executable(direction_calculator_test ${DIRECTION_CALCULATOR_TEST_SOURCES})
target_link_libraries(direction_calculator_test)

