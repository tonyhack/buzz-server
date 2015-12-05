//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 16:11:13.
// File name: session_actor_manager.h
//
// Description:
// Define class SessionActorManager.
//

#ifndef __SESSION__SERVER__SESSION__ACTOR__MANAGER__H
#define __SESSION__SERVER__SESSION__ACTOR__MANAGER__H

#include <time.h>
#include <map>
#include <set>
#include <string>

#include <ext/hash_map>
#include <mysql++/mysql++.h>

#include "core/base/types.h"
#include "global/hash_string.h"
#include "global/singleton_factory.h"
#include "session_server/server/brief_actor.h"

namespace session {

namespace server {

class SessionActor;

class SessionActorManager : public global::SingletonFactory<SessionActorManager> {
  friend class global::SingletonFactory<SessionActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, SessionActor *> ActorHashmap;
  typedef __gnu_cxx::hash_map<std::string, core::uint64, global::hash<std::string> > ActorIDHashmap;
  typedef std::vector<BriefActor *> BriefActorVector;
  typedef __gnu_cxx::hash_map<core::uint64, size_t> BriefActorIDIndexHashmap;
  typedef __gnu_cxx::hash_map<std::string, size_t, global::hash<std::string> > BriefActorNameIndexHashmap;

 public:
  bool Initialize();
  void Finalize();

  void AddBriefActor(const BriefActor &brief_actor);
  BriefActor *GetBriefActor(core::uint64 id);
  BriefActor *GetBriefActor(const std::string &name);

  SessionActor *GetLoginActor(core::uint64 id);

  bool AddLoginActor(SessionActor *actor);
  SessionActor *RemoveLoginActor(core::uint64 id);

  SessionActor *GetActor(core::uint64 id);
  SessionActor *GetActor(const std::string &name);

  bool AddActor(SessionActor *actor);
  SessionActor *RemoveActor(core::uint64 id);

  void KickActor(core::uint64 id);
  void KickActorsInGateway(core::uint32 gateway_id);

  core::int32 GetOnlineUserNumber() const {
    return this->login_actors_.size();
  }
  core::int32 GetTotalUserNumber() const {
    return this->brief_actors_store_.size();
  }

 private:
  SessionActorManager() {}
  ~SessionActorManager() {}

  bool LoadActorBriefInfo(mysqlpp::Connection &connection);

  ActorHashmap login_actors_;
  ActorHashmap actors_;
  ActorIDHashmap ids_;

  BriefActorVector brief_actors_store_;
  BriefActorIDIndexHashmap brief_actors_id_index_;
  BriefActorNameIndexHashmap brief_actors_name_index_;
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SESSION__ACTOR__MANAGER__H

