#ifndef __GAME__SERVER__ATTR__NOBLE__ACTOR__H
#define __GAME__SERVER__ATTR__NOBLE__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace database { namespace protocol {
class StorageAttrActorField;
}}

namespace game {

namespace server {

namespace attr {

class AttrActor;

class NobleActor : public core::Noncopyable {
 public:
  NobleActor();
  ~NobleActor();

  bool Initialize(AttrActor *attr_actor, const database::protocol::StorageAttrActorField &db_field);
  void Finalize();
  void Save(database::protocol::StorageAttrActorField &db_field);

  // 开启功能
  bool EnableNobleFunc(bool synchronize = true);

  inline core::int32 GetCurrentLevel() const { return this->current_level_; }
  inline core::int32 GetUsedItemCount() const { return this->used_item_count_; }
  inline core::int32 GetMaxHpAddValue() const { return this->max_hp_add_value_; }
  inline core::int32 GetMaxMpAddValue() const { return this->max_mp_add_value_; }
  inline core::int32 GetPhysicsAttackAddValue() const { return this->physics_attack_add_value_; }
  inline core::int32 GetPhysicsDefenceAddValue() const { return this->physics_defence_add_value_; }
  inline core::int32 GetMagicAttackAddValue() const { return this->magic_attack_add_value_; }
  inline core::int32 GetMagicDefenceAddValue() const { return this->magic_defence_add_value_; }
  inline core::int32 GetDodgeAddValue() const { return this->dodge_add_value_; }
  inline core::int32 GetCritAddValue() const { return this->crit_add_value_; }
  inline core::int32 GetIgnoreDefenceAddValue() const { return this->ignore_defence_add_value_; }
  bool CheckPromoteFull() const;
  void Promote(bool synchronize = true);
  void Breakthrough(core::int32 item_count, bool synchronize = true);

  void UpdateNobleFightScore(bool synchronize = true);

 private:
  void SendInitToClient();

  AttrActor *attr_actor_;
  core::int32 current_level_;
  core::int32 used_item_count_;
  core::int32 max_hp_add_value_;
  core::int32 max_mp_add_value_;
  core::int32 physics_attack_add_value_;
  core::int32 physics_defence_add_value_;
  core::int32 magic_attack_add_value_;
  core::int32 magic_defence_add_value_;
  core::int32 dodge_add_value_;
  core::int32 crit_add_value_;
  core::int32 ignore_defence_add_value_;
};

}  // namespace attr

}  // namespace game

}  // namespace game

#endif  // __GAME__SERVER__ATTR__NOBLE__ACTOR__H

