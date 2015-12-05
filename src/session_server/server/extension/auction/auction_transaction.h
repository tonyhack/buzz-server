#ifndef __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__H
#define __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__H

#include <ctime>
#include <string>

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/server/session_server.h"

namespace database { namespace protocol {
class StorageAuctionTransactionField;
}}
namespace gateway { namespace protocol {
class AuctionItemData;
}}

namespace session {

namespace server {

namespace auction {

class AuctionTransaction : public core::TimerHandler, public core::Noncopyable {
  enum { TIMER_ID_TRANSACTION_FINISH = 0 };

 public:
  typedef std::set<core::uint64> ActorIDSet;

  struct Status {
    enum type {
      MIN = 0,

      OFF_SHELF = 0,
      ON_SELL,
      SOLD,
      EXPIRED,

      MAX
    };
  };

  AuctionTransaction();
  ~AuctionTransaction();

  bool Initialize(const database::protocol::StorageAuctionTransactionField &db_field);
  bool Initialize(const std::string &id, core::uint32 item_template_id,
      core::int32 item_number, const std::string &item_extra_info,
      core::int32 auction_time_id, core::uint64 owner,
      core::int32 start_bid_price, core::int32 buyout_price);
  void InitializeSearchKey(core::uint32 item_template_id);
  void Finalize();
  void Save();
  void DeleteFromDB();

  void ExportAuctionItemData(gateway::protocol::AuctionItemData &data) const;

  const std::string &GetID() const { return this->id_; }
  core::uint32 GetItemTemplateID() const { return this->item_template_id_; }
  core::int32 GetItemNumber() const { return this->item_number_; }
  const std::string &GetItemExtraInfo() const { return this->item_extra_info_; }
  core::int32 GetAuctionTimeID() const { return this->auction_time_id_; }
  time_t GetStartBidTime() const { return this->start_bid_time_; }
  time_t GetEndBidTime() const { return this->end_bid_time_; }
  core::int32 GetLeftTime(time_t now = 0) const;
  core::uint64 GetOwner() const { return this->owner_; }
  std::string GetOwnerName() const;
  core::uint64 GetBuyer() const { return this->buyer_; }
  std::string GetBuyerName() const;
  core::int32 GetStartBidPrice() const { return this->start_bid_price_; }
  core::int32 GetLastBidPrice() const { return this->last_bid_price_; }
  core::int32 GetBuyoutPrice() const { return this->buyout_price_; }
  core::int32 GetBidPrice() const;
  core::int32 GetMinBidIncrementPrice() const;
  Status::type GetStatus() const;
  bool CheckFinished() const { return this->finished_; }
  bool CheckMoneyWithdrawn() const { return this->money_withdrawn_; }
  bool CheckItemWithdrawn() const { return this->item_withdrawn_; }

  void Cancel();
  bool Renew();
  void Bid(core::uint64 buyer, core::int32 bid_price);
  void Buyout(core::uint64 buyer);
  void SetFinished() { this->finished_ = true; }
  void SetMoneyWithdrawn() { this->money_withdrawn_ = true; }
  void SetItemWithdrawn() { this->item_withdrawn_ = true; }

  // 关注者
  void AddFollower(core::uint64 actor_id) { this->followers_.insert(actor_id); }
  void RemoveFollower(core::uint64 actor_id) { this->followers_.erase(actor_id); }
  template <typename T>
  void BroadcastToFollowerExcept(core::uint64 except_actor_id,
      const T &message, gateway::protocol::MessageType::type type);

  void StartTimer();
  virtual void OnTimer(core::uint32 id);

 private:
  std::string id_;
  core::uint32 item_template_id_;
  core::int32 item_number_;
  std::string item_extra_info_;
  core::int32 auction_time_id_;
  time_t start_bid_time_;
  time_t end_bid_time_;
  core::uint64 owner_;
  core::uint64 buyer_;
  core::int32 start_bid_price_;
  core::int32 last_bid_price_;
  core::int32 buyout_price_;
  bool finished_;
  bool money_withdrawn_;
  bool item_withdrawn_;

  ActorIDSet followers_;
};

template <typename T>
void AuctionTransaction::BroadcastToFollowerExcept(core::uint64 except_actor_id,
    const T &message, gateway::protocol::MessageType::type type) {
  std::vector<core::uint64> actors;
  for (ActorIDSet::const_iterator iter = this->followers_.begin();
       iter != this->followers_.end(); ++iter) {
    if (*iter != except_actor_id) {
      actors.push_back(*iter);
    }
  }
  if (actors.empty() == false) {
    SessionServerSingleton::GetInstance().MulticastMessage(message, type,
        &actors[0], actors.size());
  }
}

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__H

