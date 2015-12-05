//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-25 12:22:59.
// File name: buff_role.h
//
// Description:
// Define class BuffRole.
//

#ifndef __GAME__SERVER__BUFF__BUFF__ROLE__H
#define __GAME__SERVER__BUFF__BUFF__ROLE__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_buff_login_types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_role.h"
#include "gateway_server/protocol/gateway_buff_protocol_types.h"

namespace game {

namespace server {

namespace buff {

class Buff;
class BuffCell;

class BuffRole : public core::Noncopyable {
  typedef std::map<core::uint32, Buff *> BuffMap;
  typedef std::vector<database::protocol::StorageBuffField> BuffVector;

  typedef std::map<core::int32, core::int32> ForbidGroupMap;

 public:
  BuffRole();
  ~BuffRole();

  bool Initialize(GameRole *role);
  void Finalize();

  bool InitializeBuffs(const BuffVector &buffs);
  void ExportBuffs(std::vector<gateway::protocol::BuffField> &buffs);

  // 开启/结束 某个BUFF
  bool StartBuff(entity::EntityType::type source_type, core::int64 source_id,
      core::uint32 buff_id, core::uint32 during_time = 0);
  bool EndBuff(core::uint32 buff_id);

  // 增/减 BUFF禁止组
  void IncreaseForbidGroup(core::int32 group);
  void DecreaseForbidGroup(core::int32 group);
  // 检测BUFF组是否被禁止
  bool CheckForbidGroup(core::int32 group) const;

  bool CheckBuffConditionGroupExist(core::int32 group) const;

  // 清除组BUFF
  void ClearGroupBuff(core::int32 group);

  // 死亡时
  void OnDeath();
  // 受到伤害时
  void OnHurt();
  // 当前能量耗尽时
  void OnCurrentEnergyDepleted();

  void OnEvent(event::EventType::type type, const void *data, size_t size);

  // 保存
  void Save();

  Buff *GetBuff(core::uint32 buff_id);

  inline entity::EntityType::type GetType() const {
    if(this->role_) {
      return this->role_->GetType();
    }
    return entity::EntityType::TYPE_MAX;
  }

  inline core::uint64 GetID() const {
    if(this->role_) {
      return this->role_->GetID();
    }
    return 0;
  }

  inline GameRole *GetRole() {
    return this->role_;
  }

 private:
  bool AddBuff(Buff *buff);
  Buff *RemoveBuff(core::uint32 buff_id);

  GameRole *role_;
  BuffMap buffs_;

  ForbidGroupMap forbids_;

  static const size_t max_save_number_ = 256;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__ROLE__H

