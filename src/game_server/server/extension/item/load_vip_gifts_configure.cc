//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-22 00:05:14.
// File name: load_vip_gifts_configure.cc
//
// Description:
//

#include "game_server/server/extension/item/load_vip_gifts_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace item {

LoadVipGiftsConfigure::LoadVipGiftsConfigure() {}
LoadVipGiftsConfigure::~LoadVipGiftsConfigure() {}

bool LoadVipGiftsConfigure::LoadConfigure(const std::string &file) {
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
  core::int32 type = 0;
  while(cell_node) {
    VipGiftAward cell;
    std::vector<core::int32> rewards;
    if(cell_node->Attribute("viplevel", (int *)(&type)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-viplevel] 出错", file.c_str());
      return false;
    }
    if(type <= 0 || type >= (core::int32)entity::VipType::MAX) {
      LOG_ERROR("vip 礼包配置错误((%d)", type);
      return false;
    }
    const char *str = cell_node->Attribute("reward1");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 4) {
      cell.type_ = rewards[0];
      cell.id_ = rewards[1];
      cell.number_ = rewards[2];
      cell.bind_ = rewards[3];
    }
    this->awards_[type-1].push_back(cell);

    str = cell_node->Attribute("reward2");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 4) {
      cell.type_ = rewards[0];
      cell.id_ = rewards[1];
      cell.number_ = rewards[2];
      cell.bind_ = rewards[3];
    }
    this->awards_[type-1].push_back(cell);

    str = cell_node->Attribute("reward3");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 4) {
      cell.type_ = rewards[0];
      cell.id_ = rewards[1];
      cell.number_ = rewards[2];
      cell.bind_ = rewards[3];
    }
    this->awards_[type-1].push_back(cell);

    str = cell_node->Attribute("reward4");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward4]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 4) {
      cell.type_ = rewards[0];
      cell.id_ = rewards[1];
      cell.number_ = rewards[2];
      cell.bind_ = rewards[3];
    }
    this->awards_[type-1].push_back(cell);

    str = cell_node->Attribute("reward5");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward5]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 4) {
      cell.type_ = rewards[0];
      cell.id_ = rewards[1];
      cell.number_ = rewards[2];
      cell.bind_ = rewards[3];
    }
    this->awards_[type-1].push_back(cell);

    str = cell_node->Attribute("reward6");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward6]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 4) {
      cell.type_ = rewards[0];
      cell.id_ = rewards[1];
      cell.number_ = rewards[2];
      cell.bind_ = rewards[3];
    }
    this->awards_[type-1].push_back(cell);

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const LoadVipGiftsConfigure::Awards *LoadVipGiftsConfigure::GetAwards(
    entity::VipType::type type) const {
  if(type <= entity::VipType::MAX -1 && type > entity::VipType::INVALID) {
    return &(this->awards_[type-1]);
  }
  return NULL;
}

}  // namespace item

}  // namespace server

}  // namespace game

