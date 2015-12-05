//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 15:13:34.
// File name: playing.cc
//
// Description:
// Define class Playing.
//

#include "game_server/server/extension/playing/playing.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension/playing/playing_destory_checker.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/configure/configure.h"
#include "session_server/protocol/game_playing_protocol_types.h"

namespace game {

namespace server {

namespace playing {

Playing::Playing() {}
Playing::~Playing() {}

bool Playing::Initialize(core::uint32 template_id, core::uint64 id) {
  this->SetTeamplateID(template_id);
  this->SetID(id);

  this->SetStatus(StatusType::RUN);

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", this->GetTemplateID());
    return false;
  }

  if (cell->camp_type_ != entity::PlayingCampType::ACTIVITY) {
    PlayingDestoryChecker::GetInstance()->Add(this->GetID());
  }

  return true;
}

void Playing::Finalize() {
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", this->GetTemplateID());
    return;
  }

  if (cell->camp_type_ != entity::PlayingCampType::ACTIVITY) {
    PlayingDestoryChecker::GetInstance()->Remove(this->GetID());
  }
  this->id_ = 0;
  this->template_id_ = 0;
  this->scene_ = 0;

  this->actors_.clear();
}

bool Playing::AddActor(core::uint64 actor) {
  ActorSet::iterator iterator = this->actors_.find(actor);
  if(iterator != this->actors_.end()) {
    return false;
  }

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", this->GetTemplateID());
    return false;
  }

  if(this->actors_.empty() && cell->camp_type_ != entity::PlayingCampType::ACTIVITY) {
    PlayingDestoryChecker::GetInstance()->Remove(this->GetID());
  }

  this->actors_.insert(actor);

  return true;
}

void Playing::RemoveActor(core::uint64 actor) {
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", this->GetTemplateID());
    return;
  }

  this->actors_.erase(actor);

  if(this->actors_.empty() && cell->camp_type_ != entity::PlayingCampType::ACTIVITY) {
    PlayingDestoryChecker::GetInstance()->Add(this->GetID());
  }
}

void Playing::KickoutActors() {
  // 获取 PlayingCell
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", this->GetTemplateID());
    return ;
  }

  // 循环踢玩家跳转地图
  request::RequestSceneJump request;
  ActorSet::const_iterator iterator = this->actors_.begin();
  while (iterator != this->actors_.end()) {
    core::uint64 actor_id = *iterator;
    this->actors_.erase(iterator);
    GameActor *actor = GameActorManager::GetInstance()->GetActor(actor_id);
    if(actor) {
      request.__set_type_(entity::EntityType::TYPE_ACTOR);
      request.__set_id_(actor->GetID());
      if(actor->GetPreMap() != 0) {
        request.__set_map_(actor->GetPreMap());
        request.__set_x_(actor->GetPrePosx());
        request.__set_y_(actor->GetPrePosy());
      } else {
        cell->GetExitPosition(0, request.map_, request.x_, request.y_);
      }
      if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_JUMP,
            &request, sizeof(request)) != 0) {
        LOG_ERROR("请求跳转地图(%lu) 失败", actor->GetPreMap());
        return ;
      }
      gateway::protocol::MessagePlayingLeaveSynchronize synchronize;
      actor->SendMessage(synchronize,
          gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_SYNCHRONIZE);
    }
    iterator = this->actors_.begin();
  }

  // 清除玩家
  this->actors_.clear();
}

void Playing::RequireDestory() {
  // 踢出所有玩家
  this->KickoutActors();

  // 请求删除副本
  session::protocol::MessagePlayingDestoryRequest request;
  request.__set_playing_(this->GetID());
  SessionChannel::SendMessage(request,
      session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_REQUEST);

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", this->GetTemplateID());
    return;
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

