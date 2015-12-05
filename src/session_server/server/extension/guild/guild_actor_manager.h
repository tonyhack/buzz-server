#ifndef __SESSION__SERVER__GUILD__GUILD__ACTOR__MANAGER__H
#define __SESSION__SERVER__GUILD__GUILD__ACTOR__MANAGER__H

#include <ext/hash_map>
#include <mysql++/mysql++.h>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace guild {

class GuildActor;

class GuildActorManager : public global::SingletonFactory<GuildActorManager>,
                          public core::TimerHandler {
  friend class global::SingletonFactory<GuildActorManager>;
  enum { TIMER_ID_SAVE = 0 };

 public:
  typedef __gnu_cxx::hash_map<core::uint64, GuildActor *> ActorHashmap;

  bool Initialize();
  void Finalize();

  GuildActor *Get(core::uint64 id);
  bool Add(GuildActor *actor);
  GuildActor *Remove(core::uint64 id);

  GuildActor *GetFromCache(core::uint64 id);
  bool AddToCache(GuildActor *actor);
  GuildActor *RemoveFromCache(core::uint64 id);

  GuildActor *GetFromAll(core::uint64 id);

  virtual void OnTimer(core::uint32 id);

  void WeeklyClean();

 private:
  GuildActorManager() {}
  ~GuildActorManager() {}

  bool SaveCachedGuildActors(mysqlpp::Connection &connection);

  ActorHashmap actors_;
  ActorHashmap cached_actors_;
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GUILD__ACTOR__MANAGER__H

