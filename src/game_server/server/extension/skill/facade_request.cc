//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-11 10:45:14.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/skill/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_cooling_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_soul_request_types.h"
#include "game_server/server/request/game_team_request_types.h"

namespace game {

namespace server {

namespace skill {

bool FacadeRequest::AddBuff(entity::EntityType::type src_type, core::uint64 src_id,
    entity::EntityType::type dest_type, core::uint64 dest_id, core::uint32 buff) {
  request::RequestBuffStart request;
  request.__set_src_entity_type_(src_type);
  request.__set_src_entity_id_(src_id);
  request.__set_dest_entity_type_(dest_type);
  request.__set_dest_entity_id_(dest_id);
  request.__set_buff_id_(buff);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_START, &request,
        sizeof(request)) == 0 && request.succeed_ == true) {
    return true;
  }
  return false;
}

bool FacadeRequest::RemoveBuff(entity::EntityType::type type, core::uint64 id,
    core::uint32 buff) {
  request::RequestBuffEnd request;
  request.__set_dest_entity_type_(type);
  request.__set_dest_entity_id_(id);
  request.__set_buff_id_(buff);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_END, &request,
        sizeof(request)) == 0 && request.succeed_ == true) {
    return true;
  }
  return false;
}

bool FacadeRequest::CheckBuffExist(entity::EntityType::type type, core::uint64 id,
    core::int32 buff) {
  request::RequestBuffGetExist request;
  request.__set_type_(type);
  request.__set_id_(id);
  request.__set_buff_id_(buff);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_GET_EXIST, &request,
        sizeof(request)) == 0 && request.exist_ == true) {
    return true;
  }
  return false;
}

bool FacadeRequest::CheckCooling(entity::EntityType::type type, core::uint64 id,
    core::uint32 cooling) {
  request::RequestCheckInCooling request;
  request.__set_type_(type);
  request.__set_id_(id);
  request.__set_cooling_id_(cooling);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_CHECK_IN_COOLING, &request,
        sizeof(request)) == 0 && request.in_cooling_ == true) {
    return true;
  }
  return false;
}

bool FacadeRequest::Cooling(entity::EntityType::type type, core::uint64 id,
    core::uint32 cooling) {
  request::RequestStartCooling request;
  request.__set_type_(type);
  request.__set_id_(id);
  request.__set_cooling_id_(cooling);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_START_COOLING, &request,
        sizeof(request)) == 0) {
    return true;
  }
  return false;
}

bool FacadeRequest::CheckSameTeam(core::uint64 actor1_id, core::uint64 actor2_id) {
  request::RequestTeamGetActorTeam request;
  request.__set_actor_id_(actor1_id);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TEAM_GET_ACTOR_TEAM,
        &request, sizeof(request)) != 0) {
    return false;
  }
  
  core::int64 actor1_team = request.team_id_;
  request.__set_actor_id_(actor2_id);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TEAM_GET_ACTOR_TEAM,
        &request, sizeof(request)) != 0) {
    return false;
  }

  if(request.team_id_ == 0 || actor1_team == 0) {
    return false;
  }
  return actor1_team == request.team_id_ ?
    true : false;
}

entity::SoulEnergyStatus::type FacadeRequest::GetEnergyStatus(core::uint64 actor) {
  request::RequestSoulGetEnergyStatus request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SOUL_GET_ENERGY_STATUS,
        &request, sizeof(request)) != 0) {
    return entity::SoulEnergyStatus::MAX;
  }
  return request.status_;
}

}  // namespace skill

}  // namespace server

}  // namespace game

