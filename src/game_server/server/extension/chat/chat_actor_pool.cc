#include "game_server/server/extension/chat/chat_actor_pool.h"

namespace game {

namespace server {

namespace chat {

bool ChatActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void ChatActorPool::Finalize() {
  this->actors_.Finalize();
}

ChatActor *ChatActorPool::Allocate() {
  return this->actors_.Allocate();
}

void ChatActorPool::Deallocate(ChatActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace chat

}  // namespace server

}  // namespace game

