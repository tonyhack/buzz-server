//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-17 11:53:26.
// File name: request_scene_daynightinfo.h
//
// Description:
// Define class RequestSceneDayNightInfo.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__DAYNIGHTINFO__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__DAYNIGHTINFO__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneDayNightInfo : public core::Noncopyable {
 public:
  RequestSceneDayNightInfo();
  ~RequestSceneDayNightInfo();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__DAYNIGHTINFO__H
