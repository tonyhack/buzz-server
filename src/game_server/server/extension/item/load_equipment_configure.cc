//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 11:39:00.
// File name: load_equipment_configure.cc
//
// Description:
// Define class LoadEquipmentConfigure.
//

#include "game_server/server/extension/item/load_equipment_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadEquipmentConfigure::LoadEquipmentConfigure() {}
LoadEquipmentConfigure::~LoadEquipmentConfigure() {}

bool LoadEquipmentConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s)出错",
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

  int id = 0;
  const char *str = NULL;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    EquipmentCell cell;
    cell.cooling_id_ = 0;
    cell.stack_count_ = 1;

    if(cell_node->Attribute("id", &id) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    str = cell_node->Attribute("name");
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
    if(cell_node->Attribute("expire_use", (int *)(&cell.expire_use_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-expire_use]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    str = cell_node->Attribute("expire_absolute_time");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-expire_absolute_time]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(global::CheckEmptyStr(str) == false) {
      cell.expire_absolute_time_ = global::FormatTime(str);
      if(cell.expire_absolute_time_ == -1) {
        global::LogError("%s:%d (%s) 属性 [expire_absolute_time] 格式错误(%s)，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, str, file.c_str());
        return false;
      }
    } else {
      cell.expire_absolute_time_ = -1;
    }
    if(cell_node->Attribute("buy_coin", (int *)(&cell.buy_coin_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_coin]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_gold", (int *)(&cell.buy_gold_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_gold]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_lockedgold", (int *)(&cell.buy_lockedgold_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_lockedgold]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_honour", (int *)(&cell.buy_honour_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_honour]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sell_coin", (int *)(&cell.sell_coin_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sell_coin]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sell_drop", (int *)(&cell.sell_drop_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sell_drop]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("quality", (int *)(&cell.quality_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-quality]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("operate_right", (int *)(&cell.operate_right_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-operate_right]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("use_state", (int *)(&cell.use_state_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-use_state]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sub_type", (int *)(&cell.sub_type_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sub_type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("vocation", (int *)(&cell.vocation_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-vocation]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("gender", (int*)(&cell.gender_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-gender]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("equip_bind", (int*)(&cell.equip_bind_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-equip_bind]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_id", (int*)(&cell.upgrade_id_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_spend_item",
          (int*)(&cell.upgrade_spend_item_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_spend_item]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_spend_item_num",
          (int*)(&cell.upgrade_spend_item_num_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_spend_item_num]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_spend_gold",
          (int*)(&cell.upgrade_spend_gold_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_spend_gold]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_lucky", (int*)(&cell.upgrade_lucky_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_lucky]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_max_lucky", (int*)(&cell.upgrade_max_lucky_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_max_lucky]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_lucky_coe", (int*)(&cell.upgrade_lucky_coe_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_lucky_coe]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_odds", (int *)(&cell.upgrade_odds_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-upgrade_lucky_coe]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("split_id",(int *)(&cell.split_id_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-split_id]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("suit_id", (int *)(&cell.suit_id_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-suit_id], 文件(%s)", file.c_str());
      return false;
    }

    // 主属性
    str = cell_node->Attribute("attr");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-attr]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 进行字符串分割
    std::vector<std::string> pairs;
    global::StringSplit(str, "+", pairs);
    for(size_t pos = 0; pos < pairs.size(); ++pos) {
      std::vector<core::int32> attr;
      global::StringSplit(pairs[pos].c_str(), ":", attr);
      if(attr.size() != 2) {
        global::LogError("%s:%d (%s) 配置错误 [cell-attr]，id(%u)，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
        return false;
      }
      cell.attrs_.push_back(std::make_pair(attr[0], attr[1]));
    }

    // 副属性
    str = cell_node->Attribute("attr_second");
    if(str == NULL) {
      LOG_ERROR("找不到属性 [cell-attr_second]，文件(%s)", file.c_str());
      return false;
    }
    // 进行字符串分割
    global::StringSplit(str, "+", pairs);
    for(size_t pos = 0; pos < pairs.size(); ++pos) {
      std::vector<core::int32> attr;
      global::StringSplit(pairs[pos].c_str(), ":", attr);
      if(attr.size() != 2) {
        global::LogError("%s:%d (%s) 配置错误 [cell-attr]，id(%u)，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
        return false;
      }
      cell.second_attrs_.push_back(std::make_pair(attr[0], attr[1]));
    }

    // 强化5星属性
    str = cell_node->Attribute("intensify_five_attr");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-intensify_five_attr]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 进行字符串分割
    pairs.clear();
    global::StringSplit(str, "+", pairs);
    for(size_t pos = 0; pos < pairs.size(); ++pos) {
      std::vector<core::int32> attr;
      global::StringSplit(pairs[pos].c_str(), ":", attr);
      if(attr.size() != 2) {
        global::LogError("%s:%d (%s) 配置错误 [cell-intensify_five_attr]，id(%u)，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
        return false;
      }
      cell.intensify_five_.push_back(std::make_pair(attr[0], attr[1]));
    }

    // 强化10星属性
    str = cell_node->Attribute("intensify_ten_attr");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-intensify_ten_attr]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 进行字符串分割
    pairs.clear();
    global::StringSplit(str, "+", pairs);
    for(size_t pos = 0; pos < pairs.size(); ++pos) {
      std::vector<core::int32> attr;
      global::StringSplit(pairs[pos].c_str(), ":", attr);
      if(attr.size() != 2) {
        global::LogError("%s:%d (%s) 配置错误 [cell-intensify_ten_attr]，id(%u)，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
        return false;
      }
      cell.intensify_ten_.push_back(std::make_pair(attr[0], attr[1]));
    }

    if(cell_node->Attribute("max_intensify", (int*)(&cell.max_intensify_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-max_intensify]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    str = cell_node->Attribute("random_attr");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-random_attr]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    std::vector<core::int32> attr;
    global::StringSplit(str, "+", attr);
    if(attr.size() == 2) {
      cell.random_attr_id_ = attr[0];
      cell.random_attr_number_ = attr[1];
    } else {
      cell.random_attr_id_ = 0;
      cell.random_attr_number_ = 0;
    }

    this->equipments_.insert(std::make_pair(id, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const EquipmentCell *LoadEquipmentConfigure::GetEquipment(core::uint32 id) const {
  EquipmentHashmap::const_iterator iterator = this->equipments_.find(id);
  if(iterator != this->equipments_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace item

}  // namespace server

}  // namespace game

