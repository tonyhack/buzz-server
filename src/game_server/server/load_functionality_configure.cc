//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-02 11:52:40.
// File name: load_functionality_configure.cc
//
// Description:
//
#include "game_server/server/load_functionality_configure.h"

#include "game_server/server/configure.h"
#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

LoadFunctionalityConfigure::LoadFunctionalityConfigure() {}
LoadFunctionalityConfigure::~LoadFunctionalityConfigure() {}

bool LoadFunctionalityConfigure::LoadConfigure(const std::string& file) {
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

  // 加载 cell 结点
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    FunctionalityOpenCell cell;
    const char* str = cell_node->Attribute("function");
    if(str == NULL) {
      LOG_ERROR("找不到属性[cell-function]，文件(%s)", file.c_str());
      return false;
    }
    cell.func_str = str;
    cell.function_ = Configure::GetInstance()->StrConvertToFunctionality(str);
    core::int32 open_type = 0;
    if(cell_node->Attribute("type", (int *)(&open_type)) == NULL) {
      LOG_ERROR("找不到属性[cell-open_type]，文件(%s)", file.c_str());
      return false;
    }
    cell.open_type_ = (entity::FunctionalityOpenType::type)open_type;

    if(cell_node->Attribute("param", (int *)(&cell.param_)) == NULL) {
      LOG_ERROR("找不到属性[cell-param]，文件(%s)", file.c_str());
      return false;
    }

    if(cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("找不到属性[cell-id], 文件(%s)", file.c_str());
      return false;
    }

    if(cell.open_type_ == entity::FunctionalityOpenType::ACTOR_LEVEL) {
      this->level_cells_.push_back(cell);
    } else if(cell.open_type_ == entity::FunctionalityOpenType::TASK_ACCEPT) {
      this->task_accept_cells_.push_back(cell);
    } else if(cell.open_type_ == entity::FunctionalityOpenType::TASK_FINISH) {
      this->task_finish_cells_.push_back(cell);
    } else if(cell.open_type_ == entity::FunctionalityOpenType::SPIRIT_LEVEL) {
      this->spirit_level_cells_.push_back(cell);
    } else if(cell.open_type_ == entity::FunctionalityOpenType::USE_ITEM) {
      this->use_item_cells_.push_back(cell);
    } else if(cell.open_type_ == entity::FunctionalityOpenType::CLIENT_REQUEST) {
      this->client_request_cells_.push_back(cell);
    } else if(cell.open_type_ == entity::FunctionalityOpenType::HOMELAND_TEMPLE_LEVEL) {
      this->homeland_temple_level_cells_.push_back(cell);
    }
    this->open_maps_.insert(OpenCellMaps::value_type(cell.id_, cell.func_str));
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

LoadFunctionalityConfigure::OpenCells& LoadFunctionalityConfigure::GetCell(
    entity::FunctionalityOpenType::type type) {
  if(type == entity::FunctionalityOpenType::ACTOR_LEVEL) {
    return this->level_cells_;
  } else if(type == entity::FunctionalityOpenType::TASK_ACCEPT) {
    return this->task_accept_cells_;
  } else if(type == entity::FunctionalityOpenType::TASK_FINISH){
    return this->task_finish_cells_;
  } else if(type == entity::FunctionalityOpenType::SPIRIT_LEVEL) {
    return this->spirit_level_cells_;
  } else if(type == entity::FunctionalityOpenType::CLIENT_REQUEST) {
    return this->client_request_cells_;
  } else if(type == entity::FunctionalityOpenType::HOMELAND_TEMPLE_LEVEL) {
    return this->homeland_temple_level_cells_;
  } else {
    return this->use_item_cells_;
  }
}

const char *LoadFunctionalityConfigure::GetFunctionalityStr(core::int32 id) {
  OpenCellMaps::iterator iter = this->open_maps_.find(id);
  if(iter == this->open_maps_.end()) {
    return NULL;
  }
  return iter->second.c_str();
}

} // namespace server

} // namespace game
