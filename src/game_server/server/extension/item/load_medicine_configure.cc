//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 14:13:16.
// File name: load_medicine_configure.cc
//
// Description:
// Define class LoadMedicineConfigure.
//

#include "game_server/server/extension/item/load_medicine_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension/item/item_types.h"
#include "game_server/server/extension/item/medicine.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadMedicineConfigure::LoadMedicineConfigure() {}
LoadMedicineConfigure::~LoadMedicineConfigure() {}

bool LoadMedicineConfigure::LoadConfigure(const std::string &file) {
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

  MedicineCell cell;
  const char *str = NULL;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("id", &cell.template_id_) == false) {
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
    if(cell_node->Attribute("level", (int *)(&cell.level_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-level]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("expire_get", (int *)(&cell.expire_get_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-expire_get]，文件(%s)",
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
    if(cell_node->Attribute("buy_coin", (int *)(&cell.buy_coin_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_coin]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_gold", (int *)(&cell.buy_gold_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_gold]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_lockedgold", (int *)(&cell.buy_lockedgold_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_lockedgold]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_honour", (int *)(&cell.buy_honour_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_honour]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sell_coin", (int *)(&cell.sell_coin_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sell_coin]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sell_drop", (int *)(&cell.sell_drop_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sell_drop]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("quality", (int *)(&cell.quality_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-quality]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("operate_right", (int *)(&cell.operate_right_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-operate_right]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("stack_count", (int *)(&cell.stack_count_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-stack_count]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("use_state", (int *)(&cell.use_state_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-use_state]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("cooling", (int *)(&cell.cooling_id_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-cooling]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sub_type", (int *)(&cell.sub_type_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sub_type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("fighting_status",
          (int *)(&cell.fighting_status_forbid_)) == false) {
      LOG_ERROR("找不到属性 [cell-fighting_status]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("pvp_status",
          (int *)(&cell.pvp_status_forbid_)) == false) {
      LOG_ERROR("找不到属性 [cell-pvp_status]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("expire_use", (int *)(&cell.expire_use_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-expire_use]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("func_type", (int *)(&cell.func_type_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-func_type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell.func_type_ <= UseFuncType::NONE || cell.func_type_ >= UseFuncType::MAX) {
      global::LogError("%s:%d (%s) 配置错误 [cell-func_type], id(%u), 文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, cell.template_id_, file.c_str());
      return false;
    }

    str = cell_node->Attribute("func_params");
    cell.func_params_.clear();
    if(str != NULL) {
      std::vector<std::string> pairs;
      global::StringSplit(str, "+", pairs);
      for(size_t pos = 0; pos < pairs.size(); ++pos) {
        std::vector<core::int32> params;
        MedicineParams data;
        global::StringSplit(pairs[pos].c_str(),":", params);
        if(params.size() != 2) {
          global::LogError("%s:%d (%s) 配置错误 [cell-func_params]，id(%u)，文件(%s)",
              __FILE__, __LINE__, __FUNCTION__, cell.template_id_, file.c_str());
          return false;
        }
        data.param1_ = params[0];
        data.param2_ = params[1];
        cell.func_params_.push_back(data);
      }
    }

    if(cell_node->Attribute("func_python", (int *)(&cell.func_python_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-func_param_str]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->medicines_.insert(std::make_pair(cell.template_id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const MedicineCell *LoadMedicineConfigure::GetMedicine(core::uint32 id) const {
  MedicineHashmap::const_iterator iterator = this->medicines_.find(id);
  if(iterator != this->medicines_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace item

}  // namespace server

}  // namespace game

