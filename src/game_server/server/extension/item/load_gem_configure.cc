#include "game_server/server/extension/item/load_gem_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/types.h"
#include "global/logging.h"
#include "game_server/server/extension_manager.h"

namespace game {

namespace server {

namespace item {

LoadGemConfigure::LoadGemConfigure() {}
LoadGemConfigure::~LoadGemConfigure() {}

bool LoadGemConfigure::LoadConfigure(const std::string &gem_file,
    const std::string &gem_gathering_file) {
  TiXmlElement *data_node = NULL;
  TiXmlElement *cell_node = NULL; 
  core::int32 current_id = -1;
  core::int32 last_id = -1;

  // 初始化 quality_group_gems_
  this->quality_group_gems_.resize(GemCell::QualityType::MAX + 1);

  // 加载 gem.xml
  TiXmlDocument gem_doc;
  if (gem_doc.LoadFile(gem_file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed", gem_file.c_str());
    return false;
  }
  // --加载节点 data 
  data_node = gem_doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", gem_file.c_str());
    return false;
  }
  // --加载节点集合 cell
  last_id = -1;

  cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    GemCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", gem_file.c_str());
      return false;
    }
    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", gem_file.c_str());
      return false;
    }

    // 检查 quality
    core::int32 quality;
    if (cell_node->Attribute("quality", (int *)&quality) == false) {
      LOG_ERROR("Config file (%s) /data/cell[quality] not found.", gem_file.c_str());
      return false;
    }
    if (quality < GemCell::QualityType::MIN ||
        quality > GemCell::QualityType::MAX) {
      LOG_ERROR("Config file (%s) /data/cell[quality] is invalid.", gem_file.c_str());
      return false;
    } else {
      cell.quality_ = (GemCell::QualityType::type)quality;
    }

    if (cell_node->Attribute("exp", (int *)&cell.exp_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[exp] not found.", gem_file.c_str());
      return false;
    }
    if (cell_node->Attribute("promote_spend_shard_count", (int *)&cell.promote_spend_shard_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[promote_spend_shard_count] not found.", gem_file.c_str());
      return false;
    }
    if (cell_node->Attribute("promote_exp_reward", (int *)&cell.promote_exp_reward_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[promote_exp_reward] not found.", gem_file.c_str());
      return false;
    }
    if (cell_node->Attribute("decompose_get_shard_count", (int *)&cell.decompose_get_shard_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[decompose_get_shard_count] not found.", gem_file.c_str());
      return false;
    }
    if (cell_node->Attribute("shard_per_exp", (int *)&cell.shard_per_exp_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[shard_per_exp] not found.", gem_file.c_str());
      return false;
    }

    // 检查 attr_id
    core::int32 attr_id;
    if (cell_node->Attribute("attr_id", (int *)&attr_id) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_id] not found.", gem_file.c_str());
      return false;
    }
    if (attr_id < GemCell::AttrType::MIN ||
        attr_id > GemCell::AttrType::MAX) {
      LOG_ERROR("Config file (%s) /data/cell[attr_id] is invalid.", gem_file.c_str());
      return false;
    } else {
      cell.attr_id_ = (GemCell::AttrType::type)attr_id;
    }

    if (cell_node->Attribute("attr_value", (int *)&cell.attr_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_value] not found.", gem_file.c_str());
      return false;
    }

    core::uint64 index = ((core::uint64)cell.id_ << 32) + (core::uint64)cell.level_;
    this->gems_.insert(std::make_pair(index, cell));
    if (last_id != cell.id_) {
      this->quality_group_gems_[cell.quality_].push_back(cell.id_);
      last_id = cell.id_;
    }

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 加载gem_gathering.xml
  TiXmlDocument gem_gathering_doc;
  if (gem_gathering_doc.LoadFile(gem_gathering_file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed", gem_gathering_file.c_str());
    return false;
  }
  // --加载节点 data 
  data_node = gem_gathering_doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", gem_gathering_file.c_str());
    return false;
  }
  // --加载节点集合 cell
  current_id = -1;
  last_id = -1;
  GemGatheringPointCell *point_cell_ptr = NULL;

  cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {

    if (cell_node->Attribute("id", (int *)&current_id) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", gem_gathering_file.c_str());
      return false;
    }

    // 处理 POINT_CELL
    if (current_id != last_id) {
      GemGatheringPointCell point_cell;
      point_cell.id_ = current_id;
    
      if (cell_node->Attribute("spend_gold", (int *)&point_cell.spend_gold_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[spend_gold] not found.", gem_gathering_file.c_str());
        return false;
      }

      core::int32 skip;
      if (cell_node->Attribute("skip", (int *)&skip) == false) {
        LOG_ERROR("Config file (%s) /data/cell[skip] not found.", gem_gathering_file.c_str());
        return false;
      }
      point_cell.skip_ = skip;

      if (cell_node->Attribute("spend_freedom_dollars", (int *)&point_cell.spend_freedom_dollars_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[spend_freedom_dollars] not found.", gem_gathering_file.c_str());
        return false;
      }

      if (cell_node->Attribute("enter_next_point_odds", (int *)&point_cell.enter_next_point_odds_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[enter_next_point_odds] not found.", gem_gathering_file.c_str());
        return false;
      }

      this->gem_gathering_points_store_.push_back(point_cell);
      this->gem_gathering_points_index_[point_cell.id_] =
        this->gem_gathering_points_store_.size() - 1;

      last_id = current_id;
      point_cell_ptr = &this->gem_gathering_points_store_.back();
    }

    // 处理 RESULT_CELL
    GemGatheringResultCell result_cell;

    if (cell_node->Attribute("result_id", (int *)&result_cell.result_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[result_id] not found.", gem_gathering_file.c_str());
      return false;
    }
    if (cell_node->Attribute("odds", (int *)&result_cell.odds_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[odds] not found.", gem_gathering_file.c_str());
      return false;
    }

    // --检查 bonus_id
    core::int32 bonus_id;
    if (cell_node->Attribute("bonus_id", (int *)&bonus_id) == false) {
      LOG_ERROR("Config file (%s) /data/cell[bonus_id] not found.", gem_gathering_file.c_str());
      return false;
    }
    if (bonus_id < GemGatheringResultCell::BonusType::MIN ||
        bonus_id > GemGatheringResultCell::BonusType::MAX) {
      LOG_ERROR("Config file (%s) /data/cell[bonus_id] is invalid.", gem_gathering_file.c_str());
      return false;
    } else {
      result_cell.bonus_id_ = (GemGatheringResultCell::BonusType::type)bonus_id;
    }

    if (cell_node->Attribute("bonus_count", (int *)&result_cell.bonus_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[bonus_count] not found.", gem_gathering_file.c_str());
      return false;
    }

    point_cell_ptr->results.push_back(result_cell);

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const GemCell *LoadGemConfigure::GetGem(core::int32 id, core::int32 level) const {
  core::uint64 index = ((core::uint64)id << 32) + (core::uint64)level;

  GemHashmap::const_iterator iter = this->gems_.find(index); 
  if (iter == this->gems_.end()) {
    return NULL;
  }
  return &iter->second;
}

core::int32 LoadGemConfigure::GetRandomGemIDByQuality(GemCell::QualityType::type type) const {
  core::uint32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(this->quality_group_gems_[type].size());

  return this->quality_group_gems_[type][random];
}

const GemGatheringPointCell *LoadGemConfigure::GetGemGatheringPoint(core::int32 id) const {
  GemGatheringPointIndexHashmap::const_iterator iter =
    this->gem_gathering_points_index_.find(id);
  if (iter == this->gem_gathering_points_index_.end()) {
    return NULL;
  }
  return &this->gem_gathering_points_store_[iter->second];
}

const GemGatheringPointCell *LoadGemConfigure::GetNextGemGatheringPoint(core::int32 id) const {
  if (0 == id && this->gem_gathering_points_store_.size() >= 1) {
    return &this->gem_gathering_points_store_[0];
  }

  GemGatheringPointIndexHashmap::const_iterator iter =
    this->gem_gathering_points_index_.find(id);
  if (iter == this->gem_gathering_points_index_.end()) {
    return NULL;
  }
  if (iter->second >= this->gem_gathering_points_store_.size() - 1) {
    return NULL;
  }
  return &this->gem_gathering_points_store_[iter->second + 1];
}

const GemGatheringPointCell *LoadGemConfigure::GetPrevGemGatheringPoint(core::int32 id) const {
  GemGatheringPointIndexHashmap::const_iterator iter =
    this->gem_gathering_points_index_.find(id);
  if (iter == this->gem_gathering_points_index_.end()) {
    return NULL;
  }
  if (iter->second < 1) {
    return NULL;
  }
  return &this->gem_gathering_points_store_[iter->second - 1];
}

const GemGatheringPointCell *LoadGemConfigure::GetLastGemGatheringPoint() const {
  if (this->gem_gathering_points_store_.size() == 0) {
    return NULL;
  }
  return &this->gem_gathering_points_store_.back();
}

}  // namespace item

}  // namespace server

}  // namespace game

