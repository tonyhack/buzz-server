//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-21 12:31:20.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/scene/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/request/game_guild_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_team_request_types.h"

namespace game {

namespace server {

namespace scene {

GameRole *FacadeRequest::GetRole(entity::EntityType::type type, core::uint64 id) {
  if(type == entity::EntityType::TYPE_ACTOR) {
    return (GameRole *)GameActorManager::GetInstance()->GetActor(id);
  } else if(type == entity::EntityType::TYPE_NPC) {
    return (GameRole *)GameNpcManager::GetInstance()->Get(id);
  }

  return NULL;
}

core::uint64 FacadeRequest::GetGuildID(core::uint64 actor) {
  request::RequestGuildGetActorGuild request;
  request.__set_actor_id_(actor);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GUILD_GET_ACTOR_GUILD,
        &request, sizeof(request)) != 0) {
    return 0;
  }

  return request.guild_id_;
}

core::uint64 FacadeRequest::GetTeamID(core::uint64 actor) {
  request::RequestTeamGetActorTeam request;
  request.__set_actor_id_(actor);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TEAM_GET_ACTOR_TEAM,
        &request, sizeof(request)) != 0) {
    return 0;
  }

  return request.team_id_;
}

bool FacadeRequest::CheckAddItem(core::uint64 actor, core::int32 template_id,
    core::int32 number, entity::BindRequireType::type bind) {
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(template_id);
  request.__set_number_(number);
  request.__set_bind_(bind);
  request.__set_simulate_(true);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
        &request, sizeof(request)) != 0) {
    return false;
  }

  return request.result_ == request::AddItemResult::SUCCESS;
}

bool FacadeRequest::AddItem(core::uint64 actor, core::int32 template_id,
    core::int32 number, entity::BindRequireType::type bind) {
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(template_id);
  request.__set_number_(number);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
        &request, sizeof(request)) != 0) {
    return false;
  }

  return request.result_ == request::AddItemResult::SUCCESS;
}

bool FacadeRequest::RemmoveItem(core::uint64 actor, core::int32 template_id,
    core::int32 number, entity::BindRequireType::type bind) {
  request::RequestRemoveItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(template_id);
  request.__set_number_(number);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_REMOVE_ITEM,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("删除道具错误");
    return false;
  }
  return true;
}

}  // namespace scene

}  // namespace server

}  // namespace game

