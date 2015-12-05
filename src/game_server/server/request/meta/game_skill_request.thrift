include "game_request.thrift"
include "entity.thrift"
include "direction.thrift"
include "skill.thrift"

namespace cpp game.server.request
namespace py ccrequest.skill


enum SkillResultType {
  MIN = 0,
  SUCCESS = 0,                                      // 成功
  ERROR_ARGS,                                       // 参数错误
  ERROR_SKILL_ID,                                   // 技能ID错误(不存在)
  ERROR_SKILL_TYPE,                                 // 技能类型错误
  ERROR_SKILL_LEVEL,                                // 技能等级错误(未学习)
  ERROR_SKILL_MASTER,                               // 技能宿主错误
  ERROR_SKILL_SIT,                                  // 技能不在待用状态
  ERROR_HP,                                         // 生命值不足
  ERROR_MP,                                         // 魔法值不足
  ERROR_DISTANCE,                                   // 距离太远
  ERROR_TARGET,                                     // 目标错误
  ERROR_COOLING,                                    // 冷却中
  ERROR_SOUL_STATUS,                                // 幻化状态错误
  ERROR_BUFF_STATUS,                                // BUFF状态错误
  ERROR_SAFEAREA,                                   // 安区域
  ERROR_ROOKIE_PROTECT,                             // 新手保护
  ERROR_DIE_PROTECT_TIME,                           // 死亡保护时间
  ERROR_PK_MODE,                                    // PK模式错误
  ERROR_FORBID_SKILL,                               // 禁止使用技能状态
  ERROR_UNKNOWN,                                    // 未知错误
  MAX,
}


///////////////////////////////////////
// 使用技能
// type: REQUEST_USE_SKILL
struct RequestUseSkill {
  1 : required entity.EntityType type_;             // 使用者类型                                       (IN)
  2 : required i64 id_;                             // 使用者ID                                         (IN)
  3 : required i32 skill_id_;                       // 技能ID(使用者为NPC时，技能ID为0，使用第一个技能) (IN)
  4 : required entity.EntityType target_type_;      // 目标类型                                         (IN)
  5 : required i64 target_id_;                      // 目标ID                                           (IN)
  6 : required i32 x_;                              // 目标点坐标x                                      (IN)
  7 : required i32 y_;                              // 目标点坐标y                                      (IN)
  8 : required i16 direction_;                      // 方向                                             (IN)
  9 : required i32 result_;                         // 结果                                             (OUT)
}



///////////////////////////////////////
// 增加技能
// type: REQUEST_ADD_SKILL
struct RequestAddSkill {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required i32 level_;                          // 技能等级             (IN)
  5 : required bool stand_;                         // 是否设置为立即可用   (IN)
  6 : required i32 result_;                         // 结果                 (OUT)
}



///////////////////////////////////////
// 移除技能
// type: REQUEST_REMOVE_SKILL
struct RequestRemoveSkill {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required i32 result_;                         // 结果                 (OUT)
}



///////////////////////////////////////
// 技能升级
// type: REQUEST_UPGRADE_SKILL
struct RequestUpgradeSkill {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required i32 upgrade_;                        // 级数                 (IN)
  5 : required i32 result_;                         // 结果                 (OUT)
}



///////////////////////////////////////
// 技能启用
// type: REQUEST_SKILL_STAND
struct RequestSkillStand {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required i32 result_;                         // 结果                 (OUT)
}



///////////////////////////////////////
// 技能收回
// type: REQUEST_SKILL_SIT
struct RequestSkillSit {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required i32 result_;                         // 结果                 (OUT)
}



struct SkillBuff {
  1 : required i32 buff_;                           // BUFF                               (IN)
  2 : required i32 odds_;                           // 机率                               (IN)
  3 : required i32 cooling_;                        // 冷却                               (IN)
  4 : required skill.BuffTargetType target_type_;   // BUFF添加类型(0:对自己 1:对其他)    (OUT)
}


///////////////////////////////////////
// 增加技能全局DEBUFF
// type: REQUEST_ADD_SKILL_GLOBAL_DEBUFF
struct RequestAddSkillGlobalBuff {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required list<SkillBuff> buffs_;              // buff列表             (IN)
}



///////////////////////////////////////
// 删除技能全局DEBUFF
// type:REQUEST_REMOVE_SKILL_GLOBAL_DEBUFF
struct RequestRemoveSkillGlobalBuff {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required list<SkillBuff> buffs_;              // buff列表             (IN)
}



///////////////////////////////////////
// 请求技能伤害
// type: REQUEST_SKILL_HURT
struct RequestSkillHurt {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required entity.EntityType src_type_;         // 源类型               (IN)
  4 : required i64 src_id_;                         // 源ID                 (IN)
  5 : required i32 hurt_id_;                        // 伤害ID               (IN)
}



///////////////////////////////////////
// 增加临时技能
// type: REQUEST_SKILL_ADD_TEMP_SKILL
struct RequestSkillAddTempSkill {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required i32 skill_level_;                    // 技能等级             (IN)
  5 : required bool result_;                        // 结果                 (OUT)
}



///////////////////////////////////////
// 删除临时技能
// type: REQUEST_SKILL_REMOVE_TEMP_SKILL
struct RequestSkillRemoveTempSkill {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required bool result_;                        // 结果                 (OUT)
}



///////////////////////////////////////
// 改变技能形态
// type: REQUEST_SKILL_CHANGE_SKILL_FORM
struct RequestSkillChangeSkillForm {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required skill.SkillFormType form_;           // 形态                 (IN)
  4 : required bool result_;                        // 结果                 (OUT)
}



///////////////////////////////////////
// 改变技能属性
// type: REQUEST_SKILL_INCREASE_ATTRIBUTE
struct RequestSkillIncreaseAttribute {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required i32 skill_id_;                       // 技能ID               (IN)
  4 : required skill.SkillVarFields attribute_;     // 属性类型             (IN)
  5 : required i32 increment_;                      // 增量                 (IN)
}



///////////////////////////////////////
// 设置NPC的战斗状态
// type: REQUEST_SKILL_SET_FIGHTING_STATUS
struct RequestSkillSetFightingStatus {
  1 : required entity.EntityType type_;             // 作用者类型           (IN)
  2 : required i64 id_;                             // 作用者ID             (IN)
  3 : required bool status_;                        // 是否在战斗状态       (IN)
}

