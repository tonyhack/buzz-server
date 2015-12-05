#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__MESSAGE__HANDLER__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class HomelandMessageHandler : public core::Noncopyable {
 public:
  HomelandMessageHandler();
  ~HomelandMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 访问好友家园请求
  void OnMessageHomelandVisitRequest(core::uint64 id, const char *data, size_t size);
  // 清除升级冷却时间请求
  void OnMessageHomelandClearUpgradeCoolingRequest(core::uint64 id, const char *data, size_t size);
  // 神殿升级请求
  void OnMessageHomelandTempleUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 神殿征收请求
  void OnMessageHomelandTempleHarvestRequest(core::uint64 id, const char *data, size_t size);
  // 清除神殿征收冷却时间请求
  void OnMessageHomelandTempleClearHarvestCoolingRequest(core::uint64 id, const char *data, size_t size);
  // 神殿强征请求
  void OnMessageHomelandTempleForceHarvestRequest(core::uint64 id, const char *data, size_t size);
  // 金矿升级请求
  void OnMessageHomelandGoldmineUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 金矿收获请求
  void OnMessageHomelandGoldmineHarvestRequest(core::uint64 id, const char *data, size_t size);
  // 神秘之源升级请求
  void OnMessageHomelandPoolUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 神秘之源领取请求
  void OnMessageHomelandPoolHarvestRequest(core::uint64 id, const char *data, size_t size);
  // 神秘之源双倍领取请求
  void OnMessageHomelandPoolDoubleHarvestRequest(core::uint64 id, const char *data, size_t size);
  // 祈福堂祈福请求
  void OnMessageHomelandChurchPrayRequest(core::uint64 id, const char *data, size_t size);
  // 祈福堂刷新请求
  void OnMessageHomelandChurchRefreshRequest(core::uint64 id, const char *data, size_t size);
  // 祈福堂高级刷新请求
  void OnMessageHomelandChurchVipRefreshRequest(core::uint64 id, const char *data, size_t size);
  // 祈福堂领取奖励请求
  void OnMessageHomelandChurchHarvestRequest(core::uint64 id, const char *data, size_t size);
  // 神树浇水请求
  void OnMessageHomelandTreeWaterRequest(core::uint64 id, const char *data, size_t size);
  // 神树帮助好友浇水请求
  void OnMessageHomelandTreeAssistWaterRequest(core::uint64 id, const char *data, size_t size);
  // 神树抢收请求
  void OnMessageHomelandTreeStealRequest(core::uint64 id, const char *data, size_t size);
  // 神树收获请求
  void OnMessageHomelandTreeHarvestRequest(core::uint64 id, const char *data, size_t size);
  // 炮塔升级请求
  void OnMessageHomelandTowerUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 军团据点升级请求
  void OnMessageHomelandStrongholdUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 军团据点申请支援请求
  void OnMessageHomelandStrongholdCallSupportRequest(core::uint64 id, const char *data, size_t size);
  // 军团据点支援请求
  void OnMessageHomelandStrongholdSupportRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__MESSAGE__HANDLER__H

