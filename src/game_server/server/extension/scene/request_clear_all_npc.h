//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-12 14:35:47.
// File name: request_clear_all_npc.h
//
// Description:
// Define class RequestClearAllNpc.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__CLEAR__ALL__NPC__H
#define __GAME__SERVER__SCENE__REQUEST__CLEAR__ALL__NPC__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestClearAllNpc : public core::Noncopyable {
 public:
  RequestClearAllNpc();
  ~RequestClearAllNpc();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__CLEAR__ALL__NPC__H

