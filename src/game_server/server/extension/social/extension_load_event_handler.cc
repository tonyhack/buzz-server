#include "game_server/server/extension/social/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/social/social_actor.h"
#include "game_server/server/extension/social/social_actor_manager.h"
#include "game_server/server/extension/social/social_actor_pool.h"

namespace game {

namespace server {

namespace social {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  // Follow BUFF 模块加载完成事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ExtensionLoadEventHandler::OnEvent,
          this, _1, _2, _3, _4));

  return true;
}

void ExtensionLoadEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ExtensionLoadEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorExtensionLoadFinish) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  if ((ExtensionType::type)event->extension_type_ != ExtensionType::SOUL) {
    return;
  }

  // 从玩家管理器中获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if (NULL == game_actor) {
    LOG_ERROR("Get Gameactor(%lu) from GameActorManager failed.", event->id_);
    return;
  }

  // 社交扩展是否已存在
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(event->id_);
  if (social_actor != NULL) {
    LOG_ERROR("SocialActor(%lu) already exists.", event->id_);
    return;
  }

  // 分配社交玩家对象
  social_actor = SocialActorPool::GetInstance()->Allocate();
  if (NULL == social_actor) {
    LOG_ERROR("Allocate SocialActor(%lu) from SocialActorPool failed.", event->id_);
    return;
  }

  // 初始化
  if (social_actor->Initialize(game_actor) == false) {
    SocialActorPool::GetInstance()->Deallocate(social_actor);
    LOG_ERROR("Init SocialActor(%lu) failed.", event->id_);
    return;
  }

  // 加入管理器
  if (SocialActorManager::GetInstance()->Add(social_actor) == false) {
    social_actor->Finalize();
    SocialActorPool::GetInstance()->Deallocate(social_actor);
    LOG_ERROR("Add SocialActor(%lu) to SocialActorManager failed.", event->id_);
    return;
  }

  // 发送加载完成事件
  social_actor->SendLoadFinishEvent();

  // 同步英灵数据
  social_actor->SendSoulSyncToSessionServer();
}

}  // namespace social

}  // namespace server

}  // namespace game

