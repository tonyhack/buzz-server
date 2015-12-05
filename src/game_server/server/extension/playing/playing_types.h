//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-12 11:20:38.
// File name: playing_types.h
//
// Description:
// Define playing types.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__TYPES__H
#define __GAME__SERVER__PLAYING__PLAYING__TYPES__H

namespace game {

namespace server {

namespace playing {

// 副本状态类型
struct StatusType {
  enum type {
    MIN = 0,

    RUN,                                // 运行中
    END,                                // 结束

    MAX,
  };
};

// 副本页类型(未领取的副本结果)
struct PageType {
  enum type {
    MIN = 0,

    SCORE,                              // 分数

    MAX,
  };
};

}  // namespace playing_types

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__TYPES__H

