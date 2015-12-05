//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-19 17:52:22.
// File name: soul_message_handler.h
//
// Description:
// Define class SoulMessageHandler.
//

#ifndef __GAME__SERVER__SOUL__SOUL__MESSAGE__HANDLER__H
#define __GAME__SERVER__SOUL__SOUL__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace soul {

class SoulMessageHandler : public core::Noncopyable {
 public:
  SoulMessageHandler();
  ~SoulMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageSoulUnlockRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageSoulUpgradeStepRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageSoulUpgradeLevelRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageSoulSetRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageSoulResetRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageSoulSettingStandRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageSoulSkillUpgradeRequest(core::uint64 id, const char *data, size_t size);

 /*
  void OnMessageAddExp(core::uint64 id, const char *data, size_t size);

  void OnMessageSoulStand(core::uint64 id, const char *data, size_t size);

  void OnMessageSoulSit(core::uint64 id, const char *data, size_t size);

  void OnMessageSoulSettingStand(core::uint64 id, const char *data, size_t size);
  */
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__MESSAGE__HANDLER__H

