//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-19 15:35:23.
// File name: material.cc
//
// Description:
// Define class Material.
//

#include "game_server/server/extension/item/material.h"

#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_configure.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "game_server/server/game_actor.h"
#include "entity/item_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

Material::Material() {}
Material::~Material() {}

void Material::Allocate(core::uint64 id) {
  Item::Allocate(id);
}

void Material::Deallocate() {
  Item::Deallocate();
}

bool Material::Initialize(core::uint32 template_id, ItemActor *actor) {
  return Item::Initialize(template_id, actor);
}

void Material::Finalize() {
  return Item::Finalize();
}

void Material::InitializeDatabase(const database::protocol::StorageItemField &field) {
  this->SetAttribute(entity::ItemClientFields::BIND_TYPE, field.bind_, false);
  this->SetAttribute(entity::ItemClientFields::STACK_COUNT, field.number_, false);
}

void Material::ExportDatabaseField(database::protocol::StorageItemField &field) {
  field.__set_location_(this->GetLocation());
  field.__set_template_id_(this->GetTemplateID());
  field.__set_number_(this->GetAttribute(entity::ItemClientFields::STACK_COUNT));
  field.__set_bind_(this->GetAttribute(entity::ItemClientFields::BIND_TYPE));
}

void Material::ExportInitialField(entity::ItemFields &field) const {
  field.__set_id_(this->GetStringID());
  field.__set_template_id_(this->GetTemplateID());
  field.__set_type_(this->GetItemType());
  field.__set_location_(this->GetLocation());
  field.__set_item_client_fields_(this->GetItemClientAttributes());
}

void Material::Synchronize() {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 同步创建时 actor_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

/*
  // 发送属性到客户端
  gateway::protocol::MessageCreateMaterial message;
  message.__set_id_(this->GetStringID());
  message.__set_template_id_(this->GetTemplateID());
  message.__set_item_client_fields_(this->GetItemClientAttributes());

  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_CREATE_MATERIAL);
 */

  // 发送属性同步到客户端
  gateway::protocol::MessageItemCreateSynchronize message;
  message.__set_id_(this->GetStringID());
  message.__set_template_id_(this->GetTemplateID());
  message.__set_type_(entity::ItemType::MATERIAL);
  message.__set_item_client_fields_(this->GetItemClientAttributes());
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ITEM_CREATE_SYNCHRONIZE);
}

entity::MaterialType::type Material::GetEquipType() const {
  const MaterialCell *cell =
    Configure::GetInstance()->GetMaterialConfigure(this->GetTemplateID());
  if(cell == NULL) {
    return entity::MaterialType::MAX;
  }
  return (entity::MaterialType::type)cell->sub_type_;
}

}  // namespace item

}  // namespace server

}  // namespace game

