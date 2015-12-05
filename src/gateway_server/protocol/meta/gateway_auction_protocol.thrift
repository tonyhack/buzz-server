include "auction.thrift"
include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

/////////////////////////////////////////
// 拍卖错误码
enum AuctionErrorCode {
  SUCCESS = 0,                                                  // 成功
  ITEM_NOT_EXIST,                                               // 道具不存在(被售出, 被下架)
  OUTBIDDED,                                                    // 竞价被超出
  ERROR_UNKNOWN,                                                // 未知错误
}



/////////////////////////////////////////
// 玩家拍卖道具状态类型
enum AuctionSellItemStatusType {
  OFF_SHELF = 0,                                                // 下架
  ON_SELL,                                                      // 上架
  SOLD,                                                         // 已售出
}



/////////////////////////////////////////
// 玩家竞拍道具状态类型
enum AuctionBuyItemStatusType {
  HIGHEST_BID = 0,                                              // 最高出价
  OUTBIDDED,                                                    // 竞价被超出
  SOLD,                                                         // 物品已售出
  WON,                                                          // 竞拍成功
}



/////////////////////////////////////////
// 拍卖道具数据
struct AuctionItemData {
  1 : required string transaction_id_;                          // 交易ID
  2 : optional i32 item_template_id_;                           // 道具模板ID
  3 : optional i32 item_number_;                                // 道具数量
  4 : optional string item_extra_info_;                         // 道具额外信息
  5 : optional i32 left_time_;                                  // 剩余时间
  6 : optional string owner_name_;                              // 卖家玩家名
  7 : optional i32 start_bid_price_;                            // 拍卖起始价格
  8 : optional i32 last_bid_price_;                             // 最后一次出价
  9 : optional i32 buyout_price_;                               // 一口价
  10: optional i32 auction_time_id_;                            // 拍卖时间ID
  11: optional i32 my_bid_price_;                               // 玩家出价
  12: optional i32 status_;                                     // 状态
}



/////////////////////////////////////////
// 拍卖历史数据
struct AuctionHistoryData {
  1 : required auction.AuctionHistoryType type_;                // 类型
  2 : required string timestamp_;                               // 时间戳
  3 : required list<string> vars_;                              // 变量表
}



/////////////////////////////////////////
// 拍卖数据初始化
// type: MESSAGE_AUCTION_INITIALIZE
// dir: gateway->client
struct MessageAuctionInitialize {
  1 : required list<AuctionItemData> sell_items_;               // 玩家拍卖道具
  2 : required list<AuctionItemData> buy_items_;                // 玩家竞拍道具
  3 : required list<AuctionHistoryData> history_;               // 历史记录
}



/////////////////////////////////////////
// 玩家拍卖道具操作类型
enum AuctionSellItemOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 玩家拍卖道具数据同步
// type: MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE
// dir: gateway->client
struct MessageAuctionSellItemSynchronize {
  1 : required AuctionSellItemOperateType type_;                // 操作类型
  2 : required AuctionItemData data_;                           // 数据
}



/////////////////////////////////////////
// 玩家竞拍道具操作类型
enum AuctionBuyItemOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 玩家竞拍道具数据同步
// type: MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE
// dir: gateway->client
struct MessageAuctionBuyItemSynchronize {
  1 : required AuctionBuyItemOperateType type_;                 // 操作类型
  2 : required AuctionItemData data_;                           // 数据
}



/////////////////////////////////////////
// 拍卖历史操作类型
enum AuctionHistoryOperateType {
  ADD = 0,                                                      // 增加
}

// 拍卖历史数据同步
// type: MESSAGE_AUCTION_HISTORY_SYNCHRONIZE
// dir: gateway->client
struct MessageAuctionHistorySynchronize {
  1 : required AuctionHistoryOperateType type_;                 // 操作类型
  2 : required AuctionHistoryData data_;                        // 数据
}



/////////////////////////////////////////
// 取交易信息请求
// type: MESSAGE_AUCTION_QUERY_TRANSACTION_REQUEST
// dir: client->gateway
struct MessageAuctionQueryTransactionRequest {
  1 : required string transaction_id_;                          // 交易ID
}



/////////////////////////////////////////
// 取交易信息回复
// type: MESSAGE_AUCTION_QUERY_TRANSACTION_RESPONSE
// dir: gateway->client
struct MessageAuctionQueryTransactionResponse {
  1 : required bool result_;                                    // 结果
  2 : optional AuctionItemData data_;                           // 数据
}



/////////////////////////////////////////
// 搜索道具请求
// type: MESSAGE_AUCTION_SEARCH_REQUEST
// dir: client->gateway
struct MessageAuctionSearchRequest {
  1 : optional list<i32> item_template_ids_;                    // 按道具ID搜索
  2 : optional i32 item_type_;                                  // 按道具类型搜索
  3 : required auction.AuctionSearchSortType sort_type_;        // 排序类型
  4 : required i32 page_;                                       // 页数
}



/////////////////////////////////////////
// 搜索道具回复
// type: MESSAGE_AUCTION_SEARCH_RESPONSE
// dir: gateway->client
struct MessageAuctionSearchResponse {
  1 : required bool result_;                                    // 结果
  2 : optional list<AuctionItemData> items_;                    // 道具信息
  3 : optional i32 page_;                                       // 页数
  4 : optional i32 total_page_;                                 // 总页数
}



/////////////////////////////////////////
// 上架道具请求
// type: MESSAGE_AUCTION_SELL_REQUEST
// dir: client->gateway
struct MessageAuctionSellRequest {
  1 : optional string item_id_;                                 // 道具ID(用于拍卖装备)
  2 : optional i32 item_template_id_;                           // 道具模板ID(用于拍卖其他道具)
  3 : optional i32 item_number_;                                // 道具数量
  4 : optional string transaction_id_;                          // 交易ID(用于道具先下架后上架的情况)
  5 : optional i32 start_bid_price_;                            // 拍卖起始价格
  6 : optional i32 buyout_price_;                               // 拍卖一口价(0表示没有一口价)
  7 : optional i32 auction_time_id_;                            // 拍卖时间ID
}



/////////////////////////////////////////
// 上架道具回复 
// type: MESSAGE_AUCTION_SELL_RESPONSE
// dir: gateway->client
struct MessageAuctionSellResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 下架道具请求 
// type: MESSAGE_AUCTION_CANCEL_SELL_REQUEST
// dir: client->gateway
struct MessageAuctionCancelSellRequest {
  1 : required string transaction_id_;                          // 交易ID
}



/////////////////////////////////////////
// 下架道具回复
// type: MESSAGE_AUCTION_CANCEL_SELL_RESPONSE
// dir: gateway->client
struct MessageAuctionCancelSellResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 竞拍道具请求
// type: MESSAGE_AUCTION_BID_REQUEST
// dir: client->gateway
struct MessageAuctionBidRequest {
  1 : required string transaction_id_;                          // 交易ID
  2 : required i32 bid_price_;                                  // 竞拍价格
}



/////////////////////////////////////////
// 竞拍道具回复
// type: MESSAGE_AUCTION_BID_RESPONSE
// dir: gateway->client
struct MessageAuctionBidResponse {
  1 : required AuctionErrorCode result_;                        // 结果
}



/////////////////////////////////////////
// 一口价购买道具请求
// type: MESSAGE_AUCTION_BUYOUT_REQUEST
// dir: client->gateway
struct MessageAuctionBuyoutRequest {
  1 : required string transaction_id_;                          // 交易ID
}



/////////////////////////////////////////
// 一口价购买道具回复
// type: MESSAGE_AUCTION_BUYOUT_RESPONSE
// dir: gateway->client
struct MessageAuctionBuyoutResponse {
  1 : required AuctionErrorCode result_;                        // 结果
}



/////////////////////////////////////////
// 提取(道具/交易金额)请求
// type: MESSAGE_AUCTION_WITHDRAW_REQUEST
// dir: client->gateway
struct MessageAuctionWithdrawRequest {
  1 : required string transaction_id_;                          // 交易ID
}



/////////////////////////////////////////
// 提取(道具/交易金额)回复
// type: MESSAGE_AUCTION_WITHDRAW_RESPONSE
// dir: gateway->client
struct MessageAuctionWithdrawResponse {
  1 : required bool result_;                                    // 结果
}

