#ifndef __GAME__SERVER__SOCIAL__SOCIAL__ACTOR__H
#define __GAME__SERVER__SOCIAL__SOCIAL__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace social {

class SocialActor : public core::Noncopyable {
 public:
  SocialActor();
  ~SocialActor();

  bool Initialize(GameActor *game_actor);
  void Finalize();
  void SendLoadFinishEvent();
  void SendSoulSyncToSessionServer();

  GameActor *GetActor() const {
    return this->game_actor_;
  }
  core::uint64 GetID() const {
    return this->game_actor_->GetID();
  }
  std::string GetStringID() const {
    return this->game_actor_->GetStringID();
  }
  const std::string &GetName() const {
    return this->game_actor_->GetName();
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    this->game_actor_->SendMessage(message, type);
  }

 private:
  GameActor *game_actor_;
};

}  // namespace social

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOCIAL__SOCIAL__ACTOR__H

