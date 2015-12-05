//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 20:38:10.
// File name: error_code.h
//
// Description:
// Define ErrorCode.
//

#ifndef __GAME__SERVER__ERROR__CODE__H
#define __GAME__SERVER__ERROR__CODE__H

#include <string>

#include "entity/error_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_error_protocol_types.h"

namespace game {

namespace server {

class ErrorCode {
 public:
  static void Send(GameActor *actor, entity::ErrorType::type type) {
    if(actor) {
      gateway::protocol::MessageErrorCode message;
      message.__set_code_(type);
      actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_ERROR_CODE);
    }
  }
  static void Send(core::uint64 actor_id, entity::ErrorType::type type) {
    GameActor *actor = GameActorManager::GetInstance()->GetActor(actor_id);
    ErrorCode::Send(actor, type);
  }

  static void Send(GameActor *actor, entity::ErrorType::type type,
      const std::string &param1) {
    if(actor) {
      gateway::protocol::MessageErrorCode message;
      message.__set_code_(type);
      message.params_.push_back(param1);
      actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_ERROR_CODE);
    }
  }
  static void Send(core::uint64 actor_id, entity::ErrorType::type type,
      const std::string &param1) {
    GameActor *actor = GameActorManager::GetInstance()->GetActor(actor_id);
    ErrorCode::Send(actor, type, param1);
  }

  static void Send(GameActor *actor, entity::ErrorType::type type,
      const std::string &param1, const std::string &param2) {
    if(actor) {
      gateway::protocol::MessageErrorCode message;
      message.__set_code_(type);
      message.params_.push_back(param1);
      message.params_.push_back(param2);
      actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_ERROR_CODE);
    }
  }
  static void Send(core::uint64 actor_id, entity::ErrorType::type type,
      const std::string &param1, const std::string &param2) {
    GameActor *actor = GameActorManager::GetInstance()->GetActor(actor_id);
    ErrorCode::Send(actor, type, param1, param2);
  }
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ERROR__CODE__H

