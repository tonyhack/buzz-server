#include "game_server/server/extension/item/load_hunt_treasure_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadHuntTreasureConfigure::LoadHuntTreasureConfigure() {}
LoadHuntTreasureConfigure::~LoadHuntTreasureConfigure() {}

bool LoadHuntTreasureConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s)出错", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("加载配置文件(%s) [data] 结点出错", file.c_str());
    return false;
  }

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    HuntTreasureRandCell cell;
    if(cell_node->Attribute("item", (int *)(&cell.item_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-item] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("odds", (int *)(&cell.odds_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-odds] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("position", (int *)(&cell.position_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-position] 出错", file.c_str());
      return false;
    }
    core::int32 id = 0;
    if(cell_node->Attribute("id", (int *)(&id)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-id] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("integral_type", (int *)(&cell.integral_type_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-integral_type] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("drop", (int *)(&cell.drop_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-drop] 出错", file.c_str());
      return false;
    }
    RandMap::iterator iter = this->randon_map_.find(id);
    if(iter == this->randon_map_.end()) {
      RandVec temp;
      temp.push_back(cell);
      this->randon_map_.insert(RandMap::value_type(id, temp));
    } else {
      iter->second.push_back(cell);
    }

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

HuntTreasureRandCell *LoadHuntTreasureConfigure::GetRand(core::int32 id) {
  RandMap::iterator iter = this->randon_map_.find(id + 1);
  if(iter == this->randon_map_.end()) {
    return 0;
  }
  core::int32 total_rand = 0;
  for(size_t i = 0; i < iter->second.size(); ++i) {
    total_rand += iter->second[i].odds_;
  }
  core::int32 rand = (core::int32)ExtensionManager::GetInstance()->GetRandom().Random(total_rand);
  total_rand = 0;
  for(size_t i = 0; i < iter->second.size(); ++i) {
    total_rand += iter->second[i].odds_;
    if(rand <= total_rand) {
      return &(iter->second[i]);
    }
  }

  return 0;
}

IntegralType::type LoadHuntTreasureConfigure::GetItegralType(core::int32 id) {
  RandMap::const_iterator iter = this->randon_map_.find(id + 1);
  if(iter == this->randon_map_.end()) {
    return IntegralType::MIRACLE;
  }

  return (IntegralType::type)iter->second[0].integral_type_;
}

} // namespace item

} // namespace server

} // namespace game
