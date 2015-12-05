//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-22 17:52:32.
// File name: team.cc
//
// Description:
// Define class Team.
//

#include "session_server/server/extension/team/team.h"

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_team_protocol_types.h"
#include "global/common_functions.h"
#include "session_server/protocol/game_team_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/team/configure.h"
#include "session_server/server/extension/team/team_configure.h"
#include "session_server/server/extension/team/team_manager.h"
#include "session_server/server/extension/team/team_pool.h"

namespace session {

namespace server {

namespace team {

Team::Team() {}
Team::~Team() {}

bool Team::Initialize() {
  this->id_ = 0;

  return true;
}

void Team::Finalize() {
  this->members_.clear();
}

bool Team::AddMember(TeamActor *member) {
  if(member == NULL) {
    return false;
  }

  SessionActor *actor = member->GetActor();
  if(actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", member->GetID());
    return false;
  }

  // 广播增加了新成员
  gateway::protocol::MessageTeamMemberAddSynchronize synchronize;
  synchronize.__set_actor_(global::ToString(actor->GetActorID()));
  synchronize.__set_name_(actor->GetName());
  synchronize.__set_level_(actor->GetLevel());
  synchronize.__set_map_(actor->GetMap());
  this->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_ADD_SYNCHRONIZE);

  // 插入队中
  this->members_.push_back(member);

  member->SetTeamID(this->GetID());

  // 同步组队数据到客户端
  this->Synchronize(member);

  // 同步到GameServer
  session::protocol::MessageTeamSynchronize sync_game;
  sync_game.__set_actor_(actor->GetActorID());
  sync_game.__set_team_(this->GetID());
  actor->SendGameMessage(sync_game,
      session::protocol::GameMessageType::MESSAGE_TEAM_SYNCHRONIZE);

  this->SynchronizeBuff();

  return true;
}

bool Team::RemoveMember(TeamActor *member) {
  if(member == NULL) {
    return false;
  }

  bool remove = false;

  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    if(*iterator && (*iterator)->GetID() == member->GetID()) {
      this->members_.erase(iterator);
      remove = true;
      break;
    }
  }

  if(remove == true) {
    // 移除队员消息
    gateway::protocol::MessageTeamMemberRemoveSynchronize synchronize;
    synchronize.__set_actor_(global::ToString(member->GetID()));

    // 成员为0，则解散
    if(this->members_.size() <= 1) {
      if(this->RemoveAll() == true) {
        // 回收对象
        this->Finalize();
        TeamPool::GetInstance()->Deallocate(this);
      }
    } else {
      // 如果是队长
      if(this->GetID() == member->GetID()) {
        // 设置新队长
        if(this->SetNewCaptain(this->members_[0]) == false) {
          LOG_ERROR("设置新队长(%lu)时出错", this->GetID());
          return false;
        }
      }
      this->BroadcastMessage(synchronize,
          gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_REMOVE_SYNCHRONIZE);
    }

    // 被删除的队员队伍数据清0
    member->SetTeamID(0);
    member->SendMessage(synchronize,
        gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_REMOVE_SYNCHRONIZE);

    // 同步GameServer
    SessionActor *actor = member->GetActor();
    if(actor) {
      session::protocol::MessageTeamSynchronize sync_game;
      sync_game.__set_actor_(actor->GetActorID());
      sync_game.__set_team_(0);
      actor->SendGameMessage(sync_game,
          session::protocol::GameMessageType::MESSAGE_TEAM_SYNCHRONIZE);
    }

    this->SynchronizeBuff();
  }

  return remove;
}

bool Team::SetNewCaptain(TeamActor *member) {
  if(member == NULL || member->GetTeamID() != this->GetID() ||
      member->GetID() == this->GetID()) {
    LOG_ERROR("队伍(%lu)设置新队长时，参数错误", this->GetID());
    return false;
  }

  // 从管理器中移除
  Team *team = TeamManager::GetInstance()->Remove(this->GetID());
  if(team == NULL) {
    LOG_ERROR("队伍(%lu)设置新队长时，从管理器删除失败", this->GetID());
    return false;
  }

  // 设置新队伍ID
  this->SetID(member->GetID());

  // 加入管理器
  if(TeamManager::GetInstance()->Add(this) == false) {
    this->Finalize();
    TeamPool::GetInstance()->Deallocate(this);
    LOG_ERROR("设置新队长(%lu)时，加入管理器失败", this->GetID());
    return false;
  }

  session::protocol::MessageTeamSynchronize sync_game;
  sync_game.__set_team_(this->GetID());

  // 更改所有队员的队伍ID
  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    if(*iterator) {
      (*iterator)->SetTeamID(this->GetID());
      // 更新到场景玩家
      SessionActor *actor = member->GetActor();
      if(actor) {
        sync_game.__set_actor_(actor->GetActorID());
        actor->SendGameMessage(sync_game,
            session::protocol::GameMessageType::MESSAGE_TEAM_SYNCHRONIZE);
      }
    }
  }

  // 广播队伍信息同步
  gateway::protocol::MessageTeamSynchronize synchronize;
  synchronize.__set_team_id_(global::ToString(this->GetID()));
  synchronize.__set_captain_(global::ToString(this->GetID()));
  this->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_TEAM_SYNCHRONIZE);

  return true;
}

bool Team::RemoveAll() {
  // 通知队伍解散
  gateway::protocol::MessageTeamDismissNotify notify;
  this->BroadcastMessage(notify,
      gateway::protocol::MessageType::MESSAGE_TEAM_DISMISS_NOTIFY);

  // 通知GameServer队伍中的玩家退出组队
  session::protocol::MessageTeamSynchronize sync_game;
  sync_game.__set_team_(0);

  // 玩家队伍ID清0
  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    if(*iterator) {
      (*iterator)->SetTeamID(0);
      SessionActor *actor = (*iterator)->GetActor();
      if(actor) {
        sync_game.__set_actor_(actor->GetActorID());
        actor->SendGameMessage(sync_game,
            session::protocol::GameMessageType::MESSAGE_TEAM_SYNCHRONIZE);
      }
    }
  }

  // 管理器中删除队伍
  Team *team = TeamManager::GetInstance()->Remove(this->id_);
  if(team == NULL || team != this) {
    LOG_ERROR("从管理器中移除队伍(%lu)失败", this->id_);
    return false;
  }

  return true;
}

core::int32 Team::GetAverageLevel() {
  core::int32 level = 0;

  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    TeamActor *member = *iterator;
    if(member) {
      SessionActor *actor = member->GetActor();
      if(actor) {
        level += actor->GetLevel();
      }
    }
  }

  return level / this->members_.size();
}

void Team::Synchronize(TeamActor *actor) {
  if(actor == NULL) {
    return ;
  }

  gateway::protocol::MessageTeamSynchronize synchronize;
  synchronize.__set_team_id_(global::ToString(this->GetID()));
  synchronize.__set_captain_(global::ToString(this->GetID()));
  synchronize.__isset.members_ = true;

  gateway::protocol::TeamMemberFields field;
  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    TeamActor *member = *iterator;
    if(member) {
      SessionActor *session_actor = member->GetActor();
      if(session_actor == NULL) {
        LOG_ERROR("获取 SessionActor(%lu) 失败", member->GetID());
        return ;
      }
      field.__set_actor_(global::ToString(session_actor->GetActorID()));
      field.__set_name_(session_actor->GetName());
      field.__set_level_(session_actor->GetLevel());
      field.__set_map_(session_actor->GetMap());
      field.__set_x_(member->GetLocationX());
      field.__set_y_(member->GetLocationY());
      field.__set_current_hp_(member->GetCurrentHp());
      field.__set_max_hp_(member->GetMaxHp());
      field.__set_current_mp_(member->GetCurrentMp());
      field.__set_max_mp_(member->GetMaxMp());
      synchronize.members_.push_back(field);
    }
  }

  actor->SendMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_TEAM_SYNCHRONIZE);
}

void Team::SynchronizeBuff() {
  MemberVector::iterator iterator1 = this->members_.begin();
  for(; iterator1 != this->members_.end(); ++iterator1) {
    TeamActor *member = *iterator1;
    if(member) {
      core::uint32 number = 1;
      MemberVector::iterator iterator2 = this->members_.begin();
      for(; iterator2 != this->members_.end(); ++iterator2) {
        TeamActor *teammate = *iterator2;
        if(teammate && teammate != member &&
            teammate->GetMap() == member->GetMap()) {
          ++number;
        }
      }
      const TeamCell *cell = Configure::GetInstance()->GetTeamCell(number);
      if(cell == NULL) {
        continue;
      }
      member->SynchronizeBuff(cell->buff_);
    }
  }
}

void Team::BroadcastMessage(gateway::protocol::MessageType::type type,
    const char *message, size_t size) {
  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    if(*iterator) {
      (*iterator)->SendMessage(type, message, size);
    }
  }
}

}  // namespace team

}  // namespace server

}  // namespace session

