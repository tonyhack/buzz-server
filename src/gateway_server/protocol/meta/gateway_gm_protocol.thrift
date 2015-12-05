include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


// 增加道具
// type: MESSAGE_GM_ADD_ITEM
// dir: client->gateway
struct MessageGmAddItem {
  1 : required string actor_name_;                        // 玩家名
  2 : required i32 item_template_;                        // 道具模板ID
  3 : required i16 number_;                               // 道具个数
  4 : required bool bind_;                                // 是否绑定
}



// 场景跳转
// type: MESSAGE_GM_SCENE_JUMP
// dir: client->gateway
struct MessageGmSceneJump {
  1 : required i32 map_;                                  // 地图ID
  2 : required i32 x_;                                    // 坐标x
  3 : required i32 y_;                                    // 坐标y
}



// 开启魔剑
// type: MESSAGE_GM_OPEN_SWORD
// dir: client->gateway
struct MessageGmOpenSword {
}



// 通用GM指令
struct MessageGmCommand {
  1 : required string command_;
}

