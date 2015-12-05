//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-12 11:58:43.
// File name: npc_killed_event_handler.cc
//
// Description:
// Define class RoleKilledEventHandler.
//

#include "game_server/server/extension/task/role_killed_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/task_extension.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/game_npc.h"
#include "global/configure/configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

RoleKilledEventHandler::RoleKilledEventHandler() {}
RoleKilledEventHandler::~RoleKilledEventHandler() {}

bool RoleKilledEventHandler::Initialize() {
  // Follow 所有NPC被杀死事件
  this->npc_killed_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_KILL_NPC, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleKilledEventHandler::OnNpcEvent,
        this, _1, _2, _3, _4));

  // Follow 所有actor被杀死事件
  this->actor_killed_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_KILLED, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleKilledEventHandler::OnRoleEvent,
        this, _1, _2, _3, _4));
  return true;
}

void RoleKilledEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->npc_killed_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->actor_killed_);
}

void RoleKilledEventHandler::OnNpcEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorKillNpc) != size) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorKillNpc *event = (event::EventActorKillNpc *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, event->actor_);
    return ;
  }

  // 得到玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->actor_);
  if(game_actor == NULL) {
    LOG_ERROR("玩家[%lu]没有找到", event->actor_);
    return ;
  }

  // 得到NPC对象
  GameNpc *game_npc = GameNpcManager::GetInstance()->Get(event->npc_);
  if(game_npc == NULL) {
    LOG_ERROR("NPC(%lu)没有找到", event->npc_);
    return ;
  }
  bool excepts = false;

  //验证NPC是不是任务招出来的
  if(TaskExtension::GetInstance()->GetSummonNpcManager().GetNpcID(
        game_npc->GetTemplateID(), game_npc->GetScene()) == (core::uint64)event->npc_) {
    // 获取被杀死的NPC周围的玩家
    request::RequestSceneSelectTargets request;
    request.__set_type_(game_actor->GetType());
    request.__set_id_(game_actor->GetID());
    request.__set_area_type_(request::AreaType::AREA_CIRCLE);
    request.__set_max_count_(MISC_CONF()->summon_num_);
    request.__set_x_(game_npc->GetPosx());
    request.__set_y_(game_npc->GetPosy());

    request::CircleAreaCondition condition;
    condition.__set_radius_(MISC_CONF()->summon_radius_);
    request.__set_cond_circle_(condition);

    // 设置选择过滤条件
    core::uint32 select_mask = 0;
    // 实体类型
    select_mask |= request::SelectMaskType::ACTOR;
    request.__set_select_mask_(select_mask);

    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_SCENE_SELECT_TARGETS, &request,
          sizeof(request)) == -1) {
      LOG_ERROR("请求死亡NPC周围的玩家列表失败");
      return ;
    }
    std::vector<request::AreaRolePair> &actors = request.targets_; 
    for(std::vector<request::AreaRolePair>::const_iterator itr = actors.begin();
        itr != actors.end(); ++itr) {
      if(itr->type_ == entity::EntityType::TYPE_ACTOR) {
        GameActor *other_actor = GameActorManager::GetInstance()->GetActor(
            itr->id_);
        TaskActor *other_task = TaskActorManager::GetInstance()->Get(itr->id_);
        if(other_actor != NULL && other_task != NULL) {
          other_task->OnEvent(event::EventType::EVENT_ACTOR_KILL_NPC, message, size);
          if(other_actor->GetID() == game_actor->GetID()) {
            excepts = true;
          }
        }
      }
    }
    TaskExtension::GetInstance()->GetSummonNpcManager().RemoveNpc(
        game_npc->GetTemplateID(), game_npc->GetScene());
  }

  // 事件回调处理
  if(excepts == false) {
    actor->OnEvent(event::EventType::EVENT_ACTOR_KILL_NPC, message, size);
  }
}

void RoleKilledEventHandler::OnRoleEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)message;

  // 凶手如果不是玩家，则返回
  if(event->attacker_type_ != entity::EntityType::TYPE_ACTOR || event->type_
      != entity::EntityType::TYPE_NPC) {
    return ;
  }

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->attacker_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, event->attacker_id_);
    return ;
  }

  // 得到玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->attacker_id_);
  if(game_actor == NULL) {
    LOG_ERROR("玩家[%lu]没有找到", event->attacker_id_);
    return ;
  }

  actor->OnEvent(event::EventType::EVENT_ROLE_KILLED, message, size);
}


}  // namespace task

}  // namespace server

}  // namespace game

