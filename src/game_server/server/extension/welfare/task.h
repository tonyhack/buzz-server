#ifndef __GAME__SERVER__WELFARE__TASK__H
#define __GAME__SERVER__WELFARE__TASK__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_achieve_login_types.h"
#include "entity/achieve_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/condition.h"
#include "gateway_server/protocol/gateway_welfare_protocol_types.h"

namespace game {

namespace server {

namespace welfare {

class AchieveActor;

class Task : public core::Noncopyable {
 typedef std::vector<Condition *> ConditionVec;
 public:
  Task();
  ~Task();

  bool Initialize(AchieveActor *actor, core::int32 id, database::protocol::StorageAchieveField &field);
  void Finalize();

  void Save(database::protocol::StorageAchieveField &field);

  void ExportConditionVar(gateway::protocol::AchieveCondition &condition);

  // 事件回调
  void OnEvent(event::EventType::type type,
      const void *data, size_t size);

  // 条件完成做的处理
  void Clear();

  // 检查是否完成
  bool CheckFinish();

  core::int32 GetID() {
    return this->id_;
  }

  // 获取完成时间
  core::int64 GetFinishTime() {
    return this->finish_time_;
  }

  AchieveActor *GetActor();

  // 同步数据
  void Synchronize();

 private:
  // 成就玩家对象
  AchieveActor *actor_;
  // 成就ID
  core::int32 id_;
  // 成就达成时间
  core::int64 finish_time_;
  // 成就条件
  ConditionVec conditions_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__TASK__H
