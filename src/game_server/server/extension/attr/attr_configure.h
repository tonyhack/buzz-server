#ifndef __GAME__SERVER__ATTR__ATTR__CONFIGURE__H
#define __GAME__SERVER__ATTR__ATTR__CONFIGURE__H

#include <vector>

#include "core/base/types.h"
#include "entity/skill_types.h"

namespace game {

namespace server {

namespace attr {

class AttrBonusCell {
 public:
  // 奖励属性ID
  core::int32 id_;

  // 体质加成值
  core::int32 physique_add_value_;

  // 力量加成值
  core::int32 strength_add_value_;

  // 智力加成值
  core::int32 intelligence_add_value_;

  // 敏捷加成值
  core::int32 agile_add_value_;

  // 精神加成值
  core::int32 spirit_add_value_;

  // 最大血量加成值
  core::int32 max_hp_add_value_;

  // 最大魔法加成值
  core::int32 max_mp_add_value_;

  // 物理攻击加成值
  core::int32 physics_attack_add_value_;

  // 物理防御加成值
  core::int32 physics_defence_add_value_;

  // 魔法攻击加成值
  core::int32 magic_attack_add_value_;

  // 魔法防御加成值
  core::int32 magic_defence_add_value_;

  // 闪避等级加成值
  core::int32 dodge_add_value_;

  // 暴击等级加成值
  core::int32 crit_add_value_;

  // 会心等级加成值
  core::int32 ignore_defence_add_value_;

  // 移动速度加成值
  core::int32 move_speed_add_value_;

  // 闪避率加成值(万分之)
  core::int32 dodge_percent_add_value_;

  // 暴击率加成值(万分之)
  core::int32 crit_percent_add_value_;

  // 会心率加成值(万分之)
  core::int32 ignore_defence_percent_add_value_;

  // 物伤减免率(万分之)
  core::int32 physics_hurt_reduce_add_percent_;

  // 法伤减免率(万分之)
  core::int32 magic_hurt_reduce_add_percent_;

  // 伤害减免率(万分之)
  core::int32 hurt_reduce_add_percent_;

  // 伤害浮动比(万分之)
  core::int32 hurt_random_percent_add_;

  // 治疗加成比(万分之)
  core::int32 cure_percent_add_;
};

class ConstellationCell {
 public:
  // 激活编号
  core::int32 id_;

  // 奖励属性ID
  core::int32 attr_bonus_id_;

  // 额外奖励属性ID
  core::int32 attr_bonus_id_2_;

  // 成功几率
  core::uint32 odds_;

  // 失败获得经验
  core::int32 failed_exp_reward_;

  // 消耗灵气
  core::int32 spend_nimbus_;

  // 消耗金币
  core::int32 spend_gold_;

  // 累计最大血量加成值
  core::int32 max_hp_add_value_;

  // 累计最大魔法加成值
  core::int32 max_mp_add_value_;

  // 累计物理攻击加成值
  core::int32 physics_attack_add_value_;

  // 累计物理防御加成值
  core::int32 physics_defence_add_value_;

  // 累计魔法攻击加成值
  core::int32 magic_attack_add_value_;

  // 累计魔法防御加成值
  core::int32 magic_defence_add_value_;

  // 累计闪避等级加成值
  core::int32 dodge_add_value_;

  // 累计暴击等级加成值
  core::int32 crit_add_value_;

  // 累计会心等级加成值
  core::int32 ignore_defence_add_value_;
};

class NobleCell {
 public:
  // 爵位编号
  core::int32 id_;

  // 提升消耗灵气
  core::int32 spend_nimbus_;

  // 突破消耗道具ID
  core::int32 spend_item_id_;

  // 突破消耗道具数量
  core::int32 spend_item_count_;

  // 突破消耗金币(每道具)
  core::int32 spend_gold_;

  // 最大血量加成值上限
  core::int32 max_hp_add_value_limit_;

  // 最大血量加成值增量
  core::int32 max_hp_add_value_increment_;

  // 最大魔法加成值上限
  core::int32 max_mp_add_value_limit_;

  // 最大魔法加成值增量
  core::int32 max_mp_add_value_increment_;

  // 物理攻击加成值上限
  core::int32 physics_attack_add_value_limit_;

  // 物理攻击加成值增量
  core::int32 physics_attack_add_value_increment_;

  // 物理防御加成值上限
  core::int32 physics_defence_add_value_limit_;

  // 物理防御加成值增量
  core::int32 physics_defence_add_value_increment_;

  // 魔法攻击加成值上限
  core::int32 magic_attack_add_value_limit_;

  // 魔法攻击加成值增量
  core::int32 magic_attack_add_value_increment_;

  // 魔法防御加成值上限
  core::int32 magic_defence_add_value_limit_;

  // 魔法防御加成值增量
  core::int32 magic_defence_add_value_increment_;

  // 闪避等级加成值上限
  core::int32 dodge_add_value_limit_;

  // 闪避等级加成值增量
  core::int32 dodge_add_value_increment_;

  // 暴击等级加成值上限
  core::int32 crit_add_value_limit_;

  // 暴击等级加成值增量
  core::int32 crit_add_value_increment_;

  // 会心等级加成值上限
  core::int32 ignore_defence_add_value_limit_;

  // 会心等级加成值增量
  core::int32 ignore_defence_add_value_increment_;

};

// 魔剑配置
class SwordBuff {
 public:
  // 给目标增加的BUFF
  core::int32 buff_;

  // 给目标增加BUFF的机率
  core::int32 buff_odds_;

  // 冷却时间
  core::int32 buff_cooling_;

  // 释放对象是自己还是其他
  entity::BuffTargetType::type target_type_;
};

class SwordCell {
 public:
  typedef std::vector<SwordBuff> BuffVector;

  // 阶
  core::int32 step_;

  // 等级上限
  core::int32 max_level_;

  // 失效时间(为0时表示永不失效)
  core::int32 active_hours_;

  // BUFF
  BuffVector buffs_;

  // 升到本阶所需要的游戏币
  core::int32 spend_gold_;

  // 升到本阶所需要的道具
  core::int32 spend_item_;

  // 升到本阶所需要的道具数量
  core::int32 spend_item_number_;

  // 升阶失败可获得的幸运值(随机)
  core::int32 lucky_;

  // 最大幸运值
  core::int32 max_lucky_;

  // 幸运系数
  core::int32 lucky_coe_;

  // 累计最大血量加成值
  core::int32 max_hp_add_value_;

  // 累计最大魔法加成值
  core::int32 max_mp_add_value_;

  // 累计物理攻击加成值
  core::int32 physics_attack_add_value_;

  // 累计物理防御加成值
  core::int32 physics_defence_add_value_;

  // 累计魔法攻击加成值
  core::int32 magic_attack_add_value_;

  // 累计魔法防御加成值
  core::int32 magic_defence_add_value_;

  // 累计闪避等级加成值
  core::int32 dodge_add_value_;

  // 累计暴击等级加成值
  core::int32 crit_add_value_;

  // 累计会心等级加成值
  core::int32 ignore_defence_add_value_;

  // 升阶机率
  core::int32 upgrade_odds_;
};

// 摩剑等级配置
class SwordLevelCell {
 public:
  // 等级
  core::int32 level_;

  // 累计最大血量加成值
  core::int32 max_hp_add_value_;

  // 累计最大魔法加成值
  core::int32 max_mp_add_value_;

  // 累计物理攻击加成值
  core::int32 physics_attack_add_value_;

  // 累计物理防御加成值
  core::int32 physics_defence_add_value_;

  // 累计魔法攻击加成值
  core::int32 magic_attack_add_value_;

  // 累计魔法防御加成值
  core::int32 magic_defence_add_value_;

  // 累计闪避等级加成值
  core::int32 dodge_add_value_;

  // 累计暴击等级加成值
  core::int32 crit_add_value_;

  // 累计会心等级加成值
  core::int32 ignore_defence_add_value_;
};

class ElementCell {
 public:
  // 元素ID 
  core::int32 id_;

  // 奖励属性ID
  core::int32 attr_bonus_id_;
  
  // 激活消耗神威
  core::int32 spend_war_soul_;

  // 累计最大血量加成值
  core::int32 max_hp_add_value_;

  // 累计最大魔法加成值
  core::int32 max_mp_add_value_;

  // 累计物理攻击加成值
  core::int32 physics_attack_add_value_;

  // 累计物理防御加成值
  core::int32 physics_defence_add_value_;

  // 累计魔法攻击加成值
  core::int32 magic_attack_add_value_;

  // 累计魔法防御加成值
  core::int32 magic_defence_add_value_;

  // 累计闪避等级加成值
  core::int32 dodge_add_value_;

  // 累计暴击等级加成值
  core::int32 crit_add_value_;

  // 累计会心等级加成值
  core::int32 ignore_defence_add_value_;
};

class SpiritCell {
 public:
  // 精灵等级
  core::int32 level_;

  // 体质加成值
  core::int32 physique_add_value_;

  // 力量加成值
  core::int32 strength_add_value_;

  // 智力加成值
  core::int32 intelligence_add_value_;

  // 敏捷加成值
  core::int32 agile_add_value_;

  // 精神加成值
  core::int32 spirit_add_value_;

  // 移动速度加成值
  core::int32 move_speed_add_value_;

  // 进阶道具ID
  core::int32 upgrade_spend_item_id_;

  // 进阶道具消耗
  core::int32 upgrade_spend_item_count_;

  // 野兽外形ID
  core::int32 horse_facade_id_;

  // 翅膀外形ID
  core::int32 wing_facade_id_;

  // 野兽增益ID
  core::int32 horse_buff_id_;

  // 翅膀增益ID
  core::int32 wing_buff_id_;

  // 进阶失败可获得的幸运值(随机)
  core::int32 lucky_;

  // 最大幸运值
  core::int32 max_lucky_;

  // 幸运值系数
  core::int32 lucky_coe_;

  // 进阶基础成功率
  core::int32 upgrade_odds_;
};

class SpiritFacadeCell {
 public:
  // 外形ID
  core::int32 id_;

  // 所属形态
  core::int32 facade_type_;

  // 体质加成值
  core::int32 physique_add_value_;

  // 力量加成值
  core::int32 strength_add_value_;

  // 智力加成值
  core::int32 intelligence_add_value_;

  // 敏捷加成值
  core::int32 agile_add_value_;

  // 精神加成值
  core::int32 spirit_add_value_;
};

class LegendaryWeaponPieceCell {
 public:
  // 道具ID
  core::uint32 item_id_;

  // 道具数量
  core::int32 item_count_;

  // 修复经验奖励
  core::int32 exp_reward_;

  // 修复灵气奖励
  core::int32 nimbus_reward_;
};

class LegendaryWeaponCell {
 public:
  // 名武ID
  core::int32 id_;

  // 名武碎片
  std::vector<LegendaryWeaponPieceCell> pieces_;

  // 完全修复奖励属性ID
  core::int32 attr_bonus_id_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ATTR__CONFIGURE__H

