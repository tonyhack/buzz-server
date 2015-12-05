//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-01 19:34:07.
// File name: extension_manager.h
//
// Description:
// Define class ExtensionManager.
//

#ifndef __SESSION__SERVER__EXTENSION__MANAGER__H
#define __SESSION__SERVER__EXTENSION__MANAGER__H

#include <boost/function.hpp>

#include "core/base/common.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/random_generator.h"
#include "global/singleton_factory.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/actor_message_handler.h"
#include "session_server/server/extension/item/item_extension.h"
#include "session_server/server/extension_load_event_handler.h"
#include "session_server/server/extension/auction/auction_extension.h"
#include "session_server/server/extension/chat/chat_extension.h"
#include "session_server/server/extension/gm/gm_extension.h"
#include "session_server/server/extension/guild/guild_extension.h"
#include "session_server/server/extension/playing/playing_extension.h"
#include "session_server/server/extension/scene/scene_extension.h"
#include "session_server/server/extension/social/social_extension.h"
#include "session_server/server/extension/team/team_extension.h"
#include "session_server/server/extension/test/test_extension.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/extension/mail/mail_extension.h"

namespace session {

namespace server {

class SessionTerminal;

class ExtensionManager : public global::SingletonFactory<ExtensionManager> {
  friend class global::SingletonFactory<ExtensionManager>;

 public:
  typedef boost::function<void (const char *, size_t)> MessageHandler;
  typedef boost::function<void (core::uint64, const char *, size_t)> ActorMessageHandler;
  typedef boost::function<void (SessionTerminal *terminal, const char *, size_t)>
    GameMessageHandler;
  typedef boost::function<int (void *, size_t)> RequestHandler;

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  // 设置从GameServer发来的消息处理回调函数
  void SetMessageHandler(session::protocol::GameMessageType::type type,
      const GameMessageHandler &handler);
  void ResetMessageHandler(session::protocol::GameMessageType::type type);

  // 设置从玩家发送的消息处理回调函数
  inline void SetMessageHandler(gateway::protocol::MessageType::type type,
      const ActorMessageHandler &handler) {
    this->actor_handler_[type] = handler;
  }
  inline void ResetMessageHandler(gateway::protocol::MessageType::type type) {
    this->actor_handler_[type].clear();
  }

  // 玩家消息处理函数
  void OnActorMessage(gateway::protocol::MessageType::type type, core::uint64 actor,
      const char *message, size_t size);

  // 请求处理回调函数
  inline void SetRequestHandler(request::RequestType::type type,
      const RequestHandler &handler) {
    this->request_handler_[type] = handler;
  }
  inline void ResetRequestHandler(request::RequestType::type type) {
    this->request_handler_[type].clear();
  }

  // 返回 -1 表示请求没有处理者或处理失败，返回 0 表示处理成功
  int Request(request::RequestType::type type, void *message, size_t size);

  inline global::RandomGenerator &GetRandom() {
    return this->random_;
  }

 private:
  ExtensionManager() {}
  ~ExtensionManager() {}

  ActorMessageHandler actor_handler_[gateway::protocol::MessageType::MESSAGE_MAX];
  RequestHandler request_handler_[request::RequestType::REQUEST_MAX];

  ExtensionLoadEventHandler extension_load_event_;
  session::server::ActorMessageHandler actor_message_handler_;

  global::RandomGenerator random_;

  //////////////////////
  // 扩展功能
  //
  // 测试扩展
  test::TestExtension test_extension_;
  //
  // GM扩展
  gm::GmExtension gm_extension_;
  //
  // 场景扩展
  scene::SceneExtension scene_extension_;
  //
  // 聊天扩展
  chat::ChatExtension chat_extension_;
  //
  // 社交扩展
  social::SocialExtension social_extension_;
  //
  // 组队扩展
  team::TeamExtension team_extension_;
  //
  // 邮件扩展
  mail::MailExtension mail_extension_;
  //
  // 公会扩展
  guild::GuildExtension guild_extension_;
  //
  // 副本扩展
  playing::PlayingExtension playing_extension_;
  //
  // 拍卖扩展
  auction::AuctionExtension auction_extension_;
  //////////////////////////////////////

  DISALLOW_COPY_AND_ASSIGN(ExtensionManager);
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__EXTENSION__MANAGER__H

