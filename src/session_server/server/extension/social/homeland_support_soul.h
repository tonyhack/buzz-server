#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__SUPPORT__SOUL__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__SUPPORT__SOUL__H

#include <cstddef>
#include <ctime>
#include <string>

#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class HomelandSupportSoul {
 public:
  HomelandSupportSoul() {}
  HomelandSupportSoul(core::uint32 soul_id, core::uint64 support_actor_id);
  ~HomelandSupportSoul() {}

  bool Initialize(const std::string &data);
  int Serialize(char *buff, size_t size) const;

  core::uint32 GetSoulID() const { return this->soul_id_; }
  core::uint64 GetSupportActorID() const { return this->support_actor_id_; }

  void SetSupportActorID(core::uint64 support_actor_id) {
    this->support_actor_id_ = support_actor_id;
  }

  core::int32 GetNextSupportCooling(time_t now = 0) const;
  void SetNextSupportCooling(core::int32 cooling_time);

 private:
  core::uint32 soul_id_;
  core::uint64 support_actor_id_;
  time_t next_support_time_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__SUPPORT__SOUL__H

