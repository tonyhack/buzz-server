//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:31:07.
// File name: request_scene_create_map.h
//
// Description:
// Define class RequestSceneCreateMap.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__CREATE__MAP__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__CREATE__MAP__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneCreateMap : public core::Noncopyable {
 public:
  RequestSceneCreateMap();
  ~RequestSceneCreateMap();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestCreateMap(void *data, size_t size);
  int OnRequestDestoryMap(void *data, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__CREATE__MAP__H

