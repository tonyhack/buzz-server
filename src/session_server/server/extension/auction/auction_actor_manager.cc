#include "session_server/server/extension/auction/auction_actor_manager.h"

#include "global/logging.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_auction_actor_login_types.h"
#include "session_server/server/extension/auction/auction_actor.h"
#include "session_server/server/extension/auction/auction_actor_pool.h"

namespace session {

namespace server {

namespace auction {

bool AuctionActorManager::Initialize() {
  this->max_offline_task_id_ = 0;

  return true;
}

void AuctionActorManager::Finalize() {
  for (ActorHashmap::iterator iter = this->actors_.begin();
       iter != this->actors_.end(); ++iter) {
    iter->second->Finalize();
    AuctionActorPool::GetInstance()->Deallocate(iter->second);
  }
  this->actors_.clear();

  for (ActorHashmap::iterator iter = this->cached_actors_.begin();
       iter != this->cached_actors_.end(); ++iter) {
    iter->second->Finalize();
    AuctionActorPool::GetInstance()->Deallocate(iter->second);
  }
  this->cached_actors_.clear();
}

AuctionActor *AuctionActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool AuctionActorManager::Add(AuctionActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetActorID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetActorID(), actor));

  return true;
}

AuctionActor *AuctionActorManager::Remove(core::uint64 id) {
  AuctionActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
  }

  return actor;
}

AuctionActor *AuctionActorManager::GetFromCache(core::uint64 id) {
  ActorHashmap::iterator iter = this->cached_actors_.find(id);
  if (iter != this->cached_actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool AuctionActorManager::AddToCache(AuctionActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->cached_actors_.find(actor->GetActorID()) != this->cached_actors_.end()) {
    return false;
  }

  this->cached_actors_.insert(std::make_pair(actor->GetActorID(), actor));

  return true;
}

AuctionActor *AuctionActorManager::RemoveFromCache(core::uint64 id) {
  AuctionActor *actor = NULL;

  ActorHashmap::iterator iter = this->cached_actors_.find(id);
  if (iter != this->cached_actors_.end()) {
    actor = iter->second;
    this->cached_actors_.erase(iter);
  }

  return actor;
}

AuctionActor *AuctionActorManager::GetFromAll(core::uint64 id) {
  AuctionActor *actor = this->Get(id);
  if (actor != NULL) {
    return actor;
  }

  return this->GetFromCache(id);
}

void AuctionActorManager::OfflineLoad(core::uint64 id, OfflineTask offline_task) {
  if (offline_task) {
    this->offline_tasks_[++this->max_offline_task_id_] = offline_task;
  }

  // 向数据库请求数据
  database::protocol::StorageAuctionActorLoginRequest request;
  request.__set_actor_id_(id);
  request.__set_offline_load_(true);
  if (offline_task) {
    request.__set_offline_task_id_(this->max_offline_task_id_);
  }
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_AUCTION_ACTOR_LOGIN, id) == false) {
    LOG_ERROR("Send StorageAuctionActorLoginRequest to database server failed.");
    return;
  }
}

AuctionActorManager::OfflineTask AuctionActorManager::RemoveOfflineTask(
    core::uint64 offline_task_id) {
  OfflineTaskHashmap::iterator iter = this->offline_tasks_.find(offline_task_id);
  if (iter == this->offline_tasks_.end()) {
    return NULL;
  } else {
    OfflineTask task = iter->second;
    this->offline_tasks_.erase(iter);
    return task;
  }
}

void AuctionActorManager::SaveAndRemoveFromCache(core::uint64 id) {
  AuctionActor *actor = this->RemoveFromCache(id);
  if (NULL == actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager cache failed.", id);
    return;
  }

  actor->Save();
  actor->Finalize();
  AuctionActorPool::GetInstance()->Deallocate(actor);
}

}  // namespace auction

}  // namespace server

}  // namespace session

