#ifndef __GAME__SERVER__WELFARE__ACHIEVE__ACTOR__H
#define __GAME__SERVER__WELFARE__ACHIEVE__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_achieve_login_types.h"
#include "database_server/protocol/storage_achieve_save_types.h"
#include "database_server/protocol/storage_welfare_save_types.h"
#include "database_server/protocol/storage_welfare_load_types.h"
#include "entity/achieve_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/task.h"

namespace game {

namespace server {

namespace welfare {

class AchieveActor : public core::Noncopyable {
  struct AchieveFinish {
    public:
    AchieveFinish() {
      this->id_ = 0;
      this->finish_time_ = 0;
    }
    AchieveFinish(core::int32 id, core::int64 finish_time) {
      this->id_ = id;
      this->finish_time_ = finish_time;
    }
    core::int32 id_;
    core::int64 finish_time_;
  };

  typedef std::vector<AchieveFinish>                                  AchieveFinishIds;
  typedef std::vector<core::int32>                                    AchieveIds;
  typedef std::map<event::EventType::type, AchieveIds>                EventAchieveMap;
  typedef std::map<core::int32, Task *>                               AchieveMap;
 public:
  AchieveActor();
  ~AchieveActor();

  bool Initialize(std::vector<database::protocol::StorageAchieveField> &achieves, 
      std::vector<database::protocol::StorageAchieveFinishField> &field);
  bool InitializeActor(GameActor *actor, database::protocol::StorageWelfareField &field);
  void Finalize();

  void Save();
  void SaveActor(database::protocol::StorageWelfareField &field);

  core::uint64 GetActorID() {
    return this->actor_->GetActorID();
  }

  GameActor *GetActor() {
    return this->actor_;
  }

  core::int32 GetTitleID() {
    return this->title_id_;
  }
  void SetTitleID(core::int32 title_id) {
    this->title_id_ = title_id;
  }

  bool AddEventDispatch(core::int32 achieve_id, entity::AchieveConditionType::type type);

  void AddEvent(event::EventType::type type, core::int32 achieve_id);

  bool CheckFinishAchieve(core::int32 achieve_id);

  bool CheckFinishExist(core::int32 achieve_id);

  bool FinishAchieve(core::int32 achieve_id);

  void AddAttr(core::int32 achieve_id, bool sync);

  // 事件回调
  void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:

  // 玩家对象
  GameActor *actor_;
  // 正在进行成就列表
  AchieveMap achieves_map_;
  // 称号
  core::int32 title_id_;
  // 成就完成列表
  AchieveFinishIds finish_achieves_;
  // 事件
  EventAchieveMap events_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__ACHIEVE__ACTOR__H
