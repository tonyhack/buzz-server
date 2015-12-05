
#ifndef __GAME__SERVER__ACTOR__MESSAGE__HANDLER__H
#define __GAME__SERVER__ACTOR__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

class ActorMessageHandler : public core::Noncopyable {
 public:
  ActorMessageHandler();
  ~ActorMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 开启功能请求
  void OnMessageActorFunctionalityOpenRequest(core::uint64 id, const char *data, size_t size);
  // 使用护符请求
  void OnMessageActorUseTaslimanRequest(core::uint64 id, const char *data, size_t size);

};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ACTOR__MESSAGE__HANDLER__H

