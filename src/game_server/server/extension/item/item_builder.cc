//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 15:42:31.
// File name: item_builder.cc
//
// Description:
// Define class ItemBuilder.
//

#include "game_server/server/extension/item/item_builder.h"

#include "entity/constants_constants.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_pool.h"
#include "game_server/server/extension/item/material.h"
#include "game_server/server/extension/item/medicine.h"

namespace game {

namespace server {

namespace item {

Item *ItemBuilder::Create(ItemActor *actor, core::uint32 template_id) {
  const ItemCell *cell =
    Configure::GetInstance()->GetItem(template_id);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 创建 道具对象失败，道具ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, template_id);
    return NULL;
  }

  return this->Create(actor, (entity::ItemType::type)cell->type_, template_id);
}

Item *ItemBuilder::Create(ItemActor *actor, entity::ItemType::type type,
    core::uint32 template_id) {
  switch(type) {
    case entity::ItemType::EQUIPMENT:
      return this->CreateEquipment(actor, template_id);
    case entity::ItemType::MEDICINE:
      return this->CreateMedicine(actor, template_id);
    case entity::ItemType::CARD:
      break;
    case entity::ItemType::MATERIAL:
      return this->CreateMaterial(actor, template_id);
    case entity::ItemType::TASK:
      break;
    default:
      break;
  }

  return NULL;
}

Item *ItemBuilder::Create(ItemActor *actor,
    const database::protocol::StorageItemField &field) {
  const ItemCell *cell =
    Configure::GetInstance()->GetItem(field.template_id_);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 创建 Equipment对象失败，道具ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, field.template_id_);
    return NULL;
  }
  switch(cell->type_) {
    case entity::ItemType::EQUIPMENT:
      return this->CreateEquipment(actor, field);
    case entity::ItemType::MEDICINE:
      return this->CreateMedicine(actor, field);
    case entity::ItemType::CARD:
      break;
    case entity::ItemType::MATERIAL:
      return this->CreateMaterial(actor, field);
    case entity::ItemType::TASK:
      break;
    default:
      break;
  }

  return NULL;
}

Material *ItemBuilder::CreateMaterial(ItemActor *actor, core::uint32 template_id) {
  Material *material = (Material *)ItemPool::GetInstance()->Allocate(
      entity::ItemType::MATERIAL);
  if(material == NULL) {
    global::LogError("%s:%d (%s) 分配 Material对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  if(material->Initialize(template_id, actor) == false) {
    global::LogError("%s:%d (%s) 初始化 Material对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    ItemPool::GetInstance()->Deallocate(material);
    return NULL;
  }
  return material;
}

Medicine *ItemBuilder::CreateMedicine(ItemActor *actor, core::uint32 template_id) {
  Medicine *medicine = (Medicine *)ItemPool::GetInstance()->Allocate(
      entity::ItemType::MEDICINE);
  if(medicine == NULL) {
    global::LogError("%s:%d (%s) 分配 Medicine对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  if(medicine->Initialize(template_id, actor) == false) {
    global::LogError("%s:%d (%s) 初始化 Medicine对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    ItemPool::GetInstance()->Deallocate(medicine);
    return NULL;
  }
  return (Medicine *)medicine;
}

void ItemBuilder::Destory(Item *item) {
  if(item) {
    item->Finalize();
    switch(item->GetItemType()) {
      case entity::ItemType::EQUIPMENT:
        ((Equipment *)item)->Deallocate();
        break;
      case entity::ItemType::MEDICINE:
        ((Medicine *)item)->Deallocate();
      case entity::ItemType::CARD:
        break;
      case entity::ItemType::MATERIAL:
        ((Material *)item)->Deallocate();
      case entity::ItemType::TASK:
        break;
      default:
        break;
    }
    ItemPool::GetInstance()->Deallocate(item);
  }
}

Equipment *ItemBuilder::CreateEquipment(ItemActor *actor, core::uint32 template_id) {
  const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(template_id);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 创建 Equipment对象失败，道具ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, template_id);
    return NULL;
  }

  // 分配装备对象
  Equipment *equipment = (Equipment *)ItemPool::GetInstance()->Allocate(
      entity::ItemType::EQUIPMENT);
  if(equipment == NULL) {
    global::LogError("%s:%d (%s) 分配 Equipment对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(equipment->Initialize(template_id, actor) == false) {
    global::LogError("%s:%d (%s) 初始化 Equipment对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    ItemPool::GetInstance()->Deallocate(equipment);
    return NULL;
  }

  // 是否有随机属性
  if(cell->random_attr_number_ <= 0) {
    return equipment;
  }

  // 随机属性
  for(core::uint32 pos = 0; pos < cell->random_attr_number_ &&
      pos < (core::uint32)entity::g_constants_constants.kMaxRandomAttriNum; ++pos) {
    const RandomAttributeCell *random_cell = Configure::GetInstance()->GetRandomAttribute(
        cell->random_attr_id_);
    if(random_cell == NULL) {
      global::LogError("%s:%d (%s) 道具ID(%u) 随机属性ID(%u) 随机失败",
          __FILE__, __LINE__, __FUNCTION__, template_id, cell->random_attr_id_);
      continue;
    }
    equipment->SetAttribute((entity::EquipClientFields::type)(
          entity::EquipClientFields::RANDOM_ATTR_1 + pos * 2),
        random_cell->type_, false);
    equipment->SetAttribute((entity::EquipClientFields::type)(
          entity::EquipClientFields::RANDOM_VALUE_1 + pos * 2),
        random_cell->value_, false);
    global::LogDebug("道具ID(%u) 获得随机属性(%u, %u)",
        equipment->GetTemplateID(), random_cell->type_, random_cell->value_);
  }

  return equipment;
}

Material *ItemBuilder::CreateMaterial(ItemActor *actor,
    const database::protocol::StorageItemField &field) {
  Material *material = CreateMaterial(actor, field.template_id_);
  if(material) {
    material->InitializeDatabase(field);
  }
  return material;
}

Medicine *ItemBuilder::CreateMedicine(ItemActor *actor,
    const database::protocol::StorageItemField &field) {
  Medicine *medicine = CreateMedicine(actor, field.template_id_);
  if(medicine) {
    medicine->InitializeDatabase(field);
  }
  return medicine;
}

Equipment *ItemBuilder::CreateEquipment(ItemActor *actor,
    const database::protocol::StorageItemField &field) {
  const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(
      field.template_id_);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 创建 Equipment对象失败，道具ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, field.template_id_);
    return NULL;
  }

  // 分配装备对象
  Equipment *equipment = (Equipment *)ItemPool::GetInstance()->Allocate(
      entity::ItemType::EQUIPMENT);
  if(equipment == NULL) {
    global::LogError("%s:%d (%s) 分配 Equipment对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(equipment->Initialize(field.template_id_, actor) == false) {
    global::LogError("%s:%d (%s) 初始化 Equipment对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    ItemPool::GetInstance()->Deallocate(equipment);
    return NULL;
  }

  equipment->InitializeDatabase(field);

  return equipment;
}

}  // namespace item

}  // namespace server

}  // namespace game

