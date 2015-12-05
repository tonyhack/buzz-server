//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 17:30:39.
// File name: gm_extension.h
//
// Description:
// Define class GmExtension.
//

#ifndef __GAME__SERVER__GM__GM__EXTENSION__H
#define __GAME__SERVER__GM__GM__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/gm/gm_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace gm {

class GmExtension : public global::SingletonFactory<GmExtension> {
 friend class global::SingletonFactory<GmExtension>;
 public:
  GmExtension();
  ~GmExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // GM消息回调
  GmMessageHandler gm_message_handler_;
};

}  // namespace gm

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GM__GM__EXTENSION__H

