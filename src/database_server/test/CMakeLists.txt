
set(DATABASE_TEST_SOURCES
    application_server.cc
    main.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/database_test)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/database_test)

add_executable(database_server_test ${DATABASE_TEST_SOURCES} ${GLOBAL_SOURCES}
    ${DATABASE_PROTOCOL_SOURCES} ${DATABASE_CLIENT_SOURCE}
    ${SCHEDULE_CLIENT_SOURCES} ${SCHEDULE_PROTOCOL_SOURCES})
target_link_libraries(database_server_test ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift)

