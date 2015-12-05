
set(CHAT_PRESSURE_CLIENT_SOURCES
    configure.cc
    multiplex_thrift.cc
    main.cc
    pressure_client.cc
    pressure_client_set.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/pressure_client)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/pressure_client)

add_executable(pressure_client ${CHAT_PRESSURE_CLIENT_SOURCES} ${META_SOURCE})
target_link_libraries(pressure_client ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift)

