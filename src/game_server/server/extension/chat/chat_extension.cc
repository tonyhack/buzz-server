#include "game_server/server/extension/chat/chat_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "game_server/server/extension/chat/configure.h"
#include "game_server/server/extension/chat/facade_request.h"
#include "game_server/server/extension/chat/chat_actor_manager.h"
#include "game_server/server/extension/chat/chat_actor_pool.h"

namespace game {

namespace server {

namespace chat {

ChatExtension::ChatExtension() {}
ChatExtension::~ChatExtension() {}

bool ChatExtension::Initialize() {
  if (Configure::GetInstance() == NULL) {
    LOG_ERROR("Init Configure singletion failed.");
    return false;
  }

  if (FacadeRequest::GetInstance() == NULL) {
    LOG_ERROR("Init FacadeRequest singleton failed.");
    return false;
  }

  if (ChatActorManager::GetInstance() == NULL) {
    LOG_ERROR("Init ChatActorManager singleton failed.");
    return false;
  }

  if (ChatActorPool::GetInstance() == NULL) {
    LOG_ERROR("Init ChatActorPool singleton failed.");
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

  // 加载配置文件
  if (Configure::GetInstance()->LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetChatFile()) == false) {
    LOG_ERROR("Load [chat extension] config file failed.");
    return false;
  }

  // 聊天玩家对象池初始化
  if (ChatActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init ChatActorPool falied.");
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
  if (this->chat_message_handler_.Initialize() == false) {
    LOG_ERROR("Init ChatMessageHandler failed.");
    return false;
  }

  return true;
}

void ChatExtension::Finalize() {
  this->chat_message_handler_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  ChatActorPool::GetInstance()->Finalize();

  ChatActorPool::ReleaseInstance();
  ChatActorManager::ReleaseInstance();
  FacadeRequest::ReleaseInstance();
  Configure::ReleaseInstance();
}

void ChatExtension::Start() {}
void ChatExtension::Stop() {}

}  // namespace chat

}  // namespace server

}  // namespace game

