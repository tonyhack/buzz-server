#ifndef __SESSION__SERVER__SOCIAL__FACADE__REQUEST__H
#define __SESSION__SERVER__SOCIAL__FACADE__REQUEST__H

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace social {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  core::uint64 GetGuildID(core::uint64 actor_id);
  bool CheckInSameGuild(core::uint64 actor1_id, core::uint64 actor2_id);
  void AddGuildContribution(core::uint64 actor_id, core::int32 contribution);
  
 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__FACADE__REQUEST__H

