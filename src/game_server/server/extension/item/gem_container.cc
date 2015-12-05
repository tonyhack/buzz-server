#include "game_server/server/extension/item/gem_container.h"

#include <cstddef>

#include "entity/inform_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_gem_save_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "game_server/server/game_server.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/event/game_item_event_types.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/packet_container.h"

namespace game {

namespace server {

namespace item {

GemContainer::GemContainer() : load_finish_(false), item_actor_(NULL),
    storage_min_empty_index_(-1), gathering_point_(0) {}
GemContainer::~GemContainer() {}

bool GemContainer::Initialize(ItemActor *item_actor) {
  this->item_actor_ = item_actor;
  return true;
}

void GemContainer::Finalize() {
  this->gathering_point_ = 0;
  gems_index_.clear();
  gems_store_.clear();
  this->item_actor_ = NULL;
  this->load_finish_ = false;
}

bool GemContainer::InitializeGems(const database::protocol::StorageItemActorField &db_item_actor_field,
    const GemFieldVector &gems) {
  if (NULL == this->item_actor_) {
    LOG_ERROR("ItemActor is null.");
    return false;
  }
  GameActor *game_actor = this->item_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  // 初始化索引容器
  this->gems_index_.resize(entity::GemLocationType::MAX);
  // --宝石仓库栏位
  this->gems_index_[entity::GemLocationType::GEM_STORAGE].resize(
      MISC_CONF()->gem_max_storage_capacity_, NULL);
  // --装备栏位
  size_t equip_capacity = 0;
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_9)) {
    equip_capacity = 9;
  } else if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_8)) {
    equip_capacity = 8;
  } else if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_7)) {
    equip_capacity = 7;
  } else if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_6)) {
    equip_capacity = 6;
  } else if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_5)) {
    equip_capacity = 5;
  } else if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_4)) {
    equip_capacity = 4;
  } else if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_3)) {
    equip_capacity = 3;
  } else if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_SLOT_2)) {
    equip_capacity = 2;
  } else {
    equip_capacity = 1;
  }

  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    this->gems_index_[i].resize(equip_capacity, NULL);
  }

  for (size_t i = 0; i < gems.size(); ++i) {
    const database::protocol::StorageGemField &db_field = gems[i];

    if ((size_t)db_field.location_type_ > gems_index_.size() - 1) {
      LOG_ERROR("Databse location_type(%d) is invalid.", db_field.location_type_);
      return false;
    }
    if ((size_t)db_field.location_index_ > gems_index_[db_field.location_type_].size() - 1) {
      LOG_ERROR("Database location_index(%d) is invalid.", db_field.location_type_);
      return false;
    }

    Location location(db_field.location_type_, db_field.location_index_);
    Gem gem(db_field.gem_id_, db_field.gem_level_,
            db_field.gem_exp_, db_field.locked_);
    // store
    std::pair<GemMap::iterator, bool> result =
      this->gems_store_.insert(std::make_pair(location, gem));
    // index
    this->gems_index_[db_field.location_type_][db_field.location_index_] =
      &result.first->second;
  }
  // 宝石仓库最小空位
  for (size_t i = 0; i < this->gems_index_[entity::GemLocationType::GEM_STORAGE].size(); ++i) {
    if (NULL == this->gems_index_[entity::GemLocationType::GEM_STORAGE][i]) {
      this->storage_min_empty_index_ = (core::int16)i;
      break;
    }
  }

  // 宝石激活点
  this->gathering_point_ = db_item_actor_field.gem_gathering_point_;

  // 未开启功能返回
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    return true;
  }

  // 增加宝石属性
  for (GemMap::iterator iter = this->gems_store_.begin();
       iter != this->gems_store_.end(); ++iter) {
    const Location &location = iter->first;
    Gem &gem = iter->second;

    if (entity::GemLocationType::GEM_STORAGE == location.type_) {
      continue;
    }
    this->AddGemAttr(gem.id_, gem.level_, false);
  }

  // 更新战斗力
  this->UpdateGemFightScore(false);

  // 发送数据初始化协议
  this->SendInitToClient();

  return true;
}

void GemContainer::SendInitToClient() {
  if (NULL == this->item_actor_) {
    LOG_ERROR("ItemActor is null.");
    return;
  }

  gateway::protocol::MessageGemInitialize message;
  for (GemMap::iterator iter = this->gems_store_.begin();
       iter != this->gems_store_.end(); ++iter) {
    gateway::protocol::GemData data;
    const Location &location = iter->first;
    Gem &gem = iter->second;

    data.__set_location_type_((entity::GemLocationType::type)location.type_);
    data.__set_location_index_(location.index_);
    data.__set_id_(gem.id_);
    data.__set_level_(gem.level_);
    data.__set_exp_(gem.exp_);
    data.__set_locked_(gem.locked_);

    message.gems_.push_back(data);
  }
  message.__set_gathering_point_(this->gathering_point_);

  this->item_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_GEM_INITIALIZE);
}

void GemContainer::Save(database::protocol::StorageItemActorField &db_item_actor_field) {
  if (NULL == this->item_actor_) {
    LOG_ERROR("ItemActor is null.");
    return;
  }

  db_item_actor_field.gem_gathering_point_ = this->gathering_point_;

  database::protocol::StorageGemSaveRequest request;
  request.__set_actor_id_(this->item_actor_->GetID());

  for (GemMap::iterator iter = this->gems_store_.begin();
       iter != this->gems_store_.end(); ++iter) {
    database::protocol::StorageGemField db_field;
    const Location &location = iter->first;
    Gem &gem = iter->second;

    if (!gem.exist_) {
      continue;
    }

    db_field.__set_location_type_(location.type_);
    db_field.__set_location_index_(location.index_);
    db_field.__set_gem_id_(gem.id_);
    db_field.__set_gem_level_(gem.level_);
    db_field.__set_gem_exp_(gem.exp_);
    db_field.__set_locked_(gem.locked_);

    request.fields_.push_back(db_field);
  }

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_GEM_SAVE,
          this->item_actor_->GetID()) == false) {
    LOG_ERROR("Send StorageGemSaveRequest to database server failed.");
    return;
  }
}

bool GemContainer::EnableGemFunc(bool synchronize) {
  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

bool GemContainer::EnableGemSlot2Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 1) {
      return false;
    }
    this->gems_index_[i].resize(2, NULL);
  }
  return true;
}

bool GemContainer::EnableGemSlot3Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 2) {
      return false;
    }
    this->gems_index_[i].resize(3, NULL);
  }
  return true;
}

bool GemContainer::EnableGemSlot4Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 3) {
      return false;
    }
    this->gems_index_[i].resize(4, NULL);
  }
  return true;
}

bool GemContainer::EnableGemSlot5Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 4) {
      return false;
    }
    this->gems_index_[i].resize(5, NULL);
  }
  return true;
}

bool GemContainer::EnableGemSlot6Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 5) {
      return false;
    }
    this->gems_index_[i].resize(6, NULL);
  }
  return true;
}

bool GemContainer::EnableGemSlot7Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 6) {
      return false;
    }
    this->gems_index_[i].resize(7, NULL);
  }
  return true;
}

bool GemContainer::EnableGemSlot8Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 7) {
      return false;
    }
    this->gems_index_[i].resize(8, NULL);
  }
  return true;
}

bool GemContainer::EnableGemSlot9Func() {
  for (size_t i = entity::GemLocationType::GEM_STORAGE + 1;
       i < this->gems_index_.size(); ++i) {
    if (this->gems_index_[i].size() != 8) {
      return false;
    }
    this->gems_index_[i].resize(9, NULL);
  }
  return true;
}

bool GemContainer::CheckLocationValid(const Location &location) const {
  if ((size_t)location.type_ > gems_index_.size() - 1) {
    return false;
  }
  if ((size_t)location.index_ > gems_index_[location.type_].size() - 1) {
    return false;
  }

  return true;
}

bool GemContainer::CheckGemAttrTypeDuplicate(core::int16 except_index,
    entity::GemLocationType::type location_type,
    core::int32 gem_id, core::int32 gem_level) const {

  const GemCell *check_cell = Configure::GetInstance()->GetGem(gem_id, gem_level);
  if (NULL == check_cell) {
    LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
              gem_id, gem_level);
    return true;
  }

  for (size_t i = 0; i < this->gems_index_[location_type].size(); ++i) {
    Gem *gem = this->gems_index_[location_type][i];
    if (NULL == gem || !gem->exist_) {
      continue;
    }
    if (i == (size_t)except_index) {
      continue;
    }

    const GemCell *compare_cell = Configure::GetInstance()->GetGem(gem->id_, gem->level_);
    if (NULL == compare_cell) {
      LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
                gem->id_, gem->level_);
      return true;
    }
    if (compare_cell->attr_id_ == check_cell->attr_id_) {
      return true;
    }
  }

  return false;
}

GemContainer::Gem *GemContainer::GetGem(const Location &location) const {
  Gem *gem = this->gems_index_[location.type_][location.index_];
  if (NULL == gem || !gem->exist_) {
    return NULL;
  } else {
    return gem;
  }
}

void GemContainer::AddGem(const Location &location, const Gem &gem, bool synchronize) {
  if (NULL == this->gems_index_[location.type_][location.index_]) {
    std::pair<GemMap::iterator, bool> result =
      this->gems_store_.insert(std::make_pair(location, gem));
    this->gems_index_[location.type_][location.index_] = &result.first->second;
  } else {
    Gem *gem_dest = this->gems_index_[location.type_][location.index_];
    *gem_dest = gem;
    gem_dest->exist_ = true;
  }

  // 更新宝石仓库最小空位
  if (entity::GemLocationType::GEM_STORAGE == location.type_ &&
      location.index_ == this->storage_min_empty_index_) {
    bool found = false;

    for (size_t i = location.index_; i < this->gems_index_[location.type_].size(); ++i) {
      Location storage_location(location.type_, (core::int16)i);
      if (NULL == this->GetGem(storage_location)) {
        this->storage_min_empty_index_= (core::int16)i;
        found = true;
        break;
      }
    }
    if (!found) {
      this->storage_min_empty_index_ = -1;
    }
  }

  if (synchronize) {
    gateway::protocol::MessageGemSynchronize sync;
    sync.__set_type_(gateway::protocol::GemOperateType::ADD);
    sync.gem_.__set_location_type_((entity::GemLocationType::type)location.type_);
    sync.gem_.__set_location_index_(location.index_);
    sync.gem_.__set_id_(gem.id_);
    sync.gem_.__set_level_(gem.level_);
    sync.gem_.__set_exp_(gem.exp_);
    sync.gem_.__set_locked_(gem.locked_);

    this->item_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GEM_SYNCHRONIZE);
  }
}

void GemContainer::RemoveGem(const Location &location, bool synchronize) {
  this->gems_index_[location.type_][location.index_]->exist_ = false;

  // 更新宝石仓库最小空位
  if (entity::GemLocationType::GEM_STORAGE == location.type_) {
    if (-1 == this->storage_min_empty_index_) {
      this->storage_min_empty_index_ = location.index_;
    } else if (location.index_ < this->storage_min_empty_index_) {
      this->storage_min_empty_index_ = location.index_;
    }
  }

  if (synchronize) {
    gateway::protocol::MessageGemSynchronize sync;
    sync.__set_type_(gateway::protocol::GemOperateType::REMOVE);
    sync.gem_.__set_location_type_((entity::GemLocationType::type)location.type_);
    sync.gem_.__set_location_index_(location.index_);

    this->item_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GEM_SYNCHRONIZE);
  }
}

void GemContainer::SwapGem(const Location &lhs, const Location &rhs, bool synchronize) {
  std::swap(this->gems_store_[lhs], this->gems_store_[rhs]);

  if (synchronize) {
    Gem *gem = NULL;

    gateway::protocol::MessageGemSynchronize sync;
    sync.__set_type_(gateway::protocol::GemOperateType::UPDATE);

    gem = this->gems_index_[lhs.type_][lhs.index_];
    if (NULL == gem) {
      return;
    }
    sync.gem_.__set_location_type_((entity::GemLocationType::type)lhs.type_);
    sync.gem_.__set_location_index_(lhs.index_);
    sync.gem_.__set_id_(gem->id_);
    sync.gem_.__set_level_(gem->level_);
    sync.gem_.__set_exp_(gem->exp_);
    sync.gem_.__set_locked_(gem->locked_);
    this->item_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GEM_SYNCHRONIZE);

    gem = this->gems_index_[rhs.type_][rhs.index_];
    if (NULL == gem) {
      return;
    }
    sync.gem_.__set_location_type_((entity::GemLocationType::type)rhs.type_);
    sync.gem_.__set_location_index_(rhs.index_);
    sync.gem_.__set_id_(gem->id_);
    sync.gem_.__set_level_(gem->level_);
    sync.gem_.__set_exp_(gem->exp_);
    sync.gem_.__set_locked_(gem->locked_);
    this->item_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GEM_SYNCHRONIZE);
  }
}

void GemContainer::SynchronizeGem(const Location &location) {
  Gem *gem = this->GetGem(location);
  if (NULL == gem) {
    return;
  }

  gateway::protocol::MessageGemSynchronize sync;
  sync.__set_type_(gateway::protocol::GemOperateType::UPDATE);
  sync.gem_.__set_location_type_((entity::GemLocationType::type)location.type_);
  sync.gem_.__set_location_index_(location.index_);
  sync.gem_.__set_id_(gem->id_);
  sync.gem_.__set_level_(gem->level_);
  sync.gem_.__set_exp_(gem->exp_);
  sync.gem_.__set_locked_(gem->locked_);
  this->item_actor_->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_GEM_SYNCHRONIZE);
}

static void AdjustAttrValue(GameActor *game_actor,
    GemCell::AttrType::type attr_id, core::int32 attr_value, bool synchronize) {
  switch (attr_id) {
    case GemCell::AttrType::PHYSIQUE:
      game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::STRENGTH:
      game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::INTELLIGENCE:
      game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::AGILE:
      game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::SPIRIT:
      game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::MAX_HP:
      game_actor->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::MAX_MP:
      game_actor->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::PHYSICS_ATTACK:
      game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::PHYSICS_DEFENCE:
      game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::MAGIC_ATTACK:
      game_actor->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::MAGIC_DEFENCE:
      game_actor->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::DODGE:
      game_actor->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::CRIT:
      game_actor->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::IGNORE_DEFENCE:
      game_actor->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) + attr_value,
          synchronize);
      break;
    case GemCell::AttrType::MOVE_SPEED:
      game_actor->SetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE) + attr_value,
          synchronize);
      break;
    default:
      break;
  }
}

void GemContainer::AddGemAttr(core::int32 gem_id, core::int32 gem_level, bool synchronize) {
  const GemCell *cell = Configure::GetInstance()->GetGem(gem_id, gem_level);
  if (NULL == cell) {
    LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
              gem_id, gem_level);
    return;
  }

  GameActor *game_actor = this->item_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  AdjustAttrValue(game_actor, cell->attr_id_, cell->attr_value_, synchronize);
}

void GemContainer::RemoveGemAttr(core::int32 gem_id, core::int32 gem_level, bool synchronize) {
  const GemCell *cell = Configure::GetInstance()->GetGem(gem_id, gem_level);
  if (NULL == cell) {
    LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
              gem_id, gem_level);
    return;
  }

  GameActor *game_actor = this->item_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  AdjustAttrValue(game_actor, cell->attr_id_, -1 * cell->attr_value_, synchronize);
}

const GemContainer::Gem *GemContainer::GetGemInfo(const Location &location) const {
  if (this->CheckLocationValid(location) == false) {
    return NULL;
  }

  return this->GetGem(location);
}

bool GemContainer::CheckGemStorageFull() const {
  return this->storage_min_empty_index_ == -1;
}

bool GemContainer::Add(core::int32 gem_id, bool synchronize) {
  if (this->CheckGemStorageFull()) {
    return false;
  }

  Location location(entity::GemLocationType::GEM_STORAGE,
                    this->storage_min_empty_index_);
  Gem gem(gem_id, 1, 0, false);

  this->AddGem(location, gem, synchronize);

  return true;
}

bool GemContainer::Move(const Location &src, const Location &dest, bool synchronize) {
  if (this->CheckLocationValid(src) == false ||
      this->CheckLocationValid(dest) == false ) {
    return false;
  }

  Gem *src_gem = this->GetGem(src); 
  Gem *dest_gem = this->GetGem(dest);

  if (NULL == src_gem) {
    return false;
  }

  if ((src.type_ == dest.type_)) {
    // 同类容器移动
    if (NULL == dest_gem) {
      this->AddGem(dest, *src_gem, synchronize);
      this->RemoveGem(src, synchronize);
    } else {
      this->SwapGem(src, dest, synchronize);
    }
  } else if (entity::GemLocationType::GEM_STORAGE == src.type_) {
    // 仓库到其他
    if (this->CheckGemAttrTypeDuplicate(dest.index_,
            entity::GemLocationType::type(dest.type_),
            src_gem->id_, src_gem->level_)) {
      return false;
    }

    if (NULL == dest_gem) {
      this->AddGemAttr(src_gem->id_, src_gem->level_, synchronize);
      this->AddGem(dest, *src_gem, synchronize);
      this->RemoveGem(src, synchronize);
    } else {
      this->RemoveGemAttr(dest_gem->id_, dest_gem->level_, synchronize);
      this->AddGemAttr(src_gem->id_, src_gem->level_, synchronize);
      this->SwapGem(src, dest, synchronize);
      // 自动锁定
      this->Lock(src, true);
    }

    // 更新战斗力
    this->UpdateGemFightScore();

    if (this->item_actor_ != NULL) {
      // 宝石镶嵌事件通知
      event::EventItemEquipInlay event;
      event.__set_id_(this->item_actor_->GetID());
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ITEM_EQUIP_INLAY, item_actor_->GetID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
    }

  } else if (entity::GemLocationType::GEM_STORAGE == dest.type_) {
    // 其他到仓库
    if (NULL == dest_gem) {
      this->RemoveGemAttr(src_gem->id_, src_gem->level_, synchronize);
      this->AddGem(dest, *src_gem, synchronize);
      this->RemoveGem(src, synchronize);
    } else {
      if (this->CheckGemAttrTypeDuplicate(src.index_,
              entity::GemLocationType::type(src.type_),
              dest_gem->id_, dest_gem->level_)) {
        return false;
      }
      this->RemoveGemAttr(src_gem->id_, src_gem->level_, synchronize);
      this->AddGemAttr(dest_gem->id_, dest_gem->level_, synchronize);
      this->SwapGem(src, dest, synchronize);
    }
    // 自动锁定
    this->Lock(dest, true);

    // 更新战斗力
    this->UpdateGemFightScore();

  } else {
    return false;
  }

  return true;
}

bool GemContainer::Lock(const Location &location, bool locked, bool synchronize) {
  if (this->CheckLocationValid(location) == false) {
    return false;
  }

  Gem *gem = this->GetGem(location);
  if (NULL == gem) {
    return false;
  }

  if (location.type_ != entity::GemLocationType::GEM_STORAGE) {
    return false;
  }

  if (gem->locked_ == locked && synchronize) {
    synchronize = false;
  }

  gem->locked_ = locked;

  if (synchronize) {
    this->SynchronizeGem(location);
  }

  return true;
}

bool GemContainer::LockAll(bool locked, bool synchronize) {
  core::int32 location_type = entity::GemLocationType::GEM_STORAGE; 

  for (size_t i = 0; i < this->gems_index_[location_type].size(); ++i) {
    Location location(location_type, (core::int16)i);

    Gem *gem = this->GetGem(location);
    if (NULL == gem) {
      continue;
    }
    this->Lock(location, locked, synchronize);
  }

  return true;
}

core::int32 GemContainer::Decompose(bool synchronize) {
  core::int32 location_type = entity::GemLocationType::GEM_STORAGE; 
  // 获取背包容器
  PacketContainer *packet = (PacketContainer *)this->item_actor_->GetContainer(
      entity::ItemContainerType::PACKET);
  if (NULL == packet) {
    LOG_ERROR("Get packet container failed.");
    return -1;
  }
  GameActor *game_actor = this->item_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return -1;
  }

  core::int32 shard_count = 0;

  for (size_t i = 0; i < this->gems_index_[location_type].size(); ++i) {
    Location location(location_type, (core::int16)i);

    Gem *gem = this->GetGem(location);
    if (NULL == gem || gem->locked_) {
      continue;
    }

    const GemCell *cell = Configure::GetInstance()->GetGem(gem->id_, gem->level_);
    if (NULL == cell) {
      LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
                gem->id_, gem->level_);
      return -1;
    }
    
    shard_count += cell->decompose_get_shard_count_ + gem->exp_ / cell->shard_per_exp_;
  }

  // 移除非锁定的宝石
  for (size_t i = 0; i < this->gems_index_[location_type].size(); ++i) {
    Location location(location_type, (core::int16)i);

    Gem *gem = this->GetGem(location);
    if (NULL == gem || gem->locked_) {
      continue;
    }

    this->RemoveGem(location, synchronize);
  }

  // 增加宝石碎片
  game_actor->AddResource(entity::ResourceID::GEM_SHARD, shard_count);

  return shard_count;
}

void GemContainer::AddExp(const Location &location, core::int32 exp, bool synchronize) {
  if (this->CheckLocationValid(location) == false) {
    return;
  }

  Gem *gem = this->GetGem(location);
  if (NULL == gem) {
    return;
  }

  const GemCell *cell = Configure::GetInstance()->GetGem(gem->id_, gem->level_);
  if (NULL == cell) {
    LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
              gem->id_, gem->level_);
    return;
  }

  gem->exp_ += exp;

  core::int32 original_level = gem->level_;

  while (gem->exp_ >= cell->exp_) {
    cell = Configure::GetInstance()->GetGem(gem->id_, gem->level_ + 1);
    if (NULL == cell) {
      break;
    }
    ++gem->level_;
  }

  if (gem->level_ > original_level) {
    this->RemoveGemAttr(gem->id_, original_level, synchronize);
    this->AddGemAttr(gem->id_, gem->level_, synchronize);

    if (synchronize) {
      if (gem->level_ >= 6 && gem->level_ <= 9) {
        // 世界公告: 宝石升级到高等级(6~9级)
        gateway::protocol::MessageInformNotify inform;
        inform.__set_id_(entity::InformType::GEM_HIGH_LEVEL);
        inform.params_.push_back(this->item_actor_->GetStringID());
        inform.params_.push_back(this->item_actor_->GetName());
        inform.params_.push_back(global::ToString(gem->id_));
        inform.params_.push_back(global::ToString(gem->level_));
        GameServerSingleton::GetInstance().BroadcastMessage(inform,
            gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
      } else if (Configure::GetInstance()->GetGem(gem->id_, gem->level_ + 1) == NULL) {
        // 世界公告: 宝石升级到顶级
        gateway::protocol::MessageInformNotify inform;
        inform.__set_id_(entity::InformType::GEM_TOP_LEVEL);
        inform.params_.push_back(this->item_actor_->GetStringID());
        inform.params_.push_back(this->item_actor_->GetName());
        inform.params_.push_back(global::ToString(gem->id_));
        GameServerSingleton::GetInstance().BroadcastMessage(inform,
            gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
      }
    }
  }

  if (synchronize) {
    this->SynchronizeGem(location);
  }

  // 更新战斗力
  this->UpdateGemFightScore(synchronize);
}

static void AddAttributeToCalculator(FightScoreCalculator *calc,
    GemCell::AttrType::type type, core::int32 value) {
  switch (type) {
    case GemCell::AttrType::PHYSIQUE:
      calc->physique_ += value;
      break;
    case GemCell::AttrType::STRENGTH:
      calc->strength_ += value;
      break;
    case GemCell::AttrType::INTELLIGENCE:
      calc->intelligence_ += value;
      break;
    case GemCell::AttrType::AGILE:
      calc->agile_ += value;
      break;
    case GemCell::AttrType::SPIRIT:
      calc->spirit_ += value;
      break;
    case GemCell::AttrType::MAX_HP:
      calc->max_hp_ += value;
      break;
    case GemCell::AttrType::MAX_MP:
      calc->max_mp_ += value;
      break;
    case GemCell::AttrType::PHYSICS_ATTACK:
      calc->physics_attack_ += value;
      break;
    case GemCell::AttrType::PHYSICS_DEFENCE:
      calc->physics_defence_ += value;
      break;
    case GemCell::AttrType::MAGIC_ATTACK:
      calc->magic_attack_ += value;
      break;
    case GemCell::AttrType::MAGIC_DEFENCE:
      calc->magic_defence_ += value;
      break;
    case GemCell::AttrType::DODGE:
      calc->dodge_ += value;
      break;
    case GemCell::AttrType::CRIT:
      calc->crit_ += value;
      break;
    case GemCell::AttrType::IGNORE_DEFENCE:
      calc->ignore_defence_ += value;
      break;
    case GemCell::AttrType::MOVE_SPEED:
      calc->move_speed_ += value;
      break;
    default:
      break;
  }
}

void GemContainer::UpdateGemFightScore(bool synchronize) {
  if (NULL == this->item_actor_) {
    LOG_ERROR("ItemActor is null.");
    return;
  }
  GameActor *game_actor = this->item_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  FightScoreCalculator calc;

  for (GemMap::iterator iter = this->gems_store_.begin();
       iter != this->gems_store_.end(); ++iter) {
    const Location &location = iter->first;
    if (entity::GemLocationType::GEM_STORAGE == location.type_) {
      continue;
    }
    Gem *gem = this->GetGem(location);
    if (NULL == gem) {
      continue;
    }

    const GemCell *cell = Configure::GetInstance()->GetGem(gem->id_, gem->level_);
    if (NULL == cell) {
      LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
                gem->id_, gem->level_);
      return;
    }

    AddAttributeToCalculator(&calc, cell->attr_id_, cell->attr_value_);
  }

  game_actor->SetSubFightScore(entity::SubFightScoreType::GEM,
      calc.GetFightScore(), synchronize);
}

}  // namespace item

}  // namespace server

}  // namespace game

