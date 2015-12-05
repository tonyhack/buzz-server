//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 22:52:04.
// File name: storage_buff_login_response.cc
//
// Description:
// Define class StorageBuffLoginResponse.
//

#include "game_server/server/extension/buff/storage_buff_login_response.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_buff_login_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/buff/buff_constants.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "game_server/server/extension/buff/buff_role_pool.h"
#include "game_server/server/extension/buff/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_buff_protocol_types.h"
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

namespace buff {

StorageBuffLoginResponse::StorageBuffLoginResponse() {}
StorageBuffLoginResponse::~StorageBuffLoginResponse() {}

bool StorageBuffLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_BUFF_LOGIN, boost::bind(
        &StorageBuffLoginResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageBuffLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_BUFF_LOGIN);
}

void StorageBuffLoginResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageBuffLoginRequest request;
  database::protocol::StorageBuffLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%ld) buff扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }
  if(response.result_ != database::protocol::BuffLoginResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%ld) buff扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) buff扩展登陆失败，获取 GameActor失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // buff玩家对象是否已存在管理器中
  BuffRole *role = BuffRoleManager::GetInstance()->GetActor(request.id_);
  if(role) {
    global::LogError("%s:%d (%s) 玩家(%lu) buff扩展已存在 BuffRoleManager",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 从对象池中分配一个buff玩家对象
  role = BuffRolePool::GetInstance()->Allocate();
  if(role == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 分配 BuffRole 失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 初始化buff玩家对象
  if(role->Initialize(actor) == false) {
    BuffRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) 玩家(%lu) 初始化 BuffRole 失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 加入buff玩家对象管理器
  if(BuffRoleManager::GetInstance()->AddActor(role) == false) {
    role->Finalize();
    BuffRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) 玩家(%lu) 加入 BuffRole 到管理器失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 初始数据
  if(role->InitializeBuffs(response.fields_) == false) {
    BuffRoleManager::GetInstance()->RemoveActor(role->GetID());
    role->Finalize();
    BuffRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) 玩家(%lu) 初始化 BUFF列表 失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  gateway::protocol::MessageBuffInitialize message;
  message.roles_.resize(1);
  message.roles_[0].__set_type_(entity::EntityType::TYPE_ACTOR);
  message.roles_[0].__set_id_(actor->GetStringID());
  std::vector<database::protocol::StorageBuffField>::iterator it =
    response.fields_.begin();
  for(; it != response.fields_.end(); it++) {
    const BuffCell *cell = Configure::GetInstance()->GetBuffCell(it->buff_id_);
    if(cell == NULL/* || cell->boardcast_type_mark_ == 0*/) {
      continue;
    }
    gateway::protocol::BuffField field;
    field.buff_ = it->buff_id_;
    field.left_time_ = it->left_time_;
    field.stack_count_ = it->stack_count_;
    message.roles_[0].buffs_.push_back(field);
  }

  // 广播
  actor->SendMessageAround(message,
      gateway::protocol::MessageType::MESSAGE_BUFF_INITIALIZE, true);


  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(request.id_);
  load_finish.__set_extension_type_(ExtensionType::BUFF);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, request.id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu, %s) buff扩展加载成功",
      actor->GetID(), actor->GetName().c_str());
}

}  // namespace buff

}  // namespace server

}  // namespace game

