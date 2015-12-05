#include "session_server/server/extension/social/game_homeland_message_handler.h"

#include <boost/bind.hpp>

#include "entity/homeland_types.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_social_protocol_types.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"
#include "session_server/server/extension/social/homeland_actor.h"
#include "session_server/server/extension/social/homeland_event.h"

namespace session {

namespace server {

namespace social {

GameHomelandMessageHandler::GameHomelandMessageHandler() {}
GameHomelandMessageHandler::~GameHomelandMessageHandler() {}

bool GameHomelandMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandClearUpgradeCoolingResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandTempleUpgradeResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandTempleClearHarvestCoolingResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandTempleForceHarvestResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandGoldmineUpgradeResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandPoolUpgradeResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandChurchPaidPrayResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandChurchRefreshResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandChurchVipRefreshResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandChurchHarvestResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandTowerUpgradeResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandStrongholdUpgradeResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_SOUL_SYNCHRONIZE,
      boost::bind(&GameHomelandMessageHandler::OnMessageHomelandSoulSynchronize,
          this, _1, _2, _3));

  return true;
}

void GameHomelandMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_SOUL_SYNCHRONIZE);
}

static void SendMessageHomelandClearUpgradeCoolingResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandClearUpgradeCoolingResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST);
}

static void SendMessageHomelandTempleUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

static void SendMessageHomelandTempleClearHarvestCoolingResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleClearHarvestCoolingResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST);
}

static void SendMessageHomelandTempleForceHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleForceHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST);
}

static void SendMessageHomelandGoldmineUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandGoldmineUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

static void SendMessageHomelandPoolUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandPoolUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

static void SendMessageHomelandPoolDoubleHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandPoolDoubleHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST);
}

static void SendMessageHomelandChurchPrayResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchPrayResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_PRAY_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST);
}

static void SendMessageHomelandChurchRefreshResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchRefreshResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE);
  // 解锁 
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST);
}

static void SendMessageHomelandChurchVipRefreshResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchVipRefreshResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST);
}

static void SendMessageHomelandChurchHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST);
}

static void SendMessageHomelandTowerUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTowerUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

static void SendMessageHomelandStrongholdUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandStrongholdUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

void GameHomelandMessageHandler::OnMessageHomelandClearUpgradeCoolingResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandClearUpgradeCoolingResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandClearUpgradeCoolingResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d].",
           game_response.actor_id_, game_response.result_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandClearUpgradeCoolingResponseFailed(social_actor);
    return;
  }

  // 清除冷却时间
  homeland_actor->ClearUpgradeCooling();

  // 发送成功回复
  gateway::protocol::MessageHomelandClearUpgradeCoolingResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandTempleUpgradeResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleUpgradeResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleUpgradeResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], temple_level_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.temple_level_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandTempleCell *cell =
    SOCIAL_CONF()->GetHomelandTemple(game_response.temple_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTempleCell(%d) not found in config.", game_response.temple_level_);
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  // 设置等级
  homeland_actor->SetTempleLevel(game_response.temple_level_ + 1);

  // 增加冷却时间
  homeland_actor->AddUpgradeCooling(cell->upgrade_cooling_);

  // 发送成功回复
  gateway::protocol::MessageHomelandTempleUpgradeResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);

  // 发送升级通知
  session::protocol::MessageHomelandTempleUpgradeNotify game_notify;
  game_notify.__set_actor_id_(game_response.actor_id_);
  game_notify.__set_temple_level_(homeland_actor->GetTempleLevel());
  terminal->SendGameMessage(game_notify,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_NOTIFY);
}

void GameHomelandMessageHandler::OnMessageHomelandTempleClearHarvestCoolingResponse(
    SessionTerminal *terminal, const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleClearHarvestCoolingResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleClearHarvestCoolingResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d].",
           game_response.actor_id_, game_response.result_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandTempleClearHarvestCoolingResponseFailed(social_actor);
    return;
  }

  // 清除冷却时间
  homeland_actor->ClearTempleHarvestCooling();

  // 发送成功回复
  gateway::protocol::MessageHomelandTempleClearHarvestCoolingResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandTempleForceHarvestResponse(
    SessionTerminal *terminal, const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTempleForceHarvestResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleForceHarvestResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d].",
           game_response.actor_id_, game_response.result_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandTempleForceHarvestResponseFailed(social_actor);
    return;
  }

  // 增加强征次数
  homeland_actor->AddTempleForceHarvestTimes();

  // 发送成功回复
  gateway::protocol::MessageHomelandTempleForceHarvestResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandGoldmineUpgradeResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandGoldmineUpgradeResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandGoldmineUpgradeResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], goldmine_level_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.goldmine_level_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandGoldmineCell *cell =
    SOCIAL_CONF()->GetHomelandGoldmine(game_response.goldmine_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandGoldmineCell(%d) not found in config.", game_response.goldmine_level_);
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 设置等级
  homeland_actor->SetGoldmineLevel(game_response.goldmine_level_ + 1);

  // 增加冷却时间
  homeland_actor->AddUpgradeCooling(cell->upgrade_cooling_);

  // 发送成功回复
  gateway::protocol::MessageHomelandGoldmineUpgradeResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

void GameHomelandMessageHandler::OnMessageHomelandPoolUpgradeResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandPoolUpgradeResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolUpgradeResponse failed");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], pool_level_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.pool_level_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(game_response.pool_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandPoolCell(%d) not found in config.", game_response.pool_level_);
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 设置等级
  homeland_actor->SetPoolLevel(game_response.pool_level_ + 1);

  // 增加冷却时间
  homeland_actor->AddUpgradeCooling(cell->upgrade_cooling_);

  // 发送成功回复
  gateway::protocol::MessageHomelandPoolUpgradeResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

void GameHomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandPoolDoubleHarvestResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolDoubleHarvestResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], pool_level_=[%d], accumulate_times_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.pool_level_,
           game_response.accumulate_times_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandPoolDoubleHarvestResponseFailed(social_actor);
    return;
  }

  // 重置累计时间
  homeland_actor->ClearPoolAccmulateTime();

  // 转发game_server
  session::protocol::MessageHomelandPoolDoubleHarvestRequest2 game_request;
  game_request.__set_actor_id_(game_response.actor_id_);
  game_request.__set_pool_level_(game_response.pool_level_);
  game_request.__set_accumulate_times_(game_response.accumulate_times_);
  terminal->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST2);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandChurchPaidPrayResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchPaidPrayResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchPaidPrayResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_PAID_PRAY_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], chest_id_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.chest_id_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandChurchPrayResponseFailed(social_actor);
    return;
  }

  // 增加钻石祈福次数
  homeland_actor->AddChurchPaidPrayTimes();
  // 转换到领取状态
  homeland_actor->MoveChurchToHarvestStage(game_response.chest_id_);

  // 发送成功回复
  gateway::protocol::MessageHomelandChurchPrayResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_PRAY_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandChurchRefreshResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchRefreshResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchRefreshResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], chest_id_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.chest_id_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandChurchRefreshResponseFailed(social_actor);
    return;
  }

  // 转换到领取状态
  homeland_actor->MoveChurchToHarvestStage(game_response.chest_id_);
  // 发送成功回复
  gateway::protocol::MessageHomelandChurchRefreshResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE);
  // 解锁 
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandChurchVipRefreshResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchVipRefreshResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchVipRefreshResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], chest_id_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.chest_id_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandChurchVipRefreshResponseFailed(social_actor);
    return;
  }

  // 转换到领取状态
  homeland_actor->MoveChurchToHarvestStage(game_response.chest_id_);
  // 发送成功回复
  gateway::protocol::MessageHomelandChurchVipRefreshResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandChurchHarvestResponse(SessionTerminal *terminal, 
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandChurchHarvestResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchHarvestResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], chest_id_=[%d], bonus_id_=[%d].",
           game_response.actor_id_, game_response.result_,
           game_response.chest_id_, game_response.bonus_id_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandChurchHarvestResponseFailed(social_actor);
    return;
  }

  // 转换到祈福状态
  homeland_actor->MoveChurchToPrayStage();

  // 祈福堂祈福事件
  const global::configure::HomelandChurchBonusCell *cell =
    SOCIAL_CONF()->GetHomelandChurchBonus(game_response.chest_id_, game_response.bonus_id_);
  if (NULL == cell) {
    LOG_ERROR("HomelandChurchBonusCell(%d, %d) not found in config.",
              game_response.chest_id_, game_response.bonus_id_);
  } else {
    HomelandEvent event(entity::HomelandEventType::CHURCH_PRAY);
    event.PushEventVar(game_response.chest_id_);
    event.PushEventVar(cell->item_id_);
    event.PushEventVar(cell->item_count_);
    homeland_actor->AddEvent(event);
  }

  // 转发 game_server
  session::protocol::MessageHomelandChurchHarvestRequest2 game_request;
  game_request.__set_actor_id_(game_response.actor_id_);
  game_request.__set_chest_id_(game_response.chest_id_);
  game_request.__set_bonus_id_(game_response.bonus_id_);
  terminal->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST2);
  // 解锁
  social_actor->GetLock()->Unlock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST);
}

void GameHomelandMessageHandler::OnMessageHomelandTowerUpgradeResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandTowerUpgradeResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTowerUpgradeResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], tower_id_=[%d], tower_level_=[%d].",
           game_response.actor_id_, game_response.result_,
           game_response.tower_id_, game_response.tower_level_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
  }

  const global::configure::HomelandTowerCell *cell =
    SOCIAL_CONF()->GetHomelandTower(game_response.tower_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTowerCell(%d) not found in config.", game_response.tower_level_);
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 设置等级
  homeland_actor->SetTowerLevel(game_response.tower_id_, game_response.tower_level_ + 1);

  // 增加冷却时间
  homeland_actor->AddUpgradeCooling(cell->upgrade_cooling_);

  // 发送成功回复
  gateway::protocol::MessageHomelandTowerUpgradeResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

void GameHomelandMessageHandler::OnMessageHomelandStrongholdUpgradeResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandStrongholdUpgradeResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandStrongholdUpgradeResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], stronghold_level_=[%d].",
           game_response.actor_id_, game_response.result_, game_response.stronghold_level_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  if (false == game_response.result_) {
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandStrongholdCell *cell =
    SOCIAL_CONF()->GetHomelandStronghold(game_response.stronghold_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandStrongholdCell(%d) not found in config.", game_response.stronghold_level_);
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 设置等级
  homeland_actor->SetStrongholdLevel(game_response.stronghold_level_ + 1);

  // 增加冷却时间
  homeland_actor->AddUpgradeCooling(cell->upgrade_cooling_);

  // 发送成功回复
  gateway::protocol::MessageHomelandStrongholdUpgradeResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE);
  // 解锁
  social_actor->GetLock()->Unlock(SocialActor::kHomelandUpgradeLock);
}

void GameHomelandMessageHandler::OnMessageHomelandSoulSynchronize(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageHomelandSoulSynchronize game_sync;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_sync, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandSoulSynchronize failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_SOUL_SYNCHRONIZE from game_server, actor_id_=[%lu].",
            game_sync.actor_id_);

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->GetFromAll(game_sync.actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", game_sync.actor_id_);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 更新英灵数据
  for (size_t i = 0; i < game_sync.souls_.size(); ++i) {
    homeland_actor->UpdateSoulData(game_sync.souls_[i].soul_id_,
        game_sync.souls_[i].soul_level_);
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

