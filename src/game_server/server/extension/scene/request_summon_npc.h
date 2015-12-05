//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 15:49:05.
// File name: request_summon_npc.h
//
// Description:
// Define class RequestSummonNpc.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SUMMON__NPC__H
#define __GAME__SERVER__SCENE__REQUEST__SUMMON__NPC__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSummonNpc : public core::Noncopyable {
 public:
  RequestSummonNpc();
  ~RequestSummonNpc();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SUMMON__NPC__H

