#include "game_server/server/extension/item/load_item_compose_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "entity/item_types.h"
#include "global/logging.h"
#include "game_server/server/extension/item/configure.h"

namespace game {

namespace server {

namespace item {

LoadItemComposeConfigure::LoadItemComposeConfigure() {}
LoadItemComposeConfigure::~LoadItemComposeConfigure() {}

static bool AddConsume(ItemComposeRecipeCell *cell,
    core::uint32 item_template_id, core::int32 item_count)
{
    if (0 == item_template_id) {
      return true;
    }

    const ItemCell *item_cell = Configure::GetInstance()->GetItem(item_template_id);
    if (NULL == item_cell) {
      return false;
    }

    if (item_cell->type_ == entity::ItemType::EQUIPMENT) {
      cell->consume_equipments_.push_back(item_template_id);
    } else {
      if (item_count <= 0) {
        return true;
      }
      cell->consume_items_.insert(std::make_pair(item_template_id, item_count));
    }

    return true;
}

bool LoadItemComposeConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
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
    ItemComposeRecipeCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }

    // 成品ID
    {
      if (cell_node->Attribute("result_item_id", (int *)&cell.result_item_id_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[result_item_id] not found.", file.c_str());
        return false;
      }
      if (Configure::GetInstance()->GetItem(cell.result_item_id_) == NULL) {
        LOG_ERROR("Config file (%s) /data/cell[result_item_id] is invalid.", file.c_str());
        return false;
      }
    }

    // 消耗材料
    {
      core::uint32 item_template_id = 0;
      core::int32 item_count = 0;

      if (cell_node->Attribute("material1_item_id", (int *)&item_template_id) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material1_item_id] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("material1_item_count", (int *)&item_count) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material1_item_count] not found.", file.c_str());
        return false;
      }
      if (AddConsume(&cell, item_template_id, item_count) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material1_item_id] is invalid.", file.c_str());
        return false;
      }

      if (cell_node->Attribute("material2_item_id", (int *)&item_template_id) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material2_item_id] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("material2_item_count", (int *)&item_count) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material2_item_count] not found.", file.c_str());
        return false;
      }
      if (AddConsume(&cell, item_template_id, item_count) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material2_item_id] is invalid.", file.c_str());
        return false;
      }

      if (cell_node->Attribute("material3_item_id", (int *)&item_template_id) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material3_item_id] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("material3_item_count", (int *)&item_count) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material3_item_count] not found.", file.c_str());
        return false;
      }
      if (AddConsume(&cell, item_template_id, item_count) == false) {
        LOG_ERROR("Config file (%s) /data/cell[material3_item_id] is invalid.", file.c_str());
        return false;
      }
    }

    // 资源ID
    {
      core::int32 resource_id;
      if (cell_node->Attribute("resource_id", (int *)&resource_id) == false) {
        LOG_ERROR("Config file (%s) /data/cell[resource_id] not found.", file.c_str());
        return false;
      }
      if (resource_id != 0 &&
          (resource_id <= entity::ResourceID::MIN ||
           resource_id >= entity::ResourceID::MAX)) {
        LOG_ERROR("Config file (%s) /data/cell[resource_id] is invalid.", file.c_str());
        return false;
      }
      cell.resource_id_ = resource_id;
    }

    if (cell_node->Attribute("resource_number", (int *)&cell.resource_number_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[resource_number] not found.", file.c_str());
      return false;
    }

    // 绑定规则
    {
      core::int32 bind;
      if (cell_node->Attribute("bind", (int *)&bind) == false) {
        LOG_ERROR("Config file (%s) /data/cell[bind] not found.", file.c_str());
        return false;
      }
      if (bind < ItemComposeRecipeCell::BindType::MIN ||
          bind >= ItemComposeRecipeCell::BindType::MAX) {
        LOG_ERROR("Config file (%s) /data/cell[bind] is invalid.", file.c_str());
        return false;
      }
      cell.bind_ = (ItemComposeRecipeCell::BindType::type)bind;
    }

    this->recipes_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const ItemComposeRecipeCell *LoadItemComposeConfigure::GetItemComposeRecipe(
    core::int32 id) const {
  ItemComposeRecipeHashmap::const_iterator iter = this->recipes_.find(id);
  if (iter == this->recipes_.end()) {
    return NULL;
  }
  return &iter->second;
}

}  // namespace item

}  // namespace server

}  // namespace game

