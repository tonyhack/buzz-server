//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 17:14:13.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/task/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_transport_request_types.h"

namespace game {

namespace server {

namespace task {

bool FacadeRequest::AddItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, bool bind) {
  if(actor == 0 || item_template == 0 || number == 0) {
    return false;
  }
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind ? entity::BindRequireType::BIND : entity::BindRequireType::NOT_BIND);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ADD_ITEM, &request,
        sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::CheckPacketItemExist(core::uint64 actor,
    core::uint32 item_template, core::uint32 number, BindType::type bind) {
  request::RequestGetPacketItemNumber request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_bind_((entity::BindRequireType::type)bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER, &request,
        sizeof(request)) == 0 && (core::uint32)request.number_ >= number) {
    return true;
  }
  return false;
}

core::uint32 FacadeRequest::RemovePacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, BindType::type bind) {
  request::RequestRemoveItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_((entity::BindRequireType::type)bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_REMOVE_ITEM, &request,
        sizeof(request)) == 0) {
    return request.remove_number_;
  }
  return 0;
}

bool FacadeRequest::Awards(core::uint64 actor, const TaskAward *awards, core::int32 num) {
  if(awards == 0) {
    LOG_ERROR("奖励不存在");
    return false;
  }
  // 获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(actor);
  if(game_actor == NULL) {
    LOG_ERROR("玩家没有找到[%lu]", actor);
    return false;
  }
  // 请求检查并且加入道具
  request::RequestAddItems request;
  entity::AddItemFields field;
  for(core::int32 i = 0; i < num; ++i) {
    if(awards[i].id_ == 0) {
      continue ;
    }
    if(awards[i].type_ == entity::ResourceType::ITEM) {
      field.__set_bind_(awards[i].bind_ == 0 ? false : true);
      field.__set_item_template_(awards[i].id_);
      field.__set_number_(awards[i].number_);
      field.__set_intensify_level_(awards[i].intensify_level_);
      request.items_.push_back(field);
    }
  }
  request.__set_actor_id_(actor);
  request.__set_simulate_(false);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ADD_ITEMS, &request,
        sizeof(request)) != 0 || request.result_ != request::AddItemResult::SUCCESS) {
    LOG_ERROR("加入道具失败");
    return false;
  }
  // 添加资源
  for(core::int32 i = 0; i < num; ++i) {
    if(awards[i].type_ == entity::ResourceType::RESOURCE) {
      // 防沉迷惩罚
      core::int32 number = awards[i].number_;
      if(game_actor->GetAttribute(entity::ActorClientFields::FCM_STATUS) ==
          entity::FcmStatusType::HALF_INCOME) { 
        number /= 2;
      }
      game_actor->AddResource((entity::ResourceID::type)awards[i].id_, number); 
    }
  }

  return true;
}

bool FacadeRequest::Awards(core::uint64 actor, const std::vector<TaskAward> &awards) {
  // 获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(actor);
  if(game_actor == NULL) {
    LOG_ERROR("玩家没有找到[%lu]", actor);
    return false;
  }
  // 请求检查并且加入道具
  request::RequestAddItems request;
  entity::AddItemFields field;
  for(size_t i = 0; i < awards.size(); ++i) {
    if(awards[i].id_ == 0) {
      continue ;
    }
    if(awards[i].type_ == entity::ResourceType::ITEM) {
      field.__set_bind_(awards[i].bind_ == 0 ? false : true);
      field.__set_item_template_(awards[i].id_);
      field.__set_number_(awards[i].number_);
      field.__set_intensify_level_(awards[i].intensify_level_);
      request.items_.push_back(field);
    }
  }
  request.__set_actor_id_(actor);
  request.__set_simulate_(false);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ADD_ITEMS, &request,
        sizeof(request)) != 0 || request.result_ != request::AddItemResult::SUCCESS) {
    LOG_ERROR("加入道具失败");
    return false;
  }
  // 添加资源
  for(size_t i = 0; i < awards.size(); ++i) {
    if(awards[i].type_ == entity::ResourceType::RESOURCE) {
      // 防沉迷惩罚
      core::int32 number = awards[i].number_;
      if(game_actor->GetAttribute(entity::ActorClientFields::FCM_STATUS) ==
          entity::FcmStatusType::HALF_INCOME) { 
        number /= 2;
      }
      game_actor->AddResource((entity::ResourceID::type)awards[i].id_, number); 
    }
  }

  return true;

}

bool FacadeRequest::AwardsCircleTask(core::uint64 actor, const TaskCircleCell *cell, 
    const ConditionCell * conditionCell, bool round, core::int32 number) {
  if(cell == NULL) {
    return false;
  }
  // 获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(actor);
  if(game_actor == NULL) {
    LOG_ERROR("玩家没有找到[%lu]", actor);
    return false;
  }
  // 添加道具
  request::RequestAddItems request;
  entity::AddItemFields field;
  for(core::uint32 pos = 0; pos < cell->awards_.size(); ++pos) {
    if(cell->awards_[pos].type_ == entity::ResourceType::ITEM){
      // 请求增奖励的道具
      if(FacadeRequest::GetInstance()->AddItem(game_actor->GetActorID(), cell->awards_[pos].id_,
            cell->awards_[pos].number_, cell->awards_[pos].bind_ != 0) == false) {
        LOG_ERROR("玩家(%lu) 增加任务奖励道具(%u,%u)失败",game_actor->GetActorID(),
            cell->awards_[pos].id_, cell->awards_[pos].number_);
      }
      field.__set_bind_(cell->awards_[pos].bind_ == 0 ? false : true);
      field.__set_item_template_(cell->awards_[pos].id_);
      field.__set_number_(cell->awards_[pos].number_);
      field.__set_intensify_level_(cell->awards_[pos].intensify_level_);
      request.items_.push_back(field);
    }
  }
  request.__set_actor_id_(game_actor->GetActorID());
  request.__set_simulate_(false);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ADD_ITEMS, &request,
        sizeof(request)) != 0 || request.result_ != request::AddItemResult::SUCCESS) {
    LOG_ERROR("加入道具失败");
    return false;
  }
  // 添加资源
  core::int32 value = 0;
  for(core::uint32 pos = 0; pos < cell->awards_.size(); ++pos) {
    if(cell->awards_[pos].type_ == entity::ResourceType::RESOURCE) {
      if(round == true) {
        value = cell->awards_[pos].number_;
      } else {
        // 玩家循环任务资源计算公式: 资源数*星级系数*星级/100
        if(conditionCell != NULL) {
          value = cell->awards_[pos].number_ + 
            cell->awards_[pos].number_*conditionCell->star_*cell->star_coe_/100;
        } else {
          value = cell->awards_[pos].number_ + cell->awards_[pos].number_*number*cell->star_coe_/100;
        }
      }
      if(game_actor->GetAttribute(entity::ActorClientFields::FCM_STATUS) == 
          entity::FcmStatusType::HALF_INCOME) {
        value /=2;
      }
      game_actor->AddResource((entity::ResourceID::type)cell->awards_[pos].id_, value);
    }
  }

  return true;
}

bool FacadeRequest::CheckTransportAround(core::uint64 actor) {
  request::RequestTransportAround request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TRANSPORT_AROUND,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求 REQUEST_TRANSPORT_CURRENT_NPC 失败");
    return false;
  }

  return request.around_;
}

core::uint64 FacadeRequest::RequestCurrentTransportNpc(core::uint64 actor) {
  request::RequestTransportCurrentNpc request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TRANSPORT_CURRENT_NPC,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求 REQUEST_TRANSPORT_CURRENT_NPC 失败");
    return -1;
  }

  return request.transport_npc_;
}

core::uint64 FacadeRequest::RequestTransport(core::uint64 actor, core::int32 npc_template) {
  request::RequestTransportSummon request;
  request.__set_actor_(actor);
  request.__set_transport_npc_template_(npc_template);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TRANSPORT_SUMMON,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求 REQUEST_TRANSPORT_SUMMON 失败");
    return 0;
  }

  return request.transport_npc_;
}
bool FacadeRequest::RemoveTransport(core::uint64 actor) {
  request::RequestTransportRemove request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TRANSPORT_REMOVE,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求 REQUEST_TRANSPORT_REMOVE 失败");
    return false;
  }

  return true;
}

}  // namespace task

}  // namespace server

}  // namespace game

