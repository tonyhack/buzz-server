#ifndef __GAME__SERVER__SOUL__SOUL__ALTAR__MESSAGE__HANDLER__H
#define __GAME__SERVER__SOUL__SOUL__ALTAR__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace soul {

class SoulAltarMessageHandler : public core::Noncopyable {
 public:
  SoulAltarMessageHandler();
  ~SoulAltarMessageHandler();
  
  bool Initialize();
  void Finalize();

 private:
  // 英灵祭坛召唤请求
  void OnMessageSoulAltarSummonRequest(core::uint64 id, const char *data, size_t size);
  // 英灵祭坛收取请求
  void OnMessageSoulAltarTakeRequest(core::uint64 id, const char *data, size_t size);
  // 英灵祭坛放弃请求
  void OnMessageSoulAltarResetRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__ALTAR__MESSAGE__HANDLER__H

