//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 12:07:13.
// File name: request_scene_relocation.h
//
// Description:
// Define class RequestSceneRelocation.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__RELOCATION__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__RELOCATION__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneRelocation : public core::Noncopyable {
 public:
  RequestSceneRelocation();
  ~RequestSceneRelocation();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  //  namespace server

}  // namespace game



#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__RELOCATION__H

