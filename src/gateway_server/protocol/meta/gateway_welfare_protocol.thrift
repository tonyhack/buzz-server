include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


/////////////////////////////////////////
// 成就条件
struct AchieveCondition {
 1 : required i32 id_;                            // 成就ID
 2 : required list<i32> values_;                  // 成就值
 3 : required string finish_time_;                // 成就完成时间(0 未完成)
}

/////////////////////////////////////////
// 成就完成列表
struct AchieveFinish {
  1 : required i32 id_;                             // 成就ID
  2 : required string finish_time_;                 // 成就完成时间
}

/////////////////////////////////////////
// 初始化福利数据
// type: MESSAGE_WELFARE_INITILIZE
// dir: gateway->client
struct MessageWelfareInitilize {
  1 : required i32 title_id_;                        // 称号ID
  2 : required string service_time_;                 // 开服日期 
}


/////////////////////////////////////////
// 签到数据同步
// type: MESSAGE_CHECKINS_SYNCHRONIZATION
// dir: gateway->client
struct MessageCheckinsSynchronization {
  1 : required list<bool> checkins_;                 // 签到信息
  2 : required bool day_award_;                      // 签到当日奖励是否领取
  3 : required list<bool> general_awards_;           // 普通签到奖励是否领取
  4 : required list<bool> vip_awards_;               // VIP签到奖励是否领取
  5 : required list<i32> online_awrads_;             // 在线奖励是否领取
  6 : required bool resource_awards_;                // 上月的资源奖励是否领取
  7 : required i32 daily_online_time_;               // 每天在线时长
  8 : required i32 total_time_;                      // 总共在线时长
  9 : required i32 replenish_checkins_num_;          // 补签
  10 : required i32 month_resource_;                 // 本月礼金
  11 : required i32 last_month_resource_;            // 上月礼金
  12 : required list<i32> respenish_checkins_days_;  // 补签到天列表
}

/////////////////////////////////////////
// 初始化成就数据
// type: MESSAGE_ACHIEVE_INITILIZE
// dir: gateway->client
struct MessageAchieveInitilize {
  1 : required list<AchieveFinish> finishs_;         // 成就完成的列表
  2 : required list<AchieveCondition> conditions_;   // 成就正在进行的列表
}

/////////////////////////////////////////
// 成就数据同步
// type: MESSAGE_ACHIEVE_CONDITION_SYSNCHRONIZATION
// dir: gateway->client
struct MessageAchieveConditionSysnchronization {
  1 : required AchieveCondition condition_          // 成就条件列表
}

/////////////////////////////////////////
// 请求领取奖励
// type: MESSAGE_ACHIEVE_AWARD_REQUEST
// dir: client->gateway
struct MessageAchieveAwardRequest {
  1 : required i32 id_;                             // 成就ID
}

/////////////////////////////////////////
// 请求领取奖励返回
// type: MESSAGE_ACHIEVE_AWARD_RESPONSE
// dir: gateway->client
struct MessageAchieveAwardResponse {
  1 : required i32 id_;                             // 成就ID
}

/////////////////////////////////////////
// 请求称号
// type: MESSAGE_ACHIEVE_CHANGE_TITLE_REQUEST
// dir: client->gateway
struct MessageAchieveChangeTitleRequest {
  1 : required i32 id_;                              // 成就ID
}

/////////////////////////////////////////
// 请求称号
// type: MESSAGE_ACHIEVE_CHANGE_TITLE_RESPONSE
// dir: gateway->client
struct MessageAchieveChangeTitleResponse {
  1 : required i32 id_;                              // 成就ID
}

enum CheckinsOptType {
  DAILY_CHECKINS = 0,                                // 每日签到
  GENERAL_CHECKINS,                                  // 一般签到
  VIP_CHECKINS,                                      // VIP签到
  DAILY_CHECKINS_AWARD,                              // 每日签到领取奖励
}
////////////////////////////////////////
// 签到操作
// type: MESSAGE_CHECKINS_OPT_REQUEST
// dir:client->gateway
struct MessageCheckinsOptRequest {
  1 : required i32 days_;
  2 : required CheckinsOptType type_;               // 操作类型
}

////////////////////////////////////////
// 签到操作返回
// type: MESSAGE_CHECKINS_OPT_RESPONSE
// dir: gateway->client
struct MessageCheckinsOptResponse {
  1 : required i32 days_;
  2 : required CheckinsOptType type_;
}

////////////////////////////////////////
// 领取奖励的道具
// type: MESSAGE_ONLINE_AWARD_ITEM_REQUEST
// dir: client->gateway
struct MessageOnlineAwardItemRequest {
  1 : required i32 time_;                           // 时间
}

////////////////////////////////////////
// 领取奖励的道具返回
// type: MESSAGE_ONLINE_AWARD_ITEM_RESPONSE
// dir: client->gateway
struct MessageOnlineAwardItemResponse {
  1 : required i32 time_;                           // 时间
  2 : required i32 pos_;                            // 奖池下标
}

////////////////////////////////////////
// type: MESSAGE_ONLINE_AWARD_RESOURCE_REQUEST
// dir: client->gateway
struct MessageOnlineAwardResourceRequest {
}

////////////////////////////////////////
// type: MESSAGE_ONLINE_AWARD_ITEM_RESOURCE_RESPONSE
// dir: client->gateway
struct MessageOnlineAwardResourceResponse {
  1 : required bool result_;                        // 结果
}

///////////////////////////////////////
// 广播称号字段
struct MessageTitleFiled {
  1 : required string actor_id_;                    // 玩家ID
  2 : required i32 title_id_;                       // 称号ID
}

////////////////////////////////////////
// type: MESSAGE_WELFARE_TITLE_BROADCAST
// dir: gateway->client
struct MessageWelfareTitleBroadCast {
  1 : required list<MessageTitleFiled> fields_;     // 称号信息列表
}
