//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-25 11:08:01.
// File name: extension_types.h
//
// Description:
// Define extension types.
//

#ifndef __SESSION__SERVER__EXTENSION__TYPES__H
#define __SESSION__SERVER__EXTENSION__TYPES__H

namespace session {

namespace server {

struct ExtensionType {
  enum type {
    MIN = 0,

    TEST = 0,                 // 测试模块
    GM,                       // GM模块
    SCENE,                    // 场景模块
    CHAT,                     // 聊天模块
    SOCIAL,                   // 社交模块
    TEAM,                     // 组队模块
    GUILD,                    // 公会模块
    PLAYING,                  // 副本模块
    AUCTION,                  // 拍卖模块

    MAX,
  };
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__EXTENSION__TYPES__H

