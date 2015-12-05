//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-19 17:10:18.
// File name: damage_calculation.cc
//
// Description:
// Define class DamageCalculation.
//

#include "game_server/server/extension/skill/damage_calculation.h"

#include "entity/actor_types.h"
#include "entity/inform_types.h"
#include "game_server/server/according_level_configure.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/facade_request.h"
#include "game_server/server/extension/skill/skill.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_ai_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_skill_protocol_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace skill {

SkillResultType::type DamageCalculation::HurtCalculate(SkillRole *src, SkillRole *dest,
    core::uint32 hurt_id, core::int32 hit_type, const Skill *skill) {
  // 获取伤害配置
  const HurtCell *cell = Configure::GetInstance()->GetHurt(hurt_id);
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }
  // 玩家攻击处理
  // ChangeActorStatus(src, dest);

  SkillResultType::type result = SkillResultType::MAX;

  switch(cell->type_) {
    case entity::SkillHurtType::PHYSICS_ATTACK:
      result = this->HurtCalculateAttack(src, dest, cell->param1_, cell->param2_,
          cell->threat_coe_, cell->threat_value_, true, hit_type, skill);
      break;
    case entity::SkillHurtType::MAGIC_ATTACK:
      result = this->HurtCalculateAttack(src, dest, cell->param1_, cell->param2_,
          cell->threat_coe_, cell->threat_value_, false, hit_type, skill);
      break;
    case entity::SkillHurtType::REDUCE_HP:
      src = dest;
      result = this->HurtCalculateReduceHp(src, dest, cell->param1_, cell->param2_,
          cell->threat_coe_, cell->threat_value_, skill);
      break;
    case entity::SkillHurtType::REDUCE_MP:
      src = dest;
      result = this->HurtCalculateReduceMp(src, dest, cell->param1_, cell->param2_,
          cell->threat_coe_, cell->threat_value_, skill);
      break;
    case entity::SkillHurtType::PHYSICS_CURE:
      result = this->HurtCalculateCure(src, dest, cell->param1_, cell->param2_,
         cell->threat_coe_, cell->threat_value_, true, skill);
      break;
    case entity::SkillHurtType::MAGIC_CURE:
      result = this->HurtCalculateCure(src, dest, cell->param1_, cell->param2_,
         cell->threat_coe_, cell->threat_value_, false, skill);
      break;
    case entity::SkillHurtType::ADD_HP:
      src = dest;
      result = this->HurtCalculateAddHp(src, dest, cell->param1_, cell->param2_,
         cell->threat_coe_, cell->threat_value_, skill);
      break;
    case entity::SkillHurtType::ADD_MP:
      src = dest;
      result = this->HurtCalculateAddMp(src, dest, cell->param1_, cell->param2_,
         cell->threat_coe_, cell->threat_value_, skill);
      break;
    case entity::SkillHurtType::CLEAR_BUFF:
      src = dest;
      result = this->HurtCalculateClearBuff(src, dest, cell->param1_,
         cell->threat_coe_, cell->threat_value_, skill);
      break;
    case entity::SkillHurtType::CLEAR_BUFF_GROUP:
      src = dest;
      result = this->HurtCalculateClearBuffGroup(src, dest, cell->param1_,
          cell->threat_coe_, cell->threat_value_, skill);
      break;
    default:
      return SkillResultType::ERROR_ARGS;
  }

  return result;
}

SkillResultType::type DamageCalculation::HurtCalculateAttack(SkillRole *src, SkillRole *dest,
    core::int32 coe, core::int32 skill_hurt_value, core::int32 threat_coe,
    core::int32 threat_value, bool physics, core::int32 hit_type,
    const Skill *skill) {
  if(src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *src_role = src->GetGameRole();
  GameRole *dest_role = dest->GetGameRole();

  if(src_role == NULL || dest_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 等级
  core::int32 src_level = src_role->GetAttribute(entity::RoleAoiFields::LEVEL);
  core::int32 dest_level = dest_role->GetAttribute(entity::RoleAoiFields::LEVEL);

  // 目标等级配置
  const AccordingLevelCell *dest_level_cell =
    game::server::Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(dest_level);
  if(dest_level_cell == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  // 角色等级配置
  const AccordingLevelCell *src_level_cell =
    game::server::Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(src_level);
  if(src_level_cell == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 守方闪避
  core::int32 dest_dodge = dest_role->GetAttribute(entity::RoleClientFields::DODGE);
  // 攻方暴击
  core::int32 src_crit = src_role->GetAttribute(entity::RoleClientFields::CRIT);
  // 攻方会心
  core::int32 src_ignore_defence = src_role->GetAttribute(entity::RoleClientFields::IGNORE_DEFENCE);

  // 闪避率=闪避等级/(((等级-5)*(等级-5)+375)*2.5)
  // 总闪避率=[总闪避等级换算闪避率]+闪避率加成值1+闪避率加成值2+…
  core::int32 dest_dodge_per =
    (core::int32)((dest_dodge*global::kPercentDenom)/(((dest_level-5)*(dest_level-5)+375)*2.5f));
  dest_dodge_per += dest_role->GetAttribute(entity::RoleServerFields::DODGE_PERCENT_ADD_VALUE);

  // 暴击率=暴击率=暴击等级/(((lvl-5)*(lvl-5)+375)*2.5)
  // 总暴击率=[总暴击等级换算暴击率]+暴击率加成值1+暴击率加成值2+…
  core::int32 src_crit_per = (core::int32)((src_crit*global::kPercentDenom)/(((src_level-5)*(src_level-5)+375)*2.5f));
  src_crit_per += src_role->GetAttribute(entity::RoleServerFields::CRIT_PERCENT_ADD_VALUE);

  // 会心率=会心等级/(((lvl-5)*(lvl-5)+375)*5)
  // 总会心率=[总会心等级换算会心率]+会心率加成值1+会心率加成值2+…
  core::int32 src_ignore_defence_per =
    (core::int32)((src_ignore_defence*global::kPercentDenom)/(((src_level-5)*(src_level-5)+375)*5.0f));
  src_ignore_defence_per += src_role->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_PERCENT_ADD_VALUE);

  // roll点，返回0为普通，1为忽视防御，2为闪避，3为暴击
  // 随机出结果
  entity::SkillHitType::type roll = this->HurtRoll(src_ignore_defence_per,
      dest_dodge_per, src_crit_per, hit_type);

  // 广播伤害消息
  gateway::protocol::MessageSkillHit message;
  message.__set_src_id_(src_role->GetStringID());
  message.__set_src_type_(src_role->GetType());
  message.__set_target_type_(dest_role->GetType());
  message.__set_target_id_(dest_role->GetStringID());
  message.__set_hit_type_(roll);

  if(roll != entity::SkillHitType::DODGE) {
    // 伤害值
    core::int64 hurt_value = 0;
    // 守方防御
    core::int64 dest_defence = 0;
    // 攻方攻击
    core::int64 src_attack = 0;
    // 暴击效果系数
    float src_crit_effect_coe = 1.0f;
    // 守方伤害减免率
    float dest_hurt_reduce_per = 0.0f;
    // 伤害浮动倍率
    float hurt_random_per = 1.0f;

    // 物理和魔法分开取值
    if(physics == true) {
      // 计算守方防御值
      dest_defence = dest_role->GetAttribute(entity::RoleClientFields::PHYSICS_DEFENCE);
      // 计算攻方攻击值
      src_attack = src_role->GetAttribute(entity::RoleClientFields::PHYSICS_ATTACK);
      // 计算守方伤害减免率
      dest_hurt_reduce_per =
        (dest_role->GetAttribute(entity::RoleServerFields::PHYSICS_HURT_REDUCE_ADD_PERCENT) +
        dest_role->GetAttribute(entity::RoleServerFields::HURT_REDUCE_ADD_PERCENT)) / (float)global::kPercentDenom;
    } else {
      // 计算守方防御值
      dest_defence = dest_role->GetAttribute(entity::RoleClientFields::MAGIC_DEFENCE);
      // 计算攻方攻击值
      src_attack = src_role->GetAttribute(entity::RoleClientFields::MAGIC_ATTACK);
      // 计算守方伤害减免率
      dest_hurt_reduce_per =
        (dest_role->GetAttribute(entity::RoleServerFields::MAGIC_HURT_REDUCE_ADD_PERCENT) +
        dest_role->GetAttribute(entity::RoleServerFields::HURT_REDUCE_ADD_PERCENT)) / (float)global::kPercentDenom;
    }

    if(roll == entity::SkillHitType::CRIT) {
      // 暴击倍率
      src_crit_effect_coe = (src_level_cell->crit_effect_coe_ +
          src_role->GetAttribute(entity::RoleServerFields::CRIT_EFFECT_PERCENT_ADD_VALUE)) /
        (float)global::kPercentDenom;
    }

    if(roll == entity::SkillHitType::IGNORE_DEF) {
      // 忽视防御
      dest_defence = 0;
    }

    // 伤害减免率不能大于1.0
    if(dest_hurt_reduce_per > 1.0f) {
      dest_hurt_reduce_per = 1.0f;
    }

    // 伤害浮动的随机范围
    core::uint32 rand = ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
    core::uint32 hurt_random = src_level_cell->hurt_random_per_ +
      src_role->GetAttribute(entity::RoleServerFields::HURT_RANDOM_PERCENT_ADD);
    hurt_random_per += (float)(rand % hurt_random) / (float)global::kPercentDenom;

    // 伤害=(攻方攻*攻方攻/(攻方攻+守方防)*技能伤害系数+技能附加伤害)*(1-守方伤害减免率)*暴击倍率*伤害浮动倍率
    if(src_attack == 0 && dest_defence == 0) {
      hurt_value = 0;
    } else {
      hurt_value = (core::int32)((src_attack * src_attack / (src_attack+dest_defence) *
          ((float)coe/(float)global::kPercentDenom) + skill_hurt_value) * (1.0f - dest_hurt_reduce_per) *
        src_crit_effect_coe * hurt_random_per);
      // 技能效果加成
      if (skill != NULL) {
        core::int32 last_hurt_add_value = skill->GetAttribute(entity::SkillVarFields::HURT_ADD_VALUE);
        core::int32 last_hurt_add_percent = skill->GetAttribute(entity::SkillVarFields::HURT_ADD_PERCENT);
        if (last_hurt_add_percent < 0) {
          LOG_ERROR("HURT_ADD_PERCENT is negative");
          return SkillResultType::ERROR_ARGS;
        }
        hurt_value += (core::int32)(hurt_value * (float)last_hurt_add_percent/(float)global::kPercentDenom);
        hurt_value += last_hurt_add_value;
      }
    }

    // 最小伤害为1
    if(hurt_value <= 0) {
      hurt_value = 1;
    }

    // 计算仇恨
    if(src->GetType() == entity::EntityType::TYPE_ACTOR && dest->GetType() ==
        entity::EntityType::TYPE_NPC) {
      core::int32 threat = (core::int32)(threat_value + hurt_value * ((float)threat_coe/(float)global::kPercentDenom));
      this->BroadcastThreatEvent(src, dest, threat);
    }

    // 扣去血量
    core::int32 dest_hp = dest_role->GetAttribute(
        entity::RoleAoiFields::CURRENT_HP) - hurt_value;

    // 设置血量
    dest_role->SetAttribute(entity::RoleAoiFields::CURRENT_HP, dest_hp);

    if(physics == true) {
      this->BroadcastAttackHitEvent(src, dest, roll, entity::SkillHurtType::PHYSICS_ATTACK, hurt_value);
    } else {
      this->BroadcastAttackHitEvent(src, dest, roll, entity::SkillHurtType::MAGIC_ATTACK, hurt_value);
    }

    // 被杀死后处理
    if(dest_role->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
      this->OnKillRole(src, dest);
    }

    message.__set_value_(0 - hurt_value);
  }

  // global::LogDebug("[技能流程] 技能伤害血量(%u) 伤害类型(%u)", message.value_, message.hit_type_);

  // 广播
  src_role->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SKILL_HIT, true);

  return SkillResultType::SUCCESS;
}

SkillResultType::type DamageCalculation::HurtCalculateReduceHp(SkillRole *src, SkillRole *dest,
    core::int32 hp_percent, core::int32 hp_value, core::int32 threat_coe,
    core::int32 threat_value, const Skill *skill) {
  if(src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *dest_role = dest->GetGameRole();
  GameRole *src_role = src->GetGameRole();

  if(dest_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 cur_hp = dest_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
  float percent = (float)hp_percent/(float)global::kPercentDenom;
  if(percent > 1.0f) {
    percent = 1.0f;
  }

  // 直接扣除生命值: 目标当前生命 * (1 - M%) - N
  core::int32 remain_hp = (core::int32)(cur_hp * (1.0f - percent) - hp_value);
  
  if(src->GetType() == entity::EntityType::TYPE_ACTOR && dest->GetType() ==
        entity::EntityType::TYPE_NPC) {
     // 计算仇恨
     core::int32 threat = (core::int32)(threat_value + (cur_hp * percent + hp_value)*threat_coe/10000);
     BroadcastThreatEvent(src, dest, threat);
  }

  // 设置当前血量
  dest_role->SetAttribute(entity::RoleAoiFields::CURRENT_HP, remain_hp);

  this->BroadcastAttackHitEvent(src, dest, entity::SkillHitType::COMMON,
      entity::SkillHurtType::REDUCE_HP, cur_hp - remain_hp);
  
  // 被杀死后处理
  if(dest_role->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    this->OnKillRole(src, dest);
  }

  // 广播伤害消息
  gateway::protocol::MessageSkillHit message;
  message.__set_src_id_(src_role->GetStringID());
  message.__set_src_type_(src_role->GetType());
  message.__set_target_type_(dest_role->GetType());
  message.__set_target_id_(dest_role->GetStringID());
  message.__set_hit_type_(entity::SkillHitType::COMMON);
  message.__set_value_(remain_hp - cur_hp);
  dest_role->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SKILL_HIT, true);

  return SkillResultType::SUCCESS;
}

SkillResultType::type DamageCalculation::HurtCalculateReduceMp(SkillRole *src, SkillRole *dest,
    core::int32 mp_percent, core::int32 mp_value, core::int32 threat_coe,
    core::int32 threat_value, const Skill *skill) {
  if(src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *dest_role = dest->GetGameRole();
  GameRole *src_role = src->GetGameRole();

  if(dest_role == NULL || src_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 cur_mp = dest_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
  float percent = (float)mp_percent/(float)global::kPercentDenom;
  if(percent > 1.0f) {
    percent = 1.0f;
  }

  // 直接扣除魔法值: 目标当前魔法 * (1 - M%) - N
  core::int32 remain_mp  = (core::int32)(cur_mp * (1.0f - percent) - mp_value);

  if(src->GetType() == entity::EntityType::TYPE_ACTOR && dest->GetType() ==
      entity::EntityType::TYPE_NPC) {
    // 计算仇恨
    core::int32 threat = (core::int32)(threat_value + (cur_mp * percent + mp_value) * threat_coe / global::kPercentDenom);
    this->BroadcastThreatEvent(src, dest, threat);
  }

  // 设置当前魔法值
  dest_role->SetAttribute(entity::RoleAoiFields::CURRENT_MP, remain_mp);

  this->BroadcastAttackHitEvent(src, dest, entity::SkillHitType::COMMON,
      entity::SkillHurtType::REDUCE_MP, cur_mp - remain_mp);

  // 广播伤害消息
  gateway::protocol::MessageSkillHit message;
  message.__set_src_id_(src_role->GetStringID());
  message.__set_src_type_(src_role->GetType());
  message.__set_target_type_(dest_role->GetType());
  message.__set_target_id_(dest_role->GetStringID());
  message.__set_hit_type_(entity::SkillHitType::COMMON);
  message.__set_value_(remain_mp - cur_mp);
  dest_role->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SKILL_HIT, true);

  return SkillResultType::SUCCESS;
}

SkillResultType::type DamageCalculation::HurtCalculateCure(SkillRole *src, SkillRole *dest,
    core::int32 coe, core::int32 skill_cure_value, core::int32 threat_coe,
    core::int32 threat_value, bool physics, const Skill *skill) {
  if(src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *src_role = src->GetGameRole();
  GameRole *dest_role = dest->GetGameRole();

  if(src_role == NULL || dest_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 等级
  core::int32 src_level = src_role->GetAttribute(entity::RoleAoiFields::LEVEL);
  core::int32 dest_level = dest_role->GetAttribute(entity::RoleAoiFields::LEVEL);

  // 等级配置
  const AccordingLevelCell *dest_level_cell =
    game::server::Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(dest_level);
  if(dest_level_cell == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  const AccordingLevelCell *src_level_cell =
    game::server::Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(src_level);
  if(src_level_cell == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 广播伤害消息
  gateway::protocol::MessageSkillHit message;
  message.__set_src_id_(src_role->GetStringID());
  message.__set_src_type_(src_role->GetType());
  message.__set_target_type_(dest_role->GetType());
  message.__set_target_id_(dest_role->GetStringID());

  core::int32 cure_value = 0;

  // 攻方暴击
  core::int32 src_crit = src_role->GetAttribute(entity::RoleClientFields::CRIT);

  // 暴击系数
  float crit_effect_coe = 1.0f;

  // 攻方攻击力
  core::int32 src_attack = 0;

  // 守方治疗加成率
  float dest_cure_per = 0.0f;

  // 伤害浮动
  float hurt_random_per = 1.0f;

  // 暴击率=暴击率=暴击等级/(((lvl-5)*(lvl-5)+375)*2.5)
  // 总暴击率=[总暴击等级换算暴击率]+暴击率加成值1+暴击率加成值2+…
  core::int32 src_crit_per = (core::int32)((src_crit*global::kPercentDenom)/(((src_level-5)*(src_level-5)+375)*2.5f));
  src_crit_per += src_role->GetAttribute(entity::RoleServerFields::CRIT_PERCENT_ADD_VALUE);
  
  // 计算是否暴击效果
  core::int32 rand = ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  if(rand <= src_crit_per) {
    // 暴击倍率
    crit_effect_coe = (src_level_cell->crit_effect_coe_ +
        src_role->GetAttribute(entity::RoleServerFields::CRIT_EFFECT_PERCENT_ADD_VALUE)) /
      (float)global::kPercentDenom;
    message.__set_hit_type_(entity::SkillHitType::CRIT);
  }

  // 伤害浮动的随机范围
  core::uint32 hurt_random = src_level_cell->hurt_random_per_ +
    src_role->GetAttribute(entity::RoleServerFields::HURT_RANDOM_PERCENT_ADD);
  hurt_random_per += (float)(rand % hurt_random) / (float)global::kPercentDenom;

  if(physics == true) {
    src_attack = src_role->GetAttribute(entity::RoleClientFields::PHYSICS_ATTACK);
  } else {
    src_attack = src_role->GetAttribute(entity::RoleClientFields::MAGIC_ATTACK);
  }

  // 治疗 = (施法者法攻*技能伤害系数 + 技能附加伤害) * (1+受方治疗加成率) * 攻方暴击倍率
  cure_value = (core::int32)((src_attack * ((float)coe/(float)global::kPercentDenom) + skill_cure_value) *
      (1.0f + dest_cure_per) * crit_effect_coe * hurt_random_per);

  // 技能效果加成
  if (skill != NULL) {
    core::int32 last_cure_add_value = skill->GetAttribute(entity::SkillVarFields::HURT_ADD_VALUE);
    core::int32 last_cure_add_percent = skill->GetAttribute(entity::SkillVarFields::HURT_ADD_PERCENT);
    if (last_cure_add_percent < 0) {
      LOG_ERROR("HURT_ADD_PERCENT is negative");
      return SkillResultType::ERROR_ARGS;
    }
    cure_value += (core::int32)((float)last_cure_add_percent/(float)global::kPercentDenom);
    cure_value += last_cure_add_value;
  }

  // 最小治疗效果为1
  if(cure_value <= 0) {
    cure_value = 1;
  }

  // 扣去血量
  core::int32 dest_hp = dest_role->GetAttribute(
      entity::RoleAoiFields::CURRENT_HP) + cure_value;

  // 设置血量
  dest_role->SetAttribute(entity::RoleAoiFields::CURRENT_HP, dest_hp);
  
  // 被杀死后处理
  if(dest_role->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    this->OnKillRole(src, dest);
  }

  message.__set_value_(cure_value);
  
  // 广播
  src_role->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SKILL_HIT, true);

  // global::LogDebug("[技能流程] 技能治疗血量(%u) 治疗类型(%u)", message.value_, message.hit_type_);

  // 计算仇恨
  core::int32 threat = threat_value + (cure_value)*threat_coe/10000;
  this->BroadcastThreatEvent(src, dest, threat);

  return SkillResultType::SUCCESS;
}

SkillResultType::type DamageCalculation::HurtCalculateAddHp(SkillRole *src, SkillRole *dest,
    core::int32 hp_percent, core::int32 hp_value, core::int32 threat_coe,
    core::int32 threat_value, const Skill *skill) {
  if(src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *dest_role = dest->GetGameRole();
  GameRole * src_role = src->GetGameRole();

  if(dest_role == NULL || src_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 cur_hp = dest_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
  core::int32 max_hp = dest_role->GetAttribute(entity::RoleAoiFields::MAX_HP);

  float percent = (float)hp_percent/(float)global::kPercentDenom;
  if(percent > 1.0f) {
    percent = 1.0f;
  }

  // 直接增加血量值: 目标当前魔法 + 目标魔法上限*M% + N
  core::int32 hp = (core::int32)(cur_hp + max_hp * percent + hp_value);

  // 设置当前血量值
  dest_role->SetAttribute(entity::RoleAoiFields::CURRENT_HP, hp);

  // 广播伤害消息
  gateway::protocol::MessageSkillHit message;
  message.__set_src_id_(src_role->GetStringID());
  message.__set_src_type_(src_role->GetType());
  message.__set_target_type_(dest_role->GetType());
  message.__set_target_id_(dest_role->GetStringID());
  message.__set_hit_type_(entity::SkillHitType::COMMON);
  message.__set_value_(hp - cur_hp);
 
  //广播仇恨消息
  // 计算方式 该仇恨值 = 治疗/伤害值 * 系数
  core::int32 threat = (hp - cur_hp)*threat_coe + threat_value/10000;
  this->BroadcastThreatEvent(src, dest, threat);

  return SkillResultType::SUCCESS;
}

SkillResultType::type DamageCalculation::HurtCalculateAddMp(SkillRole *src, SkillRole *dest,
    core::int32 mp_percent, core::int32 mp_value, core::int32 threat_coe,
    core::int32 threat_value, const Skill *skill) {
  if(src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *dest_role = dest->GetGameRole();
  GameRole *src_role = src->GetGameRole();

  if(dest_role == NULL || src_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 cur_mp = dest_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
  core::int32 max_mp = dest_role->GetAttribute(entity::RoleAoiFields::MAX_MP);

  float percent = (float)mp_percent/(float)global::kPercentDenom;
  if(percent > 1.0f) {
    percent = 1.0f;
  }

  // 直接增加魔法值: 目标当前魔法 + 目标魔法上限*M% + N
  core::int32 mp = (core::int32)(cur_mp + max_mp * percent + mp_value);

  // 设置当前魔法值
  dest_role->SetAttribute(entity::RoleAoiFields::CURRENT_MP, mp);

  // 广播伤害消息
  gateway::protocol::MessageSkillHit message;
  message.__set_src_id_(src_role->GetStringID());
  message.__set_src_type_(src_role->GetType());
  message.__set_target_type_(dest_role->GetType());
  message.__set_target_id_(dest_role->GetStringID());
  message.__set_hit_type_(entity::SkillHitType::COMMON);
  message.__set_value_(mp - cur_mp);
  
  //广播仇恨消息
  // 计算方式 该仇恨值 = 治疗/伤害值 * 系数
  core::int32 threat = (mp - cur_mp)*threat_coe/10000 + threat_value;
  this->BroadcastThreatEvent(src, dest, threat);

  return SkillResultType::SUCCESS;
}

SkillResultType::type DamageCalculation::HurtCalculateClearBuff(SkillRole *src, SkillRole *dest,
    core::int32 buff, core::int32 threat_coe, core::int32 threat_value, const Skill *skill) {
  if(src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  request::RequestBuffClear request;
  request.__set_type_(dest->GetType());
  request.__set_id_(dest->GetID());
  request.__set_buff_id_(buff);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_CLEAR,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_BUFF_CLEAR 失败");
    return SkillResultType::ERROR_UNKNOWN;
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type DamageCalculation::HurtCalculateClearBuffGroup(SkillRole *src, SkillRole *dest,
    core::int32 buff_group, core::int32 threat_coe, core::int32 threat_value, const Skill *skill) {
  if (src == NULL || dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  request::RequestBuffClearGroup request;
  request.__set_type_(dest->GetType());
  request.__set_id_(dest->GetID());
  request.__set_clear_group_(buff_group);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_CLEAR_GROUP,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_BUFF_CLEAR_GROUP 失败");
    return SkillResultType::ERROR_UNKNOWN;
  }

  return SkillResultType::SUCCESS;
}

entity::SkillHitType::type DamageCalculation::HurtRoll(core::int32 ignore_def_per,
    core::int32 dodge_per, core::int32 crit_per, core::int32 hit_type) {
  if((hit_type & SkillHitType::DODGE) == 0 || dodge_per < 0) {
    dodge_per = 0;
  }
  if((hit_type & SkillHitType::IGNORE_DEFENCE) == 0 || ignore_def_per < 0) {
    ignore_def_per = 0;
  }
  if((hit_type & SkillHitType::CRIT) == 0 || crit_per < 0) {
    crit_per = 0;
  }
  core::int32 odds = dodge_per + ignore_def_per + crit_per;
  if((hit_type & SkillHitType::COMMON) != 0 && odds < global::kPercentDenom) {
    odds = global::kPercentDenom;
  }
  core::int32 rand = ExtensionManager::GetInstance()->GetRandom().Random(odds);
  if(dodge_per > 0 && rand <= dodge_per) {
    return entity::SkillHitType::DODGE;
  } else if(ignore_def_per > 0 && rand <= dodge_per + ignore_def_per) {
    return entity::SkillHitType::IGNORE_DEF;
  } else if(crit_per > 0 && rand <= dodge_per + ignore_def_per + crit_per) {
    return entity::SkillHitType::CRIT;
  } else {
    return entity::SkillHitType::COMMON;
  }
}

void DamageCalculation::BroadcastThreatEvent(SkillRole *src, SkillRole *dest, core::int32 threat_value) {
  // 广播增加仇恨事件
  if(src == NULL || dest == NULL || threat_value == 0) {
    LOG_ERROR("参数无效");
    return ;
  }

  request::RequestAiAddThreat request;
  request.__set_src_id_(src->GetID());
  request.__set_src_type_(src->GetType());
  request.__set_target_type_(dest->GetType());
  request.__set_target_id_(dest->GetID());
  request.__set_threat_value_(threat_value);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_AI_ADD_THREAT,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_AI_ADD_THREAT 失败");
    return ;
  }
}

void DamageCalculation::BroadcastAttackHitEvent(SkillRole *src, SkillRole *dest,
    entity::SkillHitType::type hit_type, entity::SkillHurtType::type hurt_type, core::int32 value) {
  if(src == NULL || dest == NULL || hit_type >= entity::SkillHitType::MAX) {
    LOG_ERROR("参数无效");
    return;
  }

  event::EventSkillAttackHit attack_event;
  attack_event.__set_type_(src->GetType());
  attack_event.__set_id_(src->GetID());
  attack_event.__set_dest_type_(dest->GetType());
  attack_event.__set_dest_id_(dest->GetID());
  attack_event.__set_hit_type_(hit_type);
  attack_event.__set_hurt_type_(hurt_type);
  attack_event.__set_value_(value);
  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(src->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(src->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  } else {
    LOG_ERROR("实体类型错误(%d,%lu)", src->GetType(), src->GetID());
    return;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SKILL_ATTACK_HIT, src->GetID(), channel_type,
      &attack_event, sizeof(attack_event));

  event::EventSkillAttackedHit attacked_event;
  attacked_event.__set_type_(dest->GetType());
  attacked_event.__set_id_(dest->GetID());
  attacked_event.__set_src_type_(src->GetType());
  attacked_event.__set_src_id_(src->GetID());
  attacked_event.__set_hit_type_(hit_type);
  attacked_event.__set_hurt_type_(hurt_type);
  attacked_event.__set_value_(value);
  channel_type = event::ChannelType::CHANNEL_MAX;
  if(dest->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(dest->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  } else {
    LOG_ERROR("实体类型错误(%d,%lu)", dest->GetType(), dest->GetID());
    return;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SKILL_ATTACKED_HIT, dest->GetID(), channel_type,
      &attacked_event, sizeof(attacked_event));
}

void DamageCalculation::OnKillRole(SkillRole *killer_role, SkillRole *dead_role) {
  if(killer_role == NULL || dead_role == NULL ||
      killer_role->GetGameRole() == NULL ||dead_role->GetGameRole() == NULL) {
    return ;
  }

  // 角色死亡事件
  event::EventRoleKilled event;
  event.__set_type_(dead_role->GetType());
  event.__set_id_(dead_role->GetID());
  event.__set_attacker_type_(killer_role->GetType());
  event.__set_attacker_id_(killer_role->GetID());
  event.__set_skill_id_(killer_role->GetSkillContext().id_);
  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(dead_role->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(dead_role->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  } else {
    LOG_ERROR("处理被杀死事件时，实体类型错误(%d, %lu)",
        dead_role->GetType(), dead_role->GetID());
    return ;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ROLE_KILLED, dead_role->GetID(), channel_type,
      &event, sizeof(event));

  if(killer_role->GetType() == entity::EntityType::TYPE_ACTOR) {
    // 玩家杀死NPC事件
    if(dead_role->GetType() == entity::EntityType::TYPE_NPC) {
      GameNpc *dead_npc = (GameNpc *)dead_role->GetGameRole();
      const NpcCell *npc_cell = game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
          dead_npc->GetTemplateID());
      if(npc_cell == NULL) {
        LOG_ERROR("NPC(%d)配置没有找到", dead_npc->GetTemplateID());
        return ;
      }
      event::EventActorKillNpc event_kill_npc;
      event_kill_npc.__set_actor_(killer_role->GetID());
      event_kill_npc.__set_npc_(dead_npc->GetID());
      event_kill_npc.__set_npc_template_(dead_npc->GetTemplateID());
      event_kill_npc.__set_type_(npc_cell->mob_type_);
      event_kill_npc.__set_npc_level_(dead_npc->GetAttribute(entity::RoleAoiFields::LEVEL));
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_ACTOR_KILL_NPC, killer_role->GetID(),
          event::ChannelType::CHANNEL_ACTOR, &event_kill_npc, sizeof(event_kill_npc));
    } else if(dead_role->GetType() == entity::EntityType::TYPE_ACTOR) {
      // 公告
      gateway::protocol::MessageInformNotify inform;
      inform.__set_id_(entity::InformType::SKILL_KILL_ACTOR);
      inform.params_.push_back(((GameActor *)(dead_role->GetGameRole()))->GetStringID());
      inform.params_.push_back(((GameActor *)(dead_role->GetGameRole()))->GetName());
      ((GameActor *)killer_role->GetGameRole())->SendMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
    }
  }
}

}  // namespace skill

}  // namespace server

}  // namespace game

