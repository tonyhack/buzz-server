include "entity.thrift"
include "game_event.thrift"
include "skill.thrift"

namespace cpp game.server.event
namespace py ccevent.skill

// 使用技能
// type: EVENT_SKILL_USE_SKILL
struct EventUseSkill {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required i32 skill_id_;                     // 技能ID
  4 : required i32 skill_group_;                  // 技能组ID
  5 : required i32 skill_type_;                   // 技能类型
}

// 受到技能攻击
// type: EVENT_SKILL_ATTACKED_SKILL
struct EventAttackedSkill {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required i32 skill_id_;                     // 技能ID
  4 : required i32 friend_type_;                  // 技能友好类型
}

// 使用幻化技能
// type: EVENT_SKILL_USE_APPEAR
struct EventUseAppearSkill {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
}

// 攻击命中
// type: EVENT_SKILL_ATTACK_HIT
struct EventSkillAttackHit {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required entity.EntityType dest_type_;      // 目标类型
  4 : required i64 dest_id_;                      // 目标ID
  5 : required skill.SkillHitType hit_type_;      // 命中类型
  6 : required skill.SkillHurtType hurt_type_;    // 伤害类型
  7 : required i32 value_;                        // 命中数值
}

// 被攻击命中
// type: EVENT_SKILL_ATTACKED_HIT
struct EventSkillAttackedHit {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required entity.EntityType src_type_;       // 目标类型
  4 : required i64 src_id_;                       // 目标ID
  5 : required skill.SkillHitType hit_type_;      // 命中类型
  6 : required skill.SkillHurtType hurt_type_;    // 伤害类型
  7 : required i32 value_;                        // 命中数值
}

