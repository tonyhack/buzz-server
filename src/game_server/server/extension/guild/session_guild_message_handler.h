#ifndef __GAME__SERVER__GUILD__SESSION__GUILD__MESSAGE__HANDLER__H
#define __GAME__SERVER__GUILD__SESSION__GUILD__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace guild {

class SessionGuildMessageHandler : public core::Noncopyable {
 public:
  SessionGuildMessageHandler();
  ~SessionGuildMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 公会数据回复
  void OnMessageGuildInfoResponse(const char *data, size_t size);
  // 公会数据同步
  void OnMessageGuildInfoSynchronize(const char *data, size_t size);
  // 玩家公会数据初始化
  void OnMessageGuildActorInitialize(const char *data, size_t size);
  // 玩家公会数据同步
  void OnMessageGuildActorSynchronize(const char *data, size_t size);
  // 公会技能数据同步
  void OnMessageGuildSkillSynchronize(const char *data, size_t size);
  // 公会BUFF数据同步
  void OnMessageGuildBuffSynchronize(const char *data, size_t size);
  // 购买公会商店道具回复
  void OnMessageGuildShopBuyItemResponse(const char *data, size_t size);
  // 公会副本领取奖励回复
  void OnMessageGuildPlayingAwardResponse(const char *data, size_t size);
};

}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__SESSION__GUILD__MESSAGE__HANDLER__H

