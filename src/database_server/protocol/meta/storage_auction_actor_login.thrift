namespace cpp database.protocol

enum AuctionActorField {
  SELL_ITEMS,
  BUY_ITEMS,
  HISTORY,

  MAX,
}

const list<string> kAuctionActorFieldStr = [
  "sell_items",                                    // 出售道具
  "buy_items",                                     // 竞拍道具
  "history",                                       // 历史记录
]

enum AuctionActorLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageAuctionActorField {
  1 : required string sell_items_;
  2 : required string buy_items_;
  3 : required string history_;
}

// type STORAGE_AUCTION_ACTOR_LOGIN

struct StorageAuctionActorLoginRequest {
  1 : required i64 actor_id_;                      // 玩家ID
  2 : optional bool offline_load_;                 // 是否是离线加载
  3 : optional i64 offline_task_id_;               // 离线任务ID
}

struct StorageAuctionActorLoginResponse {
  1 : required AuctionActorLoginResult result_;    // 结果
  2 : required StorageAuctionActorField field_;    // 拍卖玩家字段
}

