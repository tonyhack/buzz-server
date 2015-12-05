//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 14:01:19.
// File name: request_soul_operate.cc
//
// Description:
// Define class RequestSoulOperate.
//

#include "game_server/server/extension/soul/request_soul_operate.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_soul_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/soul.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "game_server/server/extension/soul/soul_pool.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/facade_request.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/request/game_soul_request_types.h"
#include "gateway_server/protocol/gateway_soul_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

RequestSoulOperate::RequestSoulOperate() {}
RequestSoulOperate::~RequestSoulOperate() {}

bool RequestSoulOperate::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_CHECK_SOUL_EXIST,
      boost::bind(&RequestSoulOperate::OnRequestCheckExist, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ADD_SOUL,
      boost::bind(&RequestSoulOperate::OnRequestAddSoul, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SOUL_GET,
      boost::bind(&RequestSoulOperate::OnRequestSoulGet, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SOUL_GET_ENERGY_STATUS,
      boost::bind(&RequestSoulOperate::RequestSoulGetEnergyStatus, this, _1, _2));

  return true;
}

void RequestSoulOperate::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_CHECK_SOUL_EXIST);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ADD_SOUL);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SOUL_GET);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SOUL_GET_ENERGY_STATUS);
}

int RequestSoulOperate::OnRequestCheckExist(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestCheckSoulExit) != size) {
    global::LogError("%s:%d (%s) 请求检测英灵时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestCheckSoulExit *request = (request::RequestCheckSoulExit *)message;

  // 英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求检测英灵时，玩家对象(%lu)不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_);
    return -1;
  }

  // 待增加的英灵是否存在
  Soul *soul = actor->GetSoul(request->soul_id_);
  if(soul != NULL) {
    request->__set_result_(true);
  } else {
    request->__set_result_(false);
  }

  return 0;
}

int RequestSoulOperate::OnRequestAddSoul(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestAddSoul) != size) {
    global::LogError("%s:%d 请求增加英灵时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestAddSoul *request = (request::RequestAddSoul *)message;

  // 英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求增加英灵时，玩家对象(%lu)不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_);
    return -1;
  }

  // 模块是否开启
  if(actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return -1;
  }

  // 获取英灵配置
  const SoulCell *soul_cell =
    Configure::GetInstance()->GetSoulCell(request->soul_id_);
  if (soul_cell == NULL) {
    LOG_ERROR("获取英灵配置(%d) 失败", request->soul_id_);
    return -1;
  }

  if (actor->GetSoul(request->soul_id_) != NULL) {
    LOG_ERROR("不能解锁已解锁过的英灵");
    return -1;
  }

  // 解锁条件
  if (soul_cell->unlock_type_ == entity::SoulUnlockType::ITEM) {
    core::int32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(request->actor_,
        soul_cell->unlock_param1_, soul_cell->unlock_param2_, entity::BindRequireType::BOTH);
    if (delete_number != soul_cell->unlock_param2_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，删除道具数量(%d) 不足(%d)",
          request->actor_, request->soul_id_, delete_number, soul_cell->unlock_param2_);
      return -1;
    }
  } else if (soul_cell->unlock_type_ == entity::SoulUnlockType::SPIRIT_LEVEL) {
    core::int32 spirit_level = FacadeRequest::GetInstance()->GetSpiritLevel(request->actor_);
    if (spirit_level < soul_cell->unlock_param1_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，spirit_level(%d) 不足(%d)",
          request->actor_, request->soul_id_, spirit_level, soul_cell->unlock_param1_);
      return -1;
    }
  } else if (soul_cell->unlock_type_ == entity::SoulUnlockType::SWORD_LEVEL) {
    core::int32 sword_level = FacadeRequest::GetInstance()->GetSwordStep(request->actor_);
    if (sword_level < soul_cell->unlock_param1_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，sword_level(%d) 不足(%d)",
          request->actor_, request->soul_id_, sword_level, soul_cell->unlock_param1_);
      return -1;
    }
  } else if (soul_cell->unlock_type_ == entity::SoulUnlockType::NOBLE_LEVEL) {
    core::int32 noble_level = FacadeRequest::GetInstance()->GetNobleLevel(request->actor_);
    if (noble_level < soul_cell->unlock_param1_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，noble_level(%d) 不足(%d)",
          request->actor_, request->soul_id_, noble_level, soul_cell->unlock_param1_);
      return -1;
    }
  } else {
    LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，解锁类型(%d) 错误",
        request->actor_, request->soul_id_, soul_cell->unlock_type_);
    return -1;
  }

  Soul *soul = SoulPool::GetInstance()->Allocate();
  if (soul == NULL) {
    LOG_ERROR("玩家(%lu) 分配 Soul(%lu) 失败", request->actor_, request->soul_id_);
    return -1;
  }

  if (soul->Initialize(actor, request->soul_id_) == false) {
    SoulPool::GetInstance()->Deallocate(soul);
    LOG_ERROR("玩家(%lu) Soul(%lu) 初始化失败", request->actor_, request->soul_id_);
    return -1;
  }

  soul->SetStep(0);
  soul->SetLevel(1);

  if (soul_cell->common_skill_ > 0) {
    FacadeRequest::GetInstance()->AddActorSkill(request->actor_, soul_cell->common_skill_, 1, false);
    soul->SetCommonSkillLevel(1);
  }
  if (soul_cell->appear_skill_ > 0) {
    FacadeRequest::GetInstance()->AddActorSkill(request->actor_, soul_cell->appear_skill_, 1, false);
    soul->SetAppearSkillLevel(1);
  }
  if (soul_cell->soul_skill_ > 0) {
    FacadeRequest::GetInstance()->AddActorSkill(request->actor_, soul_cell->soul_skill_, 1, false);
    soul->SetSoulSkillLevel(1);
  }

  if (actor->AddSoul(soul) == false) {
    soul->Finalize();
    SoulPool::GetInstance()->Deallocate(soul);
    LOG_ERROR("玩家(%lu) Soul(%lu) 加入列表失败", request->actor_, request->soul_id_);
    return -1;
  }

  actor->UpdateSoulFightScore();

  soul->Synchronize();

  event::EventSoulAdd event;
  event.__set_actor_(actor->GetID());
  event.__set_soul_(soul->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOUL_ADD, actor->GetID(), event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));

  return 0;
}

int RequestSoulOperate::OnRequestSoulGet(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSoulGet) != size) {
    global::LogError("%s:%d (%s) 获取玩家英灵基本信息，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSoulGet *request = (request::RequestSoulGet *)message;

  // 英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取玩家英灵基本信息，玩家对象(%lu)不存在", request->actor_);
    return -1;
  }

  // 模块是否开启
  if(actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("获取玩家英灵基本信息，英灵功能未开启");
    return 0;
  }

  if(request->__isset.soul_id_ == true) {
    // 英灵对象
    Soul *soul = actor->GetSoul(request->soul_id_);
    if(soul == NULL) {
      LOG_ERROR("获取玩家英灵基本信息，玩家(%lu)英灵(%u)不存在",
          request->actor_, request->soul_id_);
      return -1;
    }
    // 配置
    const SoulCell *cell = Configure::GetInstance()->GetSoulCell(request->soul_id_);
    if(cell == NULL) {
      LOG_ERROR("获取玩家英灵基本信息，找不到英灵配置(%d)",
          request->soul_id_);
      return -1;
    }
    request::SoulFields field;
    field.__set_id_(request->soul_id_);
    field.__set_level_(soul->GetLevel());
    field.__set_appear_(cell->appear_skill_ != 0);
    request->souls_.push_back(field);
  } else {
    request::SoulFields field;
    const SoulActor::SoulMap &souls = actor->GetSouls();
    SoulActor::SoulMap::const_iterator iterator = souls.begin();
    for(; iterator != souls.end(); ++iterator) {
      if(iterator->second == NULL) {
        continue;
      }
      Soul *soul = iterator->second;
      const SoulCell *cell = Configure::GetInstance()->GetSoulCell(soul->GetID());
      if(cell == NULL) {
        LOG_ERROR("获取玩家英灵基本信息，找不到英灵配置(%d)",
            soul->GetID());
        return -1;
      }
      if(request->__isset.appear_ && ((request->appear_ == true && cell->appear_skill_ != 0) ||
            (request->appear_ == false && cell->appear_skill_ == 0))) {
        field.__set_id_(request->soul_id_);
        field.__set_level_(soul->GetLevel());
        field.__set_appear_(cell->appear_skill_ != 0);
        request->souls_.push_back(field);
      }
    }
  }

  return 0;
}

int RequestSoulOperate::RequestSoulGetEnergyStatus(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSoulGetEnergyStatus) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSoulGetEnergyStatus *request =
    (request::RequestSoulGetEnergyStatus *)message;

  // 英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", request->actor_);
    return -1;
  }

  // 模块是否开启
  if(actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("英灵功能未开启");
    return -1;
  }

  SoulEnergy *energy = actor->GetSoulEnergy();
  if (energy == NULL) {
    LOG_ERROR("获取 SoulEnergy(%lu) 失败", request->actor_);
    return -1;
  }

  EnergyStatus *current_status = energy->GetCurrentStatus();
  if (current_status == NULL) {
    LOG_ERROR("获取玩家(%lu) EnergyStatus 失败", request->actor_);
    return -1;
  }

  request->__set_status_(current_status->GetType());

  return 0;
}

}  // namespace soul

}  // namespace server

}  // namespace game

