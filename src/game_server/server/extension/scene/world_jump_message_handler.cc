//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-14 03:08:15.
// File name: world_jump_message_handler.cc
//
// Description:
//

#include "game_server/server/extension/scene/world_jump_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/facade_request.h"
#include "game_server/server/extension/scene/load_world_jump_configure.h"
#include "game_server/server/extension/scene/jump_configure.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_task_request_types.h"
#include "game_server/server/request/game_transport_request_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/configure/configure.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/types.h"

// ------------- [冷却扩展测试代码] ------------------
/*
// TODO: 删除这里的测试代码
#include "game_server/server/request/game_cooling_request_types.h"
#include "game_server/server/request/game_request_types.h"
*/
// ------------- [冷却扩展测试代码] ------------------

namespace game {

namespace server {

namespace scene {

WorldJumpMessageHandler::WorldJumpMessageHandler() {}
WorldJumpMessageHandler::~WorldJumpMessageHandler() {}

bool WorldJumpMessageHandler::Initialize() {
  // MESSAGE_SCENE_JUMP_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_JUMP_REQUEST,
      boost::bind(&WorldJumpMessageHandler::OnRequest, this, _1, _2, _3));
  return true;
}

void WorldJumpMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_JUMP_REQUEST);
}

void WorldJumpMessageHandler::OnRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 从玩家管理器中获取玩家指针
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 死亡状态不能跳
  if(game_actor->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    LOG_ERROR("玩家死亡，不能跳");
    return ;
  }

  // PVP状态不能跳
  SceneActor *scene_actor = SceneExtension::GetInstance()->GetSceneActor(id);
  if(scene_actor == NULL) {
    LOG_ERROR("玩家(%lu)场景对象没有找到", id);
    return ;
  }
  if(scene_actor->GetPVPFightingStatus() == true) {
    LOG_ERROR("玩家处于 PVP 状态，无法跳");
    return ;
  }

  // 如果是押镖状态，不能跳
  request::RequestTransportCurrentNpc request_transport;
  request_transport.__set_actor_(id);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TRANSPORT_CURRENT_NPC, &request_transport,
        sizeof(request_transport)) == -1) {
    LOG_ERROR("请求护送任务信息失败");
    return ;
  }
  if(request_transport.transport_npc_ != 0) {
    LOG_ERROR("护送状态下，不能跳");
    return ;
  }

  // 解析消息
  gateway::protocol::MessageSceneJumpRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(
        message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  core::int32 num = 0;
  const global::configure::VipCell *vip_cell = VIP_CONF()->GetCell(
      (entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP),
      entity::VipPrivilegeType::SCENE_WORLD_JUMP_NUM);
  if (vip_cell != NULL) {
    num = vip_cell->value1_;
  }
  if(num != MISC_CONF()->infinite_num_) {
    if(num <= game_actor->GetWorldJumpVipNum()) {
      // 删除道具
      if(FacadeRequest::GetInstance()->RemmoveItem(id, MISC_CONF()->world_jump_expend_item_id_,
            MISC_CONF()->world_jump_expend_item_num_, entity::BindRequireType::BOTH) == false) {
        LOG_ERROR("道具不足，无法跳转");
        return ;
      }
    }
  }

  // 获取要跳转的坐标值
  core::int32 map = 0;
  core::int32 x = 0;
  core::int32 y = 0;
  if(message.type_ == gateway::protocol::SceneJumpType::WORLD_JUMP) {
    const WorldJumpCell *cell = Configure::GetInstance()->GetWorldJumpCell(message.id_);
    if(cell == NULL) {
      LOG_ERROR("世界跳转没有找到(%d)", message.id_);
      return ;
    }
    map = cell->map_id_;
    x = cell->x_;
    y = cell->y_;
  } else if(message.type_ == gateway::protocol::SceneJumpType::TASK_ACCEPT_JUMP ||
      message.type_ == gateway::protocol::SceneJumpType::TASK_FINISH_JUMP) {
    request::RequestTaskAcceptAndFinishCoordinate request;
    request.__set_actor_id_(id);
    request.__set_type_(message.task_type_);
    request.__set_task_id_(message.id_);
    if(message.type_ == gateway::protocol::SceneJumpType::TASK_FINISH_JUMP) {
      request.__set_opt_type_(request::RequestTaskOptType::FINISH);
    } else {
      request.__set_opt_type_(request::RequestTaskOptType::ACCEPT);
    }
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_TASK_ACCEPT_AND_FINISH_COORDINATE, &request, sizeof(request)) == -1) {
      LOG_ERROR("任务坐标找不到");
      return ;
    }
    map = request.map_id_;
    x = request.x_;
    y = request.y_;
  } else if(message.type_ == gateway::protocol::SceneJumpType::TASK_CONDITION_JIMP) {
    request::RequestTaskConditionCoordinate request;
    request.__set_actor_id_(id);
    request.__set_type_(message.task_type_);
    request.__set_task_id_(message.id_);
    request.__set_condition_pos_(message.condition_pos_);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_TASK_CONDITION_COORDINATE, &request, sizeof(request)) == -1) {
      LOG_ERROR("任务条件坐标找不到");
      return ;
    }
    map = request.map_id_;
    x = request.x_;
    y = request.y_;
  }

  // 跳转
  if(this->JumpMap(game_actor, map, x, y) == false) {
    if(num != MISC_CONF()->infinite_num_ && num <= game_actor->GetWorldJumpVipNum()) {
      if(FacadeRequest::GetInstance()->AddItem(id, MISC_CONF()->world_jump_expend_item_id_,
            MISC_CONF()->world_jump_expend_item_num_, entity::BindRequireType::BOTH) == false) {
        LOG_ERROR("返还玩家道具(%d)失败", MISC_CONF()->world_jump_expend_item_id_);
        return ;
      }
    }
    return ;
  }

  game_actor->SetBasicStatus(entity::RoleBasicStatus::STATUS_COMMON);

  if(num != MISC_CONF()->infinite_num_ && num > game_actor->GetWorldJumpVipNum()) {
    game_actor->SetWorldJumpVipNum(game_actor->GetWorldJumpVipNum() + 1);
  }

  gateway::protocol::MessageSceneJumpRsponse response;
  response.__set_result_(true);
  response.__set_num_(num == MISC_CONF()->infinite_num_ ? MISC_CONF()->infinite_num_ : num -
      game_actor->GetWorldJumpVipNum());
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SCENE_JUMP_RESPONSE);
}

bool WorldJumpMessageHandler::JumpMap(GameActor *actor, core::int32 map, core::int32 x, core::int32 y) {
  if(actor == NULL) {
    return false;
  }
  // 从场景管理器中获取对应场景指针
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    LOG_ERROR("获取玩家(%lu)场景失败", actor->GetActorID());
    return false;
  }
  if(scene->JumpMap(actor, map, x, y) == false) {
    LOG_ERROR("玩家(%lu)跳转地图(%d)x(%d)y(%d)失败", actor->GetActorID(), map, x, y);
    return false;
  }
  return true;
}

}  // namespace scene

}  // namespace server

}  // namespace game

