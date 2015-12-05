//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 05:14:15.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__TRANSPORT__FACADE__REQUEST__H
#define __GAME__SERVER__TRANSPORT__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/direction_types.h"
#include "entity/entity_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace transport {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  core::uint64 SummonNpc(core::uint64 scene, core::int32 npc_template,
      core::int32 x, core::int32 y, core::int32 width, core::int32 heigth,
      entity::DirectionType::type dir);

  bool RemoveNpc(core::uint64 npc_id);

  bool SetNpcTarget(core::uint64 npc_id, entity::EntityType::type target_type,
      core::uint64 target_id, bool force_lock = false);

  bool JumpScene(entity::EntityType::type type, core::uint64 id,
      core::uint64 scene_id, core::int32 x, core::int32 y);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__FACADE__REQUEST__H

