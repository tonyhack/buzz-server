//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-18 16:22:14.
// File name: team_request_handler.cc
//
// Description:
// Define class TeamRequestHandler.
//

#include "session_server/server/extension/team/team_request_handler.h"

#include <boost/bind.hpp>

#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_team_request_types.h"
#include "session_server/server/extension/team/team.h"
#include "session_server/server/extension/team/team_actor.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_manager.h"
#include "session_server/server/extension/team/team_pool.h"

namespace session {

namespace server {

namespace team {

TeamRequestHandler::TeamRequestHandler() {}
TeamRequestHandler::~TeamRequestHandler() {}

bool TeamRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TEAM_GET_MEMBERS,
      boost::bind(&TeamRequestHandler::OnRequestTeamGetMembers, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TEAM_ACTOR_QUIT,
      boost::bind(&TeamRequestHandler::OnRequestTeamActorQuit, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TEAM_CREATE,
      boost::bind(&TeamRequestHandler::OnRequestTeamCreate, this, _1, _2));

  return true;
}

void TeamRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TEAM_GET_MEMBERS);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TEAM_ACTOR_QUIT);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TEAM_CREATE);
}

int TeamRequestHandler::OnRequestTeamGetMembers(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestTeamGetMembers) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTeamGetMembers *request = (request::RequestTeamGetMembers *)message;

  TeamActor *team_actor = TeamActorManager::GetInstance()->Get(request->actor_);
  if(team_actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", request->actor_);
    return -1;
  }

  core::uint64 team_id = team_actor->GetTeamID();
  if(team_id == 0) {
    request->__set_team_(0);
    return 0;
  }

  Team *team = TeamManager::GetInstance()->Get(team_id);
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", team_id);
    return -1;
  }

  request->__set_captain_(team->GetID());

  const Team::MemberVector &members = team->GetMembers();
  for(Team::MemberVector::const_iterator iterator = members.begin();
      iterator != members.end(); ++iterator) {
    if(*iterator) {
      request->members_.push_back((*iterator)->GetID());
    }
  }

  return 0;
}

int TeamRequestHandler::OnRequestTeamActorQuit(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestTeamActorQuit) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTeamActorQuit *request = (request::RequestTeamActorQuit *)message;

  TeamActor *team_actor = TeamActorManager::GetInstance()->Get(request->actor_);
  if (team_actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", request->actor_);
    return -1;
  }

  core::uint64 team_id = team_actor->GetTeamID();
  if (team_id == 0) {
    return 0;
  }

  Team *team = TeamManager::GetInstance()->Get(team_id);
  if (team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", team_id);
    return -1;
  }

  if (team->RemoveMember(team_actor) == false) {
    LOG_ERROR("Team(%lu) RemoveMember(%lu) 失败", team_id, team_actor->GetID());
    return -1;
  }

  return 0;
}

int TeamRequestHandler::OnRequestTeamCreate(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestTeamCreate) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTeamCreate *request = (request::RequestTeamCreate *)message;

  TeamActor *team_actor = TeamActorManager::GetInstance()->Get(request->captain_);
  if (team_actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", request->captain_);
    return -1;
  }

  if (team_actor->GetTeamID() != 0) {
    LOG_ERROR("TeamActor(%lu) 已有队伍(%lu)", request->captain_, team_actor->GetTeamID());
    return -1;
  }

  Team *team = TeamPool::GetInstance()->Allocate();
  if (team == NULL) {
    LOG_ERROR("分配 Team 失败");
    return -1;
  }

  if (team->Initialize() == false) {
    TeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("初始化 Team 失败");
    return -1;
  }

  team->SetID(team_actor->GetID());

  if (TeamManager::GetInstance()->Add(team) == false) {
    team->Finalize();
    TeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("加入 TeamManager 失败");
    return -1;
  }

  if (team->AddMember(team_actor) == false) {
    TeamManager::GetInstance()->Remove(team->GetID());
    team->Finalize();
    TeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("队长加入队伍失败");
    return -1;
  }

  std::vector<int64_t>::const_iterator iterator = request->members_.begin();
  for (; iterator != request->members_.end(); ++iterator) {
    TeamActor *team_actor = TeamActorManager::GetInstance()->Get(*iterator);
    if (team_actor == NULL || team_actor->GetTeamID() != 0) {
      continue;
    }
    if (team->AddMember(team_actor) == false) {
      LOG_ERROR("增加 TeamActor(%lu) 失败", team_actor->GetID());
      continue;
    }
  }

  request->__set_team_(team->GetID());

  return 0;
}

}  // namespace team

}  // namespace server

}  // namespace session

