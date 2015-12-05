//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-01 19:51:12.
// File name: extension_manager.cc
//
// Description:
// Define class ExtensionManager.
//

#include "session_server/server/extension_manager.h"

#include "global/logging.h"
#include "session_server/server/game_message_handler.h"

namespace session {

namespace server {

bool ExtensionManager::Initialize() {
  if(this->extension_load_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ExtensionLoadEventHandler 失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->actor_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ActorMessageHandler 失败");
    return false;
  }

  if(this->test_extension_.Initialize() == false) {
    LOG_ERROR("初始化测试扩展失败");
    return false;
  }

  if(this->gm_extension_.Initialize() == false) {
    LOG_ERROR("初始化GM扩展失败");
    return false;
  }

  if(this->scene_extension_.Initialize() == false) {
    LOG_ERROR("初始化场景扩展失败");
    return false;
  }

  if(this->chat_extension_.Initialize() == false) {
    LOG_ERROR("初始化聊天扩展失败");
    return false;
  }

  if(this->social_extension_.Initialize() == false) {
    LOG_ERROR("初始化社交扩展失败");
    return false;
  }

  if(this->team_extension_.Initialize() == false) {
    LOG_ERROR("初始化组队扩展失败");
    return false;
  }
  
  if(this->mail_extension_.Initialize() == false) {
   LOG_ERROR("初始化邮件扩展失败");
   return false;
  }

  if(this->guild_extension_.Initialize() == false) {
    LOG_ERROR("初始化公会扩展失败");
    return false;
  }

  if(this->playing_extension_.Initialize() == false) {
    LOG_ERROR("初始化副本扩展失败");
    return false;
  }
  
  if(this->auction_extension_.Initialize() == false) {
    LOG_ERROR("初始化拍卖扩展失败");
    return false;
  }

  if(item::ItemExtension::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化道具扩展失败");
    return false;
  }

  this->random_.Initialize();

  return true;
}

void ExtensionManager::Finalize() {
  this->auction_extension_.Finalize();
  this->playing_extension_.Finalize();
  this->mail_extension_.Finalize();
  this->guild_extension_.Finalize();
  this->team_extension_.Finalize();
  this->social_extension_.Finalize();
  this->chat_extension_.Finalize();
  this->scene_extension_.Finalize();
  this->gm_extension_.Finalize();
  this->test_extension_.Finalize();
  this->actor_message_handler_.Finalize();
  item::ItemExtension::GetInstance()->Finalize();

  this->extension_load_event_.Finalize();
}

void ExtensionManager::Start() {
  this->test_extension_.Start();
  this->gm_extension_.Start();
  this->scene_extension_.Start();
  this->chat_extension_.Start();
  this->social_extension_.Start();
  this->team_extension_.Start();
  this->mail_extension_.Start();
  this->guild_extension_.Start();
  this->playing_extension_.Start();
  this->auction_extension_.Start();
  item::ItemExtension::GetInstance()->Start();
}

void ExtensionManager::Stop() {
  this->auction_extension_.Stop();
  this->playing_extension_.Stop();
  this->guild_extension_.Stop();
  this->mail_extension_.Stop();
  this->team_extension_.Stop();
  this->social_extension_.Stop();
  this->chat_extension_.Stop();
  this->scene_extension_.Stop();
  this->gm_extension_.Stop();
  this->test_extension_.Stop();
  item::ItemExtension::GetInstance()->Stop();
}

void ExtensionManager::SetMessageHandler(session::protocol::GameMessageType::type type,
    const GameMessageHandler &handler) {
  session::server::GameMessageHandler::GetInstance()->SetMessageHandler(type, handler);
}

void ExtensionManager::ResetMessageHandler(session::protocol::GameMessageType::type type) {
  session::server::GameMessageHandler::GetInstance()->ResetMessageHandler(type);
}

void ExtensionManager::OnActorMessage(gateway::protocol::MessageType::type type,
    core::uint64 actor, const char *message, size_t size) {
  if(type < gateway::protocol::MessageType::MESSAGE_MIN ||
      type >= gateway::protocol::MessageType::MESSAGE_MAX ||
      this->actor_handler_[type].empty() == true) {
    LOG_ERROR("Error message type[%u]", type);
    return ;
  }

  this->actor_handler_[type](actor, message, size);
}

int ExtensionManager::Request(request::RequestType::type type, void *message,
    size_t size) {
  if(type < request::RequestType::REQUEST_MIN ||
      type >= request::RequestType::REQUEST_MAX ||
      this->request_handler_[type].empty() == true) {
    LOG_ERROR("Error request type[%u]", type);
    return -1;
  }

  return this->request_handler_[type](message, size);
}

}  // namespace server

}  // namespace session

