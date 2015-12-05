
set(CHAT_CLIENT_SOURCES
    configure.cc
    main.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/chat_client)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/chat_client)

add_executable(chat_client ${CHAT_CLIENT_SOURCES} ${META_SOURCE})
target_link_libraries(chat_client ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift)

