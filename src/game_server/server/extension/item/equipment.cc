//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-27 19:52:08.
// File name: equipment.cc
//
// Description:
// Define class Equipment.
//

#include "game_server/server/extension/item/equipment.h"

#include <cstdlib>

#include "entity/equipment_types.h"
#include "entity/role_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/event/game_item_event_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

Equipment::Equipment() {}
Equipment::~Equipment() {}

void Equipment::Allocate(core::uint64 id) {
  Item::Allocate(id);
  this->client_attribute_.resize(entity::EquipClientFields::CLIENT_END, 0);
  this->server_attribute_.resize(entity::EquipServerFields::SERVER_END, 0);
  this->first_equip_time_ = 0;
  this->expire_ = false;
}

void Equipment::Deallocate() {
  Item::Deallocate();
  // 属性清0
  AttributeFields::iterator iterator_client = this->client_attribute_.begin();
  for(; iterator_client != this->client_attribute_.end(); ++iterator_client) {
    *iterator_client = 0;
  }
  AttributeFields::iterator iterator_server = this->server_attribute_.begin();
  for(; iterator_server != this->server_attribute_.end(); ++iterator_server) {
    *iterator_server = 0;
  }
  this->first_equip_time_ = 0;
  this->expire_ = false;
}

bool Equipment::Initialize(core::uint32 template_id, ItemActor *actor) {
  return Item::Initialize(template_id, actor);
}

void Equipment::Finalize() {
  Item::Finalize();
}

void Equipment::InitializeDatabase(
    const database::protocol::StorageItemField &field) {
  this->SetAttribute(entity::ItemClientFields::BIND_TYPE, field.bind_, false);
  this->SetAttribute(entity::ItemClientFields::STACK_COUNT, field.number_, false);
  this->SetAttribute(entity::EquipClientFields::INTENSIFY, field.intensify_level_, false);
  this->SetAttribute(entity::EquipClientFields::MAX_INTENSIFY, field.max_intensify_level_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_1, field.random_attr1_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_2, field.random_attr2_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_3, field.random_attr3_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_4, field.random_attr4_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_5, field.random_attr5_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_6, field.random_attr6_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_1, field.random_attr_value1_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_2, field.random_attr_value2_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_3, field.random_attr_value3_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_4, field.random_attr_value4_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_5, field.random_attr_value5_, false);
  this->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_6, field.random_attr_value6_, false);
  this->SetAttribute(entity::EquipClientFields::UPGRADE_LUCKY, field.upgrade_lucky_, false);
  this->first_equip_time_ = field.first_use_time_;
}

void Equipment::ExportDatabaseField(database::protocol::StorageItemField &field) {
  field.__set_location_(this->GetLocation());
  field.__set_template_id_(this->GetTemplateID());
  field.__set_number_(1);
  field.__set_bind_(this->GetAttribute(entity::ItemClientFields::BIND_TYPE));
  // 装备特有
  field.__set_intensify_level_(this->GetAttribute(entity::EquipClientFields::INTENSIFY));
  field.__set_max_intensify_level_(this->GetAttribute(entity::EquipClientFields::MAX_INTENSIFY));
  field.__set_random_attr1_(this->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1));
  field.__set_random_attr2_(this->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_2));
  field.__set_random_attr3_(this->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_3));
  field.__set_random_attr4_(this->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_4));
  field.__set_random_attr5_(this->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_5));
  field.__set_random_attr6_(this->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_6));
  field.__set_random_attr_value1_(this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_1));
  field.__set_random_attr_value2_(this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_2));
  field.__set_random_attr_value3_(this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_3));
  field.__set_random_attr_value4_(this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_4));
  field.__set_random_attr_value5_(this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_5));
  field.__set_random_attr_value6_(this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_6));
  field.__set_upgrade_lucky_(this->GetAttribute(entity::EquipClientFields::UPGRADE_LUCKY));
  field.__set_first_use_time_(this->first_equip_time_);
}

void Equipment::ExportInitialField(entity::ItemFields &field) const {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu", this->first_equip_time_);
  field.__set_first_equip_time_(buffer);
  field.__set_id_(this->GetStringID());
  field.__set_template_id_(this->GetTemplateID());
  field.__set_type_(this->GetItemType());
  field.__set_location_(this->GetLocation());
  field.__set_item_client_fields_(this->GetItemClientAttributes());
  field.__set_equipment_client_fields_(this->GetEquipClientAttributes());
}

void Equipment::ImportExtraInfo(const std::string &input) {
  std::vector<std::string> split_result;
  global::StringSplit(input.c_str(), ",", split_result);

  for(int i = 0; i < (int)split_result.size(); ++i) {
    if(split_result[i].empty()) {
      continue;
    }

    if(i >= entity::EquipClientFields::CLIENT_BEGIN &&
       i < entity::EquipClientFields::CLIENT_END) {
      this->SetAttribute((entity::EquipClientFields::type)i,
          atoi(split_result[i].c_str()), false);
    }
  }
}

void Equipment::ExportExtraInfo(std::string &output) const {
  char buffer[512] = "";
  size_t count = 0;

  for(int i = entity::EquipClientFields::CLIENT_BEGIN;
       i < entity::EquipClientFields::CLIENT_END; ++i) {
    count += snprintf(buffer + count, sizeof(buffer) - count, 
             "%d,", this->GetAttribute((entity::EquipClientFields::type)i));
  }
  // 去掉最后的逗号
  if (count > 0) {
    buffer[count - 1] = '\0';
  }

  output = buffer;
}

entity::EquipmentType::type Equipment::GetEquipType() const {
  const EquipmentCell *cell =
    Configure::GetInstance()->GetEquipmentConfigure(this->GetTemplateID());
  if(cell == NULL) {
    return entity::EquipmentType::MAX;
  }
  return (entity::EquipmentType::type)cell->sub_type_;
}

core::int32 Equipment::GetAttribute(entity::EquipClientFields::type type) const {
  if(type < entity::EquipClientFields::CLIENT_END) {
    return this->client_attribute_[type];
  }
  return 0;
}

bool Equipment::SetAttribute(entity::EquipClientFields::type type, core::int32 value,
    bool synchronize) {
  if(type < entity::EquipClientFields::CLIENT_END) {
    this->client_attribute_[type] = value;
    this->ReviseAttribute(type);
    if(synchronize) {
      this->Synchronize(type);
    }
    return true;
  }
  return false;
}

core::int32 Equipment::GetAttribute(entity::EquipServerFields::type type) const {
  if(type < entity::EquipServerFields::SERVER_END) {
    return this->server_attribute_[type];
  }
  return 0;
}

bool Equipment::SetAttribute(entity::EquipServerFields::type type, core::int32 value,
    bool synchronize) {
  if(type < entity::EquipServerFields::SERVER_END) {
    this->server_attribute_[type] = value;
    this->ReviseAttribute(type);
    return true;
  }
  return false;
}

void Equipment::Synchronize() {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 同步创建时 actor_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 发送属性到客户端
  /*
  gateway::protocol::MessageCreateEquipment message;
  message.__set_id_(this->GetStringID());
  message.__set_template_id_(this->GetTemplateID());
  message.__set_item_client_fields_(this->GetItemClientAttributes());
  message.__set_equipment_client_fields_(this->GetEquipClientAttributes());

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu", this->first_equip_time_);
  message.first_equip_time_ = buffer;

  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_CREATE_EQUIPMENT);
  */


  // 发送属性同步到客户端
  gateway::protocol::MessageItemCreateSynchronize message;
  message.__set_id_(this->GetStringID());
  message.__set_template_id_(this->GetTemplateID());
  message.__set_type_(entity::ItemType::EQUIPMENT);
  message.__set_item_client_fields_(this->GetItemClientAttributes());
  message.__set_equipment_client_fields_(this->GetEquipClientAttributes());

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu", this->first_equip_time_);
  message.first_equip_time_ = buffer;

  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ITEM_CREATE_SYNCHRONIZE);
}

void Equipment::Synchronize(entity::EquipClientFields::type type) {
  if(this->actor_ == NULL) {
    return ;
  }

  entity::EquipClientField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));

  gateway::protocol::MessageSynchronizeItem message;
  message.__set_item_id_(this->GetStringID());
  message.equip_client_fields_.push_back(field);
  message.__isset.equip_client_fields_ = true;

  this->actor_->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ITEM);
}

bool Equipment::ExpireCheck() {
  if(this->expire_ == false) {
    const EquipmentCell *cell =
      Configure::GetInstance()->GetEquipmentConfigure(this->GetTemplateID());
    if(cell == NULL) {
      return false;
    }

    time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
    if((cell->expire_use_ <= 0 || this->first_equip_time_ == 0 ||
          this->first_equip_time_ + cell->expire_use_ > now) &&
        (cell->expire_absolute_time_ <= 0 || cell->expire_absolute_time_ > now)) {
      return false;
    }

    this->expire_ = true;
  }

  return this->expire_;
}

void Equipment::ReviseAttribute(entity::EquipClientFields::type type) {
  static AttributeReviseFunctor functions[entity::EquipClientFields::CLIENT_END] = {
    NULL,                       // 强化等级
    NULL,                       // 历史最高强化等级
    NonNegativeRevise,          // 随机属性1
    NonNegativeRevise,          // 随机属性值1
    NonNegativeRevise,          // 随机属性2
    NonNegativeRevise,          // 随机属性值2
    NonNegativeRevise,          // 随机属性3
    NonNegativeRevise,          // 随机属性值3
    NonNegativeRevise,          // 随机属性4
    NonNegativeRevise,          // 随机属性值4
    NonNegativeRevise,          // 随机属性5
    NonNegativeRevise,          // 随机属性值5
    NonNegativeRevise,          // 随机属性6
    NonNegativeRevise,          // 随机属性值6
  };
  // 修正
  if(functions[type]) {
    functions[type](this->client_attribute_[type]);
  }
}

void Equipment::ReviseAttribute(entity::EquipServerFields::type type) {
  static AttributeReviseFunctor functions[entity::EquipServerFields::SERVER_END] = {
  };
  // 修正
  if(functions[type]) {
    functions[type](this->server_attribute_[type]);
  }
}

void Equipment::UpdateBroadcast(entity::EquipmentType::type location, bool equip) {
  if(this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return ;
  }

  gateway::protocol::MessageUpdateActorEquipBroadcast message;
  message.__set_actor_id_(this->actor_->GetStringID());
  message.equip_.__set_location_(location);
  if(equip) {
    message.equip_.__set_template_id_(this->GetTemplateID());
    message.equip_.__set_intensify_(
        this->GetAttribute(entity::EquipClientFields::INTENSIFY));
  } else {
    message.equip_.__set_template_id_(0);
    message.equip_.__set_intensify_(0);
  }
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_UPDATE_ACTOR_EQUIP_BROADCAST);
}

bool Equipment::OnLoadEquip(entity::EquipmentType::type location, bool synchronize) {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 穿装备时 actor_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(
      this->GetTemplateID());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 穿装备时找不到装备模板ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, this->GetTemplateID());
    return false;
  }

  if(cell->sub_type_ != (core::uint16)location) {
    global::LogError("%s:%d (%s) 穿装备时位置与类型不合适",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 主属性
  this->StartMainEffect(cell);

  // 副属性
  this->StartSecondEffect(cell);

  // 强化属性
  this->StartIntensify(cell);

  // 随机属性
  this->StartRandEffect(cell);

  // 首次使用时间
  if(this->first_equip_time_ == 0) {
    this->first_equip_time_ = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  }

  if(synchronize) {
    global::LogDebug("同步玩家(%lu) 穿装备(%u)到周围", this->actor_->GetID(),
        this->GetTemplateID());
    this->UpdateBroadcast(location, true);
    if(this->actor_ != NULL) {
      // 玩家穿装备事件
      event::EventItemWearEquip event;
      event.__set_id_(this->actor_->GetID());
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_ITEM_WEAR_EQUIP, this->actor_->GetID(),
          event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
    }   
  }
  return true;
}

bool Equipment::UnloadEquip(entity::EquipmentType::type location, bool synchronize) {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 穿装备时 actor_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  const EquipmentCell *cell = Configure::GetInstance()->GetEquipmentConfigure(
      this->GetTemplateID());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 穿装备时找不到装备模板ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, this->GetTemplateID());
    return false;
  }

  // 主属性
  this->StopMainEffect(cell);

  // 副属性
  this->StopSecondEffect(cell);

  // 强化属性
  this->StopIntensify(cell);

  // 随机属性
  this->StopRandEffect(cell);

  if(synchronize) {
    global::LogDebug("同步玩家(%lu) 脱装备(%u)到周围", this->actor_->GetID(),
        this->GetTemplateID());
    this->UpdateBroadcast(location, false);
  }

  return true;
}

void Equipment::StartMainEffect(const EquipmentCell *cell) {
  // 强化属性
  core::uint8 intensify = this->GetAttribute(entity::EquipClientFields::INTENSIFY);
 
  // 公式 = 主属性值 * (1 + (0.12*强化等级)^2 + 0.06*强化等级)
  // 主属性
  EquipmentCell::AttrVector::const_iterator iterator = cell->attrs_.begin();
  for(; iterator != cell->attrs_.end(); ++iterator) {
    float square = Configure::GetInstance()->kIntensifyVar1_ * intensify;
    square *= square;
    core::int32 value = (core::int32)(iterator->second * (1.0f + square +
          Configure::GetInstance()->kIntensifyVar2_ * intensify) + 0.5f);
    this->StartEffect((entity::EquipAttributeType::type)iterator->first, value);
  }
}

void Equipment::StopMainEffect(const EquipmentCell *cell) {
  // 强化属性
  core::uint8 intensify = this->GetAttribute(entity::EquipClientFields::INTENSIFY);
 
  // 公式 = 主属性值 * (1 + (0.12*强化等级)^2 + 0.06*强化等级)
  // 主属性
  EquipmentCell::AttrVector::const_iterator iterator = cell->attrs_.begin();
  for(; iterator != cell->attrs_.end(); ++iterator) {
    float square = Configure::GetInstance()->kIntensifyVar1_ * intensify;
    square *= square;
    core::int32 value = (core::int32)(iterator->second * (1.0f + square +
          Configure::GetInstance()->kIntensifyVar2_ * intensify) + 0.5f);
    this->StopEffect((entity::EquipAttributeType::type)iterator->first, value);
  }
}

void Equipment::StartSecondEffect(const EquipmentCell *cell) {
  EquipmentCell::AttrVector::const_iterator iterator = cell->second_attrs_.begin();
  for(; iterator != cell->second_attrs_.end(); ++iterator) {
    this->StartEffect((entity::EquipAttributeType::type)iterator->first,
        iterator->second);
  }
}

void Equipment::StopSecondEffect(const EquipmentCell *cell) {
  EquipmentCell::AttrVector::const_iterator iterator = cell->second_attrs_.begin();
  for(; iterator != cell->second_attrs_.end(); ++iterator) {
    this->StopEffect((entity::EquipAttributeType::type)iterator->first,
        iterator->second);
  }
}

void Equipment::StartIntensify(const EquipmentCell *cell) {
  // 强化属性
  core::uint8 intensify = this->GetAttribute(entity::EquipClientFields::INTENSIFY);

  if(intensify >= 5) {
    EquipmentCell::AttrVector::const_iterator iterator = cell->intensify_five_.begin();
    for(; iterator != cell->intensify_five_.end(); ++iterator) {
      this->StartEffect((entity::EquipAttributeType::type)iterator->first,
          iterator->second);
    }
    if(intensify >= 10) {
      iterator = cell->intensify_ten_.begin();
      for(; iterator != cell->intensify_ten_.end(); ++iterator) {
        this->StartEffect((entity::EquipAttributeType::type)iterator->first,
            iterator->second);
      }
    }
  }
}

void Equipment::StopIntensify(const EquipmentCell *cell) {
  // 强化属性
  core::uint8 intensify = this->GetAttribute(entity::EquipClientFields::INTENSIFY);

  if(intensify >= 5) {
    EquipmentCell::AttrVector::const_iterator iterator = cell->intensify_five_.begin();
    for(; iterator != cell->intensify_five_.end(); ++iterator) {
      this->StopEffect((entity::EquipAttributeType::type)iterator->first,
          iterator->second);
    }
    if(intensify >= 10) {
      iterator = cell->intensify_ten_.begin();
      for(; iterator != cell->intensify_ten_.end(); ++iterator) {
        this->StopEffect((entity::EquipAttributeType::type)iterator->first,
            iterator->second);
      }
    }
  }
}

void Equipment::StartRandEffect(const EquipmentCell *cell) {
  this->StartEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_1),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_1));
  this->StartEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_2),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_2));
  this->StartEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_3),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_3));
  this->StartEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_4),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_4));
  this->StartEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_5),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_5));
  this->StartEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_6),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_6));
}

void Equipment::StopRandEffect(const EquipmentCell *cell) {
  this->StopEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_1),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_1));
  this->StopEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_2),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_2));
  this->StopEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_3),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_3));
  this->StopEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_4),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_4));
  this->StopEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_5),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_5));
  this->StopEffect((entity::EquipAttributeType::type)this->GetAttribute(
        entity::EquipClientFields::RANDOM_ATTR_6),
      this->GetAttribute(entity::EquipClientFields::RANDOM_VALUE_6));
}

void Equipment::StartEffect(entity::EquipAttributeType::type type, core::int32 value) {
  if(this->actor_ == NULL || this->actor_->GetActor() == NULL) {
    return ;
  }
  switch(type) {
    case entity::EquipAttributeType::MAX_HP:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::MAX_MP:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::PHYSIQUE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::STRENGTH:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::INTELLIGENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::AGILE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::SPIRIT:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::PHYSICS_ATTACK:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::PHYSICS_DEFENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::MAGIC_ATTACK:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::MAGIC_DEFENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::DODGE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::CRIT:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::IGNORE_DEFENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) + value,
          true);
      break;
    case entity::EquipAttributeType::MOVE_SPEED:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE) + value,
          true);
      break;
    default:
      break;
  }
}

void Equipment::StopEffect(entity::EquipAttributeType::type type, core::int32 value) {
  if(this->actor_ == NULL || this->actor_->GetActor() == NULL) {
    return ;
  }
  switch(type) {
    case entity::EquipAttributeType::MAX_HP:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::MAX_MP:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::PHYSIQUE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::STRENGTH:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::INTELLIGENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::AGILE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::SPIRIT:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::PHYSICS_ATTACK:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::PHYSICS_DEFENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::MAGIC_ATTACK:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::MAGIC_DEFENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::DODGE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::CRIT:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::IGNORE_DEFENCE:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) - value,
          true);
      break;
    case entity::EquipAttributeType::MOVE_SPEED:
      this->actor_->GetActor()->SetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE,
          this->actor_->GetActor()->GetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE) - value,
          true);
      break;
    default:
      break;
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

