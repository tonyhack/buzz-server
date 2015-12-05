//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-20 15:20:24.
// File name: request_buff_handler.cc
//
// Description:
// Define class RequestBuffHandler.
//

#include "game_server/server/extension/buff/request_buff_handler.h"

#include <boost/bind.hpp>

#include "entity/role_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/buff/buff.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "gateway_server/protocol/gateway_buff_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

RequestBuffHandler::RequestBuffHandler() {}
RequestBuffHandler::~RequestBuffHandler() {}

bool RequestBuffHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BUFF_START,
      boost::bind(&RequestBuffHandler::OnRequestBuffStart, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BUFF_END,
      boost::bind(&RequestBuffHandler::OnRequestBuffEnd, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BUFF_CLEAR,
      boost::bind(&RequestBuffHandler::OnRequestBuffClear, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BUFF_CLEAR_GROUP,
      boost::bind(&RequestBuffHandler::OnRequestBuffClearGroup, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BUFF_SET_FORBID_GROUP,
      boost::bind(&RequestBuffHandler::OnRequestBuffSetForbidGroup, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BUFF_RESET_FORBID_GROUP,
      boost::bind(&RequestBuffHandler::OnRequestBuffResetForbidGroup, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BUFF_GET_EXIST,
      boost::bind(&RequestBuffHandler::OnRequestBuffGetExist, this, _1, _2));

  return true;
}

void RequestBuffHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BUFF_START);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BUFF_END);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BUFF_CLEAR);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BUFF_CLEAR_GROUP);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BUFF_SET_FORBID_GROUP);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BUFF_RESET_FORBID_GROUP);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BUFF_GET_EXIST);
}

int RequestBuffHandler::OnRequestBuffStart(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestBuffStart) != size) {
    global::LogError("%s:%d (%s) 请求开启Buff时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestBuffStart *request = (request::RequestBuffStart *)message;

  // 获取 BuffRole 对象
  BuffRole *role = NULL;
  if(request->dest_entity_type_ == entity::EntityType::TYPE_ACTOR) {
    role = BuffRoleManager::GetInstance()->GetActor(request->dest_entity_id_);
  } else if(request->dest_entity_type_ == entity::EntityType::TYPE_NPC) {
    role = BuffRoleManager::GetInstance()->GetNpc(request->dest_entity_id_);
  } else {
    global::LogError("%s:%d (%s) 请求开启Buff时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求开启Buff时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->dest_entity_type_, request->dest_entity_id_);
    return -1;
  }

  // 开启Buff
  if(role->StartBuff(request->src_entity_type_, request->src_entity_id_, request->buff_id_) == false) {
    request->__set_succeed_(false);
    global::LogError("%s:%d (%s) Role(%u,%lu) 开启BUFF失败",
        __FILE__, __LINE__, __FUNCTION__, request->dest_entity_type_,
        request->dest_entity_id_);
    return 0;
  }

  request->__set_succeed_(true);

  return 0;
}

int RequestBuffHandler::OnRequestBuffEnd(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestBuffEnd) != size) {
    global::LogError("%s:%d (%s) 请求结束Buff时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestBuffEnd *request = (request::RequestBuffEnd *)message;

  // 获取 BuffRole 对象
  BuffRole *role = NULL;
  if(request->dest_entity_type_ == entity::EntityType::TYPE_ACTOR) {
    role = BuffRoleManager::GetInstance()->GetActor(request->dest_entity_id_);
  } else if(request->dest_entity_type_ == entity::EntityType::TYPE_NPC) {
    role = BuffRoleManager::GetInstance()->GetNpc(request->dest_entity_id_);
  } else {
    global::LogError("%s:%d (%s) 请求结束Buff时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求结束Buff时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->dest_entity_type_,
        request->dest_entity_id_);
    return -1;
  }

  // 结束Buff
  if(role->EndBuff(request->buff_id_) == false) {
    request->__set_succeed_(false);
    return 0;
  }

  request->__set_succeed_(true);

  return 0;
}

int RequestBuffHandler::OnRequestBuffClear(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestBuffClear) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestBuffClear *request = (request::RequestBuffClear *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(request->type_, request->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  role->EndBuff(request->buff_id_);

  request->__set_succeed_(true);

  return 0;
}

int RequestBuffHandler::OnRequestBuffClearGroup(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestBuffClearGroup) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestBuffClearGroup *request = (request::RequestBuffClearGroup *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(request->type_, request->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  role->ClearGroupBuff(request->clear_group_);

  request->__set_succeed_(true);

  return 0;
}

int RequestBuffHandler::OnRequestBuffSetForbidGroup(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestBuffSetForbidGroup) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestBuffSetForbidGroup *request = (request::RequestBuffSetForbidGroup *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(request->type_, request->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  role->IncreaseForbidGroup(request->clear_group_);

  return 0;
}

int RequestBuffHandler::OnRequestBuffResetForbidGroup(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestBuffResetForbidGroup) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestBuffResetForbidGroup *request = (request::RequestBuffResetForbidGroup *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(request->type_, request->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  role->DecreaseForbidGroup(request->clear_group_);

  return 0;
}

int RequestBuffHandler::OnRequestBuffGetExist(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestBuffGetExist) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestBuffGetExist *request = (request::RequestBuffGetExist *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(request->type_, request->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  request->__set_exist_(role->GetBuff(request->buff_id_) != NULL);

  return 0;
}

}  // namespace buff

}  // namespace server

}  // namespace game

