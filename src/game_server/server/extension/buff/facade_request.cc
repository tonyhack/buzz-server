//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-01 13:55:23.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/buff/facade_request.h"

#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"

namespace game {

namespace server {

namespace buff {

BuffRole *FacadeRequest::GetBuffRole(entity::EntityType::type type,
    core::uint64 id) {
  if(type == entity::EntityType::TYPE_ACTOR) {
    return BuffRoleManager::GetInstance()->GetActor(id);
  } else if(type == entity::EntityType::TYPE_NPC) {
    return BuffRoleManager::GetInstance()->GetNpc(id);
  }
  return NULL;
}

}  // namespace buff

}  // namespace server

}  // namespace game

