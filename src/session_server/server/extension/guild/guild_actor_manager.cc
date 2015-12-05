#include "session_server/server/extension/guild/guild_actor_manager.h"

#include <cstddef>
#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/server_configure.h"
#include "database_server/protocol/storage_guild_actor_save_types.h"
#include "session_server/server/session_server.h"
#include "session_server/server/sync_database_operation_manager.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_pool.h"

namespace session {

namespace server {

namespace guild {

bool GuildActorManager::Initialize() {
  SyncDatabaseOperationManager::GetInstance()->AddSaver(
    boost::bind(&GuildActorManager::SaveCachedGuildActors, this, _1));

  // 注册一个定时器，用于定时保存缓存数据
  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_SAVE,
      global::ServerConfigureSingleton::GetInstance().GetAutoSave().actor_ * 1000,
      -1, this, "GuildActorManager::Initialize");

  return true;
}

void GuildActorManager::Finalize() {
  // 删除数据保存定时器
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_SAVE, this);

  ActorHashmap::iterator iterator = this->actors_.begin();
  for (; iterator != this->actors_.begin(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      GuildActorPool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->actors_.clear();

  iterator = this->cached_actors_.begin();
  for (; iterator != this->cached_actors_.begin(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      GuildActorPool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->cached_actors_.clear();
}

GuildActor *GuildActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool GuildActorManager::Add(GuildActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetActorID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetActorID(), actor));

  return true;
}

GuildActor *GuildActorManager::Remove(core::uint64 id) {
  GuildActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
  }

  return actor;
}

GuildActor *GuildActorManager::GetFromCache(core::uint64 id) {
  ActorHashmap::iterator iter = this->cached_actors_.find(id);
  if (iter != this->cached_actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool GuildActorManager::AddToCache(GuildActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->cached_actors_.find(actor->GetActorID()) != this->cached_actors_.end()) {
    return false;
  }

  this->cached_actors_.insert(std::make_pair(actor->GetActorID(), actor));

  return true;
}

GuildActor *GuildActorManager::RemoveFromCache(core::uint64 id) {
  GuildActor *actor = NULL;

  ActorHashmap::iterator iter = this->cached_actors_.find(id);
  if (iter != this->cached_actors_.end()) {
    actor = iter->second;
    this->cached_actors_.erase(iter);
  }

  return actor;
}

GuildActor *GuildActorManager::GetFromAll(core::uint64 id) {
  GuildActor *actor = this->Get(id);
  if (actor != NULL) {
    return actor;
  }
  return this->GetFromCache(id);
}

void GuildActorManager::OnTimer(core::uint32 id) {
  ActorHashmap::iterator iter = this->cached_actors_.begin();
  for (; iter != this->cached_actors_.end(); ++iter) {
    GuildActor *actor = iter->second;
    if (actor != NULL) {
      actor->Save();

      // TODO 不在线的非公会待审核成员, 非公会成员移出缓存
    }
  }
}

void GuildActorManager::WeeklyClean() {
  ActorHashmap::iterator iter = this->actors_.begin();
  for (; iter != this->actors_.end(); ++iter) {
    GuildActor *actor = iter->second;
    if (actor != NULL) {
      actor->WeeklyClean();
    }
  }
}

bool GuildActorManager::SaveCachedGuildActors(mysqlpp::Connection &connection) {
  ActorHashmap::iterator iter = this->cached_actors_.begin();
  for (; iter!= this->cached_actors_.end(); ++iter) {
    GuildActor *actor = iter->second;
    if (NULL == actor) {
      continue;
    }
    if (actor->CheckDirty() == false) {
      continue;
    }

    database::protocol::StorageGuildActorField db_field;
    actor->Save(db_field);

    mysqlpp::Query query(&connection, false);
    query << "CALL GAME_GUILD_ACTOR_SAVE("
          << actor->GetActorID() << ", "
          << db_field.guild_id_ << ", " 
          << db_field.guild_position_ << ", "
          << db_field.current_contribution_value_ << ", "
          << db_field.total_contribution_value_ << ", "
          << db_field.gold_contribution_ << ", "
          << db_field.freedom_dollars_contribution_ << ", "
          << db_field.light_crystal_contribution_ << ", "
          << db_field.dark_crystal_contribution_ << ", "
          << db_field.holy_crystal_contribution_ << ", "
          << "'" << db_field.guild_skills_ << "', "
          << "'" << db_field.guild_buffs_ << "', "
          << "'" << db_field.awarded_guild_playings_ << "'"
          << ")";

    mysqlpp::StoreQueryResult result = query.store();

    if (query.errnum() > 0) {
      LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
                query.str().c_str(), query.error());
      return false;
    }
    while (query.more_results()) {
      query.store_next();
    }   
    query.reset();
  }

  return true;
}

}  // namespace guild

}  // namespace server

}  // namespace session

