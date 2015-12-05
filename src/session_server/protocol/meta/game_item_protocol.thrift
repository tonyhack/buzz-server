include "session_game_protocol.thrift"
include "mall.thrift"

namespace cpp session.protocol

/////////////////////////////////////////////
// 寻宝公告内容
// type: MESSAGE_ITEM_TREASURE_HUNT_SYNCHRONIZE
// dir: game->session
struct MessageItemTreasureHuntSynchronize {
  1 : required list<string> info_;              // 公告信息(, 号分割)
}

/////////////////////////////////////////////
// 通知game_server 商城、藏宝阁配置
// type: MESSAGE_ITEM_MALL_CONFIG_SYNCHRONIZE
// dir: sesseion->game
struct MessageItemMallConfigSynchronize {
  1 : required list<mall.MallTreasureCell> mall_config_;  // 商城、藏宝阁配置
  2 : required bool is_new_;                              // 是不是新数据
}
