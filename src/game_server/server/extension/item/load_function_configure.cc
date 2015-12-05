//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-25 21:50:52.
// File name: load_function_configure.cc
//
// Description:
//
#include "game_server/server/extension/item/load_function_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/configure.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadFunctionConfigure::LoadFunctionConfigure() {}
LoadFunctionConfigure::~LoadFunctionConfigure() {}

bool LoadFunctionConfigure::LoadConfigure(const std::string &file) {
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
    FunctionCell cell;
    const char* str = cell_node->Attribute("id");
    if(str ==  NULL) {
      LOG_ERROR("开启功能配置,功能ID没有找到");
      return false;
    }
    cell.id_ = game::server::Configure::GetInstance()->StrConvertToFunctionality(str);

    if(cell_node->Attribute("dollars", (int *)(&cell.dollars_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-dollars] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_type", (int *)(&cell.buy_type_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-buy_type] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("spent_type", (int *)(&cell.spent_type_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-spent_type] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("time_stop_type", (int *)(&cell.time_stop_type_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-time_stop_type] 出错", file.c_str());
      return false;
    }
    const char *str_start = cell_node->Attribute("limit_start");
    if(str_start == NULL) {
      LOG_ERROR("加载配置文件(%s) [cell-limit_start] 出错", file.c_str());
      return false;
    } else {
      if(strcmp(cell_node->Attribute("limit_start"),"0") == 0) {
        cell.limit_start_ = 0;
      } else {
        cell.limit_start_ = global::FormatTime(cell_node->Attribute("limit_start"));
      }
    }
    const char *str_stop = cell_node->Attribute("limit_stop");
    if(str_stop == NULL) {
      LOG_ERROR("加载配置文件(%s) [cell-limit_stop] 出错", file.c_str());
      return false;
    } else {
      if(strcmp(cell_node->Attribute("limit_stop"),"0") == 0) {
        cell.limit_start_ = 0;
      } else {
        cell.limit_start_ = global::FormatTime(cell_node->Attribute("limit_stop"));
      }
    }

    if(cell_node->Attribute("freedom_dollars", (int *)(&cell.freedom_dollars_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-freedom_dollars] 出错", file.c_str());
      return false;
    }
    this->function_cells_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

const FunctionCell *LoadFunctionConfigure::GetFunctionCell(core::int32 id) const {
  FunctionCellMap::const_iterator itr = this->function_cells_.find(id);
  if(itr != this->function_cells_.end()) {
    return &itr->second;
  }
  return NULL;
}

} // namespace item

} // namespace  server

} // namespace game

