#ifndef __GAME__SERVER__ATTR__ATTR__ACTOR__H
#define __GAME__SERVER__ATTR__ATTR__ACTOR__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_attr_actor_login_types.h"
#include "database_server/protocol/storage_spirit_facade_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/attr/noble_actor.h"
#include "game_server/server/extension/attr/sword_actor.h"
#include "game_server/server/extension/attr/element_actor.h"
#include "game_server/server/extension/attr/spirit_actor.h"
#include "game_server/server/extension/attr/rune_actor.h"
#include "game_server/server/extension/attr/legendary_weapon_actor.h"

namespace game { namespace server {
class FightScoreCalculator;
}}

namespace game {

namespace server {

namespace attr {

class AttrActor : public core::Noncopyable {
 public:
  typedef std::vector<database::protocol::StorageSpiritFacadeField> SpiritFacadeFieldVector;

  AttrActor();
  ~AttrActor();

  bool Initialize(GameActor *game_actor,
      const database::protocol::StorageAttrActorField &db_field);
  bool InitializeSpiritActor(const SpiritFacadeFieldVector& facades);
  void Finalize();
  bool CheckLoadFinish() const;
  void SendLoadFinishEvent();
  void Save();

  inline GameActor *GetActor() {
    return this->game_actor_;
  }

  inline core::uint64 GetID() const {
    if (this->game_actor_) {
      return this->game_actor_->GetID();
    }
    return -1;
  }

  inline const std::string GetStringID() const {
    if (this->game_actor_) {
      return this->game_actor_->GetStringID();
    }
    return std::string();
  }
  
  inline const std::string GetName() const {
    if (this->game_actor_) {
      return this->game_actor_->GetName();
    }
    return std::string();
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if (this->game_actor_) {
      this->game_actor_->SendMessage(message, type);
    }
  }

  NobleActor *GetNobleActor() {
    return &this->noble_actor_;
  }

  SwordActor *GetSwordActor() {
    return &this->sword_actor_;
  }

  ElementActor *GetElementActor() {
    return &this->element_actor_;
  }

  SpiritActor *GetSpiritActor() {
    return &this->spirit_actor_;
  }
  
  RuneActor *GetRuneActor() {
    return &this->rune_actor_;
  }

  LegendaryWeaponActor *GetLegendaryWeaponActor() {
    return &this->legendary_weapon_actor_;
  }

  void DailyClean();

  // 增加属性奖励
  void AddAttrBonus(core::int32 attr_bonus_id, bool synchronize = true);
  // 移除属性奖励
  void RemoveAttrBonus(core::int32 attr_bonus_id, bool synchronize = true);
  // 计算属性奖励战斗力
  static void AddAttrBonusFightScore(core::int32 attr_bonus_id, FightScoreCalculator *calc);

 private:
  GameActor *game_actor_;
  database::protocol::StorageAttrActorField db_field_;
  NobleActor noble_actor_;
  SwordActor sword_actor_;
  ElementActor element_actor_;
  SpiritActor spirit_actor_;
  RuneActor rune_actor_;
  LegendaryWeaponActor legendary_weapon_actor_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ATTR__ACTOR__H

