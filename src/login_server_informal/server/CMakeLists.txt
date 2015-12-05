
set(LOGIN_SERVER_SOURCES
    actor_login_handler.cc
    check_actor_exist_handler.cc
    create_actor_handler.cc
    http_response.cc
    http_service_handler.cc
    login_server.cc
    main.cc
    message_thread_pool.cc
    mysql_client.cc
    mysql_session.cc
    server_configure.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/login_server)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/login_server)

add_executable(login_server ${GLOBAL_SOURCES}
${LOGIN_SERVER_SOURCES} ${LOGIN_PROTOCOL_SOURCES})

target_link_libraries(login_server coresh_static pthread rt thrift mysqlpp)

add_dependencies(login_server run coresh_static)
