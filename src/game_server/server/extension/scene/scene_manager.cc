//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-15 15:16:21.
// File name: scene_manager.cc
//
// Description:
// Define class SceneManager.
//

#include "game_server/server/extension/scene/scene_manager.h"

#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_pool.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {


bool SceneManager::Initialize() {
  return true;
}

void SceneManager::Finalize() {
  // 所有场景对象回收到场景池中
  SceneMap::iterator iterator = this->scenes_.begin();
  for(; iterator != this->scenes_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      ScenePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->scenes_.clear();
}

Scene *SceneManager::GetScene(core::uint64 id) {
  SceneMap::iterator iterator = this->scenes_.find(id);
  if(iterator != this->scenes_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool SceneManager::AddScene(Scene *scene) {
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 参数 scene 错误.",
            __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 检测场景管理器中是否存在
  SceneMap::iterator iterator_scene_map = this->scenes_.find(scene->GetSceneID());
  if(iterator_scene_map != this->scenes_.end()) {
    return false;
  }

  // 插入场景管理器
  this->scenes_.insert(std::make_pair(scene->GetSceneID(), scene));

  return true;
}

Scene *SceneManager::RemoveScene(core::uint64 id) {
  // 查询场景管理器
  SceneMap::iterator iterator_scene_map = this->scenes_.find(id);
  if(iterator_scene_map == this->scenes_.end()) {
    return NULL;
  }

  Scene *scene = iterator_scene_map->second;
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 从场景管理器中获取的指针为空");
    return NULL;
  }

  // 从场景管理器中删除
  this->scenes_.erase(iterator_scene_map);

  return scene;
}

core::uint64 SceneManager::GenerateStaticSceneID(core::uint16 map_id,
    core::uint16 branch_id) {
  core::uint64 scene_id = 0;
  this->GenerateSceneID(map_id, branch_id, scene_id);
  return scene_id;
}

core::uint64 SceneManager::AllocateSceneID(core::uint16 map_id) {
  core::uint16 sequence = 0;
  if(this->sequences_.Allocate(sequence) == false) {
    return 0;
  }
  if(sequence == 0) {
    if(this->sequences_.Allocate(sequence) == false) {
      this->sequences_.Deallocate(0);
      return 0;
    }
    this->sequences_.Deallocate(0);
  }
  core::uint64 scene_id = 0;
  this->GenerateSceneID(map_id, sequence, scene_id);
  return scene_id;
}

void SceneManager::DeallocateSceneID(core::uint64 scene_id) {
  this->sequences_.Deallocate(global::GameMisc::GetSceneSequenceID(scene_id));
}

void SceneManager::GenerateSceneID(core::uint16 map_id, core::uint16 sequence,
    core::uint64 &scene_id) {
  scene_id = global::GameMisc::MakeSceneID(
      (core::uint64)GameServerSingleton::GetInstance().GetSignature().id_,
      (core::uint64)sequence, map_id);
}

}  // namespace scene

}  // namespace server

}  // namespace game

