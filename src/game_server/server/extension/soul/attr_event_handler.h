//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-12 21:27:08.
// File name: attr_event_handler.h
//
// Description:
// Define class AttrEventHandler.
//

#ifndef __GAME__SERVER__SOUL__ATTR__EVENT__HANDLER__H
#define __GAME__SERVER__SOUL__ATTR__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace soul {

class AttrEventHandler : public core::Noncopyable {
 public:
  AttrEventHandler();
  ~AttrEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventAttrSpiritUpgrade(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventAttrSwordUpgrade(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventAttrNobleUpgrade(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_spirit_upgrade_;
  coresh::Communicator::Connection conn_sword_upgrade_;
  coresh::Communicator::Connection conn_noble_upgrade_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__ATTR__EVENT__HANDLER__H

