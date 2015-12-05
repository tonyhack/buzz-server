#include "game_server/server/extension/auction/auction_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "game_server/server/extension/auction/facade_request.h"
#include "game_server/server/extension/auction/auction_actor_manager.h"
#include "game_server/server/extension/auction/auction_actor_pool.h"

namespace game {

namespace server {

namespace auction {

AuctionExtension::AuctionExtension() {} 
AuctionExtension::~AuctionExtension() {}

bool AuctionExtension::Initialize() {
  if (FacadeRequest::GetInstance() == NULL) {
    LOG_ERROR("Init FacadeRequest singleton failed.");
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

  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if (information == NULL) {
    LOG_ERROR("Get GameServerInformation failed.");
    return false;
  }

  // 拍卖玩家对象池初始化
  if (AuctionActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init AuctionActorPool failed.");
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

  // 消息回调初始化
  if (this->session_auction_message_handler_.Initialize() == false) {
    LOG_ERROR("Init SessionAuctionMessageHandler failed.");
    return false;
  }

  return true;
}

void AuctionExtension::Finalize() {
  this->session_auction_message_handler_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  AuctionActorPool::GetInstance()->Finalize();

  AuctionActorManager::GetInstance()->ReleaseInstance();
  AuctionActorPool::GetInstance()->ReleaseInstance();
  FacadeRequest::ReleaseInstance();
}

void AuctionExtension::Start() {}
void AuctionExtension::Stop() {}

}  // namespace auction

}  // namespace server

}  // namespace game

