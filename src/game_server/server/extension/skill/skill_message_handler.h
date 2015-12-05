//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-07 15:58:25.
// File name: skill_message_handler.h
//
// Description:
// Define class SkillMessageHandler.
//

#ifndef __GAME__SERVER__SKILL__SKILL__MESSAGE__HANDLER__H
#define __GAME__SERVER__SKILL__SKILL__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace skill {

class SkillMessageHandler : public core::Noncopyable {
 public:
  SkillMessageHandler();
  ~SkillMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageSkillOperate(core::uint64 id, const char *data, size_t size);

  void OnMessageActorChoosePKMode(core::uint64 id, const char *data, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__MESSAGE__HANDLER__H

