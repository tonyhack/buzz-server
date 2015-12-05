#include "game_server/server/extension/mail/mail_event_handler.h"

#include <boost/bind.hpp>

#include "database_server/protocol/storage_mail_login_types.h"
#include "database_server/client/database_client.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/mail/mail.h"
#include "game_server/server/extension/mail/mail_actor.h"
#include "game_server/server/extension/mail/mail_pool.h"
#include "game_server/server/extension/mail/mail_actor_pool.h"
#include "game_server/server/extension/mail/mail_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/game_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace mail {

MailEventHandler::MailEventHandler() {}
MailEventHandler::~MailEventHandler() {}

bool MailEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&MailEventHandler::OnLoginEvent,
        this, _1, _2, _3, _4));

  // Follow 所有玩家登出事件
  this->conn_actor_logout_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&MailEventHandler::OnLoginOutEvent,
        this, _1, _2, _3, _4));

  return true;
}

void MailEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_login_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_logout_);
}

void MailEventHandler::OnLoginEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 邮件玩家扩展是否存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(event->id_);
  if(mail_actor != NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展已存在",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 分配邮件玩家扩展
  mail_actor = MailActorPool::GetInstance()->Allocate();
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 无法分配 玩家(%lu) 邮件扩展",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 初始化
  if(mail_actor->Initialize(actor) == false) {
    MailActorPool::GetInstance()->Deallocate(mail_actor);
    LOG_ERROR("%s:%d (%s) 玩家(%lu) 初始化 mail_actor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  if(MailActorManager::GetInstance()->Add(mail_actor) == false) {
    mail_actor->Finalize();
    MailActorPool::GetInstance()->Deallocate(mail_actor);
    global::LogError("%s:%d (%s) 无法分配 玩家(%lu) 邮件扩展",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 向数据库请求任务数据
  database::protocol::StorageMailLoginRequest request;
  request.__set_id_(event->id_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_MAIL_LOGIN,
        event->id_) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

void MailEventHandler::OnLoginOutEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  // 道具玩家对象是否已存在管理器中
  MailActor *actor = MailActorManager::GetInstance()->Remove(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 移除邮件玩家对象失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 回收道具玩家对象
  actor->Finalize();
  MailActorPool::GetInstance()->Deallocate(actor);
}

} // namespace  mail

} // namepsace server

} // namespace game
