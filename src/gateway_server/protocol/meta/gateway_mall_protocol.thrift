include "gateway_protocol.thrift"
include "mall.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

////////////////////////////////
// 商城、藏宝阁购买
// type: MESSAGE_MALL_BUY_REQUEST
// dir: client->gateway
struct MessageMallBuyRequest {
  1 : required mall.MallTreasureType shop_type_;          // 商店类型
  2 : required byte tab_;                                 // 标签页
  3 : required byte pos_;                                 // 位置
  4 : required mall.MallUseType use_type_;                // 购买类型
  5 : required i32 num_;                                  // 购买数量
}

////////////////////////////////
// 商城、藏宝阁购买返回:
// type: MESSAGE_MALL_BUY_RESPONSE
// dir: gateway->client
struct MessageMallBuyResponse {
  1 : required mall.MallUseType use_type_;                // 购买类型
  2 : required i32 id_;                                   // 购买ID
  3 : required i32 num_;                                  // 购买数量
  4 : required mall.MallTreasureType shop_type_;          // 商店类型
}

///////////////////////////////
// 商城、藏宝阁推送信息
// type: MESSAGE_MALL_SYNCHRONIZE
// dir: gateway->client
struct MessageMallSynchronize {
  1 : required list<mall.MallTreasureCell> cells_;        // 推送的内容
}

//////////////////////////////
// 藏宝阁数据初始化
// type: MESSAGE_MALL_ITEM_BUY_NUM_INITIALIZE
// dir: gateway->client
struct MessageMallItemBuyNumInitialize {
  1 : required list<mall.MallReasureBuyNum> buys_;       // 购买次数列表
}
