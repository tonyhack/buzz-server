//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-18 16:52:37.
// File name: actor_message_handler.cc
//
// Description:
// Define class ActorMessageHandler.
//

#include "game_server/server/extension/scene/actor_message_handler.h"

#include <boost/bind.hpp>

#include "entity/bar_types.h"
#include "entity/npc_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/facade_request.h"
#include "game_server/server/extension/scene/jump_configure.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "game_server/server/extension/scene/scene_item.h"
#include "game_server/server/extension/scene/scene_item_manager.h"
#include "game_server/server/extension/scene/scene_item_pool.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/request/game_task_request_types.h"
#include "game_server/server/request/game_bar_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/common_functions.h"
#include "global/distance_calculator.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/types.h"
#include "global/configure/configure.h"

// ------------- [冷却扩展测试代码] ------------------
/*
// TODO: 删除这里的测试代码
#include "game_server/server/request/game_cooling_request_types.h"
#include "game_server/server/request/game_request_types.h"
//*/
// ------------- [冷却扩展测试代码] ------------------

namespace game {

namespace server {

namespace scene {

ActorMessageHandler::ActorMessageHandler() {}
ActorMessageHandler::~ActorMessageHandler() {}

bool ActorMessageHandler::Initialize() {
  // 玩家移动消息处理
  // MESSAGE_ACTOR_MOVE
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_MOVE,
      boost::bind(&ActorMessageHandler::OnMessageActorMove, this, _1, _2, _3));

  // MESSAGE_SCENE_REQUEST_JUMP
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_REQUEST_JUMP,
      boost::bind(&ActorMessageHandler::OnMessageRequestJump, this, _1, _2, _3));

  // MESSAGE_ACTOR_CHOOSE_REVIVE_MODE
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageActorChooseReviveMode, this, _1, _2, _3));

  // MESSAGE_SCENE_GATHER 
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_GATHER,
      boost::bind(&ActorMessageHandler::OnMessageActorGather, this, _1, _2, _3));

  // MESSAGE_SCENE_PICK_UP_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_PICK_UP_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessagePickUpRequest, this, _1, _2, _3));

  // MESSAGE_SCENE_ZAZEN_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_ZAZEN_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageZazenRequest, this, _1, _2, _3));

  // MESSAGE_SCENE_CANCEL_ZAZEN_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_CANCEL_ZAZEN_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageCancelZazenRequest, this, _1, _2, _3));

  return true;
}

void ActorMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_MOVE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_REQUEST_JUMP);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_GATHER);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_PICK_UP_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_ZAZEN_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_CANCEL_ZAZEN_REQUEST);
}

void ActorMessageHandler::OnMessageActorMove(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 从玩家管理器中获取玩家指针
  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%llu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 从场景管理器中获取对应场景指针
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%llu) 所在场景失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageActorMove message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  core::uint32 x = 0, y = 0;
  actor->GetPosition(x, y);

  core::uint8 offset_x = 0, offset_y = 0;
  actor->GetPosOffset(offset_x, offset_y);

  // 移动
  if(scene->Walk(actor, message.dir_, message.offset_x_, message.offset_y_) == false) {
    // 失败拉回
    scene->Relocation(actor, x, y, offset_x, offset_y, false);
    LOG_ERROR("玩家(%lu) 移动失败，被定位到(%d, %d)", id, x, y);
  } else {
    actor->GetPosition(x, y);
  }
}

void ActorMessageHandler::OnMessageRequestJump(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 从玩家管理器中获取玩家指针
  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%llu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 从场景管理器中获取对应场景指针
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%llu) 所在场景失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageSceneRequestJump message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_SCENE_REQUEST_JUMP from actor(%lu, %s), npc_id_(%s)",
      actor->GetActorID(), actor->GetName().c_str(), message.npc_id_.c_str());

  // 获取NPC
  GameNpc *npc = GameNpcManager::GetInstance()->Get(atol(message.npc_id_.c_str()));
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 获取 GameNpc(%s) 失败",
        __FILE__, __LINE__, __FUNCTION__, message.npc_id_.c_str());
    return ;
  }

  if(npc->GetNpcType() != entity::NpcKind::TELEPORTER) {
    global::LogError("%s:%d (%s) 非跳转类NPC无效",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  const JumpCell *cell = Configure::GetInstance()->GetJump(npc->GetTemplateID());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) NPC(%u) 没有配置跳转",
        __FILE__, __LINE__, __FUNCTION__, npc->GetTemplateID());
    return ;
  }

  if(npc->GetScene() != actor->GetScene()) {
    global::LogError("%s:%d (%s) 非同场景不能跳转",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 计算距离
  core::uint32 src_x = 0, src_y = 0, dest_x = 0, dest_y = 0;
  actor->GetPosition(src_x, src_y);
  npc->GetPosition(dest_x, dest_y);
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(src_x,
      src_y, dest_x, dest_y);
  if(distance > global::kMaxTeleporterDistance) {
    global::LogError("%s:%d (%s) 距离跳转NPC过远",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 跳转
  if(scene->JumpMap(actor, cell->dest_map_, cell->dest_x_, cell->dest_y_) == false) {
    global::LogError("%s:%d (%s) 玩家(%lu,%s) 跳转场景(%u,%u,%u) 失败",
        __FILE__, __LINE__, __FUNCTION__, actor->GetID(), actor->GetName().c_str(),
        cell->dest_map_, cell->dest_x_, cell->dest_y_);
    return ;
  }
}

void ActorMessageHandler::OnMessageActorChooseReviveMode(core::uint64 id, 
    const char *data, size_t size) {

  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageSceneActorChooseReviveModeRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  SceneActor *scene_actor =
    scene::SceneExtension::GetInstance()->GetSceneActor(id);
  if(scene_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家选择复活失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  bool result = scene_actor->ChooseReviveMode(message.revive_type_, message.__isset.buy_ ? message.buy_: false);
  if(result == false) {
    LOG_ERROR("复活失败(%d)", message.revive_type_);
    return ;
  }

  gateway::protocol::MessageSceneActorChooseReviveModeResponse response;
  response.__set_revive_type_(message.revive_type_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_RESPONSE);
}

void ActorMessageHandler::OnMessageActorGather(core::uint64 id, 
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageSceneGather message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  SceneActor *scene_actor =
    scene::SceneExtension::GetInstance()->GetSceneActor(id);
  if(scene_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到玩家对象
  GameActor *game_actor = scene_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到[%lu]", id);
    return ;
  }

  // 检测NPC 存不存在
  SceneNpc *scene_npc = SceneNpcManager::GetInstance()->Get(
      atol(message.npc_.c_str()));
  if(scene_npc == NULL) {
    LOG_ERROR("采集NPC没有找到[%s]", message.npc_.c_str());
    return ;
  }

  // 得到NPC 对象
  GameNpc *npc = scene_npc->GetGameNpc();
  if(npc == NULL) {
    LOG_ERROR("NPC对象没有找到");
    return ;
  }

  // 距离判断
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      game_actor->GetPosx(), game_actor->GetPosy(), npc->GetPosx(), npc->GetPosy());
  if(distance >= Configure::GetInstance()->kGatherDistance_) {
    global::LogError("%s:%d (%s) 采集时，与NPC距离过远",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到配置
  const NpcCell *cell =
    game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
        npc->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("NPC 的配置没有找到[%d]", npc->GetTemplateID());
    return ;
  }
  if(cell->type_ != entity::NpcKind::TASK_GATHER &&
      cell->type_ != entity::NpcKind::GATHER) {
    LOG_ERROR("采集的NPC类型的不对, npc[%d]", npc->GetTemplateID());
    return ;
  }
  if(cell->type_ == entity::NpcKind::TASK_GATHER) {
    // 检测有没有任务采集
    request::RequestTaskCheckGather request;
    request.__set_npc_template_(npc->GetTemplateID());
    request.__set_actor_id_(id);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_TASK_CHECK_GATHER,
          &request, sizeof(request)) != 0) {
      LOG_ERROR("没有对应的采集任务 npc[%d]", npc->GetTemplateID());
      return ;
    }
  }

  // 请求进度条
  request::RequestBarStart request;
  request.__set_actor_id_(id);
  request.__set_bar_id_(cell->bar_id_);
  request.__set_type_(entity::BarModuleType::BAR_TYPE_GATHER);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BAR_START, &request, sizeof(request)) != 0) {
    LOG_ERROR("安装进度条失败[%d]", cell->bar_id_);
    return ;
  }
  scene_actor->SetGatherNpc(atol(message.npc_.c_str()));
}

void ActorMessageHandler::OnMessagePickUpRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 解析消息
  gateway::protocol::MessageScenePickUpRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", actor->GetScene());
    return;
  }

  core::uint64 item_id = atol(message.item_.c_str());
  SceneItem *item = SceneItemManager::GetInstance()->Get(item_id);
  if(item == NULL) {
    LOG_ERROR("获取 SceneItem(%lu) 失败", item_id);
    return;
  }

  if(actor->GetScene() != item->GetScene()) {
    LOG_ERROR("拾取道具时场景错误");
    return;
  }

  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      actor->GetPosx(), actor->GetPosy(), item->GetPosx(), item->GetPosy());
  if(distance == (core::uint32)-1) {
    LOG_ERROR("拾取道具时坐标错误");
    return;
  }

  if((core::int32)distance > MISC_CONF()->drop_pickup_distance_) {
    LOG_ERROR("拾取道具时坐标错误");
    return;
  }

  if(item->CheckOwner(id) == false) {
    LOG_ERROR("拾取道具归属错误");
    return;
  }

  entity::ResourceType::type type =
    (entity::ResourceType::type)item->GetAttribute(entity::SceneItemAoiFields::TYPE);
  if(type == entity::ResourceType::ITEM) {
    if(FacadeRequest::GetInstance()->CheckAddItem(id,
          item->GetAttribute(entity::SceneItemAoiFields::TEMPLATE),
          item->GetAttribute(entity::SceneItemAoiFields::NUMBER),
          (entity::BindRequireType::type)item->GetAttribute(entity::SceneItemAoiFields::BIND)) == false) {
      LOG_ERROR("背包空间不足");
      return;
    }
  }

  if(scene->RemoveEntity(item) == false) {
    LOG_ERROR("场景移除道具失败");
    return;
  }

  if(type == entity::ResourceType::ITEM) {
    if(FacadeRequest::GetInstance()->AddItem(id,
          item->GetAttribute(entity::SceneItemAoiFields::TEMPLATE),
          item->GetAttribute(entity::SceneItemAoiFields::NUMBER),
          (entity::BindRequireType::type)item->GetAttribute(entity::SceneItemAoiFields::BIND)) == false) {
      LOG_ERROR("拾取道具到背包失败");
    }
  } else if(type == entity::ResourceType::RESOURCE) {
    actor->AddResource((entity::ResourceID::type)item->GetAttribute(
        entity::SceneItemAoiFields::TEMPLATE),
        item->GetAttribute(entity::SceneItemAoiFields::NUMBER));
  }

  // 回收道具
  SceneItemManager::GetInstance()->Remove(item->GetID());
  item->Finalize();
  SceneItemPool::GetInstance()->Deallocate(item);

  // 回应消息
  gateway::protocol::MessageScenePickUpResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SCENE_PICK_UP_RESPONSE);
}


void ActorMessageHandler::OnMessageZazenRequest(core::uint64 id, const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 解析消息
  gateway::protocol::MessageSceneZazenRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  SceneActor *scene = scene::SceneExtension::GetInstance()->GetSceneActor(id);
  if(scene == NULL) {
    LOG_ERROR("玩家场景没有找到");
    return ;
  }
  ZazenActor &zazen = scene->GetZazen();
  if(zazen.StartTime() == false) {
    LOG_ERROR("玩家无法进入打坐状态");
    return;
  }

  gateway::protocol::MessageSceneZazenResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SCENE_ZAZEN_RESPONSE);

}

void ActorMessageHandler::OnMessageCancelZazenRequest(core::uint64 id, const char *data, size_t size) {

  // 解析消息
  gateway::protocol::MessageSceneCancelZazenRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  if(actor->GetBasicStatus() != entity::RoleBasicStatus::STATUS_ZAZEN) {
    LOG_ERROR("玩家不在打坐状态，无需取消");
    return ;
  }

  actor->SetBasicStatus(entity::RoleBasicStatus::STATUS_COMMON);

  gateway::protocol::MessageSceneCancelZazenResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SCENE_CANCEL_ZAZEN_RESPONSE);
}

}  // namespace scene

}  // namespace server

}  // namespace game

