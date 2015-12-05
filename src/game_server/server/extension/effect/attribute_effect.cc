//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-03 02:30:01.
// File name: attribute_effect.cc
//
// Description:
// Define class AttributeEffect.
//

#include "game_server/server/extension/effect/attribute_effect.h"

#include "game_server/server/extension/effect/configure.h"
#include "game_server/server/extension/effect/effect_role.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace effect {

AttributeEffect::AttributeEffect() {}
AttributeEffect::~AttributeEffect() {}

bool AttributeEffect::Initialize(EffectRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 初始化效果对象失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->role_ = role;

  return true;
}

void AttributeEffect::Finalize() {
  this->role_ = NULL;
}

bool AttributeEffect::Start(core::uint32 id) {
  if(this->role_ == NULL || id == 0) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 得到配置
  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 开启时，效果(%u)不在配置文件中",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }
  // 检测效果类型
  if(configure->type_ != this->GetType()) {
    global::LogError("%s:%d (%s) 开启时，效果(%u)不是属性类效果",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 被动类属性不能由效果改变
  if(IsPassivityAttribute((EffectAttributeType::type)configure->param2_) == true) {
    global::LogError("%s:%d (%s) 开启时，效果(%u)是被动类属性，不能被效果改变",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 得到当前值
  core::uint32 value =
    this->role_->GetEffectAttribute((EffectAttributeType::type)configure->param2_);

  return this->role_->SetEffectAttribute((EffectAttributeType::type)configure->param2_,
      value + configure->param3_);
}

bool AttributeEffect::End(core::uint32 id) {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) 关闭效果(%u)失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 得到配置
  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 关闭时，效果(%u)不在配置文件中",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 检测效果类型
  if(configure->type_ != this->GetType()) {
    global::LogError("%s:%d (%s) 关闭时，效果(%u)不是属性类效果",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 被动类属性不能由效果改变
  if(IsPassivityAttribute((EffectAttributeType::type)configure->param2_) == true) {
    global::LogError("%s:%d (%s) 关闭时，效果(%u)是被动类属性，不能被效果改变",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  // 非0且非消耗类属性 则需要恢复
  if(configure->param1_ && IsConsumingAttribute((EffectAttributeType::type)configure->param2_) == false) {
    // 得到当前值
    core::uint32 value = this->role_->GetEffectAttribute((EffectAttributeType::type)configure->param2_);

    return this->role_->SetEffectAttribute((EffectAttributeType::type)configure->param2_,
        value - configure->param3_);
  }

  return true;
}

}  // namespace effect

}  // namespace server

}  // namespace game

