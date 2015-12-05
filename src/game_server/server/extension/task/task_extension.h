//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 17:33:43.
// File name: task_extension.h
//
// Description:
// Define class TaskExtension.
//
#ifndef __GAME__SERVER__TASK__TASK__EXTENSION__H
#define __GAME__SERVER__TASK__TASK__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/task/actor_attr_event_handler.h"
#include "game_server/server/extension/task/actor_daily_clean_event_handler.h"
#include "game_server/server/extension/task/actor_extension_event_handler.h"
#include "game_server/server/extension/task/actor_logout_event_handler.h"
#include "game_server/server/extension/task/actor_save_event_handler.h"
#include "game_server/server/extension/task/attr_event_handler.h"
#include "game_server/server/extension/task/change_vip_event_handler.h"
#include "game_server/server/extension/task/functionality_event_handler.h"
#include "game_server/server/extension/task/item_event_handler.h"
#include "game_server/server/extension/task/playing_event_handler.h"
#include "game_server/server/extension/task/role_killed_event_handler.h"
#include "game_server/server/extension/task/request_task_handler.h"
#include "game_server/server/extension/task/request_task_max_layer.h"
#include "game_server/server/extension/task/soul_appear_event_handler.h"
#include "game_server/server/extension/task/storage_task_login_response.h"
#include "game_server/server/extension/task/soul_event_handler.h"
#include "game_server/server/extension/task/scene_event_handler.h"
#include "game_server/server/extension/task/summon_npc_manager.h"
#include "game_server/server/extension/task/task_message_handler.h"
#include "game_server/server/extension/task/task_circle_message_handler.h"
#include "game_server/server/extension/task/task_trip_message_handler.h"
#include "game_server/server/extension/task/task_homeland_event_handle.h"
#include "game_server/server/extension/task/task_daily_message_handler.h"
#include "game_server/server/extension/task/task_transport_message_handler.h"
#include "game_server/server/extension/task/transport_event_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

class TaskExtension : public global::SingletonFactory<TaskExtension> {
 friend class global::SingletonFactory<TaskExtension>;

 public:
  TaskExtension();
  ~TaskExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();
  
  SummonNpcManager& GetSummonNpcManager() {
    return this->npcs_;
  }

 private:
  // 事件回调
  ActorExtensionEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  RoleKilledEventHandler role_killed_event_;
  SoulAppearEventHandler soul_appear_event_;
  ItemEventHandler item_event_;
  SoulEventHandler soul_event_;
  AttrEventHandler attr_event_;
  PlayingEventHandler playing_event_;
  FunctionalityEventHandler functionality_event_;
  ActorDailyCleanEventHandler dailyHandler_event_;
  ActorAttrEventHandler actor_attr_event_;
  SceneEventHandler scene_event_;
  TaskHomelandEventHandler homeland_event_;
  ChangeVipEventHandler change_vip_event_;
  TransportEventHandler transport_event_;

  // 消息回调
  TaskMessageHandler task_message_handler_;
  TaskCircleMessageHandler circle_meesage_handler_;
  TaskTripMessageHandler trip_messagme_handler_;
  TaskDailyMessageHandler daily_message_handler_;
  TaskTransportMessageHandler transport_message_handler_;

  // 数据库回调
  StorageTaskLoginResponse storage_task_login_;

  // 请求
  RequestTaskHandler request_handler_;
  RequestTaskMaxLayer request_layer_handler_;

  // 召唤NPC管理器
  SummonNpcManager npcs_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__EXTENSION__H
