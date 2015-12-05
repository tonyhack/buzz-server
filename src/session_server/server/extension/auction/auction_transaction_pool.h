#ifndef __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__POOL__H
#define __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"
#include "session_server/server/extension/auction/auction_transaction.h"

namespace session {

namespace server {

namespace auction {

class AuctionTransactionPool : public global::SingletonFactory<AuctionTransactionPool> {
  friend class global::SingletonFactory<AuctionTransactionPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  AuctionTransaction *Allocate();
  void Deallocate(AuctionTransaction *transaction);

 private:
  AuctionTransactionPool() {}
  ~AuctionTransactionPool() {}

  global::PoolTemplate<AuctionTransaction> transactions_;
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__POOL__H

