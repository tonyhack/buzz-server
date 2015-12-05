//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-21 12:18:37.
// File name: request_select_targets.cc
//
// Description:
// Define class RequestSelectTargets.
//

#include "game_server/server/extension/scene/request_select_targets.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/scene/entity_enumerator.h"
#include "game_server/server/extension/scene/facade_request.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

RequestSelectTargets::RequestSelectTargets() {}
RequestSelectTargets::~RequestSelectTargets() {}

bool RequestSelectTargets::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_SELECT_TARGETS,
      boost::bind(&RequestSelectTargets::OnRequest, this, _1, _2));

  return true;
}

void RequestSelectTargets::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_SELECT_TARGETS);
}

int RequestSelectTargets::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneSelectTargets) != size) {
    global::LogError("%s:%d (%s) 请求选择目标时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneSelectTargets *request =
      (request::RequestSceneSelectTargets *)message;

  // 获取角色对象
  GameRole *role = FacadeRequest::GetInstance()->GetRole(
      request->type_, request->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求选择目标时，获取角色(%u,%lu)对象失败",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  // 获取场景对象
  Scene *scene = SceneManager::GetInstance()->GetScene(role->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求选择目标时，获取场景(%lu)对象失败",
        __FILE__, __LINE__, __FUNCTION__, role->GetScene());
    return -1;
  }

  if(request->area_type_ == request::AreaType::AREA_CIRCLE) {
    // 圆形区域
    EntityFilter::GetInstance()->SetRole(role);
    EntityFilter::GetInstance()->SetFilterMask(request->select_mask_);
    EntityFilter::GetInstance()->SetScene(scene);

    CircleEnumberator enumberator;
    enumberator.SetMaxCount(request->max_count_);
    enumberator.SetEntityFilter(EntityFilter::GetInstance());
    if(enumberator.Initialize(request->cond_circle_.radius_) == false) {
      global::LogError("%s:%d (%s) 初始化 CircleEnumberator 失败",
          __FILE__, __LINE__, __FUNCTION__);
      return -1;
    }

    enumberator.Execute(scene, request->x_, request->y_);
    request::AreaRolePair role_pair;
    const EntityFilter::EntityVector &entities = EntityFilter::GetInstance()->GetEntitys();
    for(size_t pos = 0; pos < entities.size(); ++pos) {
      role_pair.id_ = entities[pos].first;
      role_pair.type_ = entities[pos].second;
      request->targets_.push_back(role_pair);
    }

    return 0;
  } else if(request->area_type_ == request::AreaType::AREA_SECTOR_QUARTER) {
    // 度扇形
    EntityFilter::GetInstance()->SetRole(role);
    EntityFilter::GetInstance()->SetFilterMask(request->select_mask_);
    EntityFilter::GetInstance()->SetScene(scene);

    if(request->cond_sector_.radian_ == request::SectorRadianType::QUARTER) {
      SectorQuarterEnumberator enumberator;
      enumberator.SetMaxCount(request->max_count_);
      enumberator.SetEntityFilter(EntityFilter::GetInstance());
      if(enumberator.Initialize(request->cond_sector_.radius_,
            request->cond_sector_.dir_) == false) {
        LOG_ERROR("初始化 SectorQuarterEnumberator 失败");
        return -1;
      }
      enumberator.Execute(scene, request->x_, request->y_);
    } else if(request->cond_sector_.radian_ == request::SectorRadianType::HALF) {
      SectorHalfEnumberator enumberator;
      enumberator.SetMaxCount(request->max_count_);
      enumberator.SetEntityFilter(EntityFilter::GetInstance());
      if(enumberator.Initialize(request->cond_sector_.radius_,
            request->cond_sector_.dir_) == false) {
        LOG_ERROR("初始化 SectorQuarterEnumberator 失败");
        return -1;
      }
      enumberator.Execute(scene, request->x_, request->y_);
    } else {
      LOG_ERROR("错误的弧度类型(%d)", request->cond_sector_.radian_);
      return -1;
    }

    request::AreaRolePair role_pair;
    const EntityFilter::EntityVector &entities = EntityFilter::GetInstance()->GetEntitys();
    for(size_t pos = 0; pos < entities.size(); ++pos) {
      role_pair.id_ = entities[pos].first;
      role_pair.type_ = entities[pos].second;
      request->targets_.push_back(role_pair);
    }

    return 0;
  }

  return -1;
}

}  // namespace scene

}  // namespace server

}  // namespace game

