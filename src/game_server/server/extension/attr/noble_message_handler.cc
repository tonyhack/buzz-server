#include "game_server/server/extension/attr/noble_message_handler.h"

#include <boost/bind.hpp>

#include "global/global_packet.h"
#include "global/logging.h"
#include "global/types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/facade_request.h"
#include "game_server/server/extension/attr/noble_actor.h"

namespace game {

namespace server {

namespace attr {

NobleMessageHandler::NobleMessageHandler() {}
NobleMessageHandler::~NobleMessageHandler() {}

bool NobleMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_PROMOTE_REQUEST,
      boost::bind(&NobleMessageHandler::OnMessageAttrNoblePromoteRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_BREAKTHROUGH_REQUEST,
      boost::bind(&NobleMessageHandler::OnMessageAttrNobleBreakThroughRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_COMPARE_REQUEST,
      boost::bind(&NobleMessageHandler::OnMessageAttrNobleCompareRequest, this, _1, _2, _3));

  return true;
}

void NobleMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_PROMOTE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_BREAKTHROUGH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_COMPARE_REQUEST);
}

static void SendMessageAttrNoblePromoteResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrNoblePromoteResponse message;
  message.__set_result_(false);
  attr_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_PROMOTE_RESPONSE);
}

static void SendMessageAttrNobleBreakthroughResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrNobleBreakthroughResponse message;
  message.__set_result_(false);
  attr_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_BREAKTHROUGH_RESPONSE);
}

static void SendMessageAttrNobleCompareResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrNobleCompareResponse message;
  message.__set_result_(false);
  attr_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_COMPARE_RESPONSE);
}

void NobleMessageHandler::OnMessageAttrNoblePromoteRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  NobleActor *noble_actor = attr_actor->GetNobleActor();

  LOG_INFO("Receive MESSAGE_ATTR_NOBLE_PROMOTE_REQUEST from actor(%lu, %s).",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::NOBLE_MODULE) == false) {
    LOG_WARNING("NOBLE_MODULE is disable.");
    SendMessageAttrNoblePromoteResponseFailed(attr_actor);
    return;
  }

  const NobleCell *cell =
    Configure::GetInstance()->GetNoble(noble_actor->GetCurrentLevel());
  if (NULL == cell) {
    LOG_ERROR("NobleCell(%d) not found in config.", noble_actor->GetCurrentLevel());
    SendMessageAttrNoblePromoteResponseFailed(attr_actor);
    return;
  }

  // 检查消耗
  // --检查灵气
  if (game_actor->GetAttribute(entity::ActorClientFields::NIMBUS) < cell->spend_nimbus_) {
    LOG_WARNING("Nimbus is not enough.");
    SendMessageAttrNoblePromoteResponseFailed(attr_actor);
    return;
  }

  // 检查是否已到提升上限
  if (noble_actor->CheckPromoteFull()) {
    LOG_WARNING("Promote full.");
    SendMessageAttrNoblePromoteResponseFailed(attr_actor);
    return;
  }

  // 扣除消耗
  if (cell->spend_nimbus_ > 0) {
    game_actor->AddResource(entity::ResourceID::NIMBUS, 0 - cell->spend_nimbus_);
  }

  noble_actor->Promote();
}

void NobleMessageHandler::OnMessageAttrNobleBreakThroughRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  NobleActor *noble_actor = attr_actor->GetNobleActor();

  // 消息解析
  gateway::protocol::MessageAttrNobleBreakthroughRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrNobleBreakthroughRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_NOBLE_BREAKTHROUGH_REQUEST from actor(%lu, %s), use_item_count=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(), message.use_item_count_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::NOBLE_MODULE) == false) {
    LOG_WARNING("NOBLE_MODULE is disable.");
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }

  const NobleCell *cell =
    Configure::GetInstance()->GetNoble(noble_actor->GetCurrentLevel());
  if (NULL == cell) {
    LOG_ERROR("NobleCell(%d) not found in config.", noble_actor->GetCurrentLevel());
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }
  if (Configure::GetInstance()->GetNextNoble(noble_actor->GetCurrentLevel()) == NULL) {
    LOG_ERROR("NobleCell(next %d) not found in config.", noble_actor->GetCurrentLevel());
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }

  // 检查协议参数
  if (message.use_item_count_ < 0 ||
      noble_actor->GetUsedItemCount() + message.use_item_count_ > cell->spend_item_count_) {
    LOG_WARNING("message.use_item_count_=[%d] is invalid.", message.use_item_count_);
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }

  // 提升到上限才能突破
  if (noble_actor->CheckPromoteFull() == false) {
    LOG_WARNING("Do not Promote full.");
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }

  // 检查消耗
  // --检查道具
  if (FacadeRequest::GetInstance()->CheckPacketItemExist(id, cell->spend_item_id_,
          message.use_item_count_, BindType::BOTH) == false) {
    LOG_WARNING("Item(%u) is not enough.", cell->spend_item_id_);
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }
  // --检查金币
  core::int32 spend_gold = cell->spend_gold_ * message.use_item_count_;
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) < spend_gold) {
    LOG_WARNING("Gold is not enough.");
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }

  // 扣除消耗
  core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(id,
      cell->spend_item_id_, message.use_item_count_, BindType::BOTH);
  if (delete_number != (core::uint32)message.use_item_count_) {
    LOG_ERROR("Remove item(%d) failed, req_count=[%d], del_count=[%d].",
              cell->spend_item_id_, message.use_item_count_, delete_number);
    SendMessageAttrNobleBreakthroughResponseFailed(attr_actor);
    return;
  }
  if (spend_gold > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - cell->spend_gold_);
  }

  noble_actor->Breakthrough(message.use_item_count_);
}

void NobleMessageHandler::OnMessageAttrNobleCompareRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAttrNobleCompareRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrNobleCompareRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_NOBLE_COMPARE_REQUEST from actor(%lu, %s), actor_id_=[%s].",
           attr_actor->GetID(), attr_actor->GetName().c_str(), request.actor_id_.c_str());

  // 获取被比较者属性玩家对象
  core::uint64 compare_attr_actor_id = atol(request.actor_id_.c_str());
  AttrActor *compare_attr_actor = AttrActorManager::GetInstance()->Get(compare_attr_actor_id);
  if (NULL == compare_attr_actor) {
    LOG_WARNING("Get Compare AttrActor(%lu) from AttrActorManager failed.",
        compare_attr_actor_id);
    SendMessageAttrNobleCompareResponseFailed(attr_actor);
    return;
  }
  NobleActor *compare_noble_actor = compare_attr_actor->GetNobleActor();

  gateway::protocol::MessageAttrNobleCompareResponse response;
  response.__set_result_(true);
  response.__set_level_(compare_noble_actor->GetCurrentLevel());
  response.__set_max_hp_add_value_(compare_noble_actor->GetMaxHpAddValue());
  response.__set_max_mp_add_value_(compare_noble_actor->GetMaxMpAddValue());
  response.__set_physics_attack_add_value_(compare_noble_actor->GetPhysicsAttackAddValue());
  response.__set_physics_defence_add_value_(compare_noble_actor->GetPhysicsDefenceAddValue());
  response.__set_magic_attack_add_value_(compare_noble_actor->GetMagicAttackAddValue());
  response.__set_magic_defence_add_value_(compare_noble_actor->GetMagicDefenceAddValue());
  response.__set_dodge_add_value_(compare_noble_actor->GetDodgeAddValue());
  response.__set_crit_add_value_(compare_noble_actor->GetCritAddValue());
  response.__set_ignore_defence_add_value_(compare_noble_actor->GetIgnoreDefenceAddValue());

  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_COMPARE_RESPONSE);
}


}  // namespace attr

}  // namespace server

}  // namespace game

