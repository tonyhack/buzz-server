#include "session_server/server/extension/auction/auction_transaction.h"

#include <cstddef>
#include <algorithm>

#include "global/logging.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_auction_transaction_save_types.h"
#include "database_server/protocol/storage_auction_transaction_del_types.h"
#include "gateway_server/protocol/gateway_auction_protocol_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/auction/auction_transaction_manager.h"

namespace session {

namespace server {

namespace auction {

AuctionTransaction::AuctionTransaction() :
  item_template_id_(0), item_number_(0),
  auction_time_id_(0), start_bid_time_(0), end_bid_time_(0),
  owner_(0), buyer_(0),
  start_bid_price_(0), last_bid_price_(0), buyout_price_(0), 
  finished_(false), money_withdrawn_(false), item_withdrawn_(false) {
}
  
AuctionTransaction::~AuctionTransaction() {
}

bool AuctionTransaction::Initialize(
    const database::protocol::StorageAuctionTransactionField &db_field) {
  this->id_ = db_field.id_;
  this->item_template_id_ = db_field.item_template_id_;
  this->item_number_ = db_field.item_number_;
  this->item_extra_info_ = db_field.item_extra_info_;
  this->auction_time_id_ = db_field.auction_time_id_;
  this->start_bid_time_ = db_field.start_bid_time_;
  this->end_bid_time_ = db_field.end_bid_time_;
  this->owner_ = db_field.owner_;
  this->buyer_ = db_field.buyer_;
  this->start_bid_price_ = db_field.start_bid_price_;
  this->last_bid_price_ = db_field.last_bid_price_;
  this->buyout_price_ = db_field.buyout_price_;
  this->finished_ = db_field.finished_;
  this->money_withdrawn_ = db_field.money_withdrawn_;
  this->item_withdrawn_ = db_field.item_withdrawn_;

  return true;
}

bool AuctionTransaction::Initialize(const std::string &id, core::uint32 item_template_id,
    core::int32 item_number, const std::string &item_extra_info,
    core::int32 auction_time_id, core::uint64 owner,
    core::int32 start_bid_price, core::int32 buyout_price) {
  const global::configure::AuctionTimeCell *cell =
      AUCTION_CONF()->GetAuctionTime(auction_time_id);
  if (NULL == cell) {
    LOG_ERROR("action_time_id(%d) is invalid.", auction_time_id);
    return false;
  }

  this->id_ = id;
  this->item_template_id_ = item_template_id;
  this->item_number_ = item_number;
  this->item_extra_info_ = item_extra_info;
  this->auction_time_id_ = auction_time_id;
  this->start_bid_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  this->end_bid_time_ = this->start_bid_time_ + cell->time_;
  this->owner_ = owner;
  this->buyer_ = 0;
  this->start_bid_price_ = start_bid_price;
  this->last_bid_price_ = 0;
  this->buyout_price_ = buyout_price;
  this->finished_ = false;
  this->money_withdrawn_ = false;
  this->item_withdrawn_ = false;

  return true;
}

void AuctionTransaction::InitializeSearchKey(core::uint32 item_template_id) {
  this->id_ = "";
  this->item_template_id_ = item_template_id;
  this->start_bid_time_ = 0;
}

void AuctionTransaction::Finalize() {
  // 移除定时器 
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_TRANSACTION_FINISH, this);

  this->followers_.clear();

  this->item_withdrawn_ = false;
  this->money_withdrawn_ = false;
  this->finished_ = false;
  this->buyout_price_ = 0;
  this->last_bid_price_ = 0;
  this->start_bid_price_ = 0;
  this->buyer_ = 0;
  this->owner_ = 0;
  this->end_bid_time_ = 0;
  this->start_bid_time_ = 0;
  this->auction_time_id_ = 0;
  this->item_extra_info_.clear();
  this->item_number_ = 0;
  this->item_template_id_ = 0;
  this->id_.clear();
}

void AuctionTransaction::StartTimer() {
  if (this->finished_) {
    LOG_ERROR("Transaction(%s) is finished.", this->id_.c_str());
    return;
  }

  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_TRANSACTION_FINISH,
    this->GetLeftTime() * 1000, 1, this, "AuctionTransaction::StartTimer");
}

void AuctionTransaction::OnTimer(core::uint32 id) {
  if (TIMER_ID_TRANSACTION_FINISH == id) {
    AuctionTransactionManager::GetInstance()->FinishTransaction(this);
  }
}

void AuctionTransaction::Save() {
  database::protocol::StorageAuctionTransactionSaveRequest request;
  request.field_.id_ = this->id_;
  request.field_.item_template_id_ = this->item_template_id_;
  request.field_.item_number_ = this->item_number_;
  request.field_.item_extra_info_ = this->item_extra_info_;
  request.field_.auction_time_id_ = this->auction_time_id_;
  request.field_.start_bid_time_ = this->start_bid_time_;
  request.field_.end_bid_time_ = this->end_bid_time_;
  request.field_.owner_ = this->owner_;
  request.field_.buyer_ = this->buyer_;
  request.field_.start_bid_price_ = this->start_bid_price_;
  request.field_.last_bid_price_ = this->last_bid_price_;
  request.field_.buyout_price_ = this->buyout_price_;
  request.field_.finished_ = this->finished_;
  request.field_.money_withdrawn_ = this->money_withdrawn_;
  request.field_.item_withdrawn_ = this->item_withdrawn_;

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_AUCTION_TRANSACTION_SAVE,
          this->owner_) == false) {
    LOG_ERROR("Send StorageAuctionTransactionSaveRequest to database server failed.");
    return;
  }
}

void AuctionTransaction::DeleteFromDB() {
  database::protocol::StorageAuctionTransactionDelRequest request;
  request.id_ = this->id_;

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_AUCTION_TRANSACTION_DEL,
          this->owner_) == false) {
    LOG_ERROR("Send StorageAuctionTransactionDelRequest to database server failed.");
    return;
  }
}

void AuctionTransaction::ExportAuctionItemData(
    gateway::protocol::AuctionItemData &data) const {
  data.__set_transaction_id_(this->GetID());
  data.__set_item_template_id_(this->GetItemTemplateID());
  data.__set_item_number_(this->GetItemNumber());
  data.__set_item_extra_info_(this->GetItemExtraInfo());
  data.__set_left_time_(this->GetLeftTime());
  data.__set_owner_name_(this->GetOwnerName());
  data.__set_start_bid_price_(this->GetStartBidPrice());
  data.__set_last_bid_price_(this->GetLastBidPrice());
  data.__set_buyout_price_(this->GetBuyoutPrice());
  data.__set_auction_time_id_(this->GetAuctionTimeID());
}

core::int32 AuctionTransaction::GetLeftTime(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->finished_) {
    return 0;
  }
  if (this->end_bid_time_ > now) {
    return this->end_bid_time_ - now;
  } else {
    return 0;
  }
}

std::string AuctionTransaction::GetOwnerName() const {
  if (0 == this->owner_) {
    return std::string();
  }

  BriefActor *brief_actor =
    SessionActorManager::GetInstance()->GetBriefActor(this->owner_);
  if (NULL == brief_actor) {
    LOG_ERROR("actor_id(%lu) does not exists.", this->owner_);
    return std::string();
  } else {
    return brief_actor->GetName();
  }
}

std::string AuctionTransaction::GetBuyerName() const {
  if (0 == this->buyer_) {
    return std::string();
  }

  BriefActor *brief_actor =
    SessionActorManager::GetInstance()->GetBriefActor(this->buyer_);
  if (NULL == brief_actor) {
    LOG_ERROR("actor_id(%lu) does not exists.",  this->buyer_);
    return std::string();
  } else {
    return brief_actor->GetName();
  }
}

core::int32 AuctionTransaction::GetBidPrice() const {
  if (0 == this->buyout_price_) {
    return std::max(this->start_bid_price_, this->last_bid_price_);
  } else if (0 == this->start_bid_price_) {
    return this->buyout_price_;
  } else {
    return std::max(std::max(this->start_bid_price_, this->last_bid_price_),
              this->buyout_price_);
  }
}

core::int32 AuctionTransaction::GetMinBidIncrementPrice() const {
  return std::max(1, MISC_CONF()->auction_min_bid_increment_percent_ *
                     this->start_bid_price_ / 100);
}

AuctionTransaction::Status::type AuctionTransaction::GetStatus() const {
  if (this->start_bid_time_ <= 0) {
    return Status::OFF_SHELF;
  }

  if (this->finished_ == false) {
    return Status::ON_SELL;
  }

  if (this->buyer_ != 0) {
    return Status::SOLD;
  } else {
    return Status::EXPIRED;
  }
}

void AuctionTransaction::Cancel() {
  this->start_bid_time_ = 0;
  this->end_bid_time_ = 0;
}

bool AuctionTransaction::Renew() {
  const global::configure::AuctionTimeCell *cell =
      AUCTION_CONF()->GetAuctionTime(this->auction_time_id_);
  if (NULL == cell) {
    LOG_ERROR("action_time_id(%d) is invalid.", auction_time_id_);
    return false;
  }
  this->start_bid_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  this->end_bid_time_ = this->start_bid_time_ + cell->time_;
  this->finished_ = false;

  return true;
}

void AuctionTransaction::Bid(core::uint64 buyer, core::int32 bid_price) {
  this->buyer_ = buyer;
  this->last_bid_price_ = bid_price;
}

void AuctionTransaction::Buyout(core::uint64 buyer) {
  this->buyer_ = buyer;
  this->last_bid_price_ = this->buyout_price_;
}

}  // namespace auction

}  // namespace server

}  // namespace session

