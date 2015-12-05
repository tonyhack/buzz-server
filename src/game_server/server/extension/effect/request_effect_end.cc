//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-07 20:06:52.
// File name: request_effect_end.cc
//
// Description:
// Define class RequestEffectEnd.
//

#include "game_server/server/extension/effect/request_effect_end.h"

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

RequestEffectEnd::RequestEffectEnd() {}
RequestEffectEnd::~RequestEffectEnd() {}

bool RequestEffectEnd::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_EFFECT_END,
      boost::bind(&RequestEffectEnd::OnRequestEffectEnd, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_EFFECT_COMMON_STATUS_END,
      boost::bind(&RequestEffectEnd::OnRequestCommonStatusEnd, this, _1, _2));
  return true;
}

void RequestEffectEnd::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_EFFECT_END);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_EFFECT_COMMON_STATUS_END);
}

int RequestEffectEnd::OnRequestEffectEnd(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestEffectEnd) != size) {
    global::LogError("%s:%d (%s) 请求结束效果时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestEffectEnd *request = (request::RequestEffectEnd *)message;

  // 获取 EffectRole 对象
  EffectRole *role = NULL;
  if(request->entity_type_ == entity::EntityType::TYPE_ACTOR) {
    role = EffectRoleManager::GetInstance()->GetActor(request->entity_id_);
  } else if(request->entity_type_ == entity::EntityType::TYPE_NPC) {
    role = EffectRoleManager::GetInstance()->GetNpc(request->entity_id_);
  } else {
    global::LogError("%s:%d (%s) 请求结束效果时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求结束效果时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->entity_type_, request->entity_id_);
    return -1;
  }

  // 结束效果
  request->__set_succeed_(role->EndEffect(request->effect_id_));

  return 0;
}

int RequestEffectEnd::OnRequestCommonStatusEnd(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestEffectCommonStatusEnd) != size) {
    global::LogError("%s:%d (%s) 请求结束效果时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestEffectCommonStatusEnd *request = 
    (request::RequestEffectCommonStatusEnd *)message;

  // 获取 EffectRole 对象
  EffectRole *role = NULL;
  if(request->entity_type_ == entity::EntityType::TYPE_ACTOR) {
    role = EffectRoleManager::GetInstance()->GetActor(request->entity_id_);
  } else if(request->entity_type_ == entity::EntityType::TYPE_NPC) {
    role = EffectRoleManager::GetInstance()->GetNpc(request->entity_id_);
  } else {
    global::LogError("%s:%d (%s) 请求结束效果时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    LOG_ERROR("请求结束效果时，Role(%d, %lu) 不存在", request->entity_type_,
        request->entity_id_);
    return -1;
  }

  // 结束效果
  Effect *effect = role->GetEffect(EffectType::EFFECT_STATUS);
  if(effect == NULL) {
    LOG_ERROR("去除效果失败");
    return -1;
  }
  StatusEffect *status = (StatusEffect *)effect;
  request->__set_succeed_(status->EndCommonStatus(
      status->ConvertType(request->status_)));

  return 0;
}


}  // namespace effect

}  // namespace server

}  // namespace game

