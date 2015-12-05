include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.attr

///////////////////////////////////
// 魔剑是否开启
// type: REQUEST_ATTR_CHECK_SWORD_OPEN
struct RequestAttrCheckSwordOpen {
  1 : required i64 actor_;                      // 玩家ID       (IN)
  2 : required bool result_;                    // 结果         (OUT)
}


///////////////////////////////////
// 开启魔剑
// type: REQUEST_ATTR_OPEN_SWORD
struct RequestAttrOpenSword {
  1 : required i64 actor_;                      // 玩家ID       (IN)
  2 : required bool result_;                    // 结果         (OUT)
}


///////////////////////////////////
// 获取魔剑阶级
// type: REQUEST_ATTR_GET_SWORD_STEP
struct RequestAttrGetSwordStep {
  1 : required i64 actor_;                      // 玩家ID       (IN)
  2 : required i32 step_;                       // 阶级         (OUT)
}


///////////////////////////////////
// 增加精灵外观
// type: REQUEST_ATTR_ADD_SPIRIT_FACADE
struct RequestAttrAddSpiritFacade {
  1 : required i64 actor_;                      // 玩家ID                     (IN)
  2 : required i32 spirit_facade_id_;           // 精灵外观ID                 (IN)
  3 : required i32 duration_time_;              // 持续时间(0表示无限时间)    (IN)
  4 : required bool result_;                    // 结果                       (OUT)
}


///////////////////////////////////
// 获取精灵等级
// type: REQUEST_ATTR_GET_SPIRIT_LEVEL
struct RequestAttrGetSpiritLevel {
  1 : required i64 actor_;                      // 玩家ID       (IN)
  2 : required i32 level_;                      // 等级         (OUT)
}


///////////////////////////////////
// 获取贵族(爵位)等级
// type: REQUEST_ATTR_GET_NOBLE_LEVEL
struct RequestAttrGetNobleLevel {
  1 : required i64 actor_;                      // 玩家ID       (IN)
  2 : required i32 level_;                      // 等级         (OUT)
}


///////////////////////////////////
// 获取精灵外观类型
// type: REQUEST_ATTR_GET_SPIRIT_FACADE_TYPE
struct RequestAttrGetSpiritFacadeType {
  1 : required i64 actor_;                       // 玩家         (IN)
  2 : required i32 type_;                        // 精灵外观类型 (OUT)
}
