#ifndef __SESSION__SERVER__AUCTION__AUCTION__ACTOR__H
#define __SESSION__SERVER__AUCTION__AUCTION__ACTOR__H

#include <ctime>
#include <deque>
#include <map>
#include <string>
#include <set>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/transaction_lock.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/extension/auction/auction_history.h"

namespace database { namespace protocol {
class StorageAuctionActorField;
}}

namespace session {

namespace server {

namespace auction {

class AuctionActor : public core::Noncopyable {
 public:
  struct BuyItemInfo {
    bool Initialize(const std::string &data);
    int Serialize(char *buff, size_t size) const;
    
    std::string transaction_id_;
    core::uint32 item_template_id_;
    core::int32 item_number_;
    core::uint64 owner_;
    core::int32 buy_price_;
  };

  typedef std::set<std::string> TransactionIDSet;
  typedef std::map<std::string, BuyItemInfo> BuyItemInfoMap;
  typedef std::deque<AuctionHistory> AuctionHistoryDeque;

  static const core::int32 kAuctionLock = -1;

  AuctionActor();
  ~AuctionActor();

  bool Initialize(BriefActor *brief_actor,
      const database::protocol::StorageAuctionActorField &db_field);
  void Finalize();
  void Online(SessionActor *session_actor);
  void Offline();
  void SendLoadFinishEvent();
  void Save();
  bool CheckOnline() const { return this->session_actor_ != NULL; }

  SessionActor *GetActor() const {
    return this->session_actor_;
  }
  core::uint64 GetActorID() const {
    return this->brief_actor_->GetID();
  }
  std::string GetStringID() const {
    return this->brief_actor_->GetStringID();
  }
  const std::string &GetName() const {
    return this->brief_actor_->GetName();
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if (this->session_actor_) {
      session_actor_->SendMessage(message, type);
    }
  }
  template <typename T>
  void SendGameMessage(const T &message, session::protocol::GameMessageType::type type) {
    if (this->session_actor_) {
      session_actor_->SendGameMessage(message, type);
    }
  }

  // 开启模块功能
  bool EnableAuctionFunc(bool synchronize = true);

  bool CheckSellItemsFull() const;
  bool CheckInSellItems(const std::string &transaction_id);
  bool AddSellItem(const std::string &transaction_id);
  void RemoveSellItem(const std::string &transaction_id);
  bool CheckBuyItemsFull() const;
  bool CheckInBuyItems(const std::string &transaction_id);
  const BuyItemInfo *GetBuyItemInfo(const std::string &transaction_id);
  bool AddBuyItem(const BuyItemInfo &buy_item);
  void RemoveBuyItem(const std::string &transaction_id);

  // 交易历史
  void AddHistory(const AuctionHistory &history, bool synchronize = true);

  // 事务锁
  TransactionLock *GetLock() { return &this->lock_; }

 private:
  void SendInitToClient();

  BriefActor *brief_actor_;
  SessionActor *session_actor_;

  TransactionIDSet sell_items_;
  BuyItemInfoMap buy_items_;
  AuctionHistoryDeque history_list_;

  // 事务锁
  TransactionLock lock_;
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__ACTOR__H

