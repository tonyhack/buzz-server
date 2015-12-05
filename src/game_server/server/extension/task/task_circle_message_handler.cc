//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-14 19:43:30.
// File name: task_circle_message_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/task_circle_message_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "entity/inform_types.h"
#include "entity/resource_types.h"
#include "entity/task_types.h"
#include "game_server/server/event/game_task_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/extension/task/task_builder.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/facade_request.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

TaskCircleMessageHandler::TaskCircleMessageHandler() {}
TaskCircleMessageHandler::~TaskCircleMessageHandler() {}

bool TaskCircleMessageHandler::Initialize() {
  // MESSAGE_TASK_CIRCLE_OPT
  ExtensionManager::GetInstance()->SetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_CIRCLE_OPT_REQUEST,
    boost::bind(&TaskCircleMessageHandler::OnMessageOpt, this, _1, _2, _3));
    return true;
}

void TaskCircleMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_CIRCLE_OPT_REQUEST);
}

void TaskCircleMessageHandler::OnMessageOpt(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取任务玩家对象
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(id);
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象没有找到[%lu]", id);
    return ;
  }

  // 获取玩家游戏对象
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到[%lu]", id);
    return ;
  }
  // 消息解析
  gateway::protocol::MessageTaskCircleOptRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_CIRCLE_OPT_REQUEST from actor(%lu, %s), type_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.type_);

  // 验证功能有没有开启
  if(game_actor->CheckFunctionalityState(
        entity::FunctionalityType::TASK_CIRCLE) == false) {
    LOG_ERROR("循环任务功能尚未开启[%lu]", id);
    return ;
  }

  gateway::protocol::MessageTaskCircleOptResponse response;
  TaskCircle& circle = task_actor->GetCircle();

  if(message.type_ == entity::TaskCircleRefulshType::REFULSH_ROUND) {
    const TaskCircleCell *circle_cell = Configure::GetInstance()->GetCircleCell(
        circle.GetLvStage(), 0);
    if(circle_cell == NULL) {
      LOG_ERROR("玩家当前环数[%d]，当前等级段[%d]的配置出错",
          circle.GetCurrentCircle(),circle.GetLvStage());
      return ;
    }

    if(game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < 
        (circle_cell->jump_spend_gold_- (circle_cell->jump_spend_gold_/
            entity::g_constants_constants.kMaxCircleLimitLevel)*(circle.GetCurrentCircle()-1))) {
      LOG_ERROR("玩家钻石不足,需要消耗[%d]", circle_cell->jump_spend_gold_);
      return ;
    }
  } else if(message.type_ == entity::TaskCircleRefulshType::REFULSH_TASK_CONDITION) {
    // 玩家手动刷新条件
    const TaskCircleCell *circle_cell = Configure::GetInstance()->GetCircleCell(
        circle.GetLvStage(), circle.GetCurrentCircle());
    if(circle_cell == NULL) {
      LOG_ERROR("玩家当前环[%d]，当前等级段[%d]的配置出错",
          circle.GetCurrentCircle(),circle.GetLvStage());
      return ;
    }
    // 检测扣除刷新轮的资源
    entity::ActorClientFields::type spent_type;
    if(game_actor->GetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS) < 
        circle_cell->refresh_condition_) {
      if(game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < 
          circle_cell->refresh_condition_) {
        LOG_ERROR("玩家钻石不足,需要消耗[%d]", circle_cell->refresh_condition_);
        return ;
      }
      spent_type = entity::ActorClientFields::FREEDOM_DOLLARS;
    } else {
      spent_type = entity::ActorClientFields::RESTRICT_DOLLARS;
    }

    // 替换条件
    if(circle.ReplaceTask() == false) {
      LOG_ERROR("替换条件你失败");
      return ;
    }

    // 扣除资源
    if(spent_type == entity::ActorClientFields::RESTRICT_DOLLARS) {
      game_actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, 0 - circle_cell->refresh_condition_);
    } else if(spent_type == entity::ActorClientFields::FREEDOM_DOLLARS) {
      game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - circle_cell->refresh_condition_);
    } else {
      LOG_ERROR("扣除资源是出现错误");
    }
    response.__set_result_(circle.GetConditionID());

  } else if(message.type_ == entity::TaskCircleRefulshType::REFULSH_CIRCLE) {
    // 获取配置
    const TaskCircleCell *circle_cell = Configure::GetInstance()->GetCircleCell(
        circle.GetLvStage(), circle.GetCurrentCircle());
    if(circle_cell == NULL) {
      LOG_ERROR("玩家当前环[%d]，当前等级段[%d]的配置出错",
          circle.GetCurrentCircle(),circle.GetLvStage());
      return ;
    }
    // 检测扣除刷新轮的资源
    if(game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < 
        circle_cell->jump_spend_gold_) {
      LOG_ERROR("玩家钻石不足,需要消耗[%d]", circle_cell->jump_spend_gold_);
      return ;
    }
  } else if(message.type_ == entity::TaskCircleRefulshType::REQUEST_FINISH_ROUND) {
    // 请求完成本轮任务     
  } else if(message.type_ == entity::TaskCircleRefulshType::REQUEST_FINISH_CIRCLE) {
    // 请求完成环
    if(circle.CheckCircleFinish() == false) {
      LOG_ERROR("当前环数据有问题");
      return ;
    }
  } else if(message.type_ == entity::TaskCircleRefulshType::REQUEST_OPEN_ROUND) {
    if(circle.OpenRound() == false) {
      LOG_ERROR("当前不能开启循环任务");
      return ;
    } 
    circle.RoundSynchronize();
  } else {
    LOG_ERROR("输入参数有错误[%d]", (core::int32)message.type_);
    return ;
  }

  if(message.type_ == entity::TaskCircleRefulshType::REQUEST_FINISH_CIRCLE ||
      message.type_ == entity::TaskCircleRefulshType::REFULSH_CIRCLE) {
    // 获取配置
    const TaskCircleCell *circle_cell = Configure::GetInstance()->GetCircleCell(
        circle.GetLvStage(), circle.GetCurrentCircle());
    if(circle_cell == NULL) {
      LOG_ERROR("玩家当前环[%d]，当前等级段[%d]的配置出错",
          circle.GetCurrentCircle(),circle.GetLvStage());
      return ;
    }
    // 获取配置
    const ConditionCell * conditionCell = Configure::GetInstance()->GetConditionCell(
        circle.GetConditionID());
    if(conditionCell == NULL) {
      LOG_ERROR("任务条件ID（%d）", circle.GetConditionID());
      return ;
    }

    if(message.type_ == entity::TaskCircleRefulshType::REFULSH_CIRCLE) {
      game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - circle_cell->jump_spend_gold_);
    }
    // 奖励
    if(message.type_ == entity::TaskCircleRefulshType::REFULSH_CIRCLE) {
      this->Award(game_actor, circle_cell, NULL, false, 5);
    } else {
      this->Award(game_actor, circle_cell, conditionCell, false, 0);
    }
    // 发送新的环数据
    if(circle.NextCircle() == true) {
      response.__set_result_(circle.GetConditionID());    
    }

    // 广播任务完成事件
    event::EventTaskFinish event;
    event.__set_actor_id_(id);
    event.__set_task_id_(0);
    event.__set_type_(entity::TaskType::TASK_CIRCLE);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_TASK_FINISH, id, 
        event::ChannelType::CHANNEL_ACTOR,
        &event, sizeof(event));
    task_actor->OnEvent(event::EventType::EVENT_TASK_FINISH, &event, sizeof(event));
  }

  if(message.type_ == entity::TaskCircleRefulshType::REFULSH_ROUND ||
      message.type_ == entity::TaskCircleRefulshType::REQUEST_FINISH_ROUND) {
    // 获取配置
    const TaskCircleCell *circle_cell = Configure::GetInstance()->GetCircleCell(
        circle.GetLvStage(), 0);
    if(circle_cell == NULL) {
      return ;
    }

    // 检测完成与否
    if(message.type_ == entity::TaskCircleRefulshType::REQUEST_FINISH_ROUND) {
      if(circle.GetFlag() != (core::int32)entity::TaskCircleRoundState::FINISH) {
        LOG_ERROR("本轮任务尚未完成[%d]", circle.GetFlag());
        return ;
      }
    } 
    if(message.type_ == entity::TaskCircleRefulshType::REFULSH_ROUND) {
      // 获取配置
      core::int32 value = 0;
      for(core::int32 i = circle.GetCurrentCircle(); i <= 
          entity::g_constants_constants.kMaxCircleNum;++i) {
        const TaskCircleCell *cell = Configure::GetInstance()->GetCircleCell(
            circle.GetLvStage(), i);
        this->Award(game_actor, cell, NULL, false, 5);
        // 广播任务完成事件
        event::EventTaskFinish event;
        event.__set_actor_id_(id);
        event.__set_task_id_(0);
        event.__set_type_(entity::TaskType::TASK_CIRCLE);
        coresh::CommunicatorSingleton::GetInstance().Broadcast(
            event::EventType::EVENT_TASK_FINISH, id, 
            event::ChannelType::CHANNEL_ACTOR,
            &event, sizeof(event));
        task_actor->OnEvent(event::EventType::EVENT_TASK_FINISH, &event, sizeof(event));
      }
    }
    // 奖励
    this->Award(game_actor, circle_cell, NULL, true, 0);
    // 扣除刷新所需的资源
    if(message.type_ == entity::TaskCircleRefulshType::REFULSH_ROUND) {
      game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 -
          (circle_cell->jump_spend_gold_ - (circle_cell->jump_spend_gold_/
          entity::g_constants_constants.kMaxCircleNum)*(circle.GetCurrentCircle()-1)));
      // 公告
      gateway::protocol::MessageInformNotify notify;
      notify.__set_id_(entity::InformType::TASK_CIRCLE_QUICK_FINISH_ROUND);
      notify.params_.push_back(global::ToString(game_actor->GetActorID()));
      notify.params_.push_back(game_actor->GetName());
      GameServerSingleton::GetInstance().BroadcastMessage(notify,
          gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY); 
    }

    circle.CloseRound();
    circle.OpenTaskCircle();
    circle.RoundSynchronize();
  }
  // 返回客户端
  response.__set_type_(message.type_);
  game_actor->SendMessage(response, 
      gateway::protocol::MessageType::MESSAGE_TASK_CIRCLE_OPT_RESPONSE);
}

bool TaskCircleMessageHandler::Award(GameActor *game_actor, 
    const TaskCircleCell *cell, const ConditionCell * conditionCell,
    bool round, core::int32 number) {
  return FacadeRequest::GetInstance()->AwardsCircleTask(game_actor->GetActorID(),
      cell, conditionCell, round, number);
}

} // namespace task

} // namespace server

} // namespace game

