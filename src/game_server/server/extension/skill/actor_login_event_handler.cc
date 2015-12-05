//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-31 18:34:22.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class  ActorLoginEventHandler.
//

#include "game_server/server/extension/skill/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/extension/skill/skill_role_pool.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

namespace server {

namespace skill {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLoginEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if (actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
    return;
  }

  SkillRole *skill_actor = SkillRoleManager::GetInstance()->GetActor(event->id_);
  if (skill_actor != NULL) {
    LOG_ERROR("SkillRole(%lu) 已存在", event->id_);
    return;
  }

  skill_actor = SkillRolePool::GetInstance()->Allocate();
  if (skill_actor == NULL) {
    LOG_ERROR("分配 SkillRole(%lu) 失败", event->id_);
    return;
  }

  if (skill_actor->Initialize(actor) == false) {
    SkillRolePool::GetInstance()->Deallocate(skill_actor);
    LOG_ERROR("初始化 SkillActor(%lu) 失败", event->id_);
    return;
  }

  if (SkillRoleManager::GetInstance()->Add(skill_actor) == false) {
    skill_actor->Finalize();
    LOG_ERROR("SkillActor(%lu) 加入 SkillRoleManager 失败", event->id_);
    return;
  }

  global::LogInfo("玩家(%lu) 技能扩展加载成功", event->id_);

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::SKILL);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
}

}  // namespace skill

}  // namespace server

}  // namespace game

