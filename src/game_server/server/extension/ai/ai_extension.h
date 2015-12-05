//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 18:23:42.
// File name: ai_extension.h
//
// Description:
// Define class AiExtension.
//

#ifndef __GAME__SERVER__AI__AI__EXTENSION__H
#define __GAME__SERVER__AI__AI__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/ai/ai_event_handler.h"
#include "game_server/server/extension/ai/npc_create_destory_event_handler.h"
#include "game_server/server/extension/ai/request_ai_handler.h"
#include "game_server/server/extension/ai/request_add_threat.h"
#include "game_server/server/extension/ai/role_basic_status_change_handler.h"
#include "game_server/server/extension/ai/role_coming_leaving_event_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class AiExtension : public global::SingletonFactory<AiExtension> {
 friend class global::SingletonFactory<AiExtension>;
 public:
  AiExtension();
  ~AiExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  NpcCreateDestoryEventHandler npc_create_destory_event_;
  RoleComingLeavingEventHandler role_coming_leaving_event_;
  RoleBasicStatusChangeHandler role_basic_status_change_event_;
  AiEventHandler ai_event_;

  RequestAiHandler request_ai_handler_;
  RequestAddThreat req_add_threat_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__AI__EXTENSION__H

