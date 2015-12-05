//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 15:40:56.
// File name: container_message_handler.h
//
// Description:
// Define class ContainerMessageHandler.
//

#ifndef __GAME__SERVER__ITEM__CONTAINER__MESSAGE__HANDLER__H
#define __GAME__SERVER__ITEM__CONTAINER__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class ContainerMessageHandler : public core::Noncopyable {
 public:
  ContainerMessageHandler();
  ~ContainerMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageItemMove(core::uint64 id, const char *data, size_t size);
  void OnMessageItemRemove(core::uint64 id, const char *data, size_t size);
  void OnMessageItemUse(core::uint64 id, const char *data, size_t size);
  void OnMessageItemSplit(core::uint64 id, const char *data, size_t size);
  void OnMessageItemArrange(core::uint64 id, const char *data, size_t size);
  void OnMessageExtendContainer(core::uint64 id, const char *data, size_t size);
  void OnMessageShortcutSetRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__CONTAINER__MESSAGE__HANDLER__H

