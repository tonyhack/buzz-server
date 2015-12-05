include "session_request.thrift"
include "mall.thrift"
namespace cpp session.server.request

////////////////////////////////////////////
// 请求商城、藏宝阁数据
// type: REQUEST_ITEM_MALL_TREASURE_CONFIGURE_DATA
struct RequestItemMallTreasureConfigureData {
  1 : required list<mall.MallTreasureCell> configure_lists_;    // 配置列表
}
