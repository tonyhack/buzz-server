#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__ACTOR__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__ACTOR__H

#include <ctime>
#include <deque>
#include <set>

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"
#include "session_server/server/extension/social/homeland_supported_soul.h"
#include "session_server/server/extension/social/homeland_support_soul.h"
#include "session_server/server/extension/social/homeland_invader.h"
#include "session_server/server/extension/social/homeland_event.h"

namespace database { namespace protocol {
class StorageSocialActorField;
}}

namespace session {

namespace server {

namespace social {

class SocialActor;

class HomelandActor : public core::TimerHandler, public core::Noncopyable {
  enum { TIMER_ID_GROWING_FINISH = 0 };

 public:
  typedef std::set<core::uint64> ActorIDSet;
  typedef std::map<core::uint64, coresh::Communicator::Connection> EventConnectionMap;
  typedef std::map<core::uint64, HomelandSupportedSoul> HomelandSupportedSoulMap;
  typedef std::map<core::uint32, HomelandSupportSoul> HomelandSupportSoulMap;
  typedef std::map<core::uint32, core::int32> SoulLevelMap;
  typedef std::map<core::uint64, HomelandInvader> HomelandInvaderMap;
  typedef std::set<HomelandInvader *, HomelandInvader::InvadeTimeLess> HomelandInvaderSet;
  typedef std::deque<HomelandEvent> HomelandEventDeque;

  HomelandActor();
  ~HomelandActor();

  bool Initialize(SocialActor *social_actor,
      const database::protocol::StorageSocialActorField &db_field);
  void Finalize();
  void Online();
  void Offline();
  void Save(database::protocol::StorageSocialActorField &db_field);

  // 开启功能
  bool EnableHomelandFunc(bool synchronize = true);
  bool EnableHomelandGoldmineFunc(bool synchronize = true);
  bool EnableHomelandPoolFunc(bool synchronize = true);
  bool EnableHomelandTreeFunc(bool synchronize = true);
  bool EnableHomelandTowerFunc(bool synchronize = true);
  bool EnableHomelandStrongholdFunc(bool synchronize = true);

  // 公用功能
  core::int32 GetUpgradeCooling(time_t now = 0) const;
  bool CheckUpgradeCoolingRed(time_t now = 0) const;
  void AddUpgradeCooling(core::int32 cooling_time, bool synchronize = true);
  void ClearUpgradeCooling(bool synchronize = true);
  bool CheckUpgradeCooling() const;

  bool CheckCanVisit() const;

  // 神殿
  core::int32 GetTempleLevel() const { return this->temple_level_; }
  void SetTempleLevel(core::int32 new_level, bool synchronize = true);
  core::int32 GetTempleHarvestCooling(time_t now = 0) const;
  void SetTempleHarvestCooling(core::int32 cooling_time, bool synchronize = true);
  void ClearTempleHarvestCooling(bool synchronize = true);
  core::int32 GetTempleHarvestTimes() const { return this->temple_harvest_times_; }
  core::int32 GetTempleForceHarvestTimes() const { return this->temple_force_harvest_times_; }
  void AddTempleHarvestTimes(bool synchronize = true);
  void AddTempleForceHarvestTimes(bool synchronize = true);

  // 金矿
  core::int32 GetGoldmineLevel() const { return this->goldmine_level_; }
  void SetGoldmineLevel(core::int32 new_level, bool synchronize = true);
  core::int32 GetGoldmineHarvestCooling(time_t now = 0) const;
  void SetGoldmineHarvestCooling(core::int32 cooling_time, bool synchronize = true);

  bool CheckGoldmineCanRob() const;
  core::int32 GetGoldmineRobCooling(time_t now = 0) const;
  void SetGoldmineRobCooling(core::int32 cooling_time, bool synchronize = true);
  core::int32 GetGoldmineRobTimes() const { return this->goldmine_rob_times_; }
  core::int32 GetGoldmineRobbedTimes() const { return this->goldmine_robbed_times_; }
  core::int32 GetGoldmineLossRate() const { return this->goldmine_loss_rate_; }
  void AddGoldmineRobTimes(bool synchronize = true);
  void AddGoldmineRobbedTimes(bool synchronize = true);
  void AddGoldmineLossRate(core::int32 rate, bool synchronize = true);
  void ResetGoldmine(bool synchronize = true);

  // 神秘之源
  core::int32 GetPoolLevel() const { return this->pool_level_; }
  void SetPoolLevel(core::int32 new_level, bool synchronize = true);
  core::int32 GetPoolAccumulateTime(time_t now = 0) const;
  void ClearPoolAccmulateTime(bool synchronize = true);

  // 祈福堂
  bool CheckChurchInPrayStage() const;
  bool CheckChurchInHarvestStage() const;
  void MoveChurchToPrayStage(bool synchronize = true);
  void MoveChurchToHarvestStage(core::int32 chest_type, bool synchronize = true);
  core::int32 GetChurchChestType() const { return this->church_chest_type_; }
  core::int32 GetChurchFreePrayTimes() const { return this->church_free_pray_times_; }
  core::int32 GetChurchPaidPrayTimes() const { return this->church_paid_pray_times_; }
  core::int32 GetChurchFreeRefreshTimes() const { return this->church_free_refresh_times_; }
  void AddChurchFreePrayTimes(bool synchronize = true);
  void AddChurchPaidPrayTimes(bool synchronize = true);
  void AddChurchFreeRefreshTimes(bool synchronize = true);

  // 神树
  core::int32 GetTreeLevel() const { return this->tree_level_; }
  void AddTreeHarvestTimes(bool synchronize = true);
  core::int32 GetTreeWaterCooling(time_t now = 0) const;
  void SetTreeWaterCooling(core::int32 cooling_time, bool synchronize = true);
  core::int32 GetTreeGrowingCooling(time_t now = 0) const;
  void SetTreeGrowingCooling(core::int32 cooling_time, bool synchronize = true);
  core::int32 GetTreeStage() const { return this->tree_stage_; }
  void MoveTreeToGrowingStage(core::int32 growing_time, bool synchronize = true);
  void MoveTreeToHarvestStage(bool synchronize = true);
  void MoveTreeToWateringStage(bool synchronize = true);

  bool CheckTreeCanWater(core::uint64 actor_id) const;
  bool CheckTreeCanSteal(core::uint64 actor_id) const;
  core::int32 GetTreeWateredTimes() const { return this->tree_watered_times_; }
  core::int32 GetTreeStolenTimes() const { return this->tree_stolen_actors_.size(); }
  core::int32 GetTreeAssistWaterTimes() const { return this->tree_assist_water_times_; }
  void AddTreeWaterdTimes(core::uint64 actor_id, bool synchronize = true);
  void AddTreeStolenTimes(core::uint64 actor_id, bool synchronize = true);
  void AddTreeAssistWaterTimes(bool synchronize = true);

  // 防御建筑
  core::int32 GetTowerLevel(core::int32 tower_id) const;
  void SetTowerLevel(core::int32 tower_id, core::int32 new_level, bool synchronize = true);

  // 军团据点
  core::int32 GetStrongholdLevel() const { return this->stronghold_level_; }
  void SetStrongholdLevel(core::int32 new_level, bool synchronize = true);
  core::int32 GetStrongholdCallSupportCooling(time_t now = 0) const;
  void SetStrongholdCallSupportCooling(core::int32 cooling_time, bool synchronize = true);
  const HomelandSupportedSoulMap &GetStrongholdSupportedSouls();
  const HomelandSupportSoulMap &GetStrongholdSupportSouls();

  bool CheckStrongholdCanSupport(core::uint64 actor_id);
  bool CheckSoulCanSupport(core::uint32 soul_id) const;
  void AddStrongholdSupportedSoul(core::uint64 actor_id,
      core::uint32 soul_id, core::int32 soul_level, bool synchronize = true);
  void RemoveStrongholdSupportedSoul(core::uint64 actor_id, bool synchronize = true);
  void AddStrongholdSupportSoul(core::uint32 soul_id, core::uint64 actor_id,
      bool synchronize = true);

  // 告示板
  const HomelandEventDeque &GetEvents() const { return this->events_; }
  void AddEvent(const HomelandEvent &event, bool synchronize = true);

  // 家园好友
  void AddHomelandFriend(core::uint64 actor_id, bool synchronize = true);
  void RemoveHomelandFriend(core::uint64 actor_id, bool synchronize = true);

  // 家园侵入者
  bool IsInvader(core::uint64 actor_id) const;
  void AddInvader(core::uint64 actor_id, bool synchronize = true);
  void RemoveInvader(core::uint64 actor_id, bool synchronize = true);
  void RemoveExpiredInvader(bool synchronize = true);

  // 家园英灵
  core::int32 GetSoulLevel(core::uint32 soul_id) const;
  const SoulLevelMap &GetSouls() const { return this->souls_; }
  void UpdateSoulData(core::uint32 soul_id, core::int32 soul_level);

  // 每日清理
  void DailyClean(bool synchronize = true);

  virtual void OnTimer(core::uint32 id);

 private:
  void SendInitToClient();
  void TreeTryUpgrade(bool synchronize = true);

  void FollowHomelandFriendInfoUpdateEvent(core::uint64 actor_id);
  void UnfollowHomelandFriendInfoUpdateEvent(core::uint64 actor_id);
  void OnEventHomelandFriendInfoUpdate(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void FollowHomelandInvaderInfoUpdateEvent(core::uint64 actor_id);
  void UnfollowHomelandInvaderInfoUpdateEvent(core::uint64 actor_id);
  void OnEventHomelandInvaderInfoUpdate(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  SocialActor *social_actor_;

  time_t upgrade_start_time_;
  time_t upgrade_finish_time_;
  // 神殿
  core::int32 temple_level_;
  core::int32 temple_harvest_times_;
  core::int32 temple_force_harvest_times_;
  time_t temple_next_harvest_time_;
  // 金矿
  core::int32 goldmine_level_;
  core::int32 goldmine_robbed_times_;
  core::int32 goldmine_loss_rate_;
  time_t goldmine_next_harvest_time_;
  core::int32 goldmine_rob_times_;
  time_t goldmine_next_rob_time_;
  // 神秘之源
  core::int32 pool_level_;
  time_t pool_last_harvest_time_;
  // 祈福堂
  core::int32 church_free_pray_times_;
  core::int32 church_paid_pray_times_;
  core::int32 church_free_refresh_times_;
  core::int32 church_chest_type_;
  // 神树
  core::int32 tree_level_;
  core::int32 tree_harvest_times_;
  core::int32 tree_stage_;
  core::int32 tree_watered_times_;
  time_t tree_next_water_time_;
  time_t tree_growing_finish_time_;
  core::int32 tree_assist_water_times_;
  ActorIDSet tree_watered_actors_;
  ActorIDSet tree_stolen_actors_;
  // 防御建筑
  core::int32 tower1_level_;
  core::int32 tower2_level_;
  // 军团据点
  core::int32 stronghold_level_;
  time_t stronghold_next_call_support_time_;
  HomelandSupportedSoulMap stronghold_supported_souls_;
  HomelandSupportSoulMap stronghold_support_souls_;
  core::int32 stronghold_support_times_;
  // 告示板
  HomelandEventDeque events_;
  // 家园好友
  EventConnectionMap friend_event_conns_;
  // 家园侵入者
  HomelandInvaderMap invaders_;
  HomelandInvaderSet invaders_time_index_;
  EventConnectionMap invader_event_conns_;
  // 英灵数据
  SoulLevelMap souls_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__ACTOR__H

