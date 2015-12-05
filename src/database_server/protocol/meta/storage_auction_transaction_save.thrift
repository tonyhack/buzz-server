namespace cpp database.protocol

enum AuctionTransactionField {
  ID = 0,
  ITEM_TEMPLATE_ID,
  ITEM_NUMBER,
  ITEM_EXTRA_INFO,
  AUCTION_TIME_ID,
  START_BID_TIME,
  END_BID_TIME,
  OWNER,
  BUYER,
  START_BID_PRICE,
  LAST_BID_PRICE,
  BUYOUT_PRICE,
  FINISHED,
  MONEY_WITHDRAWN,
  ITEM_WITHDRAWN,
  
  MAX,
}

const list<string> kAuctionTransactionFieldStr = [
  "id",                                               // 交易ID
  "item_template_id",                                 // 道具模板ID
  "item_number",                                      // 道具数量
  "item_extra_info",                                  // 道具额外信息
  "auction_time_id",                                  // 拍卖时间ID
  "start_bid_time",                                   // 拍卖开始时间
  "end_bid_time",                                     // 拍卖结束时间
  "owner",                                            // 卖家
  "buyer",                                            // 买家
  "start_bid_price",                                  // 起拍价格
  "last_bid_price",                                   // 最近一次出价
  "buyout_price",                                     // 一口价
  "finished",                                         // 交易是否已结束
  "money_withdrawn",                                  // 交易付款是否已提取
  "item_withdrawn",                                   // 交易道具是否已提取
]

struct StorageAuctionTransactionField {
  1 : required string id_;
  2 : required i32 item_template_id_;
  3 : required i32 item_number_;
  4 : required string item_extra_info_;
  5 : required i32 auction_time_id_;
  6 : required i64 start_bid_time_;
  7 : required i64 end_bid_time_;
  8 : required i64 owner_;
  9 : required i64 buyer_;
  10: required i32 start_bid_price_;
  11: required i32 last_bid_price_;
  12: required i32 buyout_price_;
  13: required bool finished_;
  14: required bool money_withdrawn_;
  15: required bool item_withdrawn_;
}

// type: STORAGE_AUCTION_TRANSACTION_SAVE

struct StorageAuctionTransactionSaveRequest {
  1 : required StorageAuctionTransactionField field_; // 拍卖数据
}

struct StorageAuctionTransactionSaveResponse {
  1 : required bool result_;                          // 是否成功
}

