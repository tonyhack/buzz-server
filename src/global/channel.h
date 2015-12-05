//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-27 17:34:08.
// File name: channel.h
//
// Description:
// Define class Channel.
//

#ifndef __GLOBAL__CHANNEL__H
#define __GLOBAL__CHANNEL__H

#include "core/base/types.h"

namespace global {

class Channel {
 public:
  static core::uint16 GetZoneID(core::uint64 channel) {
    return (channel & Channel::kZoneMask) >> kZoneShift;
  }

 private:
  static const core::uint64 kZoneMask = 0xFFFF000000000000;
  static const core::uint32 kZoneShift = 48;
};

}  // namespace global

#endif  // __GLOBAL__CHANNEL__H

