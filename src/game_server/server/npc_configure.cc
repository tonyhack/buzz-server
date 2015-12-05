//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-19 10:11:23.
// File name: npc_configure.cc
//
// Description:
// Define class NpcConfigure.
//

#include "game_server/server/npc_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "entity/vocation_types.h"
#include "entity/npc_types.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

NpcConfigure::NpcConfigure() {}
NpcConfigure::~NpcConfigure() {}

bool NpcConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 标签
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  core::uint32 id = 0;

  // 加载 cell 结点标签
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    NpcCell cell;
    if(cell_node->Attribute("id", (int *)&id) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(this->npcs_.find(id) != this->npcs_.end()) {
      global::LogError("%s:%d (%s) id(%d)配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }
    if(cell_node->Attribute("type", (int *)(&cell.type_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    const char *str = cell_node->Attribute("name");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-name]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    cell.name_ = str;
    if(cell_node->Attribute("level", (int *)(&cell.level_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-level]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("vocation", (int *)(&cell.vocation_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-hp]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell.vocation_ < entity::VocationType::MIN || cell.vocation_ >= entity::VocationType::MAX) {
      LOG_ERROR("NPC(%d) 配置vocation(%u) 错误", id, cell.vocation_);
      return false;
    }
    if(cell_node->Attribute("hp", (int *)(&cell.hp_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-hp]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("mp", (int *)(&cell.mp_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-mp]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("move_speed", (int *)(&cell.move_speed_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-move_speed]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("phy_att", (int *)(&cell.phy_attrack_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-phy_att]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("phy_def", (int *)(&cell.phy_defence_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-phy_def]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("magic_att", (int *)(&cell.magic_attrack_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-magic_att]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("magic_def", (int *)(&cell.magic_defence_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-magic_def]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("dodge", (int *)(&cell.dodge_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-dodge]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("crit", (int *)(&cell.crit_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-crit]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("ignore_defence", (int *)(&cell.ignore_defence_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-ignore_defence]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("ai", (int *)(&cell.ai_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-ai]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("attack_distance", (int *)(&cell.attack_distance_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-attack_distance]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("disappear_time", (int *)(&cell.disappear_time_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-disappear_time]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("reborn", (int *)(&cell.reborn_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reborn]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    core::int32 mob_type = 0;
    if(cell_node->Attribute("mob_type", (int *)(&mob_type)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-mob_type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    cell.mob_type_ = (entity::NpcMobType::type)mob_type;
    if(cell_node->Attribute("bar_id", (int *)(&cell.bar_id_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-bar_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("camp_id", (int *)(&cell.camp_id_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-camp_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("drop_id", (int *)(&cell.drop_id_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-drop_id]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("exp_type", (int *)(&cell.exp_type_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-exp_type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("exp", (int *)(&cell.exp_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-exp]，文件(%s)", file.c_str());
      return false;
    }
    int owner_type = 0;
    if(cell_node->Attribute("owner_type", &owner_type) == NULL) {
      LOG_ERROR("找不到属性 [cell-owner_type]，文件(%s)", file.c_str());
      return false;
    }
    if(owner_type < entity::NpcOwnerType::MIN || owner_type >= entity::NpcOwnerType::MAX) {
      LOG_ERROR("属性 [cell-owner_type] 配置错误，文件(%s)", file.c_str());
      return false;
    }
    cell.owner_type_ = (entity::NpcOwnerType::type)owner_type;

    // skills
    str = cell_node->Attribute("skills");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-skills]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 分割
    std::vector<core::int32> vectors;
    core::int32 temp_value = 0;
    global::StringSplit(str, "+" , vectors);
    std::vector<core::int32>::iterator iterator = vectors.begin();
    for(; iterator != vectors.end(); ++iterator) {
      temp_value = (core::int32)(*iterator);
      if(temp_value < 0) {
        global::LogError("%s:%d (%s) npc(%d)配置 读取 skills 属性错误",
            __FILE__, __LINE__, __FUNCTION__, id);
        return false;
      }
      if(temp_value == 0)
        continue;
      cell.skills_.push_back(temp_value);
    }

    this->npcs_.insert(std::make_pair(id, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const NpcCell *NpcConfigure::GetNpc(core::uint32 template_id) const {
  const NpcHashmap::const_iterator iterator = this->npcs_.find(template_id);
  if(iterator != this->npcs_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace server

}  // namespace game

