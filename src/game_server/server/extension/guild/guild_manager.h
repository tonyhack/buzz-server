#ifndef __GAME__SERVER__GUILD__GUILD__MANAGER__H
#define __GAME__SERVER__GUILD__GUILD__MANAGER__H

#include <string>
#include <ext/hash_map>

#include "core/base/types.h"
#include "global/hash_string.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace guild {

class Guild;

class GuildManager : public global::SingletonFactory<GuildManager> {
  friend class global::SingletonFactory<GuildManager>;

 public:
  typedef __gnu_cxx::hash_map<core::uint64, Guild *> GuildHashmap;
  typedef __gnu_cxx::hash_map<std::string, core::uint64, global::hash<std::string> > GuildIDHashmap;

  Guild *GetGuild(core::uint64 id);
  Guild *GetGuild(const std::string &name);

  bool AddGuild(Guild *guild);
  Guild *RemoveGuild(core::uint64 id);

 private:
  GuildHashmap guilds_;
  GuildIDHashmap ids_;
};

}  // namespace guild

}  // namespace server

}  // namespace game


#endif  // __GAME__SERVER__GUILD__GUILD__MANAGER__H

