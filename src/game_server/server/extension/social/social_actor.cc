#include "game_server/server/extension/social/social_actor.h"

#include <cstddef>

#include "global/logging.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_social_protocol_types.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_soul_request_types.h"

namespace game {

namespace server {

namespace social {

SocialActor::SocialActor() : game_actor_(NULL) {}
SocialActor::~SocialActor() {}

bool SocialActor::Initialize(GameActor *game_actor) {
  if (NULL == game_actor) {
    LOG_ERROR("game_actor is null.");
    return false;
  }

  this->game_actor_ = game_actor;

  return true;
}

void SocialActor::Finalize() {
  this->game_actor_ = NULL;
}

void SocialActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetID());
  load_finish.__set_extension_type_(ExtensionType::SOCIAL);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish, sizeof(load_finish));
  LOG_DEBUG("玩家(%lu) SOCIAL扩展加载成功", this->GetID());
}

void SocialActor::SendSoulSyncToSessionServer() {
  if (this->game_actor_->CheckFunctionalityState(
          entity::FunctionalityType::SOUL_MODULE) == false) {
    return;
  }

  request::RequestSoulGet request;
  request.__set_actor_(this->GetID());
  request.__set_appear_(true);

  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SOUL_GET,
          &request, sizeof(request)) != 0) {
    LOG_ERROR("Get Actor(%lu)'s soul data failed.", this->GetID());
    return;
  }

  if (request.souls_.empty()) {
    return;
  }

  session::protocol::MessageHomelandSoulSynchronize sync;
  sync.__set_actor_id_(this->GetID());
  for (size_t i = 0; i < request.souls_.size(); ++i) {
    session::protocol::HomelandSoulData data;
    data.__set_soul_id_(request.souls_[i].id_);
    data.__set_soul_level_(request.souls_[i].level_);
    sync.souls_.push_back(data);
  }
  SessionChannel::SendMessage(sync,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_SOUL_SYNCHRONIZE);
}

}  // namespace social

}  // namespace server

}  // namespace game

