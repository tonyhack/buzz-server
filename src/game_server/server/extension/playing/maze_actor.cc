#include "game_server/server/extension/playing/maze_actor.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>

#include "global/logging.h"
#include "global/common_functions.h"
#include "global/configure/configure.h"
#include "database_server/protocol/storage_playing_actor_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "game_server/server/game_server.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/maze_map_manager.h"

namespace game {

namespace server {

namespace playing {

static bool CheckPositionValid(core::int32 pos) {
  if (pos < 0 || pos >= MISC_CONF()->maze_width_ * MISC_CONF()->maze_height_) {
    return false;
  }
  return true;
}

static void DecodeExploredGridSet(const std::string &source,
    MazeActor::ExploredGridSet *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }
    core::int32 pos = atoi(split_result[i].c_str());
    if (CheckPositionValid(pos) == false) {
      LOG_ERROR("Position(%d) is invalid.", pos);
      continue;
    }
    result->insert(pos);
  }
}

static void EncodeExploredGridSet(const MazeActor::ExploredGridSet &source, 
    std::string *result) {
  char buffer[256] = "";
  size_t count = 0;

  MazeActor::ExploredGridSet::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%d,", *iter);
  }

  *result = buffer;
}

static void DecodeRecentGridDeque(const std::string &source,
    MazeActor::RecentGridDeque *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }
    core::int32 pos = atoi(split_result[i].c_str());
    if (CheckPositionValid(pos) == false) {
      LOG_ERROR("Position(%d) is invalid.", pos);
      continue;
    }
    result->push_back(pos);
  }
}

static void EncodeRecentGridDeque(const MazeActor::RecentGridDeque &source,
    std::string *result) {
  char buffer[256] = "";
  size_t count = 0;

  for (size_t i = 0; i < source.size(); ++i) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%d,", source[i]);
  }

  *result = buffer;
}

///////////////////////////////////////////////////////////////////////////////

MazeActor::MazeActor() : playing_actor_(NULL),
    ap_(0), last_ap_recover_time_(0), floor_(0), gift_number_(0) {}
MazeActor::~MazeActor() {}

bool MazeActor::Initialize(PlayingActor *playing_actor,
    const database::protocol::StoragePlayingActorField &db_field) {
  if (NULL == playing_actor) {
    LOG_ERROR("playing_actor is null.");
    return false;
  }

  this->playing_actor_ = playing_actor;

  this->ap_ = db_field.maze_ap_;
  this->last_ap_recover_time_ = db_field.maze_last_ap_recover_time_;
  this->floor_ = db_field.maze_floor_;
  DecodeExploredGridSet(db_field.maze_explored_area_, &this->explored_area_);
  DecodeRecentGridDeque(db_field.maze_recent_position_, &this->recent_position_);
  this->gift_number_ = db_field.maze_gift_number_;

  // 未开启功能返回
  if (this->playing_actor_->GetActor()->CheckFunctionalityState(
          entity::FunctionalityType::MAZE_MODULE) == false) {
    return true;
  }

  // 检查数据有效性
  if (this->recent_position_.empty()) {
    LOG_ERROR("recent_position is empty.");
    return false;
  }
  if (MazeMapManager::GetInstance()->CheckReady() == false) {
    LOG_ERROR("MazeMapManager is not ready.");
    return false;
  }
  if (MazeMapManager::GetInstance()->GetMap(this->floor_) == NULL) {
    LOG_ERROR("floor(%d) is invalid.", this->floor_);
    return false;
  }

  // 检查地宫是否已重置
  if (MazeMapManager::GetInstance()->GetLastUpdateTime() >
      this->playing_actor_->GetActor()->GetLastLogoutTime()) {
    this->floor_ = 1;
    this->explored_area_.clear();
    this->recent_position_.clear();
    this->recent_position_.push_back(0);
  }

  if (this->ap_ < this->GetMaxActionPoint()) {
    // 结算离线行动点回复
    core::int32 offline_recover_ap = 0;
    time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    if (now > this->last_ap_recover_time_) {
      offline_recover_ap = (now - this->last_ap_recover_time_) /
          MISC_CONF()->maze_action_point_recover_cooling_;
      this->last_ap_recover_time_ += offline_recover_ap *
          MISC_CONF()->maze_action_point_recover_cooling_;
      this->ap_ = std::min(this->ap_ + offline_recover_ap, this->GetMaxActionPoint());
    } else {
      this->last_ap_recover_time_ = now;
    }
    // 添加定时器
    this->AddRecoverTimer(this->GetActionPointRecoverCooling(now));
  } else {
    // 调整为上限值
    this->ap_ = this->GetMaxActionPoint();
  }

  // 发送初始化信息
  this->SendInitToClient();

  return true;
}

void MazeActor::Finalize() {
  this->RemoveRecoverTimer();
  this->gift_number_ = 0;
  this->recent_position_.clear();
  this->explored_area_.clear();
  this->floor_ = 0;
  this->last_ap_recover_time_ = 0;
  this->ap_ = 0;
}

void MazeActor::SendInitToClient() {
  gateway::protocol::MessageMazeInitialize message;
  message.__set_action_point_(this->ap_);
  message.__set_action_point_recover_cooling_(this->GetActionPointRecoverCooling());
  message.__set_floor_(this->floor_);
  message.__set_x_(this->GetX());
  message.__set_y_(this->GetY());
  this->GetMapGrids(message.grids_);
  message.__set_gift_num_(this->gift_number_);
  this->playing_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_MAZE_INITIALIZE);
}

void MazeActor::Save(database::protocol::StoragePlayingActorField &db_field) {
  db_field.__set_maze_ap_(this->ap_);
  db_field.__set_maze_last_ap_recover_time_(this->last_ap_recover_time_);
  db_field.__set_maze_floor_(this->floor_);
  EncodeExploredGridSet(this->explored_area_, &db_field.maze_explored_area_);
  EncodeRecentGridDeque(this->recent_position_, &db_field.maze_recent_position_);
  db_field.__set_maze_gift_number_(this->gift_number_);
}

bool MazeActor::EnableMazeFunc(bool synchronize) {
  this->ap_ = this->GetMaxActionPoint();
  this->last_ap_recover_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  this->floor_ = 1;
  this->explored_area_.clear();
  this->recent_position_.clear();
  this->recent_position_.push_back(0);
  this->gift_number_ = 0;

  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

void MazeActor::AddRecoverTimer(core::int32 second) {
  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_ACTION_POINT_RECOVER, second * 1000,
      -1, this, "MazeActor::AddRecoverTimer");
}

void MazeActor::RemoveRecoverTimer() {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_ACTION_POINT_RECOVER, this);
}

void MazeActor::OnTimer(core::uint32 id) {
  if (TIMER_ID_ACTION_POINT_RECOVER == id) {
    this->RemoveRecoverTimer();
    this->RecoverActionPoint();
    if (this->ap_ < this->GetMaxActionPoint()) {
      this->AddRecoverTimer(this->GetActionPointRecoverCooling());
    }
  }
}

core::int32 MazeActor::GetMaxActionPoint() const {
  const global::configure::VipCell *vip_extra_ap_cell = VIP_CONF()->GetCell(
      (entity::VipType::type)this->playing_actor_->GetActor()->GetAttribute(entity::ActorAoiFields::VIP),
      entity::VipPrivilegeType::MAZE_ACTION_POINT);
  core::int32 vip_extra_ap = 0;
  if (vip_extra_ap_cell) {
    vip_extra_ap = vip_extra_ap_cell->value1_;
  }

  return MISC_CONF()->maze_max_action_point_ + vip_extra_ap;
}

core::int32 MazeActor::GetX() const {
  if (recent_position_.empty()) {
    return -1;
  }
  return recent_position_.back() % MISC_CONF()->maze_width_;
}

core::int32 MazeActor::GetY() const {
  if (recent_position_.empty()) {
    return -1;
  }
  return recent_position_.back() / MISC_CONF()->maze_width_;
}

core::int32 MazeActor::GetNearPosition(entity::MazeDirectionType::type dir) const {
  core::int32 x = this->GetX();
  core::int32 y = this->GetY();

  if (-1 == x || -1 == y) {
    return -1;
  }

  switch (dir) {
    case entity::MazeDirectionType::EAST:
      ++x;
      break;
    case entity::MazeDirectionType::WEST:
      --x;
      break;
    case entity::MazeDirectionType::SOUTH:
      --y;
      break;
    case entity::MazeDirectionType::NORTH:
      ++y;
      break;
    default:
      return -1;
  }

  if (x < 0 || y < 0 ||
      x >= MISC_CONF()->maze_width_ ||
      y >= MISC_CONF()->maze_height_) {
    return -1;
  }

  return y * MISC_CONF()->maze_width_ + x;
}

core::int32 MazeActor::GetMapGrid(core::int32 pos) const {
  if (CheckPositionValid(pos) == false) {
    LOG_ERROR("position(%d) is invalid.", pos);
    return -1;
  }

  const MazeMapManager::MazeGridVector *maze_map =
    MazeMapManager::GetInstance()->GetMap(this->floor_);
  if (NULL == maze_map) {
    LOG_ERROR("Get Map(%d) from MazeMapManager failed.", this->floor_);
    return -1;
  }

  if (pos >= (core::int32)maze_map->size()) {
    return -1;
  }
  return (*maze_map)[pos];
}

void MazeActor::GetMapGrids(std::vector<core::int32> &grids) const {
  const MazeMapManager::MazeGridVector *maze_map =
    MazeMapManager::GetInstance()->GetMap(this->floor_);
  if (NULL == maze_map) {
    LOG_ERROR("Get Map(%d) from MazeMapManager failed.", this->floor_);
    return;
  }

  // 入口
  grids.push_back(entity::MazeGridType::NORMAL);
  // 随机部分
  for (size_t i = 1; i < maze_map->size() - 1; ++i) {
    if (this->explored_area_.find(i) != this->explored_area_.end()) {
      grids.push_back((*maze_map)[i]);
    } else {
      grids.push_back(entity::MazeGridType::UNEXPLORED);
    }
  }
  // 出口
  grids.push_back(entity::MazeGridType::NORMAL);
}

core::int32 MazeActor::GetActionPointRecoverCooling(time_t now) const {
  if (this->ap_ >= this->GetMaxActionPoint()) {
    return 0;
  }

  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  core::int32 max_cooling = MISC_CONF()->maze_action_point_recover_cooling_;
  
  if (now < last_ap_recover_time_) {
    return max_cooling;
  }

  return std::min((time_t)max_cooling,
                  std::max((time_t)0, max_cooling - (now - this->last_ap_recover_time_)));
}

void MazeActor::AddActionPoint(core::int32 ap, bool synchronize) {
  if (ap <= 0) {
    return;
  }

  bool ap_full = false;

  this->ap_ += ap;
  if (this->ap_ >= this->GetMaxActionPoint()) {
    this->ap_ = this->GetMaxActionPoint();
    this->RemoveRecoverTimer();
    ap_full = true;
  }

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_action_point_(this->ap_);
    if (ap_full) {
      sync.__set_action_point_recover_cooling_(0);
    }
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

void MazeActor::DecreaseActionPoint(core::int32 ap, bool synchronize) {
  if (ap <= 0) {
    return;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  bool ap_full = (this->ap_ == this->GetMaxActionPoint());

  this->ap_ -= ap;
  if (this->ap_ <= 0) {
    this->ap_ = 0;
  }

  if (ap_full) {
    this->last_ap_recover_time_ = now;
    this->AddRecoverTimer(this->GetActionPointRecoverCooling(now));
  }

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_action_point_(this->ap_);
    if (ap_full) {
      sync.__set_action_point_recover_cooling_(this->GetActionPointRecoverCooling(now));
    }
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

void MazeActor::RecoverActionPoint(bool synchronize) {
  if (this->ap_ >= this->GetMaxActionPoint()) {
    return;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  ++this->ap_;
  this->last_ap_recover_time_ = now; 

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_action_point_(this->ap_);
    sync.__set_action_point_recover_cooling_(this->GetActionPointRecoverCooling(now));
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

void MazeActor::AdjustActionPoint(entity::VipType::type old_vip_type,
    entity::VipType::type new_vip_type, bool synchronize) {
  const global::configure::VipCell *old_vip_extra_ap_cell = VIP_CONF()->GetCell(old_vip_type,
      entity::VipPrivilegeType::MAZE_ACTION_POINT);
  const global::configure::VipCell *new_vip_extra_ap_cell = VIP_CONF()->GetCell(new_vip_type,
      entity::VipPrivilegeType::MAZE_ACTION_POINT);

  core::int32 old_vip_extra_ap = 0;
  core::int32 new_vip_extra_ap = 0;

  if (old_vip_extra_ap_cell != NULL) {
    core::int32 old_vip_extra_ap = old_vip_extra_ap_cell->value1_;
  }
  if (new_vip_extra_ap_cell != NULL) {
    core::int32 new_vip_extra_ap = new_vip_extra_ap_cell->value1_;
  }

  core::int32 old_max_ap = MISC_CONF()->maze_max_action_point_ + old_vip_extra_ap;
  core::int32 new_max_ap = MISC_CONF()->maze_max_action_point_ + new_vip_extra_ap;

  if (new_max_ap > old_max_ap) {
    // 新上限大于旧上限, 增加差值
    this->ap_ += new_max_ap - old_max_ap;

  } else if (new_max_ap < old_max_ap) {
    // 新上限小于旧上限
    if (this->ap_ > new_max_ap) {
      this->ap_ = new_max_ap;
      // 原来的ap未满, 更改后已满, 需要移除定时器
      if (this->ap_ < old_max_ap) {
        this->RemoveRecoverTimer();
      }
    }
  }

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_action_point_(this->ap_);
    if (this->ap_ == new_max_ap) {
      sync.__set_action_point_recover_cooling_(0);
    }
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

bool MazeActor::CheckPositionExlored(core::int32 pos) const {
  if (0 == pos || MISC_CONF()->maze_width_ * MISC_CONF()->maze_height_ - 1 == pos) {
    return true;
  }
  return this->explored_area_.find(pos) != this->explored_area_.end();
}

void MazeActor::Move(core::int32 pos, bool synchronize) {
  if (CheckPositionValid(pos) == false) {
    LOG_ERROR("position(%d) is invalid.", pos);
    return;
  }

  if (CheckPositionExlored(pos) == false) {
    this->explored_area_.insert(pos);
  }
  this->recent_position_.push_back(pos);
  while (this->recent_position_.size() > 3) {
    this->recent_position_.pop_front();
  }

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_x_(this->GetX());
    sync.__set_y_(this->GetY());
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

void MazeActor::MoveToStartPosition(bool synchronize) {
  this->recent_position_.clear();
  this->recent_position_.push_back(0);

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_x_(this->GetX());
    sync.__set_y_(this->GetY());
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

void MazeActor::MoveBack(core::int32 step, bool synchronize) {
  if (step <= 0) {
    return;
  }

  for (core::int32 i = 0; i < step; ++i) {
    if (this->recent_position_.size() <= 1) {
      break;
    } else {
      this->recent_position_.pop_back();
    }
  }

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_x_(this->GetX());
    sync.__set_y_(this->GetY());
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

void MazeActor::EnterNextFloor(core::int32 floor, bool synchronize) {
  const MazeMapManager::MazeGridVector *maze_map =
    MazeMapManager::GetInstance()->GetMap(floor);
  if (NULL == maze_map) {
    LOG_ERROR("Get Map(%d) from MazeMapManager failed.", floor);
    return;
  }

  this->floor_ = floor;
  this->explored_area_.clear();
  this->recent_position_.clear();
  this->recent_position_.push_back(0);

  if (synchronize) {
    gateway::protocol::MessageMazeEnterNextFloorNotify notify;
    notify.__set_floor_(this->floor_);
    this->GetMapGrids(notify.grids_);
    this->playing_actor_->SendMessage(notify,
        gateway::protocol::MessageType::MESSAGE_MAZE_ENTER_NEXT_FLOOR_NOTIFY);
  }
}

void MazeActor::AddGiftNumber(bool synchronize) {
  ++this->gift_number_;

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_gift_num_(this->gift_number_);
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

void MazeActor::DecreaseGiftNumber(bool synchronize) {
  if (this->gift_number_ > 0) {
    --this->gift_number_;
  }

  if (synchronize) {
    gateway::protocol::MessageMazeSynchronize sync;
    sync.__set_gift_num_(this->gift_number_);
    this->playing_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_MAZE_SYNCHRONIZE);
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

