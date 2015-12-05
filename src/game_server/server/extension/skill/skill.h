//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 16:09:37.
// File name: skill.h
//
// Description:
// Define class Skill.
//

#ifndef __GAME__SERVER__SKILL__SKILL__H
#define __GAME__SERVER__SKILL__SKILL__H

#include "core/base/types.h"
#include "entity/skill_types.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace skill {

class SkillRole;

class Skill {
  typedef std::vector<int32_t> AttributeFields;

 public:
  Skill();
  ~Skill();

  bool Initialize(SkillRole *role);
  void Finalize();

  inline core::uint32 GetID() const {
    return this->id_;
  }
  inline void SetID(core::uint32 id) {
    this->id_ = id;
  }

  inline core::uint8 GetLevel() const {
    return this->level_;
  }
  inline void SetLevel(core::uint8 level) {
    this->level_ = level;
  }
  void BroadCastThreatEvent(SkillRole *src, SkillRole *dest, core::int32 threat_value);

  // 检测是否可以使用技能
  SkillResultType::type CheckUseSkill(SkillRole *role);
  // 检测是否可以发射技能
  SkillResultType::type CheckLaunchSkill(SkillRole *role);

  // 技能发射
  SkillResultType::type OnLaunchSkill(SkillRole *role);
  // 技能结束
  SkillResultType::type OnFinishSkill(SkillRole *role);

  // 获取/设置/同步 属性
  core::int32 GetAttribute(entity::SkillVarFields::type type) const;
  bool SetAttribute(entity::SkillVarFields::type type, core::int32 value,
      bool synchronize = true);
  void Synchronize();
  void Synchronize(entity::SkillVarFields::type type);

 private:
  // 技能命中(单体目标类)
  SkillResultType::type SkillHitSingle(SkillRole *role);
  // 技能命中(圆形区域目标类)
  SkillResultType::type SkillHitCircle(SkillRole *role);
  // 技能命中(扇形区域目标类)
  SkillResultType::type SkillHitSector(SkillRole *role, request::SectorRadianType::type radian);

  // 额外AOE技能伤害
  void SkillHitExtraAoe(SkillRole *role);

  // 受到技能攻击
  void OnRoleAttacked(SkillRole *attacked_role, SkillRole *attacker_role, const SkillCell* cell);

  // 加 GlobalBuff
  void AddGlobalBuff(SkillRole *role, SkillRole *target_role, core::int32 buff,
      entity::BuffTargetType::type target_type);

  SkillRole *role_;

  core::uint32 id_;
  core::uint8 level_;

  // 技能变量
  AttributeFields attributes_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__H

