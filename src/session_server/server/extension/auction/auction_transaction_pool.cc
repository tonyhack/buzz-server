#include "session_server/server/extension/auction/auction_transaction_pool.h"

namespace session {

namespace server {

namespace auction {

bool AuctionTransactionPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->transactions_.Initialize(initial_number, extend_number);
}

void AuctionTransactionPool::Finalize() {
  this->transactions_.Finalize();
}

AuctionTransaction *AuctionTransactionPool::Allocate() {
  return this->transactions_.Allocate();
}

void AuctionTransactionPool::Deallocate(AuctionTransaction *transaction) {
  this->transactions_.Deallocate(transaction);
}

}  // namespace auction

}  // namespace server

}  // namespace session

