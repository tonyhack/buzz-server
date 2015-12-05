
set(CHAT_SERVER_SOURCES
    chat_server.cc
    configure.cc
    main.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/chat_server)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/chat_server)

add_executable(chat_server ${CHAT_SERVER_SOURCES} ${META_SOURCE})
target_link_libraries(chat_server ${PROJECT_BINARY_DIR}/lib/libcoresh.a pthread rt thrift)

