//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 13:56:26.
// File name: request_role_move.cc
//
// Description:
// Define class RequestRoleMove.
//

#include "game_server/server/extension/scene/request_role_move.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestRoleMove::RequestRoleMove() {}
RequestRoleMove::~RequestRoleMove() {}

bool RequestRoleMove::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_ROLE_MOVE,
      boost::bind(&RequestRoleMove::OnRequest, this, _1, _2));
  return true;
}

void RequestRoleMove::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_ROLE_MOVE);
}

int RequestRoleMove::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneRoleMove) != size) {
    global::LogError("%s:%d (%s) 请求角色移动时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneRoleMove *request = (request::RequestSceneRoleMove *)message;

  GameRole *role = NULL;
  if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = (GameRole *)GameNpcManager::GetInstance()->Get(request->id_);
  }

  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求角色移动时，找不到 role(%u, %lu)",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(role->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求角色移动时，找不到 role(%u, %lu) 所在场景(%lu)",
        __FILE__, __LINE__, __FUNCTION__, request->type_,
        request->id_, role->GetScene());
    return -1;
  }

  if(request->dir_ == entity::DirectionType::NONE) {
    entity::DirectionType::type dir = (entity::DirectionType::type)
      (ExtensionManager::GetInstance()->GetRandom().Random(entity::DirectionType::NONE));
    for(core::int32 count = 0; count < entity::DirectionType::NONE; ++count) {
      core::int32 count_dir = (count + dir) % entity::DirectionType::NONE;
      core::int32 offset_x = entity::g_constants_constants.kDirectionOffsetX[(entity::DirectionType::type)count_dir];
      core::int32 offset_y = entity::g_constants_constants.kDirectionOffsetY[(entity::DirectionType::type)count_dir];
      if(((role->GetType() == entity::EntityType::TYPE_NPC &&
            scene->CheckNpcWalkBlock(role->GetPosx() + offset_x, role->GetPosy() + offset_y) == false) ||
          (role->GetType() != entity::EntityType::TYPE_NPC &&
           scene->CheckWalkBlock(role->GetPosx() + offset_x, role->GetPosy() + offset_y) == false)) &&
          scene->Walk(role, (entity::DirectionType::type)count_dir) == true) {
        break;
      }
    }
  } else {
    if(scene->Walk(role, request->dir_) == false) {
      request->__set_result_(request::MoveResultType::FAILURE_BLOCK);
    }
  }

  request->__set_result_(request::MoveResultType::SUCCESS);
  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

