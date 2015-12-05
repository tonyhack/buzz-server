#include "global/configure/maze_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace global {

namespace configure {

MazeConfigure::MazeConfigure() {}
MazeConfigure::~MazeConfigure() {}

bool MazeConfigure::LoadConfigure(const std::string &file) {
  const std::string &config_dir =
      global::ServerConfigureSingleton::GetInstance().GetConfigueDir();
  const char *node_attr = NULL;

  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 configure
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if (NULL == configure_node) {
    LOG_ERROR("Config file (%s) /configure not found.", file.c_str());
    return false;
  }

  // 加载节点 maze_grid
  TiXmlElement *maze_grid_node = configure_node->FirstChildElement("maze_grid");
  if (NULL == maze_grid_node) {
    LOG_ERROR("Config file (%s) /configure/maze_grid not found.", file.c_str());
    return false;
  }
  node_attr = maze_grid_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/maze_grid[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadMazeGrid(config_dir + node_attr) == false) {
    LOG_ERROR("Load maze grid config file failed.");
    return false;
  }

  // 加载节点 maze_grid_bonus
  TiXmlElement *maze_grid_bonus_node = configure_node->FirstChildElement("maze_grid_bonus");
  if (NULL == maze_grid_bonus_node) {
    LOG_ERROR("Config file (%s) /configure/maze_grid_bonus not found.", file.c_str());
    return false;
  }
  node_attr = maze_grid_bonus_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/maze_grid_bonus[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadMazeGridBonus(config_dir + node_attr) == false) {
    LOG_ERROR("Load maze grid bonus config file failed.");
    return false;
  }

  return true;
}

bool MazeConfigure::LoadMazeGrid(const std::string &file) {
  TiXmlDocument doc;

  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 data
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", file.c_str());
    return false;
  }

  // 加载节点集合 cell
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    MazeGridCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("number", (int *)&cell.number_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[number] not found.", file.c_str());
      return false;
    }

    this->maze_grids_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool MazeConfigure::LoadMazeGridBonus(const std::string &file) {
  TiXmlDocument doc;

  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 data
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", file.c_str());
    return false;
  }

  // 加载节点集合 cell
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    MazeGridBonusCell bonus_cell;
    core::int32 grid_id;

    if (cell_node->Attribute("grid_id", (int *)&grid_id) == false) {
      LOG_ERROR("Config file (%s) /data/cell[grid_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("bonus_id", (int *)&bonus_cell.bonus_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[bonus_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("odds", (int *)&bonus_cell.odds_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[odds] not found.", file.c_str());
      return false;
    }

    {
      core::int32 type;
      if (cell_node->Attribute("type", (int *)&type) == false) {
        LOG_ERROR("Config file (%s) /data/cell[type] not found.", file.c_str());
        return false;
      }
      if (type < MazeGridBonusCell::BonusType::MIN ||
          type >= MazeGridBonusCell::BonusType::MAX) {
        LOG_ERROR("Config file (%s) /data/cell[type] is invalid.", file.c_str());
        return false;
      }
      bonus_cell.type_ = (MazeGridBonusCell::BonusType::type)type;
    }

    if (cell_node->Attribute("id", (int *)&bonus_cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("min_number", (int *)&bonus_cell.min_number_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[min_number] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_number", (int *)&bonus_cell.max_number_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_number] not found.", file.c_str());
      return false;
    }

    // --check
    if (bonus_cell.min_number_ < 0 ) {
      LOG_ERROR("Config file (%s) /data/cell[min_number] is invalid.", file.c_str());
      return false;
    }
    if (bonus_cell.max_number_ < bonus_cell.min_number_) {
      LOG_ERROR("Config file (%s) /data/cell[max_number] is invalid.", file.c_str());
      return false;
    }

    MazeGridCell *grid_cell = const_cast<MazeGridCell *>(
        this->GetMazeGrid(grid_id));
    if (NULL == grid_cell) {
      LOG_ERROR("Config file (%s) /data/cell[grid_id] is invalid.", file.c_str());
      return false;
    }

    grid_cell->bonus_set_.push_back(bonus_cell);

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 建立索引
  for (MazeGridMap::iterator iter = this->maze_grids_.begin();
       iter != this->maze_grids_.end(); ++iter) {
    MazeGridCell *grid_cell = &iter->second;

    for (size_t i = 0; i < grid_cell->bonus_set_.size(); ++i) {
      MazeGridBonusCell *bonus_cell = &grid_cell->bonus_set_[i];

      core::uint64 index =
        ((core::uint64)grid_cell->id_ << 32) + (core::uint64)bonus_cell->bonus_id_;

      this->maze_grid_bonus_index_.insert(std::make_pair(index, bonus_cell));
    }
  }

  return true;
}

const MazeGridCell *MazeConfigure::GetMazeGrid(core::int32 id) const {
  MazeGridMap::const_iterator iter = this->maze_grids_.find(id);
  if (iter == this->maze_grids_.end()) {
    return NULL;
  }
  return &iter->second;
}

const MazeGridBonusCell *MazeConfigure::GetMazeGridBonus(
    core::int32 grid_id, core::int32 bonus_id) const {
  core::uint64 index = ((core::uint64)grid_id << 32) + (core::uint64)bonus_id;

  MazeGridBonusIndexHashmap::const_iterator iter =
    this->maze_grid_bonus_index_.find(index);
  if (iter == this->maze_grid_bonus_index_.end()) {
    return NULL;
  }
  return iter->second;
}

const MazeGridBonusCell *MazeConfigure::GetRandomMazeGridBonus(
    core::int32 grid_id, core::uint32 random) const {
  const MazeGridCell *grid_cell = this->GetMazeGrid(grid_id);
  if (NULL == grid_cell) {
    return NULL;
  }

  core::uint32 odds = 0;

  for (size_t i = 0; i < grid_cell->bonus_set_.size(); ++i) {
    odds += grid_cell->bonus_set_[i].odds_;
    if (odds >= random) {
      return &grid_cell->bonus_set_[i];
    }
  }

  return NULL;
}

}  // namespace configure

}  // namespace global

