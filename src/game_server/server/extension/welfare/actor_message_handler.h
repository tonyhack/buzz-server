
#ifndef __GAME__SERVER__WELFARE__ACTOR__MESSAGE__HANDLER__H
#define __GAME__SERVER__WELFARE__ACTOR__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace welfare {

class ActorMessageHandler : public core::Noncopyable {
 public:
  ActorMessageHandler();
  ~ActorMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 领取奖励
  void OnMessageAward(core::uint64 id, const char *data, size_t size);
  // 更换称号
  void OnMessageAlterTitle(core::uint64 id, const char *data, size_t size);
  // 签到操作
  void OnMessageCheckins(core::uint64 id, const char *data, size_t size);
  // 领取每日在线奖励
  void OnMessageOnlineAward(core::uint64 id, const char *data, size_t size);
  // 领取上线时间累积的资源奖励
  void OnMessageResourceAward(core::uint64 id,  const char *data, size_t size);
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__ACTOR__MESSAGE__HANDLER__H

