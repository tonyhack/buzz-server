include "game_request.thrift"
include "soul.thrift"

namespace cpp game.server.request
namespace py ccrequest.soul


enum SoulResultType {
  MIN = 0,
  SUCCESS = 0,                                        // 成功
  ERROR_EXIST,                                        // 已存在
  ERROR_NOT_EXIST,                                    // 不存在
  ERROR_MAX_LEVEL,                                    // 达到最高等级
  ERROR_ACTOR_LEVEL,                                  // 玩家等级达不到
  ERROR_UNKNOWN,                                      // 未知错误
  MAX,
}



//////////////////////////////////
// 英灵是否存在
// type: REQUEST_CHECK_SOUL_EXIST
struct RequestCheckSoulExit {
  1 : required i64 actor_;                            // 玩家ID (IN)
  2 : required i32 soul_id_;                          // 英灵ID (IN)
  3 : required bool result_;                          // 结果   (OUT)
}



//////////////////////////////////
// 增加英灵
// type: REQUEST_ADD_SOUL
struct RequestAddSoul {
  1 : required i64 actor_;                            // 玩家ID (IN)
  2 : required i32 soul_id_;                          // 英灵ID (IN)
  3 : required SoulResultType result_;                // 结果   (OUT)
}



//////////////////////////////////
struct SoulFields {
  1 : required i32 id_;                               // 英灵ID
  2 : required i32 level_;                            // 等级
  3 : required bool appear_;                          // 是否幻化类
}
// 获取玩家英灵基本信息
// type: REQUEST_SOUL_GET
struct RequestSoulGet {
  1 : required i64 actor_;                            // 玩家ID   (IN)
  2 : optional i32 soul_id_;                          // 英灵ID   (IN)
  3 : optional bool appear_;                          // 是否幻化 (IN)
  4 : required list<SoulFields> souls_;               // 结果     (OUT)
}



//////////////////////////////////
// 获取玩家能量状态
// type: REQUEST_SOUL_GET_ENERGY_STATUS
struct RequestSoulGetEnergyStatus {
  1 : required i64 actor_;                            // 玩家ID (IN)
  2 : soul.SoulEnergyStatus status_;                  // 状态   (OUT)
}

