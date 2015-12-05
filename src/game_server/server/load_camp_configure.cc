//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-17 03:02:19.
// File name: load_camp_configure.cc
//
// Description:
//
#include "game_server/server/load_camp_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

LoadCampConfigure::LoadCampConfigure() {}
LoadCampConfigure::~LoadCampConfigure() {}

bool LoadCampConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *str = NULL;
  // 加载 cell 结点
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    CampCell cell;

    if(cell_node->Attribute("camp_type_src", 
        (int *)(&cell.camp_type_src_)) == NULL) {
      global::LogError("%s:%d (%s) 阵营配置 读取属性 type1 错误",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(cell_node->Attribute("camp_type_dest",
        (int *)(&cell.camp_type_dest_)) == NULL) {
      global::LogError("%s:%d (%s) 阵营(%u)配置 读取属性 type2 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.camp_type_dest_);
      return false;
    }
    if(cell_node->Attribute("relation",
       (int *)(&cell.relation_)) == NULL) {
      global::LogError("%s:%d (%s) 阵营(%u)配置 读取属性 relation 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.relation_);
      return false;
    }
    if(cell.relation_ >= entity::CampRelationType::MAX || 
        cell.relation_ <= entity::CampRelationType::MIN) {
      LOG_ERROR("阵营关系配置错误[%d]", cell.relation_);
      return false;
    }
    // 插入表中
    this->camps_.insert(std::make_pair(CampKey(
            cell.camp_type_src_, cell.camp_type_dest_), cell));
    // 下个节点
    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

entity::CampRelationType::type LoadCampConfigure::GetRelation(core::int32 camp1,
        core::int32 camp2) const {
  CampKey key(camp1, camp2);
  CampHashMap::const_iterator iterator = this->camps_.find(key);
  if(iterator == this->camps_.end()) {
    return entity::CampRelationType::MIN;
  }
  return iterator->second.relation_;
}

}  // namespace server

}  // namespace game


