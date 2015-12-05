#include "session_server/server/extension/guild/game_guild_message_handler.h"

#include <boost/bind.hpp>

#include "entity/inform_types.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/guild/guild.h"
#include "session_server/server/extension/guild/guild_manager.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"

#define MAX_GUILD_ONCE 100 

namespace session {

namespace server {

namespace guild {

GameGuildMessageHandler::GameGuildMessageHandler() {}
GameGuildMessageHandler::~GameGuildMessageHandler() {}

bool GameGuildMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_REQUEST,
      boost::bind(&GameGuildMessageHandler::OnMessageGuildInfoRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_CREATE_REQUEST,
      boost::bind(&GameGuildMessageHandler::OnMessageGuildCreateRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_DONATE_REQUEST,
      boost::bind(&GameGuildMessageHandler::OnMessageGuildDonateRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST,
      boost::bind(&GameGuildMessageHandler::OnMessageGuildShopBuyItemRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_PLAYING_AWARD_REQUEST,
      boost::bind(&GameGuildMessageHandler::OnMessageGuildPlayingAwardRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_ADD_CONTRIBUTION_REQUEST,
      boost::bind(&GameGuildMessageHandler::OnMessageGuildAddContributionRequest, this, _1, _2, _3));

  return true;
}

void GameGuildMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_CREATE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_DONATE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_PLAYING_AWARD_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_ADD_CONTRIBUTION_REQUEST);
}

static void SendMessageGuildCreateResponse(GuildActor *guild_actor,
    gateway::protocol::GuildErrorCode::type error_code) {
  gateway::protocol::MessageGuildCreateResponse response;
  response.__set_result_(error_code);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_CREATE_RESPONSE);
}

static void SendMessageGuildDonateResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildDonateResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_DONATE_RESPONSE);
}

static void SendMessageGuildShopBuyItemResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildShopBuyItemResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE);
}

static void SendMessageGuildPlayingAwardResponseFailed(GuildActor *guild_actor,
    SessionTerminal *terminal) {
  session::protocol::MessageGuildPlayingAwardResponse response;
  response.__set_actor_id_(guild_actor->GetActorID());
  response.__set_result_(false);
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_GUILD_PLAYING_AWARD_RESPONSE);
}

void GameGuildMessageHandler::OnMessageGuildInfoRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildInfoRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildInfoRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_INFO_REQUEST from game_server.");

  // 发送公会信息到 game_server
  session::protocol::MessageGuildInfoResponse response;
  const GuildManager::GuildMap &guilds = GuildManager::GetInstance()->GetGuilds();
  size_t guild_count = 0;

  GuildManager::GuildMap::const_iterator iter = guilds.begin();
  for (; iter != guilds.end(); ++iter) {
    const Guild *guild = iter->second;

    session::protocol::GuildData data;
    data.__set_id_(guild->GetID());
    data.__set_name_(guild->GetName());
    data.__set_level_(guild->GetLevel());
    data.__set_light_attribute_(guild->GetLightAttribute());
    data.__set_dark_attribute_(guild->GetDarkAttribute());
    data.__set_holy_attribute_(guild->GetHolyAttribute());
    data.__set_shop_level_(guild->GetShopLevel());
    data.__set_college_level_(guild->GetCollegeLevel());
    data.__set_barrack_level_(guild->GetBarrackLevel());

    response.guilds_.push_back(data);

    if (response.guilds_.size() >= MAX_GUILD_ONCE) {
      terminal->SendGameMessage(response,
          session::protocol::GameMessageType::MESSAGE_GUILD_INFO_RESPONSE);
      response.guilds_.clear();
    }
  }

  if (!response.guilds_.empty()) {
    terminal->SendGameMessage(response,
        session::protocol::GameMessageType::MESSAGE_GUILD_INFO_RESPONSE);
  }
}

void GameGuildMessageHandler::OnMessageGuildCreateRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildCreateRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildCreateRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_CREATE_REQUEST from game_server, "
           "actor_id_=[%lu], guild_name_=[%s], guild_logo_=[%d].",
           request.actor_id_, request.guild_name_.c_str(), request.guild_logo_);

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request.actor_id_);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", request.actor_id_);
    return;
  }

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_ERROR("GUILD_MODULE is disable.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 新建公会
  if (GuildManager::GetInstance()->CreateGuild(request.guild_name_,
          request.guild_logo_, guild_actor) == false) {
    LOG_ERROR("Create guild failed.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 创建公会事件
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_ERROR("guild is null.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }
  GuildEvent event(entity::GuildEventType::CREATE_GUILD);
  event.PushEventVar(guild_actor->GetActorID());
  guild->AddEvent(event);
  // 创建公会公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_CREATE);
  inform.params_.push_back(guild_actor->GetStringID());
  inform.params_.push_back(guild_actor->GetName());
  inform.params_.push_back(guild->GetStringID());
  inform.params_.push_back(guild->GetName());
  SessionServerSingleton::GetInstance().BroadcastMessage(inform,
      gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  // 发送创建成功回复
  SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::SUCCESS);
}

void GameGuildMessageHandler::OnMessageGuildDonateRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildDonateRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildDonateRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_DONATE_REQUEST from game_server, "
           "actor_id=[%lu], type_=[%d], num_=[%d].",
           request.actor_id_, request.type_, request.num_);

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request.actor_id_);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", request.actor_id_);
    return;
  }

  if (request.num_ <= 0) {
    LOG_ERROR("request.num_ is invalid.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  if (request.type_ < entity::GuildDonateType::MIN ||
      request.type_ >= entity::GuildDonateType::MAX) {
    LOG_ERROR("request.type_ is invalid.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_ERROR("GUILD_MODULE is disable.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_ERROR("Not in a guild.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  guild->Donate(guild_actor, request.type_, request.num_);

  gateway::protocol::MessageGuildDonateResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_DONATE_RESPONSE);
}

void GameGuildMessageHandler::OnMessageGuildShopBuyItemRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildShopBuyItemRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildShopBuyItemRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST from game_server, "
           "actor_id=[%lu], item_id_=[%d], item_count_=[%d].",
           request.actor_id_, request.item_id_, request.item_count_);

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request.actor_id_);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", request.actor_id_);
    return;
  }

  if (request.item_count_ <= 0) {
    LOG_ERROR("request.item_count_ is invalid.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_ERROR("GUILD_MODULE is disable.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_ERROR("Not in a guild.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查道具是否存在
  const global::configure::GuildShopItemCell *item_cell =
    GUILD_CONF()->GetGuildShopItem(request.item_id_);
  if (NULL == item_cell) {
    LOG_ERROR("GuildShopItem(%d) not found in config.", request.item_id_);
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查商店等级
  if (guild->GetShopLevel() < item_cell->required_guild_shop_level_) {
    LOG_ERROR("Guild shop level is too low.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查属性和
  if (guild->GetAttributeSum() < item_cell->required_guild_attribute_sum_) {
    LOG_ERROR("Guild attribute sum is too low.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查贡献值
  if (guild_actor->GetCurrentContributionValue() / request.item_count_ <
      item_cell->spend_contribution_) {
    LOG_ERROR("Contribution is not enough.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 扣除贡献值
  if (item_cell->spend_contribution_ > 0) {
    guild_actor->DelContribution(item_cell->spend_contribution_ * request.item_count_);
  }

  // 通知 game_server 发放道具
  session::protocol::MessageGuildShopBuyItemResponse game_response;
  game_response.__set_actor_id_(request.actor_id_);
  game_response.__set_item_id_(request.item_id_);
  game_response.__set_item_count_(request.item_count_);
  terminal->SendGameMessage(game_response,
      session::protocol::GameMessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE);
}

void GameGuildMessageHandler::OnMessageGuildPlayingAwardRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildPlayingAwardRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildPlayingAwardRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_PLAYING_AWARD_REQUEST from game_server, "
           "actor_id_=[%lu], playing_template_id_=[%lu].",
           request.actor_id_, request.playing_template_id_);

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request.actor_id_);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", request.actor_id_);
    return;
  }

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor, terminal);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor, terminal);
    return;
  }

  // 检查公会是否完成了该副本
  if (guild->CheckPlayingFinished(request.playing_template_id_) == false) {
    LOG_WARNING("Playing is not finished.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor, terminal);
    return;
  }

  // 检查是否已领取了该副本的奖励
  if (guild_actor->CheckPlayingAwarded(request.playing_template_id_)) {
    LOG_WARNING("Playing is already awarded.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor, terminal);
    return;
  }

  // 设置已领取奖励
  guild_actor->SetPlayingAwarded(request.playing_template_id_);

  // 转发 game_server
  session::protocol::MessageGuildPlayingAwardResponse response;
  response.__set_actor_id_(request.actor_id_);
  response.__set_result_(true);
  response.__set_playing_template_id_(request.playing_template_id_);
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_GUILD_PLAYING_AWARD_RESPONSE);
}

void GameGuildMessageHandler::OnMessageGuildAddContributionRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildAddContributionRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildAddContributionRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_ADD_CONTRIBUTION_REQUEST from game_server, "
           "actor_id_=[%lu], add_value_=[%d].",
           request.actor_id_, request.add_value_);

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->GetFromAll(request.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request.actor_id_);
    return;
  }

  if (guild_actor->GetGuild() != NULL && request.add_value_ > 0) {
    guild_actor->AddContribution(request.add_value_);
  }
}

}  // namespace guild

}  // namespace server

}  // namespace session

