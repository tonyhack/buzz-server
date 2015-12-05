//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-27 10:51:55.
// File name: equipment_container.cc
//
// Description:
// Define EquipmentContainer.
//

#include "game_server/server/extension/item/equipment_container.h"

#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/facade_request.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_builder.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

EquipmentContainer::EquipmentContainer() : load_finish_(false) {}
EquipmentContainer::~EquipmentContainer() {}

bool EquipmentContainer::Initialize(ItemActor *actor, core::uint16 capacity,
    core::uint16 max_capacity, entity::VocationType::type vocation) {
  if(vocation < entity::VocationType::MIN || vocation >= entity::VocationType::MAX) {
    LOG_ERROR("参数 vocation 错误");
    return false;
  }

  if(ItemContainer::Initialize(actor, entity::EquipmentType::MAX,
      entity::EquipmentType::MAX) == false) {
    global::LogError("%s:%d (%s) 初始化 ItemContainer 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->SetLoadFinish(false);

  this->vocation_ = vocation;

  return true;
}

void EquipmentContainer::Finalize() {
  ItemContainer::Finalize();
}

bool EquipmentContainer::InitializeItems(const ItemFieldVector &items) {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 初始化容器道具失败，玩家指针为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  ItemFieldVector::const_iterator iterator = items.begin();
  for(; iterator != items.end(); ++iterator) {
    // 检查是否是本容器的道具
    if(iterator->vocation_ != this->vocation_) {
      continue;
    }

    if(this->CheckLocationEmpty(iterator->location_) == false) {
      global::LogError("%s:%d (%s) 玩家(%lu) 容器(%u,%u,%u)，位置不合法",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID(),
          this->GetType(), iterator->location_, iterator->template_id_);
      return false;
    }

    Item *item = ItemBuilder::GetInstance()->Create(this->actor_, *iterator);
    if(item == NULL) {
      global::LogError("%s:%d (%s) 玩家(%lu) 容器(%u,%u,%u) 道具不存在",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID(),
          this->GetType(), iterator->location_, iterator->template_id_);
      return false;
    }

    // 如果是装备检测下是否过期
    if(item->GetItemType() != entity::ItemType::EQUIPMENT) {
      return false;
    }

    Equipment *equipment = (Equipment *)item;

    equipment->ExpireCheck();

    if(this->AddItem(item, iterator->location_, false, false) == false) {
      global::LogError("%s:%d (%s) 玩家(%lu) 容器(%u,%u,%u)，增加 Item 对象失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID(),
          this->GetType(), iterator->location_, iterator->template_id_);
      item->Finalize();
      ItemBuilder::GetInstance()->Destory(item);
      return false;
    }
  }

  this->Synchronize();

  this->UpdateEquipmentFightScore(false);

  return true;
}

bool EquipmentContainer::CheckAddItem(Item *item, core::uint16 index) const {
  if(this->actor_ == NULL || item == NULL) {
    return false;
  }

  GameActor *actor = this->actor_->GetActor();
  if(actor == NULL) {
    return false;
  }

  if(item->GetItemType() != entity::ItemType::EQUIPMENT) {
    return false;
  }

  const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(item->GetTemplateID());
  if(cell == NULL) {
    return false;
  }

  if((core::uint32)actor->GetAttribute(entity::RoleAoiFields::LEVEL) < cell->level_){
    return false;
  }

  if(((Equipment *)item)->GetEquipType() != index) {
    return false;
  }

  if(index >= this->GetCapacity()) {
    return false;
  }

  if(this->items_[index] != NULL) {
    return false;
  }

  return true;
}

bool EquipmentContainer::AddItem(Item *item, core::uint16 index, bool sync, bool broadcast) {
  if(this->actor_ == NULL) {
    LOG_ERROR("actor_ 为NULL");
    return false;
  }

  if(ItemContainer::AddItem(item, index, sync, broadcast) == true) {
    Equipment *equip = (Equipment *)item;
    if(this->actor_->GetActorVocation() == this->vocation_ &&
        equip->CheckExpire() == false) {
      equip->OnLoadEquip((entity::EquipmentType::type)item->GetLocation(), broadcast);
      this->EquipSuitStand(equip);
    }
    if(equip->GetAttribute(entity::ItemClientFields::BIND_TYPE) == 0) {
      equip->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1);
    }
    return true;
  }
  return false;
}

bool EquipmentContainer::CheckRemoveItem(Item *item) const {
  return ItemContainer::CheckRemoveItem(item);
}

bool EquipmentContainer::RemoveItem(Item *item, bool sync, bool broadcast) {
  if(this->actor_ == NULL) {
    LOG_ERROR("actor_ 为NULL");
    return false;
  }

  core::uint16 location = item->GetLocation();
  if(ItemContainer::RemoveItem(item, sync, broadcast) == true) {
    if(this->actor_->GetActorVocation() == this->vocation_ &&
        ((Equipment *)item)->CheckExpire() == false) {
        this->EquipSuitSit((Equipment *)item);
      ((Equipment *)item)->UnloadEquip(
        (entity::EquipmentType::type)location, broadcast);
    }
    return true;
  }
  return false;
}

void EquipmentContainer::ExpireCheck() {
  Equipment *equipment = NULL;
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    equipment = (Equipment *)*iterator;
    if(equipment && equipment->CheckExpire() == false &&
        equipment->ExpireCheck() == true) {
      this->EquipSuitSit(equipment);
      equipment->UnloadEquip((entity::EquipmentType::type)equipment->GetLocation());
    }
  }
}

void EquipmentContainer::Stand() {
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    Equipment *equipment = (Equipment *)*iterator;
    if(equipment && equipment->CheckExpire() == false) {
      equipment->OnLoadEquip((entity::EquipmentType::type)equipment->GetLocation());
      this->EquipSuitStand(equipment);
    }
  }
}

void EquipmentContainer::Sit() {
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    Equipment *equipment = (Equipment *)*iterator;
    if(equipment && equipment->CheckExpire() == false) {
      this->EquipSuitSit(equipment);
      equipment->UnloadEquip((entity::EquipmentType::type)equipment->GetLocation());
    }
  }
}

static void AddAttributeToCalculator(FightScoreCalculator *calc,
    entity::EquipAttributeType::type type, core::int32 value) {
  switch(type) {
    case entity::EquipAttributeType::PHYSIQUE:
      calc->physique_ += value;
      break;
    case entity::EquipAttributeType::STRENGTH:
      calc->strength_ += value;
      break;
    case entity::EquipAttributeType::INTELLIGENCE:
      calc->intelligence_ += value;
      break;
    case entity::EquipAttributeType::AGILE:
      calc->agile_ += value;
      break;
    case entity::EquipAttributeType::SPIRIT:
      calc->spirit_ += value;
      break;
    case entity::EquipAttributeType::MAX_HP:
      calc->max_hp_ += value;
      break;
    case entity::EquipAttributeType::MAX_MP:
      calc->max_mp_ += value;
      break;
    case entity::EquipAttributeType::PHYSICS_ATTACK:
      calc->physics_attack_ += value;
      break;
    case entity::EquipAttributeType::PHYSICS_DEFENCE:
      calc->physics_defence_ += value;
      break;
    case entity::EquipAttributeType::MAGIC_ATTACK:
      calc->magic_attack_ += value;
      break;
    case entity::EquipAttributeType::MAGIC_DEFENCE:
      calc->magic_defence_ += value;
      break;
    case entity::EquipAttributeType::DODGE:
      calc->dodge_ += value;
      break;
    case entity::EquipAttributeType::CRIT:
      calc->crit_ += value;
      break;
    case entity::EquipAttributeType::IGNORE_DEFENCE:
      calc->ignore_defence_ += value;
      break;
    case entity::EquipAttributeType::MOVE_SPEED:
      calc->move_speed_ += value;
      break;
    default:
      break;
  }
}

static void AddEquipmentAttributeToCalculator(FightScoreCalculator *calc,
    const Equipment *equipment) {
  const EquipmentCell *cell =
    Configure::GetInstance()->GetEquipmentConfigure(equipment->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("找不到装备模板ID(%u)", equipment->GetTemplateID());
    return;
  }
  core::uint8 intensify = equipment->GetAttribute(entity::EquipClientFields::INTENSIFY);

  // 主属性
  // 公式 = 主属性值 * (1 + (0.12*强化等级)^2 + 0.06*强化等级)
  for(EquipmentCell::AttrVector::const_iterator iterator = cell->attrs_.begin();
      iterator != cell->attrs_.end(); ++iterator) {
    float square = Configure::GetInstance()->kIntensifyVar1_ * intensify;
    square *= square;
    core::int32 value = (core::int32)(iterator->second * (1.0f + square +
        Configure::GetInstance()->kIntensifyVar2_ * intensify) + 0.5f);
    AddAttributeToCalculator(calc, (entity::EquipAttributeType::type)iterator->first, value);
  }

  // 副属性
  for(EquipmentCell::AttrVector::const_iterator iterator = cell->second_attrs_.begin();
      iterator != cell->second_attrs_.end(); ++iterator) {
    AddAttributeToCalculator(calc, (entity::EquipAttributeType::type)iterator->first,
        iterator->second);
  }

  // 强化属性
  if(intensify >= 5) {
    for(EquipmentCell::AttrVector::const_iterator iterator = cell->intensify_five_.begin();
        iterator != cell->intensify_five_.end(); ++iterator) {
      AddAttributeToCalculator(calc, (entity::EquipAttributeType::type)iterator->first,
          iterator->second);
    }
    if(intensify >= 10) {
      for(EquipmentCell::AttrVector::const_iterator iterator = cell->intensify_ten_.begin();
          iterator != cell->intensify_ten_.end(); ++iterator) {
        AddAttributeToCalculator(calc, (entity::EquipAttributeType::type)iterator->first,
            iterator->second);
      }
    }
  }

  // 随机属性
  AddAttributeToCalculator(calc,
      (entity::EquipAttributeType::type)equipment->GetAttribute(
          entity::EquipClientFields::RANDOM_ATTR_1),
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_1));
  AddAttributeToCalculator(calc,
      (entity::EquipAttributeType::type)equipment->GetAttribute(
          entity::EquipClientFields::RANDOM_ATTR_2),
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_2));
  AddAttributeToCalculator(calc,
      (entity::EquipAttributeType::type)equipment->GetAttribute(
          entity::EquipClientFields::RANDOM_ATTR_3),
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_3));
  AddAttributeToCalculator(calc,
      (entity::EquipAttributeType::type)equipment->GetAttribute(
          entity::EquipClientFields::RANDOM_ATTR_4),
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_4));
  AddAttributeToCalculator(calc,
      (entity::EquipAttributeType::type)equipment->GetAttribute(
          entity::EquipClientFields::RANDOM_ATTR_5),
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_5));
  AddAttributeToCalculator(calc,
      (entity::EquipAttributeType::type)equipment->GetAttribute(
          entity::EquipClientFields::RANDOM_ATTR_6),
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_6));
}

void EquipmentContainer::UpdateEquipmentFightScore(bool sync) {
  if(this->actor_ == NULL || this->actor_->GetActor() == NULL) {
    LOG_ERROR("Actor is null.");
    return ;
  }

  FightScoreCalculator calc;

  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    Equipment *equipment = (Equipment *)*iterator;
    if(equipment && equipment->CheckExpire() == false) {
      AddEquipmentAttributeToCalculator(&calc, equipment);
    }
  }

  entity::SubFightScoreType::type fs_type;
  if(entity::VocationType::AXE == this->vocation_) {
    fs_type = entity::SubFightScoreType::AXE_EQUIPMENT;
  } else if (entity::VocationType::SHOOT == this->vocation_) {
    fs_type = entity::SubFightScoreType::SHOOT_EQUIPMENT;
  } else if (entity::VocationType::MAGIC == this->vocation_) {
    fs_type = entity::SubFightScoreType::MAGIC_EQUIPMENT;
  } else {
    LOG_ERROR("Invalid vocation.");
    return ;
  }

  this->actor_->GetActor()->SetSubFightScore(fs_type, calc.GetFightScore(), sync);
}

core::int32 EquipmentContainer::OverQualityEquip(core::int32 quality) {
  core::int32 num = 0;
  for(size_t i = 0; i < this->items_.size(); ++i ) {
    if(this->items_[i] != NULL) {
      const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(
          this->items_[i]->GetTemplateID());
      if(cell == NULL) {
        LOG_ERROR("配置错误");
        return num;
      }
      if(cell->quality_ >= (core::uint32)quality) {
        num += 1;
      }
    }
  }
  return num;
}

core::int32 EquipmentContainer::OverStrengthLevelEquip(core::int32 level) {
  core::int32 num = 0;
  for(size_t i = 0; i < this->items_.size(); ++i ) {
    if(this->items_[i] != NULL) {
      Equipment *equip = (Equipment *)this->items_[i];
      if(equip->GetAttribute(entity::EquipClientFields::INTENSIFY) >= level) {
        num += 1;
      }
    }
  }

  return num;
}

void EquipmentContainer::EquipSuitStand(Equipment *item) {
  if(item == NULL) {
    LOG_ERROR("装备对象为空");
    return ;
  }

  // 获取装备所在的套装ID
  const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(
      item->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("装备(%d)配置没有找到", item->GetTemplateID());
    return ;
  }
  if(cell->suit_id_ == 0) {
    return ;
  }

  SuitMap::iterator iter = this->suits_.find(cell->suit_id_);
  if(iter == this->suits_.end()) {
    this->suits_.insert(SuitMap::value_type(cell->suit_id_, 1));
    iter = this->suits_.find(cell->suit_id_);
  } else {
    iter->second += 1;
  }

  // 获取套装配置
  const LoadSuitConfigure::EffectVecs *suits = Configure::GetInstance()->GetSuit(cell->suit_id_);
  if(suits == NULL) {
    return ;
  }

  for(size_t i = 0; i < suits->size(); ++i) {
    if((*suits)[i].suit_num_ > (iter->second-1) && (*suits)[i].suit_num_ <= iter->second) {
      // 加套装效果
      FacadeRequest::GetInstance()->StartEffect(entity::EntityType::TYPE_ACTOR,
          this->actor_->GetID(), (*suits)[i].effect_id_);
      return ;
    }
  }
}

void EquipmentContainer::EquipSuitSit(Equipment *item) {
  if(item == NULL) {
    LOG_ERROR("装备对象为空");
    return ;
  }

  // 获取装备所在的套装ID
  const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(
      item->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("装备(%d)配置没有找到", item->GetTemplateID());
    return ;
  }
  if(cell->suit_id_ == 0) {
    return ;
  }

  SuitMap::iterator iter = this->suits_.find(cell->suit_id_);
  if(iter == this->suits_.end()) {
    LOG_ERROR("此装备(%d)无套装", item->GetTemplateID());
    return ;
  } else {
    iter->second -= 1;
    if(iter->second < 0) {
      LOG_ERROR("套装数据错误");
      return ;
    }
  }

  // 获取套装配置
  const LoadSuitConfigure::EffectVecs *suits = Configure::GetInstance()->GetSuit(cell->suit_id_);
  if(suits == NULL) {
    LOG_ERROR("无此套装(%d)配置", cell->suit_id_);
    return ;
  }

  for(size_t i = 0; i < suits->size(); ++i) {
    if((*suits)[i].suit_num_ <= (iter->second + 1) && (*suits)[i].suit_num_ > iter->second) {
      // 取消套装效果
      FacadeRequest::GetInstance()->StopEffect(entity::EntityType::TYPE_ACTOR,
          this->actor_->GetID(), (*suits)[i].effect_id_);
      return ;
    }
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

