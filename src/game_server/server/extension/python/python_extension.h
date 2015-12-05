//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-02 16:17:19.
// File name: python_extension.h
//
// Description:
// Define class PythonExtension.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__EXTENSION__H
#define __GAME__SERVER__PYTHON__PYTHON__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/python/actor_login_event_handler.h"
#include "game_server/server/extension/python/actor_logout_event_handler.h"
#include "game_server/server/extension/python/npc_create_destory_event_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace python {

class PythonExtension : public global::SingletonFactory<PythonExtension> {
 friend class global::SingletonFactory<PythonExtension>;
 public:
  PythonExtension();
  ~PythonExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  NpcCreateDestoryEventHandler npc_create_destory_event_;
};

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__EXTENSION__H

