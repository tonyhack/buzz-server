#ifndef __GAME__SERVER__PLAYING__MAZE__ACTOR__H
#define __GAME__SERVER__PLAYING__MAZE__ACTOR__H

#include <ctime>
#include <deque>
#include <set>
#include <vector>

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/maze_types.h"
#include "entity/vip_types.h"

namespace database { namespace protocol {
class StoragePlayingActorField;
}}

namespace game {

namespace server {

namespace playing {

class PlayingActor;

class MazeActor : public core::TimerHandler, public core::Noncopyable {
  enum { TIMER_ID_ACTION_POINT_RECOVER = 0 };

 public:
  typedef std::set<core::int32> ExploredGridSet;
  typedef std::deque<core::int32> RecentGridDeque;
  
  MazeActor();
  ~MazeActor();

  bool Initialize(PlayingActor *playing_actor,
      const database::protocol::StoragePlayingActorField &db_field);
  void Finalize();
  void Save(database::protocol::StoragePlayingActorField &db_field);

  // 开启功能
  bool EnableMazeFunc(bool synchronize = true);

  core::int32 GetCurrentFloor() const { return this->floor_; }
  core::int32 GetActionPoint() const { return this->ap_; }
  core::int32 GetMaxActionPoint() const;
  core::int32 GetActionPointRecoverCooling(time_t now = 0) const; 
  core::int32 GetGiftNumber() const { return this->gift_number_; }
  core::int32 GetX() const;
  core::int32 GetY() const;
  core::int32 GetNearPosition(entity::MazeDirectionType::type dir) const;
  core::int32 GetMapGrid(core::int32 pos) const;
  void GetMapGrids(std::vector<core::int32> &grids) const;
  bool CheckPositionExlored(core::int32 pos) const;

  void AddActionPoint(core::int32 ap, bool synchronize = true);
  void DecreaseActionPoint(core::int32 ap, bool synchronize = true);
  void RecoverActionPoint(bool synchronize = true);
  void AdjustActionPoint(entity::VipType::type old_vip_type,
      entity::VipType::type new_vip_type, bool synchronize = true);

  void Move(core::int32 pos, bool synchronize = true);
  void MoveToStartPosition(bool synchronize = true);
  void MoveBack(core::int32 step, bool synchronize = true);
  void EnterNextFloor(core::int32 floor, bool synchronize = true);

  void AddGiftNumber(bool synchronize = true);
  void DecreaseGiftNumber(bool synchronize = true);

  // 定时器
  virtual void OnTimer(core::uint32 id);

 private:
  void SendInitToClient();
  void AddRecoverTimer(core::int32 second);
  void RemoveRecoverTimer();

  PlayingActor *playing_actor_;

  core::int32 ap_;
  time_t last_ap_recover_time_;
  core::int32 floor_;
  ExploredGridSet explored_area_;
  RecentGridDeque recent_position_;
  core::int32 gift_number_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__MAZE__ACTOR__H

