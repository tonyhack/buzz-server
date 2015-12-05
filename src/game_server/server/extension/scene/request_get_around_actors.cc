//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-27 23:02:15.
// File name: request_get_around_actors.cc
//
// Description:
// Define class RequestGetAroundActors.
//

#include "game_server/server/extension/scene/request_get_around_actors.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_item.h"
#include "game_server/server/extension/scene/scene_item_manager.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

RequestGetAroundActors::RequestGetAroundActors() {}
RequestGetAroundActors::~RequestGetAroundActors() {}

bool RequestGetAroundActors::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_GET_AROUND_ACTORS,
      boost::bind(&RequestGetAroundActors::OnRequest, this, _1, _2));

  return true;
}

void RequestGetAroundActors::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_GET_AROUND_ACTORS);
}

int RequestGetAroundActors::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneGetAroundActors) != size) {
    global::LogError("%s:%d (%s) 请求检测冷却时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneGetAroundActors *request =
    (request::RequestSceneGetAroundActors *)message;

  // 得到当前坐标
  core::uint32 x = 0, y = 0;

  core::uint64 scene_id = 0;

  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    GameActor *actor = GameActorManager::GetInstance()->GetActor(request->id_);
    if(actor == NULL) {
      global::LogError("%s:%d (%s) 获取 GameActor(%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, request->id_);
      return -1;
    }
    scene_id = actor->GetScene();
    actor->GetPosition(x, y);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    // 获取 GameNpc 对象
    GameNpc *npc = GameNpcManager::GetInstance()->Get(request->id_);
    if(npc == NULL) {
      global::LogError("%s:%d (%s) 获取 GameNpc(%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, request->id_);
      return -1;
    }
    scene_id = npc->GetScene();
    npc->GetPosition(x, y);
  } else if (request->type_ == entity::EntityType::TYPE_SCENE_ITEM) {
    SceneItem *item = SceneItemManager::GetInstance()->Get(request->id_);
    if(item == NULL) {
      LOG_ERROR("获取 SceneItem(%lu) 失败", request->id_);
      return -1;
    }
    scene_id = item->GetScene();
    item->GetPosition(x, y);
  } else {
    LOG_ERROR("未知类型(%d)", request->type_);
    return -1;
  }

  // 获取 GameActor 所在 Scene 对象
  Scene *scene = SceneManager::GetInstance()->GetScene(scene_id);
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor(%lu) Scene(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, request->id_, scene_id);
    return -1;
  }

  // 获取九屏玩家
  size_t actor_number = kMaxActorNumber_;
  if(scene->GetNineScreenActors(x, y, this->actors_, actor_number) == false) {
    global::LogError("%s:%d (%s) 获取九屏玩家列表失败",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 填充玩家id
  size_t pos = 0;
  for(; pos < actor_number; ++pos) {
    if(request->type_ != entity::EntityType::TYPE_ACTOR ||
        this->actors_[pos] != (core::uint64)request->id_) {
      request->actors_.push_back(this->actors_[pos]);
    }
  }

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

