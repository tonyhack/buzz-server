//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-25 15:40:19.
// File name: actor_id.h
//
// Description:
// Define class ActorID;
//

#ifndef __GLOBAL__ACTOR__ID__H
#define __GLOBAL__ACTOR__ID__H

#include <sstream>

namespace global {

class ActorID {
 public:
  ActorID(core::uint16 zone, core::uint16 table, core::uint32 offset)
    : zone_(zone), table_(table), offset_(offset), serialize_(0) {
    this->Serialize();
  }
  ActorID(core::uint64 value) {
    this->Deserialize(value);
  }
  ActorID() : zone_(0), table_(0), offset_(0) {}
  ~ActorID() {}

  inline core::uint64 GetID() const {
    return this->serialize_;
  }
  inline void Serialize() {
    this->serialize_ = ((core::uint64)this->zone_ << kZoneShift) |
      ((core::uint64)this->table_ << kTableShift) | (core::uint64)this->offset_;
  }
  inline void Deserialize(core::uint64 value) {
    this->zone_ = (value & kZoneMask) >> kZoneShift;
    this->table_ = (value & kTableMask) >> kTableShift;
    this->offset_ = value & kOffsetMask;
    this->serialize_ = value;
  }
  inline void Deserialize(const char *value) {
    core::uint64 temp = atoll(value);
    this->Deserialize(temp);
  }

  inline void Assign(const ActorID &id) {
    this->zone_ = id.zone_;
    this->table_ = id.table_;
    this->offset_ = id.offset_;
    this->Serialize();
  }

  static inline std::string SerializeString(core::uint64 id) {
    std::ostringstream oss;
    oss << id;
    return oss.str();
  }

  core::uint16 zone_;
  core::uint16 table_;
  core::uint32 offset_;
  core::uint64 serialize_;

 private:
  static const core::uint64 kZoneMask = 0xFFFF000000000000;
  static const core::uint64 kTableMask = 0xFFFF00000000;
  static const core::uint64 kOffsetMask = 0xFFFFFFFF;

  static const core::uint32 kZoneShift = 48;
  static const core::uint32 kTableShift = 32;
};

}  // namespace global

#endif  // __GLOBAL__ACTOR__ID__H

