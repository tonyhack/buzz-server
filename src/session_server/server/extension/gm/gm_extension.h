#ifndef __SESSION__SERVER__GM__GM__EXTENSION__H
#define __SESSION__SERVER__GM__GM__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/gm/game_gm_message_handler.h"

namespace session {

namespace server {

namespace gm {

class GmExtension : public core::Noncopyable {
 public:
  GmExtension();
  ~GmExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 消息回调
  GameGmMessageHandler game_gm_message_handler_;
};

}  // namespace gm

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GM__GM__EXTENSION__H

