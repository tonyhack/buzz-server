#add_subdirectory(event/meta)
#add_subdirectory(request/meta)

set(SESSION_SERVER_SOURCES
    actor_message_handler.cc
    brief_actor.cc
    day_pass.cc
    extension_load_event_handler.cc
    extension_manager.cc
    game_server_index.cc
    game_message_handler.cc
    gateway_message_handler.cc
    session_actor.cc
    session_actor_manager.cc
    session_actor_pool.cc
    session_server.cc
    session_terminal.cc
    storage_actor_login_response.cc
    storage_response.cc
    sync_database_operation_manager.cc
    transaction_lock.cc
    world_data_manager.cc
    main.cc
    admin/admin_message_handler.cc
    admin/admin_service.cc
    admin/admin_session.cc
    admin/admin_session_manager.cc
    admin/admin_session_pool.cc)

set(SESSION_SERVER_INCLUDES
    actor_message_handler.h
    brief_actor.h
    day_pass.h
    extension_load_event_handler.h
    extension_manager.h
    extension_types.h
    game_message_handler.h
    game_server_index.h
    gateway_message_handler.h
    global_time_tick.h
    load_progress.h
    session_actor.h
    session_actor_manager.h
    session_actor_pool.h
    session_server.h
    session_terminal.h
    storage_actor_login_response.h
    storage_response.h
    sync_database_operation_manager.h
    transaction_lock.h
    world_data_manager.h
    admin/admin_message_handler.h
    admin/admin_service.h
    admin/admin_session.h
    admin/admin_session_manager.h
    admin/admin_session_pool.h)


set(SESSION_EVENT_SOURCES
    event/session_actor_event_constants.cpp
    event/session_actor_event_types.cpp
    event/session_event_constants.cpp
    event/session_event_types.cpp
    event/session_guild_event_constants.cpp
    event/session_guild_event_types.cpp
    event/session_social_event_constants.cpp
    event/session_social_event_types.cpp
    event/session_system_event_constants.cpp
    event/session_system_event_types.cpp)

set(SESSION_EVENT_INCLUDES
    event/session_actor_event_constants.h
    event/session_actor_event_types.h
    event/session_event_constants.h
    event/session_event_types.h
    event/session_guild_event_constants.h
    event/session_guild_event_types.h
    event/session_social_event_constants.h
    event/session_social_event_types.h
    event/session_system_event_constants.h
    event/session_system_event_types.h)


set(SESSION_REQUEST_SOURCES
    request/session_guild_request_constants.cpp
    request/session_guild_request_types.cpp
    request/session_item_request_constants.cpp
    request/session_item_request_types.cpp
    request/session_playing_request_constants.cpp
    request/session_playing_request_types.cpp
    request/session_mail_request_types.cpp
    request/session_mail_request_constants.cpp
    request/session_request_constants.cpp
    request/session_request_types.cpp
    request/session_scene_request_constants.cpp
    request/session_scene_request_types.cpp
    request/session_social_request_constants.cpp
    request/session_social_request_types.cpp
    request/session_team_request_constants.cpp
    request/session_team_request_types.cpp)

set(SESSION_REQUEST_INCLUDES
    request/session_guild_request_constants.h
    request/session_guild_request_types.h
    request/session_item_request_constants.cpp
    request/session_item_request_types.cpp
    request/session_playing_request_constants.h
    request/session_playing_request_types.h
    request/session_mail_request_types.h
    request/session_mail_request_constants.h
    request/session_request_constants.h
    request/session_request_types.h
    request/session_scene_request_constants.h
    request/session_scene_request_types.h
    request/session_social_request_constants.h
    request/session_social_request_types.h
    request/session_team_request_constants.h
    request/session_team_request_types.h)

set(SESSION_EXTENSION_TEST_SOURCES
    extension/test/event_handler.cc
    extension/test/test_extension.cc)

set(SESSION_EXTENSION_TEST_INCLUDES
    extension/test/event_handler.h
    extension/test/test_extension.h)


set(SESSION_EXTENSION_GM_SOURCES
    extension/gm/game_gm_message_handler.cc
    extension/gm/gm_extension.cc)

set(SESSION_EXTENSION_GM_INCLUDES
    extension/gm/game_gm_message_handler.h
    extension/gm/gm_extension.h)


set(SESSION_EXTENSION_SCENE_SOURCES
    extension/scene/actor_login_event_handler.cc
    extension/scene/actor_logout_event_handler.cc
    extension/scene/game_message_handler.cc
    extension/scene/world_boss.cc
    extension/scene/request_scene_get_actors.cc
    extension/scene/request_get_map_scene_id.cc
    extension/scene/request_scene_get_game_server.cc
    extension/scene/request_scene_jump.cc
    extension/scene/scene.cc
    extension/scene/scene_extension.cc
    extension/scene/scene_manager.cc
    extension/scene/scene_message_handler.cc
    extension/scene/scene_pool.cc
    extension/scene/server_logout_event_handler.cc)

set(SESSION_EXTENSION_SCENE_INCLUDES
    extension/scene/actor_login_event_handler.h
    extension/scene/actor_logout_event_handler.h
    extension/scene/game_message_handler.h
    extension/scene/request_scene_get_actors.h
    extension/scene/world_boss.h
    extension/scene/request_get_map_scene_id.h
    extension/scene/request_scene_get_game_server.h
    extension/scene/request_scene_jump.h
    extension/scene/scene.h
    extension/scene/scene_extension.h
    extension/scene/scene_manager.h
    extension/scene/scene_message_handler.h
    extension/scene/scene_pool.h
    extension/scene/server_logout_event_handler.h)


set(SESSION_EXTENSION_CHAT_SOURCES
    extension/chat/chat_extension.cc
    extension/chat/facade_request.cc
    extension/chat/game_chat_message_handler.cc)

set(SESSION_EXTENSION_CHAT_INCLUDES
    extension/chat/chat_extension.h
    extension/chat/facade_request.h
    extension/chat/game_chat_message_handler.h)


set(SESSION_EXTENSION_SOCIAL_SOURCES
    extension/social/actor_change_vocation_event_handler.cc
    extension/social/actor_daily_clean_event_handler.cc
    extension/social/actor_functionality_enable_event_handler.cc
    extension/social/actor_jump_scene_event_handler.cc
    extension/social/actor_level_up_event_handler.cc
    extension/social/actor_login_event_handler.cc
    extension/social/actor_logout_event_handler.cc
    extension/social/actor_save_event_handler.cc
    extension/social/contacts_message_handler.cc
    extension/social/contacts_request_handler.cc
    extension/social/enemy_message_handler.cc
    extension/social/facade_request.cc
    extension/social/game_homeland_message_handler.cc
    extension/social/homeland_actor.cc
    extension/social/homeland_battle.cc
    extension/social/homeland_battle_manager.cc
    extension/social/homeland_battle_message_handler.cc
    extension/social/homeland_event.cc
    extension/social/homeland_invader.cc
    extension/social/homeland_message_handler.cc
    extension/social/homeland_support_soul.cc
    extension/social/homeland_supported_soul.cc
    extension/social/social_actor.cc
    extension/social/social_actor_manager.cc
    extension/social/social_actor_pool.cc
    extension/social/social_extension.cc
    extension/social/storage_social_actor_login_response.cc)

set(SESSION_EXTENSION_SOCIAL_INCLUDES
    extension/social/actor_change_vocation_event_handler.h
    extension/social/actor_daily_clean_event_handler.h
    extension/social/actor_functionality_enable_event_handler.h
    extension/social/actor_jump_scene_event_handler.h
    extension/social/actor_level_up_event_handler.h
    extension/social/actor_login_event_handler.h
    extension/social/actor_logout_event_handler.h
    extension/social/actor_save_event_handler.h
    extension/social/contacts_message_handler.h
    extension/social/contacts_request_handler.h
    extension/social/enemy_message_handler.h
    extension/social/facade_request.h
    extension/social/game_homeland_message_handler.h
    extension/social/homeland_actor.h
    extension/social/homeland_battle.h
    extension/social/homeland_battle_manager.h
    extension/social/homeland_battle_message_handler.h
    extension/social/homeland_event.h
    extension/social/homeland_invader.h
    extension/social/homeland_message_handler.h
    extension/social/homeland_support_soul.h
    extension/social/homeland_supported_soul.h
    extension/social/social_actor.h
    extension/social/social_actor_manager.h
    extension/social/social_actor_pool.h
    extension/social/social_extension.h
    extension/social/storage_social_actor_login_response.h)


set(SESSION_EXTENSION_TEAM_SOURCES
    extension/team/actor_jump_scene_event_handler.cc
    extension/team/actor_levelup_event_handler.cc
    extension/team/actor_login_event_handler.cc
    extension/team/actor_logout_event_handler.cc
    extension/team/configure.cc
    extension/team/game_message_handler.cc
    extension/team/load_team_configure.cc
    extension/team/team.cc
    extension/team/team_actor.cc
    extension/team/team_actor_manager.cc
    extension/team/team_actor_pool.cc
    extension/team/team_commands.cc
    extension/team/team_extension.cc
    extension/team/team_manager.cc
    extension/team/team_message_handler.cc
    extension/team/team_pool.cc
    extension/team/team_request_handler.cc)

set(SESSION_EXTENSION_TEAM_INCLUDES
    extension/team/actor_jump_scene_event_handler.h
    extension/team/actor_levelup_event_handler.h
    extension/team/actor_login_event_handler.h
    extension/team/actor_logout_event_handler.h
    extension/team/configure.h
    extension/team/game_message_handler.h
    extension/team/load_team_configure.h
    extension/team/team.h
    extension/team/team_actor.h
    extension/team/team_actor_manager.h
    extension/team/team_actor_pool.h
    extension/team/team_commands.h
    extension/team/team_configure.h
    extension/team/team_extension.h
    extension/team/team_manager.h
    extension/team/team_message_handler.h
    extension/team/team_pool.h
    extension/team/team_request_handler.h)


set(SESSION_EXTENSION_MAIL_SOURCES
    extension/mail/game_message_handler.cc
    extension/mail/mail_extension.cc
    extension/mail/request_send_mail_handler.cc
    extension/mail/storage_mail_response.cc)

set(SESSION_EXTENSION_MAIL_INCLUDES
    extension/mail/game_message_handler.h
    extension/mail/mail_extension.h
    extension/mail/request_send_mail_handler.h
    extension/mail/storage_mail_response.h)

set(SESSION_EXTENSION_GUILD_SOURCES
    extension/guild/actor_change_fight_score_event_handler.cc
    extension/guild/actor_functionality_enable_event_handler.cc
    extension/guild/actor_jump_scene_event_handler.cc
    extension/guild/actor_jump_login_event_handler.cc
    extension/guild/actor_level_up_event_handler.cc
    extension/guild/actor_login_event_handler.cc
    extension/guild/actor_logout_event_handler.cc
    extension/guild/actor_save_event_handler.cc
    extension/guild/actor_weekly_clean_event_handler.cc
    extension/guild/facade_request.cc
    extension/guild/game_guild_message_handler.cc
    extension/guild/guild.cc
    extension/guild/guild_actor.cc
    extension/guild/guild_actor_manager.cc
    extension/guild/guild_actor_pool.cc
    extension/guild/guild_daily_clean_event_handler.cc
    extension/guild/guild_extension.cc
    extension/guild/guild_event.cc
    extension/guild/guild_manager.cc
    extension/guild/guild_message_handler.cc
    extension/guild/guild_pool.cc
    extension/guild/guild_request_handler.cc
    extension/guild/guild_weekly_clean_event_handler.cc
    extension/guild/storage_guild_actor_login_response.cc)

set(SESSION_EXTENSION_GUILD_INCLUDES
    extension/guild/actor_change_fight_score_event_handler.h
    extension/guild/actor_functionality_enable_event_handler.h
    extension/guild/actor_jump_scene_event_handler.h
    extension/guild/actor_jump_login_event_handler.h
    extension/guild/actor_level_up_event_handler.h
    extension/guild/actor_login_event_handler.h
    extension/guild/actor_logout_event_handler.h
    extension/guild/actor_save_event_handler.h
    extension/guild/actor_weekly_clean_event_handler.h
    extension/guild/facade_request.h
    extension/guild/game_guild_message_handler.h
    extension/guild/guild.h
    extension/guild/guild_actor.h
    extension/guild/guild_actor_manager.h
    extension/guild/guild_actor_pool.h
    extension/guild/guild_daily_clean_event_handler.h
    extension/guild/guild_extension.h
    extension/guild/guild_event.h
    extension/guild/guild_manager.h
    extension/guild/guild_message_handler.h
    extension/guild/guild_pool.h
    extension/guild/guild_request_handler.h
    extension/guild/guild_weekly_clean_event_handler.h
    extension/guild/storage_guild_actor_login_response.h)


set(SESSION_EXTENSION_PLAYING_SOURCES
    extension/playing/actor_change_fight_score_event_handler.cc
    extension/playing/actor_jump_scene_event_handler.cc
    extension/playing/actor_levelup_event_handler.cc
    extension/playing/actor_login_event_handler.cc
    extension/playing/actor_logout_event_handler.cc
    extension/playing/game_maze_message_handler.cc
    extension/playing/game_message_handler.cc
    extension/playing/maze_map_manager.cc
    extension/playing/playing.cc
    extension/playing/playing_activity_controller.cc
    extension/playing/playing_actor.cc
    extension/playing/playing_actor_manager.cc
    extension/playing/playing_actor_pool.cc
    extension/playing/playing_extension.cc
    extension/playing/playing_load_balance.cc
    extension/playing/playing_manager.cc
    extension/playing/playing_message_handler.cc
    extension/playing/playing_page.cc
    extension/playing/playing_page_pool.cc
    extension/playing/playing_pool.cc
    extension/playing/playing_request_handler.cc
    extension/playing/playing_team.cc
    extension/playing/playing_team_actor.cc
    extension/playing/playing_team_manager.cc
    extension/playing/playing_team_pool.cc
    extension/playing/system_event_handler.cc)

set(SESSION_EXTENSION_PLAYING_INCLUDES
    extension/playing/actor_change_fight_score_event_handler.h
    extension/playing/actor_jump_scene_event_handler.h
    extension/playing/actor_levelup_event_handler.h
    extension/playing/actor_login_event_handler.h
    extension/playing/actor_logout_event_handler.h
    extension/playing/game_maze_message_handler.h
    extension/playing/game_message_handler.h
    extension/playing/maze_map_manager.h
    extension/playing/playing.h
    extension/playing/playing_activity_controller.h
    extension/playing/playing_actor.h
    extension/playing/playing_actor_manager.h
    extension/playing/playing_actor_pool.h
    extension/playing/playing_extension.h
    extension/playing/playing_load_balance.h
    extension/playing/playing_manager.h
    extension/playing/playing_message_handler.h
    extension/playing/playing_page.h
    extension/playing/playing_page_pool.h
    extension/playing/playing_pool.h
    extension/playing/playing_request_handler.h
    extension/playing/playing_team.h
    extension/playing/playing_team_actor.h
    extension/playing/playing_team_manager.h
    extension/playing/playing_team_pool.h
    extension/playing/playing_types.h
    extension/playing/system_event_handler.h)


set(SESSION_EXTENSION_AUCTION_SOURCES
    extension/auction/actor_functionality_enable_event_handler.cc
    extension/auction/actor_login_event_handler.cc
    extension/auction/actor_logout_event_handler.cc
    extension/auction/actor_save_event_handler.cc
    extension/auction/auction_actor.cc
    extension/auction/auction_actor_manager.cc
    extension/auction/auction_actor_pool.cc
    extension/auction/auction_extension.cc
    extension/auction/auction_history.cc
    extension/auction/auction_message_handler.cc
    extension/auction/auction_transaction.cc
    extension/auction/auction_transaction_manager.cc
    extension/auction/auction_transaction_pool.cc
    extension/auction/database_server_logined_event_handler.cc
    extension/auction/game_auction_message_handler.cc
    extension/auction/storage_auction_actor_login_response.cc)

set(SESSION_EXTENSION_AUCTION_INCLUDES
    extension/auction/actor_functionality_enable_event_handler.h
    extension/auction/actor_login_event_handler.h
    extension/auction/actor_logout_event_handler.h
    extension/auction/actor_save_event_handler.h
    extension/auction/auction_actor.h
    extension/auction/auction_actor_manager.h
    extension/auction/auction_actor_pool.h
    extension/auction/auction_extension.h
    extension/auction/auction_history.h
    extension/auction/auction_message_handler.h
    extension/auction/auction_transaction.h
    extension/auction/auction_transaction_manager.h
    extension/auction/auction_transaction_pool.h
    extension/auction/database_server_logined_event_handler.h
    extension/auction/game_auction_message_handler.h
    extension/auction/storage_auction_actor_login_response.h)

set(SESSION_EXTENSION_ITEM_SOURCES
    extension/item/actor_login_event_handler.cc
    extension/item/game_message_handler.cc
    extension/item/item_extension.cc
    extension/item/request_item_handler.cc
    extension/item/system_event_handler.cc)

set(SESSION_EXTENSION_ITEM_INCLUDES
    extension/item/actor_login_event_handler.h
    server/extension/item/game_message_handler.h
    server/extension/item/item_extension.h
    extension/item/request_item_handler.h
    extension/item/system_event_handler.h)

if(UNIX)

  set(ALL_FILES
    ${GLOBAL_SOURCES}
    ${GATEWAY_PROTOCOL_SOURCES} ${ENTITY_SOURCES}
    ${DATABASE_CLIENT_SOURCE} ${DATABASE_PROTOCOL_SOURCES}
    ${SCHEDULE_CLIENT_SOURCES} ${SCHEDULE_PROTOCOL_SOURCES}
    ${SESSION_PROTOCOL_SOURCES}
    ${SESSION_SERVER_SOURCES}
    ${SESSION_EVENT_SOURCES}
    ${SESSION_REQUEST_SOURCES}
    ${SESSION_EXTENSION_TEST_SOURCES}
    ${SESSION_EXTENSION_GM_SOURCES}
    ${SESSION_EXTENSION_SCENE_SOURCES}
    ${SESSION_EXTENSION_CHAT_SOURCES}
    ${SESSION_EXTENSION_SOCIAL_SOURCES}
    ${SESSION_EXTENSION_TEAM_SOURCES}
    ${SESSION_EXTENSION_MAIL_SOURCES}
    ${SESSION_EXTENSION_GUILD_SOURCES}
    ${SESSION_EXTENSION_PLAYING_SOURCES}
    ${SESSION_EXTENSION_AUCTION_SOURCES}
    ${SESSION_EXTENSION_ITEM_SOURCES})

elseif(MSVC)

  set(ALL_FILES
    ${GLOBAL_SOURCES}
    ${GATEWAY_PROTOCOL_SOURCES} ${ENTITY_SOURCES}
    ${DATABASE_CLIENT_SOURCE} ${DATABASE_PROTOCOL_SOURCES}
    ${SCHEDULE_CLIENT_SOURCES} ${SCHEDULE_PROTOCOL_SOURCES}
    ${SESSION_PROTOCOL_SOURCES}
    ${SESSION_SERVER_INCLUDES} ${SESSION_SERVER_SOURCES}
    ${SESSION_EVENT_INCLUDES} ${SESSION_EVENT_SOURCES}
    ${SESSION_REQUEST_INCLUDES} ${SESSION_REQUEST_SOURCES}
    ${SESSION_EXTENSION_TEST_INCLUDES} ${SESSION_EXTENSION_TEST_SOURCES}
    ${SESSION_EXTENSION_GM_INCLUDES} ${SESSION_EXTENSION_GM_SOURCES}
    ${SESSION_EXTENSION_SCENE_INCLUDES} ${SESSION_EXTENSION_SCENE_SOURCES}
    ${SESSION_EXTENSION_CHAT_INCLUDES} ${SESSION_EXTENSION_CHAT_SOURCES}
    ${SESSION_EXTENSION_SOCIAL_INCLUDES} ${SESSION_EXTENSION_SOCIAL_SOURCES}
    ${SESSION_EXTENSION_TEAM_INCLUDES} ${SESSION_EXTENSION_TEAM_SOURCES}
    ${SESSION_EXTENSION_MAIL_INCLUDES} ${SESSION_EXTENSION_MAIL_SOURCES}
    ${SESSION_EXTENSION_GUILD_INCLUDES} ${SESSION_EXTENSION_GUILD_SOURCES}
    ${SESSION_EXTENSION_PLAYING_INCLUDES} ${SESSION_EXTENSION_PLAYING_SOURCES}
    ${SESSION_EXTENSION_AUCTION_INCLUDES} ${SESSION_EXTENSION_AUCTION_SOURCES}
    ${SESSION_EXTENSION_ITEM_INCLUDES} ${SESSION_EXTENSION_ITEM_SOURCES})

source_group_by_dir(ALL_FILES)

endif()

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FILE(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/session_server)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/session_server)

add_executable(session_server ${ALL_FILES})

target_link_libraries(session_server coresh_static pthread mysqlpp rt thrift boost_signals)

add_dependencies(session_server run coresh_static)
