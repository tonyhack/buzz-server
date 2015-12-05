//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-22 18:16:19.
// File name: request_scene_get_npcs.h
//
// Description:
// Define class RequestSceneGetNpcs.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__GET__NPCS__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__GET__NPCS__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneGetNpcs : public core::Noncopyable {
 public:
  RequestSceneGetNpcs();
  ~RequestSceneGetNpcs();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *data, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__GET__NPCS__H

