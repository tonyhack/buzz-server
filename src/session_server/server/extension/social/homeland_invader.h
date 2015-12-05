#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__INVADER__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__INVADER__H

#include <cstddef>
#include <ctime>
#include <string>

#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class HomelandInvader {
 public:
  struct InvadeTimeLess {
    bool operator()(const HomelandInvader *lhs, const HomelandInvader *rhs) {
      return lhs->GetInvadeTime() < rhs->GetInvadeTime();
    }
  };

  HomelandInvader() {}
  HomelandInvader(core::uint64 actor_id, time_t invade_time);
  ~HomelandInvader() {}

  bool Initialize(const std::string &data);
  int Serialize(char *buff, size_t size) const;

  core::uint64 GetActorID() const { return this->actor_id_; }
  time_t GetInvadeTime() const { return this->invade_time_; }

 private:
  core::uint64 actor_id_;
  time_t invade_time_;
};


}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__INVADER__H

