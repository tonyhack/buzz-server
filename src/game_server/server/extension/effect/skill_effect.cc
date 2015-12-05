//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-31 16:33:30.
// File name: skill_effect.cc
//
// Description:
// Define class SkillEffect.
//

#include "game_server/server/extension/effect/skill_effect.h"

#include "game_server/server/extension/effect/configure.h"
#include "game_server/server/extension/effect/effect_role.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace effect {

SkillEffect::SkillEffect() {}
SkillEffect::~SkillEffect() {}

bool SkillEffect::Initialize(EffectRole *role) {
  if (role == NULL) {
    LOG_ERROR("初始化 SkillEffect 时，参数错误");
    return false;
  }

  this->role_ = role;

  return true;
}

void SkillEffect::Finalize() {
  this->role_ = NULL;
}

bool SkillEffect::Start(core::uint32 id) {
  if (this->role_ == NULL || id == 0) {
    LOG_ERROR("开启效果(%u) 失败", id);
    return false;
  }

  // 得到配置
  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    LOG_ERROR("开启时，效果(%u)不在配置文件中", id);
    return false;
  }

  // 检测效果类型
  if(configure->type_ != this->GetType()) {
    LOG_ERROR("开启时，效果(%u)不是属性类效果", id);
    return false;
  }

  // 发请求
  request::RequestSkillIncreaseAttribute request;
  request.__set_type_(this->role_->GetType());
  request.__set_id_(this->role_->GetID());
  request.__set_skill_id_(configure->param1_);
  request.__set_attribute_((entity::SkillVarFields::type)configure->param2_);
  request.__set_increment_(configure->param3_);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SKILL_INCREASE_ATTRIBUTE, &request,
        sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_SKILL_INCREASE_ATTRIBUTE 失败");
    return false;
  }

  return true;
}

bool SkillEffect::End(core::uint32 id) {
  if (this->role_ == NULL || id == 0) {
    LOG_ERROR("关闭效果(%u) 失败", id);
    return false;
  }

  // 得到配置
  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    LOG_ERROR("开启时，效果(%u)不在配置文件中", id);
    return false;
  }

  // 检测效果类型
  if(configure->type_ != this->GetType()) {
    LOG_ERROR("开启时，效果(%u)不是属性类效果", id);
    return false;
  }

  // 发请求
  request::RequestSkillIncreaseAttribute request;
  request.__set_type_(this->role_->GetType());
  request.__set_id_(this->role_->GetID());
  request.__set_skill_id_(configure->param1_);
  request.__set_attribute_((entity::SkillVarFields::type)configure->param2_);
  request.__set_increment_(0 - configure->param3_);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SKILL_INCREASE_ATTRIBUTE, &request,
        sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_SKILL_INCREASE_ATTRIBUTE 失败");
    return false;
  }

  return true;
}

}  // namespace effect

}  // namespace server

}  // namespace game

