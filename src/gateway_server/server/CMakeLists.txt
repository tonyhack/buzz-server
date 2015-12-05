
set(GATEWAY_SERVER_SOURCES
    check_session_queue.cc
    game_message_handler.cc
    gateway_server.cc
    gateway_user.cc
    gateway_user_pool.cc
    main.cc
    session_message_handler.cc
    status_check_session_handler.cc
    status_login_handler.cc
    status_logout_handler.cc
    storage_response_handler.cc
    status_running_handler.cc
    status_verify_question_handler.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/gateway_server)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/gateway_server)

add_executable(gateway_server ${GLOBAL_SOURCES}
${GATEWAY_SERVER_SOURCES} ${GATEWAY_PROTOCOL_SOURCES} ${ENTITY_SOURCES}
${DATABASE_CLIENT_SOURCE} ${DATABASE_PROTOCOL_SOURCES}
${SCHEDULE_CLIENT_SOURCES} ${SCHEDULE_PROTOCOL_SOURCES}
${GAME_CLIENT_SOURCES} ${GAME_PROTOCOL_SOURCES}
${SESSION_CLIENT_SOURCES} ${SESSION_PROTOCOL_SOURCES})

target_link_libraries(gateway_server coresh_static pthread rt thrift)

add_dependencies(gateway_server run coresh_static)
