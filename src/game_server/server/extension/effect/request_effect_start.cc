//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:10:23.
// File name: request_effect_start.cc
//
// Description:
// Define class RequestEffectStart.
//

#include "game_server/server/extension/effect/request_effect_start.h"

#include <boost/bind.hpp>

#include "entity/role_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/effect/effect_role.h"
#include "game_server/server/extension/effect/status_effect.h"
#include "game_server/server/extension/effect/effect_role_manager.h"
#include "game_server/server/request/game_effect_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace effect {

RequestEffectStart::RequestEffectStart() {}
RequestEffectStart::~RequestEffectStart() {}

bool RequestEffectStart::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_EFFECT_START,
      boost::bind(&RequestEffectStart::OnRequestEffectStart, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_EFFECT_COMMON_STATUS_START,
      boost::bind(&RequestEffectStart::OnRequestCommonStatusChange, this, _1, _2));
  return true;
}

void RequestEffectStart::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_EFFECT_START);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_EFFECT_COMMON_STATUS_START);
}

int RequestEffectStart::OnRequestEffectStart(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestEffectStart) != size) {
    global::LogError("%s:%d (%s) 请求开启效果时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestEffectStart *request = (request::RequestEffectStart *)message;

  // 获取 EffectRole 对象
  EffectRole *role = NULL;
  if(request->entity_type_ == entity::EntityType::TYPE_ACTOR) {
    role = EffectRoleManager::GetInstance()->GetActor(request->entity_id_);
  } else if(request->entity_type_ == entity::EntityType::TYPE_NPC) {
    role = EffectRoleManager::GetInstance()->GetNpc(request->entity_id_);
  } else {
    global::LogError("%s:%d (%s) 请求开启效果时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求开启效果时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->entity_type_, request->entity_id_);
    return -1;
  }

  // 开启效果
  request->__set_succeed_(role->StartEffect(request->effect_id_));

  return 0;
}

int RequestEffectStart::OnRequestCommonStatusChange(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestEffectCommonStatusStart) != size) {
    global::LogError("%s:%d (%s) 请求开启效果时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestEffectCommonStatusStart *request = 
    (request::RequestEffectCommonStatusStart *)message;

  // 获取 EffectRole 对象
  EffectRole *role = NULL;
  if(request->entity_type_ == entity::EntityType::TYPE_ACTOR) {
    role = EffectRoleManager::GetInstance()->GetActor(request->entity_id_);
  } else if(request->entity_type_ == entity::EntityType::TYPE_NPC) {
    role = EffectRoleManager::GetInstance()->GetNpc(request->entity_id_);
  } else {
    global::LogError("%s:%d (%s) 请求开启效果时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    LOG_ERROR("请求开启效果时，Role(%d, %lu) 不存在", request->entity_type_,
        request->status_);
    return -1;
  }

  // 开启效果
  Effect *effect = role->GetEffect(EffectType::EFFECT_STATUS);
  if(effect == NULL) {
    LOG_ERROR("添加效果失败");
    return -1;
  }
  StatusEffect *status = (StatusEffect *)effect;
  request->__set_succeed_(status->StartCommonStatus(
      status->ConvertType(request->status_)));

  return 0;

}

}  // namespace effect

}  // namespace server

}  // namespace game

