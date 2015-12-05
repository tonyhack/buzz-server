#include "session_server/server/extension/auction/game_auction_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_auction_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_auction_protocol_types.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/auction/auction_actor.h"
#include "session_server/server/extension/auction/auction_actor_manager.h"
#include "session_server/server/extension/auction/auction_transaction.h"
#include "session_server/server/extension/auction/auction_transaction_manager.h"

namespace session {

namespace server {

namespace auction  {

GameAuctionMessageHandler::GameAuctionMessageHandler() {}
GameAuctionMessageHandler::~GameAuctionMessageHandler() {}

bool GameAuctionMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_SELL_RESPONSE,
      boost::bind(&GameAuctionMessageHandler::OnMessageAuctionSellResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BID_RESPONSE,
      boost::bind(&GameAuctionMessageHandler::OnMessageAuctionBidResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BUYOUT_RESPONSE,
      boost::bind(&GameAuctionMessageHandler::OnMessageAuctionBuyoutResponse,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_RESPONE,
      boost::bind(&GameAuctionMessageHandler::OnMessageAuctionWithdrawItemResponse,
          this, _1, _2, _3));

  return true;
}

void GameAuctionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_SELL_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BID_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_BUYOUT_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_RESPONE);
}

static void SendMessageAuctionSellResponseFailed(AuctionActor *auction_actor) {
  gateway::protocol::MessageAuctionSellResponse response;
  response.__set_result_(false);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_RESPONSE);
  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
}

static void SendMessageAuctionBidResponseFailed(AuctionActor *auction_actor,
    gateway::protocol::AuctionErrorCode::type error) {
  gateway::protocol::MessageAuctionBidResponse response;
  response.__set_result_(error);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_BID_RESPONSE);
  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
}

static void SendMessageAuctionBuyoutResponseFailed(AuctionActor *auction_actor,
    gateway::protocol::AuctionErrorCode::type error) {
  gateway::protocol::MessageAuctionBuyoutResponse response;
  response.__set_result_(error);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_BUYOUT_RESPONSE);
  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
}

static void SendMessageAuctionWithdrawResponseFailed(AuctionActor *auction_actor) {
  gateway::protocol::MessageAuctionWithdrawResponse response;
  response.__set_result_(false);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_WITHDRAW_RESPONSE);
  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
}

static void SendMessageAuctionGiveMoneyBackRequest(AuctionActor *auction_actor, core::int32 money) {
  session::protocol::MessageAuctionGiveMoneyBackRequest game_request;
  game_request.__set_actor_id_(auction_actor->GetActorID());
  game_request.__set_money_(money);
  auction_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_AUCTION_GIVE_MONEY_BACK_REQUEST);
}

void GameAuctionMessageHandler::OnMessageAuctionSellResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionSellResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionSellResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_SELL_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], "
           "item_template_id_=[%u], item_number_=[%d], item_extra_info_=[%s], "
           "transaction_id_=[%s], start_bid_price_=[%d], buyout_price_=[%d], "
           "auction_time_id_=[%d].",
           game_response.actor_id_, game_response.result_,
           game_response.item_template_id_, game_response.item_number_,
           game_response.item_extra_info_.c_str(), game_response.transaction_id_.c_str(),
           game_response.start_bid_price_, game_response.buyout_price_,
           game_response.auction_time_id_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }

  if (false == game_response.result_) {
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  if (game_response.__isset.item_template_id_ &&
      game_response.__isset.item_number_ &&
      game_response.__isset.item_extra_info_) {
    // 检查玩家出售的道具是否已到达上限
    if (auction_actor->CheckSellItemsFull()) {
      LOG_ERROR("Sell items is full.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // 新增交易
    if (AuctionTransactionManager::GetInstance()->CreateTransaction(auction_actor,
            game_response.item_template_id_, game_response.item_number_,
            game_response.item_extra_info_, game_response.auction_time_id_,
            game_response.start_bid_price_, game_response.buyout_price_) == false) {
      LOG_ERROR("Create transaction failed.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

  } else if (game_response.__isset.transaction_id_) {
    // 检查交易状态
    if (auction_actor->CheckInSellItems(game_response.transaction_id_) == false) {
      LOG_ERROR("AuctionTransaction(%s) is not in sell items.",
                game_response.transaction_id_.c_str());
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }
    AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(game_response.transaction_id_);
    if (NULL == transaction) {
      LOG_ERROR("AuctionTransaction(%s) does not exist.",
                game_response.transaction_id_.c_str());
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    if (transaction->GetStatus() != AuctionTransaction::Status::OFF_SHELF &&
        transaction->GetStatus() != AuctionTransaction::Status::EXPIRED) {
      LOG_ERROR("AuctionTransaction(%s)'s status is not off shelf or expired.",
                game_response.transaction_id_.c_str());
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // 道具重新上架
    if (AuctionTransactionManager::GetInstance()->RenewTransaction(
            auction_actor, transaction) == false) {
      LOG_ERROR("Renew Transaction failed.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

  } else {
    LOG_ERROR("Invalid params.");
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  // 发送成功回复
  gateway::protocol::MessageAuctionSellResponse response;
  response.__set_result_(true);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_RESPONSE);
  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
}

void GameAuctionMessageHandler::OnMessageAuctionBidResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionBidResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionBidResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_BID_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], transaction_id_=[%s], bid_price_=[%d].",
           game_response.actor_id_, game_response.result_,
           game_response.transaction_id_.c_str(), game_response.bid_price_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }

  if (false == game_response.result_) {
    SendMessageAuctionBidResponseFailed(auction_actor, 
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查交易状态
  AuctionTransaction *transaction =
    AuctionTransactionManager::GetInstance()->Get(game_response.transaction_id_);
  if (NULL == transaction) {
    LOG_WARNING("Transaction does not exist.");
    SendMessageAuctionGiveMoneyBackRequest(auction_actor, game_response.bid_price_);
    SendMessageAuctionBidResponseFailed(auction_actor, 
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }

  if (transaction->GetStatus() != AuctionTransaction::Status::ON_SELL) {
    LOG_WARNING("Transaction status is not on sell.");
    SendMessageAuctionGiveMoneyBackRequest(auction_actor, game_response.bid_price_);
    SendMessageAuctionBidResponseFailed(auction_actor, 
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }

  // 检查交易竞拍价
  if (transaction->GetLastBidPrice() >= game_response.bid_price_) {
    LOG_WARNING("Transaction outbidded.");
    SendMessageAuctionGiveMoneyBackRequest(auction_actor, game_response.bid_price_);
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::OUTBIDDED);
    return;
  }

  // 竞拍交易
  if (AuctionTransactionManager::GetInstance()->BidTransaction(
          auction_actor, transaction, game_response.bid_price_) == false) {
    LOG_ERROR("Bid transaction failed.");
    // 这里不退还竞拍金, 此处数据已出错
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 发送成功回复
  gateway::protocol::MessageAuctionBidResponse response;
  response.__set_result_(gateway::protocol::AuctionErrorCode::SUCCESS);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_BID_RESPONSE);
  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
}

void GameAuctionMessageHandler::OnMessageAuctionBuyoutResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionBuyoutResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionBuyoutResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_BUYOUT_RESPONSE from game_server, "
           "actor_id_=[%lu], result_=[%d], transaction_id_=[%s], buyout_price_=[%d].",
           game_response.actor_id_, game_response.result_,
           game_response.transaction_id_.c_str(), game_response.buyout_price_);

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }
  
  if (false == game_response.result_) {
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN); 
    return;
  }

  // 检查交易状态
  AuctionTransaction *transaction =
    AuctionTransactionManager::GetInstance()->Get(game_response.transaction_id_);
  if (NULL == transaction) {
    LOG_WARNING("Transaction does not exist.");
    SendMessageAuctionGiveMoneyBackRequest(auction_actor, game_response.buyout_price_);
    SendMessageAuctionBuyoutResponseFailed(auction_actor, 
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }

  if (transaction->GetStatus() != AuctionTransaction::Status::ON_SELL) {
    LOG_WARNING("Transaction status is not on sell.");
    SendMessageAuctionGiveMoneyBackRequest(auction_actor, game_response.buyout_price_);
    SendMessageAuctionBuyoutResponseFailed(auction_actor, 
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }

  // 一口价
  if (AuctionTransactionManager::GetInstance()->BuyoutTransaction(
          auction_actor, transaction, game_response.buyout_price_) == false) {
    // 这里不退还竞拍金, 此处数据已出错
    SendMessageAuctionBuyoutResponseFailed(auction_actor, 
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }

  // 发送成功回复
  gateway::protocol::MessageAuctionBuyoutResponse response;
  response.__set_result_(gateway::protocol::AuctionErrorCode::SUCCESS);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_BUYOUT_RESPONSE);
  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
}

void GameAuctionMessageHandler::OnMessageAuctionWithdrawItemResponse(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAuctionWithdrawItemResponse game_response;
  if (global::GlobalPacket::GetPacket()->Deserialize(game_response, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionWithdrawItemResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_WITHDRAW_ITEM_RESPONE from game_server, "
           "actor_id_=[%lu], result_=[%d], transaction_id_=[%s].",
           game_response.actor_id_, game_response.result_,
           game_response.transaction_id_.c_str());

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(game_response.actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", game_response.actor_id_);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", game_response.actor_id_);
    return;
  }

  if (false == game_response.result_) {
    SendMessageAuctionWithdrawResponseFailed(auction_actor);
    return;
  }

  AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(game_response.transaction_id_);
  if (NULL == transaction) {
    LOG_WARNING("Transaction does not exist.");
    SendMessageAuctionWithdrawResponseFailed(auction_actor);
    return;
  }

  core::uint32 item_template_id = transaction->GetItemTemplateID();
  core::int32 item_number = transaction->GetItemNumber();
  std::string item_extra_info = transaction->GetItemExtraInfo();

  // 提取道具
  if (auction_actor->CheckInSellItems(game_response.transaction_id_)) {
    // 检查是否被竞价
    if (transaction->GetBuyer() != 0) {
      LOG_WARNING("Transaction is already been bidded.");
      SendMessageAuctionWithdrawResponseFailed(auction_actor);
      return;
    }
    AuctionTransactionManager::GetInstance()->OwnerWithdrawItem(auction_actor, transaction);
  } else if (auction_actor->CheckInBuyItems(game_response.transaction_id_)) {
    // 检查是否已领取
    if (transaction->CheckItemWithdrawn()) {
      LOG_ERROR("item has been withdrawn.");
      SendMessageAuctionWithdrawResponseFailed(auction_actor);
      return;
    }
    AuctionTransactionManager::GetInstance()->BuyerWithdrawItem(auction_actor, transaction);
  } else {
    LOG_WARNING("Transaction is not in actor's sell items or buy items.");
    SendMessageAuctionWithdrawResponseFailed(auction_actor);
    return;
  }

  // 解锁
  auction_actor->GetLock()->Unlock(AuctionActor::kAuctionLock);
  // 转发 game_server 发放道具
  session::protocol::MessageAuctionWithdrawItemRequest2 game_request;
  game_request.__set_actor_id_(auction_actor->GetActorID());
  game_request.__set_item_template_id_(item_template_id);
  game_request.__set_item_number_(item_number);
  game_request.__set_item_extra_info_(item_extra_info);
  auction_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST2);
}

}  // namespace auction

}  // namespace server

}  // namespace session

