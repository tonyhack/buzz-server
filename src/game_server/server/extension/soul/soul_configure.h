//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 16:58:19.
// File name: soul_configure.h
//
// Description:
// Define class SoulConfigure.
//

#ifndef __GAME__SERVER__SOUL__SOUL__CONFIGURE__H
#define __GAME__SERVER__SOUL__SOUL__CONFIGURE__H

#include <string>
#include <vector>

#include "core/base/types.h"
#include "entity/soul_types.h"
#include "game_server/server/extension/soul/soul_types.h"

namespace game {

namespace server {

namespace soul {

class SoulEffectValue {
 public:
  entity::SoulEffectType::type type_;
  core::int32 param1_;
  core::int32 param2_;
};

class SoulCell {
 public:
  typedef std::vector<SoulEffectValue> EffectVector;
  typedef std::map<core::int32, EffectVector> EffectMap;
  // typedef std::map<core::int32, SoulEffectValue> EffectMap;

  SoulCell() {}
  ~SoulCell() {}

  // 职业类型
  entity::VocationType::type vocation_;

  // 解锁类型和参数
  entity::SoulUnlockType::type unlock_type_;
  core::int32 unlock_param1_;
  core::int32 unlock_param2_;

  // 阶组ID
  core::int32 step_group_;
  // 级组ID
  core::int32 level_group_;

  // 普通技能
  core::int32 common_skill_;
  // 幻化技能
  core::int32 appear_skill_;
  // 英灵技能
  core::int32 soul_skill_;

  // 精灵效果
  EffectMap spirit_effects_;
  // 魔剑(神武)效果
  EffectMap sword_effects_;
  // 贵族(爵位)效果
  EffectMap noble_effects_;
};

class SoulStepCell {
 public:
  typedef std::map<entity::SoulAttributeType::type, core::int32> AttrMap;

  SoulStepCell() {}
  ~SoulStepCell() {}

  // 消耗灵气
  core::int32 spend_nimbus_;
  // 升阶机率
  core::int32 odds_;
  // 属性加成
  AttrMap attrs_;
};

class SoulLevelCell {
 public:
  SoulLevelCell() {}
  ~SoulLevelCell() {}

  // 消耗道具
  core::int32 spend_item_;
  // 消耗道具数量
  core::int32 spend_number_;
};

class SoulSkillLevelCell {
 public:
  SoulSkillLevelCell() {}
  ~SoulSkillLevelCell() {}

  core::int32 level_;
  core::int32 spend_nimbus_;
  core::int32 spend_gold_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__CONFIGURE__H

