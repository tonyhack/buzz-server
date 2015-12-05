#include "session_server/server/extension/auction/storage_auction_actor_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_auction_actor_login_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/auction/auction_actor.h"
#include "session_server/server/extension/auction/auction_actor_manager.h"
#include "session_server/server/extension/auction/auction_actor_pool.h"

namespace session {

namespace server {

namespace auction {

StorageAuctionActorLoginResponse::StorageAuctionActorLoginResponse() {}
StorageAuctionActorLoginResponse::~StorageAuctionActorLoginResponse() {}

bool StorageAuctionActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_AUCTION_ACTOR_LOGIN,
      boost::bind(&StorageAuctionActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageAuctionActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_AUCTION_ACTOR_LOGIN);
}

void StorageAuctionActorLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageAuctionActorLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize StorageAuctionActorLoginRequest failed.");
    return;
  }
  database::protocol::StorageAuctionActorLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize StorageAuctionActorLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      response.result_ != database::protocol::AuctionActorLoginResult::SUCCESS) {
    LOG_ERROR("Call StorageAuctionActorLoginRequest failed");
    return;
  }

  // 是否是离线加载
  bool offline_load = (request.__isset.offline_load_ && request.offline_load_ == true);

  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(request.actor_id_);
  if (NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", request.actor_id_);
    return;
  }
  SessionActor *session_actor = NULL;
  AuctionActor *auction_actor = NULL;

  if (!offline_load) {
    // 从管理器中获取玩家对象
    session_actor = SessionActorManager::GetInstance()->GetActor(request.actor_id_);
    if (NULL == session_actor) {
      LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", request.actor_id_);
      return;
    }
    // 拍卖扩展是否已存在
    auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
    if (auction_actor != NULL) {
      LOG_ERROR("AuctionActor(%lu) already exists.", request.actor_id_);
      return;
    }
  } else {
    // 离线加载检查内存中是否已存在, 若存在直接回调离线任务
    auction_actor = AuctionActorManager::GetInstance()->GetFromAll(request.actor_id_);
    if (auction_actor != NULL) {
      // 回调离线任务 
      if (request.__isset.offline_task_id_) {
        AuctionActorManager::OfflineTask task =
          AuctionActorManager::GetInstance()->RemoveOfflineTask(request.offline_task_id_);
        if (task) {
          task();
        }
      }
      return;
    }
  }

  // 分配拍卖玩家对象
  auction_actor = AuctionActorPool::GetInstance()->Allocate();
  if (NULL == auction_actor) {
    LOG_ERROR("Allocate AuctionActor(%lu) from AuctionActorPool failed.", request.actor_id_);
    return;
  }

  // 初始化
  if (auction_actor->Initialize(brief_actor, response.field_) == false) {
    AuctionActorPool::GetInstance()->Deallocate(auction_actor);
    LOG_ERROR("Init AuctionActor(%lu) failed.", request.actor_id_);
    return;
  }

  if (offline_load) {
    // 加入离线列表
    if (AuctionActorManager::GetInstance()->AddToCache(auction_actor) == false) {
      auction_actor->Finalize();
      AuctionActorPool::GetInstance()->Deallocate(auction_actor);
      LOG_ERROR("Add AuctionActor(%lu) to AuctionActorManager failed.", request.actor_id_); 
      return;
    }
  } else {
    // 加入在线列表
    if (AuctionActorManager::GetInstance()->Add(auction_actor) == false) {
      auction_actor->Finalize();
      AuctionActorPool::GetInstance()->Deallocate(auction_actor);
      LOG_ERROR("Add AuctionActor(%lu) to AuctionActorManager failed.", request.actor_id_); 
      return;
    }
  }

  if (offline_load) {
    // 回调离线任务
    if (request.__isset.offline_task_id_) {
      AuctionActorManager::OfflineTask task =
        AuctionActorManager::GetInstance()->RemoveOfflineTask(request.offline_task_id_);
      if (task) {
        task();
      }
    }
  } else {
    // 上线
    auction_actor->Online(session_actor);
    // 发送加载完成事件
    auction_actor->SendLoadFinishEvent();
  }
}

}  // namespace auction

}  // namespace server

}  // namespace session

