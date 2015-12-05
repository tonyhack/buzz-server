#ifndef __GAME__SERVER__ATTR__LEGENDARY__WEAPON__ACTOR__H
#define __GAME__SERVER__ATTR__LEGENDARY__WEAPON__ACTOR__H

#include <map>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace database { namespace protocol {
class StorageAttrActorField;
}}

namespace game {

namespace server {

namespace attr {

class AttrActor;

class LegendaryWeaponActor : public core::Noncopyable {
 public:
  typedef std::vector<core::int32> WeaponPieceVector;
  typedef std::map<core::int32, WeaponPieceVector> WeaponPiecesMap;

  LegendaryWeaponActor();
  ~LegendaryWeaponActor();

  bool Initialize(AttrActor *attr_actor,
      const database::protocol::StorageAttrActorField &db_field);
  void Finalize();
  void Save(database::protocol::StorageAttrActorField &db_field);

  // 开启功能
  bool EnableLegendaryWeaponFunc(bool synchronize = true);

  core::int32 GetWeaponPiece(core::int32 weapon_id,
      core::int32 piece_index) const;
  void AddWeaponPiece(core::int32 weapon_id,
      core::int32 piece_index, core::int32 count,
      bool synchronize = true);

  // 更新战斗力
  void UpdateLegendaryWeaponFightScore(bool synchronize = true);

 private:
  void SendInitToClient();
  void TryAddFullRepairedAttr(core::int32 weapon_id, bool synchronize = true);

  AttrActor *attr_actor_;
  WeaponPiecesMap weapon_pieces_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LEGENDARY__WEAPON__ACTOR__H

