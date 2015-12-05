//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-20 12:24:08.
// File name: clearing_effect.cc
//
// Description:
// Define class ClearingEffect.
//

#include "game_server/server/extension/effect/clearing_effect.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/effect/configure.h"
#include "game_server/server/extension/effect/effect_role.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace effect {

ClearingEffect::ClearingEffect() {}
ClearingEffect::~ClearingEffect() {}

bool ClearingEffect::Initialize(EffectRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 初始化效果对象失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->role_ = role;

  return true;
}

void ClearingEffect::Finalize() {
  this->role_ = NULL;
}

bool ClearingEffect::Start(core::uint32 id) {
  if(this->role_ == NULL || id == 0) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  GameRole *role = this->role_->GetRole();
  if(role == NULL) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败，得到 GameRole 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 开启效果(%u)时，不在配置文件中",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  if(configure->type_ != this->GetType()) {
    global::LogError("%s:%d (%s) 开启效果(%u)时，类型不是驱散BUFF",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 设置禁止BUFF组
  request::RequestBuffSetForbidGroup request;
  request.__set_type_(this->role_->GetType());
  request.__set_id_(this->role_->GetID());
  request.__set_clear_group_(configure->param1_);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_SET_FORBID_GROUP, &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_BUFF_SET_FORBID_GROUP 失败");
    return false;
  }

  // 清除BUFF组
  request::RequestBuffClearGroup clear_request;
  clear_request.__set_type_(this->role_->GetType());
  clear_request.__set_id_(this->role_->GetID());
  clear_request.__set_clear_group_(configure->param1_);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_CLEAR_GROUP,
        &clear_request, sizeof(clear_request)) != 0) {
    LOG_ERROR("请求 REQUEST_BUFF_CLEAR_GROUP 失败");
    return false;
  }

  LOG_DEBUG("开启 ClearingEffect(%d)", id);

  return true;
}

bool ClearingEffect::End(core::uint32 id) {
  if(this->role_ == NULL || id == 0) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  GameRole *role = this->role_->GetRole();
  if(role == NULL) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败，得到 GameRole 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 开启效果(%u)时，不在配置文件中",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  if(configure->type_ != this->GetType()) {
    global::LogError("%s:%d (%s) 开启效果(%u)时，类型不是驱散BUFF",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 重置禁止BUFF组
  request::RequestBuffResetForbidGroup request;
  request.__set_type_(this->role_->GetType());
  request.__set_id_(this->role_->GetID());
  request.__set_clear_group_(configure->param1_);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_RESET_FORBID_GROUP, &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_BUFF_RESET_FORBID_GROUP 失败");
    return false;
  }

  LOG_DEBUG("关闭 ClearingEffect(%d)", id);

  return true;
}

}  // namespace effect

}  // namespace server

}  // namespace game

