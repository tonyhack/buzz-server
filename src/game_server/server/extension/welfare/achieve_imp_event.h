#ifndef __GAME__SERVER__WELFARE__ACHIEVE__IMP__EVENT__H
#define __GAME__SERVER__WELFARE__ACHIEVE__IMP__EVENT__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/achieve_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace welfare {

class Condition;

class AchieveImpEvent : public core::Noncopyable {
 public:
  AchieveImpEvent();
  ~AchieveImpEvent();

  bool Initialize(Condition *condtion);
  void Finalize();

  // 导出条件变量
  bool ExportVar(core::uint32 &param1, core::uint32 &param2, core::uint32 &value);

  // 条件检测
  bool CheckFinish();

  // 事件回调
  void OnEvent(event::EventType::type type,
      const void *data, size_t size);

  // 条件完成做的处理
  void Clear();

 private:
  GameActor *GetActor();

  void Synchronize();

  // 事件处理
  void OnEventSoulUnlock(event::EventType::type type, const void *data, size_t size);
  void OnEventSoulUpgradeLevel(event::EventType::type type, const void *data, size_t size);
  void OnEventSkillUse(event::EventType::type type, const void *data, size_t size);
  void OnEventFunctionalityOpen(event::EventType::type type, const void *data, size_t size);
  void OnEventSpiritUpgradeLevel(event::EventType::type type, const void *data, size_t size);
  void OnEventSpiritCollectShape(event::EventType::type type, const void *data, size_t size);
  void OnEventKillRole(event::EventType::type type, const void *data, size_t size);
  void OnEventBagExpand(event::EventType::type type, const void *data, size_t size);
  void OnEventKilledSelf(event::EventType::type type, const void *data, size_t size);
  void OnEventKilledSelfFighting(event::EventType::type type, const void *data, size_t size);
  void OnEventKilledSelfLevel(event::EventType::type type, const void *data, size_t size);
  void OnEventKilledSelfAppear(event::EventType::type type, const void *data, size_t size);
  void OnEventEquipStrength(event::EventType::type type, const void *data, size_t size);
  void OnEventWearEquip(event::EventType::type type, const void *data, size_t size);
  void OnEventGemOpt(event::EventType::type type, const void *data, size_t size);
  void OnEventGemInlay(event::EventType::type type, const void *data, size_t size);
  void OnEventReliveWay(event::EventType::type type, const void *data, size_t size);
  void OnEventVocationSet(event::EventType::type type, const void *data, size_t size);
  void OnEventSoulScheme(event::EventType::type type, const void *data, size_t size);
  void OnEventSwordUpgrade(event::EventType::type type, const void *data, size_t size);
  void OnEventSwordInlay(event::EventType::type type, const void *data, size_t size);
  void OnEventHomelandBuildingUpgrade(event::EventType::type type, const void *data, size_t size);
  void OnEventHomelandRevenue(event::EventType::type type, const void *data, size_t size);
  void OnEventHomelandUse(event::EventType::type type, const void *data, size_t size);
  void OnEventHomelandWater(event::EventType::type type, const void *data, size_t size);
  void OnEventHomelandOpt(event::EventType::type type, const void *data, size_t size);
  void OnEventHomelandAttackNum(event::EventType::type type, const void *data, size_t size);
  void OnEventCorpsOpt(event::EventType::type type, const void *data, size_t size);
  void OnEventCorpsJob(event::EventType::type type, const void *data, size_t size);
  void OnEventCorpsGetHolyCity(event::EventType::type type, const void *data, size_t size);
  void OnEventCorpsToScene(event::EventType::type type, const void *data, size_t size);
  void OnEventNolibityGetLevel(event::EventType::type type, const void *data, size_t size);
  void OnEventUseItem(event::EventType::type type, const void *data, size_t size);
  void OnEventResource(event::EventType::type type, const void *data, size_t size);
  void OnEventActorGetValue(event::EventType::type type, const void *data, size_t size);
  void OnEventPassStoryCopy(event::EventType::type type, const void *data, size_t size);
  void OnEventSelfKilledSkill(event::EventType::type type, const void *data, size_t size);
  void OnEventKillActorFighting(event::EventType::type type, const void *data, size_t size);
  void OnEventKillActorLevel(event::EventType::type type, const void *data, size_t size);
  void OnEventKillActorAppear(event::EventType::type type, const void *data, size_t size);
  void OnEventKillActorSex(event::EventType::type type, const void *data, size_t size);
  void OnEventVipStatus(event::EventType::type type, const void *data, size_t size);

  Condition *conditon_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__ACHIEVE__IMP__EVENT__H
