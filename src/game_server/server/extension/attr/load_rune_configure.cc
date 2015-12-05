//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-21 20:20:01.
// File name: load_rune_configure.cc
//
// Description:
//
#include "game_server/server/extension/attr/load_rune_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "game_server/server/extension/attr/configure.h"

namespace game {

namespace server {

namespace attr {

LoadRuneConfigure::LoadRuneConfigure() {}
LoadRuneConfigure::~LoadRuneConfigure() {}

bool LoadRuneConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;

  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed", file.c_str());
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
    RuneCell cell;

    if (cell_node->Attribute("template_id", (int *)&cell.template_id_) == false) {
      LOG_ERROR("cell[template_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("type_1", (int *)&cell.type_1_) == false) {
      LOG_ERROR("[type_1] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("attr_1", (int *)&cell.attr_1_) == false) {
      LOG_ERROR("cell[attr_1] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("num", (int *)&cell.num_) == false) {
      LOG_ERROR("cell[num] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("fuse_template", (int *)&cell.fuse_template_) == false) {
      LOG_ERROR("cell[fuse_template] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("odds", (int *)&cell.odds_) == false) {
      LOG_ERROR("cell[odds] not found.", file.c_str());
      return false;
    }

    this->cells_.insert(std::make_pair(cell.template_id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const RuneCell *LoadRuneConfigure::GetRune(core::int32 id) const {
  RuneHashmap::const_iterator iter = this->cells_.find(id);
  if (iter == this->cells_.end()) {
    return NULL;
  }
  return &iter->second;
}

}  // namespace attr

}  // namespace server

}  // namespace game

