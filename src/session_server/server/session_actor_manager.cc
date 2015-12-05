//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 16:15:28.
// File name: session_actor_manager.cc
//
// Description:
// Define class SessionActorManager.
//

#include "session_server/server/session_actor_manager.h"

#include <boost/bind.hpp>

#include "entity/vip_types.h"
#include "global/channel.h"
#include "global/logging.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_pool.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/sync_database_operation_manager.h"

namespace session {

namespace server {

bool SessionActorManager::Initialize() {
  SyncDatabaseOperationManager::GetInstance()->AddLoader(
      boost::bind(&SessionActorManager::LoadActorBriefInfo, this, _1));

  return true;
}

void SessionActorManager::Finalize() {
  for (size_t i = 0; i < this->brief_actors_store_.size(); ++i) {
    delete this->brief_actors_store_[i];
    this->brief_actors_store_[i] = NULL;
  }
}

void SessionActorManager::AddBriefActor(const BriefActor &brief_actor) {
  BriefActor *old_brief_actor = this->GetBriefActor(brief_actor.GetID());
  if (NULL == old_brief_actor) {
    BriefActor *new_brief_actor = new (std::nothrow) BriefActor(brief_actor);
    if (NULL == new_brief_actor) {
      LOG_ERROR("Allocate BriefActor failed.");
      return;
    }

    this->brief_actors_store_.push_back(new_brief_actor);
    this->brief_actors_id_index_[new_brief_actor->GetID()] = 
      this->brief_actors_store_.size() - 1;
    this->brief_actors_name_index_[new_brief_actor->GetName()] =
      this->brief_actors_store_.size() - 1;
  } else {
    *old_brief_actor = brief_actor;
  }
}

BriefActor *SessionActorManager::GetBriefActor(core::uint64 id) {
  BriefActorIDIndexHashmap::iterator iterator = this->brief_actors_id_index_.find(id);
  if(iterator != this->brief_actors_id_index_.end()) {
    return this->brief_actors_store_[iterator->second];
  }
  return NULL;
}

BriefActor *SessionActorManager::GetBriefActor(const std::string &name) {
  BriefActorNameIndexHashmap::iterator iterator = this->brief_actors_name_index_.find(name);
  if(iterator != this->brief_actors_name_index_.end()) {
    return this->brief_actors_store_[iterator->second];
  }
  return NULL;
}

SessionActor *SessionActorManager::GetLoginActor(core::uint64 id) {
  ActorHashmap::iterator iterator = this->login_actors_.find(id);
  if(iterator != this->login_actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool SessionActorManager::AddLoginActor(SessionActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Actor is null.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  ActorHashmap::iterator iterator = this->login_actors_.find(actor->GetActorID());
  if(iterator != this->login_actors_.end()) {
    return false;
  }
  this->login_actors_.insert(std::make_pair(actor->GetActorID(), actor));
  return true;
}

SessionActor *SessionActorManager::RemoveLoginActor(core::uint64 id) {
  ActorHashmap::iterator iterator = this->login_actors_.find(id);
  if(iterator == this->login_actors_.end()) {
    return NULL;
  }

  SessionActor *actor = iterator->second;
  this->login_actors_.erase(iterator);

  return actor;
}

SessionActor *SessionActorManager::GetActor(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

SessionActor *SessionActorManager::GetActor(const std::string &name) {
  ActorIDHashmap::iterator iterator = this->ids_.find(name);
  if(iterator == this->ids_.end()) {
    return NULL;
  }
  return this->GetActor(iterator->second);
}

bool SessionActorManager::AddActor(SessionActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Actor is null.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  ActorHashmap::iterator iterator1 = this->actors_.find(actor->GetActorID());
  if(iterator1 != this->actors_.end()) {
    return false;
  }
  ActorIDHashmap::iterator iterator2 = this->ids_.find(actor->GetName());
  if(iterator2 != this->ids_.end()) {
    return false;
  }
  this->actors_.insert(std::make_pair(actor->GetActorID(), actor));
  this->ids_.insert(std::make_pair(actor->GetName(), actor->GetActorID()));
  return true;
}

SessionActor *SessionActorManager::RemoveActor(core::uint64 id) {
  ActorHashmap::iterator iterator1 = this->actors_.find(id);
  if(iterator1 == this->actors_.end()) {
    return NULL;
  }

  SessionActor *actor = iterator1->second;
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Actor(%lld) is null",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  ActorIDHashmap::iterator iterator2 = this->ids_.find(actor->GetName());
  if(iterator2 == this->ids_.end()) {
    global::LogError("%s:%d (%s) Actor[%lld] is not in id table.",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  this->actors_.erase(iterator1);
  this->ids_.erase(iterator2);

  return actor;
}

void SessionActorManager::KickActor(core::uint64 id) {
  SessionActor *actor = this->GetActor(id);
  if(NULL == actor) {
    actor = this->RemoveLoginActor(id);
    if(NULL == actor) {
      LOG_ERROR("Remove login actor[%lu] failed.", id);
      return;
    }   
  } else {
    // 更新 BriefActor 的玩家下线时间
    BriefActor *brief_actor = this->GetBriefActor(id);
    if(NULL == brief_actor) {
      LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", id);
    } else {
      brief_actor->SetLastLogoutTime(GlobalTimeTick::GetInstance()->TimeTick().GetRealSec());
    }

    actor->RequireLogout();
    this->RemoveActor(actor->GetActorID());
  }

  // 发送退出到游戏服务器
  SessionTerminal *game_terminal = SessionServerSingleton::GetInstance().GetTerminal(
      global::protocol::ServerType::GAME_SERVER, actor->GetGame());
  if(NULL == game_terminal) {
    LOG_ERROR("Get game server terminal[id=%d] failed.", actor->GetGame());
  } else {
    session::protocol::GameMessageActorLogout logout_message;
    logout_message.__set_actor_id_(actor->GetActorID());
    game_terminal->SendGameMessage(logout_message, protocol::GameMessageType::MESSAGE_ACTOR_LOGOUT);
  }

  // 删除回收actor
  actor->Finalize();
  SessionActorPool::GetInstance()->Deallocate(actor);
}

void SessionActorManager::KickActorsInGateway(core::uint32 gateway_id) {
  for(ActorHashmap::iterator iter = this->actors_.begin();
      iter != this->actors_.end();) {
    ActorHashmap::iterator iter2 = iter;
    ++iter2;

    SessionActor *actor = iter->second;
    if(actor != NULL && actor->GetGateway() == gateway_id) {
      this->KickActor(actor->GetActorID());
    }
    iter = iter2;
  }
}

bool SessionActorManager::LoadActorBriefInfo(mysqlpp::Connection &connection) {
  mysqlpp::Query query(&connection, false);

  query << "CALL GAME_BRIEF_ACTOR_LOAD()";
  mysqlpp::StoreQueryResult result = query.store();

  if(query.errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
              query.str().c_str(), query.error());
    return false;
  }
  while (query.more_results()) {
    query.store_next();
  }
  query.reset();

  const char *fields[] = {
    "id",
    "name",
    "level",
    "vocation",
    "gender",
    "map",
    "functionality_state",
    "axe_fight_score",
    "shoot_fight_score",
    "magic_fight_score",
    "last_logout_time",
    "vip_type",
    "vip_remainder_time",
  };

  for(size_t i = 0; i < sizeof(fields) / sizeof(char *); ++i) {
    if((size_t)result.field_num(fields[i]) != i) {
      LOG_ERROR("Database field order is incorrect.");
      return false;
    }
  }

  for(size_t row = 0; row < result.num_rows(); ++row) {
    // 处理VIP时间
    time_t vip_expire = BriefActor::VipTimeRemainderToExpired(atol(result[row][10].data()),
        atoi(result[row][12].data()));
    BriefActor *new_brief_actor =
      new (std::nothrow) BriefActor(atol(result[row][0].data()),
                                    result[row][1].data(),
                                    atoi(result[row][2].data()),
                                    (entity::VocationType::type)atoi(result[row][3].data()),
                                    (entity::GenderType::type)atoi(result[row][4].data()),
                                    atoi(result[row][5].data()),
                                    result[row][6].data(),
                                    atoi(result[row][7].data()),
                                    atoi(result[row][8].data()),
                                    atoi(result[row][9].data()),
                                    atol(result[row][10].data()),
                                    (entity::VipType::type)atoi(result[row][11].data()),
                                    vip_expire);
    if (NULL == new_brief_actor) {
      LOG_ERROR("Allocate BriefActor failed.");
      return false;
    }
    this->brief_actors_store_.push_back(new_brief_actor);
    this->brief_actors_id_index_[new_brief_actor->GetID()] = 
      this->brief_actors_store_.size() - 1;
    this->brief_actors_name_index_[new_brief_actor->GetName()] =
      this->brief_actors_store_.size() - 1;
  }

  return true;
}

}  // namespace server

}  // namespace session

