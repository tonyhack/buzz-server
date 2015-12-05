//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-18 14:53:46.
// File name: entity_builder.cc
//
// Description:
// Define class EntityBuilder.
//

#include "game_server/server/extension/scene/entity_builder.h"

#include "game_server/server/extension/scene/scene.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

EntityBuilder::EntityBuilder(Scene *scene) : scene_(scene) {}
EntityBuilder::~EntityBuilder() {}

bool EntityBuilder::Initialize() {
  if(this->scene_ == NULL) {
    global::LogError("%s:%d (%s) 场景指针为 NULL，初始化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 初始化NPC创建器
  if(this->npc_builder_.Initialize(this->scene_) == false) {
    global::LogError("%s:%d (%s) 初始化 npc创建器 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 召唤间隔定时器
  GameServerSingleton::GetInstance().AddTimer(TIMER_ENTITY_SUMMON,
      SUMMON_INTERVAL_TIME, -1, this, "EntityBuilder::Initialize");

  // 检测消失定时器
  GameServerSingleton::GetInstance().AddTimer(TIMER_ENTITY_DISAPPEAR,
      DISAPPEAR_INTERVAL_TIME, -1, this, "EntityBuilder::Initialize");

  return true;
}

void EntityBuilder::Finalize() {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ENTITY_SUMMON, this);
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ENTITY_DISAPPEAR, this);
  this->npc_builder_.Finalize();
}

void EntityBuilder::OnTimer(core::uint32 id) {
  if(id == TIMER_ENTITY_SUMMON) {
    this->OnBorn();
  } else if(id == TIMER_ENTITY_DISAPPEAR) {
    this->OnDisappear();
  }
}

GameNpc *EntityBuilder::SummonNpc(core::uint32 template_id, core::uint32 x,
    core::uint32 y, core::int32 width, core::int32 heigth, 
    entity::DirectionType::type dir, core::int32 delay) {
  core::int32 result_x = 0;
  core::int32 result_y = 0;
  if(width != 0 || heigth != 0) {
    this->npc_builder_.RandCoordinate(x, y, width, heigth, result_x, result_y);
  } else {
    result_x = x;
    result_y = y;
  }
  return this->npc_builder_.SummonNpc(template_id, result_x, result_y, dir, delay);
}

bool EntityBuilder::DisappearNpc(core::uint64 id, core::uint32 disappear_delay) {
  return this->npc_builder_.DisappearNpc(id, disappear_delay);
}

void EntityBuilder::OnBorn() {
  this->npc_builder_.BatchBorn();
}

void EntityBuilder::OnDisappear() {
  this->npc_builder_.BathchDisappear();
}

}  // namespace scene

}  // namespace server

}  // namespace game

