//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-18 13:36:56.
// File name: request_scene_pk_check.h
//
// Description:
// Define RequestScenePKCheck.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__PK__CHECK__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__PK__CHECK__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestScenePKCheck : public core::Noncopyable {
 public:
  RequestScenePKCheck();
  ~RequestScenePKCheck();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__PK__CHECK__H

