//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-16 05:31:33.
// File name: load_vip_configure.cc
//
// Description:
//

#include "global/configure/load_vip_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace global {

namespace configure {

LoadVipConfigure::LoadVipConfigure() {}
LoadVipConfigure::~LoadVipConfigure() {}

bool LoadVipConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载 配置文件(%s) 出错", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("找不到结点 [data]，文件(%s)", file.c_str());
    return false;
  }

  const char *str = NULL;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    core::int32 vip_type = 0;
    if(cell_node->Attribute("vip_type", (core::int32 *)&vip_type) == false) {
      LOG_ERROR("找不到属性 [cell-vip_type]，文件(%s)", file.c_str());
      return false;
    }
    core::int32 privilege = 0;
    if(cell_node->Attribute("privilege_type",
          (core::int32 *)&privilege) == false) {
      LOG_ERROR("找不到属性 [cell-privilege_type]，文件(%s)", file.c_str());
      return false;
    }
    int value1 = 0, value2 = 0;
    if(cell_node->Attribute("value1", &value1) == false) {
      LOG_ERROR("找不到属性 [cell-value1]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("value2", &value2) == false) {
      LOG_ERROR("找不到属性 [cell-value2]，文件(%s)", file.c_str());
      return false;
    }

    Key key((entity::VipType::type)vip_type, (entity::VipPrivilegeType::type)privilege);
    VipMap::iterator iterator = this->privileges_.find(key);
    if(iterator != this->privileges_.end()) {
      if(iterator->second.cells_.find(value1) == iterator->second.cells_.end()) {
        VipCell cell;
        cell.value1_ = value1;
        cell.value2_ = value2;
        iterator->second.cells_.insert(std::make_pair(value1, cell));
      } else {
        LOG_ERROR("VIP配置(%d,%d,%d) 重复", vip_type, privilege, value1);
        return false;
      }
    } else {
      VipCells cells;
      VipCell cell;
      cell.value1_ = value1;
      cell.value2_ = value2;
      cells.cells_.insert(std::make_pair(value1, cell));
      this->privileges_.insert(std::make_pair(key, cells));
    }

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const VipCell *LoadVipConfigure::GetCell(entity::VipType::type vip_type,
    entity::VipPrivilegeType::type privilege) const {
  VipMap::const_iterator iterator = this->privileges_.find(Key(vip_type, privilege));
  if(iterator != this->privileges_.end()) {
    return iterator->second.GetCell();
  }

  return NULL;
}

const VipCell *LoadVipConfigure::GetCell(entity::VipType::type vip_type,
    entity::VipPrivilegeType::type privilege, core::int32 key) const {
  VipMap::const_iterator iterator = this->privileges_.find(Key(vip_type, privilege));
  if(iterator != this->privileges_.end()) {
    return iterator->second.GetCell(key);
  }

  return NULL;
}

}  // namespace global

}  // namespace configure
