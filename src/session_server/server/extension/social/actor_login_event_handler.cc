#include "session_server/server/extension/social/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_social_actor_login_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"
#include "session_server/server/extension/social/social_actor_pool.h"

namespace session {

namespace server {

namespace social {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登录事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorLoginEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorLogin) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(event->id_);
  if (NULL == session_actor) {
    LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", event->id_);
    return;
  }

  // 社交扩展是否已存在
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(event->id_);
  if (social_actor != NULL) {
    LOG_ERROR("SocialActor(%lu) already exists.", event->id_);
    return;
  }

  // 先从缓存搜索
  social_actor = SocialActorManager::GetInstance()->RemoveFromCache(event->id_);
  if (social_actor != NULL) {
    // 加入在线列表
    if (SocialActorManager::GetInstance()->Add(social_actor) == false) {
      social_actor->Finalize();
      SocialActorPool::GetInstance()->Deallocate(social_actor);
      LOG_ERROR("Add SocialActor(%lu) to SocialActorManager online list failed.", event->id_);
      return;
    }
    // 执行上线操作
    social_actor->Online(session_actor);
    // 发送加载完成事件
    social_actor->SendLoadFinishEvent();

    return;
  }

  // 缓存未命中, 向数据库请求数据
  database::protocol::StorageSocialActorLoginRequest request;
  request.__set_actor_id_(event->id_);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_SOCIAL_ACTOR_LOGIN,
          event->id_) == false) {
    LOG_ERROR("Send StorageSocialActorLoginRequest to database server failed.");
    return;
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

