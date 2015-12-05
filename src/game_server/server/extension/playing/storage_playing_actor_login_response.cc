//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 13:39:05.
// File name: storage_playing_actor_login_response.cc
//
// Description:
// Define class StoragePlayingActorLoginResponse.
//

#include "game_server/server/extension/playing/storage_playing_actor_login_response.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_playing_actor_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_actor_pool.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace playing {

StoragePlayingActorLoginResponse::StoragePlayingActorLoginResponse() {}
StoragePlayingActorLoginResponse::~StoragePlayingActorLoginResponse() {}

bool StoragePlayingActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_PLAYING_ACTOR_LOGIN, boost::bind(
        &StoragePlayingActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StoragePlayingActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_PLAYING_ACTOR_LOGIN);
}

void StoragePlayingActorLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  database::protocol::StoragePlayingActorLoginRequest request;
  database::protocol::StoragePlayingActorLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize message failed.");
    return ;
  }

  if(result != database::protocol::ResponseType::SUCCESS) {
    LOG_ERROR("玩家(%lu Playing扩展登陆失败(%d))", request.actor_id_, result);
    return ;
  }
  if(response.result_ != database::protocol::PlayingActorLoginResult::SUCCESS) {
    LOG_ERROR("玩家(%lu Playing扩展登陆失败(%d))", request.actor_id_, response.result_);
    return ;
  }

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(request.actor_id_);
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", request.actor_id_);
    return ;
  }

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(request.actor_id_);
  if(playing_actor) {
    LOG_ERROR("玩家(%lu) Playing扩展已存在", request.actor_id_);
    return ;
  }

  playing_actor = PlayingActorPool::GetInstance()->Allocate();
  if(playing_actor == NULL) {
    LOG_ERROR("分配 PlayingActor(%lu) 失败", request.actor_id_);
    return ;
  }

  if(playing_actor->Initialize(game_actor, response.field_) == false) {
    PlayingActorPool::GetInstance()->Deallocate(playing_actor);
    LOG_ERROR("初始化 PlayingActor(%lu) 失败", request.actor_id_);
    return ;
  }

  if(PlayingActorManager::GetInstance()->Add(playing_actor) == false) {
    playing_actor->Finalize();
    PlayingActorPool::GetInstance()->Deallocate(playing_actor);
    LOG_ERROR("PlayingActor(%lu) 加入 PlayingActorManager 失败", request.actor_id_);
    return ;
  }

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(request.actor_id_);
  load_finish.__set_extension_type_(ExtensionType::PLAYING);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, request.actor_id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  LOG_DEBUG("玩家(%lu, %s) 副本扩展加载成功",
      game_actor->GetID(), game_actor->GetName().c_str());
}

}  // namespace playing

}  // namespace server

}  // namespace game

