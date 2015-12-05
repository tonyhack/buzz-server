#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__MESSAGE__HANDLER__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class HomelandBattleMessageHandler : public core::Noncopyable {
 public:
  HomelandBattleMessageHandler();
  ~HomelandBattleMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 家园攻防战匹配请求
  void OnMessageHomelandBattleMatchRequest(core::uint64 id, const char *data, size_t size);
  // 家园攻防战反击请求
  void OnMessageHomelandBattleStrikeBackRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__MESSAGE__HANDLER__H

