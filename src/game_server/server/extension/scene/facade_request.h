//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-21 12:29:05.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__SCENE__FACADE__REQUEST__H
#define __GAME__SERVER__SCENE__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "entity/item_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GameRole;

namespace scene {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  GameRole *GetRole(entity::EntityType::type type, core::uint64 id);

  // 获取玩家公会ID
  core::uint64 GetGuildID(core::uint64 actor);

  // 获取玩家组队ID
  core::uint64 GetTeamID(core::uint64 actor);

  // 检测是否可加入道具
  bool CheckAddItem(core::uint64 actor, core::int32 template_id,
      core::int32 number, entity::BindRequireType::type bind);

  bool AddItem(core::uint64 actor, core::int32 template_id,
      core::int32 number, entity::BindRequireType::type bind);

  bool RemmoveItem(core::uint64 actor, core::int32 template_id,
      core::int32 number, entity::BindRequireType::type bind);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__FACADE__REQUEST__H

