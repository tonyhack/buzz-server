//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-08-03 15:15:33.
// File name: scene_message_handler.h
//
// Description:
// Define class SceneMessageHandler.
//

#ifndef __SESSION__SERVER__SCENE__SCENE__MESSAGE__HANDLER__H
#define __SESSION__SERVER__SCENE__SCENE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"

namespace session {

namespace server {

namespace scene {

class SceneMessageHandler : public core::Noncopyable {
 public:
  SceneMessageHandler();
  ~SceneMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageSceneBranchSwitchRequest(core::uint64 id, const char *data, size_t size);
  void OMessageSceneBranchStatusRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__SCENE__MESSAGE__HANDLER__H

