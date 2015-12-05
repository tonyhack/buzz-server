//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-03 19:57:38.
// File name: skill_types.h
//
// Description:
// Define skill types.
//

#ifndef __GAME__SERVER__SKILL__SKILL__TYPES__H
#define __GAME__SERVER__SKILL__SKILL__TYPES__H

namespace game {

namespace server {

namespace skill {

// 技能类型
struct SkillType {
  enum type {
    MIN = 0,
    MOMENTARY,                // 瞬时类(有目标，可以是实体或坐标)
    REMOTE,                   // 远程类(有目标，可以是实体或坐标)
    COLLISION,                // 碰撞类(无目标，飞行碰撞类技能)
    MAX,
  };
};

// 技能类别
struct ClassifyType {
  enum type {
    MIN = 0,
    COMMON,                   // 普通技能
    SOUL,                     // 幻化技能
    FINAL,                    // 必杀技能
    MAX,
  };
};


// 目标类型
struct TargetType {
  enum type {
    MIN = 0,
    ENTITY,                   // 实体目标
    POSITION,                 // 坐标目标
    POSITION_SELF,            // 自己为坐标
    NONE,                     // 无目标
    MAX,
  };
};


// 击打的实体目标类型
struct DestType {
  enum type {
    MIN = 0,
    NPC = 0X1,                // NPC
    ACTOR = 0X2,              // 玩家
    MAX,
  };
};


// 范围类型
struct RangeType {
  enum type {
    MIN = 0,
    SINGLE,                   // 单体
    CIRCLE,                   // 圆
    SECTOR_QUARTER,           // 90度扇形
    SECTOR_HALF,              // 180度扇形
    MAX,
  };
};

// 状态类型
struct StatusType {
  enum type {
    MIN = 0,
    LIVE,                     // 活的
    DEAD,                     // 死的
    MAX,
  };
};


// 技能影响类型(多选)
struct AffectType {
  enum type {
    MIN = 0,
    TEAM = 0x1,               // 队伍
    OTHER_TEAM = 0x2,         // 非队伍
    SELF = 0x4,               // 自己
    OTHER_SELF = 0x8,         // 非自己
    MAX,
  };
};


// 宿主类型(多选)
struct MasterType {
  enum type {
    MIN = 0,
    NPC = 0X1,                // NPC
    ACTOR = 0X2,              // 玩家
    MAX,
  };
};


struct SoulStatusType {
  enum type {
    MIN = 0,
    ANY = 0,                  // 都可以使用
    NOT_SOUL,                 // 非幻化状态才可使用
    SOUL,                     // 幻化状态才可使用
    MAX,
  };
};


// 技能结果
struct SkillResultType {
  enum type {
    MIN = 0,
    SUCCESS = 0,              // 成功
    ERROR_ARGS,               // 参数错误
    ERROR_SKILL_ID,           // 技能ID错误(不存在)
    ERROR_SKILL_TYPE,         // 技能类型错误
    ERROR_SKILL_LEVEL,        // 技能等级错误(未学习)
    ERROR_SKILL_MASTER,       // 技能宿主错误
    ERROR_SKILL_SIT,          // 技能不在待用状态
    ERROR_HP,                 // 生命值不足
    ERROR_MP,                 // 魔法值不足
    ERROR_DISTANCE,           // 距离太远
    ERROR_TARGET,             // 目标错误
    ERROR_COOLING,            // 冷却中
    ERROR_SOUL_STATUS,        // 幻化状态错误
    ERROR_BUFF_STATUS,        // BUFF状态错误
    ERROR_SAFEAREA,           // 安区域
    ERROR_ROOKIE_PROTECT,     // 新手保护
    ERROR_DIE_PROTECT_TIME,   // 死亡保护时间
    ERROR_PK_MODE,            // PK模式错误
    ERROR_FORBID_SKILL,       // 禁止使用技能状态
    ERROR_UNKNOWN,            // 未知错误
    MAX,
  };
};

// 技能增益减益类型
struct FriendType{
  enum type {
    HURT = 0,                 // 减益
    HELP,                     // 增益
  };
};

// 判断安全区结果
struct SafeAreaResultType {
  enum type {
    ERROR = 0,                // 错误
    SAFE,                     // 安全区
    NO_SAFE,                  // 非安全区
  };
};

// 吟唱禁止移动类型
struct SkillMoveType {
  enum type {
    NO_MOVE = 0,              // 禁止移动
    MOVE = 1,                 // 容许移动
  };
};

// 命中类型
struct SkillHitType {
  enum type {
    MIN = 0,
    COMMON = 0x1,             // 普通
    DODGE = 0x2,              // 闪避
    CRIT = 0x4,               // 暴击
    IGNORE_DEFENCE = 0x8,     // 忽视防御
    MAX,
  };
};

// 附加AOE条件
struct ExtraAoeConditionType {
  enum type {
    MIN = 0,
    SOUL_ENERGY_EXPLOSION,    // 爆发状态
    MAX,
  };
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__TYPES__H

