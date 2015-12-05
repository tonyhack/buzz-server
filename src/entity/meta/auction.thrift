namespace cpp entity
namespace py ccentity.auction
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 拍卖历史类型定义
enum AuctionHistoryType {
  MIN = 0,

  BID_ITEM_SUCCESS = 0,               // 以竞价方式购买道具成功
  BUYOUT_ITEM_SUCCESS,                // 以一口价方式购买道具成功
  SELL_ITEM_BID_SUCCESS,              // 以竞价方式出售道具成功
  SELL_ITEM_BUYOUT_SUCCESS,           // 以一口价方式出售道具成功
  BID_ITEM,                           // 竞价道具
  OUTBIDDED,                          // 竞价被超过

  MAX,
}



////////////////////////////////////////////////
// 拍卖搜索排序类型
enum AuctionSearchSortType {
  MIN = 0,
  
  ITEM_TEMPLATE_ID = 0,               // 道具模板ID
  ITEM_NUMBER,                        // 道具数量
  LEFT_TIME,                          // 剩余时间
  BID_PRICE,                          // 竞拍价格

  MAX,
}

