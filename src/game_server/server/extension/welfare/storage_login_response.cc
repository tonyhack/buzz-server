//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-13 17:19:28.
// File name: storage_welfare_login_response.cc
//
// Description:
// Define class StorageLoginResponse.
//

#include "game_server/server/extension/welfare/storage_login_response.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_achieve_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "game_server/server/extension/welfare/welfare_actor_pool.h"
#include "global/global_packet.h"
#include "game_server/server/global_time_tick.h"

namespace game {

namespace server {

namespace welfare {

StorageLoginResponse::StorageLoginResponse() {}
StorageLoginResponse::~StorageLoginResponse() {}

bool StorageLoginResponse::Initialize() {
  // 成就登陆成功事件
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ACHIEVE_LOGIN, boost::bind(
        &StorageLoginResponse::OnResponse, this, _1, _2, _3));

  // 玩家福利登陆成功事件
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_WELFARE_LOAD, boost::bind(
        &StorageLoginResponse::OnActorResponse, this, _1, _2, _3));

  return true;
}

void StorageLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ACHIEVE_LOGIN);
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_WELFARE_LOAD);
}

void StorageLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageAchieveLoginRequest request;
  database::protocol::StorageAchieveLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_, result);
    return ;
  }
  if(response.result_ != database::protocol::AchieveLoginResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_, response.result_);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_);
    return ;
  }

  WelfareActor *welfare_actor = WelfareActorManager::GetInstance()->Get(request.actor_);
  if(welfare_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, request.actor_);
    return ;
  }

  // 初始化
  if(welfare_actor->Initialize(response.achieves_, response.finish_achieves_) == false) {
    WelfareActorManager::GetInstance()->Remove(welfare_actor->GetActorID());
    welfare_actor->Finalize();
    WelfareActorPool::GetInstance()->Deallocate(welfare_actor);
    global::LogError("%s:%d (%s) 玩家(%lu) 初始化 WelfareActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_);
    return ;
  }

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(request.actor_);
  load_finish.__set_extension_type_(ExtensionType::WELFARE);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, request.actor_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu, %s) 成就扩展加载成功",
      actor->GetID(), actor->GetName().c_str());
}

void StorageLoginResponse::OnActorResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageWelfareLoadRequest request;
  database::protocol::StorageWelfareLoadResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_, result);
    return ;
  }
  if(response.result_ != database::protocol::WelfareLoginReslut::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_, response.result_);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_);
    return ;
  }

  WelfareActor *welfare_actor = WelfareActorManager::GetInstance()->Get(request.actor_);
  if(welfare_actor != NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 成就扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, request.actor_);
    return ;
  }

  welfare_actor = WelfareActorPool::GetInstance()->Allocate();
  if(welfare_actor == NULL) {
    LOG_ERROR("成就对象加载失败");
    return ;
  }

  // 初始化成就玩家数据
  if(welfare_actor->InitializeActor(actor, response.field_) == false) {
    WelfareActorManager::GetInstance()->Remove(actor->GetActorID());
    welfare_actor->Finalize();
    WelfareActorPool::GetInstance()->Deallocate(welfare_actor);
    LOG_ERROR("初始化玩家(%lu)福利数据失败", actor->GetActorID());
    return ;
  }
  if(WelfareActorManager::GetInstance()->Add(welfare_actor) == false) {
    welfare_actor->Finalize();
    WelfareActorPool::GetInstance()->Deallocate(welfare_actor);
    LOG_ERROR("成就玩家管理器无法加入对象(%lu)", request.actor_);
    return ;
  }

  // 向数据库请求任务数据
  database::protocol::StorageAchieveLoginRequest achieve_request;
  achieve_request.__set_actor_(actor->GetActorID());
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        achieve_request, database::protocol::StorageType::STORAGE_ACHIEVE_LOGIN,
        actor->GetActorID()) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

}

}  // namespace welfare

}  // namespace server

}  // namespace game

