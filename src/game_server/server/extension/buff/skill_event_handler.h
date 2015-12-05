//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-25 10:47:26.
// File name: skill_event_handler.h
//
// Description:
// Define class SkillEventHandler.
//

#ifndef __GAME__SERVER__BUFF__SKILL__EVENT__HANDLER__H
#define __GAME__SERVER__BUFF__SKILL__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace buff {

class SkillEventHandler : public core::Noncopyable {
 public:
  SkillEventHandler();
  ~SkillEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventAttackHit(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventAttackedHit(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSkillUse(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_npc_attack_hit_;
  coresh::Communicator::Connection conn_actor_attack_hit_;

  coresh::Communicator::Connection conn_npc_attacked_hit_;
  coresh::Communicator::Connection conn_actor_attacked_hit_;

  coresh::Communicator::Connection conn_npc_use_skill_;
  coresh::Communicator::Connection conn_actor_use_skill_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__SKILL__EVENT__HANDLER__H

