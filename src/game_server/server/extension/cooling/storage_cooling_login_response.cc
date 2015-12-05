//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 22:52:04.
// File name: storage_cooling_login_response.cc
//
// Description:
// Define class StorageCoolingLoginResponse.
//

#include "game_server/server/extension/cooling/storage_cooling_login_response.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_cooling_login_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/cooling/cooling_role.h"
#include "game_server/server/extension/cooling/cooling_role_manager.h"
#include "game_server/server/extension/cooling/cooling_role_pool.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/logging.h"
#include "global/global_packet.h"

// ------------- [GameActor::SendMessageAround测试代码] ------------------
/*
// TODO: 删除这里的测试代码
#include "gateway_server/protocol/gateway_protocol_types.h"
//*/
// ------------- [GameActor::SendMessageAround测试代码] ------------------

namespace game {

namespace server {

namespace cooling {

StorageCoolingLoginResponse::StorageCoolingLoginResponse() {}
StorageCoolingLoginResponse::~StorageCoolingLoginResponse() {}

bool StorageCoolingLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_COOLING_LOGIN, boost::bind(
        &StorageCoolingLoginResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageCoolingLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_COOLING_LOGIN);
}

void StorageCoolingLoginResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageCoolingLoginRequest request;
  database::protocol::StorageCoolingLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%ld) 冷却扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }
  if(response.result_ != database::protocol::CoolingLoginResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%ld) 冷却扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 冷却扩展登陆失败，获取 GameActor失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 冷却玩家对象是否已存在管理器中
  CoolingRole *role = CoolingRoleManager::GetInstance()->GetActor(request.id_);
  if(role) {
    global::LogError("%s:%d (%s) 玩家(%lu) 冷却扩展已存在 CoolingRoleManager",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 从对象池中分配一个冷却玩家对象
  role = CoolingRolePool::GetInstance()->Allocate();
  if(role == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 分配 CoolingRole 失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 初始化冷却玩家对象
  if(role->Initialize(actor, response.fields_) == false) {
    CoolingRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) 玩家(%lu) 初始化 CoolingRole 失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 加入冷却玩家对象管理器
  if(CoolingRoleManager::GetInstance()->AddActor(role) == false) {
    role->Finalize();
    CoolingRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) 玩家(%lu) 加入 CoolingRole 到管理器失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(request.id_);
  load_finish.__set_extension_type_(ExtensionType::COOLING);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, request.id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  // ------------- [GameActor::SendMessageAround测试代码] ------------------
  /*
  // TODO: 删除这里的测试代码
  gateway::protocol::MessageTest test;
  test.__set_serialize_(1);
  actor->SendMessageAround(test, gateway::protocol::MessageType::MESSAGE_TEST);
  actor->SetAttribute(entity::RoleAoiFields::CURRENT_HP, 100);
  //*/
  // ------------- [GameActor::SendMessageAround测试代码] ------------------

  global::LogDebug("玩家(%lu, %s) 冷却扩展加载成功", actor->GetID(),
      actor->GetName().c_str());
}

}  // namespace cooling

}  // namespace server

}  // namespace game

