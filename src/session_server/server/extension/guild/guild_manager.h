#ifndef __SESSION__SERVER__GUILD__GUILD__MANAGER__H
#define __SESSION__SERVER__GUILD__GUILD__MANAGER__H

#include <map>
#include <string>
#include <ext/hash_map>
#include <mysql++/mysql++.h>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/hash_string.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace guild {

class Guild;
class GuildActor;

class GuildManager : public global::SingletonFactory<GuildManager>,
                     public core::TimerHandler {
  friend class global::SingletonFactory<GuildManager>;
  enum { TIMER_ID_CLEAR_GUILD_BUFF = 0 };

 public:
  typedef std::map<core::uint64, Guild *> GuildMap;
  typedef __gnu_cxx::hash_map<std::string, core::uint64, global::hash<std::string> > GuildIDHashmap;
  typedef std::vector<core::uint64> GuildActorVector;

  bool Initialize();
  void Finalize();

  Guild *GetGuild(core::uint64 id);
  Guild *GetGuild(const std::string &name);
  const GuildMap &GetGuilds() const { return this->guilds_; }

  bool CreateGuild(const std::string &guild_name, core::int32 guild_logo,
      GuildActor *leader, bool synchronize = true);
  void DismissGuild(core::uint64 id, bool synchronize = true);
  bool AddGuild(Guild *guild);
  Guild *RemoveGuild(core::uint64 id);

  // 定时器
  virtual void OnTimer(core::uint32 id);
  void DailyClean();
  void WeeklyClean();
  void ClearGuildBuff();

 private:
  GuildManager() {}
  ~GuildManager() {}

  void AddClearGuildBuffTimer();
  void RemoveClearGuildBuffTimer();

  bool LoadGuildActors(mysqlpp::Connection &connection, Guild *guild,
      const GuildActorVector &pending_members, const GuildActorVector &members);
  bool LoadGuilds(mysqlpp::Connection &connection);
  bool SaveGuilds(mysqlpp::Connection &connection);

  GuildMap guilds_;
  GuildIDHashmap ids_;
  core::uint32 max_guild_id_offset_;
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GUILD__MANAGER__H

