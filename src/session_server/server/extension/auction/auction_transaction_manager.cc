#include "session_server/server/extension/auction/auction_transaction_manager.h"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <boost/bind.hpp>

#include "entity/auction_types.h"
#include "global/logging.h"
#include "global/actor_id.h"
#include "global/configure/configure.h"
#include "database_server/protocol/storage_auction_transaction_save_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_auction_protocol_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/sync_database_operation_manager.h"
#include "session_server/server/extension/auction/auction_actor.h"
#include "session_server/server/extension/auction/auction_actor_manager.h"
#include "session_server/server/extension/auction/auction_transaction.h"
#include "session_server/server/extension/auction/auction_transaction_pool.h"
#include "session_server/server/extension/auction/auction_history.h"

namespace session {

namespace server {

namespace auction {

bool AuctionTransactionLessItemTemplateID::operator()(
    const AuctionTransaction *lhs, const AuctionTransaction *rhs) const {
  if (lhs->GetItemTemplateID() != rhs->GetItemTemplateID()) {
    return lhs->GetItemTemplateID() < rhs->GetItemTemplateID();
  }
  if (lhs->GetStartBidTime() != rhs->GetStartBidTime()) {
    return lhs->GetStartBidTime() < rhs->GetStartBidTime();
  }
  return lhs->GetID() < rhs->GetID();
}

bool AuctionTransactionLessItemNumebr::operator()(
    const AuctionTransaction *lhs, const AuctionTransaction *rhs) const {
  if (lhs->GetItemNumber() != rhs->GetItemNumber()) {
    return lhs->GetItemNumber() < rhs->GetItemNumber();
  }
  if (lhs->GetItemTemplateID() != rhs->GetItemTemplateID()) {
    return lhs->GetItemTemplateID() < rhs->GetItemTemplateID();
  }
  if (lhs->GetStartBidTime() != rhs->GetStartBidTime()) {
    return lhs->GetStartBidTime() < rhs->GetStartBidTime();
  }
  return lhs->GetID() < rhs->GetID();
}

bool AuctionTransactionLessEndBidTime::operator()(
    const AuctionTransaction *lhs, const AuctionTransaction *rhs) const {
  if (lhs->GetEndBidTime() != rhs->GetEndBidTime()) {
    return lhs->GetEndBidTime() < rhs->GetEndBidTime();
  }
  if (lhs->GetItemTemplateID() != rhs->GetItemTemplateID()) {
    return lhs->GetItemTemplateID() < rhs->GetItemTemplateID();
  }
  if (lhs->GetStartBidTime() != rhs->GetStartBidTime()) {
    return lhs->GetStartBidTime() < rhs->GetStartBidTime();
  }
  return lhs->GetID() < rhs->GetID();
}

bool AuctionTransactionLessBidPrice::operator()(
    const AuctionTransaction *lhs, const AuctionTransaction *rhs) const {
  core::int32 left_bid_price = lhs->GetBidPrice();
  core::int32 right_bid_price = rhs->GetBidPrice();

  if (left_bid_price != right_bid_price) {
    return left_bid_price < right_bid_price;
  }
  if (lhs->GetStartBidTime() != rhs->GetStartBidTime()) {
    return lhs->GetStartBidTime() < rhs->GetStartBidTime();
  }
  return lhs->GetID() < rhs->GetID();
}

///////////////////////////////////////////////////////////////////////////////

bool AuctionTransactionManager::Initialize() {
  SyncDatabaseOperationManager::GetInstance()->AddLoader(
      boost::bind(&AuctionTransactionManager::LoadAuctionTransactions, this, _1));
  this->max_transaction_id_offset_ = 0;
  this->index_built_ = false;

  return true;
}

void AuctionTransactionManager::Finalize() {
  this->index_built_ = false;
  this->bid_price_indexes_.clear();
  this->end_bid_time_indexes_.clear();
  this->item_number_indexes_.clear();
  this->item_template_id_indexes_.clear();

  this->max_transaction_id_offset_ = 0;
  for (AuctionTransactionMap::iterator iter = this->transactions_.begin();
       iter != this->transactions_.end(); ++iter) {
    iter->second->Finalize();
    AuctionTransactionPool::GetInstance()->Deallocate(iter->second);
  }
  this->transactions_.clear();
}

bool AuctionTransactionManager::LoadAuctionTransactions(
    mysqlpp::Connection &connection) {
  mysqlpp::Query query(&connection, false);

  query << "CALL GAME_AUCTION_TRANSACTION_LOAD()";
  mysqlpp::StoreQueryResult result = query.store();

  if (query.errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
              query.str().c_str(), query.error());
    return false;
  }
  while (query.more_results()) {
    query.store_next();
  }
  query.reset();

  const char *fields[] = {
    "id",
    "item_template_id",
    "item_number",
    "item_extra_info",
    "auction_time_id",
    "start_bid_time",
    "end_bid_time",
    "owner",
    "buyer",
    "start_bid_price",
    "last_bid_price",
    "buyout_price",
    "finished",
    "money_withdrawn",
    "item_withdrawn"
  };

  for (size_t i = 0; i < sizeof(fields) / sizeof(char *); ++i) {
    if ((size_t)result.field_num(fields[i]) != i) { 
      LOG_ERROR("Database field order is incorrect.");
      return false;
    }
  }

  for (size_t row = 0; row < result.num_rows(); ++row) {
    database::protocol::StorageAuctionTransactionField db_field;
    int idx = 0;
    db_field.__set_id_(result[row][idx++].data());
    db_field.__set_item_template_id_(atoi(result[row][idx++].data()));
    db_field.__set_item_number_(atoi(result[row][idx++].data()));
    db_field.__set_item_extra_info_(result[row][idx++].data());
    db_field.__set_auction_time_id_(atoi(result[row][idx++].data()));
    db_field.__set_start_bid_time_(atol(result[row][idx++].data()));
    db_field.__set_end_bid_time_(atol(result[row][idx++].data()));
    db_field.__set_owner_(atol(result[row][idx++].data()));
    db_field.__set_buyer_(atol(result[row][idx++].data()));
    db_field.__set_start_bid_price_(atoi(result[row][idx++].data()));
    db_field.__set_last_bid_price_(atoi(result[row][idx++].data()));
    db_field.__set_buyout_price_(atoi(result[row][idx++].data()));
    db_field.__set_finished_(atoi(result[row][idx++].data()));
    db_field.__set_money_withdrawn_(atoi(result[row][idx++].data()));
    db_field.__set_item_withdrawn_(atoi(result[row][idx++].data()));

    // 分配交易对象
    AuctionTransaction *transaction = AuctionTransactionPool::GetInstance()->Allocate();
    if (NULL == transaction) {
      LOG_ERROR("Allocate AuctionTransacion(%s) from AuctionTransactionPool failed.",
                db_field.id_.c_str());
      return false;
    }

    // 初始化
    if (transaction->Initialize(db_field) == false) {
      AuctionTransactionPool::GetInstance()->Deallocate(transaction);
      LOG_ERROR("Init AuctionTransaction(%s) failed.", db_field.id_.c_str());
      return false;
    }

    // 加入管理器
    if (this->Add(transaction) == false) {
      transaction->Finalize();
      AuctionTransactionPool::GetInstance()->Deallocate(transaction);
      LOG_ERROR("Add AuctionTransaction(%s) to AuctionTransactionManager failed.",
                db_field.id_.c_str());
      return false;
    }
  }

  return true;
}

void AuctionTransactionManager::BuildIndex() {

  for (AuctionTransactionMap::iterator iter = this->transactions_.begin();
       iter != this->transactions_.end(); ++iter) {
    AuctionTransaction *transaction = iter->second;

    if (transaction->GetStatus() != AuctionTransaction::Status::ON_SELL) {
      continue;
    }
    if (transaction->GetLeftTime() == 0) {
      this->FinishTransaction(transaction);
      continue;
    }

    // 设置定时器
    transaction->StartTimer();
    // 加入索引
    this->AddToIndex(transaction);
  }

  index_built_ = true;
}

AuctionTransaction *AuctionTransactionManager::Get(const std::string &id) {
  AuctionTransactionMap::iterator iter = this->transactions_.find(id);
  if (iter != this->transactions_.end()) {
    return iter->second;
  }

  return NULL;
}

bool AuctionTransactionManager::Add(AuctionTransaction *transaction) {
  if (NULL == transaction) {
    return false;
  }
  if (this->transactions_.find(transaction->GetID()) != this->transactions_.end()) {
    return false;
  }

  this->transactions_.insert(std::make_pair(transaction->GetID(), transaction));

  return true;
}

AuctionTransaction *AuctionTransactionManager::Remove(const std::string &id) {
  AuctionTransaction *transaction = NULL;

  AuctionTransactionMap::iterator iter = this->transactions_.find(id);
  if (iter != this->transactions_.end()) {
    transaction = iter->second;
    this->transactions_.erase(iter);
  }

  return transaction;
}

void AuctionTransactionManager::AddToIndex(AuctionTransaction *transaction) {
  this->item_template_id_indexes_[0].insert(transaction);
  this->item_number_indexes_[0].insert(transaction);
  this->end_bid_time_indexes_[0].insert(transaction);
  this->bid_price_indexes_[0].insert(transaction);

  core::int32 item_type = transaction->GetItemTemplateID() / 10000000;
  if (item_type > 0) {
    this->item_template_id_indexes_[item_type].insert(transaction);
    this->item_number_indexes_[item_type].insert(transaction);
    this->end_bid_time_indexes_[item_type].insert(transaction);
    this->bid_price_indexes_[item_type].insert(transaction);
  }
}

void AuctionTransactionManager::RemoveFromIndex(AuctionTransaction *transaction) {
  this->item_template_id_indexes_[0].erase(transaction);
  this->item_number_indexes_[0].erase(transaction);
  this->end_bid_time_indexes_[0].erase(transaction);
  this->bid_price_indexes_[0].erase(transaction);

  core::int32 item_type = transaction->GetItemTemplateID() / 10000000;
  if (item_type > 0) {
    this->item_template_id_indexes_[item_type].erase(transaction);
    this->item_number_indexes_[item_type].erase(transaction);
    this->end_bid_time_indexes_[item_type].erase(transaction);
    this->bid_price_indexes_[item_type].erase(transaction);
  }
}

template<class T>
void FetchSearchResult(const T &index, core::int32 page,
  std::vector<std::string> &result, core::int32 &total_page) {

  total_page = index.size() / MISC_CONF()->auction_item_per_page_ + 1;
  if (page > total_page) {
    return;
  }

  typename T::const_iterator iter = index.begin();
  std::advance(iter, (page - 1) * MISC_CONF()->auction_item_per_page_);
  for (int i = 0;
       i < MISC_CONF()->auction_item_per_page_ && iter != index.end();
       ++i, ++iter) {
    result.push_back((*iter)->GetID());
  }
}

void AuctionTransactionManager::SearchTransaction(
    core::uint32 item_template_ids[], size_t number,
    entity::AuctionSearchSortType::type sort_type, core::int32 page,
    std::vector<std::string> &result, core::int32 &total_page) {
  std::vector<AuctionTransaction *> transactions;
  const ItemTemplateIDIndex &index = this->item_template_id_indexes_[0];

  for (size_t i = 0; i < number; ++i) {
    core::uint32 item_template_id = item_template_ids[i];
    AuctionTransaction search_key;
    search_key.InitializeSearchKey(item_template_id);

    for (ItemTemplateIDIndex::const_iterator iter = index.lower_bound(&search_key);
         iter != index.end() && (*iter)->GetItemTemplateID() == item_template_id;
         ++iter) {
      transactions.push_back(*iter);
    }
  }

  // 排序 
  if (entity::AuctionSearchSortType::ITEM_TEMPLATE_ID == sort_type) {
    // 不需要排序
  } else if (entity::AuctionSearchSortType::ITEM_NUMBER == sort_type) {
    std::sort(transactions.begin(), transactions.end(), AuctionTransactionLessItemNumebr());
  } else if (entity::AuctionSearchSortType::LEFT_TIME == sort_type) {
    std::sort(transactions.begin(), transactions.end(), AuctionTransactionLessEndBidTime());
  } else if (entity::AuctionSearchSortType::BID_PRICE == sort_type) {
    std::sort(transactions.begin(), transactions.end(), AuctionTransactionLessBidPrice());
  } else {
    LOG_ERROR("Invalid sort_type(%d)", sort_type);
    return;
  }

  FetchSearchResult(transactions, page, result, total_page);
}

void AuctionTransactionManager::SearchTransaction(core::int32 item_type,
    entity::AuctionSearchSortType::type sort_type, core::int32 page,
    std::vector<std::string> &result, core::int32 &total_page) {
  if (entity::AuctionSearchSortType::ITEM_TEMPLATE_ID == sort_type) {
    if (this->item_template_id_indexes_.find(item_type) ==
        this->item_template_id_indexes_.end()) {
      return;
    }
    FetchSearchResult(this->item_template_id_indexes_[item_type],
        page, result, total_page);

  } else if (entity::AuctionSearchSortType::ITEM_NUMBER == sort_type) {
    if (this->item_number_indexes_.find(item_type) ==
        this->item_number_indexes_.end()) {
      return;
    }
    FetchSearchResult(this->item_number_indexes_[item_type],
        page, result, total_page);

  } else if (entity::AuctionSearchSortType::LEFT_TIME == sort_type) {
    if (this->end_bid_time_indexes_.find(item_type) ==
        this->end_bid_time_indexes_.end()) {
      return;
    }
    FetchSearchResult(this->end_bid_time_indexes_[item_type],
        page, result, total_page);

  } else if (entity::AuctionSearchSortType::BID_PRICE == sort_type) {
    if (this->bid_price_indexes_.find(item_type) ==
        this->bid_price_indexes_.end()) {
      return;
    }
    FetchSearchResult(this->bid_price_indexes_[item_type],
        page, result, total_page);

  } else {
    LOG_ERROR("Invalid sort_type(%d)", sort_type);
    return;
  }
}

bool AuctionTransactionManager::CreateTransaction(AuctionActor *auction_actor,
    core::uint32 item_template_id, core::int32 item_number,
    const std::string &item_extra_info, core::int32 auction_time_id,
    core::int32 start_bid_price, core::int32 buyout_price,
    bool synchronize) {
  // 分配交易ID
  char buffer[32];
  global::ActorID decoded_actor_id(auction_actor->GetActorID());
  snprintf(buffer, sizeof(buffer), "%04d%010u%010u",
           decoded_actor_id.zone_,
           (core::uint32)GlobalTimeTick::GetInstance()->TimeTick().GetRealSec(),
           ++this->max_transaction_id_offset_);
  std::string transaction_id(buffer);

  // 分配交易对象
  AuctionTransaction *transaction = AuctionTransactionPool::GetInstance()->Allocate();
  if (NULL == transaction) {
    LOG_ERROR("Allocate AuctionTransacion(%s) from AuctionTransactionPool failed.",
              transaction_id.c_str());
    return false;
  }

  // 初始化
  if (transaction->Initialize(transaction_id, item_template_id, item_number,
          item_extra_info, auction_time_id, auction_actor->GetActorID(),
          start_bid_price, buyout_price) == false) {
    AuctionTransactionPool::GetInstance()->Deallocate(transaction);
    LOG_ERROR("Init AuctionTransacion(%s) failed.", transaction_id.c_str());
    return false;
  }

  // 加入玩家出售道具列表
  if (auction_actor->AddSellItem(transaction_id) == false) {
    transaction->Finalize();
    AuctionTransactionPool::GetInstance()->Deallocate(transaction);
    LOG_ERROR("Add sell item(%s) for actor(%lu) failed.",
              transaction_id.c_str(), auction_actor->GetActorID());
    return false;
  }

  // 加入管理器
  if (this->Add(transaction) == false) {
    auction_actor->RemoveSellItem(transaction_id);
    transaction->Finalize();
    AuctionTransactionPool::GetInstance()->Deallocate(transaction);
    LOG_ERROR("Add AuctionTransaction(%s) to AuctionTransactionManager failed.",
              transaction_id.c_str());
    return false;
  }

  // 设置定时器
  transaction->StartTimer();
  // 加入索引
  this->AddToIndex(transaction);
  // 保存数据库
  transaction->Save();

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageAuctionSellItemSynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionSellItemOperateType::ADD);
    transaction->ExportAuctionItemData(sync.data_);
    sync.data_.__set_status_(gateway::protocol::AuctionSellItemStatusType::ON_SELL);
    auction_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE);
  }

  return true;
}

void AuctionTransactionManager::DestroyTransaction(const std::string &transaction_id) {
  AuctionTransaction *transaction = this->Remove(transaction_id);
  if (NULL == transaction) {
    LOG_ERROR("Remove AuctionTransaction(%s) from AuctionTransactionManager failed.",
              transaction_id.c_str());
    return;
  }
  transaction->DeleteFromDB();
  transaction->Finalize();
  AuctionTransactionPool::GetInstance()->Deallocate(transaction);
}

void AuctionTransactionManager::CancelTransaction(AuctionActor *auction_actor,
    AuctionTransaction *transaction, bool synchronize) {
  // 从索引中移除
  this->RemoveFromIndex(transaction);

  // 撤销交易
  transaction->Cancel();
  // 保存数据库
  transaction->Save();

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageAuctionSellItemSynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionSellItemOperateType::UPDATE);
    sync.data_.__set_transaction_id_(transaction->GetID());
    sync.data_.__set_left_time_(0);
    sync.data_.__set_status_(gateway::protocol::AuctionSellItemStatusType::OFF_SHELF);
    auction_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE);
  }
}

bool AuctionTransactionManager::RenewTransaction(AuctionActor *auction_actor,
    AuctionTransaction *transaction, bool synchronize) {
  // 更新交易信息
  if (transaction->Renew() == false) {
    LOG_ERROR("Renew AuctionTransaction(%s) failed.", transaction->GetID().c_str());
    return false;
  }

  // 设置定时器
  transaction->StartTimer();
  // 加入索引
  this->AddToIndex(transaction);
  // 保存数据库
  transaction->Save();

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageAuctionSellItemSynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionSellItemOperateType::UPDATE);
    sync.data_.__set_transaction_id_(transaction->GetID());
    sync.data_.__set_left_time_(transaction->GetLeftTime());
    sync.data_.__set_status_(gateway::protocol::AuctionSellItemStatusType::ON_SELL);
    auction_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE);
  }

  return true;
}

static void AddOutbiddedHistory(core::uint64 buyer_actor_id,
    std::string transaction_id, core::int32 bid_price, bool synchronize) {
  AuctionActor *buyer = AuctionActorManager::GetInstance()->GetFromAll(buyer_actor_id); 
  if (NULL == buyer) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", buyer_actor_id);
    return;
  }

  AuctionTransaction *transaction =
    AuctionTransactionManager::GetInstance()->Get(transaction_id);
  if (NULL == transaction) {
    LOG_ERROR("Get AuctionTransaction(%s) from AuctionTransactionManager failed.",
              transaction_id.c_str());
    return;
  }

  AuctionHistory history(entity::AuctionHistoryType::OUTBIDDED);
  history.PushHistoryVar(transaction->GetOwner());
  history.PushHistoryVar(transaction->GetItemNumber());
  history.PushHistoryVar(transaction->GetItemTemplateID());
  history.PushHistoryVar(bid_price);
  buyer->AddHistory(history);

  if (buyer->CheckOnline() == false) {
    AuctionActorManager::GetInstance()->SaveAndRemoveFromCache(buyer_actor_id);
  }
}

bool AuctionTransactionManager::BidTransaction(AuctionActor *auction_actor,
    AuctionTransaction *transaction, core::int32 bid_price, bool synchronize) {
  // 加入玩家购买道具列表
  AuctionActor::BuyItemInfo buy_item_info;
  buy_item_info.transaction_id_ = transaction->GetID();
  buy_item_info.item_template_id_ = transaction->GetItemTemplateID();
  buy_item_info.item_number_ = transaction->GetItemNumber();
  buy_item_info.owner_ = transaction->GetOwner();
  buy_item_info.buy_price_ = bid_price;
  if (auction_actor->AddBuyItem(buy_item_info) == false) {
    LOG_ERROR("Add buy item(%s) fro actor(%lu) failed.",
              transaction->GetID().c_str(), auction_actor->GetActorID());
    return false;
  }

  // 从索引中移除
  this->RemoveFromIndex(transaction);

  // 保存竞价被超的玩家和出价
  core::uint64 old_buyer_actor_id = transaction->GetBuyer();
  core::int32 old_bid_price = transaction->GetLastBidPrice();
  // 竞价
  transaction->Bid(auction_actor->GetActorID(), bid_price);
  // 保存数据库
  transaction->Save();

  // 加入索引
  this->AddToIndex(transaction);

  AuctionActor *owner = AuctionActorManager::GetInstance()->Get(
      transaction->GetOwner());
  AuctionActor *old_buyer = (0 == old_buyer_actor_id) ? NULL :
                            AuctionActorManager::GetInstance()->Get(old_buyer_actor_id);

  // 竞拍玩家新增交易历史
  AuctionHistory history(entity::AuctionHistoryType::BID_ITEM);
  history.PushHistoryVar(transaction->GetOwner());
  history.PushHistoryVar(transaction->GetItemNumber());
  history.PushHistoryVar(transaction->GetItemTemplateID());
  history.PushHistoryVar(bid_price);
  auction_actor->AddHistory(history);

  // 竞价被超玩家新增交易历史
  if (old_buyer_actor_id != 0) {
    if (NULL == old_buyer) {
      // 离线加载
      AuctionActorManager::GetInstance()->OfflineLoad(old_buyer_actor_id,
          boost::bind(&AddOutbiddedHistory, old_buyer_actor_id, transaction->GetID(),
              old_bid_price, synchronize));
    } else {
      AddOutbiddedHistory(old_buyer_actor_id, transaction->GetID(),
          old_bid_price, synchronize);
    }
  }

  // 同步 client
  if (synchronize) {
    // 给卖家同步消息
    if (owner != NULL) {
      gateway::protocol::MessageAuctionSellItemSynchronize sync;
      sync.__set_type_(gateway::protocol::AuctionSellItemOperateType::UPDATE);
      sync.data_.__set_transaction_id_(transaction->GetID());
      sync.data_.__set_last_bid_price_(transaction->GetLastBidPrice());
      owner->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE);
    }

    // 给竞拍玩家同步消息
    {
      gateway::protocol::MessageAuctionBuyItemSynchronize sync;
      sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::ADD);
      transaction->ExportAuctionItemData(sync.data_);
      sync.data_.__set_my_bid_price_(bid_price);
      sync.data_.__set_status_(gateway::protocol::AuctionBuyItemStatusType::HIGHEST_BID);
      auction_actor->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
    }
    // 给竞价被超的玩家同步消息
    if (old_buyer != NULL) {
      gateway::protocol::MessageAuctionBuyItemSynchronize sync;
      sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::UPDATE);
      sync.data_.__set_transaction_id_(transaction->GetID());
      sync.data_.__set_last_bid_price_(transaction->GetLastBidPrice());
      sync.data_.__set_status_(gateway::protocol::AuctionBuyItemStatusType::OUTBIDDED);
      old_buyer->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
    }
    // 给交易关注者同步消息
    {
      gateway::protocol::MessageAuctionBuyItemSynchronize sync;
      sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::UPDATE);
      sync.data_.__set_transaction_id_(transaction->GetID());
      sync.data_.__set_last_bid_price_(transaction->GetLastBidPrice());
      transaction->BroadcastToFollowerExcept(transaction->GetBuyer(), sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
    }
  }

  return true;
}

bool AuctionTransactionManager::BuyoutTransaction(AuctionActor *auction_actor,
    AuctionTransaction *transaction, core::int32 buyout_price, bool synchronize) {
  // 处理家是当前交易的最高出价, 然后一口价购买道具的情况
  if (transaction->GetBuyer() == auction_actor->GetActorID()) {
    auction_actor->RemoveBuyItem(transaction->GetID());
    if (synchronize) {
      gateway::protocol::MessageAuctionBuyItemSynchronize sync;
      sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::REMOVE);
      sync.data_.__set_transaction_id_(transaction->GetID());
      auction_actor->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
    }
  }

  // 加入玩家购买道具列表
  AuctionActor::BuyItemInfo buy_item_info;
  buy_item_info.transaction_id_ = transaction->GetID();
  buy_item_info.item_template_id_ = transaction->GetItemTemplateID();
  buy_item_info.item_number_ = transaction->GetItemNumber();
  buy_item_info.owner_ = transaction->GetOwner();
  buy_item_info.buy_price_ = buyout_price;
  if (auction_actor->AddBuyItem(buy_item_info) == false) {
    LOG_ERROR("Add buy item(%s) fro actor(%lu) failed.",
              transaction->GetID().c_str(), auction_actor->GetActorID());
    return false;
  }

  // 保存竞价被超的玩家和出价
  core::uint64 old_buyer_actor_id = transaction->GetBuyer();
  core::int32 old_bid_price = transaction->GetLastBidPrice();
  // 一口价
  transaction->Buyout(auction_actor->GetActorID());
  // 结束交易
  this->FinishTransaction(transaction, true, synchronize);

  // 竞价被超玩家新增交易历史
  if (old_buyer_actor_id != 0) {
    AuctionActor *old_buyer = AuctionActorManager::GetInstance()->Get(old_buyer_actor_id);
    if (NULL == old_buyer) {
      // 离线加载
      AuctionActorManager::GetInstance()->OfflineLoad(old_buyer_actor_id,
          boost::bind(&AddOutbiddedHistory, old_buyer_actor_id, transaction->GetID(),
              old_bid_price, synchronize));
    } else {
      AddOutbiddedHistory(old_buyer_actor_id, transaction->GetID(),
          old_bid_price, synchronize);
    }
  }

  return true;
}

static void AddSellItemHistory(core::uint64 owner_actor_id, std::string transaction_id,
    bool buyout, bool synchronize) {
  AuctionActor *owner = AuctionActorManager::GetInstance()->GetFromAll(owner_actor_id); 
  if (NULL == owner) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", owner_actor_id);
    return;
  }

  AuctionTransaction *transaction =
    AuctionTransactionManager::GetInstance()->Get(transaction_id);
  if (NULL == transaction) {
    LOG_ERROR("Get AuctionTransaction(%s) from AuctionTransactionManager failed.",
              transaction_id.c_str());
    return;
  }

  core::int32 fee = std::max(1,
      transaction->GetLastBidPrice() * MISC_CONF()->auction_transaction_fee_percent_ / 100);
  // 收入可能为负数
  core::int32 income = transaction->GetLastBidPrice() - fee;

  if (buyout) {
    AuctionHistory history(entity::AuctionHistoryType::SELL_ITEM_BUYOUT_SUCCESS);
    history.PushHistoryVar(transaction->GetItemNumber());
    history.PushHistoryVar(transaction->GetItemTemplateID());
    history.PushHistoryVar(transaction->GetLastBidPrice());
    history.PushHistoryVar(MISC_CONF()->auction_transaction_fee_percent_);
    history.PushHistoryVar(fee);
    history.PushHistoryVar(income);
    owner->AddHistory(history);
  } else {
    AuctionHistory history(entity::AuctionHistoryType::SELL_ITEM_BID_SUCCESS);
    history.PushHistoryVar(transaction->GetItemNumber());
    history.PushHistoryVar(transaction->GetItemTemplateID());
    history.PushHistoryVar(transaction->GetLastBidPrice());
    history.PushHistoryVar(MISC_CONF()->auction_transaction_fee_percent_);
    history.PushHistoryVar(fee);
    history.PushHistoryVar(income);
    owner->AddHistory(history);
  }

  if (owner->CheckOnline() == false) {
    AuctionActorManager::GetInstance()->SaveAndRemoveFromCache(owner_actor_id);
  }
}

static void AddBuyItemHistory(core::uint64 buyer_actor_id, std::string transaction_id,
    bool buyout, bool synchronize) {
  AuctionActor *buyer = AuctionActorManager::GetInstance()->GetFromAll(buyer_actor_id); 
  if (NULL == buyer) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", buyer_actor_id);
    return;
  }

  AuctionTransaction *transaction =
    AuctionTransactionManager::GetInstance()->Get(transaction_id);
  if (NULL == transaction) {
    LOG_ERROR("Get AuctionTransaction(%s) from AuctionTransactionManager failed.",
              transaction_id.c_str());
    return;
  }

  if (buyout) {
    AuctionHistory history(entity::AuctionHistoryType::BUYOUT_ITEM_SUCCESS);
    history.PushHistoryVar(transaction->GetOwner());
    history.PushHistoryVar(transaction->GetItemTemplateID());
    history.PushHistoryVar(transaction->GetItemNumber());
    history.PushHistoryVar(transaction->GetLastBidPrice());
    buyer->AddHistory(history);
  } else {
    AuctionHistory history(entity::AuctionHistoryType::BID_ITEM_SUCCESS);
    history.PushHistoryVar(transaction->GetOwner());
    history.PushHistoryVar(transaction->GetItemTemplateID());
    history.PushHistoryVar(transaction->GetItemNumber());
    history.PushHistoryVar(transaction->GetLastBidPrice());
    buyer->AddHistory(history);
  }

  if (buyer->CheckOnline() == false) {
    AuctionActorManager::GetInstance()->SaveAndRemoveFromCache(buyer_actor_id);
  }
}

void AuctionTransactionManager::FinishTransaction(AuctionTransaction *transaction,
    bool buyout, bool synchronize) {
  // 从索引中移除
  this->RemoveFromIndex(transaction);

  // 标记为已结束
  transaction->SetFinished();
  // 保存数据库
  transaction->Save();

  AuctionActor *owner = AuctionActorManager::GetInstance()->Get(
      transaction->GetOwner());
  AuctionActor *buyer = (transaction->GetBuyer() == 0) ? NULL :
                        AuctionActorManager::GetInstance()->Get(transaction->GetBuyer());

  // 新增交易历史
  if (transaction->GetStatus() == AuctionTransaction::Status::SOLD) {
    // 出售道具历史
    if (NULL == owner) {
      // 离线加载
      AuctionActorManager::GetInstance()->OfflineLoad(transaction->GetOwner(),
          boost::bind(&AddSellItemHistory, transaction->GetOwner(), transaction->GetID(),
              buyout, synchronize));
    } else {
      AddSellItemHistory(transaction->GetOwner(), transaction->GetID(), buyout, synchronize);
    }

    // 购买道具历史
    if (NULL == buyer) {
      // 离线加载
      AuctionActorManager::GetInstance()->OfflineLoad(transaction->GetBuyer(),
          boost::bind(&AddBuyItemHistory, transaction->GetBuyer(), transaction->GetID(),
              buyout, synchronize));
    } else {
      AddBuyItemHistory(transaction->GetBuyer(), transaction->GetID(), buyout, synchronize);
    }
  }

  // 同步 client
  if (synchronize) {
    // 给卖家同步消息
    if (owner != NULL) {
      gateway::protocol::MessageAuctionSellItemSynchronize sync;
      sync.__set_type_(gateway::protocol::AuctionSellItemOperateType::UPDATE);
      sync.data_.__set_transaction_id_(transaction->GetID());
      sync.data_.__set_left_time_(0);
      if (buyout) {
        sync.data_.__set_last_bid_price_(transaction->GetLastBidPrice());
      }
      if (NULL == buyer) {
        sync.data_.__set_status_(gateway::protocol::AuctionSellItemStatusType::OFF_SHELF);
      } else {
        sync.data_.__set_status_(gateway::protocol::AuctionSellItemStatusType::SOLD);
      }
      owner->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE);
    }
    // 给买家同步消息
    if (buyer != NULL) {
      gateway::protocol::MessageAuctionBuyItemSynchronize sync;
      if (buyout) {
        sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::ADD);
        transaction->ExportAuctionItemData(sync.data_);
        sync.data_.__set_my_bid_price_(transaction->GetLastBidPrice());
      } else {
        sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::UPDATE);
        sync.data_.__set_transaction_id_(transaction->GetID());
      }
      sync.data_.__set_status_(gateway::protocol::AuctionBuyItemStatusType::WON);
      buyer->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
    }
    // 给交易关注者同步消息
    {
      gateway::protocol::MessageAuctionBuyItemSynchronize sync;
      sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::UPDATE);
      sync.data_.__set_transaction_id_(transaction->GetID());
      sync.data_.__set_left_time_(0);
      if (buyout) {
        sync.data_.__set_last_bid_price_(transaction->GetLastBidPrice());
      }
      sync.data_.__set_status_(gateway::protocol::AuctionBuyItemStatusType::SOLD);
      transaction->BroadcastToFollowerExcept(transaction->GetBuyer(), sync,
          gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
    }
  }
}

void AuctionTransactionManager::OwnerWithdrawMoney(AuctionActor *auction_actor,
    AuctionTransaction *transaction, bool synchronize) {
  // 从玩家出售道具列表中删除
  auction_actor->RemoveSellItem(transaction->GetID());

  // 标记交易金额已领取
  transaction->SetMoneyWithdrawn();
  // 保存数据库
  transaction->Save();

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageAuctionSellItemSynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionSellItemOperateType::REMOVE);
    sync.data_.__set_transaction_id_(transaction->GetID());
    auction_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE);
  }

  // 道具和交易金额都已领取, 销毁该交易
  if (transaction->CheckMoneyWithdrawn() && transaction->CheckItemWithdrawn()) {
    this->DestroyTransaction(transaction->GetID());
  }
}

void AuctionTransactionManager::OwnerWithdrawItem(AuctionActor *auction_actor,
    AuctionTransaction *transaction, bool synchronize) {
  // 从玩家出售道具列表中删除
  auction_actor->RemoveSellItem(transaction->GetID());

  // 同步client
  if (synchronize) {
    gateway::protocol::MessageAuctionSellItemSynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionSellItemOperateType::REMOVE);
    sync.data_.__set_transaction_id_(transaction->GetID());
    auction_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE);
  }

  // 销毁交易
  this->DestroyTransaction(transaction->GetID());
}

void AuctionTransactionManager::BuyerWithdrawMoney(AuctionActor *auction_actor,
    const std::string &transaction_id, bool synchronize) {
  // 从玩家购买道具列表中删除
  auction_actor->RemoveBuyItem(transaction_id);

  // 同步client
  if (synchronize) {
    gateway::protocol::MessageAuctionBuyItemSynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::REMOVE);
    sync.data_.__set_transaction_id_(transaction_id);
    auction_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
  }
}

void AuctionTransactionManager::BuyerWithdrawItem(AuctionActor *auction_actor,
    AuctionTransaction *transaction, bool synchronize) {
  // 从玩家购买道具列表中删除
  auction_actor->RemoveBuyItem(transaction->GetID());

  // 标记道具已领取
  transaction->SetItemWithdrawn();
  // 保存数据库
  transaction->Save();

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageAuctionBuyItemSynchronize sync;
    sync.__set_type_(gateway::protocol::AuctionBuyItemOperateType::REMOVE);
    sync.data_.__set_transaction_id_(transaction->GetID());
    auction_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE);
  }

  // 道具和交易金额都已领取, 销毁该交易
  if (transaction->CheckMoneyWithdrawn() && transaction->CheckItemWithdrawn()) {
    this->DestroyTransaction(transaction->GetID());
  }
}

}  // namespace auction

}  // namespace server

}  // namespace session

