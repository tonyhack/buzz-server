
set(SCHEDULE_SERVER_SOURCES
    schedule_server.cc
    schedule_terminal.cc
    main.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/schedule_server)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/schedule_server)

add_executable(schedule_server ${SCHEDULE_SERVER_SOURCES} ${GLOBAL_SOURCES} ${SCHEDULE_PROTOCOL_SOURCES})
target_link_libraries(schedule_server coresh_static pthread rt thrift)

add_dependencies(schedule_server run coresh_static)
