#ifndef __GAME__SERVER__GUILD__GUILD__MESSAGE__HANDLER__H
#define __GAME__SERVER__GUILD__GUILD__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace guild {

class GuildMessageHandler : public core::Noncopyable {
 public:
  GuildMessageHandler();
  ~GuildMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 创建公会请求
  void OnMessageGuildCreateRequest(core::uint64 id, const char *data, size_t size);
  // 公会捐赠请求 
  void OnMessageGuildDonateRequest(core::uint64 id, const char *data, size_t size);
  // 购买公会商店道具请求
  void OnMessageGuildShopBuyItemRequest(core::uint64 id, const char *data, size_t size);
  // 公会副本领取奖励请求
  void OnMessageGuildPlayingAwardRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__GUILD__MESSAGE__HANDLER__H

