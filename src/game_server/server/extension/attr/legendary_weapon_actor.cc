#include "game_server/server/extension/attr/legendary_weapon_actor.h"

#include <cstdio>
#include <algorithm>
#include <string>

#include "global/common_functions.h"
#include "global/logging.h"
#include "database_server/protocol/storage_attr_actor_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/configure.h"

namespace game {

namespace server {

namespace attr {

LegendaryWeaponActor::LegendaryWeaponActor() {}
LegendaryWeaponActor::~LegendaryWeaponActor() {}

static void DecodeWeaponPieces(const std::string &source,
    LegendaryWeaponActor::WeaponPiecesMap *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    std::vector<std::string> split_result2;
    global::StringSplit(split_result[i].c_str(), ":", split_result2);

    if (split_result2.size() < 2) {
      LOG_ERROR("Weapon pieces data(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    core::int32 weapon_id = atoi(split_result2[0].c_str());

    const LegendaryWeaponCell *cell =
        Configure::GetInstance()->GetLegendaryWeapon(weapon_id);
    if (NULL == cell) {
      LOG_ERROR("weapon_id(%d) is invalid.", weapon_id);
      continue;
    }

    LegendaryWeaponActor::WeaponPieceVector pieces(cell->pieces_.size(), 0);

    for (size_t i = 0; i < split_result2.size() - 1 && i < pieces.size(); ++i) {
      pieces[i] = atoi(split_result2[i + 1].c_str());
    }

    result->insert(std::make_pair(weapon_id, pieces));
  }
}

static void EncodeWeaponPieces(const LegendaryWeaponActor::WeaponPiecesMap &source,
    std::string *result) {
  char buffer[1024] = "";
  size_t count = 0;

  for (LegendaryWeaponActor::WeaponPiecesMap::const_iterator iter = source.begin();
       iter != source.end(); ++iter) {
    core::int32 weapon_id = iter->first;
    const LegendaryWeaponActor::WeaponPieceVector &pieces = iter->second;

    count += snprintf(buffer + count, sizeof(buffer) - count, "%d", weapon_id);
    for (size_t i = 0; i < pieces.size(); ++i) {
      count += snprintf(buffer + count, sizeof(buffer) - count, ":%d", pieces[i]);
    }
    count += snprintf(buffer + count, sizeof(buffer) - count, ",");
  }

  *result = buffer;
}

///////////////////////////////////////////////////////////////////////////////

bool LegendaryWeaponActor::Initialize(AttrActor *attr_actor,
    const database::protocol::StorageAttrActorField &db_field) {
  if (NULL == attr_actor) {
    LOG_ERROR("attr_actor is null.");
    return false;
  }

  this->attr_actor_ = attr_actor;
  DecodeWeaponPieces(db_field.legendary_weapon_pieces_, &this->weapon_pieces_);

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  // 未开启功能直接返回
  if (game_actor->CheckFunctionalityState(
          entity::FunctionalityType::LEGENDARY_WEAPON_MODULE) == false) {
    return true;
  }

  // 增加完全修复属性
  for (WeaponPiecesMap::const_iterator iter = this->weapon_pieces_.begin();
       iter != this->weapon_pieces_.end(); ++iter) {
    this->TryAddFullRepairedAttr(iter->first, false);
  }

  // 更新战斗力
  this->UpdateLegendaryWeaponFightScore(false);

  // 发送初始化数据
  this->SendInitToClient();

  return true;
}

void LegendaryWeaponActor::Finalize() {
  this->weapon_pieces_.clear();
  this->attr_actor_ = NULL;
}

void LegendaryWeaponActor::SendInitToClient() {
  gateway::protocol::MessageAttrLegendaryWeaponInitialize message;

  for (WeaponPiecesMap::const_iterator iter = this->weapon_pieces_.begin();
       iter != this->weapon_pieces_.end(); ++iter) {
    core::int32 weapon_id = iter->first;
    const WeaponPieceVector &pieces = iter->second;

    gateway::protocol::LegendaryWeaponData data;
    data.__set_id_(weapon_id);
    for (size_t i = 0; i < pieces.size(); ++i) {
      data.pieces_.push_back(pieces[i]);
    }
    message.weapons_.push_back(data);
  }

  this->attr_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_LEGENDARY_WEAPON_INITIALIZE);
}

void LegendaryWeaponActor::Save(database::protocol::StorageAttrActorField &db_field) {
  EncodeWeaponPieces(this->weapon_pieces_, &db_field.legendary_weapon_pieces_);
}

bool LegendaryWeaponActor::EnableLegendaryWeaponFunc(bool synchronize) {
  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

core::int32 LegendaryWeaponActor::GetWeaponPiece(core::int32 weapon_id,
    core::int32 piece_index) const {
  WeaponPiecesMap::const_iterator iter = this->weapon_pieces_.find(weapon_id);
  if (iter == this->weapon_pieces_.end()) {
    return 0;
  }

  const LegendaryWeaponActor::WeaponPieceVector &pieces = iter->second;
  if (piece_index >= (core::int32)pieces.size()) {
    LOG_ERROR("piece_index is invalid.");
    return 0;
  }
  return pieces[piece_index];
}

void LegendaryWeaponActor::AddWeaponPiece(core::int32 weapon_id,
    core::int32 piece_index, core::int32 count, bool synchronize) {
  if (0 == count) {
    return;
  }

  const LegendaryWeaponCell *cell =
      Configure::GetInstance()->GetLegendaryWeapon(weapon_id);
  if (NULL == cell) {
    LOG_ERROR("weapon_id(%d) is invalid.", weapon_id);
    return;
  }
  if (piece_index >= (core::int32)cell->pieces_.size()) {
    LOG_ERROR("weapon_id(%d), piece_index(%d) is invalid.", weapon_id, piece_index);
    return;
  }

  WeaponPiecesMap::iterator iter = this->weapon_pieces_.find(weapon_id);
  if (iter == this->weapon_pieces_.end()) {
    iter = this->weapon_pieces_.insert(
        std::make_pair(weapon_id, WeaponPieceVector(cell->pieces_.size(), 0))).first;
  }
  LegendaryWeaponActor::WeaponPieceVector &pieces = iter->second;

  core::int32 diff = cell->pieces_[piece_index].item_count_ - pieces[piece_index];
  if (diff > 0) {
    pieces[piece_index] += std::min(diff, count);
    // 增加完全修复属性
    this->TryAddFullRepairedAttr(weapon_id, synchronize);

    // 同步 client
    if (synchronize) {
      gateway::protocol::MessageAttrLegendaryWeaponPieceSynchronize sync; 
      sync.__set_weapon_id_(weapon_id);
      sync.__set_piece_index_(piece_index);
      sync.__set_piece_count_(pieces[piece_index]);
      this->attr_actor_->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_ATTR_LEGENDARY_WEAPON_PIECE_SYNCHRONIZE);
    }
  }
}

void LegendaryWeaponActor::TryAddFullRepairedAttr(core::int32 weapon_id,
    bool synchronize) {
  const LegendaryWeaponCell *cell =
      Configure::GetInstance()->GetLegendaryWeapon(weapon_id);
  if (NULL == cell) {
    LOG_ERROR("weapon_id(%d) is invalid.", weapon_id);
    return;
  }

  WeaponPiecesMap::const_iterator iter = this->weapon_pieces_.find(weapon_id);
  if (iter == this->weapon_pieces_.end()) {
    return;
  }
  const LegendaryWeaponActor::WeaponPieceVector &pieces = iter->second;

  for (size_t i = 0; i < pieces.size(); ++i) {
    if (pieces[i] < cell->pieces_[i].item_count_) {
      return;
    }
  }

  // 增加属性
  this->attr_actor_->AddAttrBonus(cell->attr_bonus_id_);
}

void LegendaryWeaponActor::UpdateLegendaryWeaponFightScore(bool synchronize) {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  FightScoreCalculator calc;

  for (WeaponPiecesMap::const_iterator iter = this->weapon_pieces_.begin();
       iter != this->weapon_pieces_.end(); ++iter) {
    const LegendaryWeaponCell *cell =
        Configure::GetInstance()->GetLegendaryWeapon(iter->first);
    AttrActor::AddAttrBonusFightScore(cell->attr_bonus_id_, &calc);
  }

  game_actor->SetSubFightScore(entity::SubFightScoreType::LEGENDARY_WEAPON,
      calc.GetFightScore(), synchronize);
}

}  // namespace attr

}  // namespace server

}  // namespace game

