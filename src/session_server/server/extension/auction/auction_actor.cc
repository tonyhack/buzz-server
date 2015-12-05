#include "session_server/server/extension/auction/auction_actor.h"

#include "coresh/communicator.h"
#include "entity/functionality_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_auction_actor_login_types.h"
#include "database_server/protocol/storage_auction_actor_save_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_auction_protocol_types.h"
#include "session_server/server/extension_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/auction/auction_transaction.h"
#include "session_server/server/extension/auction/auction_transaction_manager.h"

namespace session {

namespace server {

namespace auction {

bool AuctionActor::BuyItemInfo::Initialize(const std::string &data) {
  std::vector<std::string> split_result;
  global::StringSplit(data.c_str(), ":", split_result);

  if (split_result.size() < 5) {
    return false;
  }

  this->transaction_id_ = split_result[0].c_str();
  this->item_template_id_ = atoi(split_result[1].c_str());
  this->item_number_ = atoi(split_result[2].c_str());
  this->owner_ = atol(split_result[3].c_str());
  this->buy_price_ = atoi(split_result[4].c_str());

  return true;
}

int AuctionActor::BuyItemInfo::Serialize(char *buff, size_t size) const {
  return snprintf(buff, size, "%s:%u:%d:%lu:%d,",
      this->transaction_id_.c_str(), this->item_template_id_, this->item_number_,
      this->owner_, this->buy_price_);
}

///////////////////////////////////////////////////////////////////////////////

static void DecodeTransactionIDSet(const std::string &source,
    AuctionActor::TransactionIDSet *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }
    const std::string &transaction_id = split_result[i];
    if (NULL == AuctionTransactionManager::GetInstance()->Get(transaction_id)) {
      LOG_ERROR("transaction_id(%s) does not exist.", transaction_id.c_str());
      continue;
    }
    result->insert(transaction_id);
  }
}

static void EncodeTransactionIDSet(const AuctionActor::TransactionIDSet &source,
    std::string *result) {
  char buffer[2048] = "";
  size_t count = 0;

  AuctionActor::TransactionIDSet::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%s,", iter->c_str());
  }

  *result = buffer;
}

static void DecodeBuyItemInfoMap(const std::string &source,
    AuctionActor::BuyItemInfoMap *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    AuctionActor::BuyItemInfo data;
    if (data.Initialize(split_result[i]) == false) {
      LOG_ERROR("BuyItemInfo(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->insert(std::make_pair(data.transaction_id_, data));
  }
}

static void EncodeBuyItemInfoMap(const AuctionActor::BuyItemInfoMap &source,
    std::string *result) {
  char buffer[8192] = "";
  size_t count = 0;

  AuctionActor::BuyItemInfoMap::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    const AuctionActor::BuyItemInfo &data = iter->second;
    count += data.Serialize(buffer + count, sizeof(buffer) - count);
  }

  *result = buffer;
}

static void DecodeAuctionHistory(const std::string &source,
    AuctionActor::AuctionHistoryDeque *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    AuctionHistory history;
    if (history.Initialize(split_result[i]) == false) {
      LOG_ERROR("AuctionHistory(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->push_back(history);
  }
}

static void EncodeAuctionHistory(const AuctionActor::AuctionHistoryDeque &source,
    std::string *result) {
  char buffer[4096] = "";
  size_t count = 0;

  AuctionActor::AuctionHistoryDeque::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    const AuctionHistory &history = *iter;

    count += snprintf(buffer + count, sizeof(buffer) - count, "%d:%s",
        history.GetType(), history.GetTimestamp().c_str());

    for (size_t i = 0; i < history.GetHistoryVars().size(); ++i) {
      count += snprintf(buffer + count, sizeof(buffer) - count, ":%s",
          history.GetHistoryVars()[i].c_str());
    }
    count += snprintf(buffer + count, sizeof(buffer) - count, ",");
  }

  *result = buffer;
}

///////////////////////////////////////////////////////////////////////////////

AuctionActor::AuctionActor() : brief_actor_(NULL), session_actor_(NULL) {}
AuctionActor::~AuctionActor() {}

bool AuctionActor::Initialize(BriefActor *brief_actor,
    const database::protocol::StorageAuctionActorField &db_field) {
  if (NULL == brief_actor) {
    LOG_ERROR("brief_actor is null.");
    return false;
  }

  this->brief_actor_ = brief_actor;

  DecodeTransactionIDSet(db_field.sell_items_, &this->sell_items_);
  DecodeBuyItemInfoMap(db_field.buy_items_, &this->buy_items_);
  DecodeAuctionHistory(db_field.history_, &this->history_list_);

  return true;
}

void AuctionActor::Finalize() {
  this->history_list_.clear();
  this->buy_items_.clear();
  this->sell_items_.clear();

  this->session_actor_ = NULL;
  this->brief_actor_ = NULL;
}

static gateway::protocol::AuctionSellItemStatusType::type GetSellItemStatus(
    const AuctionTransaction *transaction) {
  AuctionTransaction::Status::type transaction_status = transaction->GetStatus();

  if (AuctionTransaction::Status::OFF_SHELF == transaction_status ||
      AuctionTransaction::Status::EXPIRED == transaction_status) {
    return gateway::protocol::AuctionSellItemStatusType::OFF_SHELF;
  } else if (AuctionTransaction::Status::ON_SELL == transaction_status) {
    return gateway::protocol::AuctionSellItemStatusType::ON_SELL;
  } else if (AuctionTransaction::Status::SOLD == transaction_status) {
    return gateway::protocol::AuctionSellItemStatusType::SOLD;
  } else {
    LOG_ERROR("Invalid auction transaction status.");
    return (gateway::protocol::AuctionSellItemStatusType::type)-1;
  }
}

static gateway::protocol::AuctionBuyItemStatusType::type GetBuyItemStatus(
    const AuctionActor *auction_actor, const AuctionTransaction *transaction) {
  AuctionTransaction::Status::type transaction_status = transaction->GetStatus();

  if (AuctionTransaction::Status::ON_SELL == transaction_status) {
    if (transaction->GetBuyer() == auction_actor->GetActorID()) {
      return gateway::protocol::AuctionBuyItemStatusType::HIGHEST_BID;
    } else {
      return gateway::protocol::AuctionBuyItemStatusType::OUTBIDDED;
    }
  } else if (AuctionTransaction::Status::SOLD == transaction_status) {
    if (transaction->GetBuyer() == auction_actor->GetActorID()) {
      return gateway::protocol::AuctionBuyItemStatusType::WON;
    } else {
      return gateway::protocol::AuctionBuyItemStatusType::SOLD;
    }
  } else {
    LOG_ERROR("Invalid auction transaction status.");
    return (gateway::protocol::AuctionBuyItemStatusType::type)-1;
  }
}

void AuctionActor::SendInitToClient() {
  gateway::protocol::MessageAuctionInitialize message;

  // 玩家出售道具
  for (TransactionIDSet::const_iterator iter = this->sell_items_.begin();
       iter != this->sell_items_.end(); ++iter) {
    AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(*iter);
    if (NULL == transaction) {
      LOG_ERROR("AuctionTransaction(%s) does not exist.", iter->c_str());
      return;
    }

    gateway::protocol::AuctionItemData data;
    transaction->ExportAuctionItemData(data);
    data.__set_status_(GetSellItemStatus(transaction));

    message.sell_items_.push_back(data);
  }

  // 玩家购买道具
  for (BuyItemInfoMap::const_iterator iter = this->buy_items_.begin();
       iter != this->buy_items_.end(); ++iter) {
    const BuyItemInfo &buy_item = iter->second;

    AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(iter->first);
    if (transaction != NULL) {
      gateway::protocol::AuctionItemData data;
      transaction->ExportAuctionItemData(data);
      data.__set_my_bid_price_(buy_item.buy_price_);
      data.__set_status_(GetBuyItemStatus(this, transaction));

      message.buy_items_.push_back(data);

    } else {

      gateway::protocol::AuctionItemData data;
      data.__set_transaction_id_(buy_item.transaction_id_);
      data.__set_item_template_id_(buy_item.item_template_id_);
      data.__set_item_number_(buy_item.item_number_);
      data.__set_left_time_(0);
      data.__set_my_bid_price_(buy_item.buy_price_);
      data.__set_status_(gateway::protocol::AuctionBuyItemStatusType::SOLD);

      BriefActor *brief_actor =
        SessionActorManager::GetInstance()->GetBriefActor(buy_item.owner_);
      if (brief_actor != NULL) {
        data.__set_owner_name_(brief_actor->GetName());
      }

      message.buy_items_.push_back(data);
    }
  }

  // 交易历史记录
  for (AuctionHistoryDeque::const_iterator iter = this->history_list_.begin();
       iter != this->history_list_.end(); ++iter) {
    const AuctionHistory &history = *iter;
    gateway::protocol::AuctionHistoryData data;

    data.__set_type_((entity::AuctionHistoryType::type)history.GetType());
    data.__set_timestamp_(history.GetTimestamp());
    history.TranslateHistoryVar(&data.vars_);

    message.history_.push_back(data);
  }

  this->session_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_AUCTION_INITIALIZE);
}

void AuctionActor::Online(SessionActor *session_actor) {
  if (NULL == session_actor) {
    LOG_ERROR("session_actor is null.");
    return;
  }

  this->session_actor_ = session_actor;

  // 未开启功能返回
  if (this->session_actor_->CheckFunctionalityState(
          entity::FunctionalityType::AUCTION_MODULE) == false) {
    return;
  }

  // 关注购买列表中的交易
  for (BuyItemInfoMap::const_iterator iter = this->buy_items_.begin();
       iter != this->buy_items_.end(); ++iter) {
    AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(iter->first);
    if (transaction != NULL) {
      transaction->AddFollower(this->GetActorID());
    }
  }

  // 发送初始化信息
  this->SendInitToClient();
}

void AuctionActor::Offline() {
  this->session_actor_ = NULL;

  // 取消关注购买列表中的交易
  for (BuyItemInfoMap::const_iterator iter = this->buy_items_.begin();
       iter != this->buy_items_.end(); ++iter) {
    AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(iter->first);
    if (transaction != NULL) {
      transaction->RemoveFollower(this->GetActorID());
    }
  }
}

void AuctionActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetActorID());
  load_finish.__set_extension_type_(ExtensionType::AUCTION);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish, sizeof(load_finish));
}

void AuctionActor::Save() {
  database::protocol::StorageAuctionActorSaveRequest request;
  request.__set_actor_id_(this->GetActorID());
  EncodeTransactionIDSet(this->sell_items_, &request.field_.sell_items_);
  EncodeBuyItemInfoMap(this->buy_items_, &request.field_.buy_items_);
  EncodeAuctionHistory(this->history_list_, &request.field_.history_);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_AUCTION_ACTOR_SAVE,
          this->GetActorID()) == false) {
    LOG_ERROR("Send StorageAuctionActorSaveRequest to database server failed.");
    return;
  }
}

bool AuctionActor::EnableAuctionFunc(bool synchronize) {
  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

bool AuctionActor::CheckSellItemsFull() const {
  return (core::int32)this->sell_items_.size() >=
         MISC_CONF()->auction_max_sell_item_capacity_;
}

bool AuctionActor::CheckInSellItems(const std::string &transaction_id) {
  return this->sell_items_.find(transaction_id) != this->sell_items_.end();
}

bool AuctionActor::AddSellItem(const std::string &transaction_id) {
  if (this->sell_items_.insert(transaction_id).second == false) {
    LOG_ERROR("Add sell item(%s) failed.", transaction_id.c_str());
    return false;
  }
  return true;
}

void AuctionActor::RemoveSellItem(const std::string &transaction_id) {
  this->sell_items_.erase(transaction_id);
}

bool AuctionActor::CheckBuyItemsFull() const {
  return (core::int32)this->buy_items_.size() >=
         MISC_CONF()->auction_max_buy_item_capacity_;
}

bool AuctionActor::CheckInBuyItems(const std::string &transaction_id) {
  return this->buy_items_.find(transaction_id) != this->buy_items_.end();
}

const AuctionActor::BuyItemInfo *AuctionActor::GetBuyItemInfo(
    const std::string &transaction_id) {
  BuyItemInfoMap::const_iterator iter = this->buy_items_.find(transaction_id);
  if (iter == this->buy_items_.end()) {
    return NULL;
  }
  return &iter->second;
}

bool AuctionActor::AddBuyItem(const BuyItemInfo &buy_item) {
  if (this->buy_items_.insert(
          std::make_pair(buy_item.transaction_id_, buy_item)).second == false) {
    LOG_ERROR("Add buy item(%s) failed.", buy_item.transaction_id_.c_str());
    return false;
  }

  // 关注交易
  AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(buy_item.transaction_id_);
  if (transaction != NULL) {
    transaction->AddFollower(this->GetActorID());
  }

  return true;
}

void AuctionActor::RemoveBuyItem(const std::string &transaction_id) {
  this->buy_items_.erase(transaction_id);

  // 取消关注交易
  AuctionTransaction *transaction =
      AuctionTransactionManager::GetInstance()->Get(transaction_id);
  if (transaction != NULL) {
    transaction->RemoveFollower(this->GetActorID());
  }
}

void AuctionActor::AddHistory(const AuctionHistory &history, bool synchronize) {
  if (this->history_list_.size() >= (size_t)MISC_CONF()->auction_max_history_capacity_ &&
      !this->history_list_.empty()) {
    this->history_list_.pop_front();
  }
  this->history_list_.push_back(history);

  if (synchronize) {
    gateway::protocol::MessageAuctionHistorySynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionHistoryOperateType::ADD);
    sync.data_.__set_type_((entity::AuctionHistoryType::type)history.GetType());
    sync.data_.__set_timestamp_(history.GetTimestamp());
    history.TranslateHistoryVar(&sync.data_.vars_);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_HISTORY_SYNCHRONIZE);
  }
}

}  // namespace auction

}  // namespace server

}  // namespace session

