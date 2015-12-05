//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-26 09:51:25.
// File name: playing_builder.h
//
// Description:
// Define class PlayingBuilder.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__BUILDER__H
#define __GAME__SERVER__PLAYING__PLAYING__BUILDER__H

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace playing {

class Playing;

class PlayingBuilder : public global::SingletonFactory<PlayingBuilder> {
  friend class global::SingletonFactory<PlayingBuilder>;

 public:
  Playing *Create(core::uint32 template_id, core::uint64 id);
  void Destory(Playing *playing);

 private:
  PlayingBuilder() {}
  ~PlayingBuilder() {}
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__BUILDER__H

