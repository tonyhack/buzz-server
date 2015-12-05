
set(SCHEDULE_TEST_SOURCES
    application_server.cc
    main.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/schedule_test)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/schedule_test)

add_executable(schedule_server_test ${SCHEDULE_TEST_SOURCES} ${GLOBAL_SOURCES}
${SCHEDULE_PROTOCOL_SOURCES} ${SCHEDULE_CLIENT_SOURCES})
target_link_libraries(schedule_server_test ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift)

