//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 11:18:17.
// File name: storage_actor_login_response.cc
//
// Description:
// Define class StorageActorLoginResponse.
//

#include "session_server/server/storage_actor_login_response.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_login_types.h"
#include "global/actor_id.h"
#include "global/channel.h"
#include "global/game_misc.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/session_actor_pool.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/request/session_playing_request_types.h"
#include "session_server/server/request/session_scene_request_types.h"

namespace session {

namespace server {

StorageActorLoginResponse::StorageActorLoginResponse() {}
StorageActorLoginResponse::~StorageActorLoginResponse() {}

bool StorageActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_LOGIN, boost::bind(
        &StorageActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_LOGIN);
}

void StorageActorLoginResponse::OnResponseFailure(SessionActor *actor) {
  if(actor == NULL) {
    return ;
  }

  SessionTerminal *gateway_terminal = actor->GetGatewayServer();
  if(gateway_terminal) {
    // 发送登陆失败到网关
    protocol::GateMessageUserLoginResponse gate_response;
    gate_response.__set_actor_id_(actor->GetActorID());
    gate_response.__set_game_server_(actor->GetGame());
    gate_response.__set_logined_(false);
    gateway_terminal->SendGatewayMessage(gate_response,
        protocol::GatewayMessageType::MESSAGE_USER_LOGIN_RESPONSE);
  }

  // 回收玩家对象
  actor->Finalize();
  SessionActorPool::GetInstance()->Deallocate(actor);
}

void StorageActorLoginResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageActorLoginRequest request;
  database::protocol::StorageActorLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 把玩家对象从登陆表中删除
  SessionActor *actor = SessionActorManager::GetInstance()->RemoveLoginActor(
      request.id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Remove login actor[%lu] failed.",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    OnResponseFailure(actor);
    LOG_ERROR("玩家(%lld) 登陆服务器失败(%d)", request.id_, result);
    return ;
  }
  if(response.result_ != database::protocol::ActorLoginResult::SUCCESS) {
    OnResponseFailure(actor);
    LOG_ERROR("玩家(%lld) 登陆服务器失败(%d)", request.id_, response.result_);
    return ;
  }
  
  // 计算 VIP剩余时间
  time_t vip_expire = BriefActor::VipTimeRemainderToExpired(
      response.field_.last_logout_time_, response.field_.vip_remainder_time_);
  // 新增 BriefActor
  BriefActor brief_actor(request.id_,
      response.field_.name_,
      response.field_.level_,
      (entity::VocationType::type)response.field_.vocation_,
      (entity::GenderType::type)response.field_.gender_,
      response.field_.map_,
      response.field_.functionality_state_,
      response.field_.axe_fight_score_,
      response.field_.shoot_fight_score_,
      response.field_.magic_fight_score_,
      response.field_.last_logout_time_,
      (entity::VipType::type)response.field_.vip_type_,
      vip_expire);
  SessionActorManager::GetInstance()->AddBriefActor(brief_actor);
  // 设置 BriefActor
  actor->SetBriefActor(
      SessionActorManager::GetInstance()->GetBriefActor(request.id_));

  // 发送登陆请求到游戏服务器
  protocol::GameMessageActorLoginRequest game_request;

  request::RequestPlayingActorConnectInfo request_playing;
  request_playing.__set_req_actor_(actor->GetActorID());
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_PLAYING_ACTOR_CONNECT_INFO, &request_playing,
        sizeof(request_playing)) != 0) {
    LOG_ERROR("玩家(%lu) 请求 REQUEST_PLAYING_ACTOR_CONNECT_INFO 失败",
        actor->GetActorID());
    return ;
  }

  request::RequestSceneGetGameServer scene_request;

  // 如果有关联副本且需要进入，同替换场景ID
  if(request_playing.ret_playing_ != 0 && request_playing.ret_online_back_to_ == true) {
    scene_request.__set_scene_(request_playing.ret_scene_);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_SCENE_GET_GAME_SERVER, &scene_request,
          sizeof(scene_request)) != 0) {
      LOG_ERROR("玩家(%lu) 登陆服务器失败，无法获取所在场景(%lu) GameServer",
          request.id_, request_playing.ret_scene_);
      return ;
    }
    game_request.__set_scene_(request_playing.ret_scene_);
  } else {
    scene_request.__set_map_(response.field_.map_);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_SCENE_GET_GAME_SERVER, &scene_request,
          sizeof(scene_request)) != 0) {
      scene_request.__set_map_(response.field_.pre_map_);
      if(ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_SCENE_GET_GAME_SERVER, &scene_request,
            sizeof(scene_request)) != 0) {
        LOG_ERROR("玩家(%lu) 登陆服务器失败，无法获取所在地图(%u,%u)的GameServer",
            request.id_, response.field_.map_, response.field_.pre_map_);
        return ;
      }
      response.field_.map_ = response.field_.pre_map_;
      response.field_.x_ = response.field_.pre_x_;
      response.field_.y_ = response.field_.pre_y_;
    }
    game_request.__set_scene_(scene_request.scene_);
  }
  game_request.__set_x_(response.field_.x_);
  game_request.__set_y_(response.field_.y_);

  // 得到所在游戏服务器
  SessionTerminal *game_terminal = SessionServerSingleton::GetInstance().GetTerminal(
      global::protocol::ServerType::GAME_SERVER, scene_request.game_server_);
  if(game_terminal == NULL) {
    LOG_ERROR("Allocate game server failed.", __FILE__, __LINE__, __FUNCTION__);
    SessionActorManager::GetInstance()->RemoveLoginActor(request.id_);
    actor->Finalize();
    SessionActorPool::GetInstance()->Deallocate(actor);
    return ;
  }

  actor->SetScene(scene_request.scene_);
  actor->SetGameServer(game_terminal->GetSignature().id_);

  LOG_INFO("玩家(%lu) 在场景服(%u)", request.id_, game_terminal->GetSignature().id_);

  // 加入管理器失败
  if(SessionActorManager::GetInstance()->AddActor(actor) == false) {
    OnResponseFailure(actor);
    LOG_ERROR("玩家(%lu)登陆或加入管理器失败", actor->GetActorID());
    return ;
  }

  size_t response_size = 0;
  const char *new_response =
    global::GlobalPacket::GetPacket()->Serialize(response, response_size);
  if(new_response == NULL) {
    LOG_ERROR("序列化出错");
    return ;
  }

  game_request.__set_actor_id_(actor->GetActorID());
  game_request.__set_gateway_(actor->GetGateway());
  game_request.data_.assign(new_response, response_size);
  game_request.__isset.data_ = true;
  game_request.__set_jump_scene_(false);
  game_terminal->SendGameMessage(game_request,
      protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_REQUEST);

  // 广播玩家登陆事件
  event::EventActorLogin event;
  event.__set_id_(request.id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_LOGIN, actor->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventActorLogin));

  if(actor->GetLoadProgress().LoadFinish() == true) {
    actor->OnSessionLoadFinish();
  }
}

}  // namespace server

}  // namespace session

