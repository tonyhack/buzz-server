//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 20:48:27.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__AI__FACADE__REQUEST__H
#define __GAME__SERVER__AI__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/direction_types.h"
#include "entity/entity_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GameRole;

namespace ai {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  GameRole *GetRole(entity::EntityType::type type, core::uint64 id);

  bool Route(core::uint32 srcx, core::uint32 srcy, core::uint32 destx,
      core::uint32 desty, core::uint64 scene, entity::DirectionType::type *paths,
      size_t &number, size_t max_path_size, core::int32 radius);

  bool SceneRelocation(entity::EntityType::type type, core::uint64 id, 
      core::uint32 x, core::uint32 y);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__FACADE__REQUEST__H

