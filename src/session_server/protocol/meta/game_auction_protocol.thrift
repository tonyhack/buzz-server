include "session_game_protocol.thrift"

namespace cpp session.protocol

////////////////////////////////////////
// 上架道具请求
// type: MESSAGE_AUCTION_SELL_REQUEST
// dir: session->game
struct MessageAuctionSellRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : optional i64 item_id_;                                    // 道具ID(用于拍卖装备)
  3 : optional i32 item_template_id_;                           // 道具模板ID(用于拍卖其他道具)
  4 : optional i32 item_number_;                                // 道具数量
  5 : optional string transaction_id_;                          // 交易ID(用于道具先下架后上架的情况)
  6 : required i32 start_bid_price_;                            // 拍卖起始价格
  7 : required i32 buyout_price_;                               // 拍卖一口价(0表示没有一口价)
  8 : required i32 auction_time_id_;                            // 拍卖时间ID
}



////////////////////////////////////////
// 上架道具回复
// type: MESSAGE_AUCTION_SELL_RESPONSE
// dir: game->session
struct MessageAuctionSellResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 item_template_id_;                           // 道具模板ID
  4 : optional i32 item_number_;                                // 道具数量
  5 : optional string item_extra_info_;                         // 道具额外信息
  6 : optional string transaction_id_;                          // 交易ID
  7 : optional i32 start_bid_price_;                            // 拍卖起始价格
  8 : optional i32 buyout_price_;                               // 拍卖一口价(0表示没有一口价)
  9 : optional i32 auction_time_id_;                            // 拍卖时间ID
}



////////////////////////////////////////
// 竞拍道具请求
// type: MESSAGE_AUCTION_BID_REQUEST
// dir: session->game
struct MessageAuctionBidRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required string transaction_id_;                          // 交易ID
  3 : required i32 bid_price_;                                  // 竞拍价
}



////////////////////////////////////////
// 竞拍道具回复
// type: MESSAGE_AUCTION_BID_RESPONSE
// dir: game->session
struct MessageAuctionBidResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional string transaction_id_;                          // 交易ID
  4 : optional i32 bid_price_;                                  // 竞拍价
}



////////////////////////////////////////
// 一口价购买道具请求
// type: MESSAGE_AUCTION_BUYOUT_REQUEST
// dir: session->game
struct MessageAuctionBuyoutRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required string transaction_id_;                          // 交易ID
  3 : required i32 buyout_price_;                               // 一口价
  4 : required i32 give_back_money_;                            // 玩家一口价购买自己最高出价的道具时补偿的金额
}



////////////////////////////////////////
// 一口价购买道具回复
// type: MESSAGE_AUCTION_BUYOUT_RESPONSE
// dir: game->session
struct MessageAuctionBuyoutResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional string transaction_id_;                          // 交易ID
  4 : optional i32 buyout_price_;                               // 一口价
}



////////////////////////////////////////
// 提取道具请求
// type: MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST
// dir: session->game
struct MessageAuctionWithdrawItemRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required string transaction_id_;                          // 交易ID
  3 : required i32 item_template_id_;                           // 道具模板ID
  4 : required i32 item_number_;                                // 道具数量
}



////////////////////////////////////////
// 提取道具回复
// type: MESSAGE_AUCTION_WITHDRAW_ITEM_RESPONE
// dir: game->session
struct MessageAuctionWithdrawItemResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional string transaction_id_;                          // 交易ID
}



////////////////////////////////////////
// 提取道具请求2
// type: MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST2
// dir: session->game
struct MessageAuctionWithdrawItemRequest2 {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 item_template_id_;                           // 道具模板ID
  3 : required i32 item_number_;                                // 道具数量
  4 : required string item_extra_info_;                         // 道具额外信息
}



////////////////////////////////////////
// 提取交易金额请求
// type: MESSAGE_AUCTION_WITHDRAW_MONEY_REQUEST
// dir: session->game
struct MessageAuctionWithdrawMoneyRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 money_;                                      // 交易金额
}



////////////////////////////////////////
// 退还拍卖金请求
// type: MESSAGE_AUCTION_GIVE_MONEY_BACK_REQUEST
// dir: session->game
struct MessageAuctionGiveMoneyBackRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 money_;                                      // 金额
}

