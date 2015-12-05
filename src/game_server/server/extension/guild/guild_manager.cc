#include "game_server/server/extension/guild/guild_manager.h"

#include <cstddef>

#include "global/logging.h"
#include "game_server/server/extension/guild/guild.h"

namespace game {

namespace server {

namespace guild {

Guild *GuildManager::GetGuild(core::uint64 id) {
  GuildHashmap::iterator iter = this->guilds_.find(id);
  if (iter != this->guilds_.end()) {
    return iter->second;
  }

  return NULL;
}

Guild *GuildManager::GetGuild(const std::string &name) {
  GuildIDHashmap::iterator iter = this->ids_.find(name);
  if (iter != this->ids_.end()) {
    return this->GetGuild(iter->second);
  }

  return NULL;
}

bool GuildManager::AddGuild(Guild *guild) {
  if (NULL == guild) {
    return false;
  }
  if (this->guilds_.find(guild->GetID()) != this->guilds_.end()) {
    return false;
  }
  if (this->ids_.find(guild->GetName()) != this->ids_.end()) {
    return false;
  }

  this->guilds_.insert(std::make_pair(guild->GetID(), guild));
  this->ids_.insert(std::make_pair(guild->GetName(), guild->GetID()));

  return true;
}

Guild *GuildManager::RemoveGuild(core::uint64 id) {
  GuildHashmap::iterator iter1 = this->guilds_.find(id);
  if (iter1 == this->guilds_.end()) {
    return NULL;
  }
  Guild *guild = iter1->second;
  if (NULL == guild) {
    LOG_ERROR("Guild is null.");
    return NULL;
  }
  this->guilds_.erase(iter1);

  GuildIDHashmap::iterator iter2 = this->ids_.find(guild->GetName());
  if (iter2 != this->ids_.end()) {
    this->ids_.erase(iter2);
  } else {
    LOG_ERROR("Guild(%s) is not in id table.", guild->GetName().c_str());
  }

  return guild;
}

}  // namespace guild

}  // namespace server

}  // namespace game

