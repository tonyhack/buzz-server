#ifndef __GAME__SERVER__WELFARE__ITEM__EVENT__HANDLER__H
#define __GAME__SERVER__WELFARE__ITEM__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace welfare {

class ItemEventHandler {
 public:
  ItemEventHandler();
  ~ItemEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventEquipStrength(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventEquipWear(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventUseItem(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_equip_strength_;
  coresh::Communicator::Connection conn_equip_wear_;
  coresh::Communicator::Connection conn_use_item_;

};

} // namespace welfare

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__WELFARE__ITEM__EVENT__HANDLER__H
