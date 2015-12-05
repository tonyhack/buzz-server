//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-12 13:55:00.
// File name: scene_item.h
//
// Description:
// Define class SceneItem.
//

#ifndef __GAME__SERVER__SCENE__SCENE__ITEM__H
#define __GAME__SERVER__SCENE__SCENE__ITEM__H

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/npc_types.h"
#include "entity/scene_item_types.h"
#include "game_server/server/game_entity.h"

namespace game {

namespace server {

namespace scene {

class Scene;

class SceneItem : public GameEntity, public core::TimerHandler {
  enum { TIMER_ID_DISAPPEAR = 0, TIMER_ID_CLEAR_OWNER,};

  typedef std::vector<int32_t> AttributeFields;
  typedef void (*AttributeReviseFunctor)(core::int32 &value);

 public:
  typedef std::vector<core::uint64> OwnerVector;

  SceneItem();
  virtual ~SceneItem();

  void Allocate(core::uint64 id);
  void Deallocate();

  bool Initialize(core::uint32 template_id);
  void Finalize();

  core::int32 GetAttribute(entity::SceneItemAoiFields::type type) const;
  bool SetAttribute(entity::SceneItemAoiFields::type type,
      core::int32 value, bool synchronize = true);
  
  // 设置归属类型
  void SetOwnerType(entity::NpcOwnerType::type type) {
    this->owner_type_ = type;
  }
  // 增加所属者
  void AddOwner(core::uint64 actor, bool synchronize = true);
  // 清除所属者
  void ClearOwner(bool synchronize = true);
  // 判断归属者
  bool CheckOwner(core::uint64 actor) const;

  const OwnerVector &GetOwners() const {
    return this->owners_;
  }

  inline void SetItemScene(Scene *scene) {
    this->scene_ = scene;
  }
  inline Scene *GetItemScene() const {
    return this->scene_;
  }

  inline const AttributeFields &GetSceneItemAoiAttribute() const {
    return this->aoi_attribute_;
  }

  virtual entity::EntityType::type GetType() const {
    return entity::EntityType::TYPE_SCENE_ITEM;
  }

  virtual entity::NpcOwnerType::type GetOwnerType() const {
    return this->owner_type_;
  }

  virtual void Synchronize();
  virtual void Synchronize(entity::SceneItemAoiFields::type type);
  virtual void Synchronize(entity::EntityClientFields::type type);

  virtual void OnTimer(core::uint32 id);

 private:
  void ReviseAttribute(entity::SceneItemAoiFields::type type);

  void OnTimerDisappear();
  void OnTimerClearOwner();

  AttributeFields aoi_attribute_;

  Scene *scene_;

  OwnerVector owners_;

  entity::NpcOwnerType::type owner_type_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__ITEM__H

