#include "session_server/server/extension/auction/auction_message_handler.h"

#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_auction_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_auction_protocol_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/auction/auction_actor.h"
#include "session_server/server/extension/auction/auction_actor_manager.h"
#include "session_server/server/extension/auction/auction_transaction.h"
#include "session_server/server/extension/auction/auction_transaction_manager.h"

namespace session {

namespace server {

namespace auction {

AuctionMessageHandler::AuctionMessageHandler() {}
AuctionMessageHandler::~AuctionMessageHandler() {}

bool AuctionMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_QUERY_TRANSACTION_REQUEST,
      boost::bind(&AuctionMessageHandler::OnMessageAuctionQueryTransactionRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_SEARCH_REQUEST,
      boost::bind(&AuctionMessageHandler::OnMessageAuctionSearchRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_REQUEST,
      boost::bind(&AuctionMessageHandler::OnMessageAuctionSellRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_CANCEL_SELL_REQUEST,
      boost::bind(&AuctionMessageHandler::OnMessageAuctionCancelSellRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_BID_REQUEST,
      boost::bind(&AuctionMessageHandler::OnMessageAuctionBidRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_BUYOUT_REQUEST,
      boost::bind(&AuctionMessageHandler::OnMessageAuctionBuyoutRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_WITHDRAW_REQUEST,
      boost::bind(&AuctionMessageHandler::OnMessageAuctionWithdrawRequest,
          this, _1, _2, _3));

  return true;
}

void AuctionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_QUERY_TRANSACTION_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_SEARCH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_CANCEL_SELL_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_BID_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_BUYOUT_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_AUCTION_WITHDRAW_REQUEST);
}

static void SendMessageAuctionQueryTransactionResponseFailed(AuctionActor *auction_actor) {
  gateway::protocol::MessageAuctionQueryTransactionResponse response;
  response.__set_result_(false);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_QUERY_TRANSACTION_RESPONSE);
}

static void SendMessageAuctionSearchResponseFailed(AuctionActor *auction_actor) {
  gateway::protocol::MessageAuctionSearchResponse response;
  response.__set_result_(false);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_SEARCH_RESPONSE);
}

static void SendMessageAuctionSellResponseFailed(AuctionActor *auction_actor) {
  gateway::protocol::MessageAuctionSellResponse response;
  response.__set_result_(false);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_RESPONSE);
}

static void SendMessageAuctionCancelSellResponseFailed(AuctionActor *auction_actor) {
  gateway::protocol::MessageAuctionCancelSellResponse response;
  response.__set_result_(false);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_CANCEL_SELL_RESPONSE);
}

static void SendMessageAuctionBidResponseFailed(AuctionActor *auction_actor,
    gateway::protocol::AuctionErrorCode::type error) {
  gateway::protocol::MessageAuctionBidResponse response;
  response.__set_result_(error);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_BID_RESPONSE);
}

static void SendMessageAuctionBuyoutResponseFailed(AuctionActor *auction_actor,
    gateway::protocol::AuctionErrorCode::type error) {
  gateway::protocol::MessageAuctionBuyoutResponse response;
  response.__set_result_(error);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_BUYOUT_RESPONSE);
}

static void SendMessageAuctionWithdrawResponseFailed(AuctionActor *auction_actor) {
  gateway::protocol::MessageAuctionWithdrawResponse response;
  response.__set_result_(false);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_WITHDRAW_RESPONSE);
}

static void SendMessageAuctionWithdrawMoneyRequest(AuctionActor *auction_actor,
    core::int32 money) {
  session::protocol::MessageAuctionWithdrawMoneyRequest game_request;
  game_request.__set_actor_id_(auction_actor->GetActorID());
  game_request.__set_money_(money);
  auction_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_MONEY_REQUEST);
}

static void SendMessageAuctionWithdrawItemRequest(AuctionActor *auction_actor,
    const std::string &transaction_id,
    core::uint32 item_template_id, core::int32 item_number) {
  // 加锁
  auction_actor->GetLock()->Lock(AuctionActor::kAuctionLock);

  // 转发 game_server
  session::protocol::MessageAuctionWithdrawItemRequest game_request;
  game_request.__set_actor_id_(auction_actor->GetActorID());
  game_request.__set_transaction_id_(transaction_id);
  game_request.__set_item_template_id_(item_template_id);
  game_request.__set_item_number_(item_number);
  auction_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST);
}

void AuctionMessageHandler::OnMessageAuctionQueryTransactionRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(id);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAuctionQueryTransactionRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionQueryTransactionRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_QUERY_TRANSACTION_REQUEST from actor(%lu, %s), "
           "transaction_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.transaction_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    LOG_WARNING("AUCTION_MODULE is disable.");
    SendMessageAuctionQueryTransactionResponseFailed(auction_actor);
    return;
  }

  // 检查交易管理器是否完成索引
  if (AuctionTransactionManager::GetInstance()->CheckIndexBuilt() == false) {
    LOG_ERROR("AuctionTransactionManager index is not built.");
    SendMessageAuctionQueryTransactionResponseFailed(auction_actor);
    return;
  }

  AuctionTransaction *transaction =
    AuctionTransactionManager::GetInstance()->Get(request.transaction_id_);
  if (NULL == transaction) {
    LOG_WARNING("AuctionTransaction(%s) does not exist.", request.transaction_id_.c_str());
    SendMessageAuctionQueryTransactionResponseFailed(auction_actor);
    return;
  }

  if (transaction->GetStatus() != AuctionTransaction::Status::ON_SELL) {
    LOG_WARNING("AuctionTransaction(%s) is not on sell.", request.transaction_id_.c_str());
    SendMessageAuctionQueryTransactionResponseFailed(auction_actor);
    return;
  }

  gateway::protocol::MessageAuctionQueryTransactionResponse response;
  response.__set_result_(true);
  response.__isset.data_ = true;
  transaction->ExportAuctionItemData(response.data_);
  response.data_.__set_status_(gateway::protocol::AuctionSellItemStatusType::ON_SELL);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_QUERY_TRANSACTION_RESPONSE);
}

void AuctionMessageHandler::OnMessageAuctionSearchRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(id);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAuctionSearchRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionSearchRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_SEARCH_REQUEST from actor(%lu, %s), "
           "sort_type_=[%d], page_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.sort_type_, request.page_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    LOG_WARNING("AUCTION_MODULE is disable.");
    SendMessageAuctionSearchResponseFailed(auction_actor);
    return;
  }

  // 检查交易管理器是否完成索引
  if (AuctionTransactionManager::GetInstance()->CheckIndexBuilt() == false) {
    LOG_ERROR("AuctionTransactionManager index is not built.");
    SendMessageAuctionSearchResponseFailed(auction_actor);
    return;
  }

  // 检查参数
  if (request.sort_type_ < entity::AuctionSearchSortType::MIN ||
      request.sort_type_ >= entity::AuctionSearchSortType::MAX) {
    LOG_ERROR("sort_type is invalid.");
    SendMessageAuctionSearchResponseFailed(auction_actor);
    return;
  }

  if (request.page_ <= 0) {
    LOG_ERROR("page is invalid.");
    SendMessageAuctionSearchResponseFailed(auction_actor);
    return;
  }

  std::vector<std::string> search_result;
  core::int32 total_page  = 1;

  if (request.__isset.item_template_ids_) {
    // 按道具ID搜索
    if (request.item_template_ids_.empty()) {
      LOG_WARNING("item_template_ids is empty.");
      SendMessageAuctionSearchResponseFailed(auction_actor);
      return;
    }
    if (request.item_template_ids_.size() > 50) {
      LOG_WARNING("item_template_ids is too large.");
      SendMessageAuctionSearchResponseFailed(auction_actor);
      return;
    }

    AuctionTransactionManager::GetInstance()->SearchTransaction(
      (core::uint32 *)&request.item_template_ids_[0], request.item_template_ids_.size(),
      request.sort_type_, request.page_, search_result, total_page);

  } else if (request.__isset.item_type_) {
    // 按道具类型搜索
    AuctionTransactionManager::GetInstance()->SearchTransaction(request.item_type_,
      request.sort_type_, request.page_, search_result, total_page);

  } else {
    LOG_ERROR("Invalid params.");
    SendMessageAuctionSearchResponseFailed(auction_actor);
    return;
  }

  gateway::protocol::MessageAuctionSearchResponse response;
  response.__set_result_(true);
  response.__isset.items_ = true;
  response.__set_page_(request.page_);
  response.__set_total_page_(total_page);

  for (size_t i = 0; i < search_result.size(); ++i) {
    AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(search_result[i]);
    if (NULL == transaction) {
      LOG_ERROR("AuctionTransaction(%s) does not exist.", search_result[i].c_str());
      continue;
    }
    gateway::protocol::AuctionItemData data;
    transaction->ExportAuctionItemData(data);
    data.__set_status_(gateway::protocol::AuctionSellItemStatusType::ON_SELL);

    response.items_.push_back(data);
  }
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_SEARCH_RESPONSE);
}

void AuctionMessageHandler::OnMessageAuctionSellRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(id);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAuctionSellRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionSellRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_SELL_REQUEST from actor(%lu, %s), "
           "item_id_=[%s], "
           "item_template_id_=[%d], item_number_=[%d], "
           "transaction_id_=[%s], "
           "start_bid_price_=[%d], buyout_price_=[%d], auction_time_id_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.item_id_.c_str(),
           request.item_template_id_, request.item_number_,
           request.transaction_id_.c_str(),
           request.start_bid_price_, request.buyout_price_, request.auction_time_id_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    LOG_WARNING("AUCTION_MODULE is disable.");
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  // 锁检查
  if (auction_actor->GetLock()->CheckLocked(AuctionActor::kAuctionLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  // 检查交易管理器是否完成索引
  if (AuctionTransactionManager::GetInstance()->CheckIndexBuilt() == false) {
    LOG_ERROR("AuctionTransactionManager index is not built.");
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  // 检查玩家出售的道具是否已到达上限
  if (auction_actor->CheckSellItemsFull()) {
    LOG_WARNING("Sell items is full.");
    SendMessageAuctionSellResponseFailed(auction_actor);
    return;
  }

  session::protocol::MessageAuctionSellRequest game_request;
  game_request.__set_actor_id_(id);

  if (request.__isset.transaction_id_) {
    // 下架道具再次上架
    // --检查交易ID
    if (auction_actor->CheckInSellItems(request.transaction_id_) == false) {
      LOG_WARNING("transaction_id is invalid.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }
    AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(request.transaction_id_);
    if (NULL == transaction) {
      LOG_WARNING("transaction_id is invalid.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // --检查交易状态
    if (transaction->GetStatus() != AuctionTransaction::Status::OFF_SHELF &&
        transaction->GetStatus() != AuctionTransaction::Status::EXPIRED) {
      LOG_WARNING("Transaction status is not off shelf or expired.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // --填写请求参数
    game_request.__set_transaction_id_(request.transaction_id_);
    game_request.__set_start_bid_price_(transaction->GetStartBidPrice());
    game_request.__set_buyout_price_(transaction->GetBuyoutPrice());
    game_request.__set_auction_time_id_(transaction->GetAuctionTimeID());

  } else {
    // 新出售的道具

    // --检查参数
    if (request.__isset.start_bid_price_ == false ||
        request.__isset.buyout_price_ == false ||
        request.__isset.auction_time_id_ == false) {
      LOG_WARNING("Sell new item required params not found.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    if (request.start_bid_price_ != 0 &&
        request.start_bid_price_ < MISC_CONF()->auction_min_start_bid_price_) {
      LOG_WARNING("start_bid_price is invalid.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }
    if (request.buyout_price_ != 0 &&
        (request.buyout_price_ < request.start_bid_price_ ||
         request.buyout_price_ < MISC_CONF()->auction_min_buyout_price_)) {
      LOG_WARNING("buyout_price is invalid.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }
    if (request.start_bid_price_ <= 0 && request.buyout_price_ <= 0) {
      LOG_WARNING("start_bid_price and buyout_price both invalid.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }
    const global::configure::AuctionTimeCell *cell =
      AUCTION_CONF()->GetAuctionTime(request.auction_time_id_);
    if (NULL == cell) {
      LOG_WARNING("action_time_id is invalid.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }

    // --填写请求参数
    if (request.__isset.item_id_) {
      // 出售装备类道具
      game_request.__set_item_id_(atol(request.item_id_.c_str()));

    } else if (request.__isset.item_template_id_ && request.__isset.item_number_) {
      // 出售其他道具
      if (request.item_number_ <= 0) {
        LOG_WARNING("item_number is invalid.");
        SendMessageAuctionSellResponseFailed(auction_actor);
        return;
      }
      game_request.__set_item_template_id_(request.item_template_id_);
      game_request.__set_item_number_(request.item_number_);

    } else {
      LOG_WARNING("Sell new item required params not found.");
      SendMessageAuctionSellResponseFailed(auction_actor);
      return;
    }
    game_request.__set_start_bid_price_(request.start_bid_price_);
    game_request.__set_buyout_price_(request.buyout_price_);
    game_request.__set_auction_time_id_(request.auction_time_id_);
  }

  // 加锁
  auction_actor->GetLock()->Lock(AuctionActor::kAuctionLock);

  // 转发 game_server
  auction_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_AUCTION_SELL_REQUEST);
}

void AuctionMessageHandler::OnMessageAuctionCancelSellRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(id);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAuctionCancelSellRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionCancelSellRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_CANCEL_SELL_REQUEST from actor(%lu, %s), "
           "transaction_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.transaction_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    LOG_WARNING("AUCTION_MODULE is disable.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }

  // 锁检查
  if (auction_actor->GetLock()->CheckLocked(AuctionActor::kAuctionLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }

  // 检查交易管理器是否完成索引
  if (AuctionTransactionManager::GetInstance()->CheckIndexBuilt() == false) {
    LOG_ERROR("AuctionTransactionManager index is not built.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }

  // 检查交易ID
  if (auction_actor->CheckInSellItems(request.transaction_id_) == false) {
    LOG_WARNING("transaction_id is invalid.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }
  AuctionTransaction *transaction =
    AuctionTransactionManager::GetInstance()->Get(request.transaction_id_);
  if (NULL == transaction) {
    LOG_WARNING("transaction_id is invalid.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }

  // 检查交易状态
  if (transaction->GetStatus() != AuctionTransaction::Status::ON_SELL) {
    LOG_WARNING("Transaction status is not on sell.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }
  if (transaction->GetOwner() != auction_actor->GetActorID()) {
    LOG_ERROR("Actor do not own the transaction.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }
  if (transaction->GetBuyer() != 0) {
    LOG_WARNING("Transaction is already been bidded.");
    SendMessageAuctionCancelSellResponseFailed(auction_actor);
    return;
  }

  // 交易下架
  AuctionTransactionManager::GetInstance()->CancelTransaction(
      auction_actor, transaction);

  gateway::protocol::MessageAuctionCancelSellResponse response;
  response.__set_result_(true);
  auction_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_AUCTION_CANCEL_SELL_RESPONSE);
}

void AuctionMessageHandler::OnMessageAuctionBidRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(id);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAuctionBidRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionBidRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_BID_REQUEST from actor(%lu, %s), "
           "transaction_id_=[%s], bid_price_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.transaction_id_.c_str(), request.bid_price_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    LOG_WARNING("AUCTION_MODULE is disable.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 锁检查
  if (auction_actor->GetLock()->CheckLocked(AuctionActor::kAuctionLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查交易管理器是否完成索引
  if (AuctionTransactionManager::GetInstance()->CheckIndexBuilt() == false) {
    LOG_ERROR("AuctionTransactionManager index is not built.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查玩家购买的道具是否已达到上限
  if (auction_actor->CheckBuyItemsFull()) {
    LOG_WARNING("Buy items is full.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查交易ID
  if (auction_actor->CheckInSellItems(request.transaction_id_) ||
      auction_actor->CheckInBuyItems(request.transaction_id_)) {
    LOG_WARNING("Transaction is already in actor's sell items or buy items.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(request.transaction_id_);
  if (NULL == transaction) {
    LOG_WARNING("transaction_id is invalid.", request.transaction_id_.c_str());
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }

  // 检查交易状态
  if (transaction->GetStatus() != AuctionTransaction::Status::ON_SELL) {
    LOG_WARNING("Transaction status is not on sell.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }
  if (transaction->GetOwner() == auction_actor->GetActorID()) {
    LOG_ERROR("Can not bid own transaction.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查交易竞拍价
  if (transaction->GetStartBidPrice() == 0) {
    LOG_WARNING("Transaction can not bid, start bid price is zero.");
    SendMessageAuctionBidResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }
  if (transaction->GetLastBidPrice() == 0) {
    if (request.bid_price_ < transaction->GetStartBidPrice()) {
      LOG_WARNING("bid_price is lower than start bid price.");
      SendMessageAuctionBidResponseFailed(auction_actor,
          gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
      return;
    }
  } else {
    if (request.bid_price_ < transaction->GetLastBidPrice() +
                             transaction->GetMinBidIncrementPrice()) {
      LOG_WARNING("bid_price is lower than min next bid price.");
      SendMessageAuctionBidResponseFailed(auction_actor,
          gateway::protocol::AuctionErrorCode::OUTBIDDED);
      return;
    }
    if (transaction->GetBuyoutPrice() != 0 &&
        request.bid_price_ >= transaction->GetBuyoutPrice()) {
      LOG_WARNING("bid_price is greater than buyout price.");
      SendMessageAuctionBidResponseFailed(auction_actor,
          gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
      return;
    }
  }

  // 加锁
  auction_actor->GetLock()->Lock(AuctionActor::kAuctionLock);

  // 转发 game_server
  session::protocol::MessageAuctionBidRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_transaction_id_(request.transaction_id_);
  game_request.__set_bid_price_(request.bid_price_);
  auction_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_AUCTION_BID_REQUEST);
}

void AuctionMessageHandler::OnMessageAuctionBuyoutRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(id);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAuctionBuyoutRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionBuyoutRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_BUYOUT_REQUEST from actor(%lu, %s), "
           "transaction_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.transaction_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    LOG_WARNING("AUCTION_MODULE is disable.");
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 锁检查
  if (auction_actor->GetLock()->CheckLocked(AuctionActor::kAuctionLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查交易管理器是否完成索引
  if (AuctionTransactionManager::GetInstance()->CheckIndexBuilt() == false) {
    LOG_ERROR("AuctionTransactionManager index is not built.");
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查交易ID
  AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(request.transaction_id_);
  if (NULL == transaction) {
    LOG_WARNING("transaction_id is invalid.", request.transaction_id_.c_str());
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }

  // 检查交易状态
  if (transaction->GetStatus() != AuctionTransaction::Status::ON_SELL) {
    LOG_WARNING("Transaction status is not on sell.");
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ITEM_NOT_EXIST);
    return;
  }
  if (transaction->GetOwner() == auction_actor->GetActorID()) {
    LOG_ERROR("Can not bid own transaction.");
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查是否可以一口价
  if (transaction->GetBuyoutPrice() == 0) {
    LOG_WARNING("Transaction can not buyout, buyout price is zero.");
    SendMessageAuctionBuyoutResponseFailed(auction_actor,
        gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
    return;
  }

  core::int32 give_back_money = 0;

  if (transaction->GetBuyer() == auction_actor->GetActorID()) {
    // 玩家是当前交易的最高出价, 然后一口价购买道具
    give_back_money = transaction->GetLastBidPrice();

  } else {
    // 正常情况
    // --检查玩家购买的道具是否已达到上限
    if (auction_actor->CheckBuyItemsFull()) {
      LOG_WARNING("Buy items is full.");
      SendMessageAuctionBuyoutResponseFailed(auction_actor,
          gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
      return;
    }
    // --检查道具是否已在购买和出售列表
    if (auction_actor->CheckInSellItems(request.transaction_id_) ||
        auction_actor->CheckInBuyItems(request.transaction_id_)) {
      LOG_WARNING("Transaction is already in actor's sell items or buy items.");
      SendMessageAuctionBuyoutResponseFailed(auction_actor,
          gateway::protocol::AuctionErrorCode::ERROR_UNKNOWN);
      return;
    }
  }

  // 加锁
  auction_actor->GetLock()->Lock(AuctionActor::kAuctionLock);

  // 转发 game_server
  session::protocol::MessageAuctionBuyoutRequest game_request;
  game_request.__set_actor_id_(id);
  game_request.__set_transaction_id_(request.transaction_id_);
  game_request.__set_buyout_price_(transaction->GetBuyoutPrice());
  game_request.__set_give_back_money_(give_back_money);
  auction_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_AUCTION_BUYOUT_REQUEST);
}

void AuctionMessageHandler::OnMessageAuctionWithdrawRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取拍卖玩家对象
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(id);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = auction_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageAuctionWithdrawRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAuctionWithdrawRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_AUCTION_WITHDRAW_REQUEST from actor(%lu, %s), "
           "transaction_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.transaction_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    LOG_WARNING("AUCTION_MODULE is disable.");
    SendMessageAuctionWithdrawResponseFailed(auction_actor);
    return;
  }

  // 锁检查
  if (auction_actor->GetLock()->CheckLocked(AuctionActor::kAuctionLock)) {
    LOG_WARNING("Lock check failed.");
    SendMessageAuctionWithdrawResponseFailed(auction_actor);
    return;
  }

  // 检查交易管理器是否完成索引
  if (AuctionTransactionManager::GetInstance()->CheckIndexBuilt() == false) {
    LOG_ERROR("AuctionTransactionManager index is not built.");
    SendMessageAuctionWithdrawResponseFailed(auction_actor);
    return;
  }

  if (auction_actor->CheckInSellItems(request.transaction_id_)) {

    // 处理出售列表

    AuctionTransaction *transaction =
        AuctionTransactionManager::GetInstance()->Get(request.transaction_id_);
    if (NULL == transaction) {
      LOG_WARNING("transaction_id is invalid.", request.transaction_id_.c_str());
      SendMessageAuctionWithdrawResponseFailed(auction_actor);
      return;
    }

    AuctionTransaction::Status::type transaction_status = transaction->GetStatus();
    if (AuctionTransaction::Status::SOLD == transaction_status) {
      // 检查是否已领取
      if (transaction->CheckMoneyWithdrawn()) {
        LOG_ERROR("money has been withdrawn.");
        SendMessageAuctionWithdrawResponseFailed(auction_actor);
        return;
      }

      // 扣除手续费
      core::int32 fee = std::max(1,
          transaction->GetLastBidPrice() * MISC_CONF()->auction_transaction_fee_percent_ / 100);
      // 收入可能为负数
      core::int32 money = transaction->GetLastBidPrice() - fee;
      // 提取金额
      AuctionTransactionManager::GetInstance()->OwnerWithdrawMoney(
          auction_actor, transaction);
      // 转发 game_server
      SendMessageAuctionWithdrawMoneyRequest(auction_actor, money);

    } else if (AuctionTransaction::Status::OFF_SHELF == transaction_status ||
               AuctionTransaction::Status::EXPIRED == transaction_status) {
      // 转发 game_server 进行道具检查
      SendMessageAuctionWithdrawItemRequest(auction_actor, request.transaction_id_,
          transaction->GetItemTemplateID(), transaction->GetItemNumber());

    } else {
      LOG_ERROR("Can not withdraw due to transaction's status.");
      SendMessageAuctionWithdrawResponseFailed(auction_actor);
      return;
    }

  } else if (auction_actor->CheckInBuyItems(request.transaction_id_)) {

    // 处理购买列表

    // 取购买信息
    const AuctionActor::BuyItemInfo *buy_item_info =
      auction_actor->GetBuyItemInfo(request.transaction_id_); 
    if (NULL == buy_item_info) {
      LOG_ERROR("Get buy item info failed.");
      SendMessageAuctionWithdrawResponseFailed(auction_actor);
      return;
    }
    core::int32 bid_price = buy_item_info->buy_price_;

    AuctionTransaction *transaction =
        AuctionTransactionManager::GetInstance()->Get(request.transaction_id_);
    if (transaction != NULL) {
      AuctionTransaction::Status::type transaction_status = transaction->GetStatus();
      if (AuctionTransaction::Status::SOLD == transaction_status &&
          transaction->GetBuyer() == auction_actor->GetActorID()) {
        // 转发 game_server 进行道具检查
        SendMessageAuctionWithdrawItemRequest(auction_actor, request.transaction_id_,
            transaction->GetItemTemplateID(), transaction->GetItemNumber());

      } else if ((AuctionTransaction::Status::ON_SELL == transaction_status || 
                  AuctionTransaction::Status::SOLD == transaction_status) &&
                  transaction->GetBuyer() != auction_actor->GetActorID()) {
        // 提取金额
        AuctionTransactionManager::GetInstance()->BuyerWithdrawMoney(
            auction_actor, request.transaction_id_);
        // 转发game_server
        SendMessageAuctionWithdrawMoneyRequest(auction_actor, bid_price);
      } else {
        LOG_ERROR("Can not withdraw due to transaction's status.");
        SendMessageAuctionWithdrawResponseFailed(auction_actor);
        return;
      }

    } else {
      // 提取金额
      AuctionTransactionManager::GetInstance()->BuyerWithdrawMoney(
          auction_actor, request.transaction_id_);
      // 转发game_server
      SendMessageAuctionWithdrawMoneyRequest(auction_actor, bid_price);
    }

  } else {
    LOG_WARNING("Transaction is not in actor's sell items or buy items.");
    SendMessageAuctionWithdrawResponseFailed(auction_actor);
    return;
  }
}

}  // namespace auction

}  // namespace server

}  // namespace session

