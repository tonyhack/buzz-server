#include "session_server/server/extension/social/enemy_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

EnemyMessageHandler::EnemyMessageHandler() {}
EnemyMessageHandler::~EnemyMessageHandler() {}

bool EnemyMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_ADD_REQUEST,
      boost::bind(&EnemyMessageHandler::OnMessageSocialEnemyAddRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_REMOVE_REQUEST,
      boost::bind(&EnemyMessageHandler::OnMessageSocialEnemyRemoveRequest,
          this, _1, _2, _3));

  return true;
}

void EnemyMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_ADD_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_REMOVE_REQUEST);
}

static void SendMessageSocialEnemyAddResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageSocialEnemyAddResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_ADD_RESPONSE);
}

static void SendMessageSocialEnemyRemoveResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageSocialEnemyRemoveResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_REMOVE_RESPONSE);
}

void EnemyMessageHandler::OnMessageSocialEnemyAddRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageSocialEnemyAddRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSocialEnemyAddRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOCIAL_ENEMY_ADD_REQUEST from actor(%lu, %s), "
           "actor_name_=[%s]",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_name_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::CONTACTS_MODULE) == false) {
    LOG_WARNING("CONTACTS_MODULE is disable.");
    SendMessageSocialEnemyAddResponseFailed(social_actor);
    return;
  }

  bool result = social_actor->AddEnemy(request.actor_name_);

  if (false == result) {
    LOG_WARNING("AddEnemy failed.");
  }

  gateway::protocol::MessageSocialEnemyAddResponse response;
  response.__set_result_(result);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_ADD_RESPONSE);
}

void EnemyMessageHandler::OnMessageSocialEnemyRemoveRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageSocialEnemyRemoveRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSocialEnemyRemoveRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOCIAL_ENEMY_REMOVE_REQUEST from actor(%lu, %s),"
           "actor_id_=[%s]",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::CONTACTS_MODULE) == false) {
    LOG_WARNING("CONTACTS_MODULE is disable.");
    SendMessageSocialEnemyRemoveResponseFailed(social_actor);
    return;
  }

  bool result = social_actor->RemoveEnemy(atol(request.actor_id_.c_str()));

  if (false == result) {
    LOG_WARNING("RemoveEnemy failed.");
  }

  gateway::protocol::MessageSocialEnemyRemoveResponse response;
  response.__set_result_(result);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_REMOVE_RESPONSE);
}

}  // namespace social

}  // namespace server

}  // namespace session

