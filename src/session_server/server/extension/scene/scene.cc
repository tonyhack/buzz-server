//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-06 12:05:06.
// File name: scene.cc
//
// Description:
// Define class Scene.
//

#include "session_server/server/extension/scene/scene.h"

#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/game_misc.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/scene/scene_manager.h"

namespace session {

namespace server {

namespace scene {

Scene::Scene() {}
Scene::~Scene() {}

bool Scene::Initialize(core::uint64 id, bool dynamic) {
  this->id_ = id;

  this->game_server_ = 0;

  this->world_boss_.Initialize(this);

  this->dynamic_ = dynamic;

  return true;
}

void Scene::Finalize() {
  this->actors_.clear();
  this->world_boss_.Finalize();
}

bool Scene::AddActor(core::uint64 id) {
  if(this->actors_.find(id) != this->actors_.end()) {
    return false;
  }
  this->actors_.insert(id);

  if (this->CheckDynamic() == false) {
    SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
    if (session_actor != NULL) {
      gateway::protocol::MessageSceneBranchSynchronize message;
      message.__set_current_branch_(this->GetBranchID());
      session_actor->SendMessage(message,
          gateway::protocol::MessageType::MESSAGE_SCENE_BRANCH_SYNCHRONIZE);
    }
  }

  SceneManager::GetInstance()->TryBranchScene(this->GetMapID());

  return true;
}

bool Scene::RemoveActor(core::uint64 id) {
  ActorSet::iterator iterator = this->actors_.find(id);
  if(iterator == this->actors_.end()) {
    return false;
  }
  this->actors_.erase(iterator);
  return true;
}

}  // namespce scene

}  // namespace server

}  // namespce session

