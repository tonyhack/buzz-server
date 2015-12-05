//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-11 01:37:27.
// File name: scene_finish_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/scene/bar_finish_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_bar_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/load_gather_configure.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "global/logging.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace scene {

BarFinishEventHandler::BarFinishEventHandler() {}
BarFinishEventHandler::~BarFinishEventHandler() {}

bool BarFinishEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_bar_finish_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_BAR_FINISH,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&BarFinishEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void BarFinishEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_bar_finish_);
}

void BarFinishEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBarFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventBarFinish *event = (event::EventBarFinish *)message;
  if(event->type_ != entity::BarModuleType::BAR_TYPE_GATHER) {
    return ; 
  }

  // 得到玩家
  SceneActor *scene_actor =
    scene::SceneExtension::GetInstance()->GetSceneActor(event->actor_id_);
  if(scene_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到玩家游戏对象
  GameActor *game_actor = scene_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家没有找到");
    return ;
  }

  // 检测NPC 存不存在
  SceneNpc *scene_npc = SceneNpcManager::GetInstance()->Get(
      scene_actor->GetGatherNpc());
  if(scene_npc == NULL) {
    LOG_ERROR("采集NPC没有找到");
    this->NoticeGatherResult(game_actor, 0);
    return ;
  }

  // 得到NPC 所在的场景
  Scene *scene = scene_npc->GetScene();
  if(scene == NULL) {
    LOG_ERROR("场景中不存在此NPC[%lu]", scene_actor->GetGatherNpc());
    this->NoticeGatherResult(game_actor, 0);
    return ;
  }

  // 得到NPC 对象
  GameNpc *npc = scene_npc->GetGameNpc();
  if(npc == NULL) {
    LOG_ERROR("场景中不存在此NPC[%lu]", scene_actor->GetGatherNpc());
    this->NoticeGatherResult(game_actor, 0);
    return ;
  }

  // 得到配置
  const NpcCell *cell =
    game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
        npc->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("NPC 的配置没有找到[%d]", npc->GetTemplateID());
    this->NoticeGatherResult(game_actor, 0);
    return ;
  }
  if(cell->type_ != entity::NpcKind::TASK_GATHER  &&
      cell->type_ != entity::NpcKind::GATHER) {
    LOG_ERROR("采集的NPC类型的不对[%d]", cell->type_);
    this->NoticeGatherResult(game_actor, 0);
    return ;
  }

  // 采集类型
  if(cell->type_ == entity::NpcKind::GATHER) {
    const GatherCell *gather_cell = Configure::GetInstance()->GetGatherCell(
        npc->GetTemplateID());
    if(gather_cell == NULL) {
      LOG_ERROR("采集的配置没有找到 NPC [%d]", npc->GetTemplateID());
      this->NoticeGatherResult(game_actor, 0);
      return ;
    }
    if(gather_cell->template_id_ != 0) {
      request::RequestAddItem add_item;
      add_item.__set_actor_id_(event->actor_id_);
      add_item.__set_item_template_(gather_cell->template_id_);
      add_item.__set_number_(gather_cell->num_);
      add_item.__set_bind_(entity::BindRequireType::NOT_BIND);
      add_item.__set_simulate_(false);
      if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
            &add_item, sizeof(add_item)) != 0 || 
          request::AddItemResult::SUCCESS != add_item.result_ ) {
        LOG_ERROR("采集添加背包失败, 道具[%d], 错误[%d]", 
            gather_cell->template_id_, add_item.result_);
        this->NoticeGatherResult(game_actor, 0);
      }
    }

    // 请求加BUFF
    request::RequestBuffStart request;
    request.__set_src_entity_type_(entity::EntityType::TYPE_ACTOR);
    request.__set_src_entity_id_(game_actor->GetActorID());
    request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR); 
    request.__set_dest_entity_id_(game_actor->GetActorID());
    request.__set_buff_id_(gather_cell->buff_);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_BUFF_START, &request, sizeof(request)) == -1) {
      LOG_ERROR("请求BUFF(%d)失败", game_actor->GetActorID());
    }
  }

  this->NoticeGatherResult(game_actor, npc->GetTemplateID());

  // 广播采集事件
  event::EventSceneGather gather_event;
  gather_event.__set_npc_template_id_(npc->GetTemplateID());
  gather_event.__set_id_(event->actor_id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SCENE_GATHER, event->actor_id_, event::ChannelType::CHANNEL_ACTOR,
      &gather_event, sizeof(gather_event));
  if(cell->disappear_time_ != -1) {
    npc->SetAttribute(entity::RoleAoiFields::CURRENT_HP, 0);
  }

  scene_actor->SetGatherNpc(0);
}

void BarFinishEventHandler::NoticeGatherResult(GameActor *actor, 
    core::int32 npc) {
  if(actor != NULL) {
    // 通知客户端
    gateway::protocol::MessageSceneGatherResponse response;
    response.__set_npc_template_(global::ToString(npc));
    actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_SCENE_GATHER_RESPONSE);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game
