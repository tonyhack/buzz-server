//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 14:29:00.
// File name: soul_actor.h
//
// Description:
// Define class SoulActor.
//

#ifndef __GAME__SERVER__SOUL__SOUL__ACTOR__H
#define __GAME__SERVER__SOUL__SOUL__ACTOR__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_soul_actor_login_types.h"
#include "database_server/protocol/storage_soul_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/soul/soul_altar_actor.h"
#include "game_server/server/extension/soul/soul_energy.h"
#include "game_server/server/extension/soul/soul_setting.h"
#include "game_server/server/extension/soul/soul_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"

namespace game {

namespace server {

namespace soul {

class Soul;
class SoulSetting;

class SoulActor : public core::Noncopyable {
 public:
  typedef std::map<core::uint32, Soul *> SoulMap;
  typedef std::vector<database::protocol::StorageSoulField> SoulVector;
  typedef std::pair<entity::VocationType::type, core::int32> SoulSettingKey;
  typedef std::map<SoulSettingKey, SoulSetting> SoulSettingMap;

  SoulActor();
  ~SoulActor();

  bool Initialize(GameActor *actor,
      const database::protocol::StorageSoulActorField &field);
  void Finalize();

  inline GameActor *GetActor() { return this->actor_; }
  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return -1;
  }
  inline const std::string GetStringID() const {
    if(this->actor_) {
      return this->actor_->GetStringID();
    }
    return std::string();
  }
  inline bool CheckSoulFunctionStatus() const {
    if(this->actor_) {
      return this->actor_->CheckFunctionalityState(
          entity::FunctionalityType::SOUL_MODULE);
    }
    return false;
  }
  inline void SetFightingStatus(bool status) { this->fighting_status_ = status; }
  inline bool GetFightingStatus() const { return this->fighting_status_; }
  inline SoulMap &GetSouls() { return this->souls_; }
  inline const SoulMap &GetSouls() const { return this->souls_; }

  inline void SetCurrentSetting(entity::VocationType::type vocation,
      core::int32 group) {
    this->current_setting_.first = vocation;
    this->current_setting_.second = group;
  }
  inline SoulSetting *GetCurrentSetting() {
    return this->GetSetting(current_setting_.first, current_setting_.second);
  }
  inline const SoulSetting *GetCurrentSetting() const {
    return this->GetSetting(current_setting_.first, current_setting_.second);
  }

  // 英灵祭坛
  SoulAltarActor *GetSoulAltarActor() { return &this->soul_altar_actor_; }

  // 能量
  SoulEnergy *GetSoulEnergy() { return &this->soul_energy_; }

  bool CheckLoadFinish() const { return this->load_finish_; }

  template <typename T>
  inline void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if(this->actor_) {
      this->actor_->SendMessage(message, type);
    }
  }

  bool ImportSouls(const SoulVector &souls);
  void Synchronize();
  void Save();
  bool LoadFinish();

  bool AddSoul(Soul *soul);
  Soul *RemoveSoul(core::int32 soul_id);
  Soul *GetSoul(core::int32 soul_id);

  // 快捷设置
  SoulSetting *GetSetting(entity::VocationType::type vocation,
      core::int32 group);
  const SoulSetting *GetSetting(entity::VocationType::type vocation,
      core::int32 group) const;

  // 更新英灵图鉴解锁属性战斗力
  void UpdateSoulFightScore(bool sync = true);

  void DailyClean();

 private:
  // 玩家对象
  GameActor *actor_;
  // 英灵表
  SoulMap souls_;
  // 战斗状态
  bool fighting_status_;
  // 英灵祭坛
  SoulAltarActor soul_altar_actor_;

  // 设置
  SoulSettingMap settings_;
  SoulSettingKey current_setting_;
  core::int32 axe_stand_group_;
  core::int32 shoot_stand_group_;
  core::int32 magic_stand_group_;

  std::string setting_fields_;

  // 能量
  SoulEnergy soul_energy_;

  bool load_finish_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__ACTOR__H

