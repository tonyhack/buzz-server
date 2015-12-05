//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 10:45:50.
// File name: extension_manager.h
//
// Description:
// Define class ExtensionManager.
//

#ifndef __GAME__SERVER__EXTENSION__MANAGER__H
#define __GAME__SERVER__EXTENSION__MANAGER__H

#include <boost/function.hpp>

#include "core/base/common.h"
#include "core/base/types.h"
#include "game_server/server/actor_extension_event_handler.h"
#include "game_server/server/actor_message_handler.h"
#include "game_server/server/extension_load_event_handler.h"
#include "game_server/server/login_session_event_handler.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/task_event_handler.h"
#include "game_server/server/vip_alter_event_handler.h"
#include "game_server/server/spirit_event_handler.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/random_generator.h"
#include "global/singleton_factory.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

// 扩展类型
struct ExtensionType {
  enum type {
    MIN = 0,

    COOLING = MIN,            // 冷却扩展
    EFFECT,                   // 效果扩展
    SCENE,                    // 场景扩展
    SKILL,                    // 技能扩展
    BUFF,                     // buf 扩展
    PYTHON,                   // python 扩展
    AI,                       // ai 扩展
    ITEM,                     // 道具扩展
    GM,                       // GM 扩展
    SOUL,                     // 英灵扩展
    TASK,                     // 任务扩展
    ATTR,                     // 属性扩展
    CHAT,                     // 聊天扩展
    MAIL,                     // 邮件扩展
    SOCIAL,                   // 社交扩展
    TEAM,                     // 组队扩展
    GUILD,                    // 公会扩展
    PLAYING,                  // 副本扩展
    BAR,                      // 进度条扩展
    AUCTION,                  // 拍卖扩展
    WELFARE,                  // 福利扩展
    TRANSPORT,                // 护送扩展

    MAX,
  };
};

class ExtensionManager : public global::SingletonFactory<ExtensionManager> {
  friend class global::SingletonFactory<ExtensionManager>;

 public:
  typedef boost::function<void (const char *, size_t)> MessageHandler;
  typedef boost::function<void (core::uint64, const char *, size_t)> ActorMessageHandler;
  typedef boost::function<int (void *, size_t)> RequestHandler;

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  // 设置从会话服务器发来的消息处理回调函数
  void SetMessageHandler(session::protocol::GameMessageType::type type,
      const MessageHandler &handler);
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

  game::server::ExtensionManager::ActorMessageHandler actor_handler_[gateway::protocol::MessageType::MESSAGE_MAX];
  RequestHandler request_handler_[request::RequestType::REQUEST_MAX];

  ExtensionLoadEventHandler extension_load_event_;
  LoginSessionEventHandler load_session_event_;
  TaskEventHandler task_event_;
  VipAlterEventHandler vip_alter_event_;
  ActorExtensionEventHandler actor_extension_event_;
  SpiritEventHandler spirit_event_;

  // 消息
  game::server::ActorMessageHandler actor_message_handler_;

  global::RandomGenerator random_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionManager);
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EXTENSION__MANAGER__H

