//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-05 16:49:41.
// File name: item_types.h
//
// Description:
// Define item types.
//

#ifndef __GAME__SERVER__ITEM__ITEM__TYPES__H
#define __GAME__SERVER__ITEM__ITEM__TYPES__H

namespace game {

namespace server {

namespace item {

// 道具使用功能类型
struct UseFuncType {
  enum type {
    NONE = 0,               // 不使用
    ADD_BUFF,               // 添加buff
    ADD_ITEM,               // 添加道具
    ADD_SOUL,               // 增加一个英灵
    ADD_EXP,                // 增加经验 
    ADD_HP,                 // 增加血量 
    ADD_MP,                 // 增加魔法
    ADD_NIMBUS,             // 增加灵气
    OPEN_RISK,              // 开启历险任务
    OPEN_TRIP,              // 开启奇遇任务
    USE_VIP,                // 使用VIP
    OPEN_FUNCTIONALITY,     // 开启功能
    ADD_ENERGY,             // 增加能量
    ADD_HP_TASLIMAN,        // 增加血护符值
    ADD_MP_TASLIMAN,        // 增加蓝护符值
    MAX,
  };
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__TYPES__H

