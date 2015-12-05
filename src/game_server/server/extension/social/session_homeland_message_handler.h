#ifndef __GAME__SERVER__SOCIAL__SESSION__HOMELAND__MESSAGE__HANDLER__H
#define __GAME__SERVER__SOCIAL__SESSION__HOMELAND__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace social {

class SessionHomelandMessageHandler : public core::Noncopyable  {
 public:
  SessionHomelandMessageHandler();
  ~SessionHomelandMessageHandler();

  bool Initialize();
  void Finalize();
 
 private:
  // 清除升级冷却时间请求
  void OnMessageHomelandClearUpgradeCoolingRequest(const char *data, size_t size);
  // 神殿升级请求
  void OnMessageHomelandTempleUpgradeRequest(const char *data, size_t size);
  // 神殿升级通知
  void OnMessageHomelandTempleUpgradeNotify(const char *data, size_t size);
  // 神殿征收请求
  void OnMessageHomelandTempleHarvestRequest(const char *data, size_t size);
  // 清除神殿征收冷却时间请求
  void OnMessageHomelandTempleClearHarvestCoolingRequest(const char *data, size_t size);
  // 神殿强征请求
  void OnMessageHomelandTempleForceHarvestRequest(const char *data, size_t size);
  // 金矿升级请求
  void OnMessageHomelandGoldmineUpgradeRequest(const char *data, size_t size);
  // 金矿收获请求
  void OnMessageHomelandGoldmineHarvestRequest(const char *data, size_t size);
  // 神秘之源升级请求
  void OnMessageHomelandPoolUpgradeRequest(const char *data, size_t size);
  // 神秘之源领取请求
  void OnMessageHomelandPoolHarvestRequest(const char *data, size_t size);
  // 神秘之源双倍领取请求
  void OnMessageHomelandPoolDoubleHarvestRequest(const char *data, size_t size);
  // 神秘之源双倍领取请求2
  void OnMessageHomelandPoolDoubleHarvestRequest2(const char *data, size_t size);
  // 祈福堂钻石祈福请求
  void OnMessageHomelandChurchPaidPrayRequest(const char *data, size_t size);
  // 祈福堂刷新请求
  void OnMessageHomelandChurchRefreshRequest(const char *data, size_t size);
  // 祈福堂高级刷新请求
  void OnMessageHomelandChurchVipRefreshRequest(const char *data, size_t size);
  // 祈福堂领取奖励请求
  void OnMessageHomelandChurchHarvestRequest(const char *data, size_t size);
  // 祈福堂领取奖励请求2
  void OnMessageHomelandChurchHarvestRequest2(const char *data, size_t size);
  // 神树帮助好友浇水请求
  void OnMessageHomelandTreeAssistWaterRequest(const char *data, size_t size);
  // 神树抢收请求
  void OnMessageHomelandTreeStealRequest(const char *data, size_t size);
  // 神树收获请求
  void OnMessageHomelandTreeHarvestRequest(const char *data, size_t size);
  // 炮塔升级请求
  void OnMessageHomelandTowerUpgradeRequest(const char *data, size_t size);
  // 军团据点升级请求
  void OnMessageHomelandStrongholdUpgradeRequest(const char *data, size_t size);
  // 家园攻防战匹配请求
  void OnMessageHomelandBattleMatchReqeust(const char *data, size_t size);
  // 家园攻防战反击请求
  void OnMessageHomelandBattleStrikeBackRequest(const char *data, size_t size);
  // 神树浇水
  void OnMessageTreeWater(const char *data, size_t size);
};

}  // namespace social

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOCIAL__SESSION__HOMELAND__MESSAGE__HANDLER__H

