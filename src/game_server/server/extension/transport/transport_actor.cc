//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 01:05:51.
// File name: transport_actor.cc
//
// Description:
// Define class TransportActor.
//

#include "game_server/server/extension/transport/transport_actor.h"

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_transport_protocol_types.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace transport {

TransportActor::TransportActor() {}
TransportActor::~TransportActor() {}

bool TransportActor::Initialize(GameActor *actor) {
  if (actor == NULL) {
    LOG_ERROR("actor is null.");
    return false;
  }

  this->actor_ = actor;
  this->transport_npc_ = 0;

  return true;
}

void TransportActor::Finalize() {
  this->actor_ = NULL;
}

void TransportActor::Synchronize() {
  gateway::protocol::MessageTransportSynchronize message;
  message.__set_npc_id_(global::ToString(this->GetTransportNpc()));
  this->SendMessage(message,
    gateway::protocol::MessageType::MESSAGE_TRANSPORT_SYNCHRONIZE);
}

}  // namespace transport

}  // namespace server

}  // namespace game

