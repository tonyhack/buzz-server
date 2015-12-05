//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-06 10:35:22.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/skill/configure.h"

#include <vector>

#include "core/tinyxml/tinyxml.h"
#include "entity/camp_types.h"
#include "entity/skill_types.h"
#include "game_server/server/extension/skill/skill_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace skill {

Configure::Configure() : role_pool_initial_(0), role_pool_extend_(0),
    skill_pool_initial_(0), skill_pool_extend_(0) {}

Configure::~Configure() {}

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }


  const char *str = NULL;
  const std::string &configure_dir =
    global::ServerConfigureSingleton::GetInstance().GetConfigueDir();


  // 加载 role_pool 结点
  TiXmlElement *role_pool_node = configure_node->FirstChildElement("role_pool");
  if(role_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-role_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(role_pool_node->Attribute("initial_number",
        (int *)(&this->role_pool_initial_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [role_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(role_pool_node->Attribute("extend_number",
        (int *)(&this->role_pool_extend_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [role_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 skill_pool 结点
  TiXmlElement *skill_pool_node = configure_node->FirstChildElement("skill_pool");
  if(skill_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-skill_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(skill_pool_node->Attribute("initial_number",
        (int *)(&this->skill_pool_initial_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [skill_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(skill_pool_node->Attribute("extend_number",
        (int *)(&this->skill_pool_extend_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [skill_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }


  // 加载 skill 结点
  TiXmlElement *skill_node = configure_node->FirstChildElement("skill");
  if(skill_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[skill]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = skill_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[skill-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  // 加载 skill 配置文件
  if(this->LoadSkillConfigure(configure_dir + str) == false) {
    global::LogError("%s:%d (%s) 加载 skill 配置失败",
            __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 hurt 结点
  TiXmlElement *hurt_node = configure_node->FirstChildElement("hurt");
  if(hurt_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[hurt]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = hurt_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[hurt-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  // 加载 hurt 配置文件
  if(this->LoadHurtConfigure(configure_dir + str) == false) {
    global::LogError("%s:%d (%s) 加载 hurt 配置失败",
            __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

bool Configure::LoadSkillConfigure(const std::string &file) {
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
    SkillCell cell;

    // id
    if(cell_node->Attribute("id", (int *)(&cell.id_)) == NULL) {
      global::LogError("%s:%d (%s) 技能配置 读取属性 id 错误",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }

    // level
    if(cell_node->Attribute("level", (int *)(&cell.level_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 level 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    SkillHashmap::iterator iterator = this->skills_.find(
        SkillKey(cell.id_, cell.level_));
    if(iterator != this->skills_.end()) {
      global::LogError("%s:%d (%s) 技能配置 id(%u) 重复",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // group
    if(cell_node->Attribute("group", (int *)(&cell.group_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 group 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // cooling
    if(cell_node->Attribute("cooling", (int *)(&cell.cooling_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 cooling 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // type
    if(cell_node->Attribute("type", (int *)(&cell.type_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // classify_type
    if(cell_node->Attribute("classify_type", (int *)(&cell.classify_type_)) == NULL) {
      LOG_ERROR("技能(%u)配置 读取属性 classify_type 错误");
      return false;
    }

    // hit_type
    str = cell_node->Attribute("hit_type");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 hit_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    std::vector<core::int32> hit_types;
    global::StringSplit(str, "+", hit_types);
    cell.hit_type_ = 0;
    for(std::vector<core::int32>::iterator iterator = hit_types.begin();
        iterator != hit_types.end(); ++iterator) {
      core::int32 value = *iterator;
      if(value >= 32) {
        LOG_ERROR("技能(%u) 配置属性 hit_type 错误", cell.id_);
        return false;
      }
      cell.hit_type_ |= value > 1 ? 1 << (value-1) : 1;
    }

    // target_type
    if(cell_node->Attribute("target_type", (int *)(&cell.target_type_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 target_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // dest_type 多选
    str = cell_node->Attribute("dest_type");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 dest_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    // 分割
    std::vector<core::int32> dest_types;
    global::StringSplit(str, "+", dest_types);
    cell.dest_type_ = 0;
    std::vector<core::int32>::iterator iterator_d = dest_types.begin();
    for(; iterator_d != dest_types.end(); ++iterator_d) {
      core::int32 value = *iterator_d;
      if(value >= 32) {
        global::LogError("%s:%d (%s) 技能(%u)配置 属性 dest_type 错误",
            __FILE__, __LINE__, __FUNCTION__, cell.id_);
        return false;
      }
      cell.dest_type_ |= value > 1 ? 1 << (value-1) : 1;
    }

    // range_type
    if(cell_node->Attribute("range_type", (int *)(&cell.range_type_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 range_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // status_type 多选
    str = cell_node->Attribute("status_type");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 status_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    // 分割
    std::vector<core::int32> status_types;
    global::StringSplit(str, "+", status_types);
    cell.status_type_ = 0;
    std::vector<core::int32>::iterator iterator_s = status_types.begin();
    for(; iterator_s != status_types.end(); ++iterator_s) {
      core::int32 value = *iterator_s;
      if(value >= 32) {
        global::LogError("%s:%d (%s) 技能(%u)配置 属性 status_type 错误",
            __FILE__, __LINE__, __FUNCTION__, cell.id_);
        return false;
      }
      cell.status_type_ |= value > 1 ? 1 << (value-1) : 1;
    }

    // camp_type 多选
    str = cell_node->Attribute("camp_type");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 camp_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    // 分割
    std::vector<core::int32> camp_types;
    global::StringSplit(str, "+", camp_types);
    cell.camp_type_ = 0;
    std::vector<core::int32>::iterator iterator_c = camp_types.begin();
    for(; iterator_c != camp_types.end(); ++iterator_c) {
      core::int32 value = *iterator_c;
      if(value >= entity::CampRelationType::MAX ||
          value <= entity::CampRelationType::MIN) {
        global::LogError("%s:%d (%s) 技能(%u)配置 属性 camp_type 错误",
            __FILE__, __LINE__, __FUNCTION__, cell.id_);
        return false;
      }
      cell.camp_type_ |= value > 1 ? 1 << (value-1) : 1;
    }

    // cast_type
    core::int32 cast_val = 0;
    if(cell_node->Attribute("cast_type", (int *)(&cast_val)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 cast_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    if(cast_val == 0) {
      cell.cast_type_ = AffectType::TEAM | AffectType::OTHER_TEAM
        | AffectType::SELF | AffectType::OTHER_SELF;
    } else {
      cell.cast_type_ = 1 << (cast_val - 1);
    }

    // master_type 多选
    str = cell_node->Attribute("master_type");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 master_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    // 分割
    std::vector<core::int32> master_types;
    global::StringSplit(str, "+", master_types);
    cell.master_type_ = 0;
    std::vector<core::int32>::iterator iterator_m = master_types.begin();
    for(; iterator_m != master_types.end(); ++iterator_m) {
      core::int32 value = *iterator_m;
      if(value >= 32) {
        global::LogError("%s:%d (%s) 技能(%u)配置 属性 master_type 错误",
            __FILE__, __LINE__, __FUNCTION__, cell.id_);
        return false;
      }
      cell.master_type_ |= value > 1 ? 1 << (value-1) : 1;
    }

    // friend_type
    if(cell_node->Attribute("friend_type", (int *)(&cell.friend_type_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 friend_type 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // aoe_max_target
    if(cell_node->Attribute("aoe_max_target", (int *)(&cell.aoe_max_target_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 aoe_max_target 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // aoe_radius
    if(cell_node->Attribute("aoe_radius", (int *)(&cell.aoe_radius_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 aoe_radius 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // sing_time
    if(cell_node->Attribute("sing_time", (int *)(&cell.sing_time_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 sing_time 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // distance
    if(cell_node->Attribute("distance", (int *)(&cell.distance_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 distance 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // sing_distance
    if(cell_node->Attribute("sing_distance", (int *)(&cell.sing_distance_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 sing_distance 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // speed
    if(cell_node->Attribute("speed", (int *)(&cell.speed_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 speed 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // spend_hp
    if(cell_node->Attribute("spend_hp", (int *)(&cell.spend_hp_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 spend_hp 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // spend_hp_per
    if(cell_node->Attribute("spend_hp_per", (int *)(&cell.spend_hp_per_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 spend_hp_per 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // spend_mp
    if(cell_node->Attribute("spend_mp", (int *)(&cell.spend_mp_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 spend_mp 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // spend_mp_per
    if(cell_node->Attribute("spend_mp_per", (int *)(&cell.spend_mp_per_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 spend_mp_per 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // spend_item
    if(cell_node->Attribute("spend_item", (int *)(&cell.spend_item_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 spend_item 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // spend_item_num
    if(cell_node->Attribute("spend_item_num", (int *)(&cell.spend_item_num_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 spend_item_num 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // cond_war_status
    if(cell_node->Attribute("cond_war_status", (int *)(&cell.cond_war_status_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 cond_war_status 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // soul_status
    if(cell_node->Attribute("soul_status", (int *)(&cell.soul_status_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 soul_status 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // buff_status
    if(cell_node->Attribute("buff_status", (int *)(&cell.buff_status_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 buff_status 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // cond_day_night
    if(cell_node->Attribute("cond_day_night", (int *)(&cell.cond_day_night_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 cond_day_night 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // hurt_id
    if(cell_node->Attribute("hurt_id", (int *)(&cell.hurt_id_)) == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 hurt_id 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }

    // buff
    str = cell_node->Attribute("buffs");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 buff 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    // 分割
    std::vector<std::string> vectors;
    global::StringSplit(str, "+", vectors);
    std::vector<std::string>::iterator iterator_b = vectors.begin();
    for(; iterator_b != vectors.end(); ++iterator_b) {
      std::vector<core::int32> buffs;
      global::StringSplit(iterator_b->c_str(), ":", buffs);
      if(buffs.size() != 2) {
        global::LogError("%s:%d (%s) 技能(%u)配置 属性 buff 格式错误",
            __FILE__, __LINE__, __FUNCTION__, cell.id_);
        return false;
      }
      cell.buffs_.insert(std::make_pair(buffs[0], buffs[1]));
    }

    str = cell_node->Attribute("sit_break_buffs");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 技能(%u)配置 读取属性 sit_break_buffs 错误",
          __FILE__, __LINE__, __FUNCTION__, cell.id_);
      return false;
    }
    global::StringSplit(str, "+", cell.sit_break_buffs_);

    // 移动类型
    core::int32 move = 0;
    if(cell_node->Attribute("move", (int *)(&move)) == NULL) {
      LOG_ERROR("技能(%u)配置 读取属性 move 错误", cell.id_);
      return false;
    }
    cell.move_ = (SkillMoveType::type)move;

    core::int32 extra_aoe_condition = 0;
    if (cell_node->Attribute("extra_aoe_condition", (int *)&extra_aoe_condition) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_condition 错误", cell.id_);
      return false;
    }
    cell.extra_aoe_condition_ = (ExtraAoeConditionType::type)extra_aoe_condition;

    if (cell_node->Attribute("extra_aoe_dest_type", (int *)&cell.extra_aoe_dest_type_) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_dest_type 错误", cell.id_);
      return false;
    }

    if (cell_node->Attribute("extra_aoe_max_target", (int *)&cell.extra_aoe_max_target_) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_max_target 错误", cell.id_);
      return false;
    }

    if (cell_node->Attribute("extra_aoe_radius", (int *)&cell.extra_aoe_radius_) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_radius 错误", cell.id_);
      return false;
    }

    core::int32 extra_aoe_cast_type = 0;
    if (cell_node->Attribute("extra_aoe_cast_type", (int *)&extra_aoe_cast_type) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_cast_type 错误", cell.id_);
      return false;
    }
    if(cast_val == 0) {
      cell.extra_aoe_cast_type_ = AffectType::TEAM | AffectType::OTHER_TEAM
        | AffectType::SELF | AffectType::OTHER_SELF;
    } else {
      cell.extra_aoe_cast_type_ = 1 << (extra_aoe_cast_type - 1);
    }

    if (cell_node->Attribute("extra_aoe_camp_type", (int *)&cell.extra_aoe_camp_type_) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_camp_type 错误", cell.id_);
      return false;
    }

    if (cell_node->Attribute("extra_aoe_hurt_id", (int *)&cell.extra_aoe_hurt_id_) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_hurt_id 错误", cell.id_);
      return false;
    }

    if (cell_node->Attribute("extra_aoe_friend_type", (int *)&cell.extra_aoe_friend_type_) == NULL) {
      LOG_ERROR("技能(%s) 配置 extra_aoe_friend_type 错误", cell.id_);
      return false;
    }

    // 插入表中
    this->skills_.insert(std::make_pair(SkillKey(
            cell.id_, cell.level_), cell));

    // 下个节点
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool Configure::LoadHurtConfigure(const std::string &file) {
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
    HurtCell cell;
    int id = 0;

    // id
    if(cell_node->Attribute("id", &id) == NULL) {
      global::LogError("%s:%d (%s) 技能伤害配置 读取属性 id 错误",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(this->hurts_.find(id) != this->hurts_.end()) {
      global::LogError("%s:%d (%s) 技能伤害配置 属性 id(%d) 重复",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }

    // type
    if(cell_node->Attribute("type", (int *)(&cell.type_)) == NULL) {
      global::LogError("%s:%d (%s) 技能伤害(%u)配置 读取属性 type 错误",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }
    if(cell.type_ <= entity::SkillHurtType::MIN || cell.type_ >= entity::SkillHurtType::MAX) {
      global::LogError("%s:%d (%s) 技能伤害(%u)配置 属性 type(%u) 不存在",
          __FILE__, __LINE__, __FUNCTION__, id, cell.type_);
      return false;
    }

    // param1
    if(cell_node->Attribute("param1", (int *)(&cell.param1_)) == NULL) {
      global::LogError("%s:%d (%s) 技能伤害(%u)配置 读取属性 param1 错误",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }

    // param2
    if(cell_node->Attribute("param2", (int *)(&cell.param2_)) == NULL) {
      global::LogError("%s:%d (%s) 技能伤害(%u)配置 读取属性 param2 错误",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }

    // threat_value
    if(cell_node->Attribute("threat_value", (int *)(&cell.threat_value_)) == NULL) {
      global::LogError("%s:%d (%s) 技能伤害(%u)配置 读取属性 threat_value 错误",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }

    // threat_coe
    if(cell_node->Attribute("threat_coe", (int *)(&cell.threat_coe_)) == NULL) {
      global::LogError("%s:%d (%s) 技能伤害(%u)配置 读取属性 threat_coe 错误",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }

    this->hurts_.insert(std::make_pair(id, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const SkillCell *Configure::GetSkill(core::uint32 id, core::uint16 level) {
  SkillHashmap::iterator iterator = this->skills_.find(SkillKey(id, level));
  if(iterator != this->skills_.end()) {
    return &iterator->second;
  }
  return NULL;
}

const HurtCell *Configure::GetHurt(core::uint32 id) {
  HurtHashmap::iterator iterator = this->hurts_.find(id);
  if(iterator != this->hurts_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace skill

}  // namespace server

}  // namespace game

