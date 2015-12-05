//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-23 13:29:07.
// File name: team_request_handler.cc
//
// Description:
// Define class TeamRequestHandler.
//

#include "game_server/server/extension/team/team_request_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_team_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/extension/team/team_actor.h"
#include "game_server/server/extension/team/team_actor_manager.h"
#include "global/logging.h"

namespace game  {

namespace server {

namespace team {

TeamRequestHandler::TeamRequestHandler() {}
TeamRequestHandler::~TeamRequestHandler() {}

bool TeamRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TEAM_GET_ACTOR_TEAM,
      boost::bind(&TeamRequestHandler::OnRequestTeamGetActorTeam, this, _1, _2));

  return true;
}

void TeamRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TEAM_GET_ACTOR_TEAM);
}

int TeamRequestHandler::OnRequestTeamGetActorTeam(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestTeamGetActorTeam) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTeamGetActorTeam *request =
    (request::RequestTeamGetActorTeam *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", request->actor_id_);
    return -1;
  }

  request->__set_team_id_(actor->GetTeam());

  return 0;
}

}  // namespace team

}  // namespace server

}  // namespace game

