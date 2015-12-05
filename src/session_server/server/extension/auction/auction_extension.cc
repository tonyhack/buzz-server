#include "session_server/server/extension/auction/auction_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "session_server/server/extension/auction/auction_transaction_manager.h"
#include "session_server/server/extension/auction/auction_transaction_pool.h"
#include "session_server/server/extension/auction/auction_actor_manager.h"
#include "session_server/server/extension/auction/auction_actor_pool.h"

namespace session {

namespace server {

namespace auction {

AuctionExtension::AuctionExtension() {}
AuctionExtension::~AuctionExtension() {}

bool AuctionExtension::Initialize() {
  if (AuctionTransactionPool::GetInstance() == NULL) {
    LOG_ERROR("Init AuctionTransactionPool singleton failed.");
    return false;
  }

  if (AuctionTransactionManager::GetInstance() == NULL) {
    LOG_ERROR("Init AuctionTransactionManager singleton failed.");
    return false;
  }

  if (AuctionActorPool::GetInstance() == NULL) {
    LOG_ERROR("Init AuctionActorPool singleton failed.");
    return false;
  }

  if (AuctionActorManager::GetInstance() == NULL) {
    LOG_ERROR("Init AuctionActorManager singleton failed.");
    return false;
  }

  // 获取服务器信息
  const global::SessionServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetSessionServer();
  if (NULL == information) {
    LOG_ERROR("Get SessionServerInformation failed."); 
    return false;
  }

  // 拍卖交易对象池初始化
  if (AuctionTransactionPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init AuctionTransactionPool failed.");
    return false;
  }

  // 拍卖交易管理器初始化
  if (AuctionTransactionManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Init AuctionTransactionManager failed.");
    return false;
  }

  // 拍卖玩家对象池初始化
  if (AuctionActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init AuctionActorPool failed.");
    return false;
  }

  // 拍卖玩家管理器初始化
  if (AuctionActorManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Init AuctionActorManager failed.");
    return false;
  }

  // 事件回调初始化
  if (this->actor_login_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLoginEventHandler failed.");
    return false;
  }
  if (this->actor_logout_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLogoutEventHandler failed.");
    return false;
  }
  if (this->actor_save_event_.Initialize() == false) {
    LOG_ERROR("Init ActorSaveEventHandler failed.");
    return false;
  }
  if (this->actor_functionality_enable_event_.Initialize() == false) {
    LOG_ERROR("Init ActorFunctionalityEnableEventHandler failed.");
    return false;
  }
  if (this->database_server_logined_event_.Initialize() == false) {
    LOG_ERROR("Init DatabaseServerLoginedEventHandler failed.");
    return false;
  }

  // 消息回调初始化
  if (this->auction_message_handler_.Initialize() == false) {
    LOG_ERROR("Init AuctionMessageHandler failed.");
    return false;
  }
  if (this->game_auction_message_handler_.Initialize() == false) {
    LOG_ERROR("Init GameAuctionMessageHandler failed.");
    return false;
  }

  // 数据库回调初始化
  if (storage_auction_actor_login_.Initialize() == false) {
    LOG_ERROR("Init StorageAuctionActorLoginResponse failed.");
    return false;
  }

  return true;
}

void AuctionExtension::Finalize() {
  this->storage_auction_actor_login_.Finalize();
  this->game_auction_message_handler_.Finalize();
  this->auction_message_handler_.Finalize();
  this->database_server_logined_event_.Finalize();
  this->actor_functionality_enable_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  AuctionActorManager::GetInstance()->Finalize();
  AuctionActorPool::GetInstance()->Finalize();

  AuctionActorManager::GetInstance()->ReleaseInstance();
  AuctionActorPool::GetInstance()->ReleaseInstance();
}

void AuctionExtension::Start() {}
void AuctionExtension::Stop() {}

}  // namespace auction

}  // namespace server

}  // namespace session

