#include "game_server/server/extension/attr/spirit_actor.h"

#include <cstddef>
#include <algorithm>

#include "global/logging.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_spirit_facade_save_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/facade_request.h"

namespace game {

namespace server {

namespace attr {

SpiritActor::SpiritActor()
  : load_finish_(false), attr_actor_(NULL),
    level_(0),
    current_facade_type_(0), horse_facade_id_(0), wing_facade_id_(0) {}
SpiritActor::~SpiritActor() {}

bool SpiritActor::Initialize(AttrActor *attr_actor,
    const database::protocol::StorageAttrActorField &db_field,
    const SpiritFacadeFieldVector &facades) {
  // 初始化数据
  this->attr_actor_ = attr_actor;
  this->level_ = db_field.spirit_level_;
  this->lucky_value_ = db_field.spirit_lucky_value_;
  if (db_field.spirit_current_facade_type_ < entity::SpiritFacadeType::MIN ||
      db_field.spirit_current_facade_type_ >= entity::SpiritFacadeType::MAX) {
    LOG_ERROR("db_field.spirit_current_facade_type_=[%d] is invalid",
              db_field.spirit_current_facade_type_);
    return false;
  } else {
    this->current_facade_type_ = db_field.spirit_current_facade_type_;
  }
  this->current_facade_type_ = db_field.spirit_current_facade_type_;
  this->horse_facade_id_ = db_field.spirit_horse_facade_id_;
  this->wing_facade_id_ = db_field.spirit_wing_facade_id_;

  // --精灵外观
  for (size_t i = 0; i < facades.size(); ++i) {
    const database::protocol::StorageSpiritFacadeField &db_facade_field = facades[i];
    SpiritFacade facade(db_facade_field.spirit_facade_id_, db_facade_field.expire_time_);

    this->facades_store_.push_back(facade);
    this->facades_index_[facade.GetID()] = this->facades_store_.size() - 1;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  // 未开启功能返回
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    return true;
  }

  // 设置AOI属性
  game_actor->SetAttribute(entity::ActorAoiFields::CURRENT_SPIRIT_FACADE,
      this->GetCurrentFacadeID());
  // 增加属性值
  if (this->AddSpiritAttr(false) == false) {
    LOG_ERROR("AddSpiritAttr failed.");
    return false;
  }
  // 增加Buff
  this->AddFacadeBuff(this->current_facade_type_);

  // 发送初始化数据
  this->SendInitToClient();

  // 移除失效的外观
  this->RemoveExpiredFacade();

  return true;
}

void SpiritActor::Finalize() {
  this->facades_store_.clear();
  this->facades_index_.clear();
  this->wing_facade_id_ = 0;
  this->horse_facade_id_ = 0;
  this->current_facade_type_ = 0;
  this->level_ = 0;
  this->attr_actor_ = NULL;
  this->load_finish_ = false;
}

void SpiritActor::SendInitToClient() {
  gateway::protocol::MessageAttrSpiritInitialize message;
  message.__set_level_(this->level_);
  message.__set_lucky_value_(this->lucky_value_);
  message.__set_current_facade_type_((entity::SpiritFacadeType::type)this->current_facade_type_);
  message.__set_horse_facade_id_(this->horse_facade_id_);
  message.__set_wing_facade_id_(this->wing_facade_id_);
  // 符文
  this->attr_actor_->GetRuneActor()->InitSynchronize(message);

  for (size_t i = 0; i < this->facades_store_.size(); ++i) {
    gateway::protocol::SpiritFacadeData data;
    SpiritFacade &facade = this->facades_store_[i];

    data.__set_id_(facade.GetID());
    data.__set_expire_time_(facade.GetExpireTime());

    message.facades_.push_back(data);
  }

  this->attr_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_INITIALIZE);
}

void SpiritActor::Save(database::protocol::StorageAttrActorField &db_field) {
  if (NULL == this->attr_actor_) {
    LOG_ERROR("Save SpiritActor failed, attr_actor is null.");
    return;
  }

  db_field.spirit_level_ = this->level_;
  db_field.spirit_lucky_value_ = this->lucky_value_;
  db_field.spirit_current_facade_type_ = this->current_facade_type_;
  db_field.spirit_horse_facade_id_ = this->horse_facade_id_;
  db_field.spirit_wing_facade_id_ = this->wing_facade_id_;

  database::protocol::StorageSpiritFacadeSaveRequest request;
  request.__set_actor_id_(this->attr_actor_->GetID());

  for (size_t i = 0; i < this->facades_store_.size(); ++i) {
    database::protocol::StorageSpiritFacadeField db_facade_field;
    SpiritFacade &facade = this->facades_store_[i];

    if (facade.CheckExist() == false) {
      continue;
    }

    db_facade_field.__set_spirit_facade_id_(facade.GetID());
    db_facade_field.__set_expire_time_(facade.GetExpireTime());

    request.fields_.push_back(db_facade_field);
  }

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_SPIRIT_FACADE_SAVE, 
          this->attr_actor_->GetID()) == false) {
    LOG_ERROR("Send StorageSpiritFacadeSaveRequest to database server failed.");
    return;
  }
}

bool SpiritActor::EnableHorseFacadeFunc(bool synchronize) {
  if (this->level_ != 0) {
    return false;
  }

  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(1);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(1) not found in config.");
    return false;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  // 更新数据
  this->level_ = 1;
  this->current_facade_type_ = entity::SpiritFacadeType::HORSE;
  this->horse_facade_id_ = cell->horse_facade_id_;

  // 设置AOI属性
  game_actor->SetAttribute(entity::ActorAoiFields::CURRENT_SPIRIT_FACADE,
      this->GetCurrentFacadeID());
  // 增加属性值
  if (this->AddSpiritAttr() == false) {
    LOG_ERROR("AddSpiritAttr failed.");
    return false;
  }
  // 增加Buff
  this->AddFacadeBuff(this->current_facade_type_);

  // 发送初始化数据
  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

bool SpiritActor::EnableWingFacadeFunc(bool synchronize) {
  if (0 == this->level_) {
    return false;
  }
  if (this->wing_facade_id_ != 0) {
    return false;
  }

  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return false;
  }

  // 更新数据
  this->wing_facade_id_ = cell->wing_facade_id_;

  // 同步数据
  if (synchronize) {
    gateway::protocol::MessageAttrSpiritSynchronize sync;
    sync.__set_wing_facade_id_(this->wing_facade_id_);
    this->attr_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_SYNCHRONIZE);
  }

  return true;
}

core::int32 SpiritActor::GetCurrentFacadeID() const {
  if (entity::SpiritFacadeType::HORSE == this->current_facade_type_) {
    return this->horse_facade_id_;
  } else {
    return this->wing_facade_id_;
  }
}

void SpiritActor::SetCurrentLevel(core::int32 new_level, bool synchronize) {
  if (new_level == this->level_) {
    return;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    LOG_ERROR("SPIRIT_MODULE is disable.");
    return;
  }

  const SpiritCell *current_cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == current_cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return;
  }

  const SpiritCell *new_cell = Configure::GetInstance()->GetSpirit(new_level);
  if (NULL == new_cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", new_level);
    return;
  }

  // 更新属性
  core::int32 physique_add_value = new_cell->physique_add_value_ -
      current_cell->physique_add_value_;
  core::int32 strength_add_value = new_cell->strength_add_value_ -
      current_cell->strength_add_value_;
  core::int32 intelligence_add_value = new_cell->intelligence_add_value_ -
      current_cell->intelligence_add_value_;
  core::int32 agile_add_value = new_cell->agile_add_value_ -
      current_cell->agile_add_value_;
  core::int32 spirit_add_value = new_cell->spirit_add_value_ -
      current_cell->spirit_add_value_;
  core::int32 move_speed_add_value = new_cell->move_speed_add_value_ -
      current_cell->move_speed_add_value_;

  if (physique_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) + 
        physique_add_value, synchronize);
  }
  if (strength_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) +
        strength_add_value, synchronize);
  }
  if (intelligence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) +
        intelligence_add_value, synchronize);
  }
  if (agile_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) +
        agile_add_value, synchronize);
  }
  if (spirit_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) +
        spirit_add_value, synchronize);
  }
  if (move_speed_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE) +
        move_speed_add_value, synchronize);
  }

  // 更新外观
  if (this->horse_facade_id_ != 0 &&
      this->horse_facade_id_ == current_cell->horse_facade_id_) {
    this->horse_facade_id_ = new_cell->horse_facade_id_;
  }
  if (this->wing_facade_id_ != 0 &&
      this->wing_facade_id_ == current_cell->wing_facade_id_) {
    this->wing_facade_id_ = new_cell->wing_facade_id_;
  }

  // 更新数据
  this->level_ = new_level;
  this->lucky_value_ = 0;

  // 设置AOI属性
  game_actor->SetAttribute(entity::ActorAoiFields::CURRENT_SPIRIT_FACADE,
      this->GetCurrentFacadeID(), synchronize);

  // 更新战斗力
  this->UpdateSpiritFightScore(synchronize);

  // 同步数据
  if (synchronize) {
    gateway::protocol::MessageAttrSpiritSynchronize sync;
    sync.__set_level_(this->level_);
    sync.__set_lucky_value_(this->lucky_value_);
    sync.__set_horse_facade_id_(this->horse_facade_id_);
    sync.__set_wing_facade_id_(this->wing_facade_id_);
    this->attr_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_SYNCHRONIZE);
  }
}

void SpiritActor::SetLuckyValue(core::int32 lucky_value, bool synchronize)
{
  this->lucky_value_ = lucky_value;

  if (synchronize) {
    gateway::protocol::MessageAttrSpiritSynchronize sync;
    sync.__set_lucky_value_(this->lucky_value_);
    this->attr_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_SYNCHRONIZE);
  }
}

bool SpiritActor::AddFacade(core::int32 facade_id, core::int32 duration_time,
    bool synchronize) {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    LOG_ERROR("SPIRIT_MODULE is disable.");
    return false;
  }

  const SpiritFacadeCell *cell = Configure::GetInstance()->GetSpiritFacade(facade_id);
  if (NULL == cell) {
    LOG_ERROR("SpiritFacadeCell(%d) not found in config.", facade_id);
    return false;
  }

  if (cell->facade_type_ == entity::SpiritFacadeType::WING &&
      game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_WING_FACADE) == false) {
    LOG_ERROR("SPIRIT_WING_FACADE is disbale.");
    return false;
  }

  core::int32 expire_time = 0;
  bool exist = false;

  SpiritFacadeIndexHashmap::iterator iter = this->facades_index_.find(facade_id); 
  if (this->facades_index_.end() == iter) {
    // 外观不存在
    exist = false;

    if (0 == duration_time) {
      expire_time = 0;
    } else {
      time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
      expire_time = now + duration_time;
    }

    SpiritFacade facade(facade_id, expire_time);
    this->facades_store_.push_back(facade);
    this->facades_index_[facade.GetID()] = this->facades_store_.size() - 1;

  } else {
    // 外观续期
    SpiritFacade &facade = this->facades_store_[iter->second];
    if (facade.GetExpireTime() == 0) {
      LOG_ERROR("Facade(%d) is never expire", facade_id);
      return false;
    }
    exist = facade.CheckExist();

    if (0 == duration_time) {
      facade.SetExpireTime(0);
    } else {
      time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
      facade.SetExpireTime(std::max(now, (time_t)facade.GetExpireTime()) + duration_time);
    }
    facade.SetExist(true);
  }

  // 更新属性
  if (!exist) {
    if (cell->physique_add_value_ != 0) {
      game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) + 
          cell->physique_add_value_, synchronize);
    }
    if (cell->strength_add_value_ != 0) {
      game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) +
          cell->strength_add_value_, synchronize);
    }
    if (cell->intelligence_add_value_ != 0) {
      game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) +
          cell->intelligence_add_value_, synchronize);
    }
    if (cell->agile_add_value_ != 0) {
      game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) +
          cell->agile_add_value_, synchronize);
    }
    if (cell->spirit_add_value_ > 0) {
      game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) +
          cell->spirit_add_value_, synchronize);
    }

    // 更新战斗力
    this->UpdateSpiritFightScore(synchronize);
  }

  // 同步数据
  if (synchronize) {
    gateway::protocol::MessageAttrSpiritFacadeSynchronize sync;
    if (exist) {
      sync.__set_type_(gateway::protocol::SpiritFacadeOperateType::UPDATE);
    } else {
      sync.__set_type_(gateway::protocol::SpiritFacadeOperateType::ADD);
    }
    sync.facade_.__set_id_(facade_id);
    sync.facade_.__set_expire_time_(expire_time);
    this->attr_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_FACADE_SYNCHRONIZE);
  }

  return true;
}

bool SpiritActor::TransformFacade(core::int32 facade_id, bool synchronize) {
  if (this->GetCurrentFacadeID() == facade_id) {
    return true;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    LOG_ERROR("SPIRIT_MODULE is disable.");
    return false;
  }

  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return false;
  }
  const SpiritFacadeCell *facade_cell =
    Configure::GetInstance()->GetSpiritFacade(facade_id);
  if (NULL == facade_cell) {
    LOG_ERROR("SpiritFacadeCell(%d) not found in config.", facade_id);
    return false;
  }

  core::int32 from_type = this->current_facade_type_;
  core::int32 to_type = facade_cell->facade_type_;

  if (entity::SpiritFacadeType::WING == to_type &&
      game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_WING_FACADE) == false) {
    LOG_ERROR("SPIRIT_WING_FACADE is disable.");
    return false;
  }

  if (facade_id != cell->horse_facade_id_ &&
      facade_id != cell->wing_facade_id_) {
    SpiritFacadeIndexHashmap::iterator iter = this->facades_index_.find(facade_id); 
    if (iter == this->facades_index_.end() ||
        this->facades_store_[iter->second].CheckExist() == false) {
      LOG_ERROR("Actor doesn't own this facade.");
      return false;
    }
  }

  // 更新数据
  if (from_type != to_type) {
    this->RemoveFacadeBuff(from_type);
    this->AddFacadeBuff(to_type);
    this->current_facade_type_ = to_type;
  }

  if (entity::SpiritFacadeType::HORSE == to_type) {
    this->horse_facade_id_ = facade_id;
  } else {
    this->wing_facade_id_ = facade_id;
  }

  game_actor->SetAttribute(entity::ActorAoiFields::CURRENT_SPIRIT_FACADE,
      this->GetCurrentFacadeID());

  // 同步数据
  if (synchronize) {
    gateway::protocol::MessageAttrSpiritSynchronize sync;
    sync.__set_current_facade_type_((entity::SpiritFacadeType::type)this->current_facade_type_);
    sync.__set_horse_facade_id_(this->horse_facade_id_);
    sync.__set_wing_facade_id_(this->wing_facade_id_);
    this->attr_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_SYNCHRONIZE);
  }

  return true;
}

void SpiritActor::DailyClean() {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    return;
  }

  // 清空幸运值
  this->SetLuckyValue(0);
  // 移除失效的外观
  this->RemoveExpiredFacade();
}

///////////////////////////////////////////////////////////////////////////////

bool SpiritActor::AddSpiritAttr(bool synchronize) {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return false;
  }

  core::int32 physique_add_value = cell->physique_add_value_;
  core::int32 strength_add_value = cell->strength_add_value_;
  core::int32 intelligence_add_value = cell->intelligence_add_value_;
  core::int32 agile_add_value = cell->agile_add_value_;
  core::int32 spirit_add_value = cell->spirit_add_value_;
  core::int32 move_speed_add_value = cell->move_speed_add_value_;

  for (size_t i = 0; i < this->facades_store_.size(); ++i) {
    const SpiritFacadeCell *facade_cell = Configure::GetInstance()->GetSpiritFacade(
        this->facades_store_[i].GetID());
    if (NULL == facade_cell) {
      LOG_ERROR("SpiritFacadeCell(%d) not found in config.", this->facades_store_[i].GetID());
      return false;
    }
    physique_add_value += facade_cell->physique_add_value_;
    strength_add_value += facade_cell->strength_add_value_;
    intelligence_add_value += facade_cell->intelligence_add_value_;
    agile_add_value += facade_cell->agile_add_value_;
    spirit_add_value += facade_cell->spirit_add_value_;
  }

  if (physique_add_value > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) +
        physique_add_value, synchronize);
  }
  if (strength_add_value > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) +
        strength_add_value, synchronize);
  }
  if (intelligence_add_value > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) +
        intelligence_add_value, synchronize);
  }
  if (agile_add_value > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) +
        agile_add_value, synchronize);
  }
  if (spirit_add_value > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) +
        spirit_add_value, synchronize);
  }
  if (move_speed_add_value > 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE) +
        move_speed_add_value, synchronize);
  }

  // 更新战斗力
  this->UpdateSpiritFightScore(synchronize);

  return true;
}

bool SpiritActor::AddFacadeBuff(core::int32 facade_type) {
  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return false;
  }

  core::uint32 buff_id; 
  if (entity::SpiritFacadeType::HORSE == facade_type) {
    buff_id = cell->horse_buff_id_;
  } else {
    buff_id = cell->wing_buff_id_;
  }

  if (FacadeRequest::GetInstance()->AddBuff(this->attr_actor_->GetID(), buff_id) == false) {
    LOG_ERROR("Add buff(%u) failed", buff_id);
    return false;
  }

  return true;
}

bool SpiritActor::RemoveFacadeBuff(core::int32 facade_type) {
  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return false;
  }

  core::uint32 buff_id; 
  if (entity::SpiritFacadeType::HORSE == facade_type) {
    buff_id = cell->horse_buff_id_;
  } else {
    buff_id = cell->wing_buff_id_;
  }

  if (FacadeRequest::GetInstance()->RemoveBuff(this->attr_actor_->GetID(), buff_id) == false) {
    LOG_ERROR("Remove buff(%u) failed", buff_id);
    return false;
  }
  
  return true;
}

void SpiritActor::RemoveExpiredFacade(bool synchronize) {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  for (size_t i = 0; i < this->facades_store_.size(); ++i) {
    SpiritFacade &facade = this->facades_store_[i];

    if (facade.CheckExist() && facade.GetExpireTime() > 0 &&
        now > facade.GetExpireTime()) {
      facade.SetExist(false);

      // 切换为默认外观
      if (this->GetCurrentFacadeID() == facade.GetID()) {
        if (entity::SpiritFacadeType::HORSE == this->current_facade_type_) {
          this->TransformFacade(cell->horse_facade_id_);
        } else {
          this->TransformFacade(cell->wing_buff_id_);
        }
      }

      // 更新属性
      const SpiritFacadeCell *facade_cell =
        Configure::GetInstance()->GetSpiritFacade(facade.GetID());
      if (NULL == facade_cell) {
        LOG_ERROR("SpiritFacadeCell(%d) not found in config.", facade.GetID());
        continue;
      }

      if (facade_cell->physique_add_value_ != 0) {
        game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
            game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) -
            facade_cell->physique_add_value_, synchronize);
      }
      if (facade_cell->strength_add_value_ != 0) {
        game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
            game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) -
            facade_cell->strength_add_value_, synchronize);
      }
      if (facade_cell->intelligence_add_value_ != 0) {
        game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
            game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) -
            facade_cell->intelligence_add_value_, synchronize);
      }
      if (facade_cell->agile_add_value_ != 0) {
        game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
            game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) -
            facade_cell->agile_add_value_, synchronize);
      }
      if (facade_cell->spirit_add_value_ > 0) {
        game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
            game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) -
            facade_cell->spirit_add_value_, synchronize);
      }

      // 更新战斗力
      this->UpdateSpiritFightScore(synchronize);
      
      // 同步数据
      if (synchronize) {
        gateway::protocol::MessageAttrSpiritFacadeSynchronize sync;
        sync.__set_type_(gateway::protocol::SpiritFacadeOperateType::REMOVE);
        sync.facade_.__set_id_(facade.GetID());
        sync.facade_.__set_expire_time_(facade.GetExpireTime());
        this->attr_actor_->SendMessage(sync,
            gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_FACADE_SYNCHRONIZE);
      }
    }
  }
}

void SpiritActor::UpdateSpiritFightScore(bool synchronize) {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  const SpiritCell *cell = Configure::GetInstance()->GetSpirit(this->level_);
  if (NULL == cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", this->level_);
    return;
  }

  FightScoreCalculator calc;
  calc.physique_ += cell->physique_add_value_;
  calc.strength_ += cell->strength_add_value_;
  calc.intelligence_ += cell->intelligence_add_value_;
  calc.agile_ += cell->agile_add_value_;
  calc.spirit_ += cell->spirit_add_value_;
  calc.move_speed_ += cell->move_speed_add_value_;

  for (size_t i = 0; i < this->facades_store_.size(); ++i) {
    const SpiritFacadeCell *facade_cell = Configure::GetInstance()->GetSpiritFacade(
        this->facades_store_[i].GetID());
    if (NULL == facade_cell) {
      LOG_ERROR("SpiritFacadeCell(%d) not found in config.", this->facades_store_[i].GetID());
      return;
    }
    calc.physique_ += facade_cell->physique_add_value_;
    calc.strength_ += facade_cell->strength_add_value_;
    calc.intelligence_ += facade_cell->intelligence_add_value_;
    calc.agile_ += facade_cell->spirit_add_value_;
    calc.spirit_ += facade_cell->spirit_add_value_;
  }

  game_actor->SetSubFightScore(entity::SubFightScoreType::SPIRIT,
       calc.GetFightScore(), synchronize);
}

}  // namespace attr

}  // namespace server

}  // namespace game

