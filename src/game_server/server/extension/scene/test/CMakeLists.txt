
set(ROUTER_TEST_SOURCES
    router_test.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/game_server/test)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/game_server/test)

add_executable(router_test ${ROUTER_TEST_SOURCES} ${ENTITY_SOURCES})
target_link_libraries(router_test ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift) 


