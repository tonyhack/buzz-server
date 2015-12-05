include "entity.thrift"
include "gateway_protocol.thrift"


namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway



////////////////////////////////////////////////////
// 场景中出入视野时发送
// 玩家上线的时发送给自己的
struct BuffField {
  1 : required i32 buff_;                         // BUFF
  2 : required i32 left_time_;                    // 剩余时间
  3 : required i32 stack_count_;                  // 叠加层数
}
struct BuffRoleField {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required string id_;                        // 角色ID
  3 : required list<BuffField> buffs_;            // BUFF列表
}

// MESSAGE_BUFF_INITIALIZE
// dir: gateway->client
struct MessageBuffInitialize {
  1 : required list<BuffRoleField> roles_;        // BUFF角色数据
}




////////////////////////////////////////////////////
// 增加一个BUFF
// MESSAGE_ADD_BUFF
// dir: gateway->client
struct MessageAddBuff {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required string id_;                        // 角色ID
  3 : required i32 buff_;                         // Buff
  4 : required i32 stack_count_;                  // 层数
}



////////////////////////////////////////////////////
// 删除一个BUFF
// MESSAGE_REMOVE_BUFF
// dir: gateway->client
struct MessageRemoveBuff {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required string id_;                        // 角色ID
  3 : required i32 buff_;                         // Buff
}

