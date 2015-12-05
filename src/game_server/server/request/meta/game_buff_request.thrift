include "game_request.thrift"
include "entity.thrift"

namespace cpp game.server.request
namespace py ccrequest.buff

//////////////////////////////////////////
// 效果创建者类型
// 这里描述的是非实体类型和实体类型
// 非实体类型罗列到以下 enum,
// 实体类型值为 EntityType::type + EffectCreatorType::TYPE_ENTITY
// type: REQUEST_BUFF_START
struct RequestBuffStart {
  1 : required entity.EntityType src_entity_type_;                // 创建者类型 (IN)
  2 : required i64 src_entity_id_;                                // 创建者ID   (IN)
  3 : required entity.EntityType dest_entity_type_;               // 宿主者类型 (IN)
  4 : required i64 dest_entity_id_;                               // 宿主者ID   (IN)
  5 : required i32 buff_id_;                                      // BUFF       (IN)
  6 : required bool succeed_;                                     // 是否成功   (OUT)
}


//////////////////////////////////////////
// 关闭一个效果
// type: REQUEST_BUFF_END
struct RequestBuffEnd {
  1 : required entity.EntityType dest_entity_type_;               // 宿主者类型 (IN)
  2 : required i64 dest_entity_id_;                               // 宿主者ID   (IN)
  3 : required i32 buff_id_;                                      // BUFF       (IN)
  4 : required bool succeed_;                                     // 是否成功   (OUT)
}


//////////////////////////////////////////
// 清除BUFF
// type: REQUEST_BUFF_CLEAR
struct RequestBuffClear {
  1 : required entity.EntityType type_;                           // 实体类型   (IN)
  2 : required i64 id_;                                           // 实体ID     (IN)
  3 : required i32 buff_id_;                                      // 驱散BUFFID   (IN)
  4 : required bool succeed_;                                     // 是否成功   (OUT)
}


//////////////////////////////////////////
// 清除BUFF组
// type: REQUEST_BUFF_CLEAR_GROUP
struct RequestBuffClearGroup {
  1 : required entity.EntityType type_;                           // 实体类型   (IN)
  2 : required i64 id_;                                           // 实体ID     (IN)
  3 : required i32 clear_group_;                                  // 驱散组ID   (IN)
  4 : required bool succeed_;                                     // 是否成功   (OUT)
}


//////////////////////////////////////////
// 设置禁止BUFF组
// type: REQUEST_BUFF_SET_FORBID_GROUP
struct RequestBuffSetForbidGroup {
  1 : required entity.EntityType type_;                           // 实体类型   (IN)
  2 : required i64 id_;                                           // 实体ID     (IN)
  3 : required i32 clear_group_;                                  // 驱散组ID   (IN)
  4 : required bool succeed_;                                     // 是否成功   (OUT)
}


//////////////////////////////////////////
// 重置禁止BUFF组
// type: REQUEST_BUFF_RESET_FORBID_GROUP
struct RequestBuffResetForbidGroup {
  1 : required entity.EntityType type_;                           // 实体类型   (IN)
  2 : required i64 id_;                                           // 实体ID     (IN)
  3 : required i32 clear_group_;                                  // 驱散组ID   (IN)
  4 : required bool succeed_;                                     // 是否成功   (OUT)
}


//////////////////////////////////////////
// 请求获取BUFF是否存在
// type: REQUEST_BUFF_GET_EXIST
struct RequestBuffGetExist {
  1 : required entity.EntityType type_;                           // 实体类型   (IN)
  2 : required i64 id_;                                           // 实体ID     (IN)
  3 : required i32 buff_id_;                                      // buff id    (IN)
  4 : required bool exist_;                                       // 是否存在   (OUT)
}

