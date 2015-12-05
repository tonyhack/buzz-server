//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 01:01:36.
// File name: transport_npc.cc
//
// Description:
// Define class TransportNpc.
//

#include "game_server/server/extension/transport/transport_npc.h"

#include "game_server/server/extension/transport/transport_destory_checker.h"
#include "game_server/server/extension/transport/facade_request.h"

namespace game {

namespace server {

namespace transport {

TransportNpc::TransportNpc() {}
TransportNpc::~TransportNpc() {}

bool TransportNpc::Initialize(GameNpc *npc) {
  if (npc == NULL) {
    LOG_ERROR("npc is null.");
    return false;
  }

  this->npc_ = npc;
  this->master_actor_ = 0;
  this->master_name_ = "";
  this->lost_ = false;

  return true;
}

void TransportNpc::Finalize() {
  this->npc_ = NULL;
}

void TransportNpc::Lose() {
  if (FacadeRequest::GetInstance()->SetNpcTarget(this->GetID(),
        entity::EntityType::TYPE_MAX, 0, true) == false) {
    LOG_ERROR("请求设置NPC目标失败");
    return;
  }
  this->SetLost(true);
  TransportDestoryChecker::GetInstance()->Add(this->GetID());
  LOG_DEBUG("TransportNpc(%lu) Lose");
}

void TransportNpc::Regain() {
  if (FacadeRequest::GetInstance()->SetNpcTarget(this->GetID(),
        entity::EntityType::TYPE_ACTOR, this->master_actor_, true) == false) {
    LOG_ERROR("请求设置NPC目标失败");
    return;
  }
  this->SetLost(false);
  TransportDestoryChecker::GetInstance()->Remove(this->GetID());
  LOG_DEBUG("TransportNpc(%lu) Regain");
}

}  // namespace transport

}  // namespace server

}  // namespace game

