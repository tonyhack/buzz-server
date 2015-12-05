//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 14:03:31.
// File name: ai_types.h
//
// Description:
// Define operator type.
//

#ifndef __GAME__SERVER__AI__AI__TYPES__H
#define __GAME__SERVER__AI__AI__TYPES__H

namespace game {

namespace server {

namespace ai {

struct RelationalOperator {
  enum type {
    GREATER = 0,      // 大于
    LESS,             // 小于
    EQUAL,            // 等于
    NO_GREATER,       // 小于等于
    NO_LESS,          // 大于等于
    UNEQUAL,          // 不等于
    MAX,
  };
};

struct SkillTargetType {
  enum type {
    MIN = 0,

    SELF,             // 自己
    SELF_POSITION,    // 自己所在坐标
    TARGET,           // 目标
    TARGET_POSITION,  // 目标所在坐标

    MAX,
  };
};

struct AttrConditionType {
  enum type {
    MIN = 0,

    HP,               // 当前血量值
    HP_PERCENT,       // 当前血量百分比
    MP,               // 当前魔法值
    MP_PERCENT,       // 当前魔法百分比

    MAX,
  };
};

struct AttrType {
  enum type {
    MIN = 0,

    HP,               // 当前血量值
    MP,               // 当前魔法值

    MAX,
  };
};

struct TargetType {
  enum type {
    MIN = 0,

    SELF,             // 自己
    TARGET,           // 目标

    MAX,
  };
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__AI__TYPES__H

