//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-14 14:42:20.
// File name: load_drop_configure.cc
//
// Description:
// Define class LoadDropConfigure.
//

#include "game_server/server/load_drop_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

LoadDropConfigure::LoadDropConfigure() {}
LoadDropConfigure::~LoadDropConfigure() {}

bool LoadDropConfigure::LoadDropConfig(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s) 失败", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("找不到结点[data]，文件(%s)", file.c_str());
    return false;
  }

  int group = 0;

  DropCell cell;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("group", &group) == NULL) {
      LOG_ERROR("找不到[cell-group]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("odds", (int *)&cell.odds_) == NULL) {
      LOG_ERROR("找不到[cell-odds]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("sub_id", (int *)&cell.sub_id_) == NULL) {
      LOG_ERROR("找不到[cell-sub_id]，文件(%s)", file.c_str());
      return false;
    }
    DropMap::iterator iterator = this->drops_.find(group);
    if(iterator != this->drops_.end()) {
      iterator->second.push_back(cell);
    } else {
      DropVector drop_vector;
      drop_vector.push_back(cell);
      this->drops_.insert(std::make_pair(group, drop_vector));
    }
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool LoadDropConfigure::LoadDropSubConfig(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s) 失败", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("找不到结点[data]，文件(%s)", file.c_str());
    return false;
  }

  int group = 0;

  DropSubCell cell;
  int resource_type = 0;
  int resource_id = 0;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("group", &group) == NULL) {
      LOG_ERROR("找不到[cell-group]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("odds", (int *)&cell.odds_) == NULL) {
      LOG_ERROR("找不到[cell-odds]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("bind", (int *)&cell.bind_) == NULL) {
      LOG_ERROR("找不到[cell-bind]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("resource_type", &resource_type) == NULL) {
      LOG_ERROR("找不到[cell-resource_type]，文件(%s)", file.c_str());
      return false;
    }
    if(resource_type <= entity::ResourceType::MIN || resource_type >= entity::ResourceType::MAX) {
      LOG_ERROR("[cell-resource_type] %d 错误", resource_type);
      return false;
    }
    cell.resource_type_ = (entity::ResourceType::type)resource_type;
    if(cell_node->Attribute("resource_id", &resource_id) == NULL) {
      LOG_ERROR("找不到[cell-resource_id]，文件(%s)", file.c_str());
      return false;
    }
    if(cell.resource_type_ == entity::ResourceType::RESOURCE &&
        (resource_id <= entity::ResourceID::MIN || resource_id >= entity::ResourceID::MAX)) {
      LOG_ERROR("[cell-resource_id] %d 错误", resource_id);
      return false;
    }
    cell.resource_id_ = resource_id;
    if(cell_node->Attribute("max_num",(int *)&cell.max_) == NULL) {
      LOG_ERROR("找不到[cell-max_num]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("min_num",(int *)&cell.min_) == NULL) {
      LOG_ERROR("找不到[cell-min_num]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("intensify",(int *)&cell.intensify_) == NULL) {
      LOG_ERROR("找不到[cell-intensify]，文件(%s)", file.c_str());
      return false;
    }
    DropSubMap::iterator iterator = this->drop_subs_.find(group);
    if(iterator != this->drop_subs_.end()) {
      iterator->second.push_back(cell);
    } else {
      DropSubVector drop_sub_vector;
      drop_sub_vector.push_back(cell);
      this->drop_subs_.insert(std::make_pair(group, drop_sub_vector));
    }
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const LoadDropConfigure::DropElementVector &LoadDropConfigure::Drop(core::int32 id) const {
  DropElement element;
  this->elements_.clear();

  DropMap::const_iterator iterator1 = this->drops_.find(id);
  if(iterator1 != this->drops_.end()) {
    for(size_t i = 0; i < iterator1->second.size(); ++i) {
      core::int32 rand =
        ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
      if(iterator1->second[i].odds_ < rand) {
        continue;
      }
      DropSubMap::const_iterator iterator2 =
        this->drop_subs_.find(iterator1->second[i].sub_id_);
      if(iterator2 ==this->drop_subs_.end()) {
        continue;
      }
      rand = ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
      core::int32 odds_comp = 0;
      for(size_t j = 0; j < iterator2->second.size(); ++j) {
        odds_comp += iterator2->second[j].odds_;
        if(odds_comp >=  rand) {
          element.resource_type_ = iterator2->second[j].resource_type_;
          element.resource_id_ = iterator2->second[j].resource_id_;
          element.bind_ = iterator2->second[j].bind_;
          element.intensify_level_ = iterator2->second[j].intensify_;
          core::int32 differ = iterator2->second[j].max_ - iterator2->second[j].min_;
          if(differ <= 0) {
            element.number_ = iterator2->second[j].min_;
          } else {
            element.number_ = iterator2->second[j].min_ + rand % differ;
          }
          this->elements_.push_back(element);
          break;
        }
      }
    }
  }

  return this->elements_;
}

}  // namespace server

}  // namespace game

