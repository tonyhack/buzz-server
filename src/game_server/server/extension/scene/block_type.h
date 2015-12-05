//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 15:05:58.
// File name: block_type.h
//
// Description:
// Define type of block.
//

#ifndef __GAME__SERVER__SCENE__BLOCK__TYPE__H
#define __GAME__SERVER__SCENE__BLOCK__TYPE__H

namespace game {

namespace server {

namespace scene {

struct BlockType {
  enum type {
    BLOCK_WALK = 0x1,
    BLOCK_SAFE = 0x2, // 安全区域
  };
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__BLOCK__TYPE__H

