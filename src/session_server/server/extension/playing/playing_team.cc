//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-12 04:24:54.
// File name: playing_team.cc
//
// Description:
// Define class PlayingTeam.
//

#include "session_server/server/extension/playing/playing_team.h"

#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/common_functions.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_manager.h"
#include "session_server/server/extension/playing/playing_team_actor.h"
#include "session_server/server/extension/playing/playing_team_manager.h"
#include "session_server/server/extension/playing/playing_team_pool.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_team_request_types.h"

namespace session {

namespace server {

namespace playing {

PlayingTeam::PlayingTeam() {}
PlayingTeam::~PlayingTeam() {}

bool PlayingTeam::Initialize(core::int32 playing_template, core::uint64 id) {
  this->id_ = id;
  this->min_fight_score_ = 0;
  this->status_ = entity::PlayingTeamStatus::WAITING;
  this->capacity_ = 0;
  this->playing_ = 0;
  this->playing_template_ = playing_template;
  this->SetCapacity(global::configure::PlayingConfigure::kTeamMaxMembers_);

  return true;
}

void PlayingTeam::Finalize() {
  this->id_ = 0;
  this->members_.clear();

  if (this->GetStatus() == entity::PlayingTeamStatus::READY) {
    SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_READY, this);
  }
}

bool PlayingTeam::AddMember(PlayingTeamActor *actor) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  PlayingActor *playing_actor = actor->GetActor();
  if (playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor 失败");
    return false;
  }

  SessionActor *session_actor = playing_actor->GetActor();
  if (session_actor == NULL) {
    LOG_ERROR("获取 SessionActor 失败");
    return false;
  }

  BriefActor *brief_actor = session_actor->GetBriefActor();
  if (brief_actor == NULL) {
    LOG_ERROR("获取 BriefActor 失败");
    return false;
  }

  actor->SetTeam(this->GetID());
  actor->SetReady(false);

  gateway::protocol::MessagePlayingTeamJoinSynchronize synchronize;
  synchronize.__set_actor_(global::ToString(playing_actor->GetID()));
  synchronize.__set_name_(session_actor->GetName());
  synchronize.__set_fight_score_(brief_actor->GetCurrentFightScore());
  synchronize.__set_ready_(actor->GetReady());
  this->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_JOIN_SYNCHRONIZE);

  this->members_.push_back(actor);

  // 同步到 game server
  session::protocol::MessagePlayingTeamSynchronize sync_game;
  sync_game.__set_actor_(actor->GetID());
  sync_game.__set_playing_team_(this->GetID());
  session_actor->SendGameMessage(sync_game,
      session::protocol::GameMessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);

  return true;
}

bool PlayingTeam::RemoveMember(PlayingTeamActor *actor) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  PlayingActor *playing_actor = actor->GetActor();
  if (playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor 失败");
    return false;
  }

  bool remove = false;

  MemberVector::iterator iterator = this->members_.begin();
  for (; iterator != this->members_.end(); ++iterator) {
    if (*iterator && (*iterator)->GetID() == actor->GetID()) {
      this->members_.erase(iterator);
      remove = true;
      break;
    }
  }

  if (remove == true) {
    gateway::protocol::MessagePlayingTeamQuitSynchronize synchronize;
    synchronize.__set_actor_(global::ToString(actor->GetID()));

    actor->SetTeam(0);
    actor->SetReady(false);

    playing_actor->SendMessage(synchronize,
        gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_QUIT_SYNCHRONIZE);

    SessionActor *session_actor = playing_actor->GetActor();
    if (session_actor) {
      // 同步到 game server
      session::protocol::MessagePlayingTeamSynchronize sync_game;
      sync_game.__set_actor_(actor->GetID());
      sync_game.__set_playing_team_(0);
      session_actor->SendGameMessage(sync_game,
          session::protocol::GameMessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);
    }

    if (this->GetSize() <= 0) {
      PlayingTeamManager::GetInstance()->Remove(this->GetID());
      this->Finalize();
      PlayingTeamPool::GetInstance()->Deallocate(this);
    } else {
      if (this->GetID() == actor->GetID()) {
        if (this->SetNewCaptain(this->members_[0]) == false) {
          LOG_ERROR("设置新队长(%lu) 失败", this->GetID());
          return false;
        }
      }
      this->BroadcastMessage(synchronize,
          gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_QUIT_SYNCHRONIZE);
    }
  }

  return remove;
}

bool PlayingTeam::SetCaptain(PlayingTeamActor *actor) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if (actor->GetTeam() != this->GetID() || actor->GetID() == this->GetID()) {
    LOG_ERROR("队伍ID不符或ID不符");
    return false;
  }

  this->SetID(actor->GetID());

  return true;
}

bool PlayingTeam::SetNewCaptain(PlayingTeamActor *actor) {
  if (actor == NULL || actor->GetTeam() != this->GetID()) {
    LOG_ERROR("参数错误");
    return false;
  }

  PlayingTeamManager::GetInstance()->Remove(this->GetID());

  this->SetID(actor->GetID());

  if (PlayingTeamManager::GetInstance()->Add(this) == false) {
    this->Finalize();
    PlayingTeamPool::GetInstance()->Deallocate(this);
    LOG_ERROR("队伍(%lu) 加入管理器失败", this->GetID());
    return false;
  }

  MemberVector::iterator iterator = this->members_.begin();
  for (; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor != NULL) {
      team_actor->SetTeam(this->GetID());
    }
  }

  // 同步队伍ID改变
  gateway::protocol::MessagePlayingTeamSynchronize synchronize;
  synchronize.__set_id_(global::ToString(this->GetID()));
  this->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);

  // 同步到 game server
  this->SyncGameServer();

  return true;
}

const std::string PlayingTeam::GetName() const {
  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(this->GetID());
  if (actor != NULL) {
    return actor->GetName();
  }

  return "";
}

bool PlayingTeam::Dismiss() {
  gateway::protocol::MessagePlayingTeamQuitSynchronize synchronize;
  session::protocol::MessagePlayingTeamSynchronize sync_game;
  sync_game.__set_playing_team_(0);

  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor == NULL) {
      continue;
    }
    PlayingActor *playing_actor = team_actor->GetActor();
    if (playing_actor == NULL) {
      continue;
    }
    SessionActor *session_actor = playing_actor->GetActor();
    if (session_actor == NULL) {
      continue;
    }
    synchronize.__set_actor_(global::ToString(session_actor->GetActorID()));
    sync_game.__set_actor_(session_actor->GetActorID());
    session_actor->SendMessage(synchronize,
        gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_QUIT_SYNCHRONIZE);
    session_actor->SendGameMessage(sync_game,
        session::protocol::GameMessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);
    team_actor->SetTeam(0);
  }

  this->members_.clear();

  if (this->GetPlaying() != 0) {
    PlayingTeamManager::GetInstance()->RemoveReadyTeam(this->GetPlaying());
  }

  return true;
}

bool PlayingTeam::CheckReady() const {
  MemberVector::const_iterator iterator = this->members_.begin();
  for (; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor && team_actor->GetID() != this->GetID() &&
        team_actor->GetReady() == false) {
      return false;
    }
  }

  return true;
}

bool PlayingTeam::GotoReady() {
  if (this->GetStatus() != entity::PlayingTeamStatus::WAITING) {
    LOG_ERROR("副本房间的状态错误");
    return false;
  }

  this->SetStatus(entity::PlayingTeamStatus::READY);
  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_READY,
      MISC_CONF()->playing_team_ready_secs_ * 1000, 1, this,
      "PlayingTeam::GotoReady");

  gateway::protocol::MessagePlayingTeamSynchronize synchronize;
  synchronize.__set_id_(global::ToString(this->GetID()));
  synchronize.__set_status_(this->GetStatus());
  this->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);

  return true;
}

bool PlayingTeam::GotoPlay() {
  if (this->GetStatus() != entity::PlayingTeamStatus::READY) {
    return false;
  }

  Playing *playing = PlayingManager::GetInstance()->Get(this->GetPlaying());
  if (playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", this->GetPlaying());
    return false;
  }

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetPlayingTemplate());
  if (cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", playing->GetTemplateID());
    return false;
  }

  core::int32 x = 0, y = 0;
  if (cell->GetEnterPosition(0, x, y) == false) {
    LOG_ERROR("获取 进入副本(%u)坐标 失败", playing->GetTemplateID());
    return false;
  }

  // 把房间玩家新组成一个队伍
  request::RequestTeamCreate request_create_team;
  request_create_team.__set_captain_(this->GetID());

  // 房间中所有玩家离队
  request::RequestTeamActorQuit request_quit_team;
  PlayingTeam::MemberVector::const_iterator iterator = this->members_.begin();
  for (; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor == NULL) {
      continue;
    }
    request_quit_team.__set_actor_(team_actor->GetID());
    if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_TEAM_ACTOR_QUIT,
          &request_quit_team, sizeof(request_quit_team)) != 0) {
      LOG_ERROR("请求 REQUEST_TEAM_ACTOR_QUIT 失败");
      return false;
    }
    if (team_actor->GetID() != this->GetID()) {
      request_create_team.members_.push_back(team_actor->GetID());
    }
  }

  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TEAM_CREATE, &request_create_team,
        sizeof(request_create_team)) != 0) {
    LOG_ERROR("请求 REQEUST_TEAM_CREATE 失败");
    return false;
  }

  iterator = this->members_.begin();
  for (; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor == NULL) {
      continue;
    }
    request::RequestSceneJump request_jump;
    request_jump.__set_actor_(team_actor->GetID());
    request_jump.__set_scene_(playing->GetScene());
    request_jump.__set_x_(x);
    request_jump.__set_y_(y);
    if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_JUMP,
          &request_jump, sizeof(request_jump)) != 0) {
      LOG_ERROR("请求跳转场景(%lu) 失败", playing->GetScene());
      continue;
    }
  }

  this->Dismiss();

  PlayingTeamManager::GetInstance()->Remove(this->GetID());
  this->Finalize();
  PlayingTeamPool::GetInstance()->Deallocate(this);

  return true;
}

void PlayingTeam::Synchronize() {
  gateway::protocol::MessagePlayingTeamSynchronize synchronize;
  synchronize.__set_id_(global::ToString(this->GetID()));
  synchronize.__set_name_(this->GetName());
  synchronize.__set_status_(this->GetStatus());
  synchronize.__set_capacity_(this->GetCapacity());
  synchronize.__set_min_fight_score_(this->GetMinFightScore());
  synchronize.__set_auto_open_(this->GetAutoOpen());
  synchronize.__isset.members_ = true;
  entity::PlayingTeamMemberField member_field;
  MemberVector::iterator iterator = this->members_.begin();
  for (; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor == NULL) {
      LOG_ERROR("PlayingTeam(%lu) members_ 中有空指针", this->GetID());
      continue;
    }
    PlayingActor *playing_actor = team_actor->GetActor();
    if (playing_actor == NULL) {
      LOG_ERROR("获取 PlayingActor 失败");
      continue;
    }
    SessionActor *session_actor = playing_actor->GetActor();
    if (session_actor == NULL) {
      LOG_ERROR("获取 SessionActor 失败");
      continue;
    }
    BriefActor *brief_actor = session_actor->GetBriefActor();
    if (brief_actor == NULL) {
      LOG_ERROR("获取 BriefActor 失败");
      continue;
    }
    member_field.__set_actor_(session_actor->GetStringID());
    member_field.__set_name_(session_actor->GetName());
    member_field.__set_level_(session_actor->GetLevel());
    member_field.__set_fight_score_(brief_actor->GetCurrentFightScore());
    member_field.__set_ready_(team_actor->GetReady());
    synchronize.members_.push_back(member_field);
  }
  this->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);
}

void PlayingTeam::Synchronize(PlayingActor *actor) {
  gateway::protocol::MessagePlayingTeamSynchronize synchronize;
  synchronize.__set_id_(global::ToString(this->GetID()));
  synchronize.__set_name_(this->GetName());
  synchronize.__set_status_(this->GetStatus());
  synchronize.__set_capacity_(this->GetCapacity());
  synchronize.__set_min_fight_score_(this->GetMinFightScore());
  synchronize.__set_auto_open_(this->GetAutoOpen());
  synchronize.__isset.members_ = true;
  entity::PlayingTeamMemberField member_field;
  MemberVector::iterator iterator = this->members_.begin();
  for (; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor == NULL) {
      LOG_ERROR("PlayingTeam(%lu) members_ 中有空指针", this->GetID());
      continue;
    }
    PlayingActor *playing_actor = team_actor->GetActor();
    if (playing_actor == NULL) {
      LOG_ERROR("获取 PlayingActor 失败");
      continue;
    }
    SessionActor *session_actor = playing_actor->GetActor();
    if (session_actor == NULL) {
      LOG_ERROR("获取 SessionActor 失败");
      continue;
    }
    BriefActor *brief_actor = session_actor->GetBriefActor();
    if (brief_actor == NULL) {
      LOG_ERROR("获取 BriefActor 失败");
      continue;
    }
    member_field.__set_actor_(session_actor->GetStringID());
    member_field.__set_name_(session_actor->GetName());
    member_field.__set_level_(session_actor->GetLevel());
    member_field.__set_fight_score_(brief_actor->GetCurrentFightScore());
    member_field.__set_ready_(team_actor->GetReady());
    synchronize.members_.push_back(member_field);
  }
  actor->SendMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);
}

void PlayingTeam::SyncGameServer() {
  // 同步到 game server
  session::protocol::MessagePlayingTeamSynchronize sync_game;
  sync_game.__set_playing_team_(this->GetID());
  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if (team_actor == NULL) {
      continue;
    }
    PlayingActor *playing_actor = team_actor->GetActor();
    if (playing_actor == NULL) {
      continue;
    }
    SessionActor *session_actor = playing_actor->GetActor();
    if (session_actor == NULL) {
      continue;
    }
    sync_game.__set_actor_(session_actor->GetActorID());
    session_actor->SendGameMessage(sync_game,
        session::protocol::GameMessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);
  }
}

void PlayingTeam::OnTimer(core::uint32 id) {
  switch (id) {
    case TIMER_ID_READY:
      this->GotoPlay();
      break;
    default:
      break;
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session

