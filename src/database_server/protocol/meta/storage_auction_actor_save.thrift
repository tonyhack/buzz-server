include "storage_auction_actor_login.thrift"

namespace cpp database.protocol

// type STORAGE_AUCTION_ACTOR_SAVE

struct StorageAuctionActorSaveRequest {
  1 : required i64 actor_id_;                                               // 玩家ID
  2 : required storage_auction_actor_login.StorageAuctionActorField field_; // 社交玩家字段
}

struct StorageAuctionActorSaveResponse {
  1 : required bool result_;                                                // 是否成功
}

