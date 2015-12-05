#include "game_server/server/extension/social/session_homeland_message_handler.h"

#include <algorithm>
#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_social_protocol_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_social_event_types.h"
#include "game_server/server/extension/social/facade_request.h"
#include "game_server/server/extension/social/social_actor.h"
#include "game_server/server/extension/social/social_actor_manager.h"

namespace game {

namespace server {

namespace social {

SessionHomelandMessageHandler::SessionHomelandMessageHandler() {}
SessionHomelandMessageHandler::~SessionHomelandMessageHandler() {}

bool SessionHomelandMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandClearUpgradeCoolingRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTempleUpgradeRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_NOTIFY,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTempleUpgradeNotify,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTempleHarvestRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTempleClearHarvestCoolingRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTempleForceHarvestRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandGoldmineUpgradeRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandGoldmineHarvestRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandPoolUpgradeRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_HARVEST_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandPoolHarvestRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST2,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestRequest2,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandChurchPaidPrayRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandChurchRefreshRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandChurchVipRefreshRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandChurchHarvestRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST2,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandChurchHarvestRequest2,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTreeAssistWaterRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_STEAL_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTreeStealRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_HARVEST_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTreeHarvestRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandTowerUpgradeRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandStrongholdUpgradeRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandBattleMatchReqeust,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST,
      boost::bind(&SessionHomelandMessageHandler::OnMessageHomelandBattleStrikeBackRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_WATER,
      boost::bind(&SessionHomelandMessageHandler::OnMessageTreeWater,
          this, _1, _2));

  return true;
}

void SessionHomelandMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_NOTIFY);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST2);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST2);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_STEAL_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_WATER);
}

static void SendMessageHomelandClearUpgradeCoolingResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandClearUpgradeCoolingResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE);
}

static void SendMessageHomelandTempleUpgradeResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandTempleUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE);
}

static void SendMessageHomelandTempleHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_RESPONSE);
}

static void SendMessageHomelandTempleClearHarvestCoolingResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandTempleClearHarvestCoolingResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE);
}

static void SendMessageHomelandTempleForceHarvestResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandTempleForceHarvestResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE);
}

static void SendMessageHomelandGoldmineUpgradeResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandGoldmineUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE);
}

static void SendMessageHomelandPoolUpgradeResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandPoolUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE);
}

static void SendMessageHomelandPoolHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandPoolHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_HARVEST_RESPONSE);
}

static void SendMessageHomelandPoolDoubleHarvestResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandPoolDoubleHarvestResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE);
}

static void SendMessageHomelandPoolDoubleHarvestResponseFailed2(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandPoolDoubleHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE);
}

static void SendMessageHomelandChurchPaidPrayResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandChurchPaidPrayResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_RESPONSE);
}

static void SendMessageHomelandChurchRefreshResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandChurchRefreshResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE);
}

static void SendMessageHomelandChurchVipRefreshResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandChurchVipRefreshResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE);
}

static void SendMessageHomelandChurchHarvestResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandChurchHarvestResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE);
}

static void SendMessageHomelandChurchHarvestResponseFailed2(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE);
}

static void SendMessageHomelandTreeStealResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTreeStealResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_STEAL_RESPONSE);
}

static void SendMessageHomelandTreeHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTreeHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_HARVEST_RESPONSE);
}

static void SendMessageHomelandTowerUpgradeResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandTowerUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE);
}

static void SendMessageHomelandStrongholdUpgradeResponseFailed(SocialActor *social_actor) {
  session::protocol::MessageHomelandStrongholdUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandClearUpgradeCoolingRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandClearUpgradeCoolingRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandClearUpgradeCoolingRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST from session_server, "
           "actor_id_=[%lu], upgrade_cooling_=[%d].",
           request.actor_id_, request.upgrade_cooling_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查消耗
  // --检查钻石
  core::int32 freedom_dollars =
      (core::int32)std::ceil((double)request.upgrade_cooling_ /
                             MISC_CONF()->homeland_upgrade_cooling_unit_) *
      MISC_CONF()->homeland_clear_upgrade_cooling_unit_freedom_dollars_;

  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageHomelandClearUpgradeCoolingResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (freedom_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - freedom_dollars);
  }

  // 转发 session_server
  session::protocol::MessageHomelandClearUpgradeCoolingResponse session_response;
  session_response.__set_actor_id_(social_actor->GetID());
  session_response.__set_result_(true);
  SessionChannel::SendMessage(session_response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTempleUpgradeRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST from session_server, "
           "actor_id_=[%lu], temple_level_=[%d].",
           request.actor_id_, request.temple_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandTempleCell *cell =
      SOCIAL_CONF()->GetHomelandTemple(request.temple_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTempleCell(%d) not found in config.", request.temple_level_);
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }
    
  // 检查消耗
  // --检查金币
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) <
      cell->upgrade_spend_gold_) {
    LOG_WARNING("Gold is not enough.");
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (cell->upgrade_spend_gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - cell->upgrade_spend_gold_);
  }

  // 转发 sesssion_server
  session::protocol::MessageHomelandTempleUpgradeResponse response;
  response.__set_actor_id_(request.actor_id_);
  response.__set_result_(true);
  response.__set_temple_level_(request.temple_level_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTempleUpgradeNotify(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleUpgradeNotify notify;
  if (global::GlobalPacket::GetPacket()->Deserialize(notify, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleUpgradeNotify failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_UPGRADE_NOTIFY from session_server, "
           "actor_id_=[%lu], temple_level_=[%d.]",
           notify.actor_id_, notify.temple_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(notify.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", notify.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  LoadFunctionalityConfigure::OpenCells &cells = Configure::GetInstance()->GetFunctionalityCell(
      entity::FunctionalityOpenType::HOMELAND_TEMPLE_LEVEL);
  
  for (size_t i = 0; i < cells.size(); ++i) {
    if (notify.temple_level_ >= cells[i].param_ &&
        game_actor->CheckFunctionalityState(cells[i].function_) == false) {
      game_actor->SetFunctionalityState(cells[i].function_, true);
      LOG_DEBUG("开启功能类型[%d]", cells[i].function_);
    }
  }
}

void SessionHomelandMessageHandler::OnMessageHomelandTempleHarvestRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST from session_server, "
           "actor_id_=[%lu], temple_level_=[%d].",
           request.actor_id_, request.temple_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandTempleCell *cell =
      SOCIAL_CONF()->GetHomelandTemple(request.temple_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTempleCell(%d) not found in config.", request.temple_level_);
    SendMessageHomelandTempleHarvestResponseFailed(social_actor);
    return;
  }

  // 增加金币
  if (cell->harvest_gold_reward_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, cell->harvest_gold_reward_);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandTempleHarvestResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTempleClearHarvestCoolingRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleClearHarvestCoolingRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleClearHarvestCoolingRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST from session_server, "
           "actor_id_=[%lu], temple_harvest_cooling_=[%d].",
           request.actor_id_, request.temple_harvest_cooling_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查消耗
  // --检查钻石
  core::int32 freedom_dollars =
      (core::int32)std::ceil((double)request.temple_harvest_cooling_ /
                             MISC_CONF()->homeland_temple_harvest_cooling_unit_) *
      MISC_CONF()->homeland_clear_temple_harvest_cooling_unit_freedom_dollars_;

  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageHomelandTempleClearHarvestCoolingResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (freedom_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - freedom_dollars);
  }

  // 转发 session_server
  session::protocol::MessageHomelandTempleClearHarvestCoolingResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTempleForceHarvestRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleForceHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleForceHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST from session_server,"
           "actor_id_=[%lu], temple_level_[%d], temple_force_harvest_times_=[%d].",
           request.actor_id_, request.temple_level_, request.temple_force_harvest_times_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandTempleCell *cell =
      SOCIAL_CONF()->GetHomelandTemple(request.temple_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTempleCell(%d) not found in config.", request.temple_level_);
    SendMessageHomelandTempleForceHarvestResponseFailed(social_actor);
    return;
  }

  // 检查消耗
  // --检查钻石
  core::int32 freedom_dollars = std::min(
      cell->force_harvest_max_spend_freedom_dollars_,
      cell->force_harvest_min_spend_freedom_dollars_ +
      request.temple_force_harvest_times_ * cell->force_harvest_step_spend_freedom_dollars_);

  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageHomelandTempleForceHarvestResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (freedom_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - freedom_dollars);
  }

  // 增加金币
  if (cell->force_harvest_gold_reward_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, cell->force_harvest_gold_reward_);
  }

  // 转发 session_server
  session::protocol::MessageHomelandTempleForceHarvestResponse session_response;
  session_response.__set_actor_id_(social_actor->GetID());
  session_response.__set_result_(true);
  SessionChannel::SendMessage(session_response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandGoldmineUpgradeRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandGoldmineUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandGoldmineUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST from session_server, "
           "actor_id_=[%lu], goldmine_level_=[%d].",
           request.actor_id_, request.goldmine_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandGoldmineCell *cell =
    SOCIAL_CONF()->GetHomelandGoldmine(request.goldmine_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandGoldmineCell(%d) not found in config.", request.goldmine_level_);
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查消耗
  // --检查金币
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) <
      cell->upgrade_spend_gold_) {
    LOG_WARNING("Gold is not enough.");
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (cell->upgrade_spend_gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - cell->upgrade_spend_gold_);
  }

  // 转发 session_server
  session::protocol::MessageHomelandGoldmineUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_goldmine_level_(request.goldmine_level_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandGoldmineHarvestRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandGoldmineHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandGoldmineHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST from session_server, "
           "actor_id_=[%lu], goldmine_level_=[%d], goldmine_loss_rate=[%d].",
           request.actor_id_, request.goldmine_level_, request.goldmine_loss_rate_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandGoldmineCell *cell =
      SOCIAL_CONF()->GetHomelandGoldmine(request.goldmine_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandGoldmineCell(%d) not found in config.", request.goldmine_level_);
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 增加金币
  core::int32 gold = cell->harvest_gold_reward_ * std::max(0, 100 - request.goldmine_loss_rate_) / 100;
  if (gold > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, gold);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandGoldmineHarvestResponse response;
  response.__set_result_(true);
  response.__set_gold_(gold);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandPoolUpgradeRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandPoolUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST from session_server, "
           "actor_id_=[%lu], pool_level_=[%d].",
           request.actor_id_, request.pool_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(request.pool_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandPoolCell(%d) not found in config.", request.pool_level_);
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查消耗
  // --检查金币
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) <
      cell->upgrade_spend_gold_) {
    LOG_WARNING("Gold is not enough.");
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (cell->upgrade_spend_gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - cell->upgrade_spend_gold_);
  }

  // 转发 session_server
  session::protocol::MessageHomelandPoolUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_pool_level_(request.pool_level_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandPoolHarvestRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandPoolHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_HARVEST_REQUEST from session_server, "
           "actor_id_=[%lu], pool_level_=[%d], accumulate_times_=[%d].",
           request.actor_id_, request.pool_level_, request.accumulate_times_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(request.pool_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandPoolCell(%d) not found in config.", request.pool_level_);
    SendMessageHomelandPoolHarvestResponseFailed(social_actor);
    return;
  }

  // 增加经验
  core::int32 exp = cell->accumulate_exp_reward_ * request.accumulate_times_;
  if (exp > 0) {
    game_actor->AddResource(entity::ResourceID::EXP, exp);
  }
  // 增加灵气
  core::int32 nimbus = cell->accumulate_nimbus_reward_ * request.accumulate_times_;
  if (nimbus > 0) {
    game_actor->AddResource(entity::ResourceID::NIMBUS, nimbus);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandPoolHarvestResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandPoolDoubleHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolDoubleHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST from session_server, "
           "actor_id_=[%lu], pool_level_=[%d], accumulate_times_=[%d].",
           request.actor_id_, request.pool_level_, request.accumulate_times_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(request.pool_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandPoolCell(%d) not found in config.", request.pool_level_);
    SendMessageHomelandPoolDoubleHarvestResponseFailed(social_actor);
    return;
  }

  // 检查道具
  if (FacadeRequest::GetInstance()->CheckPacketItemExist(request.actor_id_,
          cell->double_harvest_spend_item_id_, cell->double_harvest_spend_item_count_,
          entity::BindRequireType::BOTH) == false) {
    LOG_WARNING("Item(%u) is not enough.", cell->double_harvest_spend_item_id_);
    SendMessageHomelandPoolDoubleHarvestResponseFailed(social_actor);
    return;
  }
  
  // 扣除道具
  core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(request.actor_id_,
      cell->double_harvest_spend_item_id_, cell->double_harvest_spend_item_count_,
      entity::BindRequireType::BOTH);
  if (delete_number != (core::uint32)cell->double_harvest_spend_item_count_) {
    LOG_ERROR("Remove item(%u) failed, req_count=[%d], del_count=[%d].",
              cell->double_harvest_spend_item_id_,
              cell->double_harvest_spend_item_count_, delete_number);
    SendMessageHomelandPoolDoubleHarvestResponseFailed(social_actor);
    return;
  }

  // 发送成功回复
  session::protocol::MessageHomelandPoolDoubleHarvestResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_pool_level_(request.pool_level_);
  response.__set_accumulate_times_(request.accumulate_times_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestRequest2(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandPoolDoubleHarvestRequest2 request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolDoubleHarvestRequest2 failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST2 from session_server, "
           "actor_id_=[%lu], pool_level_=[%d], accumulate_times_=[%d].",
           request.actor_id_, request.pool_level_, request.accumulate_times_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(request.pool_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandPoolCell(%d) not found in config.", request.pool_level_);
    SendMessageHomelandPoolDoubleHarvestResponseFailed2(social_actor);
    return;
  }

  // 增加经验
  core::int32 exp = cell->accumulate_exp_reward_ * request.accumulate_times_ * 2;
  if (exp > 0) {
    game_actor->AddResource(entity::ResourceID::EXP, exp);
  }
  // 增加灵气
  core::int32 nimbus = cell->accumulate_nimbus_reward_ * request.accumulate_times_ * 2;
  if (nimbus > 0) {
    game_actor->AddResource(entity::ResourceID::NIMBUS, nimbus);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandPoolDoubleHarvestResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandChurchPaidPrayRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchPaidPrayRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchPaidPrayRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST from session_server, "
           "actor_id_=[%lu], chest_id_=[%d].",
           request.actor_id_, request.chest_id_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查消耗
  // --检查钻石
  core::int32 freedom_dollars = MISC_CONF()->homeland_church_paid_pray_spend_freedom_dollars_;
  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageHomelandChurchPaidPrayResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (freedom_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - freedom_dollars);
  }

  // 转发 session_server
  session::protocol::MessageHomelandChurchPaidPrayResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_chest_id_(request.chest_id_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandChurchRefreshRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchRefreshRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchRefreshRequest failed.");
    return;
  }
  
  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST from session_server, "
           "actor_id_=[%lu], chest_id_=[%d].",
           request.actor_id_, request.chest_id_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查消耗
  core::int32 freedom_dollars = MISC_CONF()->homeland_church_refresh_spend_freedom_dollars_;
  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageHomelandChurchRefreshResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (freedom_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - freedom_dollars);
  }

  // 转发 session_server
  session::protocol::MessageHomelandChurchRefreshResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_chest_id_(request.chest_id_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandChurchVipRefreshRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchVipRefreshRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchVipRefreshRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST from session_server, "
           "actor_id_=[%lu], chest_id_=[%d].",
           request.actor_id_, request.chest_id_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查消耗
  core::int32 freedom_dollars = MISC_CONF()->homeland_church_vip_refresh_spend_freddom_dollars_;
  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageHomelandChurchVipRefreshResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (freedom_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - freedom_dollars);
  }

  // 转发 session_server
  session::protocol::MessageHomelandChurchVipRefreshResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_chest_id_(request.chest_id_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandChurchHarvestRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST from session_server, "
           "actor_id_=[%lu], chest_id_=[%d], bonus_id_=[%d].",
           request.actor_id_, request.chest_id_, request.bonus_id_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandChurchBonusCell *cell =
    SOCIAL_CONF()->GetHomelandChurchBonus(request.chest_id_, request.bonus_id_);
  if (NULL == cell) {
    LOG_ERROR("HomelandChurchBonusCell(%d, %d) not found in config.",
              request.chest_id_, request.bonus_id_);
    SendMessageHomelandChurchHarvestResponseFailed(social_actor);
    return;
  }

  // 检查背包空位
  if (FacadeRequest::GetInstance()->CheckAddPacketItem(request.actor_id_,
          cell->item_id_, cell->item_count_, entity::BindRequireType::BIND) == false) {
    LOG_WARNING("Can not add item.");
    SendMessageHomelandChurchHarvestResponseFailed(social_actor);
    return;
  }

  // 发送成功回复
  session::protocol::MessageHomelandChurchHarvestResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_chest_id_(request.chest_id_);
  response.__set_bonus_id_(request.bonus_id_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandChurchHarvestRequest2(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchHarvestRequest2 request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchHarvestRequest2 failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST2 from session_server, "
           "actor_id_=[%lu], chest_id_=[%d], bonus_id_=[%d].",
           request.actor_id_, request.chest_id_, request.bonus_id_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandChurchBonusCell *cell =
    SOCIAL_CONF()->GetHomelandChurchBonus(request.chest_id_, request.bonus_id_);
  if (NULL == cell) {
    LOG_ERROR("HomelandChurchBonusCell(%d, %d) not found in config.", request.chest_id_, request.bonus_id_);
    SendMessageHomelandChurchHarvestResponseFailed2(social_actor);
    return;
  }

  // 添加道具
  if (FacadeRequest::GetInstance()->AddPacketItem(request.actor_id_,
          cell->item_id_, cell->item_count_, entity::BindRequireType::BOTH) == false) {
    LOG_ERROR("Add item failed.");
    SendMessageHomelandChurchHarvestResponseFailed2(social_actor);
    return;
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandChurchHarvestResponse response;
  response.__set_result_(true);
  response.__set_chest_type_(request.chest_id_);
  response.__set_bonus_id_(request.bonus_id_);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTreeAssistWaterRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTreeAssistWaterRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTreeAssistWaterRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST from session_server, "
           "actor_id_=[%lu], friend_actor_id_=[%s], tree_stage_=[%d], tree_watered_times_=[%d].",
           request.actor_id_, request.friend_actor_id_.c_str(),
           request.tree_stage_, request.tree_watered_times_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 增加礼券
  core::int32 restrict_dollars = MISC_CONF()->homeland_tree_assist_water_restrict_dollars_reward_;
  if (restrict_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, restrict_dollars);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandTreeAssistWaterResponse response;
  response.__set_result_(true);
  response.__set_actor_id_(request.friend_actor_id_);
  response.__set_tree_stage_(request.tree_stage_);
  response.__set_tree_watered_times_(request.tree_watered_times_);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTreeStealRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTreeStealRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTreeStealRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TREE_STEAL_REQUEST from session_server, "
           "actor_id_=[%lu], tree_level_=[%d].",
           request.actor_id_, request.tree_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(request.tree_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", request.tree_level_);
    SendMessageHomelandTreeStealResponseFailed(social_actor);
    return;
  }

  // 增加礼券
  core::int32 restrict_dollars = cell->harvest_restrict_dollars_reward_ *
      cell->stolen_loss_rate_ / 100;
  if (restrict_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, restrict_dollars);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandTreeStealResponse response;
  response.__set_result_(true);
  response.__set_restrict_dollars_(restrict_dollars);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_STEAL_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTreeHarvestRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTreeHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTreeHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TREE_HARVEST_REQUEST from session_server, "
           "actor_id_=[%lu], tree_level_=[%d], stolen_times_=[%d].",
           request.actor_id_, request.tree_level_, request.stolen_times_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(request.tree_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", request.tree_level_);
    SendMessageHomelandTreeHarvestResponseFailed(social_actor);
    return;
  }

  // 增加礼券
  core::int32 restrict_dollars = cell->harvest_restrict_dollars_reward_ *
      std::max(100 - cell->stolen_loss_rate_ * request.stolen_times_, 0) / 100;
  if (restrict_dollars > 0) {
    game_actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, restrict_dollars);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandTreeHarvestResponse response;
  response.__set_result_(true);
  response.__set_restrict_dollars_(restrict_dollars);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_HARVEST_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandTowerUpgradeRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTowerUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTowerUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST from session_server, "
           "actor_id=[%lu], tower_id_=[%d], tower_level_=[%d].",
           request.actor_id_, request.tower_id_, request.tower_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandTowerCell *cell =
    SOCIAL_CONF()->GetHomelandTower(request.tower_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTowerCell(%d) not found in config.", request.tower_level_);
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查消耗
  // --检查金币
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) <
      cell->upgrade_spend_gold_) {
    LOG_WARNING("Gold is not enough.");
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (cell->upgrade_spend_gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, cell->upgrade_spend_gold_);
  }

  // 转发 session_server
  session::protocol::MessageHomelandTowerUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_tower_id_(request.tower_id_);
  response.__set_tower_level_(request.tower_level_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandStrongholdUpgradeRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandStrongholdUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandStrongholdUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST from session_server, "
           "actor_id_=[%lu], stronghold_level_=[%d].",
           request.actor_id_, request.stronghold_level_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  const global::configure::HomelandStrongholdCell *cell =
    SOCIAL_CONF()->GetHomelandStronghold(request.stronghold_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandStrongholdCell(%d) not found in config.", request.stronghold_level_);
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查消耗
  // --检查金币
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) <
      cell->upgrade_spend_gold_) {
    LOG_WARNING("Gold is not enough.");
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 扣除消耗
  if (cell->upgrade_spend_gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - cell->upgrade_spend_gold_);
  }

  // 转发 session_server
  session::protocol::MessageHomelandStrongholdUpgradeResponse response;
  response.__set_actor_id_(social_actor->GetID());
  response.__set_result_(true);
  response.__set_stronghold_level_(request.stronghold_level_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE);
}

void SessionHomelandMessageHandler::OnMessageHomelandBattleMatchReqeust(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandBattleMatchReqeust request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandBattleMatchReqeust failed.");
    return;
  }
  
  LOG_INFO("Receive MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST from session_server, "
           "actor_id_=[%lu], target_actor_name_=[%s], gold_=[%d].",
           request.actor_id_, request.target_actor_name_.c_str(), request.gold_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  if (request.gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, request.gold_);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandBattleMatchResponse response;
  response.__set_result_(true);
  response.__set_actor_name_(request.target_actor_name_);
  response.__set_gold_(request.gold_);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_MATCH_RESPONSE);

  // 发送掠夺事件
  event::EventSocialHomeLandBattleStrike event;
  event.__set_actor_(game_actor->GetActorID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE, game_actor->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void SessionHomelandMessageHandler::OnMessageHomelandBattleStrikeBackRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandBattleStrikeBackRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandBattleStrikeBackRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST from session_server, "
           "actor_id_=[%lu], gold_=[%d].",
           request.actor_id_, request.gold_);

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  if (request.gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, request.gold_);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandBattleStrikeBackResponse response;
  response.__set_result_(true);
  response.__set_gold_(request.gold_);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_RESPONSE);

  // 发送掠夺事件
  event::EventSocialHomeLandBattleStrike event;
  event.__set_actor_(game_actor->GetActorID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE, game_actor->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

}


void SessionHomelandMessageHandler::OnMessageTreeWater(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTreeWater message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandBattleStrikeBackRequest failed.");
    return;
  }

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(message.id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", message.id_);
    return;
  }
  GameActor *game_actor = social_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 发送事件
  event::EventSocialHomeLandTreeWater event;
  event.__set_id_(message.id_);
  event.__set_watered_id_(message.watered_id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_HOMELAND_TREE_WATER, message.id_,
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

}  // namespace social

}  // namespace server

}  // namespace game

