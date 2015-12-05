//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 19:44:22.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/cooling/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace cooling {

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) [冷却配置] 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点标签
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到结点 [configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 cooling_pool 结点标签
  TiXmlElement *cooling_pool_node = configure_node->FirstChildElement("cooling_pool");
  if(cooling_pool_node == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到结点 [configure-cooling_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 冷却对象池初始数量
  if(cooling_pool_node->Attribute("initial_number",
        (int *)(&this->cooling_pool_.initial_number_)) == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cooling_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  // 冷却对象池扩展数量
  if(cooling_pool_node->Attribute("extend_number",
        (int *)(&this->cooling_pool_.extend_number_)) == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cooling_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 cooling_group_pool 结点标签
  TiXmlElement *cooling_group_pool_node = configure_node->FirstChildElement("cooling_group_pool");
  if(cooling_group_pool_node == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到结点 [configure-cooling_group_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 冷却对象池初始数量
  if(cooling_group_pool_node->Attribute("initial_number",
        (int *)(&this->cooling_group_pool_.initial_number_)) == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cooling_group_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  // 冷却对象池扩展数量
  if(cooling_group_pool_node->Attribute("extend_number",
        (int *)(&this->cooling_group_pool_.extend_number_)) == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cooling_group_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  const std::string &configure_dir =
    global::ServerConfigureSingleton::GetInstance().GetConfigueDir();

  // 冷却表
  TiXmlElement *cooling_file_node = configure_node->FirstChildElement("cooling");
  if(cooling_file_node == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到结点 [configure-cooling]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *cooling_file = cooling_file_node->Attribute("file");
  if(cooling_file == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cooling-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载冷却配置
  if(this->LoadCoolingConfigure(configure_dir + cooling_file) == false) {
    global::LogError("%s:%d (%s) [冷却配置] 加载冷却表失败",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 冷却组表
  TiXmlElement *group_node = configure_node->FirstChildElement("group");
  if(group_node == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到结点 [configure-group]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *group_file = group_node->Attribute("file");
  if(group_file == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [group-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载组配置
  if(this->LoadGroupConfigure(configure_dir + group_file) == false) {
    global::LogError("%s:%d (%s) [冷却配置] 加载冷却组表失败",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  return true;
}

bool Configure::LoadCoolingConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) [冷却配置] 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 结点标签
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到结点 [data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  CoolingCell cell;

  // 加载 cell 结点标签
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    core::uint32 id = 0;
    // 冷却ID
    if(cell_node->Attribute("id", (int *)(&id)) == NULL) {
      global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 是否重复
    if(this->coolings_.find(id) != this->coolings_.end()) {
      global::LogError("%s:%d (%s) [冷却配置] 冷却ID(%u) 配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }
    // 冷却组
    if(cell_node->Attribute("group", (int *)(&cell.group_)) == NULL) {
      global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cell-group]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 冷却时间
    if(cell_node->Attribute("time", (int *)(&cell.time_)) == NULL) {
      global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cell-time]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 插入表中
    this->coolings_.insert(std::make_pair(id, cell));
    // 下个结点
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool Configure::LoadGroupConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) [冷却配置] 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 结点标签
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) [冷却配置] 找不到结点 [data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  GroupCell cell;

  // 加载 data 结点标签
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    core::uint32 id = 0;
    // 冷却ID
    if(cell_node->Attribute("id", (int *)(&id)) == NULL) {
      global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 是否重复
    if(this->groups_.find(id) != this->groups_.end()) {
      global::LogError("%s:%d (%s) [冷却配置] 冷却ID(%u) 配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }
    // 冷却时间
    if(cell_node->Attribute("time", (int *)(&cell.time_)) == NULL) {
      global::LogError("%s:%d (%s) [冷却配置] 找不到属性 [cell-time]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 插入表中
    this->groups_.insert(std::make_pair(id, cell));
    // 下个结点
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const CoolingCell *Configure::GetCoolingCell(core::uint32 cooling_id) {
  CoolingHashmap::const_iterator iterator = this->coolings_.find(cooling_id);
  if(iterator != this->coolings_.end()) {
    return &iterator->second;
  }

  return NULL;
}

const GroupCell *Configure::GetGroupCell(core::uint32 group_id) {
  GroupHashmap::const_iterator iterator = this->groups_.find(group_id);
  if(iterator != this->groups_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace cooling

}  // namespace server

}  // namespace game

