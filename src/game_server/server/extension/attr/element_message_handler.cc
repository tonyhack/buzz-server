#include "game_server/server/extension/attr/element_message_handler.h"

#include <cstdlib>
#include <boost/bind.hpp>

#include "entity/inform_types.h"
#include "global/types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "game_server/server/game_server.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/element_actor.h"

namespace game {

namespace server {

namespace attr {

ElementMessageHandler::ElementMessageHandler() {}
ElementMessageHandler::~ElementMessageHandler() {}

bool ElementMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_UPGRADE_REQUEST,
      boost::bind(&ElementMessageHandler::OnMessageAttrElementUpgradeRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_REQUEST,
      boost::bind(&ElementMessageHandler::OnMessageAttrElementCompareRequest, this, _1, _2, _3));

  return true;
}

void ElementMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_REQUEST);
}

static void SendMessageAttrElementUpgradeResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrElementUpgradeResponse message;
  message.__set_result_(false);
  attr_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_UPGRADE_RESPONSE);
}

static void SendMessageAttrElementCompareResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrElementCompareResponse message;
  message.__set_result_(false);
  attr_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_RESPONSE);
}

void ElementMessageHandler::OnMessageAttrElementUpgradeRequest(core::uint64 id,
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
  ElementActor *element_actor = attr_actor->GetElementActor();

  // 消息解析
  gateway::protocol::MessageAttrElementUpgradeRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrElementUpgradeRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_ELEMENT_UPGRADE_REQUEST from actor(%lu, %s), level_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(), message.level_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::ELEMENT_MODULE) == false) {
    LOG_WARNING("ELEMENT_MODULE is disable.");
    SendMessageAttrElementUpgradeResponseFailed(attr_actor);
    return;
  }

  // 检查协议参数
  const ElementCell *cell = Configure::GetInstance()->GetElement(message.level_);
  if (NULL == cell ||
      cell != Configure::GetInstance()->GetNextElement(element_actor->GetCurrentLevel())) {
    LOG_WARNING("message.level_=[%d] is invalid.", message.level_);
    SendMessageAttrElementUpgradeResponseFailed(attr_actor);
    return;
  }

  // 检查消耗
  // --检查神威
  if (game_actor->GetAttribute(entity::ActorClientFields::WAR_SOUL) < cell->spend_war_soul_) {
    LOG_WARNING("WarSoul is not enough.");
    SendMessageAttrElementUpgradeResponseFailed(attr_actor);
    return;
  }
  
  // --扣除消耗
  if (cell->spend_war_soul_ > 0) {
    game_actor->AddResource(entity::ResourceID::WAR_SOUL, 0 - cell->spend_war_soul_);
  }

  element_actor->SetCurrentLevel(message.level_);

  const ElementCell *next_cell =
    Configure::GetInstance()->GetNextElement(element_actor->GetCurrentLevel());
  if (NULL == next_cell) {
    // 世界公告: 元素升到顶级
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::ATTR_ELEMENT_TOP_LEVEL);
    inform.params_.push_back(attr_actor->GetStringID());
    inform.params_.push_back(attr_actor->GetName());
    inform.params_.push_back(global::ToString(element_actor->GetCurrentLevel()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  } else if (cell->id_ / 100 != next_cell->id_ / 100) {
    // 世界公告: 元素一层升满
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::ATTR_ELEMENT_UPGRADE);
    inform.params_.push_back(attr_actor->GetStringID());
    inform.params_.push_back(attr_actor->GetName());
    inform.params_.push_back(global::ToString(element_actor->GetCurrentLevel()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }
}

void ElementMessageHandler::OnMessageAttrElementCompareRequest(core::uint64 id,
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
  gateway::protocol::MessageAttrElementCompareRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrElementCompareRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_ELEMENT_COMPARE_REQUEST from actor(%lu, %s), actor_id_=[%s].",
           attr_actor->GetID(), attr_actor->GetName().c_str(), message.actor_id_.c_str());

  // 获取被比较者属性玩家对象
  core::uint64 compare_attr_actor_id = atol(message.actor_id_.c_str());
  AttrActor *compare_attr_actor = AttrActorManager::GetInstance()->Get(compare_attr_actor_id);
  if (NULL == compare_attr_actor) {
    LOG_WARNING("Get Compare AttrActor(%lu) from AttrActorManager failed.",
        compare_attr_actor_id);
    SendMessageAttrElementCompareResponseFailed(attr_actor);
    return;
  }
  ElementActor *compare_element_actor = compare_attr_actor->GetElementActor();

  if (compare_element_actor->GetCurrentLevel() == 0) {
    gateway::protocol::MessageAttrElementCompareResponse response;
    response.__set_result_(true);
    response.__set_level_(0);
    response.__set_max_hp_add_value_(0);
    response.__set_max_mp_add_value_(0);
    response.__set_physics_attack_add_value_(0);
    response.__set_physics_defence_add_value_(0);
    response.__set_magic_attack_add_value_(0);
    response.__set_magic_defence_add_value_(0);
    response.__set_dodge_add_value_(0);
    response.__set_crit_add_value_(0);
    response.__set_ignore_defence_add_value_(0);

    attr_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_RESPONSE);
  } else {
    const ElementCell *cell = Configure::GetInstance()->GetElement(
        compare_element_actor->GetCurrentLevel());
    if (NULL == cell) {
      LOG_ERROR("ElementCell(%d) not found in config.",
          compare_element_actor->GetCurrentLevel());
      SendMessageAttrElementCompareResponseFailed(attr_actor);
      return;
    }

    gateway::protocol::MessageAttrElementCompareResponse response;
    response.__set_result_(true);
    response.__set_level_(compare_element_actor->GetCurrentLevel());
    response.__set_max_hp_add_value_(cell->max_hp_add_value_);
    response.__set_max_mp_add_value_(cell->max_mp_add_value_);
    response.__set_physics_attack_add_value_(cell->physics_attack_add_value_);
    response.__set_physics_defence_add_value_(cell->physics_defence_add_value_);
    response.__set_magic_attack_add_value_(cell->magic_attack_add_value_);
    response.__set_magic_defence_add_value_(cell->magic_defence_add_value_);
    response.__set_dodge_add_value_(cell->dodge_add_value_);
    response.__set_crit_add_value_(cell->crit_add_value_);
    response.__set_ignore_defence_add_value_(cell->ignore_defence_add_value_);

    attr_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_RESPONSE);
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

