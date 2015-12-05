//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-12 14:57:37.
// File name: scene_item.cc
//
// Description:
// Define class SceneItem.
//

#include "game_server/server/extension/scene/scene_item.h"

#include "game_server/server/extension/scene/scene.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "game_server/server/extension/scene/scene_item_manager.h"
#include "game_server/server/extension/scene/scene_item_pool.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/configure/configure.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace scene {

SceneItem::SceneItem() {}
SceneItem::~SceneItem() {}

void SceneItem::Allocate(core::uint64 id) {
  GameEntity::Allocate();
  this->aoi_attribute_.resize(entity::SceneItemAoiFields::AOI_END, 0);
  this->SetID(id);
}

void SceneItem::Deallocate() {
  GameEntity::Deallocate();
  this->aoi_attribute_.assign(this->aoi_attribute_.size(), 0);
}

bool SceneItem::Initialize(core::uint32 template_id) {
  GameEntity::Initialize();

  this->scene_ = NULL;

  this->SetAttribute(entity::SceneItemAoiFields::TEMPLATE, template_id, false);

  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_DISAPPEAR,
      MISC_CONF()->drop_item_disappear_secs_ * 1000, 1, this, "SceneItem::Initialize");

  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_CLEAR_OWNER,
      MISC_CONF()->drop_item_clear_owner_secs_ * 1000, 1, this, "SceneItem::Initialize");

  return true;
}

void SceneItem::Finalize() {
  GameEntity::Finalize();
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_DISAPPEAR, this);
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_CLEAR_OWNER, this);
  this->scene_ = NULL;
  this->owners_.clear();
}

core::int32 SceneItem::GetAttribute(entity::SceneItemAoiFields::type type) const {
  if(type < entity::SceneItemAoiFields::AOI_END) {
    return this->aoi_attribute_[type];
  }
  return 0;
}

bool SceneItem::SetAttribute(entity::SceneItemAoiFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::SceneItemAoiFields::AOI_END) {
    // 设置属性
    this->aoi_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    if(synchronize) {
      this->Synchronize(type);
    }
    return true;
  }
  return false;
}

void SceneItem::AddOwner(core::uint64 actor, bool synchronize) {
  this->owners_.push_back(actor);

  gateway::protocol::MessageSynchronizeSceneItem message;
  message.__isset.owner_actors_ = true;
  message.__set_owner_type_(this->GetOwnerType());

  OwnerVector::const_iterator iterator = this->owners_.begin();
  for(; iterator != this->owners_.end(); ++iterator) {
    message.owner_actors_.push_back(global::ToString(*iterator));
  }

  // 发送到周围
  if(synchronize) {
    this->SendMessageAround(message,
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_SCENE_ITEM);
  }
}

void SceneItem::ClearOwner(bool synchronize) {
  this->owners_.clear();

  gateway::protocol::MessageSynchronizeSceneItem message;
  message.__isset.owner_actors_ = true;
  message.__set_owner_type_(this->GetOwnerType());

  // 发送到周围
  if(synchronize) {
    this->SendMessageAround(message,
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_SCENE_ITEM);
  }
}

bool SceneItem::CheckOwner(core::uint64 actor) const {
  if(this->owners_.size() == 0) {
    return true;
  }
  if(this->GetOwnerType() == entity::NpcOwnerType::ALL) {
    return true;
  }
  else if(this->GetOwnerType() == entity::NpcOwnerType::KILL || this->GetOwnerType() ==
      entity::NpcOwnerType::DAMAGE || this->GetOwnerType() == entity::NpcOwnerType::CAMP_EACH) {
    OwnerVector::const_iterator iterator = this->owners_.begin();
    for(; iterator != this->owners_.end(); ++iterator) {
      if(*iterator == actor) {
        return true;
      }
    }
  } else if(this->GetOwnerType() == entity::NpcOwnerType::CAMP_COMPETE) {
    // 获取玩家阵营
    GameActor *game_actor = GameActorManager::GetInstance()->GetActor(actor);
    if(game_actor == NULL) {
      LOG_ERROR("获取玩家(%lu)失败", actor);
      return false;
    }
    if((core::int32)this->owners_[0] == game_actor->GetAttribute(entity::RoleAoiFields::CAMP)) {
      return true;
    }
    return false;
  }

  return false;
}

void SceneItem::Synchronize() {}

void SceneItem::Synchronize(entity::SceneItemAoiFields::type type) {
  entity::SceneItemAoiField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeSceneItem message;
  message.__set_id_(this->GetStringID());
  message.scene_item_aoi_fields_.push_back(field);
  message.__isset.scene_item_aoi_fields_ = true;
  message.__set_owner_type_(this->GetOwnerType());

  // 发送到周围
  this->SendMessageAround(message,
      gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_SCENE_ITEM);
}

void SceneItem::Synchronize(entity::EntityClientFields::type type) {}

void SceneItem::OnTimer(core::uint32 id) {
  switch(id) {
    case TIMER_ID_DISAPPEAR:
      this->OnTimerDisappear();
      break;
    case TIMER_ID_CLEAR_OWNER:
      this->OnTimerClearOwner();
      break;
    default:
      break;
  }
}

void SceneItem::OnTimerDisappear() {
  if(this->scene_->RemoveEntity(this) == false) {
    LOG_ERROR("场景移除道具失败");
    return;
  }

  SceneItemManager::GetInstance()->Remove(this->GetID());
  this->Finalize();
  SceneItemPool::GetInstance()->Deallocate(this);
}

void SceneItem::OnTimerClearOwner() {
  this->ClearOwner();
}

void SceneItem::ReviseAttribute(entity::SceneItemAoiFields::type type) {
  static AttributeReviseFunctor functions[entity::SceneItemAoiFields::AOI_END] = {
    NULL,                       // 类型
    NULL,                       // 模板ID
    NULL,                       // 绑定
    NULL,                       // 数量
  };
  // 修正
  if(functions[type]) {
    functions[type](this->aoi_attribute_[type]);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

