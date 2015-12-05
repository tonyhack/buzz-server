namespace cpp database.protocol

// type STORAGE_AUCTION_TRANSACTION_DEL

struct StorageAuctionTransactionDelRequest {
  1 : required string id_;                         // 交易ID
}

struct StorageAuctionTransactionDelResponse {
  1 : required bool result_;                       // 是否成功
}

