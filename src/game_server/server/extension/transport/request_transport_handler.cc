//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 03:27:26.
// File name: request_transport_handler.cc
//
// Description:
// Define class RequestTransportHandler.
//

#include "game_server/server/extension/transport/request_transport_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/transport_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/game_server.h"
#include "game_server/server/extension/transport/transport_actor.h"
#include "game_server/server/extension/transport/transport_actor_manager.h"
#include "game_server/server/extension/transport/transport_npc.h"
#include "game_server/server/extension/transport/transport_npc_manager.h"
#include "game_server/server/extension/transport/transport_npc_pool.h"
#include "game_server/server/extension/transport/facade_request.h"
#include "game_server/server/request/game_transport_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace transport {

RequestTransportHandler::RequestTransportHandler() {}
RequestTransportHandler::~RequestTransportHandler() {}

bool RequestTransportHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_SUMMON,
      boost::bind(&RequestTransportHandler::OnRequestTransportSummon,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_REMOVE,
      boost::bind(&RequestTransportHandler::OnRequestTransportRemove,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_CURRENT_NPC,
      boost::bind(&RequestTransportHandler::OnRequestTransportCurrentNpc,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_AROUND,
      boost::bind(&RequestTransportHandler::OnRequestTransportAround,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_GET_SNAPSHOT,
      boost::bind(&RequestTransportHandler::OnRequestTransportGetSnapshot,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_SUMMON_FROM_SNAPSHOT,
      boost::bind(&RequestTransportHandler::OnRequestTransportSummonFromSnapshot,
        this, _1, _2));

  return true;
}

void RequestTransportHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_SUMMON);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_REMOVE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_CURRENT_NPC);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_AROUND);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_GET_SNAPSHOT);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TRANSPORT_SUMMON_FROM_SNAPSHOT);
}

int RequestTransportHandler::OnRequestTransportSummon(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestTransportSummon) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTransportSummon *request = (request::RequestTransportSummon *)message;

  TransportActor *transport_actor =
    TransportActorManager::GetInstance()->Get(request->actor_);
  if (transport_actor == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败");
    return -1;
  }

  if (transport_actor->GetTransportNpc() != 0) {
    LOG_ERROR("玩家正在运送状态");
    return -1;
  }

  GameActor *game_actor = transport_actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败");
    return -1;
  }

  core::uint64 npc_id = FacadeRequest::GetInstance()->SummonNpc(game_actor->GetScene(),
      request->transport_npc_template_, game_actor->GetPosx(),
      game_actor->GetPosy(), 2, 2, entity::DirectionType::NONE);
  if (npc_id == 0) {
    LOG_ERROR("请求召唤NPC失败");
    return -1;
  }

  GameNpc *game_npc = GameNpcManager::GetInstance()->Get(npc_id);
  if (game_npc == NULL) {
    LOG_ERROR("获取 GameNpc(%lu) 失败");
    return -1;
  }

  if (FacadeRequest::GetInstance()->SetNpcTarget(npc_id,
        entity::EntityType::TYPE_ACTOR, game_actor->GetID(), true) == false) {
    LOG_ERROR("请求设置NPC目标失败");
    return -1;
  }

  TransportNpc *transport_npc = TransportNpcPool::GetInstance()->Allocate();
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败");
    return -1;
  }

  if (transport_npc->Initialize(game_npc) == false) {
    TransportNpcPool::GetInstance()->Deallocate(transport_npc);
    LOG_ERROR("初始化 TransportNpc(%lu) 失败", game_npc->GetID());
    return -1;
  }

  if (TransportNpcManager::GetInstance()->Add(transport_npc) == false) {
    transport_npc->Finalize();
    TransportNpcPool::GetInstance()->Deallocate(transport_npc);
    LOG_ERROR("TransportNpc(%lu) 加入 TransportNpcManager 失败", game_npc->GetID());
    return -1;
  }

  if (TransportActorManager::GetInstance()->AddTransport(game_actor->GetID(),
        game_npc->GetID()) == false) {
    TransportNpcManager::GetInstance()->Remove(transport_npc->GetID());
    transport_npc->Finalize();
    TransportNpcPool::GetInstance()->Deallocate(transport_npc);
    FacadeRequest::GetInstance()->RemoveNpc(npc_id);
    LOG_ERROR("TransportActorManager AddTransport(%lu,%lu) 失败", game_actor->GetID(),
        game_npc->GetID());
    return -1;
  }

  transport_npc->SetMasterActor(game_actor->GetID());
  transport_npc->SetMasterName(game_actor->GetName());
  transport_actor->SetTransportNpc(game_npc->GetID());

  request->__set_transport_npc_(npc_id);

  return 0;
}

int RequestTransportHandler::OnRequestTransportRemove(void *message,size_t size) {
  if (message == NULL || sizeof(request::RequestTransportRemove) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTransportRemove *request = (request::RequestTransportRemove *)message;

  core::uint64 transport_npc_id = 0;

  if (request->__isset.actor_ == true) {
    TransportActor *transport_actor =
      TransportActorManager::GetInstance()->Get(request->actor_);
    if (transport_actor == NULL) {
      LOG_ERROR("获取 TransportActor(%lu) 失败");
      return -1;
    }

    transport_npc_id = transport_actor->GetTransportNpc();
    if (transport_npc_id == 0) {
      return 0;
    }
  } else if (request->__isset.transport_npc_ == true) {
    transport_npc_id = request->transport_npc_;
  } else {
    LOG_ERROR("请求参数错误");
    return -1;
  }

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(transport_npc_id);
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportNpc(%lu) 失败", transport_npc_id);
    return -1;
  }

  core::uint64 transport_actor_id = transport_npc->GetMasterActor();

  if (FacadeRequest::GetInstance()->RemoveNpc(transport_npc_id) == false) {
    LOG_ERROR("请求删除NPC失败");
    return -1;
  }

  transport_npc->Finalize();
  TransportNpcManager::GetInstance()->Remove(transport_npc_id);
  TransportNpcPool::GetInstance()->Deallocate(transport_npc);
  TransportActorManager::GetInstance()->RemoveTransport(transport_actor_id);

  TransportActor *transport_actor =
    TransportActorManager::GetInstance()->Get(transport_actor_id);
  if (transport_actor != NULL) {
    transport_actor->SetTransportNpc(0);
  }

  return 0;
}

int RequestTransportHandler::OnRequestTransportCurrentNpc(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestTransportCurrentNpc) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTransportCurrentNpc *request =
    (request::RequestTransportCurrentNpc *)message;

  TransportActor *transport_actor =
    TransportActorManager::GetInstance()->Get(request->actor_);
  if (transport_actor == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败");
    return -1;
  }

  request->__set_transport_npc_(0);
  request->__set_transport_npc_template_(0);

  core::uint64 transport_npc_id = transport_actor->GetTransportNpc();
  if (transport_npc_id == 0) {
    return 0;
  }

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(transport_npc_id);
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportNpc(%lu) 失败", transport_npc_id);
    return -1;
  }

  request->__set_transport_npc_(transport_npc_id);
  request->__set_transport_npc_template_(transport_npc->GetNpc()->GetTemplateID());

  return 0;
}

int RequestTransportHandler::OnRequestTransportAround(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestTransportAround) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTransportAround *request = (request::RequestTransportAround *)message;

  TransportActor *transport_actor =
    TransportActorManager::GetInstance()->Get(request->actor_);
  if (transport_actor == NULL) {
    LOG_ERROR("获取 TransportActor 失败");
    return -1;
  }

  core::uint64 transport_npc_id = transport_actor->GetTransportNpc();
  if (transport_npc_id == 0) {
    request->__set_around_(false);
    return 0;
  }

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(transport_npc_id);
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportNpc(%lu) 失败", transport_npc_id);
    return -1;
  }

  request->__set_around_(transport_npc->GetLost() == false);

  return 0;
}

int RequestTransportHandler::OnRequestTransportGetSnapshot(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestTransportGetSnapshot) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTransportGetSnapshot *request = (request::RequestTransportGetSnapshot *)message;

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(request->transport_npc_id_);
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportNpc(%lu) 失败", request->transport_npc_id_);
    return -1;
  }

  entity::TransportSnapshotField snapshot;
  snapshot.__set_actor_id_(transport_npc->GetMasterActor());
  snapshot.__set_actor_name_(transport_npc->GetMasterName());

  size_t size_str = 0;
  const char *serialize_str = global::GlobalPacket::GetPacket()->Serialize(snapshot, size_str);
  if (serialize_str == NULL) {
    LOG_ERROR("序列化失败");
    return -1;
  }

  request->snapshot_.assign(serialize_str, size_str);

  return 0;
}

int RequestTransportHandler::OnRequestTransportSummonFromSnapshot(void *message,
    size_t size) {
  if (message == NULL || sizeof(request::RequestTransportSummonFromSnapshot) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestTransportSummonFromSnapshot *request =
    (request::RequestTransportSummonFromSnapshot *)message;

  entity::TransportSnapshotField snapshot;
  if (global::GlobalPacket::GetPacket()->Deserialize(snapshot, request->snapshot_) == false) {
    LOG_ERROR("反序列化失败");
    return -1;
  }

  core::uint64 npc_id = FacadeRequest::GetInstance()->SummonNpc(request->scene_,
      request->transport_npc_template_, request->x_, request->y_,
      2, 2, entity::DirectionType::NONE);
  if (npc_id == 0) {
    LOG_ERROR("请求召唤NPC失败");
    return -1;
  }

  GameNpc *game_npc = GameNpcManager::GetInstance()->Get(npc_id);
  if (game_npc == NULL) {
    LOG_ERROR("获取 GameNpc(%lu) 失败");
    return -1;
  }

  if (FacadeRequest::GetInstance()->SetNpcTarget(npc_id,
        entity::EntityType::TYPE_ACTOR, snapshot.actor_id_, true) == false) {
    LOG_ERROR("请求设置NPC目标失败");
    return -1;
  }

  TransportNpc *transport_npc = TransportNpcPool::GetInstance()->Allocate();
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败");
    return -1;
  }

  if (transport_npc->Initialize(game_npc) == false) {
    TransportNpcPool::GetInstance()->Deallocate(transport_npc);
    LOG_ERROR("初始化 TransportNpc(%lu) 失败", game_npc->GetID());
    return -1;
  }

  if (TransportNpcManager::GetInstance()->Add(transport_npc) == false) {
    transport_npc->Finalize();
    TransportNpcPool::GetInstance()->Deallocate(transport_npc);
    LOG_ERROR("TransportNpc(%lu) 加入 TransportNpcManager 失败", game_npc->GetID());
    return -1;
  }

  if (TransportActorManager::GetInstance()->AddTransport(snapshot.actor_id_,
        game_npc->GetID()) == false) {
    TransportNpcManager::GetInstance()->Remove(transport_npc->GetID());
    transport_npc->Finalize();
    TransportNpcPool::GetInstance()->Deallocate(transport_npc);
    FacadeRequest::GetInstance()->RemoveNpc(npc_id);
    LOG_ERROR("TransportActorManager AddTransport(%lu,%lu) 失败", snapshot.actor_id_,
        game_npc->GetID());
    return -1;
  }

  transport_npc->SetMasterActor(snapshot.actor_id_);
  transport_npc->SetMasterName(snapshot.actor_name_);

  TransportActor *transport_actor =
    TransportActorManager::GetInstance()->Get(snapshot.actor_id_);
  if (transport_actor != NULL) {
    transport_actor->SetTransportNpc(transport_npc->GetID());
  }

  request->__set_transport_npc_(npc_id);

  return 0;
}

}  // namespace transport

}  // namespace server

}  // namespace game

