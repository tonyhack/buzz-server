include "entity.thrift"
include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.ai

///////////////////////////////////
// 请求增加仇恨值
// type: REQUEST_AI_ADD_THREAT
struct RequestAiAddThreat {
  1 : required i64 src_id_;                       // 原ID
  2 : required entity.EntityType src_type_        // 原类型
  3 : required i64 target_id_;                    // 目标ID
  4 : required entity.EntityType target_type_;    // 目标类型
  5 : required i32 threat_value_;                 // 仇恨值
}


///////////////////////////////////
// 请求设置目标
// type: REQUEST_AI_SET_TARGET
struct RequestAiSetTarget {
  1 : required i64 id_;                           // ID
  2 : required entity.EntityType type_;           // 类型
  3 : required i64 target_id_;                    // 目标ID
  4 : required entity.EntityType target_type_;    // 目标类型
  5 : required bool force_lock_ = false;          // 强制锁定目标
}


///////////////////////////////////
// 设置AI
// type: REQUEST_AI_SET_AI
struct RequestAiSetAi {
  1 : required i64 id_;                           // ID
  2 : required entity.EntityType type_;           // 类型
  3 : required i32 ai_;                           // AI
}

