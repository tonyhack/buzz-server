//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 20:51:44.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/ai/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace ai {

GameRole *FacadeRequest::GetRole(entity::EntityType::type type, core::uint64 id) {
  if(type == entity::EntityType::TYPE_ACTOR) {
    return (GameRole *)GameActorManager::GetInstance()->GetActor(id);
  } else if(type == entity::EntityType::TYPE_NPC) {
    return (GameRole *)GameNpcManager::GetInstance()->Get(id);
  }

  return NULL;
}

bool FacadeRequest::Route(core::uint32 srcx, core::uint32 srcy, core::uint32 destx,
    core::uint32 desty, core::uint64 scene, entity::DirectionType::type *paths,
    size_t &number, size_t max_path_size, core::int32 radius) {
  if(srcx == destx && srcy == desty) {
    number = 0;
    return true;
  }

  request::RequestSceneRouter request;
  request.__set_scene_(scene);
  request.__set_src_x_(srcx);
  request.__set_src_y_(srcy);
  request.__set_dest_x_(destx);
  request.__set_dest_y_(desty);
  request.__set_dest_radius_(radius);
  request.__set_max_path_(max_path_size);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_ROUTER,
        &request, sizeof(request)) != 0) {
    return false;
  }

  if(request.result_ == false) {
    return false;
  }

  number = request.paths_.size();
  for(size_t pos = 0; pos < number; ++pos) {
    paths[pos] = request.paths_[pos];
  }

  return true;
}

bool FacadeRequest::SceneRelocation(entity::EntityType::type type, core::uint64 id, 
    core::uint32 x, core::uint32 y) {
  request::RequestSceneRelocation request;
  request.__set_type_(type);
  request.__set_id_(id);
  request.__set_x_(x);
  request.__set_y_(y);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_RELOCATION,
        &request, sizeof(request)) != 0) {
    return false;
  }

  if(request.result_ == false) {
    return false;
  }

  return true;
}

}  // namespace ai

}  // namespace server

}  // namespace game

