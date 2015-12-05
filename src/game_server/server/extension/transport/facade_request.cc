//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-10 21:02:46.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/transport/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_ai_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace transport {

core::uint64 FacadeRequest::SummonNpc(core::uint64 scene, core::int32 npc_template,
    core::int32 x, core::int32 y, core::int32 width, core::int32 heigth,
    entity::DirectionType::type dir) {
  request::RequestSceneSummonNpc request;
  request.__set_scene_(scene);
  request.__set_template_id_(npc_template);
  request.__set_x_(x);
  request.__set_y_(y);
  request.__set_width_(width);
  request.__set_heigth_(heigth);
  request.__set_dir_(dir);

  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_SUMMON_NPC,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求 REQUEST_SCENE_SUMMON_NPC 失败");
    return 0;
  }

  return request.npc_id_;
}

bool FacadeRequest::RemoveNpc(core::uint64 npc_id) {
  request::RequestSceneClearNpc request;
  request.__set_id_(npc_id);
  request.__set_delay_secs_(0);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_CLEAR_NPC,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求 REQUEST_SCENE_CLEAR_NPC 失败");
    return false;
  }

  return true;
}

bool FacadeRequest::SetNpcTarget(core::uint64 npc_id, entity::EntityType::type target_type,
    core::uint64 target_id, bool force_lock) {
  request::RequestAiSetTarget request;
  request.__set_id_(npc_id);
  request.__set_type_(entity::EntityType::TYPE_NPC);
  request.__set_target_id_(target_id);
  request.__set_target_type_(target_type);
  request.__set_force_lock_(force_lock);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_AI_SET_TARGET,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求 REQUEST_AI_SET_TARGET 失败");
    return false;
  }

  return true;
}

bool FacadeRequest::JumpScene(entity::EntityType::type type, core::uint64 id,
    core::uint64 scene_id, core::int32 x, core::int32 y) {
  request::RequestSceneJump request;
  request.__set_type_(type);
  request.__set_id_(id);
  request.__set_scene_(scene_id);
  request.__set_x_(x);
  request.__set_y_(y);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_JUMP,
        &request, sizeof(request)) == -1 || request.result_ == false) {
    LOG_ERROR("请求 REQUEST_SCENE_JUMP 失败");
    return false;
  }

  return true;
}

}  // namespace transport

}  // namespace server

}  // namespace game

