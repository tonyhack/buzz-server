#ifndef __SESSION__SERVER__GUILD__GUILD__MESSAGE__HANDLER__H
#define __SESSION__SERVER__GUILD__GUILD__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace guild {

class GuildMessageHandler : public core::Noncopyable {
 public:
  GuildMessageHandler();
  ~GuildMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 公会列表数据请求
  void OnMessageGuildListRequest(core::uint64 id, const char *data, size_t size);
  // 公会关闭申请请求
  void OnMessageGuildSuppressJoinRequest(core::uint64 id, const char *data, size_t size);
  // 公会申请请求
  void OnMessageGuildJoinRequest(core::uint64 id, const char *data, size_t size);
  // 公会取消申请请求
  void OnMessageGuildCancelJoinRequest(core::uint64 id, const char *data, size_t size);
  // 公会审核成员加入请求
  void OnMessageGuildAuditPendingMemberRequest(core::uint64 id, const char *data, size_t size);
  // 公会邀请请求
  void OnMessageGuildInviteRequest(core::uint64 id, const char *data, size_t size);
  // 公会邀请处理请求
  void OnMessageGuildInviteDisposeRequest(core::uint64 id, const char *data, size_t size);
  // 离开公会请求
  void OnMessageGuildLeaveRequest(core::uint64 id, const char *data, size_t size);
  // 踢出成员请求
  void OnMessageGuildKickMemberRequest(core::uint64 id, const char *data, size_t size);
  // 放弃会长职位请求
  void OnMessageGuildGiveUpLeaderPositionRequest(core::uint64 id, const char *data, size_t size);
  // 放弃公会职位请求
  void OnMessageGuildGiveUpPositionRequest(core::uint64 id, const char *data, size_t size);
  // 公会职位任免请求
  void OnMessageGuildSetMemberPositionRequest(core::uint64 id, const char *data, size_t size);
  // 编辑公会公告请求
  void OnMessageGuildEditAnnouncementRequest(core::uint64 id, const char *data, size_t size);
  // 公会升级请求
  void OnMessageGuildUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 公会商店升级请求
  void OnMessageGuildShopUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 公会学院升级请求
  void OnMessageGuildCollegeUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 学习公会技能请求
  void OnMessageGuildCollegeLearnSkillRequest(core::uint64 id, const char *data, size_t size);
  // 神像祈祷请求
  void OnMessageGuildIdolPrayRequest(core::uint64 id, const char *data, size_t size);
  // 公会军营升级请求
  void OnMessageGuildBarrackUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 公会副本组解封请求
  void OnMessageGuildPlayingGroupOpenRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GUILD__MESSAGE__HANDLER__H

