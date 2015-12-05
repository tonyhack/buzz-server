//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-29 10:12:07.
// File name: session_message_handler.h
//
// Description:
// Define class SessionMessageHandler.
//

#ifndef __GAME__SERVER__SCENE__SESSION__MESSAGE__HANDLER__H
#define __GAME__SERVER__SCENE__SESSION__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace scene {

class SessionMessageHandler : public core::Noncopyable {
 public:
  SessionMessageHandler();
  ~SessionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageSceneLoginResponse(const char *data, size_t size);
  void OnMessageSceneBatchLoginResponse(const char *data, size_t size);
  void OnMessageSceneLogoutResponse(const char *data, size_t size);
  void OnMessageSceneJumpNotice(const char *data, size_t size);
  void OnMessageSceneSummonWorldBoss(const char *data, size_t size);
  void OnMessageSceneActorJumpMapResponse(const char *data, size_t size);
  void OnMessageSceneJumpSceneResponse(const char *data, size_t size);
  void OnMessageSceneNpcJumpSynchronize(const char *data, size_t size);
  void OnMessageSceneBranchMapSynchronize(const char *data, size_t size);
  void OnMessageSceneActorSwitchBranchNotice(const char *data, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SESSION__MESSAGE__HANDLER__H

