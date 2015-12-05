//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-17 17:24:52.
// File name: game_npc.cc
//
// Description:
// Define class GameNpc.
//

#include "game_server/server/game_npc.h"

#include "game_server/server/configure.h"
#include "game_server/server/npc_configure.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"

namespace game {

namespace server {

GameNpc::GameNpc() : template_id_(0), need_reborn_(true) {}
GameNpc::~GameNpc() {}

void GameNpc::Allocate(core::uint64 id) {
  GameRole::Allocate();
  this->aoi_attribute_.resize(entity::NpcAoiFields::AOI_END, 0);
  this->client_attribute_.resize(entity::NpcClientFields::CLIENT_END, 0);
  this->template_id_ = 0;
  this->SetID(id);
}

void GameNpc::Deallocate() {
  GameRole::Deallocate();
  this->aoi_attribute_.assign(this->aoi_attribute_.size(), 0);
  this->client_attribute_.assign(this->client_attribute_.size(), 0);
}

bool GameNpc::Initialize(core::uint32 template_id, entity::DirectionType::type dir, 
    bool need_reborn) {
  GameRole::Initialize();
  this->template_id_ = template_id;
  this->need_reborn_ = need_reborn;

  this->SetBornPosition(0, 0);

  if(this->load_progress_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 LoadProgress 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 配置信息
  const NpcCell *cell = Configure::GetInstance()->GetNpcConfigure().GetNpc(template_id);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 配置中找不到 npc(%u) ",
        __FILE__, __LINE__, __FUNCTION__, template_id);
    return false;
  }

  this->npc_type_ = (entity::NpcKind::type)cell->type_;

  // 属性初始化
  this->SetAttribute(entity::RoleAoiFields::VOCATION, cell->vocation_, false);
  this->SetAttribute(entity::RoleAoiFields::LEVEL, cell->level_, false);
  this->SetAttribute(entity::RoleAoiFields::DIRECTION, dir, false);
  this->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE, cell->hp_, false);
  this->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE, cell->mp_, false);
  this->SetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE, cell->move_speed_, false);
  this->SetAttribute(entity::RoleAoiFields::CAMP, cell->camp_id_, false);

  this->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE, cell->phy_attrack_, false);
  this->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE, cell->phy_defence_, false);

  this->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE, cell->magic_attrack_, false);
  this->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE, cell->magic_defence_, false);

  this->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE, cell->dodge_, false);
  this->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE, cell->crit_, false);
  this->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE, cell->ignore_defence_, false);

  // 最后设置当前血量值、魔法值和状态
  this->SetAttribute(entity::RoleAoiFields::CURRENT_HP,
      this->GetAttribute(entity::RoleAoiFields::MAX_HP), false);
  this->SetAttribute(entity::RoleAoiFields::CURRENT_MP,
      this->GetAttribute(entity::RoleAoiFields::MAX_MP), false);
  this->SetBasicStatus(entity::RoleBasicStatus::STATUS_COMMON, false);

  // 设置道具扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::ITEM);
  // 设置GM扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::GM);
  // 设置英灵扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::SOUL);
  // 设置任务扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::TASK);
  // 设置属性扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::ATTR);
  // 设置聊天扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::CHAT);
  // 设置邮件扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::MAIL);
  // 设置社交扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::SOCIAL);
  // 设置组队扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::TEAM);
  // 设置公会扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::GUILD);
  // 设置副本扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::PLAYING);
  // 设置进度条扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::BAR);
  // 设置拍卖扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::AUCTION);
  // 设置福利扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::WELFARE);
  // 设置运送扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::TRANSPORT);

  return true;
}

void GameNpc::Finalize() {
  GameRole::Finalize();
  this->load_progress_.Finalize();
}

core::int32 GameNpc::GetAttribute(entity::NpcAoiFields::type type) const {
  if(type < entity::NpcAoiFields::AOI_END) {
    return this->aoi_attribute_[type];
  }
  return 0;
}

bool GameNpc::SetAttribute(entity::NpcAoiFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::NpcAoiFields::AOI_END) {
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

core::int32 GameNpc::GetAttribute(entity::NpcClientFields::type type) const {
  if(type < entity::NpcClientFields::CLIENT_END) {
    return this->client_attribute_[type];
  }
  return 0;
}

bool GameNpc::SetAttribute(entity::NpcClientFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::NpcClientFields::CLIENT_END) {
    // 设置属性
    this->client_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    return true;
  }
  return false;
}

void GameNpc::Synchronize() {}

void GameNpc::Synchronize(entity::NpcAoiFields::type type) {
  // Get field.
  entity::NpcAoiField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeNpc message;
  message.__set_npc_id_(this->GetStringID());
  message.npc_aoi_fields_.push_back(field);
  message.__isset.npc_aoi_fields_ = true;
  // 发送到周围
  this->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_NPC);
}

void GameNpc::Synchronize(entity::RoleAoiFields::type type) {
  // Get field.
  entity::RoleAoiField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeNpc message;
  message.__set_npc_id_(this->GetStringID());
  message.role_aoi_fields_.push_back(field);
  message.__isset.role_aoi_fields_ = true;
  // 发送到周围
  this->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_NPC);
}

void GameNpc::Synchronize(entity::RoleClientFields::type type) {}

void GameNpc::Synchronize(entity::EntityClientFields::type type) {}

void GameNpc::ReviseAttribute(entity::NpcAoiFields::type type) {
  static AttributeReviseFunctor functions[entity::NpcAoiFields::AOI_END] = {
  };
  // 修正
  if(functions[type]) {
    functions[type](this->aoi_attribute_[type]);
  }
}

void GameNpc::ReviseAttribute(entity::NpcClientFields::type type) {
  static AttributeReviseFunctor functions[entity::NpcClientFields::CLIENT_END] = {
  };
  // 修正
  if(functions[type]) {
    functions[type](this->client_attribute_[type]);
  }
}

}  // namespace server

}  // namespace game

