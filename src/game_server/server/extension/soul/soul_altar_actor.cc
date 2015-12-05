#include "game_server/server/extension/soul/soul_altar_actor.h"

#include <cstddef>
#include <cstdio>
#include <map>

#include "global/common_functions.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "database_server/protocol/storage_soul_actor_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_soul_protocol_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/soul/soul_actor.h"

namespace game {

namespace server {

namespace soul {

bool SoulAltarCard::Initialize(const std::string &data) {
  std::vector<std::string> split_result;
  global::StringSplit(data.c_str(), ":", split_result);

  if (split_result.size() < 2) {
    return false;
  }

  this->card_id_ = atoi(split_result[0].c_str());
  this->exist_ = atoi(split_result[1].c_str());

  return true;
}

int SoulAltarCard::Serialize(char *buff, size_t size) const {
  return snprintf(buff, size, "%u:%d,",
      this->card_id_, this->exist_);
}

///////////////////////////////////////////////////////////////////////////////

SoulAltarActor::SoulAltarActor() : soul_actor_(NULL),
    summon_paid_(false), free_summon_times_(0), card_bind_(false), left_card_count_(0) {}
SoulAltarActor::~SoulAltarActor() {}

static void DecodeSoulAltarCard(const std::string &source, SoulAltarActor::SoulAltarCardVector *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }   

    SoulAltarCard data;
    if (data.Initialize(split_result[i]) == false) {
      LOG_ERROR("SoulAltarCard(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->push_back(data);
  }
}

static void EncodeSoulAltarCard(const SoulAltarActor::SoulAltarCardVector &source, std::string *result) {
  char buffer[128] = "";
  size_t count = 0;

  for (size_t i = 0; i < source.size(); ++i) {
    count += source[i].Serialize(buffer + count, sizeof(buffer) - count);
  }

  *result = buffer;
}

///////////////////////////////////////////////////////////////////////////////

bool SoulAltarActor::Initialize(SoulActor *soul_actor,
    const database::protocol::StorageSoulActorField &db_field) {
  if (NULL == soul_actor) {
    LOG_ERROR("SoulActor is null.");
    return false;
  }

  this->soul_actor_ = soul_actor;

  this->summon_paid_ = db_field.altar_summon_paid_;
  this->free_summon_times_ = db_field.altar_free_summon_times_;
  DecodeSoulAltarCard(db_field.altar_cards_, &this->cards_);
  this->card_bind_ = db_field.altar_card_bind_;

  for (size_t i = 0; i < this->cards_.size(); ++i) {
    if (this->cards_[i].exist_) {
      ++this->left_card_count_;
    }
  }

  GameActor *game_actor = this->soul_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  // 未开启功能直接返回
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SOUL_ALTAR) == false) {
    return true;
  }

  // 发送初始化数据
  this->SendInitToClient();

  return true;
}

void SoulAltarActor::Finalize() {
  this->left_card_count_ = 0;
  this->card_bind_ = false;
  this->cards_.clear();
  this->free_summon_times_ = 0;
  this->summon_paid_ = false;

  this->soul_actor_ = NULL;
}

void SoulAltarActor::SendInitToClient() {
  gateway::protocol::MessageSoulAltarInitialize message;
  message.__set_summon_paid_(this->summon_paid_);
  message.__set_free_summon_times_(this->free_summon_times_);

  for (size_t i = 0; i < this->cards_.size(); ++i) {
    gateway::protocol::SoulAltarCardData data;
    data.__set_card_pos_(i);
    data.__set_card_id_(this->cards_[i].card_id_);
    data.__set_exist_(this->cards_[i].exist_);
    message.cards_.push_back(data);
  }

  this->soul_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_INITIALIZE);
}

void SoulAltarActor::Save(database::protocol::StorageSoulActorField &db_field) {
  db_field.__set_altar_summon_paid_(this->summon_paid_);
  db_field.__set_altar_free_summon_times_(this->free_summon_times_);
  EncodeSoulAltarCard(this->cards_, &db_field.altar_cards_);
  db_field.__set_altar_card_bind_(this->card_bind_);
}

bool SoulAltarActor::EnableSoulAltarFunc(bool synchronize) {
  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

void SoulAltarActor::SetSummonPaid(bool synchronize) {
  this->summon_paid_ = true;

  if (synchronize) {
    gateway::protocol::MessageSoulAltarSynchronize sync;
    sync.__set_summon_paid_(this->summon_paid_);
    this->soul_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SYNCHRONIZE);
  }
}

core::uint32 SoulAltarActor::GetCardID(core::int32 pos) const {
  if (pos < 0 || pos > (core::int32)this->cards_.size()) {
    return 0;
  }
  if (this->cards_[pos].exist_) {
    return this->cards_[pos].card_id_;
  } else {
    return 0;
  }
}

void SoulAltarActor::AddFreeSummonTimes(bool synchronize) {
  ++this->free_summon_times_;

  if (synchronize) {
    gateway::protocol::MessageSoulAltarSynchronize sync;
    sync.__set_free_summon_times_(this->free_summon_times_);
    this->soul_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SYNCHRONIZE);
  }
}

void SoulAltarActor::SummonCards(bool card_bind, core::int32 hint_pos, bool synchronize) {
  if (hint_pos > kMaxCardSlotNum - 1 || hint_pos < 0) {
    LOG_ERROR("hint_pos is invalid.");
    return;
  }

  // 随机较差的掉落
  const LoadDropConfigure::DropElementVector &drop_items1 =
    game::server::Configure::GetInstance()->Drop(MISC_CONF()->soul_altar_drop_group1_id_);
  if (drop_items1.empty()) {
    LOG_ERROR("drop_items1 is empty.");
    return;
  }
  core::int32 cheat_card_id = drop_items1[0].resource_id_;

  // 随机较好的掉落
  const LoadDropConfigure::DropElementVector &drop_items2 =
    game::server::Configure::GetInstance()->Drop(MISC_CONF()->soul_altar_drop_group2_id_);
  if (drop_items2.size() < kMaxCardSlotNum - 1) {
    LOG_ERROR("drop_items2 not enough.");
    return;
  }

  this->cards_.clear();
  this->cards_.resize(kMaxCardSlotNum);

  size_t j = 0;
  for (size_t i = 0; i < this->cards_.size(); ++i) {
    if ((core::int32)i == hint_pos) {
      this->cards_[i].card_id_ = cheat_card_id;
    } else {
      this->cards_[i].card_id_ = drop_items2[j++].resource_id_;
    }
    this->cards_[i].exist_ = true;
  }
  this->left_card_count_ = this->cards_.size();
  this->card_bind_ = card_bind;

  if (synchronize) {
    for (size_t i = 0; i < this->cards_.size(); ++i) {
      gateway::protocol::MessageSoulAltarCardSynchronize sync;
      sync.__set_type_(gateway::protocol::SoulAltarCardOperateType::ADD);
      sync.data_.__set_card_pos_(i);
      sync.data_.__set_card_id_(this->cards_[i].card_id_);
      sync.data_.__set_exist_(this->cards_[i].exist_);
      this->soul_actor_->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_CARD_SYNCHRONIZE);
    }
  }
}

void SoulAltarActor::TakeCard(core::int32 pos, bool synchronize) {
  if (0 == this->GetCardID(pos)) {
    return;
  }

  this->cards_[pos].exist_ = false;

  if (synchronize) {
    gateway::protocol::MessageSoulAltarCardSynchronize sync;
    sync.__set_type_(gateway::protocol::SoulAltarCardOperateType::UPDATE);
    sync.data_.__set_card_pos_(pos);
    sync.data_.__set_exist_(false);
    this->soul_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_CARD_SYNCHRONIZE);
  }

  if (--this->left_card_count_ == 0) {
    this->ResetCards(synchronize);
  }
}

void SoulAltarActor::ResetCards(bool synchronize) {
  this->summon_paid_ = false;
  this->cards_.clear();
  this->left_card_count_ = 0;

  if (synchronize) {
    gateway::protocol::MessageSoulAltarSynchronize sync;
    sync.__set_summon_paid_(this->summon_paid_);
    this->soul_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SYNCHRONIZE);

    gateway::protocol::MessageSoulAltarCardSynchronize card_sync;
    card_sync.__set_type_(gateway::protocol::SoulAltarCardOperateType::CLEAR);
    this->soul_actor_->SendMessage(card_sync,
        gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_CARD_SYNCHRONIZE);
  }
}

void SoulAltarActor::DailyClean(bool synchronize) {
  this->free_summon_times_ = 0;

  if (synchronize) {
    gateway::protocol::MessageSoulAltarSynchronize sync;
    sync.__set_free_summon_times_(this->free_summon_times_);
    this->soul_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SYNCHRONIZE);
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game

