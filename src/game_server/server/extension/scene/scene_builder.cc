//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:49:07.
// File name: scene_builder.cc
//
// Description:
// Define class SceneBuilder.
//

#include "game_server/server/extension/scene/scene_builder.h"

#include "game_server/server/session_channel.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_pool.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_protocol_types.h"

namespace game {

namespace server {

namespace scene {

Scene *SceneBuilder::CreateStatic(core::uint32 map, core::uint16 branch_id) {
  Scene *scene = ScenePool::GetInstance()->Allocate(map);
  if(scene == NULL) {
    LOG_ERROR("分配 Scene对象 失败，地图(%u)", map);
    return NULL;
  }
  if(scene->Initialize(SceneManager::GetInstance()->GenerateStaticSceneID(
          map, branch_id), false) == false) {
    LOG_ERROR("初始化 Scene对象 失败，地图(%u)", map);
    ScenePool::GetInstance()->Deallocate(scene);
    return NULL;
  }
  if(SceneManager::GetInstance()->AddScene(scene) == false) {
    LOG_ERROR("加入 SceneManager 失败，地图(%u)", map);
    scene->Finalize();
    ScenePool::GetInstance()->Deallocate(scene);
    return NULL;
  }

  if (branch_id > 0) {
    // 场景登陆到会话服务器
    session::protocol::MessageSceneLoginRequest message;
    message.__set_scene_(scene->GetSceneID());
    message.__set_dynamic_(false);
    SessionChannel::SendMessage(message,
        session::protocol::GameMessageType::MESSAGE_SCENE_LOGIN_REQUEST);
  }

  return scene;
}

Scene *SceneBuilder::CreateDynamic(core::uint32 map) {
  core::uint64 scene_id = SceneManager::GetInstance()->AllocateSceneID(map);
  if(scene_id == 0) {
    LOG_ERROR("分配 场景ID 失败，地图(%u)", map);
    return NULL;
  }

  // 分配 Scene 对象
  Scene *scene = ScenePool::GetInstance()->Allocate(map);
  if(scene == NULL) {
    LOG_ERROR("分配 Scene对象 失败，地图(%u)", map);
    SceneManager::GetInstance()->DeallocateSceneID(scene_id);
    return NULL;
  }

  // 初始化 Scene 对象
  if(scene->Initialize(scene_id, true) == false) {
    LOG_ERROR("初始化 Scene对象 失败，地图(%u)", map);
    ScenePool::GetInstance()->Deallocate(scene);
    SceneManager::GetInstance()->DeallocateSceneID(scene_id);
    return NULL;
  }

  // 加入管理器
  if(SceneManager::GetInstance()->AddScene(scene) == false) {
    LOG_ERROR("加入 SceneManager 失败，地图(%u)", map);
    scene->Finalize();
    ScenePool::GetInstance()->Deallocate(scene);
    SceneManager::GetInstance()->DeallocateSceneID(scene_id);
    return NULL;
  }

  // 场景登陆到会话服务器
  session::protocol::MessageSceneLoginRequest message;
  message.__set_scene_(scene->GetSceneID());
  message.__set_dynamic_(true);
  SessionChannel::SendMessage(message,
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGIN_REQUEST);

  return scene;
}

void SceneBuilder::Destory(Scene *scene) {
  if(scene == NULL) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 场景登出到会话服务器
  session::protocol::MessageSceneLogoutRequest request;
  request.__set_scene_(scene->GetSceneID());
  SessionChannel::SendMessage(request,
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGOUT_REQUEST);

  // 动态场景需要释放场景ID
  if(scene->CheckDynamic()) {
    SceneManager::GetInstance()->DeallocateSceneID(scene->GetSceneID());
  }

  // 从管理器中删除，并释放对象到对象池
  core::uint64 scene_id = scene->GetSceneID();
  scene->Finalize();
  SceneManager::GetInstance()->RemoveScene(scene_id);
  ScenePool::GetInstance()->Deallocate(scene);
}

}  // namespace scene

}  // namespace server

}  // namespace game

