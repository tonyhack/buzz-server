include "game_request.thrift"
include "role.thrift"
include "entity.thrift"

namespace cpp game.server.request
namespace py ccrequest.effect

// 开启一个效果
// type: REQUEST_EFFECT_START
struct RequestEffectStart {
  1 : required entity.EntityType entity_type_;      // 宿主者类型
  2 : required i64 entity_id_;                      // 宿主者ID
  3 : required i32 effect_id_;                      // 效果ID
  4 : required bool succeed_;                       // 是否成功
}

// 关闭一个效果
// type: REQUEST_EFFECT_END
struct RequestEffectEnd {
  1 : required entity.EntityType entity_type_;      // 宿主者类型
  2 : required i64 entity_id_;                      // 宿主者ID
  3 : required i32 effect_id_;                      // 效果ID
  4 : required bool succeed_;                       // 是否成功
}

// 开启角色普通状态效果
// type: REQUEST_EFFECT_COMMON_STATUS_START
struct RequestEffectCommonStatusStart {
  1 : required entity.EntityType entity_type_;      // 宿主者类型
  2 : required i64 entity_id_;                      // 宿主者ID
  3 : required role.RoleCommonStatus status_;       // 效果类型
  4 : required bool succeed_;                       // 是否成功
}

// 关闭角色普通状态效果
// type: REQUEST_EFFECT_COMMON_STATUS_END
struct RequestEffectCommonStatusEnd {
  1 : required entity.EntityType entity_type_;      // 宿主者类型
  2 : required i64 entity_id_;                      // 宿主者ID
  3 : required role.RoleCommonStatus status_;       // 效果类型
  4 : required bool succeed_;                       // 是否成功
}

