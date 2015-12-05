namespace cpp entity
namespace py ccentity.mall
namespace as3 com.dreamfire.sanguo.data.definition.entity

//////////////////////////////////////////
// 商城购买使用

enum MallUseType {
  USE_FREEDOM_DOLLARS,            // 使用钻石
  USE_RESTRICT_DOLLARS,           // 使用礼券
  USE_FREEDOM_RESTRICT,           // 礼券或者钻石
  USE_ARENA_INTEGRAL,             // 竞技场积分
  USE_JUSTICE,                    // 正义点
  USE_MIRACLE_INTEGRAL,           // 奇迹积分
  USE_LEGEND_INTEGRAL,            // 传奇积分
}


/////////////////////////////////////////
// 商店类型
enum MallTreasureType {
  MALL,                           // 商城
  TREASURE,                       // 藏宝阁
}

////////////////////////////////////////////
// 商城、藏宝阁公共配置定义
struct MallTreasureCell {
  1 : required byte tab_;
  2 : required byte shop_type_;
  3 : required byte location_;
  4 : required i32 id_;
  5 : required bool hot_;
  6 : required i32 dollars_;
  7 : required byte spend_type_;
  8 : required string up_start_time_;
  9 : required string up_end_time_;
  10 : required string dollars_start_time_;
  11 : required string dollars_end_time_;
  12 : required i32 limit_buy_num_;
  13 : required i32 sell_price_;
  14 : required byte bind_;
}

////////////////////////////////////////////
// 藏宝阁数据结构
struct MallReasureBuyNum {
  1 : required i32 item_id_;
  2 : required i32 num_;
}
