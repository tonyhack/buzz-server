#ifndef __GLOBAL__CONFIGURE__MISC__CONFIGURE__H
#define __GLOBAL__CONFIGURE__MISC__CONFIGURE__H

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace global {

namespace configure {

class MiscConfigure : public core::Noncopyable {
 public:
  typedef std::vector<core::int32> WorldBossTimeVecs;

  MiscConfigure();
  ~MiscConfigure();

  bool LoadConfigure(const std::string &file);

  // 移动速度
  // --最小速度值
  core::int32 move_speed_min_;
  // --最大速度值
  core::int32 move_speed_max_;

  // 默认技能
  core::int32 axe_skill_;
  core::int32 axe_skill_level_;
  core::int32 shoot_skill_;
  core::int32 shoot_skill_level_;
  core::int32 magic_skill_;
  core::int32 magic_skill_level_;
  
  // 新手保护
  // --新手保护等级
  core::int32 rookie_protect_level_;

  // 死亡设定
  // --死亡保护buff_id
  core::int32 protect_buff_id_;
  // --PK惩罚buff_id
  core::int32 pk_buff_id_;
  // --复活道具id
  core::uint32 revive_item_id_;

  // 名字颜色
  // --褐名时间
  core::int32 brown_time_;

  // 英灵祭坛
  // --每日免费召唤次数
  core::int32 soul_altar_max_free_summon_times_;
  // --召唤消耗道具ID
  core::uint32 soul_altar_summon_spend_item_id_;
  // --召唤消耗道具数量
  core::int32 soul_altar_summon_spend_item_count_;
  // --收取第二张英灵卡消耗钻石
  core::int32 soul_altar_second_card_spend_freedom_dollars_;
  // --收取第三张英灵卡消耗钻石
  core::int32 soul_altar_third_card_spend_freedom_dollars_;
  // --英灵祭坛掉落组1 ID
  core::int32 soul_altar_drop_group1_id_;
  // --英灵祭坛掉落组2 ID
  core::int32 soul_altar_drop_group2_id_;

  // 容器开格子
  core::int32 extend_container_gold_;
  core::int32 extend_container_lockedgold_;

  // 宝石
  // --宝石仓库的栏位数量
  core::int32 gem_max_storage_capacity_;
  // --宝石在装备上的栏位数量
  core::int32 gem_max_equip_capacity_;

  // 道具
  // --回购列表最大值
  core::uint32 buyback_capacity_;
  // --背包最小容量
  core::uint32 packet_min_capacity_;
  // --背包最大容量
  core::uint32 packet_max_capacity_;
  // --仓库最小容量
  core::uint32 storage_min_capacity_;
  // --仓库最大容量
  core::uint32 storage_max_capacity_;
  // --礼品仓库容量
  core::uint32 treasure_storage_capacity_;

  // 联系人
  // --最大好友数量
  core::int32 contacts_max_friend_capacity_;
  // --最大黑名单数量
  core::int32 contacts_max_blacklist_capacity_;
  // --最大签名长度
  core::int32 contacts_max_signature_length_;
  // --推荐等级划分范围
  core::int32 contacts_recommend_level_range_;
  // --推荐个数
  core::int32 contacts_recommend_count_;

  // 仇人
  // --最大仇人数量
  core::int32 social_enemy_max_capacity_;

  // 家园
  // --升级冷却时间上限(单位s)
  core::int32 homeland_max_upgrade_cooling_;
  // --单位升级冷却时间(单位s)
  core::int32 homeland_upgrade_cooling_unit_;
  // --清除单位升级冷却时间消耗的钻石数量
  core::int32 homeland_clear_upgrade_cooling_unit_freedom_dollars_;
  // --单位神殿收获冷却时间(单位s)
  core::int32 homeland_temple_harvest_cooling_unit_;
  // --清除单位神殿收获冷却时间消耗的钻石数量
  core::int32 homeland_clear_temple_harvest_cooling_unit_freedom_dollars_;
  // --金矿掠夺最大次数
  core::int32 homeland_max_goldmine_rob_times_;
  // --金矿掠夺冷却时间(单位s)
  core::int32 homeland_goldmine_rob_cooling_;
  // --祈福堂免费祈福最大次数
  core::int32 homeland_max_church_free_pray_times_;
  // --祈福堂钻石祈福最大次数
  core::int32 homeland_max_church_paid_pray_times_;
  // --祈福堂钻石祈福消耗的钻石数量
  core::int32 homeland_church_paid_pray_spend_freedom_dollars_;
  // --祈福堂刷新消耗的钻石数量
  core::int32 homeland_church_refresh_spend_freedom_dollars_;
  // --祈福堂高级刷新消耗的钻石数量
  core::int32 homeland_church_vip_refresh_spend_freddom_dollars_;
  // --祈福堂免费刷新最大次数
  core::int32 homeland_max_church_free_refresh_times_;
  // --每天给好友浇水(可获得奖励)的最大次数
  core::int32 homeland_max_tree_assist_water_times_;
  // --给好友浇水获得的礼券数量
  core::int32 homeland_tree_assist_water_restrict_dollars_reward_;
  // --军团据点支援最大次数
  core::int32 homeland_max_stronghold_support_times_;
  // --军团据点支援冷却时间(单位s)
  core::int32 homeland_stronghold_support_cooling_;
  // --军团据点支援获得的军团贡献数量
  core::int32 homeland_stronghold_support_guild_contribution_reward_;
  // --入侵者最大数量
  core::int32 homeland_max_invader_capacity_;
  // --告示板最大信息数量
  core::int32 homeland_max_event_capacity_;
  // --家园攻防主将战最大回合数
  core::int32 homeland_max_commander_battle_round_; 
  // --家园攻防英灵战最大回合数
  core::int32 homeland_max_soul_battle_round_;
  // --家园攻防每次参加防御的支援英灵上限
  core::int32 homeland_max_battle_support_soul_;
  // --家园攻防战血量buff系数
  core::int32 homeland_battle_hp_buff_coe_;
  // --家园攻防战攻击buff系数
  core::int32 homeland_battle_attack_buff_coe_;

  // 公会
  // --公会名最大长度
  core::int32 guild_max_name_length_;
  // --创建公会消耗金币
  core::int32 guild_create_spend_gold_;
  // --创建公会时的起始公会金币数量(万)
  core::int32 guild_init_gold_;
  // --最多同时申请的公会数量
  core::int32 guild_max_join_capacity_;
  // --最大待审核成员数
  core::int32 guild_max_pending_member_capacity_;
  // --公告最大长度
  core::int32 guild_max_announcement_length_;
  // --最大事件数量
  core::int32 guild_max_event_capacity_;
  // --光晶石道具ID
  core::uint32 guild_light_crystal_item_id_;
  // --暗晶石道具ID
  core::uint32 guild_dark_crystal_item_id_;
  // --圣晶石道具ID
  core::uint32 guild_holy_crystal_item_id_;
  // --捐献1万金币得到的贡献值
  core::int32 guild_gold_unit_contribution_;
  // --捐献1钻石得到的贡献值
  core::int32 guild_freedom_dollars_unit_contribution_;
  // --捐献1钻石增加的公会金币数量
  core::int32 guild_freedom_dollars_unit_gold_;
  // --捐献1光晶石得到的贡献值
  core::int32 guild_light_crystal_unit_contribution_; 
  // --捐献1暗晶石得到的贡献值
  core::int32 guild_dark_crystal_unit_contribution_;
  // --捐献1圣晶石得到的贡献值
  core::int32 guild_holy_crystal_unit_contribution_;
  // --清空公会buff的时间(单位s)
  core::int32 guild_clear_buff_time_;

  // 精灵
  // -- 精灵变身进度条ID
  core::int32 spirit_bar_id_;

  // 掉落
  // -- 掉落道具消失时间
  core::int32 drop_item_disappear_secs_;
  core::int32 drop_item_clear_owner_secs_;
  core::int32 drop_pickup_distance_;
  // --场景掉落物最大可拾取人数
  core::int32 drop_item_owner_max_;

  // 任务
  // -- 新手给的第一个主线任务
  core::uint32 new_actor_task_;
  // -- 最大圆半径内玩家都完成召唤任务
  core::int32 summon_radius_;
  // -- 最大圆半径内玩家都完成召唤任务最多人数
  core::int32 summon_num_;

  // 世界跳转
  // -- 世界跳转消耗的道具ID
  core::int32 world_jump_expend_item_id_;
  // -- 世界跳转消耗的道具数量
  core::int32 world_jump_expend_item_num_;

  // VIP功能
  //  -- 无限飞次数
  core::int32 infinite_num_;

  // 拍卖
  // --最小拍卖起始价格
  core::int32 auction_min_start_bid_price_;
  // --最小一口价
  core::int32 auction_min_buyout_price_;
  // --最小竞价加价金额(百分之)
  core::int32 auction_min_bid_increment_percent_;
  // --交易手续费(百分之)
  core::int32 auction_transaction_fee_percent_;
  // --玩家拍卖道具最大数量
  core::int32 auction_max_sell_item_capacity_;
  // --玩家竞拍道具最大数量
  core::int32 auction_max_buy_item_capacity_;
  // --历史记录最大数量
  core::int32 auction_max_history_capacity_;
  // --每页道具数量
  core::int32 auction_item_per_page_;

  // 福利
  // --补签到次数
  core::int32 replenish_checkins_num_;
  // --每日在线奖励等级段1
  core::int32 online_time_level1_;
  // --每日在线奖励等级段1奖励
  core::int32 online_time_award1_;
  // --每日在线奖励等级段2
  core::int32 online_time_level2_;
  // --每日在线奖励等级段2奖励
  core::int32 online_time_award2_;
  // --每日在线奖励等级段3奖励
  core::int32 online_time_award3_;
  // --每个等级段可以计算的时间
  core::int32 max_online_time_;
  // --领取上月的奖励最迟天数
  core::int32 pick_awards_limit_time_;

  // 资源
  // --灵气上限
  core::int32 max_nimbus_;

  // 地宫
  // --最大层数
  core::int32 maze_max_floor_;
  // --宽(格子)
  core::int32 maze_width_;
  // --高(格子)
  core::int32 maze_height_;
  // --行动点最大值
  core::int32 maze_max_action_point_;
  // --行动点回复冷却时间(秒)
  core::int32 maze_action_point_recover_cooling_;
  // --重置时间
  core::int32 maze_reset_time_week_day_;
  core::int32 maze_reset_time_hour_;
  core::int32 maze_reset_time_minute_;
  core::int32 maze_reset_time_second_;
  // --礼包道具ID
  core::uint32 maze_gift_item_id_;

  // 防沉迷
  // --收益减半累计在线时间(秒)
  core::int32 fcm_half_income_online_time_;
  // --零收益累计在线时间(秒)
  core::int32 fcm_zero_income_online_time_;
  // --清除防沉迷状态需要累计下线时间(秒)
  core::int32 fcm_clear_status_offline_time_;

  // 能量
  // --能量恢复间隔
  core::int32 soul_energy_interval_msecs_;
  // --能量单位时间减缩值
  core::int32 soul_energy_decrement_;
  // --能量爆发BUFF
  core::int32 soul_energy_explosion_buff_;
  // --杀死怪物获得的能量值
  core::int32 soul_energy_kill_npc_energy_;
  // --杀死怪物获得能量值的等级差
  core::int32 soul_energy_kill_npc_level_diff_;
  // --杀死玩家获得的能量值
  core::int32 soul_energy_kill_actor_energy_;
  // --杀死玩家获得能量值的等级差
  core::int32 soul_energy_kill_actor_level_diff_;

  // 场景
  // --场景世界boss召唤时间
  WorldBossTimeVecs worldboss_appear_time_vecs_;
  // --连斩时NPC与玩家等级差值才累加连斩次数
  core::int32 consecutive_kill_level_;

  // 护符
  // --血护符自动使用间隔时间
  core::int32 hp_talisman_auto_span_time_;
  // --蓝护符自动使用间隔时间
  core::int32 mp_talisman_auto_span_time_;
  // --血护符血量百分比时自动加血
  core::int32 hp_under_percent_;
  // --蓝护符血量百分比时自动加蓝
  core::int32 mp_under_percent_;
  // --血护符上限
  core::int32 hp_talisman_max_value_;
  // --蓝护符上限
  core::int32 mp_talisman_max_value_;

  // 打坐
  // --打坐收益间隔时间
  core::int32 zazen_revenue_time_;

  // 副本
  // --房间类副本的准备时间
  core::int32 playing_team_ready_secs_;

  // 装备继承
  // --消耗道具ID
  core::uint32 equip_inherit_spend_item_id_;
  // --消耗道具数量
  core::int32 equip_inherit_spend_item_count_;
  // --装备最低等级
  core::int32 equip_inherit_min_level_;
  // --源装备要求最低品质
  core::uint32 equip_inherit_min_src_quality_;
  // --目标装备要求最低品质
  core::uint32 equip_inherit_min_dest_quality_;

  // 运送
  // --运送NPC删除定时器单位检测时间
  core::int32 transport_destory_check_sec_;
  // --运送NPC删除时间
  core::int32 transport_destory_sec_;

  // 运送任务
  // --每日运送接收任务的NPC
  core::int32 transport_task_accept_npc_;
  // --每日运送接受任务的NPC坐标x
  core::int32 transport_task_accept_npc_x_;
  // --每日运送接受任务的NPC坐标y
  core::int32 transport_task_accept_npc_y_;
  // --每日运送接受任务的NPC坐标地图map
  core::int32 transport_task_accept_npc_map_;
  // --每日运送完成任务的NPC
  core::int32 transport_task_finish_npc_;
  // --每日运送次数
  core::int32 transport_task_count_;
  // --每日运送抢劫次数
  core::int32 transport_task_rob_count_;
  // --每日运送刷新消耗
  core::int32 transport_task_refresh_spend_item_;
  // --每日运送默认护送ID
  core::int32 transport_task_default_id_;

  // 地图
  // --分线的最大数量
  core::int32 map_branch_max_number_;
  // --分线开启的人数
  core::int32 map_branch_max_actor_number_;
};

}  // namespace configure

}  // namespace global

#endif  // __GLOBAL__MISC__CONFIGURE__H

