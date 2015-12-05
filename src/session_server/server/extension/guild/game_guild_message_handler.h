#ifndef __SESSION__SERVER__GUILD__GAME__GUILD__MESSAGE__HANDLER__H
#define __SESSION__SERVER__GUILD__GAME__GUILD__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

class SessionTerminal;

namespace guild {

class GameGuildMessageHandler : public core::Noncopyable {
 public:
  GameGuildMessageHandler();
  ~GameGuildMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 公会数据请求
  void OnMessageGuildInfoRequest(SessionTerminal *terminal, const char *data, size_t size);
  // 创建公会请求
  void OnMessageGuildCreateRequest(SessionTerminal *terminal, const char *data, size_t size);
  // 公会捐赠请求
  void OnMessageGuildDonateRequest(SessionTerminal *terminal, const char *data, size_t size);
  // 购买公会商店道具请求
  void OnMessageGuildShopBuyItemRequest(SessionTerminal *terminal, const char *data, size_t size);
  // 公会副本领取奖励请求
  void OnMessageGuildPlayingAwardRequest(SessionTerminal *terminal, const char *data, size_t size);
  // 增加公会贡献值请求
  void OnMessageGuildAddContributionRequest(SessionTerminal *terminal, const char *data, size_t size);
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GAME__GUILD__MESSAGE__HANDLER__H

