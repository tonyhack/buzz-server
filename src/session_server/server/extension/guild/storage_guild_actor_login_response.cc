#include "session_server/server/extension/guild/storage_guild_actor_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_guild_actor_login_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"
#include "session_server/server/extension/guild/guild_actor_pool.h"

namespace session {

namespace server {

namespace guild {

StorageGuildActorLoginResponse::StorageGuildActorLoginResponse() {}
StorageGuildActorLoginResponse::~StorageGuildActorLoginResponse() {}

bool StorageGuildActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_GUILD_ACTOR_LOGIN,
      boost::bind(&StorageGuildActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageGuildActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_GUILD_ACTOR_LOGIN);
}

void StorageGuildActorLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageGuildActorLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize StorageGuildActorLoginRequest failed.");
    return;
  }
  database::protocol::StorageGuildActorLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize StorageGuildActorLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      (response.result_ != database::protocol::GuildActorLoginResult::SUCCESS &&
       response.result_ != database::protocol::GuildActorLoginResult::NOT_FOUND)) {
    LOG_ERROR("Call StorageGuildActorLoginRequest failed.");
    return;
  }

  // 从管理器中获取玩家对象
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(request.actor_id_);
  if (NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", request.actor_id_);
    return;
  }
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(request.actor_id_);
  if (NULL == session_actor) {
    LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", request.actor_id_);
    return;
  }

  // 公会扩展是否已存在
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request.actor_id_);
  if (guild_actor != NULL) {
    LOG_ERROR("GuildActor(%lu) already exists.", request.actor_id_);
    return;
  }

  // 分配公会玩家对象
  guild_actor = GuildActorPool::GetInstance()->Allocate();
  if (NULL == guild_actor) {
    LOG_ERROR("Allocate GuildActor(%lu) from GuildActorPool failed.", request.actor_id_);
    return;
  }

  // 初始化
  if (guild_actor->Initialize(brief_actor, response.field_) == false) {
    GuildActorPool::GetInstance()->Deallocate(guild_actor);
    LOG_ERROR("Init GuildActor(%lu) failed.", request.actor_id_);
    return;
  }

  // 加入管理器
  if (GuildActorManager::GetInstance()->Add(guild_actor) == false) {
    guild_actor->Finalize();
    GuildActorPool::GetInstance()->Deallocate(guild_actor);
    LOG_ERROR("Add GuildActor(%lu) to GuildActorManager failed.", request.actor_id_);
    return;
  }

  // 如果数据库未找到则立即保存
  if (database::protocol::GuildActorLoginResult::NOT_FOUND == response.result_) {
    guild_actor->SetDirty();
    guild_actor->Save();
  }

  // 执行上线操作
  guild_actor->Online(session_actor);
  // 发送加载完成事件
  guild_actor->SendLoadFinishEvent();
}

}  // namespace guild

}  // namespace server

}  // namespace session

