//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-25 10:12:09.
// File name: actor_message_handler.cc
//
// Description:
// Define class ActorMessageHandler.
//

#include "session_server/server/actor_message_handler.h"

#include <boost/bind.hpp>

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/error_code.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"

namespace session {

namespace server {

ActorMessageHandler::ActorMessageHandler() {}
ActorMessageHandler::~ActorMessageHandler() {}

bool ActorMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_FACADE_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageActorFacadeRequest, this, _1, _2, _3));

  return true;
}

void ActorMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_FACADE_REQUEST);
}

void ActorMessageHandler::OnMessageActorFacadeRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("Invalid params.");
    return;
  }

  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("Get SessionActor(%lu) failed.", id);
    return;
  }

  gateway::protocol::MessageActorFacadeRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("Deserialize MessageActorFacadeRequest failed.");
    return;
  }

  SessionActor *observe_actor =
    SessionActorManager::GetInstance()->GetActor(atol(request.actor_.c_str()));
  if(observe_actor == NULL) {
    ErrorCode::Send(actor, entity::ErrorType::ACTOR_OFFLINE);
    return;
  }

  session::protocol::MessageActorFacadeSynchronize synchronize;
  synchronize.__set_actor_(id);
  synchronize.__set_observe_actor_(observe_actor->GetActorID());
  observe_actor->SendGameMessage(synchronize,
      session::protocol::GameMessageType::MESSAGE_ACTOR_FACADE_SYNCHRONIZE);
}

}  // namespace server

}  // namespace session

