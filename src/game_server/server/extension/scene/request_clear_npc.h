//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 11:50:24.
// File name: request_clear_npc.h
//
// Description:
// Define class RequestClearNpc.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__CLEAR__NPC__H
#define __GAME__SERVER__SCENE__REQUEST__CLEAR__NPC__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestClearNpc : public core::Noncopyable {
 public:
  RequestClearNpc();
  ~RequestClearNpc();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__CLEAR__NPC__H

