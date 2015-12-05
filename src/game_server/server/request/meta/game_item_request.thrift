include "game_request.thrift"
include "achieve.thrift"
include "item.thrift"

namespace cpp game.server.request
namespace py ccrequest.item

enum AddItemResult {
  SUCCESS = 0,          // 成功
  PACKET_FULL,          // 背包满
  ERROR_UNKNOWN,        // 未知
}

///////////////////////////////////////
// 增加道具
// type: REQUEST_ADD_ITEM
struct RequestAddItem {
  1 : required i64 actor_id_;                       // 玩家ID(IN)
  2 : required i32 item_template_                   // 道具模板(IN)
  3 : required i32 number_;                         // 数量(IN)
  4 : required item.BindRequireType bind_;          // 是否绑定(0:绑定，非0:不绑定)(IN)
  5 : optional i16 location_;                       // 道具位置(IN)
  6 : optional string item_extra_info_;             // 道具额外信息(IN)
  7 : optional bool simulate_ = false;              // 只进行检查, 不添加道具(IN)
  8 : required i32 intensify_level_;                // 强化等级(IN)
  9 : required AddItemResult result_;               // 结果(OUT)
}


///////////////////////////////////////
// 批量增加道具
// type: REQUEST_ADD_ITEMS
struct RequestAddItems {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : required list<item.AddItemFields> items_;     // 道具列表
  3 : optional bool simulate_ = false;              // 只进行检查, 不添加道具
  4 : required AddItemResult result_;               // 结果
}


///////////////////////////////////////
// 得到背包道具数量
// type: REQUEST_GET_PAKCET_ITEM_NUMBER
struct RequestGetPacketItemNumber {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : required i32 item_template_;                  // 道具模板ID
  3 : required item.BindRequireType bind_;          // 绑定需求(0:非绑定, 1:绑定, 2:混合)
  4 : required i32 number_;                         // 道具数量
}                                                
                                                 
                                                 
///////////////////////////////////////          
// 得到背包指定位置或者道具ID的模板ID和额外信息(可选)
// type: REQUEST_GET_PAKCET_ITEM_TEMPLATE        
struct RequestGetPacketItemTemplate {            
  1 : required i64 actor_id_;                       // 玩家ID(IN)
  2 : optional i64 item_id_;                        // 道具ID(IN)
  3 : optional i16 location_;                       // 道具位置(IN)
  4 : required i32 item_template_;                  // 道具模板ID(OUT)
  5 : required bool bind_;                          // 绑定属性(OUT)
  6 : optional string item_extra_info_;             // 道具额外信息(IN, OUT)
}                                                
                                                 
                                                 
///////////////////////////////////////          
// 删除道具                                      
// type: REQUEST_REMOVE_ITEM                     
struct RequestRemoveItem {                       
  1 : required i64 actor_id_;                       // 玩家ID
  2 : required i32 item_template_;                  // 道具模板ID
  3 : required i32 number_;                         // 数量
  4 : required item.BindRequireType bind_;          // 绑定需求(0:非绑定, 1:绑定, 2:混合)
  5 : required i32 remove_number_;                  // 实际删除的道具数量
}


enum RemoveResult {
  SUCCESS = 0,          // 成功
  ERROR_UNKNOWN,        // 未知
}

///////////////////////////////////////
// 删除道具(位置 or ID)
// type: REQUEST_REMOVE_ITEM_LOCATION_ID
struct RequestRemoveItemLocationID {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : optional string item_id_;                     // 道具id 
  3 : optional i16 location_;                       // 道具位置
  4 : required i32 number_;                         // 道具数量 
  5 : required RemoveResult result_;                // 结果
}

//////////////////////////////////////
// 获取装备栏信息
// type: REQUEST_ITEM_WEARED_EQUIP_INFO
struct RequestItemWearedEquipInfo {
  1 : required i64 actor_id_;                      // 玩家ID
  2 : required achieve.AchieveWearEquipType type_; // 穿装备统计方式
  3 : required i32 num_;                           // 数目(out)
  4 : required i32 param1_;                        // 参数(in)
}
