//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-18 13:36:56.
// File name: request_scene_pkinfo.h
//
// Description:
// Define RequestScenePKInfo.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__PKINFO__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__PKINFO__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestScenePKInfo : public core::Noncopyable {
 public:
  RequestScenePKInfo();
  ~RequestScenePKInfo();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__PKINFO__H

