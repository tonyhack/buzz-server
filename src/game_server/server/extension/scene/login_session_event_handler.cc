//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 15:52:30.
// File name: login_session_event_handler.cc
//
// Description:
// Define LoginSessionEventHandler.
//

#include "game_server/server/extension/scene/login_session_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_system_event_types.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/session_channel.h"
#include "global/channel.h"
#include "global/game_misc.h"
#include "session_server/protocol/game_scene_protocol_types.h"

namespace game {

namespace server {

namespace scene {

LoginSessionEventHandler::LoginSessionEventHandler() {}
LoginSessionEventHandler::~LoginSessionEventHandler() {}

bool LoginSessionEventHandler::Initialize() {
  // Follow 登陆 Session 成功事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SYSTEM_LOGIN_SESSION, 0, event::ChannelType::CHANNEL_SYSTEM,
      boost::bind(&LoginSessionEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void LoginSessionEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void LoginSessionEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  // 设置登陆会话服务器成功
  SessionChannel::SetSessionLogined(true);

  const SceneManager::SceneMap &scenes = SceneManager::GetInstance()->GetScenes();
  if(scenes.empty() == true) {
    return ;
  }

  // 发送批量场景登陆到会话服务器
  session::protocol::MessageSceneBatchLoginRequest request;
  session::protocol::SceneFields field;

  SceneManager::SceneMap::const_iterator iterator = scenes.begin();
  for(; iterator != scenes.end(); ++iterator) {
    if(iterator->second) {
      field.__set_scene_id_(iterator->second->GetSceneID());
      field.__set_dynamic_(iterator->second->CheckDynamic());
      request.scenes_.push_back(field);
    }
  }

  SessionChannel::SendMessage(request,
      session::protocol::GameMessageType::MESSAGE_SCENE_BATCH_LOGIN_REQUEST);
}

}  // namespace scene

}  // namespace server

}  // namespace game

