//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-21 02:23:01.
// File name: vip_alter_event_handler.cc
//
// Description:
//

#include "game_server/server/vip_alter_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "global/common_functions.h"
#include "global/configure/configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "entity/inform_types.h"

namespace game {

namespace server {

VipAlterEventHandler::VipAlterEventHandler() {}
VipAlterEventHandler::~VipAlterEventHandler() {}

bool VipAlterEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_VIP_CHANGE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&VipAlterEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void VipAlterEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void VipAlterEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorVipChange) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  LOG_DEBUG("vip变更");
  event::EventActorVipChange *event = (event::EventActorVipChange *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%llu) 登陆失败，无法从 GameActorManager 获取 GameActor",
        __FILE__, __LINE__, __FUNCTION__, event->actor_);
    return ;
  }

  // 公告
  if(event->new_type_ == entity::VipType::VIP_3) {
    gateway::protocol::MessageInformNotify notify;
    notify.__set_id_(entity::InformType::ACTOR_ALTER_VIP_LEVEL3);
    notify.params_.push_back(global::ToString(actor->GetActorID()));
    notify.params_.push_back(actor->GetName());
    notify.params_.push_back(global::ToString(event->new_type_));
    GameServerSingleton::GetInstance().BroadcastMessage(notify,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY); 
  }

  const global::configure::VipCell *old_cell = VIP_CONF()->GetCell(event->old_type_,
      entity::VipPrivilegeType::ATTR_ATTRIBUTE_UP);
  const global::configure::VipCell *new_cell = VIP_CONF()->GetCell(event->new_type_,
      entity::VipPrivilegeType::ATTR_ATTRIBUTE_UP);

  if (old_cell) {
    request::RequestBuffEnd request_end;
    request_end.__set_dest_entity_type_(actor->GetType());
    request_end.__set_dest_entity_id_(actor->GetID());
    request_end.__set_buff_id_(old_cell->value1_);
    if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_BUFF_END,
          &request_end, sizeof(request_end)) != 0 || request_end.succeed_ != true) {
      LOG_ERROR("请求 REQUEST_BUFF_START(%d) 失败", old_cell->value1_);
    }
  }

  if (new_cell) {
    request::RequestBuffStart request_start;
    request_start.__set_src_entity_type_(actor->GetType());
    request_start.__set_src_entity_id_(actor->GetID());
    request_start.__set_dest_entity_type_(actor->GetType());
    request_start.__set_dest_entity_id_(actor->GetID());
    request_start.__set_buff_id_(new_cell->value1_);
    if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_BUFF_START,
          &request_start, sizeof(request_start)) != 0 || request_start.succeed_ != true) {
      LOG_ERROR("请求 REQUEST_BUFF_START(%d) 失败", new_cell->value1_);
    }
  }

  // 处理世界跳转
  actor->SetWorldJumpVipNum(0);
  gateway::protocol::MessageWorldJumpVipNumSynchronize world_message;
  const global::configure::VipCell *world_jump_cell =
    VIP_CONF()->GetCell((entity::VipType::type)actor->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::SCENE_WORLD_JUMP_NUM);
  if(world_jump_cell != NULL) {
    world_message.__set_remainder_num_(world_jump_cell->value1_);
  } else {
    world_message.__set_remainder_num_(0);
  }
  actor->SendMessage(world_message,
      gateway::protocol::MessageType::MESSAGE_WORLD_JUMP_VIP_NUM_SYNCHRONIZE);
}

}  // namespace server

}  // namespace game

