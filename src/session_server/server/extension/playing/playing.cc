//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-17 19:32:36.
// File name: playing.cc
//
// Description:
// Define class Playing.
//

#include "session_server/server/extension/playing/playing.h"

#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/configure/configure.h"
#include "global/configure/playing_configure.h"
#include "global/common_functions.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/session_server.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_manager.h"

namespace session {

namespace server {

namespace playing {

Playing::Playing() : status_(StatusType::MIN) {}
Playing::~Playing() {}

void Playing::Allocate(core::uint64 id) {
  this->id_ = id;
}

void Playing::Deallocate() {}

bool Playing::Initialize(core::uint64 template_id) {
  this->template_id_ = template_id;
  this->scene_ = 0;
  this->status_ = StatusType::CREATE;

  return true;
}

void Playing::Finalize() {
  this->status_ = StatusType::DESTORY;
  this->ClearActors();
}

bool Playing::AddActor(core::uint64 actor) {
  if(this->actors_.find(actor) == this->actors_.end()) {
    this->actors_.insert(actor);
    PlayingManager::GetInstance()->AddActorPlaying(actor, this->GetID()); 
    return true;
  }
  return false;
}

void Playing::ClearActors() {
  ActorSet::const_iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    PlayingActor *actor = PlayingActorManager::GetInstance()->Get(*iterator);
    if(actor) {
      actor->SetPlaying(0);
    }
    PlayingManager::GetInstance()->RemoveActorPlaying(*iterator);
  }

  this->actors_.clear();
}

void Playing::BroadcastCreateFinish() {
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        this->GetTemplateID());
  if (cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", this->GetTemplateID());
    return;
  }

  gateway::protocol::MessagePlayingCreateFinishNotice notice;
  notice.__set_template_(this->template_id_);
  notice.__set_playing_(global::ToString(this->GetID()));

  if (cell->camp_type_ == entity::PlayingCampType::ACTIVITY) {
    SessionServerSingleton::GetInstance().BroadcastMessage(notice,
        gateway::protocol::MessageType::MESSAGE_PLAYING_CREATE_FINISH_NOTICE);
  } else {
    ActorSet::const_iterator iterator = this->actors_.begin();
    for(; iterator != this->actors_.end(); ++iterator) {
      SessionActor *session_actor =
        SessionActorManager::GetInstance()->GetActor(*iterator);
      if(session_actor) {
        session_actor->SendMessage(notice,
            gateway::protocol::MessageType::MESSAGE_PLAYING_CREATE_FINISH_NOTICE);
      }
    }
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session

