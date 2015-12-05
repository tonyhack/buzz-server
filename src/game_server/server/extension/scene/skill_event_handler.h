//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-03 20:23:49.
// File name: skill_event_handler.h
//
// Description:
// Define class SkillEventHandler.
//

#ifndef __GAME__SERVER__SCENE__SKILL__EVENT__HANDLER__H
#define __GAME__SERVER__SCENE__SKILL__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace scene {

class SkillEventHandler : public core::Noncopyable {
 public:
  SkillEventHandler();
  ~SkillEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventSkillAttackedHit(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_npc_attacked_hit_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SKILL__EVENT__HANDLER__H

