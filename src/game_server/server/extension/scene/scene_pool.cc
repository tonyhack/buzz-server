//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-14 18:03:35.
// File name: scene_pool.cc
//
// Description:
// Define class ScenePool.
//

#include "game_server/server/extension/scene/scene_pool.h"

#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/game_server.h"

namespace game {

namespace server {

namespace scene {

bool ScenePool::Initialize() {
  // 定时器设置对象池缩减时间
  GameServerSingleton::GetInstance().AddTimer(0, this->check_deallocate_time_ * 1000,
      -1, this, "ScenePool::Initialize");

  return true;
}

void ScenePool::Finalize() {
  // 取消定时器
  GameServerSingleton::GetInstance().RemoveTimer(0, this);
  // 释放所有池中的对象
  SceneListMap::iterator iterator = this->scene_lists_.begin();
  for(; iterator != this->scene_lists_.end(); ++iterator) {
    SceneList::iterator iterator2 = iterator->second.begin();
    for(; iterator2 != iterator->second.end(); ++iterator2) {
      iterator2->scene_->Deallocate();
      delete iterator2->scene_;
    }
  }
  this->scene_lists_.clear();
}

Scene *ScenePool::Allocate(core::uint16 map) {
  Scene *scene = NULL;
  SceneListMap::iterator iterator = this->scene_lists_.find(map);
  if(iterator != this->scene_lists_.end()) {
    SceneList::iterator iterator2 = iterator->second.begin();
    if(iterator2 != iterator->second.end()) {
      scene = iterator2->scene_;
      iterator->second.erase(iterator2);
      return scene;
    }
  }
  // 不在对象池列表中，分配
  scene = new (std::nothrow) Scene();
  if(scene) {
    if(scene->Allocate(map) == false) {
      delete scene;
      scene = NULL;
    }
  }
  return scene;
}

void ScenePool::Deallocate(Scene *scene) {
  SceneInfo info;
  info.scene_ = scene;
  info.time_ = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  SceneListMap::iterator iterator = this->scene_lists_.find(scene->GetMapID());
  if(iterator == this->scene_lists_.end()) {
    SceneList scenes;
    scenes.push_back(info);
    this->scene_lists_.insert(std::make_pair(scene->GetMapID(), scenes));
  } else {
    iterator->second.push_back(info);
  }
}

void ScenePool::OnTimer(core::uint32 id) {
  this->Reduce();
}

ScenePool::ScenePool() {}
ScenePool::~ScenePool() {}

void ScenePool::Reduce() {
  time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  SceneListMap::iterator iterator_map = this->scene_lists_.begin();
  for(; iterator_map != this->scene_lists_.end(); ++iterator_map) {
    SceneList::iterator iterator_list = iterator_map->second.begin();
    for(; iterator_list != iterator_map->second.end();) {
      if(iterator_list->time_ + this->deallocate_time_ >= now) {
        iterator_list->scene_->Deallocate();
        delete iterator_list->scene_;
        iterator_list = iterator_map->second.erase(iterator_list);
      } else {
        ++iterator_list;
      }
    }
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

