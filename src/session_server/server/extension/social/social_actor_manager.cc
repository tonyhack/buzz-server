#include "session_server/server/extension/social/social_actor_manager.h"

#include <algorithm>

#include "global/logging.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_social_actor_login_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_social_event_types.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_pool.h"
#include "session_server/server/extension/social/homeland_battle_manager.h"

#define SLOT_COUNT 20
#define SAVE_INTERVAL 200
#define SAVE_COUNT 100

namespace session {

namespace server {

namespace social {

bool SocialActorManager::Initialize() {
  this->max_offline_task_id_ = 0;
  this->real_time_level_actors_.resize(SLOT_COUNT);

  // 注册一个定时器，用于保存待保存的玩家数据
  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_SAVE,
      SAVE_INTERVAL, -1, this, "SocialActorManager::Initialize");

  return true;
}

void SocialActorManager::Finalize() {
  // 删除数据保存定时器
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_SAVE, this);

  for (ActorHashmap::iterator iter = this->actors_.begin();
       iter != this->actors_.end(); ++iter) {
    iter->second->Finalize();
    SocialActorPool::GetInstance()->Deallocate(iter->second);
  }
  this->actors_.clear();

  for (ActorHashmap::iterator iter = this->cached_actors_.begin();
       iter != this->cached_actors_.end(); ++iter) {
    iter->second->Finalize();
    SocialActorPool::GetInstance()->Deallocate(iter->second);
  }
  this->cached_actors_.clear();
}

static core::int32 GetLevelSlot(core::int32 level) {
  if (0 == level) {
    return 0;
  }

  core::int32 slot = (level - 1) / MISC_CONF()->contacts_recommend_level_range_;
  if (slot > SLOT_COUNT - 1) {
    slot = SLOT_COUNT - 1;
  } else if (slot < 0) {
    slot = 0;
  }
  return slot;
}

void SocialActorManager::AddToLevelPool(core::uint64 id) {
  // 加入等级池
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(id);
  if (brief_actor != NULL) {
    core::int32 slot = GetLevelSlot(brief_actor->GetLevel());
    this->real_time_level_actors_[slot].insert(id);
  } else {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", id);
  }
}

void SocialActorManager::RemoveFromLevelPool(core::uint64 id) {
  // 移出等级池
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(id);
  if (brief_actor != NULL) {
    core::int32 slot = GetLevelSlot(brief_actor->GetLevel());
    this->real_time_level_actors_[slot].erase(id);
  } else {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", id);
  }
}

SocialActor *SocialActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool SocialActorManager::Add(SocialActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetActorID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetActorID(), actor));
  this->AddToLevelPool(actor->GetActorID());
  HomelandBattleManager::GetInstance()->AddToBattleLadder(actor);

  return true;
}

SocialActor *SocialActorManager::Remove(core::uint64 id) {
  SocialActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
    this->RemoveFromLevelPool(id);
  }

  return actor;
}

SocialActor *SocialActorManager::GetFromCache(core::uint64 id) {
  ActorHashmap::iterator iter = this->cached_actors_.find(id);
  if (iter != this->cached_actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool SocialActorManager::AddToCache(SocialActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->cached_actors_.find(actor->GetActorID()) != this->cached_actors_.end()) {
    return false;
  }

  this->cached_actors_.insert(std::make_pair(actor->GetActorID(), actor));
  this->AddToLevelPool(actor->GetActorID());
  HomelandBattleManager::GetInstance()->AddToBattleLadder(actor);

  return true;
}

SocialActor *SocialActorManager::RemoveFromCache(core::uint64 id) {
  SocialActor *actor = NULL;

  ActorHashmap::iterator iter = this->cached_actors_.find(id);
  if (iter != this->cached_actors_.end()) {
    actor = iter->second;
    this->cached_actors_.erase(iter);
    this->RemoveFromLevelPool(id);
  }

  return actor;
}

SocialActor *SocialActorManager::GetFromAll(core::uint64 id) {
  SocialActor *actor = this->Get(id);
  if (actor != NULL) {
    return actor;
  }

  return this->GetFromCache(id);
}

void SocialActorManager::OfflineLoad(core::uint64 id, OfflineTask offline_task) {
  if (offline_task) {
    this->offline_tasks_[++this->max_offline_task_id_] = offline_task;
  }

  // 向数据库请求数据
  database::protocol::StorageSocialActorLoginRequest request;
  request.__set_actor_id_(id);
  request.__set_offline_load_(true);
  if (offline_task) {
    request.__set_offline_task_id_(this->max_offline_task_id_);
  }
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_SOCIAL_ACTOR_LOGIN, id) == false) {
    LOG_ERROR("Send StorageSocialActorLoginRequest to database server failed.");
    return;
  }
}

SocialActorManager::OfflineTask SocialActorManager::RemoveOfflineTask(core::uint64 offline_task_id) {
  OfflineTaskHashmap::iterator iter = this->offline_tasks_.find(offline_task_id);
  if (iter == this->offline_tasks_.end()) {
    return NULL;
  } else {
    OfflineTask task = iter->second;
    this->offline_tasks_.erase(iter);
    return task;
  }
}

void SocialActorManager::AddToPendingSaveQueue(core::uint64 id) {
  this->pending_save_queue_.push_back(id);
}

const SocialActorManager::ActorIDSet *SocialActorManager::GetActorsNearLevel(
    core::int32 level) {
  if (0 == level) {
    return NULL;
  }

  core::int32 slot = (level - 1) / MISC_CONF()->contacts_recommend_level_range_;
  if (slot < 0 || slot > SLOT_COUNT - 1) {
    return NULL;
  }

  return &this->real_time_level_actors_[slot];
}

void SocialActorManager::OnActorLevelup(core::uint64 id,
    core::int32 old_level, core::int32 new_level) {
  SocialActor *social_actor = this->Get(id);
  if (NULL == social_actor) {
      LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
      return;
  }

  // 发送信息更新事件
  {
    event::EventSocialContactsInfoUpdate event;
    event.__set_actor_id_(id);
    event.__set_level_(new_level);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SOCIAL_CONTACTS_INFO_UPDATE, id,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
  {
    event::EventSocialEnemyInfoUpdate event; 
    event.__set_actor_id_(id);
    event.__set_level_(new_level);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SOCIAL_ENEMY_INFO_UPDATE, id,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
  {
    event::EventHomelandInvaderInfoUpdate event;
    event.__set_actor_id_(id);
    event.__set_level_(new_level);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_HOMELAND_INVADER_INFO_UPDATE, id,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }

  // 更新等级池
  core::int32 slot = GetLevelSlot(old_level);
  this->real_time_level_actors_[slot].erase(id);
  slot = GetLevelSlot(new_level);
  this->real_time_level_actors_[slot].insert(id);
}

void SocialActorManager::DailyClean() {
  // 在线玩家每日清理
  for (ActorHashmap::iterator iter = this->actors_.begin();
       iter != this->actors_.end(); ++iter) {
    SocialActor *actor = iter->second;
    if (NULL == actor) {
      continue;
    }
    actor->DailyClean();
  }

  // 离线玩家每日清理
  for (ActorHashmap::iterator iter = this->cached_actors_.begin();
       iter != this->cached_actors_.end(); ++iter) {
    SocialActor *actor = iter->second;
    if (NULL == actor) {
      continue;
    }
    actor->DailyClean();
    this->AddToPendingSaveQueue(actor->GetActorID());
  }
}

void SocialActorManager::PendingSaveActor() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  size_t save_count = std::min((size_t)SAVE_COUNT, this->pending_save_queue_.size());
  for (size_t i = 0; i < save_count; ++i) {
    SocialActor *social_actor = this->GetFromCache(this->pending_save_queue_[i]);
    if (NULL == social_actor) {
      LOG_WARNING("Get SocialActor(%lu) from SocialActorManager cache failed.",
                  this->pending_save_queue_[i]);
      continue;
    }
    time_t last_save_time = social_actor->GetLastSaveTime();
    social_actor->Save();

    // 移除72小时未登陆, 20小时未修改的玩家
    if (social_actor->GetLastLogoutTime() + 72 * 60 * 60 < now &&
        last_save_time + 20 * 60 * 60  < now) {
      HomelandBattleManager::GetInstance()->RemoveFromBattleLadder(social_actor->GetActorID());
      this->RemoveFromCache(this->pending_save_queue_[i]);
      social_actor->Finalize();
      SocialActorPool::GetInstance()->Deallocate(social_actor);
    }
  }

  // 从队列中移除玩家ID
  for (size_t i = 0; i < save_count; ++i) {
    this->pending_save_queue_.pop_front();
  }
}

void SocialActorManager::OnTimer(core::uint32 id) {
  if (TIMER_ID_SAVE == id) {
    this->PendingSaveActor();
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

