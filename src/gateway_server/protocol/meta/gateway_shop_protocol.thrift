include "gateway_protocol.thrift"
include "entity.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

////////////////////////////////////////
// 商店回购单元
struct ShopUnit {
  1 : required i32 template_id_;
  2 : required i16 location_; 
}

///////////////////////////////////////////
// 初始化商店容器
// type: MESSAGE_SHOP_BUYBACK_INITIALIZE
// dir: gateway->client
struct MessageShopBuybackInitialize {            
   1 : required list<ShopUnit> buybacks_;                // 商店回购列表
}

//////////////////////////////////////////
// 请求购买商品
// type: MESSAGE_SHOP_BUY_REQUEST
// dir: client->gateway
struct MessageShopBuyRequest {
  1 : required i32 template_id_;                         // 道具ID
  2 : required i32 num_;                                 // 数目
}

//////////////////////////////////////////
// 请求购买商品返回
// type: MESSAGE_SHOP_BUY_RESPONSE
// dir: gateway->client
struct MessageShopBuyResponse {
  1 : required i32 template_id_;                        // 道具ID
}

/////////////////////////////////////////
// 请求回购商品
// type: MESSAGE_SHOP_BUYBACK_REQUEST
// dir: client->gateway
struct MessageShopBuybackRequest {
  1 : required i16 location_;                          // 列表里的位置
}

///////////////////////////////////////
// 返回购买回购商品结果
// type: MESSAGE_SHOP_BUYBACK_RESPONSE
// dir: gateway->client
struct MessageShopBuybackResponse {
  1 : required bool result_;                          // 回购结果
}

///////////////////////////////////////
// 请求出售道具
// type: MESSAGE_SHOP_SELL_REQUEST
// dir: client->gateway
struct MessageShopSellRequest {
  1 : required i16 location_;                          // 出售道具
}

//////////////////////////////////////
// 请求出售道具返回
// type: MESSAGE_SHOP_SELL_RESPONSE
struct MessageShopSellResponse {
  1 : required bool result_;                          // 出售道具返回结果
}
