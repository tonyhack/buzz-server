include "gateway_protocol.thrift"
include "item.thrift"
include "equipment.thrift"
include "gem.thrift"
include "vocation.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway



//////////////////////////////////////////////
// 道具容器初始化
// type: MESSAGE_ITEM_CONTAINER_INITIALIZE
// dir: gateway->protocol
struct MessageItemContainerInitialize {
  1 : required item.ItemContainerType container_type_;          // 道具容器类型
  2 : optional vocation.VocationType vocation_;                 // 容器职业类型
  3 : required list<item.ItemFields> items_;                    // 道具列表
}




//////////////////////////////////////////////
// 创建道具同步
// type: MESSAGE_ITEM_CREATE_SYNCHRONIZE
// dir: gateway->protocol
struct MessageItemCreateSynchronize {
  1 : required string id_;                                      // ID
  2 : required i32 template_id_;                                // 模板ID
  3 : required item.ItemType type_;                             // 道具类型
  4 : required list<i32> item_client_fields_;                   // 道具基础属性
  5 : optional list<i32> equipment_client_fields_;              // 装备属性
  6 : optional string first_equip_time_;                        // 首次装备时间
}




//////////////////////////////////////////////
// 道具属性更新
// type: MESSAGE_SYNCHRONIZE_ITEM
// dir: gateway->client
struct MessageSynchronizeItem {
  1 : required string item_id_;                                         // 道具ID
  2 : optional list<item.ItemClientField> item_client_fields_;          // Item中的属性
  3 : optional list<equipment.EquipClientField> equip_client_fields_;   // Equipment中的属性
}



//////////////////////////////////////////////
enum ContainerOperateType {
  ADD = 0,              // 增加
  REMOVE,               // 删除
  
  MAX,
}

// 道具容器操作
// type: MESSAGE_ITEM_CONTAINER_OPERATE
// dir: gateway->client
struct MessageItemContainerOperate {
  1 : required item.ItemContainerType container_type_;          // 道具容器类型
  2 : required i16 location_;                                   // 容器内的位置
  3 : optional vocation.VocationType vocation_;                 // 容器职业类型(装备栏有效)
  4 : required ContainerOperateType operate_;                   // 操作类型
  5 : required string item_id_;                                 // 道具ID
}



//////////////////////////////////////////////
// 移动容器内的道具
// type: MESSAGE_ITEM_MOVE
// dir: client->gateway, gateway->client
struct MessageItemMove {
  1 : required string item_id_;                                 // 物品ID
  2 : required item.ItemContainerType src_container_;           // 源容器类型
  3 : required i16 src_location_;                               // 源位置
  4 : optional vocation.VocationType src_vocation_;             // 源容器职业类型(装备栏有效)
  5 : required item.ItemContainerType dest_container_;          // 目的容器类型
  6 : required i16 dest_location_;                              // 目的位置
  7 : optional vocation.VocationType dest_vocation_;            // 目的容器职业类型(装备栏有效)
}



//////////////////////////////////////////////
// 丢弃容器内的道具
// type: MESSAGE_ITEM_REMOVE
// dir: client->gateway
struct MessageItemRemove {
  1 : required string item_id_;
  2 : required item.ItemContainerType container_;               // 源容器类型
  3 : required i16 location_;                                   // 源位置
  4 : optional vocation.VocationType vocation_;                 // 源容器职业类型(装备栏有效)
}



//////////////////////////////////////////////
// 使用容器内的道具
// type: MESSAGE_ITEM_USE
// dir: client->gateway
struct MessageItemUse {
  1 : required string item_id_;
  2 : required item.ItemContainerType container_;               // 源容器类型
  3 : required i16 location_;
  4 : required i32 number_;                                     // 使用的数目
}



//////////////////////////////////////////////
// 拆分容器内的道具
// type: MESSAGE_ITEM_SPLIT
// dir: client->gateway
struct MessageItemSplit {
  1 : required string item_id_;                                 // 道具ID
  2 : required i16 src_location_;                               // 拆分道具的位置
  3 : required i16 dest_location_;                              // 拆分到的位置
  4 : required i32 number_;                                     // 拆出的个数
}



//////////////////////////////////////////////
// 整理容器内的道具
// type: MESSAGE_ITEM_ARRANGE
// dir: client->gateway
struct MessageItemArrange {
  1 : required item.ItemContainerType container_;               // 容器类型
}



//////////////////////////////////////////////
// 整理容器后的结果
// type: MESSAGE_ITEM_ARRANGE_RESULT
// dir: client->gateway
struct MessageItemArrageResult {
  1 : required list<string> items_;                             // 排序结果
}



//////////////////////////////////////////////
// 扩展容器格子
// type: MESSAGE_EXTEND_CONTAINER
// dir: client->gateway
struct MessageExtendContainer {
  1 : required item.ItemContainerType container_;               // 容器类型
  2 : required i32 extend_number_;                              // 扩展的数量
  3 : required byte cost_type_;                                 // 消费方式 1: 钻石(人民币) 2: 礼券(绑定的人民币)
}



//////////////////////////////////////////////
// 道具合成请求
// type: MESSAGE_ITEM_COMPOSE_REQUEST
// dir: client->gateway
struct MessageItemComposeRequest {
  1 : required i32 recipe_id_;                                  // 配方ID
  2 : optional list<string> item_ids_;                          // 道具ID(合成材料包含装备时使用)
  3 : optional i32 number_;                                     // 合成数量
  4 : optional bool bind_;                                      // 是否使用绑定材料
}



//////////////////////////////////////////////
// 道具合成回复
// type: MESSAGE_ITEM_COMPOSE_RESPONSE
// dir: gateway->client
struct MessageItemComposeResponse {
  1 : required bool result_;                                    // 结果
}



//////////////////////////////////////////////
struct EquipInformation {
  1 : required item.EquipmentType location_;                    // 装备位置
  2 : required i32 template_id_;                                // 装备模板ID
  3 : required byte intensify_;                                 // 强化等级
}

struct ActorEquipInformation {
  1 : required string actor_id_;                                // 玩家ID
  2 : required list<EquipInformation> equips_;                  // 装备信息
}

// 创建玩家装备广播(广播给周围玩家)
// type: MESSAGE_CREATE_ACTOR_EQUIP_BROADCAST
// dir: gateway->client
struct MessageCreateActorEquipBroadcast {
  1 : required list<ActorEquipInformation> actors_;             // 玩家列表
}



//////////////////////////////////////////////
// 更新装备广播
// type: MESSAGE_UPDATE_ACTOR_EQUIP_BROADCAST
// dir: gateway->client
struct MessageUpdateActorEquipBroadcast {
  1 : required string actor_id_;                                // 玩家ID
  2 : required EquipInformation equip_;                         // 装备信息
}



//////////////////////////////////////////////
// 装备栏与玩家基本信息对比请求
// type: MESSAGE_EQUIP_COMPARE_REQUEST
// dir: client->server
struct MessageEquipCompareRequest {
  1 : required string actor_id_;                                // 玩家ID
}



//////////////////////////////////////////////
// 装备栏与玩家基本信息对比回应
// type: MESSAGE_EQUIP_COMPARE_RESPONSE
// dir: server->client
struct CompareEquip {
  1 : required i32 template_id_;                                // 装备模板ID
  2 : required list<i32> fields_;                               // 装备属性
}
struct MessageEquipCompareResponse {
  1 : required string actor_id_;                                // 玩家ID
  2 : required string name_;                                    // 名称
  3 : required string guild_name_;                              // 公会名
  4 : required i32 level_;                                      // 等级
  5 : required i32 vocation_;                                   // 职业
  6 : required i32 gender_;                                     // 性别
  7 : required i32 appearance_;                                 // 外貌(0:头像, 1:发型)
  8 : required i32 current_hp_;                                 // 当前生命值
  9 : required i32 max_hp_;                                     // 最大生命值
  10: required i32 current_mp_;                                 // 当前魔法值
  11: required i32 max_mp_;                                     // 最大魔法值
  12: required i32 physique_;                                   // 体质
  13: required i32 strength_;                                   // 力量
  14: required i32 intelligence_;                               // 智力
  15: required i32 agile_;                                      // 敏捷
  16: required i32 spirit_;                                     // 精神
  17: required i32 physics_attack_;                             // 物理攻击
  18: required i32 physics_defence_;                            // 物理防御
  19: required i32 magic_attack_;                               // 魔法攻击
  20: required i32 magic_defence_;                              // 魔法防御
  21: required i32 dodge_;                                      // 闪避等级
  22: required i32 crit_;                                       // 暴击等级
  23: required i32 ignore_defence_;                             // 会心等级(忽视防御)
  24: required i32 speed_;                                      // 移动速度
  25: required i32 current_exp_;                                // 当前经验
  26: required i32 nimbus_;                                     // 灵气
  27: required i32 task_layer_;                                 // 任务纹章
  28: required i32 spirit_facade_;                              // 精灵外观
  29: required list<i32> sub_fight_scores_;                     // 子战斗力
  30: required string functionality_state_;                     // 系统功能开启状态
  31: required list<CompareEquip> equipments_;                  // 装备信息
  32: required i32 title_;                                      // 称号ID(成就ID)
}



//////////////////////////////////////////////
// 装备强化请求
// type: MESSAGE_EQUIP_INTENSIFY_REQUEST
// dir: client->gateway
struct MessageEquipIntensifyRequest {
  1 : required string id_;                                      // 装备ID
  2 : required item.ItemContainerType container_;               // 容器类型
  3 : required i16 location_;                                   // 位置
  4 : optional vocation.VocationType vocation_;                 // 源容器职业类型(装备栏有效)
  5 : required bool spend_freedom_item_;                        // 是否消耗非绑定道具
}



//////////////////////////////////////////////
// 装备强化回应
// type: MESSAGE_EQUIP_INTENSIFY_RESPONSE
// dir: gateway->client
struct MessageEquipIntensifyResponse {
  1 : required bool result_;                                    // 是否成功
}



//////////////////////////////////////////////
// 装备洗练请求
// type: MESSAGE_EQUIP_REFINE_REQUEST
// dir: client->gateway
struct MessageEquipRefineRequest {
  1 : required item.ItemContainerType container_;               // 容器类型
  2 : required i16 location_;                                   // 位置
  3 : optional vocation.VocationType vocation_;                 // 容器职业类型(装备栏有效)
  4 : required bool lock1_;                                     // 是否加锁
  5 : required bool lock2_;                                     // 是否加锁
  6 : required bool lock3_;                                     // 是否加锁
  7 : required bool lock4_;                                     // 是否加锁
  8 : required bool lock5_;                                     // 是否加锁
  9 : required bool lock6_;                                     // 是否加锁
  10: required bool spend_freedom_item_;                        // 是否消耗非绑定道具
}



//////////////////////////////////////////////
// 装备洗练回应
// type: MESSAGE_EQUIP_REFINE_RESPONSE
// dir: gateway->client
struct MessageEquipRefineResponse {
  1 : required bool ret_ok_;                                    // 是否成功
  2 : required string id_;                                      // 装备ID
  3 : required byte new_random_type1_;                          // 装备随机属性1
  4 : required i32 new_random_value1_;
  5 : required byte new_random_type2_;
  6 : required i32 new_random_value2_;
  7 : required byte new_random_type3_;
  8 : required i32 new_random_value3_;
  9 : required byte new_random_type4_;
  10: required i32 new_random_value4_;
  11: required byte new_random_type5_;
  12: required i32 new_random_value5_;
  13: required byte new_random_type6_;
  14: required i32 new_random_value6_;
}


//////////////////////////////////////////////
// 装备洗练玩家确认
// type: MESSAGE_EQUIP_REFINE_CONFIRM_REQUEST
// dir: client->gateway
struct MessageEquipRefineConfirmRequest {
  1 : required bool ok_;                                        // 是否放弃当前随机的属性
  2 : required item.ItemContainerType container_;               // 容器类型
  3 : required i16 location_;                                   // 位置 
  4 : optional vocation.VocationType vocation_;                 // 容器职业类型(装备栏有效)
}

/////////////////////////////////////////////
// 装备洗练玩家确认返回
// type: MESSAGE_EQUIP_REFINE_CONFIRM_RESPONSE
// dir: gateway->client
struct MessageEquipRefineConfirmResponse {
  1 : required bool ok_;
  2 : required string id_;                                      // 装备ID
}

//////////////////////////////////////////////
// 装备升阶请求
// type: MESSAGE_EQUIP_UPGRADE_REQUEST
// dir: client->gateway
struct MessageEquipUpgradeRequest {
  1 : required string id_;                                      // 装备ID
  2 : required item.ItemContainerType type_;                    // 容器类型
  3 : required i16 location_;                                   // 容器位置
  4 : optional vocation.VocationType vocation_;                 // 容器职业类型(装备栏有效)
  5 : required bool spend_freedom_item_;                        // 是否使用非绑定道具
}



//////////////////////////////////////////////
// 装备升阶回应
// type: MESSAGE_EQUIP_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageEquipUpgradeResponse {
  1 : required bool result_;                                    // 结果
  2 : optional string id_;                                      // 装备ID
}



//////////////////////////////////////////////
// 装备继承请求
// type: MESSAGE_EQUIP_INHERIT_REQUEST
// dir: client->gateway
struct MessageEquipInheritRequest {
  1 : required item.ItemContainerType src_type_;                // 源装备容器类型
  2 : required i16 src_location_;                               // 源装备位置
  3 : optional vocation.VocationType src_vocation_;             // 源装备容器职业类型(装备栏有效)
  4 : required item.ItemContainerType dest_type_;               // 目标装备容器类型
  5 : required i16 dest_location_;                              // 目标装备位置
  6 : optional vocation.VocationType dest_vocation_;            // 目标装备容器职业类型(装备栏有效)
}



//////////////////////////////////////////////
// 装备继承回复
// type: MESSAGE_EQUIP_INHERIT_RESPONSE
// dir: gateway->client
struct MessageEquipInheritResponse {
  1 : required bool result_;                                    // 结果
}



//////////////////////////////////////////////
// 宝石数据
struct GemData {
  1 : required gem.GemLocationType location_type_;              // 宝石位置类型
  2 : required i16 location_index_;                             // 宝石位置索引
  3 : required i32 id_;                                         // 宝石ID
  4 : required i32 level_;                                      // 宝石等级
  5 : required i32 exp_;                                        // 宝石经验
  6 : required bool locked_;                                    // 宝石是否锁定
}



//////////////////////////////////////////////
// 宝石数据初始化
// type: MESSAGE_GEM_INITIALIZE
// dir: gateway->client
struct MessageGemInitialize {
  1 : required list<GemData> gems_;                             // 宝石数据
  2 : required i32 gathering_point_;                            // 当前激活点
}



//////////////////////////////////////////////
// 宝石操作类型
enum GemOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 宝石数据同步
// type: MESSAGE_GEM_SYNCHRONIZE
// dir: gateway->client
struct MessageGemSynchronize {
  1 : required GemOperateType type_;                            // 操作类型
  2 : required GemData gem_;                                    // 宝石数据
}



//////////////////////////////////////////////
// 宝石聚集请求
// type: MESSAGE_GEM_GATHER_REQUEST
// dir: client->gateway
struct MessageGemGatherRequest {
}



//////////////////////////////////////////////
// 宝石聚集回复
// type: MESSAGE_GEM_GATHER_RESPONSE
// dir: server->gateway
struct MessageGemGatherResponse {
  1 : required bool result_;                                    // 是否成功
  2 : optional i32 gathering_point_;                            // 当前激活点
  3 : optional i32 bonus_id_;                                   // 奖励ID
  4 : optional i32 bonus_count_;                                // 奖励数量
}



//////////////////////////////////////////////
// 宝石略过激活点请求
// type: MESSAGE_GEM_SKIP_GATHERING_POINT_REQUEST
// dir: client->gateway
struct MessageGemSkipGatheringPointRequest {
}



//////////////////////////////////////////////
// 宝石略过激活点回复
// type: MESSAGE_GEM_SKIP_GATHERING_POINT_RESPONSE
// dir: gateway->client
struct MessageGemSkipGatheringPointResponse {
  1 : required bool result_;                                    // 是否成功
  2 : optional i32 gathering_point_;                            // 当前激活点
}



//////////////////////////////////////////////
// 宝石移动请求
// type: MESSAGE_GEM_MOVE_REQUEST
// dir: client->gateway
struct MessageGemMoveRequest {
  1 : required gem.GemLocationType src_location_type_;          // 源宝石位置类型
  2 : required i16 src_location_index_;                         // 源宝石位置索引
  3 : required gem.GemLocationType dest_location_type_;         // 目标宝石位置类型
  4 : required i16 dest_location_index_;                        // 目标宝石位置索引
}



//////////////////////////////////////////////
// 宝石移动回复
// type: MESSAGE_GEM_MOVE_RESPONSE
// dir: gateway->client
struct MessageGemMoveResponse {
  1 : required bool result_;                                    // 是否成功
}



//////////////////////////////////////////////
// 宝石提升请求
// type: MESSAGE_GEM_PROMOTE_REQUEST
// dir: client->gateway
struct MessageGemPromoteRequest {
  1 : required gem.GemLocationType location_type_;              // 宝石位置类型
  2 : required i16 location_index_;                             // 宝石位置索引
}



//////////////////////////////////////////////
// 宝石提升回复 
// type: MESSAGE_GEM_PROMOTE_RESPONSE
// dir: gateway->client
struct MessageGemPromoteResponse {
  1 : required bool result_;                                    // 是否成功
}



//////////////////////////////////////////////
// 宝石拆解请求
// type: MESSAGE_GEM_DECOMPOSE_REQUEST
// dir: client->gateway
struct MessageGemDecomposeRequest {
}



//////////////////////////////////////////////
// 宝石拆解回复
// type: MESSAGE_GEM_DECOMPOSE_RESPONSE
// dir: gateway->client
struct MessageGemDecomposeResponse {
  1 : required bool result_;                                    // 是否成功
  2 : optional i32 shard_count_;                                // 拆解得到的宝石碎片数量
}



//////////////////////////////////////////////
// 宝石锁定请求
// type: MESSAGE_GEM_LOCK_REQUEST
// dir: client->gateway
struct MessageGemLockRequest {
  1 : required gem.GemLocationType location_type_;              // 宝石位置类型
  2 : required i16 location_index_;                             // 宝石位置索引
  3 : required bool locked_;                                    // 是否锁定
}



//////////////////////////////////////////////
// 宝石锁定回复
// type: MESSAGE_GEM_LOCK_RESPONSE
// dir: gateway->client
struct MessageGemLockResponse {
  1 : required bool result_;                                    // 是否成功
}



//////////////////////////////////////////////
// 宝石全部锁定请求
// type: MESSAGE_GEM_LOCK_ALL_REQUEST
// dir: client->gateway
struct MessageGemLockAllRequest {
  3 : required bool locked_;                                    // 是否锁定
}



//////////////////////////////////////////////
// 宝石全部锁定回复
// type: MESSAGE_GEM_LOCK_ALL_RESPONSE
// dir: gateway->client
struct MessageGemLockAllResponse {
  1 : required bool result_;                                    // 是否成功
}



//////////////////////////////////////////////
// 道具快捷栏初始化
// type: MESSAGE_SHORTCUT_INITIALIZE
// dir: gateway->client
struct MessageShortcutInitialize {
  1 : required list<i32> shortcuts_;                            // 初始化快捷设置
}



//////////////////////////////////////////////
// 设置道具快捷栏请求
// type: MESSAGE_SHORTCUT_SET_REQUEST
// dir: client->gateway
struct MessageShortcutSetRequest {
  1 : required i32 index_;                                      // 索引(目前是0~2)
  2 : required i32 item_template_;                              // 快捷设置
}



//////////////////////////////////////////////
// 设置道具快捷栏回应
// type: MESSAGE_SHORTCUT_SET_RESPONSE
// dir: gateway->client
struct MessageShortcutSetResponse {
  1 : required bool result_;                                    // 是否成功
  2 : required i32 index_;                                      // 索引(目前是0~2)
  3 : required i32 item_template_;                              // 快捷设置
}


///////////////////////////////
// VIP 礼包领取
// type: MESSAGE_VIP_GIFTS_PICK_REQUEST
// dir: client->gateway
struct MessageVipGiftsPickRequest {
}


///////////////////////////////
// VIP 礼包领取响应
// type: MESSAGE_VIP_GIFTS_PICK_RESPONSE
// dir: gateway->client
struct MessageVipGiftsPickResponse {
  1 : required bool result_;                                    // 结果
}


////////////////////////////////////////////////
// vip 礼包数据同步
// type: MESSAGE_VIP_GIFTS_SYNCHRONIZE
// dir: gateway->client
struct MessageVipGiftsSynchronize {
  1 : required bool status_;                                    // true : 可以领取 false : 不可领取
}

///////////////////////////////////////////////
// 花费类型
enum TreasureSpendType {
  DIAMOND = 1,                                                  // 钻石
  GOLD,                                                         // 金币
}

///////////////////////////////////////////////
// 请求寻宝
// type: MESSAGE_ITEM_TREASURE_HUNT_REQUEST                     // 寻宝请求
// dir: client->gateway
struct MessageItemTreasureHuntRequest {
  1 : required i32 num_;                                        // 抽奖次数
  2 : required i32 tab_id_;                                     // 标签页(从0开始)
}

///////////////////////////////////////////////
// 寻宝响应
// type: MESSAGE_ITEM_TREASURE_HUNT_RESPONSE
// dir: gateway->client
struct MessageItemTreasureHuntResponse {
  1 : required i32 tab_id_;                                     // 标签页(从0开始)
  2 : required list<i32> template_id_list_;                     // 抽到的道具ID
  3 : required list<i32> rand_pos_;                             // 随机出的位置
}


///////////////////////////////////////////////
// 寻宝公告
// type: MESSAGE_ITEM_TREASURE_HUNT_NOTICE
// dir: gateway->client
struct MessageItemTreasureHuntNotice {
  1 : required list<string> notices_;                           // 寻宝公告列表(参数用','分割)
}
