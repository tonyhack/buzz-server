//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-13 16:53:28.
// File name: playing_types.h
//
// Description:
// Define playing types.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__TYPES__H
#define __SESSION__SERVER__PLAYING__PLAYING__TYPES__H

namespace session {

namespace server {

namespace playing {

struct StatusType {
  enum type {
    MIN = 0,

    CREATE,                             // 创建中
    RUN,                                // 运行中
    END,                                // 结束
    DESTORY,                            // 销毁

    MAX,
  };
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__TYPES__H

