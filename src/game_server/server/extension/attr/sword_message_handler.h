//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-08 10:30:36.
// File name: sword_message_handler.h
//
// Description:
// Define SwordMessageHandler.
//

#ifndef __GAME__SERVER__ATTR__SWORD__MESSAGE__HANDLER__H
#define __GAME__SERVER__ATTR__SWORD__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class AttrActor;

class SwordMessageHandler : public core::Noncopyable {
 public:
  SwordMessageHandler();
  ~SwordMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageUpgradeRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageEquip(core::uint64 id, const char *data, size_t size);
  void OnMessageUnequip(core::uint64 id, const char *data, size_t size);
  void OnMessageCompareRequest(core::uint64 id, const char *data, size_t size);

  void SendUpgradeFailed(AttrActor *actor);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__SWORD__MESSAGE__HANDLER__H

