#ifndef __GAME__SERVER__CHAT__CHAT__ACTOR__H
#define __GAME__SERVER__CHAT__CHAT__ACTOR__H

#include <ctime>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/chat_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace chat {

class ChatActor : public core::Noncopyable {
 public:
  typedef std::vector<time_t> LastChatTimeVector;

  ChatActor();
  ~ChatActor();

  bool Initialize(GameActor *game_actor);
  void Finalize();
  void SendLoadFinishEvent();

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

  time_t GetLastChatTime(entity::ChatChannelType::type channel);
  void SetLastChatTime(entity::ChatChannelType::type channel, time_t chat_time);

 private:
  GameActor *game_actor_;

  LastChatTimeVector last_chat_time_;
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__CHAT__ACTOR__H

