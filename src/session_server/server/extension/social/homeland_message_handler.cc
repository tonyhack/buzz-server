#include "session_server/server/extension/social/homeland_message_handler.h"

#include <cstdlib>
#include <boost/bind.hpp>

#include "entity/homeland_types.h"
#include "global/types.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_social_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"
#include "session_server/server/extension/social/facade_request.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"
#include "session_server/server/extension/social/homeland_actor.h"
#include "session_server/server/extension/social/homeland_event.h"

namespace session {

namespace server {

namespace social {

HomelandMessageHandler::HomelandMessageHandler() {}
HomelandMessageHandler::~HomelandMessageHandler() {}

bool HomelandMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_VISIT_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandVisitRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandClearUpgradeCoolingRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTempleUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTempleHarvestRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTempleClearHarvestCoolingRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTempleForceHarvestRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandGoldmineUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandGoldmineHarvestRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandPoolUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_HARVEST_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandPoolHarvestRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_PRAY_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandChurchPrayRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandChurchRefreshRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandChurchVipRefreshRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandChurchHarvestRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_WATER_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTreeWaterRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTreeAssistWaterRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_STEAL_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTreeStealRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_HARVEST_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTreeHarvestRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandTowerUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandStrongholdUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandStrongholdCallSupportRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_REQUEST,
      boost::bind(&HomelandMessageHandler::OnMessageHomelandStrongholdSupportRequest,
          this, _1, _2, _3));

  return true;
}

void HomelandMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_VISIT_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_PRAY_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_WATER_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_STEAL_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_HARVEST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_REQUEST);
}

static void SendMessageMessageHomelandVisitResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandVisitResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_VISIT_RESPONSE);
}

static void SendMessageHomelandClearUpgradeCoolingResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandClearUpgradeCoolingResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE);
}

static void SendMessageHomelandTempleUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE);
}

static void SendMessageHomelandTempleHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_RESPONSE);
}

static void SendMessageHomelandTempleClearHarvestCoolingResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleClearHarvestCoolingResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE);
}

static void SendMessageHomelandTempleForceHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTempleForceHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE);
}

static void SendMessageHomelandGoldmineUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandGoldmineUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE);
}

static void SendMessageHomelandGoldmineHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandGoldmineHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_RESPONSE);
}

static void SendMessageHomelandPoolUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandPoolUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE);
}

static void SendMessageHomelandPoolHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandPoolHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_HARVEST_RESPONSE);
}

static void SendMessageHomelandPoolDoubleHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandPoolDoubleHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE);
}

static void SendMessageHomelandChurchPrayResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchPrayResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_PRAY_RESPONSE);
}

static void SendMessageHomelandChurchRefreshResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchRefreshResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE);
}

static void SendMessageHomelandChurchVipRefreshResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchVipRefreshResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE);
}

static void SendMessageHomelandChurchHarvestResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandChurchHarvestResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE);
}

static void SendMessageHomelandTreeWaterResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTreeWaterResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_WATER_RESPONSE);
}

static void SendMessageHomelandTreeAssistWaterResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandTreeAssistWaterResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_RESPONSE);
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
  gateway::protocol::MessageHomelandTowerUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE);
}

static void SendMessageHomelandStrongholdUpgradeResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandStrongholdUpgradeResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE);
}

static void SendMessageHomelandStrongholdCallSupportResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandStrongholdCallSupportResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_RESPONSE);
}

static void SendMessageHomelandStrongholdSupportResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandStrongholdSupportResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_RESPONSE);
}

static void HomelandVisitRequest(core::uint64 actor_id, core::uint64 target_actor_id) {
  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", actor_id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", actor_id);
    return;
  }

  // 获取目标社交玩家对象
  SocialActor *target_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(target_actor_id);
  if (NULL == target_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", target_actor_id);
    SendMessageMessageHomelandVisitResponseFailed(social_actor);
    return;
  }
  HomelandActor *target_homeland_actor = target_social_actor->GetHomelandActor();

  // 家园拜访事件
  HomelandEvent event(entity::HomelandEventType::HOMELAND_VISIT);
  event.PushEventVar(social_actor->GetActorID());
  target_homeland_actor->AddEvent(event);

  // 保存离线数据
  if (target_social_actor->CheckOnline() == false) {
    target_social_actor->Save();
  }

  gateway::protocol::MessageHomelandVisitResponse response;
  response.__set_result_(true);
  response.__set_actor_id_(target_social_actor->GetStringID());
  response.__set_actor_name_(target_social_actor->GetName());
  response.__set_temple_level_(target_homeland_actor->GetTempleLevel());
  response.__set_goldmine_level_(target_homeland_actor->GetGoldmineLevel());
  response.__set_pool_level_(target_homeland_actor->GetPoolLevel());
  response.__set_church_enable_(target_social_actor->CheckFunctionalityState(
      entity::FunctionalityType::HOMELAND_CHURCH));
  response.__set_tree_level_(target_homeland_actor->GetTreeLevel());
  response.__set_tree_stage_(
      (entity::HomelandTreeStageType::type)target_homeland_actor->GetTreeStage());
  response.__set_tree_watered_times_(target_homeland_actor->GetTreeWateredTimes());
  response.__set_tree_can_water_(target_homeland_actor->CheckTreeCanWater(actor_id));
  response.__set_tree_can_steal_(target_homeland_actor->CheckTreeCanSteal(actor_id));
  response.__set_tower1_level_(target_homeland_actor->GetTowerLevel(1));
  response.__set_tower2_level_(target_homeland_actor->GetTowerLevel(2));
  response.__set_stronghold_level_(target_homeland_actor->GetStrongholdLevel());
  response.__set_stronghold_can_support_(
      target_homeland_actor->CheckStrongholdCanSupport(actor_id));
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_VISIT_RESPONSE);
}

void HomelandMessageHandler::OnMessageHomelandVisitRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageHomelandVisitRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandVisitRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_VISIT_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false) {
    LOG_WARNING("HOMELAND_MODULE is disable.");
    SendMessageMessageHomelandVisitResponseFailed(social_actor);
    return;
  }

  core::uint64 target_actor_id = atol(request.actor_id_.c_str());

  if (SocialActorManager::GetInstance()->GetFromAll(target_actor_id) == NULL) {
    // 离线加载
    SocialActorManager::GetInstance()->OfflineLoad(target_actor_id,
        boost::bind(&HomelandVisitRequest, id, target_actor_id));
  } else {
    HomelandVisitRequest(id, target_actor_id);
  }
}

void HomelandMessageHandler::OnMessageHomelandClearUpgradeCoolingRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandClearUpgradeCoolingRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandClearUpgradeCoolingRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false) {
    LOG_WARNING("HOMELAND_MODULE is disable.");
    SendMessageHomelandClearUpgradeCoolingResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandClearUpgradeCoolingResponseFailed(social_actor);
    return;
  }

  core::int32 upgrade_cooling = homeland_actor->GetUpgradeCooling();
  if (0 == upgrade_cooling) {
    LOG_WARNING("Upgrade cooling is zero.");
    SendMessageHomelandClearUpgradeCoolingResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST);

  // 转发 game_server
  session::protocol::MessageHomelandClearUpgradeCoolingRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_upgrade_cooling_(upgrade_cooling);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandTempleUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandTempleUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false) {
    LOG_WARNING("HOMELAND_MODULE is disable.");
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(SocialActor::kHomelandUpgradeLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查建造冷却时间
  if (homeland_actor->CheckUpgradeCooling() == false) {
    LOG_WARNING("In upgrade cooling.");
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  // 神殿等级不能超过人物等级
  if (homeland_actor->GetTempleLevel() >= social_actor->GetLevel()) {
    LOG_WARNING("Temple level can not greater than actor level.");
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查是否顶级
  const global::configure::HomelandTempleCell *cell =
    SOCIAL_CONF()->GetHomelandTemple(homeland_actor->GetTempleLevel() + 1);
  if (NULL == cell) {
    LOG_WARNING("Temple is already in top level.");
    SendMessageHomelandTempleUpgradeResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(SocialActor::kHomelandUpgradeLock);

  // 转发 game_server
  session::protocol::MessageHomelandTempleUpgradeRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_temple_level_(homeland_actor->GetTempleLevel());
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandTempleHarvestRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandTempleHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false) {
    LOG_WARNING("HOMELAND_MODULE is disable.");
    SendMessageHomelandTempleHarvestResponseFailed(social_actor);
    return;
  }

  // 检查征收冷却时间
  if (homeland_actor->GetTempleHarvestCooling() != 0) {
    LOG_WARNING("In temple harvest cooling.");
    SendMessageHomelandTempleHarvestResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandTempleCell *cell =
    SOCIAL_CONF()->GetHomelandTemple(homeland_actor->GetTempleLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandTempleCell(%d) not found in config.", homeland_actor->GetTempleLevel());
    SendMessageHomelandTempleHarvestResponseFailed(social_actor);
    return;
  }

  // 检查次数
  if (homeland_actor->GetTempleHarvestTimes() >= cell->max_harvest_times_) {
    LOG_WARNING("Harvest times not enough.");
    SendMessageHomelandTempleHarvestResponseFailed(social_actor);
    return;
  }

  // 增加次数
  homeland_actor->AddTempleHarvestTimes();

  // 设置冷却时间
  homeland_actor->SetTempleHarvestCooling(cell->harvest_cooling_);

  // 转发 game_server
  session::protocol::MessageHomelandTempleHarvestRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_temple_level_(homeland_actor->GetTempleLevel());
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandTempleClearHarvestCoolingRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandTempleClearHarvestCoolingRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleClearHarvestCoolingRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false) {
    LOG_WARNING("HOMELAND_MODULE is disable.");
    SendMessageHomelandTempleClearHarvestCoolingResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandTempleClearHarvestCoolingResponseFailed(social_actor);
    return;
  }

  core::int32 temple_harvest_cooling = homeland_actor->GetTempleHarvestCooling();
  if (0 == temple_harvest_cooling) {
    LOG_WARNING("Temple harvest cooling is zero.");
    SendMessageHomelandTempleClearHarvestCoolingResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST);

  // 转发 game_server
  session::protocol::MessageHomelandTempleClearHarvestCoolingRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_temple_harvest_cooling_(temple_harvest_cooling);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandTempleForceHarvestRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandTempleForceHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTempleForceHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false) {
    LOG_WARNING("HOMELAND_MODULE is disable.");
    SendMessageHomelandTempleForceHarvestResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandTempleForceHarvestResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandTempleCell *cell =
    SOCIAL_CONF()->GetHomelandTemple(homeland_actor->GetTempleLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandTempleCell(%d) not found in config.", homeland_actor->GetTempleLevel());
    SendMessageHomelandTempleForceHarvestResponseFailed(social_actor);
    return;
  }

  // 检查次数 TODO VIP次数
  if (homeland_actor->GetTempleForceHarvestTimes() >= cell->max_force_harvest_times_) {
    LOG_WARNING("Force Harvest times not enough.");
    SendMessageHomelandTempleForceHarvestResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST);

  // 转发 game_server
  session::protocol::MessageHomelandTempleForceHarvestRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_temple_level_(homeland_actor->GetTempleLevel());
  game_request.__set_temple_force_harvest_times_(homeland_actor->GetTempleForceHarvestTimes());
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandGoldmineUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandGoldmineUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandGoldmineUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_GOLDMINE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_GOLDMINE is disable.");
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(SocialActor::kHomelandUpgradeLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查建造冷却时间
  if (homeland_actor->CheckUpgradeCooling() == false) {
    LOG_WARNING("In upgrade cooling.");
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 金矿等级不能超过神殿等级
  if (homeland_actor->GetGoldmineLevel() >= homeland_actor->GetTempleLevel()) {
    LOG_WARNING("Goldmine level can not greater than temple level.");
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查是否顶级
  const global::configure::HomelandGoldmineCell *cell =
    SOCIAL_CONF()->GetHomelandGoldmine(homeland_actor->GetGoldmineLevel() + 1);
  if (NULL == cell) {
    LOG_WARNING("Goldmine is already in top level.");
    SendMessageHomelandGoldmineUpgradeResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(SocialActor::kHomelandUpgradeLock);

  // 转发 game_server
  session::protocol::MessageHomelandGoldmineUpgradeRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_goldmine_level_(homeland_actor->GetGoldmineLevel());
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandGoldmineHarvestRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandGoldmineHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandGoldmineHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_GOLDMINE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_GOLDMINE is disable.");
    SendMessageHomelandGoldmineHarvestResponseFailed(social_actor);
    return;
  }

  // 检查收获冷却时间
  if (homeland_actor->GetGoldmineHarvestCooling() != 0) {
    LOG_WARNING("In goldmine harvest cooling.");
    SendMessageHomelandGoldmineHarvestResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandGoldmineCell *cell =
    SOCIAL_CONF()->GetHomelandGoldmine(homeland_actor->GetGoldmineLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandGoldmineCell(%d) not found in config.", homeland_actor->GetGoldmineLevel());
    SendMessageHomelandGoldmineHarvestResponseFailed(social_actor);
    return;
  }

  core::int32 loss_rate = homeland_actor->GetGoldmineLossRate();

  // 重置金矿
  homeland_actor->ResetGoldmine();

  // 设置冷却时间
  homeland_actor->SetGoldmineHarvestCooling(cell->harvest_cooling_);

  // 转发 game_server
  session::protocol::MessageHomelandGoldmineHarvestRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_goldmine_level_(homeland_actor->GetGoldmineLevel());
  game_request.__set_goldmine_loss_rate_(loss_rate);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandPoolUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandPoolUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_POOL) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_POOL is disable.");
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(SocialActor::kHomelandUpgradeLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查建造冷却时间
  if (homeland_actor->CheckUpgradeCooling() == false) {
    LOG_WARNING("In upgrade cooling.");
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 神秘之源等级不能超过神殿等级
  if (homeland_actor->GetPoolLevel() >= homeland_actor->GetTempleLevel()) {
    LOG_WARNING("Pool level can not greater than temple level.");
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查是否顶级
  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(homeland_actor->GetPoolLevel() + 1);
  if (NULL == cell) {
    LOG_WARNING("Pool is already in top level.");
    SendMessageHomelandPoolUpgradeResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(SocialActor::kHomelandUpgradeLock);

  // 转发 game_server
  session::protocol::MessageHomelandPoolUpgradeRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_pool_level_(homeland_actor->GetPoolLevel());
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandPoolHarvestRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandPoolHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_HARVEST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_POOL) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_POOL is disable.");
    SendMessageHomelandPoolHarvestResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(homeland_actor->GetPoolLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandPoolCell(%d) not found in config.", homeland_actor->GetPoolLevel());
    SendMessageHomelandPoolHarvestResponseFailed(social_actor);
    return;
  }

  core::int32 accumulate_times =
    homeland_actor->GetPoolAccumulateTime() / cell->accumulate_spend_time_;

  if (accumulate_times <= 0) {
    LOG_WARNING("Pool accumulate time not enough.");
    SendMessageHomelandPoolHarvestResponseFailed(social_actor);
    return;
  }

  // 重置累计时间
  homeland_actor->ClearPoolAccmulateTime();

  // 转发 game_server
  session::protocol::MessageHomelandPoolHarvestRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_pool_level_(homeland_actor->GetPoolLevel());
  game_request.__set_accumulate_times_(accumulate_times);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_HARVEST_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandPoolDoubleHarvestRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandPoolDoubleHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandPoolDoubleHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_POOL) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_POOL is disable.");
    SendMessageHomelandPoolDoubleHarvestResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandPoolHarvestResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandPoolCell *cell =
    SOCIAL_CONF()->GetHomelandPool(homeland_actor->GetPoolLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandPoolCell(%d) not found in config.", homeland_actor->GetPoolLevel());
    SendMessageHomelandPoolHarvestResponseFailed(social_actor);
    return;
  }

  core::int32 accumulate_times =
    homeland_actor->GetPoolAccumulateTime() / cell->accumulate_spend_time_;

  // 加锁
  social_actor->GetLock()->Lock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST);

  // 转发 game_server
  session::protocol::MessageHomelandPoolDoubleHarvestRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_pool_level_(homeland_actor->GetPoolLevel());
  game_request.__set_accumulate_times_(accumulate_times);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandChurchPrayRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandChurchPrayRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchPrayRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_PRAY_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_CHURCH) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_CHURCH is disable.");
    SendMessageHomelandChurchPrayResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandChurchPrayResponseFailed(social_actor);
    return;
  }

  // 检查是否在祈福状态
  if (homeland_actor->CheckChurchInPrayStage() == false) {
    LOG_WARNING("Not in pray stage.");
    SendMessageHomelandChurchPrayResponseFailed(social_actor);
    return;
  }

  core::int32 max_free_pray_times = MISC_CONF()->homeland_max_church_free_pray_times_;
  core::int32 max_paid_pray_times = MISC_CONF()->homeland_max_church_paid_pray_times_;

  // 随机宝箱
  core::uint32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  const global::configure::HomelandChurchChestCell *cell =
    SOCIAL_CONF()->GetRandomHomelandChurchChestByPrayOdds(random);
  if (NULL == cell) {
    LOG_ERROR("Get random HomelandChurchChestCell by pray odds failed.");
    SendMessageHomelandChurchPrayResponseFailed(social_actor);
    return;
  }

  if (homeland_actor->GetChurchFreePrayTimes() < max_free_pray_times) {
    // 免费祈福
    // --增加免费祈福次数
    homeland_actor->AddChurchFreePrayTimes();
    // --转换到领取状态
    homeland_actor->MoveChurchToHarvestStage(cell->id_);
    // --发送成功回复
    gateway::protocol::MessageHomelandChurchPrayResponse response;
    response.__set_result_(true);
    social_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_PRAY_RESPONSE);
    return;

  } else if (homeland_actor->GetChurchPaidPrayTimes() < max_paid_pray_times) {
    // 钻石祈福
    // --加锁
    social_actor->GetLock()->Lock(
        session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST);
    // --转发 game_server
    session::protocol::MessageHomelandChurchPaidPrayRequest game_request;
    game_request.__set_actor_id_(id);
    game_request.__set_chest_id_(cell->id_);
    social_actor->SendGameMessage(game_request,
        session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST);
    return;

  } else {
    // 次数不足
    LOG_WARNING("pray times not enough.");
    SendMessageHomelandChurchPrayResponseFailed(social_actor);
    return;
  }
}

void HomelandMessageHandler::OnMessageHomelandChurchRefreshRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandChurchRefreshRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchRefreshRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_CHURCH) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_CHURCH is disable.");
    SendMessageHomelandChurchRefreshResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandChurchRefreshResponseFailed(social_actor);
    return;
  }

  // 检查是否在领取状态
  if (homeland_actor->CheckChurchInHarvestStage() == false) {
    LOG_WARNING("Not in harvest stage.");
    SendMessageHomelandChurchRefreshResponseFailed(social_actor);
    return;
  }

  core::int32 max_free_refresh_times = MISC_CONF()->homeland_max_church_free_refresh_times_;

  // 随机宝箱
  core::uint32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  const global::configure::HomelandChurchChestCell *cell =
    SOCIAL_CONF()->GetRandomHomelandChurchChestByRefreshOdds(random);
  if (NULL == cell) {
    LOG_ERROR("Get random HomelandChurchChestCell by refresh odds failed.");
    SendMessageHomelandChurchRefreshResponseFailed(social_actor);
    return;
  }

  if (homeland_actor->GetChurchFreeRefreshTimes() < max_free_refresh_times) {
    // 免费刷新
    // --增加免费刷新次数
    homeland_actor->AddChurchFreeRefreshTimes();
    // --转换到领取状态
    homeland_actor->MoveChurchToHarvestStage(cell->id_);
    // --发送成功回复
    gateway::protocol::MessageHomelandChurchRefreshResponse response;
    response.__set_result_(true);
    social_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST);
  // 转发 game_server
  session::protocol::MessageHomelandChurchRefreshRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_chest_id_(cell->id_);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandChurchVipRefreshRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandChurchVipRefreshRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchVipRefreshRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_CHURCH) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_CHURCH is disable.");
    SendMessageHomelandChurchVipRefreshResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandChurchVipRefreshResponseFailed(social_actor);
    return;
  }

  // 检查是否在领取状态
  if (homeland_actor->CheckChurchInHarvestStage() == false) {
    LOG_WARNING("Not in harvest stage.");
    SendMessageHomelandChurchVipRefreshResponseFailed(social_actor);
    return;
  }

  // 直接取最后一个宝箱
  const global::configure::HomelandChurchChestCell *cell =
    SOCIAL_CONF()->GetLastHomelandChurchChest();
  if (NULL == cell) {
    LOG_ERROR("Get last HomelandChurchChest failed.");
    SendMessageHomelandChurchVipRefreshResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST);
  // 转发 game_server
  session::protocol::MessageHomelandChurchVipRefreshRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_chest_id_(cell->id_);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandChurchHarvestRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandChurchHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandChurchHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_CHURCH) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_CHURCH is disable.");
    SendMessageHomelandChurchHarvestResponseFailed(social_actor);
    return;
  }
  
  // 锁检查
  if (social_actor->GetLock()->CheckLocked(
          session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandChurchVipRefreshResponseFailed(social_actor);
    return;
  }

  // 检查是否在领取状态
  if (homeland_actor->CheckChurchInHarvestStage() == false) {
    LOG_WARNING("Not in harvest stage.");
    SendMessageHomelandChurchHarvestResponseFailed(social_actor);
    return;
  }

  // 随机奖励
  core::uint32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  const global::configure::HomelandChurchBonusCell *cell =
    SOCIAL_CONF()->GetRandomHomelandChurchBonus(homeland_actor->GetChurchChestType(), random);
  if (NULL == cell) {
    LOG_ERROR("Get random HomelandChurchBonusCell failed.");
    SendMessageHomelandChurchHarvestResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST);
  // 转发 game_server
  session::protocol::MessageHomelandChurchHarvestRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_chest_id_(homeland_actor->GetChurchChestType());
  game_request.__set_bonus_id_(cell->bonus_id_);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandTreeWaterRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandTreeWaterRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTreeWaterRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TREE_WATER_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_TREE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_TREE is disable.");
    SendMessageHomelandTreeWaterResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(homeland_actor->GetTreeLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", homeland_actor->GetTreeLevel());
    SendMessageHomelandTreeWaterResponseFailed(social_actor);
    return;
  }

  // 检查是否可以浇水
  if (homeland_actor->CheckTreeCanWater(id) == false) {
    LOG_WARNING("Can not water tree (probably not in watering stage).");
    SendMessageHomelandTreeWaterResponseFailed(social_actor);
    return;
  }

  // 检查是否在冷却中
  if (homeland_actor->GetTreeWaterCooling() != 0) {
    LOG_WARNING("In tree water cooling.");
    SendMessageHomelandTreeWaterResponseFailed(social_actor);
    return;
  }

  // 增加浇水次数
  homeland_actor->AddTreeWaterdTimes(id);
  // 设置浇水冷却时间
  homeland_actor->SetTreeWaterCooling(cell->water_cooling_);

  // 如果次数已满转换到成长阶段
  if (homeland_actor->GetTreeWateredTimes() >= cell->growing_required_water_times_) {
    homeland_actor->MoveTreeToGrowingStage(cell->growing_cooling_);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandTreeWaterResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_WATER_RESPONSE);

  //通知game
  session::protocol::MessageHomelandTreeWater water_msg;
  water_msg.__set_id_(id);
  water_msg.__set_watered_id_(id);
  social_actor->SendGameMessage(water_msg,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_WATER);
}

static void HomelandTreeAssistWaterRequest(core::uint64 actor_id, core::uint64 friend_actor_id) {
  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", actor_id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", actor_id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 获取好友社交玩家对象
  SocialActor *friend_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(friend_actor_id);
  if (NULL == friend_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", friend_actor_id);
    SendMessageHomelandTreeAssistWaterResponseFailed(social_actor);
    return;
  }
  HomelandActor *friend_homeland_actor = friend_social_actor->GetHomelandActor();

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(friend_homeland_actor->GetTreeLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", friend_homeland_actor->GetTreeLevel());
    SendMessageHomelandTreeAssistWaterResponseFailed(social_actor);
    return;
  }

  // 检查是否可以浇水
  if (friend_homeland_actor->CheckTreeCanWater(actor_id) == false) {
    LOG_WARNING("Actor(%lu) can not water tree for Actor(%lu).", actor_id, friend_actor_id);
    SendMessageHomelandTreeAssistWaterResponseFailed(social_actor);
    return;
  }

  // 增加浇水次数
  friend_homeland_actor->AddTreeWaterdTimes(actor_id);

  // 如果次数已满转换到成长阶段
  if (friend_homeland_actor->GetTreeWateredTimes() >= cell->growing_required_water_times_) {
    friend_homeland_actor->MoveTreeToGrowingStage(cell->growing_cooling_);
  }

  // 增加帮助浇水次数
  homeland_actor->AddTreeAssistWaterTimes();

  // 帮助好友神树浇水事件
  {
    HomelandEvent event(entity::HomelandEventType::TREE_ASSIST_WATER);
    event.PushEventVar(friend_social_actor->GetActorID());
    event.PushEventVar(homeland_actor->GetTreeAssistWaterTimes());
    if (homeland_actor->GetTreeAssistWaterTimes() <=
        MISC_CONF()->homeland_max_tree_assist_water_times_) {
      event.PushEventVar(MISC_CONF()->homeland_tree_assist_water_restrict_dollars_reward_);
    } else {
      event.PushEventVar(0);
    }
    homeland_actor->AddEvent(event);
  }

  // 神树被好友帮助浇水事件
  {
    HomelandEvent event(entity::HomelandEventType::TREE_ASSIST_WATERED);
    event.PushEventVar(social_actor->GetActorID());
    friend_homeland_actor->AddEvent(event);
  }

  // 保存离线数据
  if (friend_social_actor->CheckOnline() == false) {
    friend_social_actor->Save(); 
  }

  // 发送家园好友数据同步
  gateway::protocol::MessageHomelandFriendSynchronize sync;
  sync.__set_type_(gateway::protocol::HomelandFriendOperateType::UPDATE);
  sync.data_.__set_actor_id_(friend_social_actor->GetStringID());
  sync.data_.__set_tree_can_water_(false);
  social_actor->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE);

  if (homeland_actor->GetTreeAssistWaterTimes() <=
      MISC_CONF()->homeland_max_tree_assist_water_times_) {
    // 转发 game_server
    session::protocol::MessageHomelandTreeAssistWaterRequest game_request;
    game_request.__set_actor_id_(actor_id);
    game_request.__set_friend_actor_id_(friend_social_actor->GetStringID());
    game_request.__set_tree_stage_(
        (entity::HomelandTreeStageType::type)friend_homeland_actor->GetTreeStage());
    game_request.__set_tree_watered_times_(friend_homeland_actor->GetTreeWateredTimes());
    social_actor->SendGameMessage(game_request,
        session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST);
  } else {
    // 发送成功回复
    gateway::protocol::MessageHomelandTreeAssistWaterResponse response;
    response.__set_result_(true);
    response.__set_actor_id_(friend_social_actor->GetStringID());
    response.__set_tree_stage_(
        (entity::HomelandTreeStageType::type)friend_homeland_actor->GetTreeStage());
    response.__set_tree_watered_times_(friend_homeland_actor->GetTreeWateredTimes());
    social_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_RESPONSE);
  }

  //通知game
  session::protocol::MessageHomelandTreeWater water_msg;
  water_msg.__set_id_(actor_id);
  water_msg.__set_watered_id_(friend_actor_id);
  social_actor->SendGameMessage(water_msg,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_WATER);
}

void HomelandMessageHandler::OnMessageHomelandTreeAssistWaterRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageHomelandTreeAssistWaterRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTreeAssistWaterRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s]",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_TREE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_TREE is disable.");
    SendMessageHomelandTreeAssistWaterResponseFailed(social_actor);
    return;
  }

  core::uint64 friend_actor_id = atol(request.actor_id_.c_str());

  // 检查是否是好友
  if (social_actor->IsFriend(friend_actor_id) == false) {
    LOG_WARNING("Not a friend.");
    SendMessageHomelandTreeAssistWaterResponseFailed(social_actor);
    return;
  }

  if (SocialActorManager::GetInstance()->GetFromAll(friend_actor_id) == NULL) {
    // 离线加载
    SocialActorManager::GetInstance()->OfflineLoad(friend_actor_id,
        boost::bind(&HomelandTreeAssistWaterRequest, id, friend_actor_id));
  } else {
    HomelandTreeAssistWaterRequest(id, friend_actor_id);
  }

}

static void HomelandTreeStealRequest(core::uint64 actor_id, core::uint64 friend_actor_id) {
  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", actor_id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", actor_id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 获取好友社交玩家对象
  SocialActor *friend_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(friend_actor_id);
  if (NULL == friend_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", friend_actor_id);
    SendMessageHomelandTreeStealResponseFailed(social_actor);
    return;
  }
  HomelandActor *friend_homeland_actor = friend_social_actor->GetHomelandActor();

  // 检查是否可以抢收
  if (friend_homeland_actor->CheckTreeCanSteal(actor_id) == false) {
    LOG_WARNING("Actor(%lu) can not steal Actor(%lu)'s tree.", actor_id, friend_actor_id);
    SendMessageHomelandTreeStealResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(friend_homeland_actor->GetTreeLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.",
              friend_homeland_actor->GetTreeLevel());
    SendMessageHomelandTreeStealResponseFailed(social_actor);
    return;
  }

  // 增加抢收次数
  friend_homeland_actor->AddTreeStolenTimes(actor_id);

  core::int32 restrict_dollars =
    cell->harvest_restrict_dollars_reward_ * cell->stolen_loss_rate_ / 100;
  // 抢收好友神树事件
  {
    HomelandEvent event(entity::HomelandEventType::TREE_STEAL);
    event.PushEventVar(friend_social_actor->GetActorID());
    event.PushEventVar(restrict_dollars);
    homeland_actor->AddEvent(event);
  }
  // 神树被好友抢收事件
  {
    HomelandEvent event(entity::HomelandEventType::TREE_STOLEN);
    event.PushEventVar(social_actor->GetActorID());
    event.PushEventVar(restrict_dollars);
    friend_homeland_actor->AddEvent(event);
  }

  // 保存离线数据
  if (friend_social_actor->CheckOnline() == false) {
    friend_social_actor->Save();
  }

  // 发送家园好友数据同步
  gateway::protocol::MessageHomelandFriendSynchronize sync;
  sync.__set_type_(gateway::protocol::HomelandFriendOperateType::UPDATE);
  sync.data_.__set_actor_id_(friend_social_actor->GetStringID());
  sync.data_.__set_tree_can_steal_(false);
  social_actor->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE);

  // 转发 game_server
  session::protocol::MessageHomelandTreeStealRequest game_request;
  game_request.__set_actor_id_(actor_id);
  game_request.__set_tree_level_(friend_homeland_actor->GetTreeLevel());
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_STEAL_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandTreeStealRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageHomelandTreeStealRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTreeStealRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TREE_STEAL_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s]",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_TREE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_TREE is disable.");
    SendMessageHomelandTreeStealResponseFailed(social_actor);
    return;
  }

  core::uint64 friend_actor_id = atol(request.actor_id_.c_str());

  // 检查是否是好友
  if (social_actor->IsFriend(friend_actor_id) == false) {
    LOG_WARNING("Not a friend.");
    SendMessageHomelandTreeStealResponseFailed(social_actor);
    return;
  }

  if (SocialActorManager::GetInstance()->GetFromAll(friend_actor_id) == NULL) {
    // 离线加载
    SocialActorManager::GetInstance()->OfflineLoad(friend_actor_id,
        boost::bind(&HomelandTreeStealRequest, id, friend_actor_id));
  } else {
    HomelandTreeStealRequest(id, friend_actor_id);
  }
}

void HomelandMessageHandler::OnMessageHomelandTreeHarvestRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandTreeHarvestRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTreeHarvestRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TREE_HARVEST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_TREE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_TREE is disable.");
    SendMessageHomelandTreeHarvestResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(homeland_actor->GetTreeLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", homeland_actor->GetTreeLevel());
    SendMessageHomelandTreeHarvestResponseFailed(social_actor);
    return;
  }

  if (homeland_actor->GetTreeStage() != entity::HomelandTreeStageType::HARVEST) {
    LOG_WARNING("Not in harvest stage.");
    SendMessageHomelandTreeHarvestResponseFailed(social_actor);
    return;
  }

  core::int32 old_tree_level = homeland_actor->GetTreeLevel();
  core::int32 old_stolen_times = homeland_actor->GetTreeStolenTimes();
  // 增加收获次数
  homeland_actor->AddTreeHarvestTimes();
  // 转移到浇水阶段
  homeland_actor->MoveTreeToWateringStage();

  // 转发 game_server
  session::protocol::MessageHomelandTreeHarvestRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_tree_level_(old_tree_level);
  game_request.__set_stolen_times_(old_stolen_times);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TREE_HARVEST_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandTowerUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandTowerUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandTowerUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST from actor(%lu, %s), "
           "tower_id_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.tower_id_);

  // 检查参数
  if (request.tower_id_ != 1 && request.tower_id_ != 2) {
    LOG_WARNING("request.tower_id_ is invalid.");
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_TOWER) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_TOWER is disable.");
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(SocialActor::kHomelandUpgradeLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查建造冷却时间
  if (homeland_actor->CheckUpgradeCooling() == false) {
    LOG_WARNING("In upgrade cooling.");
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 炮塔等级不能超过神殿等级
  if (homeland_actor->GetTowerLevel(request.tower_id_) >= homeland_actor->GetTempleLevel()) {
    LOG_WARNING("Tower level can not greater than temple level.");
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查是否顶级
  const global::configure::HomelandTowerCell *cell =
    SOCIAL_CONF()->GetHomelandTower(homeland_actor->GetTowerLevel(request.tower_id_) + 1);
  if (NULL == cell) {
    LOG_WARNING("Tower(%d) is already in top level.", request.tower_id_);
    SendMessageHomelandTowerUpgradeResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(SocialActor::kHomelandUpgradeLock);

  // 转发 game_server
  session::protocol::MessageHomelandTowerUpgradeRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_tower_id_(request.tower_id_);
  game_request.__set_tower_level_(homeland_actor->GetTowerLevel(request.tower_id_));
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandStrongholdUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandStrongholdUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandStrongholdUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_STRONGHOLD) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_STRONGHOLD is disable.");
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 锁检查
  if (social_actor->GetLock()->CheckLocked(SocialActor::kHomelandUpgradeLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查建造冷却时间
  if (homeland_actor->CheckUpgradeCooling() == false) {
    LOG_WARNING("In upgrade cooling.");
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 军团据点等级不能超过神殿等级
  if (homeland_actor->GetStrongholdLevel() >= homeland_actor->GetTempleLevel()) {
    LOG_WARNING("Stronghold level can not greater than temple level.");
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 检查是否顶级
  const global::configure::HomelandStrongholdCell *cell =
    SOCIAL_CONF()->GetHomelandStronghold(homeland_actor->GetStrongholdLevel() + 1);
  if (NULL == cell) {
    LOG_WARNING("Stronghold is already in top level.");
    SendMessageHomelandStrongholdUpgradeResponseFailed(social_actor);
    return;
  }

  // 加锁
  social_actor->GetLock()->Lock(SocialActor::kHomelandUpgradeLock);

  // 转发 game_server
  session::protocol::MessageHomelandStrongholdUpgradeRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_stronghold_level_(homeland_actor->GetStrongholdLevel());
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST);
}

void HomelandMessageHandler::OnMessageHomelandStrongholdCallSupportRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandStrongholdCallSupportRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandStrongholdCallSupportRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_STRONGHOLD) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_STRONGHOLD is disable.");
    SendMessageHomelandStrongholdCallSupportResponseFailed(social_actor);
    return;
  }

  // 检查申请支援冷却时间
  if (homeland_actor->GetStrongholdCallSupportCooling() != 0) {
    LOG_WARNING("In stronghold call support cooling.");
    SendMessageHomelandStrongholdCallSupportResponseFailed(social_actor);
    return;
  }

  // 获取公会成员列表
  request::RequestGuildGetMembers guild_request;
  guild_request.__set_actor_id_(session_actor->GetActorID());
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GUILD_GET_MEMBERS,
          &guild_request, sizeof(guild_request)) != 0) {
    LOG_WARNING("Get guild members of Actor(%lu) failed.", session_actor->GetActorID());
    SendMessageHomelandStrongholdCallSupportResponseFailed(social_actor);
    return;
  }

  const global::configure::HomelandStrongholdCell *cell =
    SOCIAL_CONF()->GetHomelandStronghold(homeland_actor->GetStrongholdLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandStrongholdCell(%d) not found in config.",
              homeland_actor->GetStrongholdLevel());
    SendMessageHomelandStrongholdCallSupportResponseFailed(social_actor);
    return;
  }

  // 设置申请支援冷却时间
  homeland_actor->SetStrongholdCallSupportCooling(cell->call_support_cooling_);

  // 广播通知
  gateway::protocol::MessageHomelandStrongholdCallSupportNotify notify;
  notify.__set_actor_id_(session_actor->GetStringID());
  notify.__set_actor_name_(session_actor->GetName());

  for (size_t i = 0; i < guild_request.members_.size(); ++i) {
    SessionActor *guild_session_actor =
          SessionActorManager::GetInstance()->GetActor(guild_request.members_[i]);
      if (NULL == guild_session_actor) {
        continue;
      }
      guild_session_actor->SendMessage(notify,
          gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_NOTIFY);
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandStrongholdCallSupportResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_RESPONSE);
}

static void HomelandStrongholdSupportRequest(core::uint64 actor_id,
    core::uint64 support_actor_id, core::uint32 soul_id) {
  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", actor_id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", actor_id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 获取支援目标社交玩家对象
  SocialActor *support_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(support_actor_id);
  if (NULL == support_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", support_actor_id);
    SendMessageHomelandStrongholdSupportResponseFailed(social_actor);
    return;
  }
  HomelandActor *support_homeland_actor = support_social_actor->GetHomelandActor();

  // 检查英灵是否可用于支援
  if (homeland_actor->CheckSoulCanSupport(soul_id) == false) {
    LOG_WARNING("Soul(%u) can not support, maybe in cooling or not exist.", soul_id);
    SendMessageHomelandStrongholdSupportResponseFailed(social_actor);
    return;
  }

  // 检查是否可支援目标玩家
  if (support_homeland_actor->CheckStrongholdCanSupport(actor_id) == false) {
    LOG_WARNING("Actor(%lu) can not support Actor(%lu)'s stronghold, "
                "maybe in cooling or not in same guild.",
                actor_id, support_actor_id);
    SendMessageHomelandStrongholdSupportResponseFailed(social_actor);
    return;
  }

  core::int32 soul_level = homeland_actor->GetSoulLevel(soul_id);
  if (-1 == soul_level) {
    LOG_WARNING("Get Soul(%u)'s level failed.", soul_id);
    SendMessageHomelandStrongholdSupportResponseFailed(social_actor);
    return;
  }

  // 支援
  homeland_actor->AddStrongholdSupportSoul(soul_id, support_actor_id);
  support_homeland_actor->AddStrongholdSupportedSoul(actor_id, soul_id, soul_level);

  // 增加公会贡献值
  FacadeRequest::GetInstance()->AddGuildContribution(actor_id,
      MISC_CONF()->homeland_stronghold_support_guild_contribution_reward_);

  // 军团据点支援事件
  {
    HomelandEvent event(entity::HomelandEventType::STRONGHOLD_SUPPORT);
    event.PushEventVar(soul_id);
    event.PushEventVar(support_social_actor->GetActorID());
    event.PushEventVar(MISC_CONF()->homeland_stronghold_support_guild_contribution_reward_);
    homeland_actor->AddEvent(event);
  }

  // 军团据点被支援事件
  {
    HomelandEvent event(entity::HomelandEventType::STRONGHOLD_SUPPORTED);
    event.PushEventVar(social_actor->GetActorID());
    event.PushEventVar(soul_id);
    support_homeland_actor->AddEvent(event);
  }

  // 保存离线数据
  if (support_social_actor->CheckOnline() == false) {
    support_social_actor->Save();
  }

  // 发送成功回复
  gateway::protocol::MessageHomelandStrongholdSupportResponse response;
  response.__set_result_(true);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_RESPONSE);
}

void HomelandMessageHandler::OnMessageHomelandStrongholdSupportRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandStrongholdSupportRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandStrongholdSupportRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s], soul_id_=[%u].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str(), request.soul_id_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false) {
    LOG_WARNING("HOMELAND_MODULE is disable.");
    SendMessageHomelandStrongholdSupportResponseFailed(social_actor);
    return;
  }

  core::uint64 support_actor_id = atol(request.actor_id_.c_str());

  // 检查 support_actor_id 是否存在
  if (SessionActorManager::GetInstance()->GetBriefActor(support_actor_id) == NULL) {
    LOG_WARNING("support_actor_id(%lu) is invalid.", support_actor_id);
    SendMessageHomelandStrongholdSupportResponseFailed(social_actor);
    return;
  }

  if (SocialActorManager::GetInstance()->GetFromAll(support_actor_id) == NULL) {
    // 离线加载
    SocialActorManager::GetInstance()->OfflineLoad(support_actor_id,
        boost::bind(&HomelandStrongholdSupportRequest, id, support_actor_id, request.soul_id_));
  } else {
    HomelandStrongholdSupportRequest(id, support_actor_id, request.soul_id_);
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

