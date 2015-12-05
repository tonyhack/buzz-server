//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-20 01:44:00.
// File name: actor_extension_event_handler.cc
//
// Description:
//
#include "game_server/server/actor_extension_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

ActorExtensionEventHandler::ActorExtensionEventHandler() {}
ActorExtensionEventHandler::~ActorExtensionEventHandler() {}

bool ActorExtensionEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_extension_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorExtensionEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorExtensionEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_extension_);
}

void ActorExtensionEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorExtensionLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  if(event->extension_type_ != ExtensionType::BUFF) {
    return ;
  }
  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)actor->GetAttribute(
          entity::ActorAoiFields::VIP), entity::VipPrivilegeType::ATTR_ATTRIBUTE_UP);
  if(vip_cell == NULL) {
    return ;
  }

  // 加BUFF
  request::RequestBuffStart request;
  request.__set_src_entity_type_(actor->GetType());
  request.__set_src_entity_id_(actor->GetID());
  request.__set_dest_entity_type_(actor->GetType());
  request.__set_dest_entity_id_(actor->GetID());
  request.__set_buff_id_(vip_cell->value1_);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_BUFF_START,
        &request, sizeof(request)) != 0 || request.succeed_ != true) {
    LOG_ERROR("请求 REQUEST_BUFF_START(%d) 失败", vip_cell->value1_);
    return ;
  }
}

}  // namespace server

}  // namespace game

