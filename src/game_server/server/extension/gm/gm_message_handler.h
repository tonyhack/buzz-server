//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 18:06:19.
// File name: gm_message_handler.h
//
// Description:
// Define class GmMessageHandler.
//

#ifndef __GAME__SERVER__GM__GM__MESSAGE__HANDLER__H
#define __GAME__SERVER__GM__GM__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace gm {

class GmMessageHandler : public core::Noncopyable {
 public:
  GmMessageHandler();
  ~GmMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageAddItem(core::uint64 id, const char *data, size_t size);
  void OnMessageSceneJump(core::uint64 id, const char *data, size_t size);
  void OnMessageGmCommand(core::uint64 id, const char *data, size_t size);
};

}  // namespace gm

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GM__GM__MESSAGE__HANDLER__H

