//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-29 18:20:30.
// File name: game_misc.h
//
// Description:
// Basic definition.
//

#ifndef __GLOBAL__GAME__MISC__H
#define __GLOBAL__GAME__MISC__H

#include "core/base/types.h"

namespace global {

class GameMisc {
 public:
  static core::uint16 GetSceneSequenceID(core::uint64 scene_id) {
    return (scene_id & 0xFFFF00000000) >> 32;
  }
  static core::uint16 GetServerID(core::uint64 scene_id) {
    return (scene_id & 0xFFFF000000000000) >> 48;
  }
  static core::uint16 GetSceneMapID(core::uint64 scene_id) {
    return scene_id & 0xFFFF;
  }
  static core::uint64 MakeSceneID(core::uint16 server, core::uint16 sequence,
      core::uint16 map) {
    return ((core::uint64)server << 48) | ((core::uint64)sequence << 32) |
      ((core::uint64)map);
  }
};

}  // namespace global

#endif  // __GLOBAL__GAME__MISC__H

