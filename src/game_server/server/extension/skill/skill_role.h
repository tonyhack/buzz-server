//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 15:24:13.
// File name: skill_role.h
//
// Description:
// Define class SkillRole.
//

#ifndef __GAME__SERVER__SKILL__SKILL__ROLE__H
#define __GAME__SERVER__SKILL__SKILL__ROLE__H

#include <map>
#include <vector>

#include "core/base/noncopyable.h"
#include "entity/entity_types.h"
#include "entity/skill_types.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/skill/fighting_status.h"
#include "game_server/server/extension/skill/skill.h"
#include "game_server/server/extension/skill/skill_boot_status.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_idle_status.h"
#include "game_server/server/extension/skill/skill_finish_status.h"
#include "game_server/server/extension/skill/skill_launch_status.h"
#include "game_server/server/extension/skill/skill_status.h"
#include "game_server/server/extension/skill/skill_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

class GlobalBuff {
 public:
  GlobalBuff() {}
  GlobalBuff(core::uint32 buff, core::uint32 odds,
      core::uint32 cooling, entity::BuffTargetType::type target_type ) : 
      buff_(buff), odds_(odds), cooling_(cooling), target_type_(target_type) {}

  core::uint32 buff_;
  core::uint32 odds_;
  core::uint32 cooling_;
  entity::BuffTargetType::type target_type_;
};

class SkillRole : public core::Noncopyable {
  typedef std::map<core::uint32, Skill *> SkillMap;

  typedef std::vector<core::uint32> SkillVector;

 public:
  typedef std::vector<GlobalBuff> DebuffVector;

  SkillRole();
  ~SkillRole();

  bool Initialize(GameRole *role);
  void Finalize();

  // 获取对象类型
  entity::EntityType::type GetType() const;

  // 获取对象ID
  core::uint64 GetID() const;

  // 得到技能对象
  Skill *GetSkill(core::uint32 skill_id);
  const Skill *GetSkill(core::uint32 skill_id) const;

  // 增加/删除 技能
  bool AddSkill(Skill *skill);
  Skill *RemoveSkill(core::uint32 skill_id);

  // 技能 待用/收回
  bool SkillStand(core::uint32 id);
  bool SkillSit(core::uint32 id);

  // 获取临时技能对象
  Skill *GetTempSkill(core::uint32 id);

  // 增加/删除 临时技能
  bool AddTempSkill(core::uint32 id, core::uint32 level);
  bool RemoveTempSkill(core::uint32 id);

  // 状态转换
  void GotoStatus(SkillStatusType::type type);

  // 得到当前技能距离
  core::int32 GetSkillDistance(const SkillCell *cell) const;

  // 得到场景PK信息
  bool GetScenePKInfo(bool &rookie_protect, bool &ban_change_pk_mode, bool &die_protect);

  // 检测是否可以设置PK模式
  bool CheckChangePKMode();
  // 是否在安全区域
  SafeAreaResultType::type IsInSafeArea() const;

  // 检测技能BUFF条件
  bool CheckBuffCondition(const SkillCell *cell) const;
  // 检测技能幻化条件
  bool CheckSoulCondition(const SkillCell *cell) const; 
  // 技能是否可用
  bool CheckSkillReady(core::uint32 id) const;

  // 检测技能释放目标
  SkillResultType::type CheckTarget(const SkillCell *cell) const;
  // 检测技能释放距离
  SkillResultType::type CheckDistance(const SkillCell *cell) const;
  // 检测技能吟唱后的释放距离
  SkillResultType::type CheckSingDistance(const SkillCell *cell) const;
  // 检测技能冷却
  SkillResultType::type CheckCooling(const SkillCell *cell) const;
  // 检测技能释放消耗
  SkillResultType::type CheckConsume(const SkillCell *cell) const;
  // 检测技能释放条件
  SkillResultType::type CheckCondition(const SkillCell *cell);
  // 检测PK条件
  SkillResultType::type CheckPKCondition(const SkillCell* cell);

  // 技能开始冷却
  SkillResultType::type SkillCooling(const SkillCell *cell, core::int64 add_value);
  // 技能开始消耗
  SkillResultType::type SkillConsume(const SkillCell *cell);

  inline Skill *GetUseSkill(core::uint32 id) {
    if(this->GetSkillForm() == entity::SkillFormType::COMMON) {
      return this->GetSkill(id);
    } else if(this->GetSkillForm() == entity::SkillFormType::TEMP) {
      return this->GetTempSkill(id);
    } else {
      return NULL;
    }
  }

  // 设置/获取 技能信息
  inline void SetSkillContext(const SkillContext &context) {
    this->current_skill_ = context;
  }
  inline SkillContext &GetSkillContext() {
    return this->current_skill_;
  }
  inline const SkillContext &GetSkillContext() const {
    return this->current_skill_;
  }

  // 得到当前状态
  inline SkillStatus *GetCurrentStatus() {
    return this->current_status_;
  }

  // 获取角色对象
  inline GameRole *GetGameRole() {
    return this->role_;
  }

  // 获取角色对象
  inline const GameRole *GetGameRole() const {
    return this->role_;
  }

  // 获取公共Debuff列表
  inline DebuffVector &GetGlobalDebuffs() {
    return this->global_debuffs_;
  }

  // 获取战斗状态
  inline FightingStatus &GetFightingStatus() {
    return this->fighting_status_;
  }

  // 获取/设置 技能形态
  inline entity::SkillFormType::type GetSkillForm() const {
    return this->skill_form_;
  }
  inline void SetSkillForm(entity::SkillFormType::type form) {
    this->skill_form_ = form;
  }

  // 增加/删除公共debuff
  void AddGlobalDebuff(core::uint32 buff, core::uint32 odds,
      core::uint32 cooling, entity::BuffTargetType::type target_type);
  void RemoveGlobalDebuff(core::uint32 buff, core::uint32 odds,
      core::uint32 cooling);

  // 发送消息到周围
  template <typename T>
  void SendMessageAround(const T &message, gateway::protocol::MessageType::type type,
      bool self = false) {
    if(this->role_ == NULL) {
      global::LogError("%s:%d (%s) 发送消息时 role_ 为空",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    this->role_->SendMessageAround(message, type, self);
  }

 private:
  // 技能是否待用
  bool CheckSkillStand(core::uint32 id) const;
  // 临时技能是否待用
  bool CheckSkillTemp(core::uint32 id) const;

  // 检测阵营条件
  SkillResultType::type CheckCampCondition(const SkillCell *cell);
  // 检测技能作用类型
  SkillResultType::type CheckSkillCondition(const SkillCell *cell);
  // 检测目标状态条件
  SkillResultType::type CheckStatusCondition(const SkillCell *cell);

  // 角色主对象指针
  GameRole *role_;

  // 学习过的技能表
  SkillMap skills_;

  // 待用技能(当前可以使用的技能)
  SkillVector stand_skills_;

  // 临时技能表
  SkillMap temp_skills_;

  // 当前技能信息
  SkillContext current_skill_;

  // 技能当前状态
  SkillStatus *current_status_;

  // 技能状态
  SkillIdleStatus idle_status_;
  SkillBootStatus boot_status_;
  SkillLaunchStatus launch_status_;
  SkillFinishStatus finish_status_;

  // 所有技能都触发的debuff集合
  DebuffVector global_debuffs_;

  // 战斗状态
  FightingStatus fighting_status_;

  // 技能形态
  entity::SkillFormType::type skill_form_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__ROLE__H

