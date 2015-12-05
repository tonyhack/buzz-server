//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-29 12:07:58.
// File name: request_get_map_scene_id.h
//
// Description:
// Define class RequestGetMapSceneID.
//

#ifndef __SESSION__SERVER__SCENE__REQUEST__GET__MAP__SCENE__ID__H
#define __SESSION__SERVER__SCENE__REQUEST__GET__MAP__SCENE__ID__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace scene {

class RequestGetMapSceneID : public core::Noncopyable {
 public:
  RequestGetMapSceneID();
  ~RequestGetMapSceneID();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__REQUEST__GET__MAP__SCENE__ID__H

