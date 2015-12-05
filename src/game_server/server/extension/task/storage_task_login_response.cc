//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-13 17:19:28.
// File name: storage_task_login_response.cc
//
// Description:
// Define class StorageTaskLoginResponse.
//

#include "game_server/server/extension/task/storage_task_login_response.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_task_actor_save_types.h"
#include "database_server/protocol/storage_task_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/extension/task/task_actor_pool.h"
#include "global/global_packet.h"
#include "game_server/server/global_time_tick.h"

namespace game {

namespace server {

namespace task {

StorageTaskLoginResponse::StorageTaskLoginResponse() {}
StorageTaskLoginResponse::~StorageTaskLoginResponse() {}

bool StorageTaskLoginResponse::Initialize() {
  // 任务登陆成功事件
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_TASK_LOGIN, boost::bind(
        &StorageTaskLoginResponse::OnResponse, this, _1, _2, _3));

  // 任务登陆成功事件
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_TASK_ACTOR_LOGIN, boost::bind(
        &StorageTaskLoginResponse::OnActorResponse, this, _1, _2, _3));
  return true;
}

void StorageTaskLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_TASK_LOGIN);
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_TASK_ACTOR_LOGIN);
}

void StorageTaskLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageTaskLoginRequest request;
  database::protocol::StorageTaskLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, result);
    return ;
  }
  if(response.result_ != database::protocol::TaskLoginResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, response.result_);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }

  // 任务扩展是否已存在
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(request.actor_id_);
  if(task_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }

  // 初始化
  if(task_actor->InitializeTasks(response.actives_, response.finishes_) == false) {
    TaskActorManager::GetInstance()->Remove(task_actor->GetID());
    task_actor->Finalize();
    TaskActorPool::GetInstance()->Deallocate(task_actor);
    global::LogError("%s:%d (%s) 玩家(%lu) 初始化 TaskActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }
  
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(request.actor_id_);
  load_finish.__set_extension_type_(ExtensionType::TASK);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, request.actor_id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu, %s) 任务扩展加载成功",
      actor->GetID(), actor->GetName().c_str());
}

void StorageTaskLoginResponse::OnActorResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageTaskActorLoginRequest request;
  database::protocol::StorageTaskActorLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, result);
    return ;
  }
  if(response.result_ != database::protocol::TaskActorResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, response.result_);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }

  // 任务扩展是否已存在
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(request.actor_id_);
  if(task_actor) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展已存在",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }

  // 分配任务玩家对象
  task_actor = TaskActorPool::GetInstance()->Allocate();
  if(task_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 分配任务玩家对象失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }

  // 初始化
  if(task_actor->Initialize(actor, response.data_) == false) {
    TaskActorPool::GetInstance()->Deallocate(task_actor);
    global::LogError("%s:%d (%s) 玩家(%lu) 初始化 TaskActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }

  // 加入到管理器中
  if(TaskActorManager::GetInstance()->Add(task_actor) == false) {
    actor->Finalize();
    TaskActorPool::GetInstance()->Deallocate(task_actor);
    global::LogError("%s:%d (%s) 玩家(%lu) 加入到 TaskActorManager 失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }
  
  global::LogDebug("玩家(%lu, %s) 任务扩展加载成功,玩家任务数据加载完毕",
      actor->GetID(), actor->GetName().c_str());

  // 向数据库请求任务数据
  database::protocol::StorageTaskLoginRequest request_task;
  request_task.__set_actor_id_(actor->GetActorID());
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request_task, database::protocol::StorageType::STORAGE_TASK_LOGIN,
        actor->GetActorID()) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

}  // namespace task

}  // namespace server

}  // namespace game

