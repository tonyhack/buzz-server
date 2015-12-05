//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-04 20:50:56.
// File name: storage_actor_login_response.cc
//
// Description:
// Define class StorageActorLoginResponse.
//

#include "game_server/server/storage_actor_login_response.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_actor_pool.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "global/channel.h"
#include "global/game_misc.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

StorageActorLoginResponse::StorageActorLoginResponse() {}
StorageActorLoginResponse::~StorageActorLoginResponse() {}

bool StorageActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_LOGIN,
      boost::bind(&StorageActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_LOGIN);
}

void StorageActorLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageActorLoginRequest request;
  database::protocol::StorageActorLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    LOG_ERROR("玩家(%lu) 登陆服务器失败(%d)", request.id_, result);
    return ;
  }
  if(response.result_ != database::protocol::ActorLoginResult::SUCCESS) {
    LOG_ERROR("玩家(%lu) 登陆服务器失败(%d)", request.id_, response.result_);
    return ;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.id_);
  if(actor != NULL) {
    LOG_ERROR("玩家(%lu) 跨服跳转服务器，重复登陆", request.id_);
    return ;
  }

  // 分配玩家对象
  actor = GameActorPool::GetInstance()->Allocate();
  if(actor == NULL) {
    this->ResponseFailed(request.id_);
    GameActorPool::GetInstance()->Deallocate(actor);
    LOG_ERROR("分配玩家对象失败，玩家(%lu) 跨服跳转服务器失败", request.id_);
    return ;
  }

  actor->SetActorID(request.id_);
  actor->SetGateway(request.gateway_);

  // 初始化玩家对象
  if(actor->Initialize(response.field_) == false) {
    this->ResponseFailed(request.id_);
    GameActorPool::GetInstance()->Deallocate(actor);
    LOG_ERROR("初始化玩家对象失败，玩家(%lu) 跨服跳转服务器失败", request.id_);
    return ;
  }

  // 加入玩家管理器
  if(GameActorManager::GetInstance()->AddActor(actor) == false) {
    this->ResponseFailed(request.id_);
    actor->Finalize();
    GameActorPool::GetInstance()->Deallocate(actor);
    LOG_ERROR("加入管理器失败，玩家(%lu) 跨服跳转服务器失败",request.id_);
    return ;
  }

  core::uint64 scene_id = request.jump_scene_;

  actor->SetScene(scene_id);
  actor->SetPosition(request.jump_x_, request.jump_y_);
  actor->SetJumpLogin(true);

  // 发送登陆事件
  event::EventActorLogin message;
  message.__set_id_(actor->GetID());
  message.__set_map_(global::GameMisc::GetSceneMapID(request.jump_scene_));
  message.__set_x_(request.jump_x_);
  message.__set_y_(request.jump_y_);

  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_LOGIN, actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &message,
      sizeof(event::EventActorLogin));

  session::protocol::MessageSceneActorJumpLoginRequest jump_login;
  jump_login.__set_id_(actor->GetID());
  jump_login.__set_scene_(request.jump_scene_);
  jump_login.__set_x_(request.jump_x_);
  jump_login.__set_y_(request.jump_y_);
  SessionChannel::SendMessage(jump_login,
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_LOGIN_REQUEST);
}

void StorageActorLoginResponse::ResponseFailed(core::uint64 actor) {
  session::protocol::GameMessageActorLoginResponse response_message;
  response_message.__set_actor_id_(actor);
  response_message.__set_logined_(false);
  response_message.__set_jump_scene_(true);
  SessionChannel::SendMessage(response_message,
      session::protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_RESPONSE);
}

}  // namespace server

}  // namespace game

