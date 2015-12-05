include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


//////////////////////////////////////
// 护送AOI数据
struct TransportAoiData {
  1 : required string npc_id_;                      // npc的ID
  2 : required string master_name_;                 // 主人的名称
}


//////////////////////////////////////
// 护送AOI数据同步
// type: MESSAGE_TRANSPORT_AOI_SYNCHRONIZE
// dir: gateway->client
struct MessageTransportAoiSynchronize {
  1 : required list<TransportAoiData> aoi_data_;    // AOI数据
}


//////////////////////////////////////
// 护送数据初始化
// type: MESSAGE_TRANSPORT_INITIALIZE
// dir: gateway->client
struct MessageTransportInitialize {
  1 : required string npc_id_;                      // 当前护送NPC的ID
}


//////////////////////////////////////
// 护送数据同步
// type: MESSAGE_TRANSPORT_SYNCHRONIZE
// dir: gateway->client
struct MessageTransportSynchronize {
  1 : optional string npc_id_;                      // 当前护送NPC的ID
}

