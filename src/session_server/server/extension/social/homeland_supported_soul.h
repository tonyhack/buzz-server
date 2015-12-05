#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__SUPPORTED__SOUL__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__SUPPORTED__SOUL__H

#include <cstddef>
#include <ctime>
#include <string>

#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class HomelandSupportedSoul {
 public:
  HomelandSupportedSoul() {}
  HomelandSupportedSoul(core::uint64 actor_id,
      core::uint32 soul_id, core::int32 soul_level);
  ~HomelandSupportedSoul() {}

  bool Initialize(const std::string &data);
  int Serialize(char *buff, size_t size) const;

  core::uint64 GetActorID() const { return this->actor_id_; }
  core::uint32 GetSoulID() const { return this->soul_id_; }
  core::int32 GetSoulLevel() const { return this->soul_level_; }

  void SetSoulID(core::uint32 soul_id) {
    this->soul_id_ = soul_id;
  }
  void SetSoulLevel(core::int32 soul_level) {
    this->soul_level_ = soul_level;
  }

  core::int32 GetLeaveCooling(time_t now = 0) const;
  void SetLeaveCooling(core::int32 cooling_time);

 private:
  core::uint64 actor_id_;
  core::uint32 soul_id_;
  core::int32 soul_level_;
  time_t leave_time_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__SUPPORTED__SOUL__H

