#include "session_server/server/extension/playing/maze_map_manager.h"

#include <cstdlib>
#include <algorithm>

#include "entity/maze_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/configure/configure.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/world_data_manager.h"
#include "session_server/server/session_server.h"

namespace session {

namespace server {

namespace playing {

MazeMapManager::MazeMapManager() : last_update_time_(0) {}
MazeMapManager::~MazeMapManager() {}

bool MazeMapManager::Initialize() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t reset_time_in_week = GlobalTimeTick::GetInstance()->GetWeekTime(
      MISC_CONF()->maze_reset_time_week_day_,
      MISC_CONF()->maze_reset_time_hour_,
      MISC_CONF()->maze_reset_time_minute_,
      MISC_CONF()->maze_reset_time_second_, now);
  std::string db_data =
    WorldDataManager::GetInstance()->GetWorldData(WorldDataType::MAZE_MAPS);

  if (db_data.empty()) {
    // 重置地宫
    this->ResetMazeMaps(true);
  } else {
    // 数据解析
    if (this->Deserialize(db_data) == false) {
      LOG_ERROR("Deserialize data failed.");
      return false;
    }
    // 根据上次更新时间判断是否需要更新
    if (this->last_update_time_ < reset_time_in_week &&
        now >= reset_time_in_week) {
      // 重置地宫
      this->ResetMazeMaps(true);
    }
  }

  // 设置重置定时器
  this->AddResetMazeTimer(now);

  return true;
}

void MazeMapManager::Finalize() {
  this->RemoveResetMazeTimer();
  this->last_update_time_ = 0;
  this->maps_.clear();
}

void MazeMapManager::ExportMapInfo(session::protocol::MessageMazeMapInfoSynchronize &sync) {
  for (size_t i = 0; i < this->maps_.size(); ++i) {
    session::protocol::MazeMapData data;
    data.__set_grids_(this->maps_[i]);
    sync.maps_.push_back(data);
  }
  sync.__set_last_update_time_(this->last_update_time_);
}

static bool DecodeMazeGridVector(const std::string &source,
    MazeMapManager::MazeGridVector *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ":", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    core::int32 grid_id = atoi(split_result[i].c_str());
    // 检查格子ID
    if (grid_id != entity::MazeGridType::UNEXPLORED &&
        grid_id != entity::MazeGridType::NORMAL &&
        MAZE_CONF()->GetMazeGrid(grid_id) == NULL) {
      LOG_ERROR("maze grid id(%d) is invalid.", grid_id);
      return false;
    }
    result->push_back(grid_id);
  }

  if (result->size() != (size_t)(MISC_CONF()->maze_width_ * MISC_CONF()->maze_height_)) {
    LOG_ERROR("maze grid size(%zd) is invalid.", result->size());
    return false;
  }

  return true;
}

static bool DecodeMazeMapVector(const std::string &source,
    MazeMapManager::MazeMapVector *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    MazeMapManager::MazeGridVector grid_vector;
    if (DecodeMazeGridVector(split_result[i], &grid_vector) == false) {
      LOG_ERROR("Map data(%s) is invalid.", split_result[i].c_str());
      return false;
    }
    result->push_back(grid_vector);
  }

  if (result->size() != (size_t)MISC_CONF()->maze_max_floor_) {
    LOG_ERROR("maze map size(%zd) is not correct.", result->size());
    return false;
  }

  return true;
}

bool MazeMapManager::Deserialize(const std::string &input) {
  std::vector<std::string> split_result;
  global::StringSplit(input.c_str(), "|", split_result);
  if (split_result.size() < 2) {
    LOG_ERROR("Data(%s) is invalid.", input.c_str());
    return false;
  }

  this->last_update_time_ = atol(split_result[0].c_str());
  if (DecodeMazeMapVector(split_result[1], &this->maps_) == false) {
    LOG_ERROR("Maps data(%s) is invalid.", split_result[1].c_str());
    return false;
  }

  return true;
}

void MazeMapManager::Serialize(std::string *ouput) {
  char buffer[4096] = "";
  size_t count = 0;

  count += snprintf(buffer + count, sizeof(buffer) - count, "%lu|", this->last_update_time_);

  for (size_t i = 0; i < this->maps_.size(); ++i) {
    for (size_t j = 0; j < this->maps_[i].size(); ++j) {
      count += snprintf(buffer + count, sizeof(buffer) - count, "%d:", this->maps_[i][j]);
    }
    count += snprintf(buffer + count, sizeof(buffer) - count, ",");
  }

  *ouput = buffer;
}

void MazeMapManager::AddResetMazeTimer(time_t now) {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }
  time_t reset_time_in_week = GlobalTimeTick::GetInstance()->GetWeekTime(
      MISC_CONF()->maze_reset_time_week_day_,
      MISC_CONF()->maze_reset_time_hour_,
      MISC_CONF()->maze_reset_time_minute_,
      MISC_CONF()->maze_reset_time_second_, now);

  time_t next_reset_time;
  if (now < reset_time_in_week) {
    next_reset_time = reset_time_in_week;
  } else {
    next_reset_time = reset_time_in_week + 7 * 24 * 60 * 60;
  }

  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_RESET_MAP,
    (next_reset_time - now) * 1000, -1, this, "MazeMapManager::AddResetMazeTimer");
}

void MazeMapManager::RemoveResetMazeTimer() {
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_RESET_MAP, this);
}

void MazeMapManager::ResetMazeMaps(bool startup) {

  this->maps_.clear();
  core::int32 random_grid_count =
    MISC_CONF()->maze_width_ * MISC_CONF()->maze_height_ - 2;

  for (core::int32 i = 0; i < MISC_CONF()->maze_max_floor_; ++i) {
    MazeGridVector grid_vector;
    MazeGridVector random_grid_vector;

    // 生成地图
    global::configure::MazeConfigure::MazeGridMap::const_iterator iter =
      MAZE_CONF()->GetAllMazeGrids().begin();
    for (; iter != MAZE_CONF()->GetAllMazeGrids().end(); ++iter) {
      const global::configure::MazeGridCell *cell = &iter->second;
      random_grid_vector.insert(random_grid_vector.end(), cell->number_, cell->id_);
    }
    random_grid_vector.resize(random_grid_count, entity::MazeGridType::NORMAL);
    std::random_shuffle(random_grid_vector.begin(), random_grid_vector.end());

    // 入口
    grid_vector.push_back(entity::MazeGridType::NORMAL);
    // 随机部分
    grid_vector.insert(grid_vector.end(),
        random_grid_vector.begin(), random_grid_vector.end());
    // 出口
    grid_vector.push_back(entity::MazeGridType::NORMAL);

    this->maps_.push_back(grid_vector);
  }

  this->last_update_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  std::string db_data;
  this->Serialize(&db_data);

  if (startup) {
    WorldDataManager::GetInstance()->SyncSaveWorldData(
        WorldDataType::MAZE_MAPS, db_data);
  } else {
    WorldDataManager::GetInstance()->SaveWorldData(
        WorldDataType::MAZE_MAPS, db_data);
    // 同步 game_server
    session::protocol::MessageMazeMapInfoSynchronize game_sync;
    MazeMapManager::GetInstance()->ExportMapInfo(game_sync);
    SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
        session::protocol::GameMessageType::MESSAGE_MAZE_MAP_INFO_SYNCHRONIZE);
  }
}

void MazeMapManager::OnTimer(core::uint32 id) {
  if (TIMER_ID_RESET_MAP == id) {
    this->ResetMazeMaps();
    this->RemoveResetMazeTimer();
    this->AddResetMazeTimer();
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session

