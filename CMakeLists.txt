project("buzz")

cmake_minimum_required(VERSION 0.1)


#macro file source group.
macro(source_group_by_dir source_files)
if(MSVC)
  set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
  foreach(sgbd_file ${${source_files}})
    string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
    string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
    string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
    string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
    if(sgbd_nogroup)
      set(sgbd_group_name "\\")
    endif(sgbd_nogroup)
    source_group(${sgbd_group_name} FILES ${sgbd_file})
  endforeach(sgbd_file)
endif(MSVC)
endmacro(source_group_by_dir)


#default compile
if(UNIX)

set(MY_CXX_FLAGS_WARNING "-Wall -Wno-unused-variable -Wno-deprecated -Werror")
set(MY_CXX_FLAGS_MACRO "-DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -D_TEST_GAME_EXTENSION -DBOOST_SIGNALS_NO_DEPRECATION_WARNING")
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -g ${MY_CXX_FLAGS_WARNING} -rdynamic -fno-stack-protector -fno-inline -fno-strict-aliasing ${MY_CXX_FLAGS_MACRO}")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g -Wall ${MY_CXX_FLAGS_WARNING} -rdynamic -fno-inline -fno-strict-aliasing ${MY_CXX_FLAGS_MACRO}")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g -O2 ${MY_CXX_FLAGS_WARNING} -rdynamic -DNDEBUG -fno-strict-aliasing ${MY_CXX_FLAGS_MACRO}")

endif(UNIX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(THRIFT_GENERATE_PATH ${PROJECT_BINARY_DIR}/thrift)

set(GLOBAL_META_DIR ${PROJECT_SOURCE_DIR}/src/global/meta/)
set(ENTITY_META_DIR ${PROJECT_SOURCE_DIR}/src/entity/meta/)

include_directories(
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src/global"
	"${PROJECT_SOURCE_DIR}/src/entity"
	"/usr/include/mysql/"
	)

FILE(MAKE_DIRECTORY ${THRIFT_GENERATE_PATH})

add_custom_target(run ALL
    COMMAND ${PROJECT_SOURCE_DIR}/generate/compile_thrift.sh ${PROJECT_SOURCE_DIR}/src ${THRIFT_GENERATE_PATH}/thrift.mk
    COMMAND +make -f ${THRIFT_GENERATE_PATH}/thrift.mk)

set(CORE_BASE_SOURCES
		${PROJECT_SOURCE_DIR}/src/core/base/bound_queue.cc
		${PROJECT_SOURCE_DIR}/src/core/base/boundless_queue.cc
		${PROJECT_SOURCE_DIR}/src/core/base/byte_buffer_dynamic.cc
		${PROJECT_SOURCE_DIR}/src/core/base/common.cc
		${PROJECT_SOURCE_DIR}/src/core/base/logging.cc
		${PROJECT_SOURCE_DIR}/src/core/base/thread.cc)

set(CORE_TINYXML_SOURCES
		${PROJECT_SOURCE_DIR}/src/core/tinyxml/tinystr.cpp
		${PROJECT_SOURCE_DIR}/src/core/tinyxml/tinyxml.cpp
		${PROJECT_SOURCE_DIR}/src/core/tinyxml/tinyxmlerror.cpp
		${PROJECT_SOURCE_DIR}/src/core/tinyxml/tinyxmlparser.cpp)

set(CORE_SOURCES
		${PROJECT_SOURCE_DIR}/src/core/acceptor.cc
		${PROJECT_SOURCE_DIR}/src/core/async_logging.cc
		${PROJECT_SOURCE_DIR}/src/core/async_logging_loop.cc
		${PROJECT_SOURCE_DIR}/src/core/async_logging_singleton.cc
		${PROJECT_SOURCE_DIR}/src/core/channel.cc
		${PROJECT_SOURCE_DIR}/src/core/configure.cc
		${PROJECT_SOURCE_DIR}/src/core/connector.cc
		${PROJECT_SOURCE_DIR}/src/core/epoller.cc
		${PROJECT_SOURCE_DIR}/src/core/event_control.cc
		${PROJECT_SOURCE_DIR}/src/core/event_loop.cc
		${PROJECT_SOURCE_DIR}/src/core/event_loop_thread_group.cc
		${PROJECT_SOURCE_DIR}/src/core/inet_address.cc
		${PROJECT_SOURCE_DIR}/src/core/loop_thread.cc
		${PROJECT_SOURCE_DIR}/src/core/signal_set.cc
		${PROJECT_SOURCE_DIR}/src/core/tcp_client.cc
		${PROJECT_SOURCE_DIR}/src/core/tcp_connection.cc
		${PROJECT_SOURCE_DIR}/src/core/tcp_server.cc
		${PROJECT_SOURCE_DIR}/src/core/tcp_socket.cc
		${PROJECT_SOURCE_DIR}/src/core/timing_base.cc
		${PROJECT_SOURCE_DIR}/src/core/timing_group.cc
		${PROJECT_SOURCE_DIR}/src/core/timing_wheel.cc)

set(GLOBAL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/global/ac_automata.cc
    ${PROJECT_SOURCE_DIR}/src/global/area_point_define.cc
    ${PROJECT_SOURCE_DIR}/src/global/common_functions.cc
    ${PROJECT_SOURCE_DIR}/src/global/global_packet.cc
    ${PROJECT_SOURCE_DIR}/src/global/kickout_actor_type_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/global/kickout_actor_type_types.cpp
    ${PROJECT_SOURCE_DIR}/src/global/logging.cc
    ${PROJECT_SOURCE_DIR}/src/global/server_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/server_type_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/global/server_type_types.cpp
    ${PROJECT_SOURCE_DIR}/src/global/simple_encrypt.cc
    ${PROJECT_SOURCE_DIR}/src/global/word_filter.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/auction_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/guild_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/load_vip_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/load_world_boss_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/maze_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/misc_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/playing_configure.cc
    ${PROJECT_SOURCE_DIR}/src/global/configure/social_configure.cc)

SET(SCHEDULE_PROTOCOL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/schedule_server/protocol/schedule_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/schedule_server/protocol/schedule_protocol_types.cpp)

set(SCHEDULE_CLIENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/schedule_server/client/schedule_client.cc
    ${PROJECT_SOURCE_DIR}/src/schedule_server/client/schedule_client2.cc)

SET(DATABASE_PROTOCOL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/async_request_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/async_request_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/database_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/database_storage_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/database_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/database_storage_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_achieve_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_achieve_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_achieve_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_achieve_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_logout_center_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_logout_center_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_online_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_online_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_attr_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_attr_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_attr_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_attr_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_transaction_del_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_transaction_del_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_transaction_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_auction_transaction_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_buff_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_buff_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_buff_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_buff_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_check_session_constants.cpp  
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_check_session_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_cooling_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_cooling_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_cooling_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_cooling_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_gem_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_gem_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_gem_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_gem_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_del_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_del_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_guild_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_item_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_add_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_add_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_affix_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_affix_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_affix_state_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_affix_state_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_del_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_del_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_read_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_mail_read_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_playing_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_playing_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_playing_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_playing_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_skill_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_skill_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_skill_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_skill_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_social_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_social_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_social_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_social_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_soul_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_spirit_facade_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_spirit_facade_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_spirit_facade_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_spirit_facade_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_actor_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_actor_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_actor_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_actor_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_login_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_login_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_task_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_welfare_load_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_welfare_load_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_welfare_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_welfare_save_types.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_world_data_save_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/database_server/protocol/storage_world_data_save_types.cpp)


SET(DATABASE_CLIENT_SOURCE
    ${PROJECT_SOURCE_DIR}/src/database_server/client/database_client.cc
    ${PROJECT_SOURCE_DIR}/src/database_server/client/database_session.cc)

SET(GATEWAY_PROTOCOL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_attr_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_attr_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_auction_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_auction_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_buff_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_buff_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_bar_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_bar_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_chat_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_chat_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_cooling_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_cooling_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_entity_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_entity_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_error_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_error_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_gm_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_gm_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_guild_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_guild_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_inform_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_inform_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_item_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_item_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_mail_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_mail_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_mall_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_mall_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_playing_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_playing_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_scene_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_scene_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_skill_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_skill_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_shop_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_shop_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_social_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_social_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_soul_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_soul_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_task_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_task_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_team_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_team_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_transport_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_transport_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_welfare_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/gateway_welfare_protocol_constants.cpp)

SET(SESSION_PROTOCOL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_actor_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_actor_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_auction_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_auction_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_chat_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_chat_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_gm_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_gm_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_guild_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_guild_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_item_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_item_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_mail_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_mail_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_playing_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_playing_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_scene_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_scene_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_social_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_social_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_team_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/game_team_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_admin_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_admin_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_game_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_game_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_gateway_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_gateway_protocol_types.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/session_server/protocol/session_protocol_types.cpp)

SET(SESSION_CLIENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/session_server/client/session_client.cc)

SET(GAME_PROTOCOL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/game_server/protocol/game_protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/game_server/protocol/game_protocol_types.cpp)

SET(GAME_CLIENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/game_server/client/game_client.cc
    ${PROJECT_SOURCE_DIR}/src/game_server/client/game_session.cc)

SET(ENTITY_SOURCES
    ${PROJECT_SOURCE_DIR}/src/entity/actor_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/actor_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/achieve_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/achieve_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/bar_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/bar_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/constants_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/constants_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/contacts_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/contacts_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/cooling_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/cooling_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/chat_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/chat_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/camp_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/camp_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/direction_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/direction_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/error_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/error_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/entity_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/entity_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/equipment_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/equipment_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/fight_score_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/fight_score_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/functionality_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/functionality_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/gem_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/gem_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/gender_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/gender_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/homeland_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/homeland_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/inform_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/inform_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/item_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/item_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/mail_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/mail_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/mail_system_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/mail_system_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/mall_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/mall_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/maze_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/maze_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/npc_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/npc_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/playing_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/playing_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/resource_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/resource_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/role_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/role_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/scene_item_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/scene_item_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/skill_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/skill_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/soul_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/soul_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/spirit_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/spirit_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/task_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/task_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/transport_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/transport_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/vip_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/vip_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/vocation_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/vocation_types.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/welfare_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/entity/welfare_types.cpp)

SET(LOGIN_PROTOCOL_SOURCES
  ${PROJECT_SOURCE_DIR}/src/login_server_informal/protocol/database_storage_constants.cpp
  ${PROJECT_SOURCE_DIR}/src/login_server_informal/protocol/database_storage_types.cpp
  ${PROJECT_SOURCE_DIR}/src/login_server_informal/protocol/login_protocol_constants.cpp
  ${PROJECT_SOURCE_DIR}/src/login_server_informal/protocol/login_protocol_types.cpp)

#add_subdirectory(src/core)

add_subdirectory(src/coresh)

#add_subdirectory(src/entity/meta)

#add_subdirectory(src/test)

#add_subdirectory(src/global/meta)
#add_subdirectory(src/global/test)

add_subdirectory(src/schedule_server)
add_subdirectory(src/database_server)
add_subdirectory(src/gateway_server)
add_subdirectory(src/session_server)
add_subdirectory(src/game_server)
add_subdirectory(src/login_server_informal)

#add_subdirectory(src/mytest)
