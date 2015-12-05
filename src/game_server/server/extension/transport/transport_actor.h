//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-06 05:01:34.
// File name: transport_actor.h
//
// Description:
// Define class TransportActor.
//

#ifndef __GAME__SERVER__TRANSPORT__TRANSPORT__ACTOR__H
#define __GAME__SERVER__TRANSPORT__TRANSPORT__ACTOR__H

#include "core/base/noncopyable.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace transport {

class TransportActor : public core::Noncopyable {
 public:
  TransportActor();
  ~TransportActor();

  bool Initialize(GameActor *actor);
  void Finalize();

  void Synchronize();

  inline GameActor *GetActor() {
    return this->actor_;
  }
  inline core::uint64 GetID() const {
    return this->actor_->GetID();
  }
  inline std::string GetName() const {
    return this->actor_->GetName();
  }

  inline core::uint64 GetTransportNpc() const {
    return this->transport_npc_;
  }
  inline void SetTransportNpc(core::uint64 npc, bool synchronize = true) {
    this->transport_npc_ = npc;
    if (synchronize) {
      this->Synchronize();
    }
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    this->actor_->SendMessage(message, type);
  }

 private:
  GameActor *actor_;

  core::uint64 transport_npc_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__TRANSPORT__ACTOR__H

