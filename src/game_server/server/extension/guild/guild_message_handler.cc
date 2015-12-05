#include "game_server/server/extension/guild/guild_message_handler.h"

#include <boost/bind.hpp>

#include "entity/guild_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/actor_id.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/game_server.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/guild/facade_request.h"
#include "game_server/server/extension/guild/guild.h"
#include "game_server/server/extension/guild/guild_manager.h"
#include "game_server/server/extension/guild/guild_actor.h"
#include "game_server/server/extension/guild/guild_actor_manager.h"

namespace game {

namespace server {

namespace guild {

GuildMessageHandler::GuildMessageHandler() {}
GuildMessageHandler::~GuildMessageHandler() {}

bool GuildMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_CREATE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildCreateRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_DONATE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildDonateRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildShopBuyItemRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_AWARD_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildPlayingAwardRequest, this, _1, _2, _3));

  return true;
}

void GuildMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_CREATE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_DONATE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_AWARD_REQUEST);
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

static void SendMessageGuildPlayingAwardResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildPlayingAwardResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_AWARD_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildCreateRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  GameActor *game_actor = guild_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildCreateRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildCreateRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_CREATE_REQUEST from actor(%lu, %s), "
           "guild_name_=[%s], guild_logo_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.guild_name_.c_str(), request.guild_logo_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查是否已加入公会
  if (guild_actor->GetGuildID() != 0) {
    LOG_WARNING("already in a guild.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查公会名长度
  ssize_t visual_length = global::GetStrVisualLengthUTF8(request.guild_name_.c_str());
  if (visual_length < 0 || visual_length > MISC_CONF()->guild_max_name_length_) {
    LOG_WARNING("Guild name is too long.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 公会名关键字过滤
  if (GameServerSingleton::GetInstance().GetWordFilter()->Search(request.guild_name_) == true) {
    LOG_WARNING("Guild name is invalid.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::GUILD_NAME_INVALID);
    return;
  }

  // 拼出实际的公会名
  global::ActorID decoded_actor_id(id);
  char guild_name[256] = "";
  snprintf(guild_name, sizeof(guild_name), "[%d区]%s",
      decoded_actor_id.zone_, request.guild_name_.c_str()); 

  // 检查公会名是否重复
  if (GuildManager::GetInstance()->GetGuild(guild_name) != NULL) {
    LOG_WARNING("Guild name already exists.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::GUILD_NAME_EXIST);
    return;
  }

  // 检查公会标志是否有效
  if (GUILD_CONF()->CheckGuildLogoExist(request.guild_logo_) == false) {
    LOG_WARNING("Guild logo is invalid.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查 session_server 连接是否存在
  if (SessionChannel::CheckSessionLogined() == false) {
    LOG_WARNING("session not logined.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查消耗
  // --检查金币
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) <
      MISC_CONF()->guild_create_spend_gold_) {
    LOG_WARNING("Gold is not enough.");
    SendMessageGuildCreateResponse(guild_actor, gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 扣除消耗
  if (MISC_CONF()->guild_create_spend_gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - MISC_CONF()->guild_create_spend_gold_);
  }

  // 向 session_server 发出创建公会请求
  session::protocol::MessageGuildCreateRequest session_request;
  session_request.__set_actor_id_(id);
  session_request.__set_guild_name_(guild_name);
  session_request.__set_guild_logo_(request.guild_logo_);
  SessionChannel::SendMessage(session_request,
      session::protocol::GameMessageType::MESSAGE_GUILD_CREATE_REQUEST);
}

void GuildMessageHandler::OnMessageGuildDonateRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  GameActor *game_actor = guild_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildDonateRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildDonateRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_DONATE_REQUEST from actor(%lu, %s), "
           "type_=[%d], num_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.type_, request.num_);

  // 检查协议参数
  if (request.num_ <= 0) {
    LOG_WARNING("request.num_ is invalid.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  // 检查是否已加入公会
  if (guild_actor->GetGuildID() == 0) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  // 检查是否有捐献的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_donate_) {
    LOG_WARNING("No authority_donate.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  // 检查 session_server 连接是否存在
  if (SessionChannel::CheckSessionLogined() == false) {
    LOG_WARNING("session not logined.");
    SendMessageGuildDonateResponseFailed(guild_actor);
    return;
  }

  if (entity::GuildDonateType::GOLD == request.type_) {
    // 检查金币
    core::int32 gold = game_actor->GetAttribute(entity::ActorClientFields::GOLD);
    // 防溢出检查
    if (gold / 10000 < request.num_) {
      LOG_WARNING("Gold is not enough");
      SendMessageGuildDonateResponseFailed(guild_actor);
      return;
    }
    // 扣除金币
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - request.num_ * 10000);

  } else if (entity::GuildDonateType::FREEDOM_DOLLARS == request.type_) {
    // 检查钻石
    core::int32 freedom_dollars =
      game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
    if (freedom_dollars < request.num_) {
      LOG_WARNING("Freedom dollars is not enough.");
      SendMessageGuildDonateResponseFailed(guild_actor);
      return;
    }
    // 扣除钻石
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - request.num_);

  } else {
    // 捐献道具
    core::uint32 item_id = 0; 
    core::uint32 item_count = request.num_;

    if (entity::GuildDonateType::LIGHT_CRYSTAL == request.type_) {
      item_id = MISC_CONF()->guild_light_crystal_item_id_;
    } else if (entity::GuildDonateType::DARK_CRYSTAL == request.type_) {
      item_id = MISC_CONF()->guild_dark_crystal_item_id_;
    } else if (entity::GuildDonateType::HOLY_CRYSTAL == request.type_) {
      item_id = MISC_CONF()->guild_holy_crystal_item_id_;
    } else {
      LOG_WARNING("Invalid request type");
      SendMessageGuildDonateResponseFailed(guild_actor);
      return;
    }
    // 检查道具数量
    if (FacadeRequest::GetInstance()->CheckPacketItemExist(id,
            item_id, item_count, entity::BindRequireType::BOTH) == false) {
      LOG_WARNING("Item(%u) is not enough.", item_id);
      SendMessageGuildDonateResponseFailed(guild_actor);
      return;
    }
    // 扣除道具
    core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(id,
        item_id, item_count, entity::BindRequireType::BOTH);
    if (delete_number != item_count) {
      LOG_ERROR("Remove item(%d) failed, req_count=[%d], del_count=[%d].",
                item_id, item_count, delete_number);
      SendMessageGuildDonateResponseFailed(guild_actor);
      return;
    }
  }

  // 转发 session_server
  session::protocol::MessageGuildDonateRequest session_request;
  session_request.__set_actor_id_(id);
  session_request.__set_type_(request.type_);
  session_request.__set_num_(request.num_);
  SessionChannel::SendMessage(session_request,
      session::protocol::GameMessageType::MESSAGE_GUILD_DONATE_REQUEST);
}

void GuildMessageHandler::OnMessageGuildShopBuyItemRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  GameActor *game_actor = guild_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildShopBuyItemRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildShopBuyItemRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST from actor(%lu, %s), "
           "item_id_=[%d], item_count=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.item_id_, request.item_count_);

  // 检查协议参数
  if (request.item_count_ <= 0) {
    LOG_WARNING("request.item_count_ is invalid.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查是否已加入公会
  if (guild_actor->GetGuildID() == 0) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查是否有购买公会商店道具的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_shop_buy_item_) {
    LOG_WARNING("No authority_shop_buy_item.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查 session_server 连接是否存在
  if (SessionChannel::CheckSessionLogined() == false) {
    LOG_WARNING("session not logined.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查道具是否存在
  const global::configure::GuildShopItemCell *item_cell =
    GUILD_CONF()->GetGuildShopItem(request.item_id_);
  if (NULL == item_cell) {
    LOG_WARNING("GuildShopItem(%d) not found in config.", request.item_id_);
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  Guild *guild = GuildManager::GetInstance()->GetGuild(guild_actor->GetGuildID());
  if (NULL == guild) {
    LOG_ERROR("Get Guild(%lu) from GuildManager failed.", guild_actor->GetGuildID());
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查商店等级
  if (guild->GetShopLevel() < item_cell->required_guild_shop_level_) {
    LOG_WARNING("Guild shop level is too low.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查属性和
  if (guild->GetAttributeSum() < item_cell->required_guild_attribute_sum_) {
    LOG_WARNING("Guild attribute sum is too low.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查背包空位
  if (FacadeRequest::GetInstance()->CheckAddPacketItem(id,
          request.item_id_, request.item_count_, entity::BindRequireType::BIND) == false) {
    LOG_WARNING("Can not add item.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 检查消耗
  // --检查钻石
  core::int32 freedom_dollars =
    game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
  // --防溢出检查
  if (freedom_dollars / request.item_count_ < item_cell->spend_freedom_dollars_) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }
  // --检查贡献值
  // --防溢出检查
  if (guild_actor->GetCurrentContributionValue() / request.item_count_ < item_cell->spend_contribution_) {
    LOG_WARNING("Contribution is not enough.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  // --扣除钻石
  if (item_cell->spend_freedom_dollars_ > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - item_cell->spend_freedom_dollars_ * request.item_count_);
  }
  // --扣除贡献值(锁定该值, 实际扣除在 session_server 上操作, 成功后会被 session_server 同步)
  if (item_cell->spend_contribution_ > 0) {
    guild_actor->SetCurrentContributionValue(
        guild_actor->GetCurrentContributionValue() -
        item_cell->spend_contribution_ * request.item_count_);
  }

  // 转发 session_server
  session::protocol::MessageGuildShopBuyItemRequest session_request;
  session_request.__set_actor_id_(id);
  session_request.__set_item_id_(request.item_id_);
  session_request.__set_item_count_(request.item_count_);
  SessionChannel::SendMessage(session_request,
      session::protocol::GameMessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST);
}

void GuildMessageHandler::OnMessageGuildPlayingAwardRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  GameActor *game_actor = guild_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildPlayingAwardRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildPlayingAwardRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_PLAYING_AWARD_REQUEST from actor(%lu, %s), "
           "playing_template_id_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.playing_template_id_);

  // 检查协议参数
  const global::configure::GuildPlayingCell *cell =
      GUILD_CONF()->GetGuildPlaying(request.playing_template_id_);
  if (NULL == cell) {
    LOG_WARNING("playing_template_id is invalid.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor);
    return;
  }

  // 检查背包空位
  if (FacadeRequest::GetInstance()->CheckAddPacketItems(id, cell->awards_) == false) {
    LOG_WARNING("Can not add item.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor);
    return;
  }

  // 转发 session_server
  session::protocol::MessageGuildPlayingAwardRequest session_request;
  session_request.__set_actor_id_(id);
  session_request.__set_playing_template_id_(request.playing_template_id_);
  SessionChannel::SendMessage(session_request,
      session::protocol::GameMessageType::MESSAGE_GUILD_PLAYING_AWARD_REQUEST);
}

}  // namespace guild

}  // namespace server

}  // namespace game

