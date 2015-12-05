#include "game_server/server/extension/guild/guild_pool.h"

namespace game {

namespace server {

namespace guild {

bool GuildPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->guilds_.Initialize(initial_number, extend_number);
}

void GuildPool::Finalize() {
  this->guilds_.Finalize();
}

Guild *GuildPool::Allocate() {
  return this->guilds_.Allocate();
}

void GuildPool::Deallocate(Guild *guild) {
  this->guilds_.Deallocate(guild);
}

}  // namespace guild

}  // namespace server

}  // namespace game

