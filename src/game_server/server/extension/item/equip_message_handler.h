//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-06 11:15:44.
// File name: equip_message_handler.h
//
// Description:
// Define class EquipMessageHandler.
//

#ifndef __GAME__SERVER__EQUIP__MESSAGE__HANDLER__H
#define __GAME__SERVER__EQUIP__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class EquipMessageHandler : public core::Noncopyable {
 public:
  EquipMessageHandler();
  ~EquipMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageEquipCompare(core::uint64 id, const char *data, size_t size);
  void OnMessageEquipIntensify(core::uint64 id, const char *data, size_t size);
  void OnMessageEquipRefine(core::uint64 id, const char *data, size_t size);
  void OnMessageEquipUpgrade(core::uint64 id, const char *data, size_t size);
  void OnMessageEquipRefineConfirm(core::uint64 id, const char *data, size_t size);
  void OnMessageEquipInherit(core::uint64 id, const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EQUIP__MESSAGE__HANDLER__H

