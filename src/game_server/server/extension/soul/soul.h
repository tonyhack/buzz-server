//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 10:49:02.
// File name: soul.h
//
// Description:
// Define class Soul.
//

#ifndef __GAME__SERVER__SOUL__SOUL__H
#define __GAME__SERVER__SOUL__SOUL__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/vocation_types.h"
#include "game_server/server/extension/soul/soul_configure.h"

namespace game {

namespace server {

namespace soul {

class SoulActor;

class Soul : public core::Noncopyable {
 public:
  Soul();
  ~Soul();

  bool Initialize(SoulActor *actor, core::int32 soul_id);
  void Finalize();

  inline core::int32 GetID() const { return this->id_; } 
  inline core::int32 GetStep() const { return this->step_; }
  inline void SetStep(core::int32 step) { this->step_ = step; }

  inline core::int32 GetLevel() const { return this->level_; }
  inline void SetLevel(core::int32 level) { this->level_ = level; }

  inline core::int32 GetCommonSkillLevel() const { return this->common_skill_level_; }
  inline core::int32 GetAppearSkillLevel() const { return this->appear_skill_level_; }
  inline core::int32 GetSoulSkillLevel() const { return this->soul_skill_level_; }
  inline void SetCommonSkillLevel(core::int32 level) { this->common_skill_level_ = level; }
  inline void SetAppearSkillLevel(core::int32 level) { this->appear_skill_level_ = level; }
  inline void SetSoulSkillLevel(core::int32 level) { this->soul_skill_level_ = level; }

  inline bool GetStandStatus() const { return this->stand_status_; }
  inline void SetStandStatus(bool status) { this->stand_status_ = status; }
  inline bool GetSettingStatus() const { return this->setting_status_; }
  inline void SetSettingStatus(bool status) { this->setting_status_ = status; }

  void Synchronize();

  bool Stand();
  bool Sit();

  bool StartStandEffect(const SoulEffectValue &effect);
  bool StopStandEffect(const SoulEffectValue &effect);

  bool AddAttr(entity::SoulAttributeType::type type, core::int32 increament);
  core::int32 GetMaxSkillLevel() const;

 private:
  bool AddEffect(core::int32 effect);
  bool RemoveEffect(core::int32 effect);

  SoulActor *actor_;

  core::int32 id_;
  core::int32 step_;
  core::int32 level_;
  core::int32 common_skill_level_;
  core::int32 appear_skill_level_;
  core::int32 soul_skill_level_;

  bool stand_status_;
  bool setting_status_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__H

