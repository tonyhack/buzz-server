//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-24 10:36:04.
// File name: scene_npc.h
//
// Description:
// Define class SceneNpc.
//

#ifndef __GAME__SERVER__SCENE__SCENE__NPC__H
#define __GAME__SERVER__SCENE__SCENE__NPC__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/direction_types.h"
#include "game_server/server/extension/scene/ownership_all.h"
#include "game_server/server/extension/scene/ownership_camp_compete.h"
#include "game_server/server/extension/scene/ownership_camp_each.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/extension/scene/ownership_damage.h"
#include "game_server/server/extension/scene/ownership_kill.h"

namespace game {

namespace server {

class GameRole;

namespace scene {

class Scene;

class SceneNpc : public core::Noncopyable {
 public:
  SceneNpc();
  ~SceneNpc();

  bool Initialize(GameNpc *npc, bool need_reborn,
      core::uint32 reborn_secs);
  void Finalize();

  inline void SetScene(Scene *scene) {
    this->scene_ = scene;
  }
  inline Scene *GetScene() {
    return this->scene_;
  }
  inline GameNpc *GetGameNpc() {
    return this->npc_;
  }

  inline core::uint64 GetID() {
    if(this->npc_) {
      return this->npc_->GetID();
    }
    return 0;
  }

  // 是否需要重生
  inline bool CheckNeedReborn() const {
    return this->need_reborn_;
  }

  // 获取重生时间
  inline core::uint32 GetRebornSecs() const {
    return this->reborn_secs_;
  }

  // 获取出生坐标
  inline void GetBornPosition(core::uint32 &x, core::uint32 &y) {
    if(this->npc_ != NULL) {
      this->npc_->GetBornPosition(x, y);
    }
  }

  // 设置出生坐标
  inline void SetBornPosition(core::uint32 x, core::uint32 y) {
    if(this->npc_ != NULL) {
      this->npc_->SetBornPosition(x, y);
    }
  }

  // 获取出生面向
  inline entity::DirectionType::type GetBornDir() const {
    return this->born_dir_;
  }

  inline bool GetForbidDropItems() const {
    return this->forbid_drop_items_;
  }
  inline void SetForbidDropItems(bool value) {
    this->forbid_drop_items_ = value;
  }

  // 掉落
  void DropItems(GameRole *owner);

  // NPC经验
  void LoseExp(GameRole *owner);

  // 事件触发
  void OnEvent(event::EventType::type type, const void *message, size_t size);

 private:
  void DropItemsOwner(core::uint64 owner);

  // 所在场景
  Scene *scene_;

  // npc对象
  GameNpc *npc_;

  // 是否需要重生
  bool need_reborn_;

  // 出生信息(重生时间、面向)
  core::uint32 reborn_secs_;
  entity::DirectionType::type born_dir_;

  // 禁止掉落
  bool forbid_drop_items_;

  // 归属
  Ownership *ownership_;
  OwnershipAll ownership_all_;
  OwnershipKill ownership_kill_;
  OwnershipDamage ownership_damage_;
  OwnershipCampCompete ownership_camp_compete_;
  OwnershipCampEach ownership_camp_each_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__NPC__H

