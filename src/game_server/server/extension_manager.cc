//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 10:51:45.
// File name: extension_manager.cc
//
// Description:
// Define class ExtensionManager.
//

#include "game_server/server/extension_manager.h"

#include "game_server/server/extension/ai/ai_extension.h"
#include "game_server/server/extension/attr/attr_extension.h"
#include "game_server/server/extension/auction/auction_extension.h"
#include "game_server/server/extension/bar/bar_extension.h"
#include "game_server/server/extension/buff/buff_extension.h"
#include "game_server/server/extension/chat/chat_extension.h"
#include "game_server/server/extension/cooling/cooling_extension.h"
#include "game_server/server/extension/effect/effect_extension.h"
#include "game_server/server/extension/gm/gm_extension.h"
#include "game_server/server/extension/guild/guild_extension.h"
#include "game_server/server/extension/item/item_extension.h"
#include "game_server/server/extension/mail/mail_extension.h"
#include "game_server/server/extension/playing/playing_extension.h"
#include "game_server/server/extension/python/python_extension.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "game_server/server/extension/skill/skill_extension.h"
#include "game_server/server/extension/social/social_extension.h"
#include "game_server/server/extension/soul/soul_extension.h"
#include "game_server/server/extension/task/task_extension.h"
#include "game_server/server/extension/team/team_extension.h"
#include "game_server/server/extension/test/test_extension.h"
#include "game_server/server/extension/transport/transport_extension.h"
#include "game_server/server/extension/welfare/welfare_extension.h"
#include "game_server/server/game_server.h"
#include "game_server/server/session_message_handler.h"

namespace game {

namespace server {

bool ExtensionManager::Initialize() {
  this->random_.Initialize();

  if(this->extension_load_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化扩展模块登陆完成处理失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_session_event_.Initialize() == false) {
    LOG_ERROR("初始化登陆会话服务器处理失败.");
    return false;
  }
  if(this->task_event_.Initialize() == false) {
    LOG_ERROR("初始化任务相关的事件失败");
    return false;
  }
  if(this->vip_alter_event_.Initialize() == false) {
    LOG_ERROR("初始化 VipAlterEventHandler 事件失败");
    return false;
  }
  if(this->spirit_event_.Initialize() == false) {
    LOG_ERROR("初始化 SpiritEventHandler 事件失败");
    return false;
  }
  if(this->actor_extension_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorExtensionEventHandler 事件失败");
    return false;
  }
  if(this->actor_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ActorMessageHandler 事件失败");
    return false;
  }

  if(scene::SceneExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化场景功能扩展失败.",
      __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(cooling::CoolingExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化冷却功能扩展失败.",
      __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(skill::SkillExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化技能功能扩展失败.",
      __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(effect::EffectExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化效果功能扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

#ifdef _TEST_GAME_EXTENSION
  if(test::TestExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化测试扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
#endif  // _TEST_GAME_EXTENSION

  if(buff::BuffExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化buff功能扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(ai::AiExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ai 扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(item::ItemExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 item 扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(soul::SoulExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 英灵 扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(gm::GmExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 gm 扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(task::TaskExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 任务 扩展失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(attr::AttrExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 属性 扩展失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(chat::ChatExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 聊天 扩展失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(mail::MailExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 邮件 扩展失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(social::SocialExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 社交 扩展失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(team::TeamExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 组队 扩展失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(guild::GuildExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 公会 扩展失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(playing::PlayingExtension::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 副本 扩展失败");
    return false;
  }

  if(python::PythonExtension::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 python 扩展失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  
  if(bar::BarExtension::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 进度条 扩展失败");
    return false;
  }

  if(auction::AuctionExtension::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 拍卖扩展失败");
    return false;
  }

  if(welfare::WelfareExtension::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 福利系统 失败");
    return false;
  }

  if (transport::TransportExtension::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 运送扩展 失败");
    return false;
  }

  return true;
}

void ExtensionManager::Finalize() {
#ifdef _TEST_GAME_EXTENSION
  test::TestExtension::GetInstance()->Finalize();
#endif  // _TEST_GAME_EXTENSION
  auction::AuctionExtension::GetInstance()->Finalize();
  bar::BarExtension::GetInstance()->Finalize();
  python::PythonExtension::GetInstance()->Finalize();
  playing::PlayingExtension::GetInstance()->Finalize();
  guild::GuildExtension::GetInstance()->Finalize();
  team::TeamExtension::GetInstance()->Finalize();
  social::SocialExtension::GetInstance()->Finalize();
  mail::MailExtension::GetInstance()->Finalize();
  chat::ChatExtension::GetInstance()->Finalize();
  attr::AttrExtension::GetInstance()->Finalize();
  task::TaskExtension::GetInstance()->Finalize();
  gm::GmExtension::GetInstance()->Finalize();
  soul::SoulExtension::GetInstance()->Finalize();
  item::ItemExtension::GetInstance()->Finalize();
  ai::AiExtension::GetInstance()->Finalize();
  buff::BuffExtension::GetInstance()->Finalize();
  effect::EffectExtension::GetInstance()->Finalize();
  skill::SkillExtension::GetInstance()->Finalize();
  cooling::CoolingExtension::GetInstance()->Finalize();
  scene::SceneExtension::GetInstance()->Finalize();
  welfare::WelfareExtension::GetInstance()->Finalize();
  transport::TransportExtension::GetInstance()->Finalize();

  this->load_session_event_.Finalize();
  this->extension_load_event_.Finalize();
  this->task_event_.Finalize();
  this->vip_alter_event_.Finalize();
  this->spirit_event_.Finalize();
  this->actor_extension_event_.Finalize();
  this->actor_message_handler_.Finalize();
}

void ExtensionManager::Start() {
#ifdef _TEST_GAME_EXTENSION
  test::TestExtension::GetInstance()->Start();
#endif  // _TEST_GAME_EXTENSION
  python::PythonExtension::GetInstance()->Start();
  playing::PlayingExtension::GetInstance()->Start();
  guild::GuildExtension::GetInstance()->Start();
  team::TeamExtension::GetInstance()->Start();
  social::SocialExtension::GetInstance()->Start();
  mail::MailExtension::GetInstance()->Start();
  chat::ChatExtension::GetInstance()->Start();
  attr::AttrExtension::GetInstance()->Start();
  task::TaskExtension::GetInstance()->Start();
  gm::GmExtension::GetInstance()->Start();
  soul::SoulExtension::GetInstance()->Start();
  item::ItemExtension::GetInstance()->Start();
  ai::AiExtension::GetInstance()->Start();
  buff::BuffExtension::GetInstance()->Start();
  effect::EffectExtension::GetInstance()->Start();
  skill::SkillExtension::GetInstance()->Start();
  cooling::CoolingExtension::GetInstance()->Start();
  scene::SceneExtension::GetInstance()->Start();
  bar::BarExtension::GetInstance()->Start();
  auction::AuctionExtension::GetInstance()->Start();
  welfare::WelfareExtension::GetInstance()->Start();
  transport::TransportExtension::GetInstance()->Start();
}

void ExtensionManager::Stop() {
#ifdef _TEST_GAME_EXTENSION
  test::TestExtension::GetInstance()->Stop();
#endif  // _TEST_GAME_EXTENSION
  playing::PlayingExtension::GetInstance()->Stop();
  guild::GuildExtension::GetInstance()->Stop();
  team::TeamExtension::GetInstance()->Stop();
  mail::MailExtension::GetInstance()->Stop();
  chat::ChatExtension::GetInstance()->Stop();
  attr::AttrExtension::GetInstance()->Stop();
  task::TaskExtension::GetInstance()->Stop();
  gm::GmExtension::GetInstance()->Stop();
  soul::SoulExtension::GetInstance()->Stop();
  item::ItemExtension::GetInstance()->Stop();
  ai::AiExtension::GetInstance()->Stop();
  python::PythonExtension::GetInstance()->Stop();
  buff::BuffExtension::GetInstance()->Stop();
  effect::EffectExtension::GetInstance()->Stop();
  skill::SkillExtension::GetInstance()->Stop();
  cooling::CoolingExtension::GetInstance()->Stop();
  scene::SceneExtension::GetInstance()->Stop();
  bar::BarExtension::GetInstance()->Stop();
  auction::AuctionExtension::GetInstance()->Stop();
  welfare::WelfareExtension::GetInstance()->Stop();
  transport::TransportExtension::GetInstance()->Stop();
}

void ExtensionManager::SetMessageHandler(session::protocol::GameMessageType::type type,
    const MessageHandler &handler) {
  SessionMessageHandler *session =
    GameServerSingleton::GetInstance().GetSessionMessageHandler();
  if(session == NULL) {
    global::LogError("%s:%d (%s) Get session message handler failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  session->SetMessageHandler(type, handler);
}

void ExtensionManager::ResetMessageHandler(session::protocol::GameMessageType::type type) {
  SessionMessageHandler *session =
    GameServerSingleton::GetInstance().GetSessionMessageHandler();
  if(session == NULL) {
    global::LogError("%s:%d (%s) Get session message handler failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  session->ResetMessageHandler(type);
}

void ExtensionManager::OnActorMessage(gateway::protocol::MessageType::type type,
    core::uint64 actor, const char *message, size_t size) {
  if((type < gateway::protocol::MessageType::MESSAGE_MIN ||
        type >= gateway::protocol::MessageType::MESSAGE_MAX) ||
      this->actor_handler_[type].empty() == true) {
    global::LogError("%s:%d (%s) Error message type[%d]",
        __FILE__, __LINE__, __FUNCTION__, type);
    return ;
  }

  this->actor_handler_[type](actor, message, size);
}

int ExtensionManager::Request(request::RequestType::type type,
    void *message, size_t size) {
  if((type < request::RequestType::REQUEST_MIN ||
        type >= request::RequestType::REQUEST_MAX) ||
      this->request_handler_[type].empty() == true) {
    global::LogError("%s:%d (%s) Error request type[%d]",
        __FILE__, __LINE__, __FUNCTION__, type);
    return -1;
  }
  return this->request_handler_[type](message, size);
}

}  // namespace server

}  // namespace game

