#include "game_server/server/extension/auction/session_auction_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_auction_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_auction_protocol_types.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/extension/auction/facade_request.h"
#include "game_server/server/extension/auction/auction_actor.h"
#include "game_server/server/extension/auction/auction_actor_manager.h"

namespace game {

namespace server {

namespace auction {

SessionAuctionMessageHandler::SessionAuctionMessageHandler() {}
SessionAuctionMessageHandler::~SessionAuctionMessageHandler() {}

bool SessionAuctionMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_SELL_REQUEST,
      boost::bind(&SessionAuctionMessageHandler::OnMessageAuctionSellRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BID_REQUEST,
      boost::bind(&SessionAuctionMessageHandler::OnMessageAuctionBidRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BUYOUT_REQUEST,
      boost::bind(&SessionAuctionMessageHandler::OnMessageAuctionBuyoutRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST,
      boost::bind(&SessionAuctionMessageHandler::OnMessageAuctionWithdrawItemRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST2,
      boost::bind(&SessionAuctionMessageHandler::OnMessageAuctionWithdrawItemRequest2,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_MONEY_REQUEST,
      boost::bind(&SessionAuctionMessageHandler::OnMessageAuctionWithdrawMoneyRequest,
          this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_GIVE_MONEY_BACK_REQUEST,
      boost::bind(&SessionAuctionMessageHandler::OnMessageAuctionGiveMoneyBackRequest,
          this, _1, _2));

  return true;
}

void SessionAuctionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_SELL_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BID_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BUYOUT_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST2);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_MONEY_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_GIVE_MONEY_BACK_REQUEST);
}

static void SendMessageAuctionSellResponseFailed(AuctionActor *auction_actor) {
  session::protocol::MessageAuctionSellResponse response;
  response.__set_actor_id_(auction_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_SELL_RESPONSE);
}

static void SendMessageAuctionBidResponseFailed(AuctionActor *auction_actor) {
  session::protocol::MessageAuctionBidResponse response;
  response.__set_actor_id_(auction_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_BID_RESPONSE);
}

static void SendMessageAuctionBuyoutResponseFailed(AuctionActor *auction_actor) {
  session::protocol::MessageAuctionBuyoutResponse response;
  response.__set_actor_id_(auction_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_BUYOUT_RESPONSE);
}

static void SendMessageAuctionWithdrawItemResponseFailed(AuctionActor *auction_actor) {
  session::protocol::MessageAuctionWithdrawItemResponse response;
  response.__set_actor_id_(auction_actor->GetID());
  response.__set_result_(false);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_RESPONE);
}

void SessionAuctionMessageHandler::OnMessageAuctionSellRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionSellRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionSellRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_SELL_REQUEST from session_server, actor_id_=[%lu], "
           "item_id_=[%lu], item_template_id_=[%u], item_number_=[%d], "
           "transaction_id_=[%s], start_bid_price_=[%d], buyout_price_=[%d], "
           "auction_time_id_=[%d].",
           request.actor_id_, request.item_id_, request.item_template_id_,
           request.item_number_, request.transaction_id_.c_str(), request.start_bid_price_,
           request.buyout_price_, request.auction_time_id_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = auction_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查是否足够支付拍卖金
  const global::configure::AuctionTimeCell *cell =
    AUCTION_CONF()->GetAuctionTime(request.auction_time_id_);
  if (NULL == cell) {
    LOG_ERROR("AuctionTimeCell(%d) not found in config.", request.auction_time_id_);
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) < cell->fee_) {
    LOG_WARNING("gold is not enough");
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  session::protocol::MessageAuctionSellResponse response;
  response.__set_actor_id_(request.actor_id_);
  response.__set_result_(true);
  response.__set_start_bid_price_(request.start_bid_price_);
  response.__set_buyout_price_(request.buyout_price_);
  response.__set_auction_time_id_(request.auction_time_id_);

  if (request.__isset.item_id_) {
    // 装备类道具
    // --获取道具信息
    request::RequestGetPacketItemTemplate item_info_request;
    item_info_request.__set_actor_id_(request.actor_id_);
    item_info_request.__set_item_id_(request.item_id_);
    item_info_request.__isset.item_extra_info_ = true;
    if (ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_GET_PAKCET_ITEM_TEMPLATE,
            &item_info_request, sizeof(item_info_request)) == -1) {
      LOG_WARNING("Get item info for item_id(%lu) failed.", request.item_id_);
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // --检查是否是绑定道具
    if (item_info_request.bind_) {
      LOG_WARNING("Item(%lu) is bind, can not sell.",request.item_id_);
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // --扣除道具
    if (FacadeRequest::GetInstance()->RemovePacketItemByItemID(
            request.actor_id_, request.item_id_, 1) == false) {
      LOG_ERROR("Remove item(%lu) failed.", request.item_id_);
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    response.__set_item_template_id_(item_info_request.item_template_);
    response.__set_item_number_(1);
    response.__set_item_extra_info_(item_info_request.item_extra_info_);

  } else if (request.__isset.item_template_id_ && request.__isset.item_number_) {
    // 其他道具
    // --检查道具是否足够
    if (FacadeRequest::GetInstance()->CheckPacketItemExist(
            request.actor_id_, request.item_template_id_, request.item_number_,
            entity::BindRequireType::NOT_BIND) == false) {
      LOG_WARNING("Item(%u) is not enough.", request.item_template_id_);
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // --扣除道具
    core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(
        request.actor_id_, request.item_template_id_, request.item_number_,
        entity::BindRequireType::NOT_BIND);
    if (delete_number != (core::uint32)request.item_number_) {
      LOG_ERROR("Remove item(%u) failed, req_count=[%d], del_count=[%d].",
                request.item_template_id_, request.item_number_, delete_number);
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    response.__set_item_template_id_(request.item_template_id_);
    response.__set_item_number_(request.item_number_);
    response.__set_item_extra_info_("");

  } else if (request.__isset.transaction_id_) {
    // 道具重新上架
    response.__set_transaction_id_(request.transaction_id_);

  } else {
    LOG_ERROR("invalid params.");
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  // 扣除金币
  if (cell->fee_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, -1 * cell->fee_);
  }

  // 转发 session_server
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_SELL_RESPONSE);
}

void SessionAuctionMessageHandler::OnMessageAuctionBidRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionBidRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionBidRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_BID_REQUEST from session_server, "
           "actor_id_=[%lu], transaction_id_=[%s], bid_price_=[%d].",
           request.actor_id_, request.transaction_id_.c_str(), request.bid_price_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = auction_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查钻石
  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < request.bid_price_) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageAuctionBidResponseFailed(auction_actor);
    return;
  }

  // 扣除钻石
  if (request.bid_price_ > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, -1 * request.bid_price_);
  }

  // 转发 session_server
  session::protocol::MessageAuctionBidResponse response;
  response.__set_actor_id_(request.actor_id_);
  response.__set_result_(true);
  response.__set_transaction_id_(request.transaction_id_);
  response.__set_bid_price_(request.bid_price_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_BID_RESPONSE);
}

void SessionAuctionMessageHandler::OnMessageAuctionBuyoutRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionBuyoutRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionBuyoutRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_BUYOUT_REQUEST from session_server, "
           "actor_id_=[%lu], transaction_id_=[%s], "
           "buyout_price_=[%d], give_back_money_=[%d].",
           request.actor_id_, request.transaction_id_.c_str(),
           request.buyout_price_, request.give_back_money_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = auction_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查钻石
  core::int32 freedom_dollars = request.buyout_price_ - request.give_back_money_;
  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageAuctionBuyoutResponseFailed(auction_actor);
    return;
  }

  // 扣除钻石
  if (request.buyout_price_ > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, -1 * freedom_dollars);
  }

  // 转发 session_server
  session::protocol::MessageAuctionBuyoutResponse response;
  response.__set_actor_id_(request.actor_id_);
  response.__set_result_(true);
  response.__set_transaction_id_(request.transaction_id_);
  response.__set_buyout_price_(request.buyout_price_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_BUYOUT_RESPONSE);
}

void SessionAuctionMessageHandler::OnMessageAuctionWithdrawItemRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionWithdrawItemRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionWithdrawItemRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST from session_server, "
           "actor_id_=[%lu], transaction_id_=[%s], "
           "item_template_id_=[%u], item_number_=[%d].",
           request.actor_id_, request.transaction_id_.c_str(),
           request.item_template_id_, request.item_number_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = auction_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 检查背包空位
  if (FacadeRequest::GetInstance()->CheckAddPacketItem(request.actor_id_,
          request.item_template_id_, request.item_number_,
          entity::BindRequireType::NOT_BIND) == false) {
    LOG_WARNING("Can not add item.");
    SendMessageAuctionWithdrawItemResponseFailed(auction_actor);
    return;
  }

  // 发送成功回复
  session::protocol::MessageAuctionWithdrawItemResponse response;
  response.__set_actor_id_(auction_actor->GetID());
  response.__set_result_(true);
  response.__set_transaction_id_(request.transaction_id_);
  SessionChannel::SendMessage(response,
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_RESPONE);
}

void SessionAuctionMessageHandler::OnMessageAuctionWithdrawItemRequest2(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionWithdrawItemRequest2 request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionWithdrawItemRequest2 failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST2 from session_server, "
           "actor_id_=[%lu], item_template_id_=[%u], item_number_=[%d], "
           "item_extra_info_=[%s].",
           request.actor_id_, request.item_template_id_, request.item_number_,
           request.item_extra_info_.c_str());

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = auction_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 添加道具
  bool result = FacadeRequest::GetInstance()->AddPacketItem(request.actor_id_,
      request.item_template_id_, request.item_number_,
      entity::BindRequireType::NOT_BIND, request.item_extra_info_);

  // 发送成功回复
  gateway::protocol::MessageAuctionWithdrawResponse response;
  response.__set_result_(result);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_WITHDRAW_RESPONSE);
}

void SessionAuctionMessageHandler::OnMessageAuctionWithdrawMoneyRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionWithdrawMoneyRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionWithdrawMoneyRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_WITHDRAW_MONEY_REQUEST from session_server, "
           "actor_id_=[%lu], money_=[%d].",
           request.actor_id_, request.money_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = auction_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 增加钻石 (request.money_可能为负数)
  game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, request.money_);

  // 发送成功回复
  gateway::protocol::MessageAuctionWithdrawResponse response;
  response.__set_result_(true);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_WITHDRAW_RESPONSE);
}

void SessionAuctionMessageHandler::OnMessageAuctionGiveMoneyBackRequest(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionGiveMoneyBackRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionGiveMoneyBackRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_GIVE_MONEY_BACK_REQUEST from session_server, "
           "actor_id_=[%lu], money_=[%d].",
           request.actor_id_, request.money_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", request.actor_id_);
    return;
  }
  GameActor *game_actor = auction_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.");
    return;
  }

  // 增加钻石
  if (request.money_ > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, request.money_);
  }
}

}  // namespace auction

}  // namespace server

}  // namespace game

