#ifndef __GAME__SERVER__WELFARE__WELFARE__ACTOR__H
#define __GAME__SERVER__WELFARE__WELFARE__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_achieve_login_types.h"
#include "database_server/protocol/storage_welfare_save_types.h"
#include "database_server/protocol/storage_welfare_load_types.h"
#include "game_server/server/extension/welfare/achieve_actor.h"
#include "game_server/server/extension/welfare/checkins_actor.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace welfare {

class WelfareActor : public core::Noncopyable {
 public:
  typedef std::vector<database::protocol::StorageAchieveField>        StorageAchieveFields;
  typedef std::vector<database::protocol::StorageAchieveFinishField>  StorageAchieveFinishField;
  typedef database::protocol::StorageWelfareField                     DatabaseWelfareField;

  WelfareActor();
  ~WelfareActor();

  bool Initialize(StorageAchieveFields &achieves, StorageAchieveFinishField &field);
  bool InitializeActor(GameActor *actor, DatabaseWelfareField &field);
  void Finalize();

  void Save();

  core::uint64 GetActorID() {
    return this->actor_->GetActorID();
  }

  GameActor *GetActor() {
    return this->actor_;
  }

  AchieveActor *GetAchieve() {
    return &this->achieve_;
  }

  CheckinsActor *GetCheckins() {
    return &this->checkins_;
  }

  void DailyClean(core::int32 days);

  // 事件回调
  void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:

  // 玩家对象
  GameActor *actor_;
  // 成就对象
  AchieveActor achieve_;
  // 签到对象
  CheckinsActor checkins_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__WELFARE__ACTOR__H
