#ifndef __GLOBAL__GUILD__ID__H
#define __GLOBAL__GUILD__ID__H

namespace global {

#include <sstream>

class GuildID {
 public:
  GuildID(core::uint16 zone, core::uint32 offset)
    : zone_(zone), offset_(offset), serialize_(0) {
    this->Serialize();
  }

  GuildID(core::uint64 value) {
    this->Deserialize(value);
  }

  GuildID() : zone_(0), offset_(0), serialize_(0) {}

  inline core::int64 GetID() const {
    return this->serialize_;
  }
  inline void Serialize() {
    this->serialize_ = 0;
    this->serialize_ = ((core::uint64)this->zone_ << kZoneShift) |
      (core::uint64)this->offset_;
  }
  inline void Deserialize(core::uint64 value) {
    this->zone_ = (value & kZoneMask) >> kZoneShift;
    this->offset_ = value & kOffsetMask;
    this->serialize_ = value;
  }
  inline void Deserialize(const char *value) {
    core::uint64 temp = atoll(value);
    this->Deserialize(temp);
  }

  inline void Assign(const GuildID &id) {
    this->zone_ = id.zone_;
    this->offset_ = id.offset_;
    this->Serialize();
  }

  static inline std::string SerializeString(core::uint64 id) {
    std::ostringstream oss;
    oss << id;
    return oss.str();
  }

  core::uint16 zone_;
  core::uint32 offset_;
  core::uint64 serialize_;

 private:
  static const core::uint64 kZoneMask = 0xFFFF000000000000;
  static const core::uint64 kOffsetMask = 0xFFFFFFFF;

  static const core::uint32 kZoneShift = 48;
};

}  // namespace global 

#endif  // __GLOBAL__GUILD__ID__H

