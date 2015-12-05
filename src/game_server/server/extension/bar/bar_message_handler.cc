//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-11 00:45:43.
// File name: bar_message_handler.cc
//
// Description:
//

#include "game_server/server/extension/bar/bar_message_handler.h"

#include <boost/bind.hpp>

#include "entity/bar_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/bar/configure.h"
#include "game_server/server/extension/bar/bar_actor.h"
#include "game_server/server/extension/bar/bar_actor_manager.h"
#include "game_server/server/event/game_bar_event_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_bar_protocol_types.h"
#include "global/distance_calculator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace bar {

BarMessageHandler::BarMessageHandler() {}
BarMessageHandler::~BarMessageHandler() {}

bool BarMessageHandler::Initialize() {
  // 请求完成进度条
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_BAR_FINISH_REQUEST,
      boost::bind(&BarMessageHandler::OnMessageFinishBar, this, _1, _2, _3));

  return true;
}

void BarMessageHandler::Finalize() {
  // MESSAGE_BAR_FINISH_REQUEST
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_BAR_FINISH_REQUEST);
}

void BarMessageHandler::OnMessageFinishBar(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 获取进度条玩家对象
  BarActor *actor = BarActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageBarFinishRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_BAR_FINISH_REQUEST from actor(%lu, %s), bar_id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.bar_id_);

  // 得到配置信息
  const BarCell *cell = Configure::GetInstance()->GetCell(message.bar_id_);
  if(cell == NULL) {
    LOG_ERROR("进度条配置没有找到[%d]", message.bar_id_);
    return ;
  }
  
  // 校验进度条
  if(actor->CheckBar(message.bar_id_) == false) {
    LOG_ERROR("进度条校验失败");
    return ;
  }

  // 发送到客户端
  gateway::protocol::MessageBarFinishResponse response;
  response.__set_type_(actor->GetType());
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_BAR_FINISH_RESPONSE);

  // 广播进度条完成事件
  event::EventBarFinish event;
  event.__set_actor_id_(id);
  event.__set_type_(actor->GetType());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_BAR_FINISH, id, event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));

  // 重置进度条
  actor->StopBar();
}

}  // namespace bar

}  // namespace server

}  // namespace game

