//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-08-03 15:19:31.
// File name: scene_message_handler.cc
//
// Description:
// Define class SceneMessageHandler.
//

#include "session_server/server/extension/scene/scene_message_handler.h"

#include <boost/bind.hpp>

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/global_packet.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/scene/scene.h"
#include "session_server/server/extension/scene/scene_manager.h"

namespace session {

namespace server {

namespace scene {

SceneMessageHandler::SceneMessageHandler() {}
SceneMessageHandler::~SceneMessageHandler() {}

bool SceneMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_BRANCH_SWITCH_REQUEST,
      boost::bind(&SceneMessageHandler::OnMessageSceneBranchSwitchRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_BRANCH_STATUS_REQUEST,
      boost::bind(&SceneMessageHandler::OMessageSceneBranchStatusRequest,
        this, _1, _2, _3));

  return true;
}

void SceneMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_BRANCH_SWITCH_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SCENE_BRANCH_STATUS_REQUEST);
}

void SceneMessageHandler::OnMessageSceneBranchSwitchRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    return;
  }

  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if (session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败");
    return;
  }

  Scene *current_scene = SceneManager::GetInstance()->GetScene(session_actor->GetScene());
  if (current_scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", session_actor->GetScene());
    return;
  }

  gateway::protocol::MessageSceneBranchSwitchRequest message; 
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageSceneBranchSwitchRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SCENE_BRANCH_SWITCH_REQUEST actor(%lu, %s) branch_id_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), message.branch_id_);

  Scene *branch_scene = SceneManager::GetInstance()->GetBranchScene(session_actor->GetMap(),
      message.branch_id_);
  if (branch_scene == NULL) {
    LOG_ERROR("获取分线 Scene(%d, %d) 失败", session_actor->GetMap(), message.branch_id_);
    return;
  }

  session::protocol::MessageSceneActorSwitchBranchNotice notice_game_message;
  notice_game_message.__set_actor_(id);
  notice_game_message.__set_scene_(branch_scene->GetID());
  core::uint32 game_server = current_scene->GetGameServer();
  SessionTerminal *game_terminal = SessionServerSingleton::GetInstance().GetTerminal(
      global::protocol::ServerType::GAME_SERVER, game_server);
  if(game_terminal == NULL) {
    LOG_ERROR("获取 GameServer(%u) 失败", game_server);
    return ;
  }

  game_terminal->SendGameMessage(notice_game_message,
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_SWITCH_BRANCH_NOTICE);

  gateway::protocol::MessageSceneBranchSwitchResponse response;
  response.__set_success_(true);
  session_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SCENE_BRANCH_SWITCH_RESPONSE);
}

void SceneMessageHandler::OMessageSceneBranchStatusRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    return;
  }

  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if (session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", session_actor->GetScene());
    return;
  }

  gateway::protocol::MessageSceneBranchStatusRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageSceneBranchStatusRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SCENE_BRANCH_STATUS_REQUEST actor(%lu, %s)",
      session_actor->GetActorID(), session_actor->GetName().c_str());

  const SceneManager::BranchSceneSet *branches =
    SceneManager::GetInstance()->GetBranches(session_actor->GetMap());
  if (branches == NULL) {
    LOG_ERROR("获取 BranchSceneSet(%d) 失败", session_actor->GetMap());
    return;
  }

  gateway::protocol::MessageSceneBranchStatusResponse response;

  for (SceneManager::BranchSceneSet::const_iterator iterator = branches->begin();
      iterator != branches->end(); ++iterator) {
    Scene *scene = SceneManager::GetInstance()->GetScene(*iterator);
    if (scene == NULL) {
      LOG_ERROR("获取 Scene(%lu) 失败", *iterator);
      return;
    }
    response.actor_number_.push_back(scene->GetActorSize());
  }

  session_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SCENE_BRANCH_STATUS_RESPONSE);
}

}  // namespace scene

}  // namespace server

}  // namespace session

