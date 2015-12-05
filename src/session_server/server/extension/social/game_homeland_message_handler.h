#ifndef __SESSION__SERVER__SOCIAL__GAME__HOMELAND__MESSAGE__HANDLER__H
#define __SESSION__SERVER__SOCIAL__GAME__HOMELAND__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

class SessionTerminal;

namespace social {

class GameHomelandMessageHandler : public core::Noncopyable {
 public:
  GameHomelandMessageHandler();
  ~GameHomelandMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 清除升级冷却时间回复
  void OnMessageHomelandClearUpgradeCoolingResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 神殿升级回复
  void OnMessageHomelandTempleUpgradeResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 清除神殿征收冷却时间回复
  void OnMessageHomelandTempleClearHarvestCoolingResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 神殿强征回复 
  void OnMessageHomelandTempleForceHarvestResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 金矿升级回复
  void OnMessageHomelandGoldmineUpgradeResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 神秘之源升级回复
  void OnMessageHomelandPoolUpgradeResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 神秘之源双倍领取回复
  void OnMessageHomelandPoolDoubleHarvestResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 祈福堂钻石祈福回复
  void OnMessageHomelandChurchPaidPrayResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 祈福堂刷新回复
  void OnMessageHomelandChurchRefreshResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 祈福堂高级刷新回复
  void OnMessageHomelandChurchVipRefreshResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 祈福堂领取奖励回复
  void OnMessageHomelandChurchHarvestResponse(SessionTerminal *terminal, 
      const char *data, size_t size);
  // 炮塔升级回复
  void OnMessageHomelandTowerUpgradeResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 军团据点升级回复
  void OnMessageHomelandStrongholdUpgradeResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 家园英灵数据同步
  void OnMessageHomelandSoulSynchronize(SessionTerminal *terminal,
      const char *data, size_t size);
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__GAME__HOMELAND__MESSAGE__HANDLER__H

