//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 18:57:29.
// File name: skill_configure.h
//
// Description:
// Define class SkillConfigure.
//

#ifndef __GAME__SERVER__SKILL__SKILL__CONFIGURE__H
#define __GAME__SERVER__SKILL__SKILL__CONFIGURE__H

#include <map>
#include <vector>

#include "core/base/types.h"
#include "game_server/server/extension/skill/skill_types.h"

namespace game {

namespace server {

namespace skill {

class SkillCell {
 public:
  // 技能 ID/组/等级
  core::uint32 id_;
  core::uint32 group_;
  core::uint32 level_;

  // 冷却ID
  core::uint32 cooling_;

  // 技能类型(1:近身瞬时类 2:远身瞬时坐标目标类 3:远身瞬时实体目标类 4:远身非瞬时坐标目标类 5:远身非瞬时实体目标类 6:无目标碰撞类)
  core::uint8 type_;

  // 技能类别(ClassifyType::type)
  core::uint32 classify_type_;

  // 技能命中类型(SkillHitType::type)
  core::uint32 hit_type_;

  // 目标类型(1:实体对象 2:坐标 3:无)
  core::uint8 target_type_;

  // 目标实体类型(1:怪物(NPC) 2:玩家)  多选
  core::uint16 dest_type_;

  // 范围类型(1:单体 2:圆形)
  core::uint16 range_type_;

  // 目标状态类型(1:活的 2:死亡) 多选
  core::uint16 status_type_;

  // 释放目标的阵营类型(1:敌方 2:友方 3:本队伍 4:非本队) 多选
  core::uint16 cast_type_;

  // 命中阵营类型(1:敌方 2:友方 3:本队伍 4:非本队) 多选
  core::uint16 camp_type_;

  // 宿主类型(1:怪物与NPC 2:玩家) 多选
  core::uint16 master_type_;

  // 友好类型(0:减益 1:增益)
  core::uint16 friend_type_;

  // AOE最大目标数
  core::int16 aoe_max_target_;
  // AOE半径
  core::int16 aoe_radius_;

  // 吟唱时间(单位毫秒)
  core::int32 sing_time_;

  // 技能释放距离(单位格子)
  core::int32 distance_;

  // 技能吟唱中断距离(单位格子，吟唱结束后判断的距离)
  core::int32 sing_distance_;

  // 技能飞行速度
  core::int32 speed_;

  // 技能消耗 生命值/魔法值
  core::int32 spend_hp_;
  core::int32 spend_hp_per_;
  core::int32 spend_mp_;
  core::int32 spend_mp_per_;

  // 技能消耗道具
  core::int32 spend_item_;
  core::int32 spend_item_num_;

  // 幻化状态条件(0:无需求 1:非幻化状态 2:幻化状态)
  core::uint16 soul_status_;

  // BUFF状态条件(0:无需要 >0:BUFFID)
  core::int32 buff_status_;

  // 战斗状态条件 0:无需求 1:非战斗状态 2:战斗状态
  core::uint8 cond_war_status_; 
  // 时间条件  0:无需求 1:白天 2:黑夜
  core::uint8 cond_day_night_;

  // 伤害计算ID
  core::uint32 hurt_id_;

  // 技能转换到收回状态时，需要打断的BUFF
  std::vector<core::int32> sit_break_buffs_;

  // 需要触发的buff列表(机率, BUFF的ID)
  std::map<core::uint32 ,core::uint32> buffs_;

  SkillMoveType::type move_;

  // 附加AOE条件
  ExtraAoeConditionType::type extra_aoe_condition_;

  // 附加AOE目标实体类型(1:怪物(NPC) 2:玩家)  多选
  core::uint16 extra_aoe_dest_type_;

  // 附加AOE 最大数量
  core::int32 extra_aoe_max_target_;

  // 附加AOE半径
  core::int32 extra_aoe_radius_;

  // 附加AOE释放目标的阵营类型(1:敌方 2:友方 3:本队伍 4:非本队) 多选
  core::int16 extra_aoe_cast_type_;

  // 附加AOE命中阵营类型(1:敌方 2:友方 3:本队伍 4:非本队) 多选
  core::int16 extra_aoe_camp_type_;

  // 附加AOE伤害ID
  core::int32 extra_aoe_hurt_id_;

  // 附加AOE友好类型
  core::int16 extra_aoe_friend_type_;
};

class HurtCell {
 public:
  // 伤害类型
  core::uint16 type_;

  // 2个参数
  core::uint32 param1_;
  core::uint32 param2_;

  // 仇恨值/系数
  core::uint32 threat_value_;
  core::uint32 threat_coe_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__CONFIGURE__H

