#include "game_server/server/extension/attr/attr_actor.h"

#include <cstddef>

#include "coresh/communicator.h"
#include "global/logging.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_attr_actor_save_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/attr/configure.h"

namespace game {

namespace server {

namespace attr {

AttrActor::AttrActor() : game_actor_(NULL) {}
AttrActor::~AttrActor() {}

bool AttrActor::Initialize(GameActor *game_actor,
    const database::protocol::StorageAttrActorField &db_field) {
  if (NULL == game_actor) {
    LOG_ERROR("game_actor is null.");
    return false;
  }

  this->game_actor_ = game_actor;
  this->db_field_ = db_field;

  if (this->noble_actor_.Initialize(this, this->db_field_) == false) {
    LOG_ERROR("Init NobleActor(%lu) failed.", game_actor->GetID());
    return false;
  }

  if (this->sword_actor_.Initialize(this, this->db_field_) == false) {
    LOG_ERROR("Init SwordActor(%lu) failed.", game_actor->GetID());
    return false;
  }

  if (this->element_actor_.Initialize(this, this->db_field_) == false) {
    LOG_ERROR("Init ElementActor(%lu) failed.", game_actor->GetID());
    return false;
  }
  
  if (this->rune_actor_.Initialize(this, this->db_field_) == false) {
    LOG_ERROR("Init RuneActor(%lu) faild.", game_actor_->GetID());
    return false;
  }

  if (this->legendary_weapon_actor_.Initialize(this, this->db_field_) == false) {
    LOG_ERROR("Init LegendaryWeaponActor(%lu) failed.", game_actor_->GetID());
    return false;
  }

  return true;
}

bool AttrActor::InitializeSpiritActor(const SpiritFacadeFieldVector& facades) {
  if (this->spirit_actor_.Initialize(this, this->db_field_, facades) == false) {
    LOG_ERROR("Init SpiritActor(%lu) failed.", this->GetID());
    return false;
  }
  this->spirit_actor_.SetLoadFinish();

  return true;
}

void AttrActor::Finalize() {
  this->legendary_weapon_actor_.Finalize();
  this->rune_actor_.Finalize();
  this->spirit_actor_.Finalize();
  this->element_actor_.Finalize();
  this->sword_actor_.Finalize();
  this->noble_actor_.Finalize();

  this->game_actor_ = NULL;
}

bool AttrActor::CheckLoadFinish() const {
  return this->spirit_actor_.CheckLoadFinish();
}

void AttrActor::SendLoadFinishEvent() {
  LOG_DEBUG("玩家(%lu) ATTR扩展加载成功", this->GetID());

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetID());
  load_finish.__set_extension_type_(ExtensionType::ATTR);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
}

void AttrActor::Save() {
  if (NULL == this->game_actor_) {
    LOG_ERROR("Save AttrActor failed, game_actor is null.");
    return;
  }
  if (this->CheckLoadFinish() == false) {
    return;
  }

  database::protocol::StorageAttrActorSaveRequest request;
  request.__set_actor_id_(this->GetID());

  this->noble_actor_.Save(request.field_);
  this->sword_actor_.Save(request.field_);
  this->element_actor_.Save(request.field_);
  this->spirit_actor_.Save(request.field_);
  this->rune_actor_.Save(request.field_);
  this->legendary_weapon_actor_.Save(request.field_);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_ATTR_ACTOR_SAVE, this->GetID()) == false) {
    LOG_ERROR("Send StorageAttrSaveRequest to database server failed.");
    return;
  }
}

void AttrActor::DailyClean() {
  this->sword_actor_.DailyClean();
  this->spirit_actor_.DailyClean();
}

static void AdjustAttrBonus(GameActor *game_actor,
    core::int32 attr_bonus_id, core::int32 factor, bool synchronize) {
  const AttrBonusCell *cell = Configure::GetInstance()->GetAttrBonus(attr_bonus_id);
  if (NULL == cell) {
    LOG_ERROR("attr_bonus_id(%d) is invalid.", attr_bonus_id);
    return;
  }

#define ADJUST_ATTR(_cell_attr, _attr_enum)                              \
  if (cell->_cell_attr > 0) {                                            \
    game_actor->SetAttribute(entity::RoleServerFields::_attr_enum,       \
        game_actor->GetAttribute(entity::RoleServerFields::_attr_enum) + \
            factor * cell->_cell_attr, synchronize);                     \
  }                                                                      \

  ADJUST_ATTR(physique_add_value_, PHYSIQUE_ADD_VALUE)
  ADJUST_ATTR(strength_add_value_, STRENGTH_ADD_VALUE) 
  ADJUST_ATTR(intelligence_add_value_, INTELLIGENCE_ADD_VALUE)
  ADJUST_ATTR(agile_add_value_, AGILE_ADD_VALUE)
  ADJUST_ATTR(spirit_add_value_, SPIRIT_ADD_VALUE)
  ADJUST_ATTR(max_hp_add_value_, MAX_HP_ADD_VALUE)
  ADJUST_ATTR(max_mp_add_value_, MAX_MP_ADD_VALUE)
  ADJUST_ATTR(physics_attack_add_value_, PHYSICS_ATTACK_ADD_VALUE)
  ADJUST_ATTR(physics_defence_add_value_, PHYSICS_DEFENCE_ADD_VALUE)
  ADJUST_ATTR(magic_attack_add_value_, MAGIC_ATTACK_ADD_VALUE)
  ADJUST_ATTR(magic_defence_add_value_, MAGIC_DEFENCE_ADD_VALUE)
  ADJUST_ATTR(dodge_add_value_, DODGE_ADD_VALUE)
  ADJUST_ATTR(crit_add_value_, CRIT_ADD_VALUE)
  ADJUST_ATTR(ignore_defence_add_value_, IGNORE_DEFENCE_ADD_VALUE)
  ADJUST_ATTR(move_speed_add_value_, MOVE_SPEED_ADD_VALUE)
  ADJUST_ATTR(dodge_percent_add_value_, DODGE_PERCENT_ADD_VALUE)
  ADJUST_ATTR(crit_percent_add_value_, CRIT_PERCENT_ADD_VALUE)
  ADJUST_ATTR(ignore_defence_percent_add_value_, IGNORE_DEFENCE_PERCENT_ADD_VALUE)
  ADJUST_ATTR(physics_hurt_reduce_add_percent_, PHYSICS_HURT_REDUCE_ADD_PERCENT)
  ADJUST_ATTR(magic_hurt_reduce_add_percent_, MAGIC_HURT_REDUCE_ADD_PERCENT)
  ADJUST_ATTR(hurt_reduce_add_percent_, HURT_REDUCE_ADD_PERCENT)
  ADJUST_ATTR(hurt_random_percent_add_, HURT_RANDOM_PERCENT_ADD)
  ADJUST_ATTR(cure_percent_add_, CURE_PERCENT_ADD)

#undef ADJUST_ATTR 
}

void AttrActor::AddAttrBonus(core::int32 attr_bonus_id, bool synchronize) {
  if (NULL == this->game_actor_) {
    LOG_ERROR("GameActor is null");
    return;
  }
  AdjustAttrBonus(this->game_actor_, attr_bonus_id, 1, synchronize);
}

void AttrActor::RemoveAttrBonus(core::int32 attr_bonus_id, bool synchronize) {
  if (NULL == this->game_actor_) {
    LOG_ERROR("GameActor is null");
    return;
  }
  AdjustAttrBonus(this->game_actor_, attr_bonus_id, -1, synchronize);
}

void AttrActor::AddAttrBonusFightScore(core::int32 attr_bonus_id, FightScoreCalculator *calc) {
  const AttrBonusCell *cell = Configure::GetInstance()->GetAttrBonus(attr_bonus_id);
  if (NULL == cell) {
    LOG_ERROR("attr_bonus_id(%d) is invalid.", attr_bonus_id);
    return;
  }

  calc->physique_ += cell->physique_add_value_;
  calc->strength_ += cell->strength_add_value_;
  calc->intelligence_ += cell->intelligence_add_value_;
  calc->agile_ += cell->agile_add_value_;
  calc->spirit_ += cell->spirit_add_value_;
  calc->max_hp_ += cell->max_hp_add_value_;
  calc->max_mp_ += cell->max_mp_add_value_;
  calc->physics_attack_ += cell->physics_attack_add_value_;
  calc->physics_defence_ += cell->physics_defence_add_value_;
  calc->magic_attack_ += cell->magic_attack_add_value_;
  calc->magic_defence_ += cell->magic_defence_add_value_;
  calc->dodge_ += cell->dodge_add_value_;
  calc->crit_ += cell->crit_add_value_;
  calc->ignore_defence_ += cell->ignore_defence_add_value_;
  calc->move_speed_ += cell->move_speed_add_value_;
  calc->dodge_percent_add_value_ += cell->dodge_percent_add_value_;
  calc->crit_percent_add_value_ += cell->crit_percent_add_value_;
  calc->ignore_defence_percent_add_value_ += cell->ignore_defence_percent_add_value_;
  calc->physics_hurt_reduce_add_percent_ += cell->physics_hurt_reduce_add_percent_;
  calc->magic_hurt_reduce_add_percent_ += cell->magic_hurt_reduce_add_percent_;
  calc->hurt_reduce_add_percent_ += cell->hurt_reduce_add_percent_;
  calc->hurt_random_percent_add_ += cell->hurt_random_percent_add_;
  calc->cure_percent_add_ += cell->cure_percent_add_;
}

}  // namespace attr

}  // namespace server

}  // namespace game

