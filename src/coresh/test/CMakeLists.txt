
set(MESSAGE_QUEUE_TEST_SOURCES
    message_queue_test.cc)
set(COMMUNICATOR_TEST_SOURCES
    communicator_test.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/coresh/test)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/coresh/test)

add_executable(message_queue_test ${MESSAGE_QUEUE_TEST_SOURCES})
target_link_libraries(message_queue_test ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift)

add_executable(communicator_test ${COMMUNICATOR_TEST_SOURCES})
target_link_libraries(communicator_test ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift boost_signals)

