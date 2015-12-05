#ifndef __SESSION__SERVER__CHAT__FACADE__REQUEST__H
#define __SESSION__SERVER__CHAT__FACADE__REQUEST__H

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace chat {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 检查目标玩家是否在玩家的黑名单中
  bool CheckInBlacklist(core::uint64 actor_id, core::uint64 target_actor_id);
  // 检查是否有在公会频道聊天的权限
  bool CheckGuildAuthorityChat(core::uint64 actor_id);
  
 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace chat

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__CHAT__FACADE__REQUEST__H

