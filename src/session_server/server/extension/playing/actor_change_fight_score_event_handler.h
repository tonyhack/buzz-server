//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-19 09:04:51.
// File name: actor_change_fight_score_event_handler.h
//
// Description:
// Define class ActorChangeFightScoreEventHandler.
//

#ifndef __SESSION__SERVER__PLAYING__ACTOR__CHANGE__FIGHT__SCORE__EVENT__HANDLER__H
#define __SESSION__SERVER__PLAYING__ACTOR__CHANGE__FIGHT__SCORE__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace session {

namespace server {

namespace playing {

class ActorChangeFightScoreEventHandler : public core::Noncopyable {
 public:
  ActorChangeFightScoreEventHandler();
  ~ActorChangeFightScoreEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__ACTOR__CHANGE__FIGHT__SCORE__EVENT__HANDLER__H

