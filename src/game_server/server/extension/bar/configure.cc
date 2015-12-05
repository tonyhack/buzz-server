//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Ebar: lxf@gbar.com.
// Last modify: 2013-11-03 23:48:34.
// File name: configure.cc
//
// Description:
//

#include "game_server/server/extension/bar/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace bar {

Configure::Configure() {}
Configure::~Configure() {}

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  const char *str = NULL;
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    BarCell cell;
    if(cell_node->Attribute("id", &cell.id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    core::int32 tmp = 0;
    if(cell_node->Attribute("time", (int *)&tmp) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-time]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    cell.time_ = tmp;

    if(cell.time_ < 100) {
      LOG_ERROR("配置文件不能小于 100 ", cell.time_);
      return false;
    }

    str = cell_node->Attribute("events");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-events]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(strcmp(str, "0") != 0) {
      global::StringSplit(str, "+", cell.events_);
    }

    this->bars_.insert(std::make_pair(cell.id_, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

bool Configure::LoadBarConfigure(const std::string & file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  // 加载 bar_pool 结点
  TiXmlElement *bar_pool_node = configure_node->FirstChildElement("bar_pool");
  if(bar_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-bar_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(bar_pool_node->Attribute("initial_number",
        (int *)(&this->bar_init_num_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [bar_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(bar_pool_node->Attribute("extend_number",
        (int *)(&this->bar_extend_num_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [bar_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  // 加载 bar_configure 结点
  TiXmlElement *bar_node = configure_node->FirstChildElement("bar_configure");
  if(bar_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [bar_configure，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *str = bar_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [bar_configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(LoadConfigure(global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    LOG_ERROR("加载 进度条配置失败!");
    return false;
  }

  return true;
}

const BarCell *Configure::GetCell(core::int32 id) const {
  Configure::BarMap::const_iterator iterator
    = this->bars_.find(id);
  if(iterator == this->bars_.end()) {
    LOG_ERROR("配置文件出错,[%d]", id);
    return NULL;
  }
  return &(iterator->second);
}

}  // namespace bar

}  // namespace server

}  // namespace game

