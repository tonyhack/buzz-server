#include "game_server/server/extension/attr/noble_actor.h"

#include <cstddef>
#include <algorithm>

#include "coresh/communicator.h"
#include "entity/inform_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "database_server/protocol/storage_attr_actor_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_server.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/configure.h"

namespace game {

namespace server {

namespace attr {

NobleActor::NobleActor()
  : attr_actor_(NULL), current_level_(0), used_item_count_(0),
    max_hp_add_value_(0), max_mp_add_value_(0), physics_attack_add_value_(0), physics_defence_add_value_(0),
    magic_attack_add_value_(0), magic_defence_add_value_(0), dodge_add_value_(0), crit_add_value_(0),
    ignore_defence_add_value_(0) {}
NobleActor::~NobleActor() {}

bool NobleActor::Initialize(AttrActor *attr_actor, const database::protocol::StorageAttrActorField &db_field) {
  if (NULL == attr_actor) {
    LOG_ERROR("attr_actor is null.");
    return false;
  }

  this->attr_actor_ = attr_actor;
  this->current_level_ = db_field.noble_level_;
  this->used_item_count_ = db_field.noble_used_item_count_;
  this->max_hp_add_value_ = db_field.noble_max_hp_add_value_;
  this->max_mp_add_value_ = db_field.noble_max_mp_add_value_;
  this->physics_attack_add_value_ = db_field.noble_physics_attack_add_value_;
  this->physics_defence_add_value_ = db_field.noble_physics_defence_add_value_;
  this->magic_attack_add_value_ = db_field.noble_magic_attack_add_value_;
  this->magic_defence_add_value_ = db_field.noble_magic_defence_add_value_;
  this->dodge_add_value_ = db_field.noble_dodge_add_value_;
  this->crit_add_value_ = db_field.noble_crit_add_value_;
  this->ignore_defence_add_value_ = db_field.noble_ignore_defence_add_value_;

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  // 未开启功能直接返回
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::NOBLE_MODULE) == false) {
    return true;
  }

  if (this->max_hp_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) + 
        this->max_hp_add_value_);
  }
  if (this->max_mp_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) +
        this->max_mp_add_value_);
  }
  if (this->physics_attack_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) +
        this->physics_attack_add_value_);
  }
  if (this->physics_defence_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) +
        this->physics_defence_add_value_);
  }
  if (this->magic_attack_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) +
        this->magic_attack_add_value_);
  }
  if (this->magic_defence_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) +
        this->magic_defence_add_value_);
  }
  if (this->dodge_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) +
        this->dodge_add_value_);
  }
  if (this->crit_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) +
        this->crit_add_value_);
  }
  if (this->ignore_defence_add_value_ > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) +
        this->ignore_defence_add_value_);
  }

  // 调整溢出值
  Breakthrough(0, false);

  // 更新战斗力
  this->UpdateNobleFightScore(false);

  // 发送初始化数据
  this->SendInitToClient();

  return true;
}

void NobleActor::Finalize() {
  this->attr_actor_ = NULL;
  this->current_level_ = 0;
  this->used_item_count_ = 0;
  this->max_hp_add_value_ = 0;
  this->max_mp_add_value_ = 0;
  this->physics_attack_add_value_ = 0;
  this->physics_defence_add_value_ = 0;
  this->magic_attack_add_value_ = 0;
  this->magic_defence_add_value_ = 0;
  this->dodge_add_value_ = 0;
  this->crit_add_value_ = 0;
  this->ignore_defence_add_value_ = 0;
}

void NobleActor::SendInitToClient() {
  gateway::protocol::MessageAttrNobleInitialize message;
  message.__set_level_(this->current_level_);
  message.__set_used_item_count_(this->used_item_count_);
  message.__set_max_hp_add_value_(this->max_hp_add_value_);
  message.__set_max_mp_add_value_(this->max_mp_add_value_);
  message.__set_physics_attack_add_value_(this->physics_attack_add_value_);
  message.__set_physics_defence_add_value_(this->physics_defence_add_value_);
  message.__set_magic_attack_add_value_(this->magic_attack_add_value_);
  message.__set_magic_defence_add_value_(this->magic_defence_add_value_);
  message.__set_dodge_add_value_(this->dodge_add_value_);
  message.__set_crit_add_value_(this->crit_add_value_);
  message.__set_ignore_defence_add_value_(this->ignore_defence_add_value_);
  this->attr_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_INITIALIZE);
}

void NobleActor::Save(database::protocol::StorageAttrActorField &db_field) {
  db_field.noble_level_ = this->current_level_;
  db_field.noble_used_item_count_ = this->used_item_count_;
  db_field.noble_max_hp_add_value_ = this->max_hp_add_value_;
  db_field.noble_max_mp_add_value_ = this->max_mp_add_value_;
  db_field.noble_physics_attack_add_value_ = this->physics_attack_add_value_;
  db_field.noble_physics_defence_add_value_ = this->physics_defence_add_value_;
  db_field.noble_magic_attack_add_value_ = this->magic_attack_add_value_;
  db_field.noble_magic_defence_add_value_ = this->magic_defence_add_value_;
  db_field.noble_dodge_add_value_ = this->dodge_add_value_;
  db_field.noble_crit_add_value_ = this->crit_add_value_;
  db_field.noble_ignore_defence_add_value_ = this->ignore_defence_add_value_;
}

bool NobleActor::EnableNobleFunc(bool synchronize) {
  if (this->current_level_ != 0) {
    return false;
  }

  const NobleCell *cell = Configure::GetInstance()->GetFirstNoble();
  if (NULL == cell) {
    LOG_ERROR("FirstNobleCell not found in config.");
    return false;
  }
  this->current_level_ = cell->id_;

  if (synchronize) {
    this->SendInitToClient();

    // 世界公告: 激活第一个爵位
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::ATTR_NOBLE_ENABLE_FUNC);
    inform.params_.push_back(this->attr_actor_->GetStringID());
    inform.params_.push_back(this->attr_actor_->GetName());
    inform.params_.push_back(global::ToString(this->current_level_));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

    // 同步场景AOI属性
    if (this->attr_actor_->GetActor() != NULL) {
      gateway::protocol::MessageAttrAoiSynchronize sync;
      gateway::protocol::AttrAoiData data;
      data.__set_actor_id_(this->attr_actor_->GetStringID());
      data.__set_noble_level_(this->current_level_);
      sync.aoi_data_.push_back(data);
      this->attr_actor_->GetActor()->SendMessageAround(sync,
          gateway::protocol::MessageType::MESSAGE_ATTR_AOI_SYNCHRONIZE, true);
    }
  }

  return true;
}

bool NobleActor::CheckPromoteFull() const {
  const NobleCell *cell =
    Configure::GetInstance()->GetNoble(this->current_level_);
  if (NULL == cell) {
    return true;
  }

  if (this->max_hp_add_value_ >= cell->max_hp_add_value_limit_ &&
      this->max_mp_add_value_ >= cell->max_mp_add_value_limit_ &&
      this->physics_attack_add_value_ >= cell->physics_attack_add_value_limit_ &&
      this->physics_defence_add_value_ >= cell->physics_defence_add_value_limit_ &&
      this->magic_attack_add_value_ >= cell->magic_attack_add_value_limit_ &&
      this->magic_defence_add_value_ >= cell->magic_defence_add_value_limit_ &&
      this->dodge_add_value_ >= cell->dodge_add_value_limit_ &&
      this->crit_add_value_ >= cell->crit_add_value_limit_ &&
      this->ignore_defence_add_value_ >= cell->ignore_defence_add_value_limit_)
  {
    return true;
  }

  return false;
}

void NobleActor::Promote(bool synchronize) {
  const NobleCell *cell =
    Configure::GetInstance()->GetNoble(this->current_level_);
  if (NULL == cell) {
    LOG_ERROR("NobleCell(%d) not found in config.", this->current_level_);
    return;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  core::int32 max_hp_add_value = std::min(cell->max_hp_add_value_increment_,
      cell->max_hp_add_value_limit_ - this->max_hp_add_value_);
  core::int32 max_mp_add_value = std::min(cell->max_mp_add_value_increment_,
      cell->max_mp_add_value_limit_ - this->max_mp_add_value_);
  core::int32 physics_attack_add_value = std::min(cell->physics_attack_add_value_increment_,
      cell->physics_attack_add_value_limit_ - this->physics_attack_add_value_);
  core::int32 physics_defence_add_value = std::min(cell->physics_defence_add_value_increment_,
      cell->physics_defence_add_value_limit_ - this->physics_defence_add_value_);
  core::int32 magic_attack_add_value = std::min(cell->magic_attack_add_value_increment_,
      cell->magic_attack_add_value_limit_ - this->magic_attack_add_value_);
  core::int32 magic_defence_add_value = std::min(cell->magic_defence_add_value_increment_,
      cell->magic_defence_add_value_limit_ - this->magic_defence_add_value_);
  core::int32 dodge_add_value = std::min(cell->dodge_add_value_increment_,
      cell->dodge_add_value_limit_ - this->dodge_add_value_);
  core::int32 crit_add_value = std::min(cell->crit_add_value_increment_,
      cell->crit_add_value_limit_ - this->crit_add_value_);
  core::int32 ignore_defence_add_value = std::min(cell->ignore_defence_add_value_increment_,
      cell->ignore_defence_add_value_limit_ - this->ignore_defence_add_value_);


  if (max_hp_add_value > 0) {
    this->max_hp_add_value_ += max_hp_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) + 
        max_hp_add_value, synchronize);
  }
  if (max_mp_add_value > 0) {
    this->max_mp_add_value_ += max_mp_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) +
        max_mp_add_value, synchronize);
  }
  if (physics_attack_add_value > 0) {
    this->physics_attack_add_value_ += physics_attack_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) +
        physics_attack_add_value, synchronize);
  }
  if (physics_defence_add_value > 0) {
    this->physics_defence_add_value_ += physics_defence_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) +
        physics_defence_add_value, synchronize);
  }
  if (magic_attack_add_value > 0) {
    this->magic_attack_add_value_ += magic_attack_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) +
        magic_attack_add_value, synchronize);
  }
  if (magic_defence_add_value > 0) {
    this->magic_defence_add_value_ += magic_defence_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) +
        magic_defence_add_value, synchronize);
  }
  if (dodge_add_value > 0) {
    this->dodge_add_value_ += dodge_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) +
        dodge_add_value, synchronize);
  }
  if (crit_add_value > 0) {
    this->crit_add_value_ += crit_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) +
        crit_add_value, synchronize);
  }
  if (ignore_defence_add_value > 0) {
    this->ignore_defence_add_value_ += ignore_defence_add_value;
    game_actor->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) +
        ignore_defence_add_value, synchronize);
  }

  // 更新战斗力
  this->UpdateNobleFightScore();

  if (synchronize) {
    gateway::protocol::MessageAttrNoblePromoteResponse message;
    message.__set_result_(true);
    message.__set_max_hp_add_value_(this->max_hp_add_value_);
    message.__set_max_mp_add_value_(this->max_mp_add_value_);
    message.__set_physics_attack_add_value_(this->physics_attack_add_value_);
    message.__set_physics_defence_add_value_(this->physics_defence_add_value_);
    message.__set_magic_attack_add_value_(this->magic_attack_add_value_);
    message.__set_magic_defence_add_value_(this->magic_defence_add_value_);
    message.__set_dodge_add_value_(this->dodge_add_value_);
    message.__set_crit_add_value_(this->crit_add_value_);
    message.__set_ignore_defence_add_value_(this->ignore_defence_add_value_);
    this->attr_actor_->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_PROMOTE_RESPONSE);
  }
}

void NobleActor::Breakthrough(core::int32 item_count, bool synchronize) {
  const NobleCell *cell =
    Configure::GetInstance()->GetNoble(this->current_level_);
  if (NULL == cell) {
    LOG_ERROR("NobleCell(%d) not found in config.", this->current_level_);
    return;
  }
  const NobleCell *next_cell =
    Configure::GetInstance()->GetNextNoble(this->current_level_);
  if (NULL == next_cell) {
    return;
  }

  this->used_item_count_ += item_count;
  if (this->used_item_count_ >= cell->spend_item_count_) {
    // 升级事件
    event::EventAttrNobleUpgrade event;
    event.__set_actor_id_(this->attr_actor_->GetID());
    event.__set_old_level_(this->current_level_);

    this->used_item_count_ = 0;
    this->current_level_ = next_cell->id_;

    event.__set_new_level_(this->current_level_);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ATTR_NOBLE_UPGRADE, this->attr_actor_->GetID(), 
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

    if (synchronize) {
      gateway::protocol::MessageInformNotify inform;

      if (Configure::GetInstance()->GetNextNoble(this->current_level_) == NULL) {
        // 世界公告: 激活最后一个爵位
        inform.__set_id_(entity::InformType::ATTR_NOBLE_TOP_LEVEL);
      } else {
        // 世界公告: 成功激活新爵位时
        inform.__set_id_(entity::InformType::ATTR_NOBLE_UPGRADE_LEVEL);
      }
      inform.params_.push_back(this->attr_actor_->GetStringID());
      inform.params_.push_back(this->attr_actor_->GetName());
      inform.params_.push_back(global::ToString(this->current_level_));
      GameServerSingleton::GetInstance().BroadcastMessage(inform,
          gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

      // 同步场景AOI属性
      if (this->attr_actor_->GetActor() != NULL) {
        gateway::protocol::MessageAttrAoiSynchronize sync;
        gateway::protocol::AttrAoiData data;
        data.__set_actor_id_(this->attr_actor_->GetStringID());
        data.__set_noble_level_(this->current_level_);
        sync.aoi_data_.push_back(data);
        this->attr_actor_->GetActor()->SendMessageAround(sync,
            gateway::protocol::MessageType::MESSAGE_ATTR_AOI_SYNCHRONIZE, true);
      }
    }
  }

  if (synchronize) {
    gateway::protocol::MessageAttrNobleBreakthroughResponse message;
    message.__set_result_(true);
    message.__set_level_(this->current_level_);
    message.__set_used_item_count_(this->used_item_count_);
    this->attr_actor_->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_BREAKTHROUGH_RESPONSE);
  }
}

void NobleActor::UpdateNobleFightScore(bool synchronize) {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  FightScoreCalculator calc;
  calc.max_hp_ += this->max_hp_add_value_;
  calc.max_mp_ += this->max_mp_add_value_;
  calc.physics_attack_ += this->physics_attack_add_value_;
  calc.physics_defence_ += this->physics_defence_add_value_;
  calc.magic_attack_ += this->magic_attack_add_value_;
  calc.magic_defence_ += this->magic_defence_add_value_;
  calc.dodge_ += this->dodge_add_value_;
  calc.crit_ += this->crit_add_value_;
  calc.ignore_defence_ += this->ignore_defence_add_value_;

  game_actor->SetSubFightScore(entity::SubFightScoreType::NOBLE,
      calc.GetFightScore(), synchronize);
}

}  // namespace attr

}  // namespace server

}  // namespace game

