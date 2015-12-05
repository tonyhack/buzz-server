//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 11:32:37.
// File name: event_handler.cc
//
// Description:
// Define class EventHandler.
//

#include "game_server/server/extension/test/event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace test {

EventHandler::EventHandler() {}
EventHandler::~EventHandler() {}

bool EventHandler::Initialize() {
  // Follow 所有场景广播创建角色事件
  this->conn_scene_broadcast_create_role_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_SCENE, 
      boost::bind(&EventHandler::OnSceneBroadcastCreateRoleEvent, this, _1, _2, _3, _4));

  // Follow 所有场景创建视野内角色列表事件
  this->conn_scene_create_aoi_roles_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_CREATE_AOI_ROLES,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_SCENE, 
      boost::bind(&EventHandler::OnSceneCreateAoiRolesEvent, this, _1, _2, _3, _4));

  // Follow 所有玩家加载完成的事件
  this->conn_actor_load_finish_ = 
    coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOAD_FINISH,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&EventHandler::OnActorLoadFinish, this, _1, _2, _3, _4));

  return true;
}

void EventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_scene_broadcast_create_role_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_scene_create_aoi_roles_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_actor_load_finish_);
}

void EventHandler::OnSceneBroadcastCreateRoleEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneBroadcastCreateRole) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSceneBroadcastCreateRole *event =
    (event::EventSceneBroadcastCreateRole *)message;

/*
  LOG_ERROR("[日志宏测试] error 角色(%u, %lu)", event->type_, event->id_);
  LOG_INFO("[日志宏测试] info 角色(%u, %lu)", event->type_, event->id_);
  LOG_DEBUG("[日志宏测试] debug 角色(%u, %lu)", event->type_, event->id_);
  LOG_WARNING("[日志宏测试] warning 角色(%u, %lu)", event->type_, event->id_);
  LOG_FATAL("[日志宏测试] fatal");
  */
}

void EventHandler::OnSceneCreateAoiRolesEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneCreateAoiRoles) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSceneCreateAoiRoles *event = (event::EventSceneCreateAoiRoles *)message;

}

void EventHandler::OnActorLoadFinish(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLoadFinish *event = (event::EventActorLoadFinish *)message;

  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 不存在在 GameActorManager",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }
/*
  // 测试BroadcastSceneMessage接口
  gateway::protocol::MessageActorRelocation relocation;
  relocation.__set_id_(actor->GetStringID());
  relocation.__set_x_(50);
  relocation.__set_y_(50);
  GameServerSingleton::GetInstance().BroadcastSceneMessage(relocation,
      gateway::protocol::MessageType::MESSAGE_ACTOR_RELOCATION, actor->GetScene());
      */
/*
  // 请求增加技能
  request::RequestAddSkill request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(event->id_);
  request.__set_level_(1);
  request.__set_stand_(true);

  request.__set_skill_id_(2);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
      &request, sizeof(request));

  request.__set_skill_id_(3);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
      &request, sizeof(request));

  request.__set_skill_id_(4);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
      &request, sizeof(request));

  request.__set_skill_id_(5);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
      &request, sizeof(request));

  request.__set_skill_id_(6);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
      &request, sizeof(request));

  request.__set_skill_id_(7);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
      &request, sizeof(request));

  request.__set_skill_id_(10001);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
      &request, sizeof(request));

  request.__set_skill_id_(1);
  // 请求
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_SKILL,
        &request, sizeof(request)) != 0) {
    global::LogError("%s:%d (%s) 玩家(%lu) 增加技能失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 结果
  if(request.result_ != request::SkillResultType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 增加技能失败(%u)",
        __FILE__, __LINE__, __FUNCTION__, event->id_, request.result_);
    return ;
  }

  global::LogDebug("玩家(%lu) 增加技能(%u)成功", event->id_, request.skill_id_);
  */
}

}  // namespace test

}  // namespace server

}  // namespace game

