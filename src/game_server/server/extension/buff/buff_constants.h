

#ifndef __GAME__SERVER__BUFF__BUFF_CONSTANST__H
#define __GAME__SERVER__BUFF__BUFF_CONSTANST__H


namespace game {

namespace server {

namespace buff {

//buff 广播类型
struct BuffBoardcastType {
  enum type {
    boardcast_none = 0x00,      //不广播
    boardcast_single = 0x01,    //只广播自己
    boardcast_all = 0x02,
    //boardcast_all = 0x03,


  };
};

//特殊结束条件(1:死亡, 2:受伤, 3:被治疗, 4:使用技能, 5:使用道具)
struct BuffEndConditionType {
  enum type {
    actor_die     = 1, //角色死亡
    actor_injured = 2, //受伤
    actor_cured   = 3, //被治疗
    use_skill     = 4, //使用技能
    use_item      = 5, //使用道具
  };
};



}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF_CONSTANST__H

