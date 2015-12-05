//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 11:17:44.
// File name: item.cc
//
// Description:
// Define class Item.
//

#include "game_server/server/extension/item/item.h"

#include "entity/item_types.h"
#include "game_server/server/extension/item/item_actor.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

Item::Item() : actor_(NULL), template_id_(0),
  container_type_(entity::ItemContainerType::MAX),
  location_(0) {}
Item::~Item() {}

void Item::Allocate(core::uint64 id) {
  GameObject::Allocate();
  this->client_attribute_.resize(entity::ItemClientFields::CLIENT_END, 0);
  this->server_attribute_.resize(entity::ItemServerFields::SERVER_END, 0);
  this->SetAttribute(entity::ItemClientFields::STACK_COUNT, 1, false);
  this->actor_ = NULL;
  this->template_id_ = 0;
  this->location_ = 0;
  this->SetID(id);
}

void Item::Deallocate() {
  GameObject::Deallocate();
  // 属性清0
  AttributeFields::iterator iterator_client = this->client_attribute_.begin();
  for(; iterator_client != this->client_attribute_.end(); ++iterator_client) {
    *iterator_client = 0;
  }
  AttributeFields::iterator iterator_server = this->server_attribute_.begin();
  for(; iterator_server != this->server_attribute_.end(); ++iterator_server) {
    *iterator_server = 0;
  }
}

bool Item::Initialize(core::uint32 template_id, ItemActor *actor) {
  GameObject::Initialize();
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 为NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->actor_ = actor;
  this->template_id_ = template_id;
  return true;
}

void Item::Finalize() {
  GameObject::Finalize();
  this->actor_ = NULL;
  this->template_id_ = 0;
}

core::int32 Item::GetAttribute(entity::ItemClientFields::type type) const {
  if(type < entity::ItemClientFields::CLIENT_END) {
    return this->client_attribute_[type];
  }
  return 0;
}

bool Item::SetAttribute(entity::ItemClientFields::type type, core::int32 value,
    bool synchronize) {
  if(type < entity::ItemClientFields::CLIENT_END) {
    this->client_attribute_[type] = value;
    this->ReviseAttribute(type);
    if(synchronize) {
      this->Synchronize(type);
    }
    return true;
  }
  return false;
}

core::int32 Item::GetAttribute(entity::ItemServerFields::type type) const {
  if(type < entity::ItemServerFields::SERVER_END) {
    return this->server_attribute_[type];
  }
  return 0;
}

bool Item::SetAttribute(entity::ItemServerFields::type type, core::int32 value,
    bool synchronize) {
  if(type < entity::ItemServerFields::SERVER_END) {
    this->server_attribute_[type] = value;
    this->ReviseAttribute(type);
    return true;
  }
  return false;
}

void Item::Synchronize(entity::ItemClientFields::type type) {
  if(this->actor_ == NULL) {
    return ;
  }

  entity::ItemClientField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));

  gateway::protocol::MessageSynchronizeItem message;
  message.__set_item_id_(this->GetStringID());
  message.item_client_fields_.push_back(field);
  message.__isset.item_client_fields_ = true;

  this->actor_->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ITEM);
}

void Item::ReviseAttribute(entity::ItemClientFields::type type) {
  static AttributeReviseFunctor functions[entity::ItemClientFields::CLIENT_END] = {
    NonNegativeRevise,          // 绑定类型
    NonNegativeRevise,          // 叠加数量
  };
  // 修正
  if(functions[type]) {
    functions[type](this->client_attribute_[type]);
  }
}

void Item::ReviseAttribute(entity::ItemServerFields::type type) {
  static AttributeReviseFunctor functions[entity::ItemServerFields::SERVER_END] = {
  };
  // 修正
  if(functions[type]) {
    functions[type](this->server_attribute_[type]);
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

