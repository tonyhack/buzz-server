
set(DATABASE_SERVER_SOURCES
    async_request_loop.cc
    async_request_pool.cc
    async_request_service.cc
    async_request_thread.cc
    database_server.cc
    database_terminal.cc
    database_terminal_set.cc
    main.cc
    mysql_client.cc
    mysql_session.cc
    storage_achieve_login.cc
    storage_achieve_save.cc
    storage_actor_login.cc
    storage_actor_logout_center.cc
    storage_actor_online.cc
    storage_actor_save.cc
    storage_attr_actor_login.cc
    storage_attr_actor_save.cc
    storage_auction_actor_login.cc
    storage_auction_actor_save.cc
    storage_auction_transaction_del.cc
    storage_auction_transaction_save.cc
    storage_buff_login.cc
    storage_buff_save.cc
    storage_check_session.cc
    storage_cooling_login.cc
    storage_cooling_save.cc
    storage_gem_login.cc
    storage_gem_save.cc
    storage_guild_actor_login.cc
    storage_guild_actor_save.cc
    storage_guild_del.cc
    storage_guild_save.cc
    storage_item_actor_login.cc
    storage_item_actor_save.cc
    storage_item_load.cc
    storage_item_save.cc
    storage_mail_add.cc
    storage_mail_affix_state.cc
    storage_mail_del.cc
    storage_mail_login.cc
    storage_mail_pick_affix.cc
    storage_mail_read.cc
    storage_playing_actor_login.cc
    storage_playing_actor_save.cc
    storage_skill_login.cc
    storage_skill_save.cc
    storage_social_actor_login.cc
    storage_social_actor_save.cc
    storage_soul_actor_load.cc
    storage_soul_actor_save.cc
    storage_soul_load.cc
    storage_soul_save.cc
    storage_spirit_facade_login.cc
    storage_spirit_facade_save.cc
    storage_task_actor_load.cc
    storage_task_actor_save.cc
    storage_task_load.cc
    storage_task_save.cc
    storage_test_echo.cc
    storage_welfare_load.cc
    storage_welfare_save.cc
    storage_world_data_save.cc)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/database_server)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/database_server)

add_executable(database_server ${DATABASE_SERVER_SOURCES}
${SCHEDULE_PROTOCOL_SOURCES} ${GLOBAL_SOURCES} ${ENTITY_SOURCES} ${DATABASE_PROTOCOL_SOURCES}
${SCHEDULE_CLIENT_SOURCES})
target_link_libraries(database_server coresh_static pthread thrift mysqlpp rt)

add_dependencies(database_server run coresh_static)
