#include "global/configure/misc_configure.h"

#include <vector>

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/common_functions.h"

namespace global {

namespace configure {

MiscConfigure::MiscConfigure() :
  move_speed_min_(0),
  move_speed_max_(0),
  axe_skill_(0),
  axe_skill_level_(0),
  shoot_skill_(0),
  shoot_skill_level_(0),
  magic_skill_(0),
  magic_skill_level_(0),
  rookie_protect_level_(0),
  protect_buff_id_(0),
  pk_buff_id_(0),
  revive_item_id_(0),
  brown_time_(0),
  soul_altar_max_free_summon_times_(0),
  soul_altar_summon_spend_item_id_(0),
  soul_altar_summon_spend_item_count_(0),
  soul_altar_second_card_spend_freedom_dollars_(0),
  soul_altar_third_card_spend_freedom_dollars_(0),
  soul_altar_drop_group1_id_(0),
  soul_altar_drop_group2_id_(0),
  extend_container_gold_(0),
  extend_container_lockedgold_(0),
  gem_max_storage_capacity_(0),
  gem_max_equip_capacity_(0),
  buyback_capacity_(0),
  packet_min_capacity_(0),
  packet_max_capacity_(0),
  storage_min_capacity_(0),
  storage_max_capacity_(0),
  treasure_storage_capacity_(0),
  contacts_max_friend_capacity_(0),
  contacts_max_blacklist_capacity_(0),
  contacts_max_signature_length_(0),
  contacts_recommend_level_range_(0),
  contacts_recommend_count_(0),
  social_enemy_max_capacity_(0),
  homeland_max_upgrade_cooling_(0),
  homeland_upgrade_cooling_unit_(0),
  homeland_clear_upgrade_cooling_unit_freedom_dollars_(0),
  homeland_temple_harvest_cooling_unit_(0),
  homeland_clear_temple_harvest_cooling_unit_freedom_dollars_(0),
  homeland_max_goldmine_rob_times_(0),
  homeland_goldmine_rob_cooling_(0),
  homeland_max_church_free_pray_times_(0),
  homeland_max_church_paid_pray_times_(0),
  homeland_church_paid_pray_spend_freedom_dollars_(0),
  homeland_church_refresh_spend_freedom_dollars_(0),
  homeland_church_vip_refresh_spend_freddom_dollars_(0),
  homeland_max_church_free_refresh_times_(0),
  homeland_max_tree_assist_water_times_(0),
  homeland_tree_assist_water_restrict_dollars_reward_(0),
  homeland_max_stronghold_support_times_(0),
  homeland_stronghold_support_cooling_(0),
  homeland_stronghold_support_guild_contribution_reward_(0),
  homeland_max_invader_capacity_(0),
  homeland_max_event_capacity_(0),
  homeland_max_commander_battle_round_(0),
  homeland_max_soul_battle_round_(0),
  homeland_max_battle_support_soul_(0),
  homeland_battle_hp_buff_coe_(0),
  homeland_battle_attack_buff_coe_(0),
  guild_max_name_length_(0),
  guild_create_spend_gold_(0),
  guild_init_gold_(0),
  guild_max_join_capacity_(0),
  guild_max_pending_member_capacity_(0),
  guild_max_announcement_length_(0),
  guild_max_event_capacity_(0),
  guild_light_crystal_item_id_(0),
  guild_dark_crystal_item_id_(0),
  guild_holy_crystal_item_id_(0),
  guild_gold_unit_contribution_(0),
  guild_freedom_dollars_unit_contribution_(0),
  guild_freedom_dollars_unit_gold_(0),
  guild_light_crystal_unit_contribution_(0),
  guild_dark_crystal_unit_contribution_(0),
  guild_holy_crystal_unit_contribution_(0),
  guild_clear_buff_time_(0),
  spirit_bar_id_(0),
  drop_item_disappear_secs_(0),
  drop_item_clear_owner_secs_(0),
  drop_pickup_distance_(0),
  drop_item_owner_max_(0),
  new_actor_task_(0),
  summon_radius_(0),
  summon_num_(0),
  world_jump_expend_item_id_(0),
  world_jump_expend_item_num_(0),
  infinite_num_(0),
  auction_min_start_bid_price_(0),
  auction_min_buyout_price_(0),
  auction_min_bid_increment_percent_(0),
  auction_transaction_fee_percent_(0),
  auction_max_sell_item_capacity_(0),
  auction_max_buy_item_capacity_(0),
  auction_max_history_capacity_(0),
  auction_item_per_page_(0),
  replenish_checkins_num_(0),
  online_time_level1_(0),
  online_time_award1_(0),
  online_time_level2_(0),
  online_time_award2_(0),
  online_time_award3_(0),
  max_online_time_(0),
  pick_awards_limit_time_(0),
  maze_max_floor_(0),
  maze_width_(0),
  maze_height_(0),
  maze_max_action_point_(0),
  maze_action_point_recover_cooling_(0),
  maze_reset_time_week_day_(0),
  maze_reset_time_hour_(0),
  maze_reset_time_minute_(0),
  maze_reset_time_second_(0),
  maze_gift_item_id_(0),
  fcm_half_income_online_time_(0),
  fcm_zero_income_online_time_(0),
  fcm_clear_status_offline_time_(0),
  soul_energy_interval_msecs_(0),
  soul_energy_decrement_(0),
  soul_energy_explosion_buff_(0),
  soul_energy_kill_npc_energy_(0),
  soul_energy_kill_npc_level_diff_(0),
  soul_energy_kill_actor_energy_(0),
  soul_energy_kill_actor_level_diff_(0),
  hp_talisman_auto_span_time_(0),
  mp_talisman_auto_span_time_(0),
  hp_under_percent_(0),
  mp_under_percent_(0),
  hp_talisman_max_value_(0),
  mp_talisman_max_value_(0),
  zazen_revenue_time_(0),
  playing_team_ready_secs_(0),
  equip_inherit_spend_item_id_(0),
  equip_inherit_spend_item_count_(0),
  equip_inherit_min_level_(0),
  equip_inherit_min_src_quality_(0),
  equip_inherit_min_dest_quality_(0),
  transport_destory_check_sec_(0),
  transport_destory_sec_(0),
  transport_task_accept_npc_(0),
  transport_task_accept_npc_x_(0),
  transport_task_accept_npc_y_(0),
  transport_task_accept_npc_map_(0),
  transport_task_finish_npc_(0),
  transport_task_count_(0),
  transport_task_rob_count_(0),
  transport_task_refresh_spend_item_(0),
  transport_task_default_id_(0),
  map_branch_max_number_(0),
  map_branch_max_actor_number_(0) {}

MiscConfigure::~MiscConfigure() {}

bool MiscConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed", file.c_str());
    return false;
  }

  // 加载 data 结点标签
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", file.c_str());
    return false;
  }

  // NODE: move_speed
  TiXmlElement *move_speed_node = data_node->FirstChildElement("move_speed");
  if (NULL == move_speed_node) {
    LOG_ERROR("Config file (%s) /data/move_speed not found.", file.c_str());
    return false;
  }
  if (move_speed_node->Attribute("min", (int *)(&this->move_speed_min_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/move_speed[min] not found.", file.c_str());
    return false;
  }
  if (move_speed_node->Attribute("max", (int *)(&this->move_speed_max_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/move_speed[max] not found.", file.c_str());
    return false;
  }

  // NODE: default_skill
  TiXmlElement *default_skill_node = data_node->FirstChildElement("default_skill");
  if (NULL == default_skill_node) {
    LOG_ERROR("Config file (%s) /data/default_skill not found.", file.c_str());
    return false;
  }
  if (default_skill_node->Attribute("axe", (int *)(&this->axe_skill_)) == false) {
    LOG_ERROR("Config file (%s) /data/default_skill[axe] not found.", file.c_str());
    return false;
  }
  if (default_skill_node->Attribute("axe_level", (int *)(&this->axe_skill_level_)) == false) {
    LOG_ERROR("Config file (%s) /data/default_skill[axe_level] not found.", file.c_str());
    return false;
  }
  if (default_skill_node->Attribute("shoot", (int *)(&this->shoot_skill_)) == false) {
    LOG_ERROR("Config file (%s) /data/default_skill[shoot] not found.", file.c_str());
    return false;
  }
  if (default_skill_node->Attribute("shoot_level", (int *)(&this->shoot_skill_level_)) == false) {
    LOG_ERROR("Config file (%s) /data/default_skill[shoot_level] not found.", file.c_str());
    return false;
  }
  if (default_skill_node->Attribute("magic", (int *)(&this->magic_skill_)) == false) {
    LOG_ERROR("Config file (%s) /data/default_skill[magic] not found.", file.c_str());
    return false;
  }
  if (default_skill_node->Attribute("magic_level", (int *)(&this->magic_skill_level_)) == false) {
    LOG_ERROR("Config file (%s) /data/default_skill[magic_level] not found.", file.c_str());
    return false;
  }

  // NODE: rookie 
  TiXmlElement *rookie_node = data_node->FirstChildElement("rookie");
  if (NULL == rookie_node) {
    LOG_ERROR("Config file (%s) /data/rookie not found.", file.c_str());
    return false;
  }
  if (rookie_node->Attribute("protect_level", (int *)(&this->rookie_protect_level_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/rookie[protect_level] not found.", file.c_str());
    return false;
  }

  // NODE: die
  TiXmlElement *die_node = data_node->FirstChildElement("die");
  if (NULL == die_node) {
    LOG_ERROR("Config file (%s) /data/die not found.", file.c_str());
    return false;
  }
  if (die_node->Attribute("protect_buff_id", (int *)(&this->protect_buff_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/die[protect_buff_id] not found.", file.c_str());
    return false;
  }
  if (die_node->Attribute("pk_buff_id", (int *)(&this->pk_buff_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/die[pk_buff_id] not found.", file.c_str());
    return false;
  }
  if (die_node->Attribute("revive_item_id", (int *)(&this->revive_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/die[revive_item_id] not found.", file.c_str());
    return false;
  }

  // NODE: name_color
  TiXmlElement *name_color_node = data_node->FirstChildElement("name_color");
  if (NULL == name_color_node) {
    LOG_ERROR("Config file (%s) /data/name_color not found.", file.c_str());
    return false;
  }
  if (name_color_node->Attribute("brown_time", (int *)(&this->brown_time_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/name_color[brown_time] not found.", file.c_str());
    return false;
  }

  // NODE: soul_altar
  TiXmlElement *soul_altar_node = data_node->FirstChildElement("soul_altar");
  if (NULL == soul_altar_node) {
    LOG_ERROR("Config file (%s) /data/soul_altar not found.", file.c_str());
    return false;
  }
  if (soul_altar_node->Attribute("max_free_summon_times",
          (int *)(&this->soul_altar_max_free_summon_times_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/soul_altar[max_free_summon_times] not found.", file.c_str());
    return false;
  }
  if (soul_altar_node->Attribute("summon_spend_item_id",
          (int *)(&this->soul_altar_summon_spend_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/soul_altar[summon_spend_item_id] not found.", file.c_str());
    return false;
  }
  if (soul_altar_node->Attribute("summon_spend_item_count",
          (int *)(&this->soul_altar_summon_spend_item_count_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/soul_altar[summon_spend_item_count] not found.", file.c_str());
    return false;
  }
  if (soul_altar_node->Attribute("second_card_spend_freedom_dollars",
          (int *)(&this->soul_altar_second_card_spend_freedom_dollars_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/soul_altar[second_card_spend_freedom_dollars] not found.",
              file.c_str());
    return false;
  }
  if (soul_altar_node->Attribute("third_card_spend_freedom_dollars",
          (int *)(&this->soul_altar_third_card_spend_freedom_dollars_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/soul_altar[third_card_spend_freedom_dollars] not found.",
              file.c_str());
    return false;
  }
  if (soul_altar_node->Attribute("drop_group1_id",
          (int *)(&this->soul_altar_drop_group1_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/soul_altar[drop_group1_id] not found.", file.c_str());
    return false;
  }
  if (soul_altar_node->Attribute("drop_group2_id",
          (int *)(&this->soul_altar_drop_group2_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/soul_altar[drop_group2_id] not found.", file.c_str());
    return false;
  }

  // NODE: extend_container
  TiXmlElement *extend_container_node = data_node->FirstChildElement("extend_container");
  if (NULL == extend_container_node) {
    LOG_ERROR("Config file (%s) /data/extend_container not found.", file.c_str());
    return false;
  }
  if (extend_container_node->Attribute("gold",
          (int *)(&this->extend_container_gold_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/extend_container[gold] not found.", file.c_str());
    return false;
  }
  if (extend_container_node->Attribute("lockedgold",
          (int *)(&this->extend_container_lockedgold_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/extend_container[lockedgold] not found.", file.c_str());
    return false;
  }

  // NODE: gem
  TiXmlElement *gem_node = data_node->FirstChildElement("gem");
  if (NULL == gem_node) {
    LOG_ERROR("Config file (%s) /data/gem not found.", file.c_str());
    return false;
  }
  if (gem_node->Attribute("max_storage_capacity", (int *)(&this->gem_max_storage_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/gem[max_storage_capacity] not found.", file.c_str());
    return false;
  }

  // item
  TiXmlElement *item_node = data_node->FirstChildElement("item");
  if(NULL == item_node) {
    LOG_ERROR("Config file (%s) /data/item not found.", file.c_str());
    return false;
  }
  if(item_node->Attribute("max_buyback_capacity",
        (core::int32 *)(&this->buyback_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/item[buyback_capacity] not found.", file.c_str());
    return false;
  }
  if(item_node->Attribute("packet_min_capacity",
        (core::int32 *)(&this->packet_min_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/item[packet_min_capacity] not found.", file.c_str());
    return false;
  }
  if(item_node->Attribute("packet_max_capacity",
        (core::int32 *)(&this->packet_max_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/item[packet_max_capacity] not found.", file.c_str());
    return false;
  }
  if(item_node->Attribute("storage_min_capacity",
        (core::int32 *)(&this->storage_min_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/item[storage_min_capacity] not found.", file.c_str());
    return false;
  }
  if(item_node->Attribute("storage_max_capacity",
        (core::int32 *)(&this->storage_max_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/item[storage_max_capacity] not found.", file.c_str());
    return false;
  }
  if(item_node->Attribute("treasure_storage_capacity",
        (core::int32 *)(&this->treasure_storage_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/item[treasure_storage_capacity] not found.", file.c_str());
    return false;
  }

  // NODE: contacts
  TiXmlElement *contacts_node = data_node->FirstChildElement("contacts");
  if (NULL == contacts_node) {
    LOG_ERROR("Config file (%s) /data/contacts not found.", file.c_str());
    return false;
  }
  if (contacts_node->Attribute("max_friend_capacity",
          (int *)(&this->contacts_max_friend_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/contacts[max_friend_capacity] not found.", file.c_str());
    return false;
  }
  if (contacts_node->Attribute("max_blacklist_capacity",
          (int *)(&this->contacts_max_blacklist_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/contacts[max_blacklist_capacity] not found.", file.c_str());
    return false;
  }
  if (contacts_node->Attribute("max_signature_length",
          (int *)(&this->contacts_max_signature_length_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/contacts[max_signature_length] not found.", file.c_str());
    return false;
  }
  if (contacts_node->Attribute("recommend_level_range",
          (int *)(&this->contacts_recommend_level_range_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/contacts[recommend_level_range] not found.", file.c_str());
    return false;
  }
  if (contacts_node->Attribute("recommend_count",
          (int *)(&this->contacts_recommend_count_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/contacts[recommend_count] not found.", file.c_str());
    return false;
  }
  // --check
  if (this->contacts_recommend_level_range_ <= 0) {
    LOG_ERROR("Config file (%s) /data/contacts[recommend_level_range] is invalid.", file.c_str());
    return false;
  }

  // NODE: social_enemy
  TiXmlElement *social_enemy_node = data_node->FirstChildElement("social_enemy");
  if (NULL == social_enemy_node) {
    LOG_ERROR("Config file (%s) /data/social_enemy not found.", file.c_str());
    return false;
  }
  if (social_enemy_node->Attribute("max_capacity",
          (int *)(&this->social_enemy_max_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/social_enemy[max_capacity] not found.", file.c_str());
    return false;
  }

  // NODE: homeland
  TiXmlElement *homeland_node = data_node->FirstChildElement("homeland");
  if (NULL == homeland_node) {
    LOG_ERROR("Config file (%s) /data/homeland not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_upgrade_cooling",
          (int *)(&this->homeland_max_upgrade_cooling_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_upgrade_cooling] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("upgrade_cooling_unit",
          (int *)(&this->homeland_upgrade_cooling_unit_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[upgrade_cooling_unit] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("clear_upgrade_cooling_unit_freedom_dollars",
          (int *)(&this->homeland_clear_upgrade_cooling_unit_freedom_dollars_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[clear_upgrade_cooling_unit_freedom_dollars] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("temple_harvest_cooling_unit",
          (int *)(&this->homeland_temple_harvest_cooling_unit_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[temple_harvest_cooling_unit] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("clear_temple_harvest_cooling_unit_freedom_dollars",
          (int *)(&this->homeland_clear_temple_harvest_cooling_unit_freedom_dollars_)) == NULL) {
    LOG_ERROR("Config file (%s) "
              "/data/homeland[clear_temple_harvest_cooling_unit_freedom_dollars] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_goldmine_rob_times",
          (int *)(&this->homeland_max_goldmine_rob_times_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_goldmine_rob_times] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("goldmine_rob_cooling",
          (int *)(&this->homeland_goldmine_rob_cooling_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[goldmine_rob_cooling] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_church_free_pray_times",
          (int *)(&this->homeland_max_church_free_pray_times_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_church_free_pray_times] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_church_paid_pray_times",
          (int *)(&this->homeland_max_church_paid_pray_times_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_church_paid_pray_times] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("church_paid_pray_spend_freedom_dollars",
          (int *)(&this->homeland_church_paid_pray_spend_freedom_dollars_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[church_paid_pray_spend_freedom_dollars] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("church_refresh_spend_freedom_dollars",
          (int *)(&this->homeland_church_refresh_spend_freedom_dollars_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[church_refresh_spend_freedom_dollars] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("church_vip_refresh_spend_freddom_dollars",
          (int *)(&this->homeland_church_vip_refresh_spend_freddom_dollars_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[church_vip_refresh_spend_freddom_dollars] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_church_free_refresh_times",
          (int *)(&this->homeland_max_church_free_refresh_times_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_church_free_refresh_times] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_tree_assist_water_times",
          (int *)(&this->homeland_max_tree_assist_water_times_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_tree_assist_water_times] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("tree_assist_water_restrict_dollars_reward",
          (int *)(&this->homeland_tree_assist_water_restrict_dollars_reward_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[tree_assist_water_restrict_dollars_reward] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_stronghold_support_times",
          (int *)(&this->homeland_max_stronghold_support_times_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_stronghold_support_times] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("stronghold_support_cooling",
          (int *)(&this->homeland_stronghold_support_cooling_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[stronghold_support_cooling] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("stronghold_support_guild_contribution_reward",
          (int *)(&this->homeland_stronghold_support_guild_contribution_reward_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[stronghold_support_guild_contribution_reward] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_invader_capacity",
          (int *)(&this->homeland_max_invader_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_invader_capacity] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_event_capacity",
          (int *)(&this->homeland_max_event_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_event_capacity] not found.", file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_commander_battle_round",
          (int *)(&this->homeland_max_commander_battle_round_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_commander_battle_round] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_soul_battle_round",
          (int *)(&this->homeland_max_soul_battle_round_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_soul_battle_round] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("max_battle_support_soul",
          (int *)(&this->homeland_max_battle_support_soul_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[max_battle_support_soul] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("battle_hp_buff_coe",
          (int *)(&this->homeland_battle_hp_buff_coe_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[battle_hp_buff_coe] not found.",
              file.c_str());
    return false;
  }
  if (homeland_node->Attribute("battle_attack_buff_coe",
          (int *)(&this->homeland_battle_attack_buff_coe_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/homeland[battle_attack_buff_coe] not found.",
              file.c_str());
    return false;
  }
  // --check
  if (this->homeland_upgrade_cooling_unit_ <= 0) {
    LOG_ERROR("Config file (%s) /data/homeland[upgrade_cooling_unit] is invalid.");
    return false;
  }
  if (this->homeland_temple_harvest_cooling_unit_ <= 0) {
    LOG_ERROR("Config file (%s) /data/homeland[temple_harvest_cooling_unit] is invalid.", file.c_str());
    return false;
  }

  // NODE: guild
  TiXmlElement *guild_node = data_node->FirstChildElement("guild");
  if (NULL == guild_node) {
    LOG_ERROR("Config file (%s) /data/guild not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("max_name_length",
          (int *)(&this->guild_max_name_length_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[max_name_length] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("create_spend_gold",
          (int *)(&this->guild_create_spend_gold_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[create_spend_gold] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("init_gold",
          (int *)(&this->guild_init_gold_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[init_gold] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("max_join_capacity",
          (int *)(&this->guild_max_join_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[max_join_capacity] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("max_pending_member_capacity",
          (int *)(&this->guild_max_pending_member_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[max_pending_member_capacity] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("max_announcement_length",
          (int *)(&this->guild_max_announcement_length_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[max_announcement_length] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("max_event_capacity",
          (int *)(&this->guild_max_event_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[max_event_capacity] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("light_crystal_item_id",
          (int *)(&this->guild_light_crystal_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[light_crystal_item_id] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("dark_crystal_item_id",
          (int *)(&this->guild_dark_crystal_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[dark_crystal_item_id] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("holy_crystal_item_id",
          (int *)(&this->guild_holy_crystal_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[holy_crystal_item_id] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("gold_unit_contribution",
          (int *)(&this->guild_gold_unit_contribution_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[gold_unit_contribution] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("freedom_dollars_unit_contribution",
          (int *)(&this->guild_freedom_dollars_unit_contribution_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[freedom_dollars_unit_contribution] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("freedom_dollars_unit_gold",
          (int *)(&this->guild_freedom_dollars_unit_gold_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[freedom_dollars_unit_gold] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("light_crystal_unit_contribution",
          (int *)(&this->guild_light_crystal_unit_contribution_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[light_crystal_unit_contribution] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("dark_crystal_unit_contribution",
          (int *)(&this->guild_dark_crystal_unit_contribution_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[dark_crystal_unit_contribution] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("holy_crystal_unit_contribution",
          (int *)(&this->guild_holy_crystal_unit_contribution_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[holy_crystal_unit_contribution] not found.", file.c_str());
    return false;
  }
  if (guild_node->Attribute("clear_buff_time",
          (int *)(&this->guild_clear_buff_time_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[clear_buff_time] not found.", file.c_str());
    return false;
  }

  // NODE: spirit
  TiXmlElement *spirit_node = data_node->FirstChildElement("spirit");
  if (NULL == spirit_node) {
    LOG_ERROR("Config file (%s) /data/spirit not found.", file.c_str());
    return false;
  }
  if (spirit_node->Attribute("bar_id",
        (int *)(&this->spirit_bar_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/guild[bar_id] not found.", file.c_str());
    return false;
  }

  // NODE: drop
  TiXmlElement *drop_node = data_node->FirstChildElement("drop");
  if (NULL == drop_node) {
    LOG_ERROR("Config file (%s) /data/drop not found.", file.c_str());
    return false;
  }
  if (drop_node->Attribute("disappear_secs",
        (int *)(&this->drop_item_disappear_secs_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/drop[disappear_secs] not found.", file.c_str());
    return false;
  }
  if (drop_node->Attribute("clear_owner_secs",
        (int *)(&this->drop_item_clear_owner_secs_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/drop[clear_owner_secs] not found.", file.c_str());
    return false;
  }
  if (drop_node->Attribute("pickup_distance",
        (int *)(&this->drop_pickup_distance_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/drop[pickup_distance] not found.", file.c_str());
    return false;
  }
  if(drop_node->Attribute("drop_item_owner_max",
        (int *)(&this->drop_item_owner_max_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/drop[dorp_item_owner_max] not found.", file.c_str());
    return false;
  }

  // NODE: task
  TiXmlElement *task_node = data_node->FirstChildElement("task");
  if (NULL == task_node) {
    LOG_ERROR("Config file (%s) /data/task not found.", file.c_str());
    return false;
  }
  if (task_node->Attribute("new_actor_task",
        (int *)(&this->new_actor_task_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/task[new_actor_task] not found.", file.c_str());
    return false;
  }
  if(task_node->Attribute("summon_radius",
        (int *)(&this->summon_radius_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/task[summon_radius] not find.", file.c_str());
    return false;
  }
  if(task_node->Attribute("summon_num",
        (int *)(&this->summon_num_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/task[summon_num] not find.", file.c_str());
    return false;
  }

  // NODE: world_jump
  TiXmlElement *world_jump_node = data_node->FirstChildElement("world_jump");
  if (NULL == world_jump_node) {
    LOG_ERROR("Config file (%s) /data/world_jump not found.", file.c_str());
    return false;
  }
  if (world_jump_node->Attribute("expend_item_id",
        (int *)(&this->world_jump_expend_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/task[expend_item_id] not found.", file.c_str());
    return false;
  }
  if (world_jump_node->Attribute("expend_item_num",
        (int *)(&this->world_jump_expend_item_num_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/task[expend_item_num] not found.", file.c_str());
    return false;
  }


  // NODE: vip
  TiXmlElement *vip_node = data_node->FirstChildElement("vip");
  if (NULL == vip_node) {
    LOG_ERROR("Config file (%s) /data/world_jump not found.", file.c_str());
    return false;
  }
  if (vip_node->Attribute("infinite_num",
        (int *)(&this->infinite_num_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/task[infinite_num] not found.", file.c_str());
    return false;
  }

  // NODE: auction
  TiXmlElement *auction_node = data_node->FirstChildElement("auction");
  if (NULL == auction_node) {
    LOG_ERROR("Config file (%s) /data/auction not found.", file.c_str());
    return false;
  }
  if (auction_node->Attribute("min_start_bid_price",
          (int *)(&this->auction_min_start_bid_price_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[min_start_bid_price] not found.",
              file.c_str());
    return false;
  }
  if (auction_node->Attribute("min_buyout_price",
          (int *)(&this->auction_min_buyout_price_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[min_buyout_price] not found.",
              file.c_str());
    return false;
  }
  if (auction_node->Attribute("min_bid_increment_percent",
          (int *)(&this->auction_min_bid_increment_percent_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[min_bid_increment_percent] not found.",
              file.c_str());
    return false;
  }
  if (auction_node->Attribute("transaction_fee_percent",
          (int *)(&this->auction_transaction_fee_percent_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[transaction_fee_percent] not found.",
              file.c_str());
    return false;
  }
  if (auction_node->Attribute("max_sell_item_capacity",
          (int *)(&this->auction_max_sell_item_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[max_sell_item_capacity] not found.", file.c_str());
    return false;
  }
  if (auction_node->Attribute("max_buy_item_capacity",
          (int *)(&this->auction_max_buy_item_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[max_buy_item_capacity] not found.", file.c_str());
    return false;
  }
  if (auction_node->Attribute("max_history_capacity",
          (int *)(&this->auction_max_history_capacity_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[max_history_capacity] not found.", file.c_str());
    return false;
  }
  if (auction_node->Attribute("item_per_page",
          (int *)(&this->auction_item_per_page_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/auction[item_per_page] not found.", file.c_str());
    return false;
  }
  // --check
  if (this->auction_item_per_page_ <= 0) {
    LOG_ERROR("Config file (%s) /data/auction[item_per_page] is invalid.", file.c_str());
    return false;
  }

  // NODE: welfare
  TiXmlElement *welfare_mode = data_node->FirstChildElement("welfare");
  if (NULL == welfare_mode) {
    LOG_ERROR("Config file (%s) /data/welfare not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("replenish_checkins_num",
        (int *)(&this->replenish_checkins_num_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[replenish_checkins_num] not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("online_time_level1",
        (int *)(&this->online_time_level1_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[online_time_level1] not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("online_time_award1",
        (int *)(&this->online_time_award1_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[online_time_award1] not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("online_time_level2",
        (int *)(&this->online_time_level2_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[online_time_level2] not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("online_time_award2",
        (int *)(&this->online_time_award2_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[online_time_award2] not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("online_time_award3",
        (int *)(&this->online_time_award3_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[online_time_award3] not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("max_online_time",
        (int *)(&this->max_online_time_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[max_online_time] not found.", file.c_str());
    return false;
  }
  if (welfare_mode->Attribute("pick_awards_limit_time",
        (int *)(&this->pick_awards_limit_time_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/welfare[pick_awards_limit_time] not found.", file.c_str());
    return false;
  }

  // NODE: resource
  TiXmlElement *resource_node = data_node->FirstChildElement("resource");
  if (NULL == resource_node) {
    LOG_ERROR("Config file (%s) /data/resource not found.", file.c_str());
    return false;
  }
  if (resource_node->Attribute("max_nimbus", (int *)(&this->max_nimbus_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/resource[max_nimbus] not found.", file.c_str());
    return false;
  }

  // NODE: maze
  TiXmlElement *maze_node = data_node->FirstChildElement("maze");
  if (NULL == maze_node) {
    LOG_ERROR("Config file (%s) /data/maze not found.", file.c_str());
    return false;
  }
  if (maze_node->Attribute("max_floor", (int *)(&this->maze_max_floor_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[max_floor] not found.", file.c_str());
    return false;
  }
  if (maze_node->Attribute("width", (int *)(&this->maze_width_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[width] not found.", file.c_str());
    return false;
  }
  if (maze_node->Attribute("height", (int *)(&this->maze_height_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[height] not found.", file.c_str());
    return false;
  }
  if (maze_node->Attribute("max_action_point", (int *)(&this->maze_max_action_point_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[max_action_point] not found.", file.c_str());
    return false;
  }
  if (maze_node->Attribute("action_point_recover_cooling",
          (int *)(&this->maze_action_point_recover_cooling_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[action_point_recover_cooling] not found.", file.c_str());
    return false;
  }
  {
    const char *str = maze_node->Attribute("reset_time");
    if (NULL == str) {
      LOG_ERROR("Config file (%s) /data/maze[reset_time] not found.", file.c_str());
      return false;
    }
    std::vector<std::string> split_result;
    global::StringSplit(str, ",", split_result);
    if (split_result.size() != 4) {
      LOG_ERROR("Config file (%s) /data/maze[reset_time] is invalid.", file.c_str());
      return false;
    }
    this->maze_reset_time_week_day_ = atoi(split_result[0].c_str()) % 7;
    this->maze_reset_time_hour_ = atoi(split_result[1].c_str()) % 24;
    this->maze_reset_time_minute_ = atoi(split_result[2].c_str()) % 60;
    this->maze_reset_time_second_ = atoi(split_result[3].c_str()) % 60;
  }
  if (maze_node->Attribute("gift_item_id", (int *)(&this->maze_gift_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[gift_item_id] not found.", file.c_str());
    return false;
  }
  // --check
  if (this->maze_max_floor_ <= 0) {
    LOG_ERROR("Config file (%s) /data/maze[max_floor] is invalid.", file.c_str());
    return false;
  }
  if (this->maze_width_ <= 0) {
    LOG_ERROR("Config file (%s) /data/maze[width] is invalid.", file.c_str());
    return false;
  }
  if (this->maze_height_ <= 0) {
    LOG_ERROR("Config file (%s) /data/maze[height] is invalid.", file.c_str());
    return false;
  }
  if (1 == this->maze_width_ && 1 == this->maze_height_) {
    LOG_ERROR("Config file (%s) /data/maze[width] and /data/maze[height] is invalid.",
              file.c_str());
    return false;
  }
  if (this->maze_action_point_recover_cooling_ <= 0) {
    LOG_ERROR("Config file (%s) /data/maze[action_point_recover_cooling] is invalid.",
              file.c_str());
    return false;
  }

  // NODE: fcm
  TiXmlElement *fcm_node = data_node->FirstChildElement("fcm");
  if (NULL == fcm_node) {
    LOG_ERROR("Config file (%s) /data/fcm not found.", file.c_str());
    return false;
  }
  if (fcm_node->Attribute("half_income_online_time",
          (int *)(&this->fcm_half_income_online_time_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/fcm/half_income_online_time not found.", file.c_str());
    return false;
  }
  if (fcm_node->Attribute("zero_income_online_time",
          (int *)(&this->fcm_zero_income_online_time_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/fcm/zero_income_online_time not found.", file.c_str());
    return false;
  }
  if (fcm_node->Attribute("clear_status_offline_time",
          (int *)(&this->fcm_clear_status_offline_time_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/fcm/clear_status_offline_time not found.", file.c_str());
    return false;
  }
  // --check
  if (this->fcm_zero_income_online_time_ < this->fcm_half_income_online_time_) {
    LOG_ERROR("Config file (%s) /data/fcm/zero_income_online_time is invalid.", file.c_str());
    return false;
  }

  // NODE: soul_energy
  TiXmlElement *soul_energy_node = data_node->FirstChildElement("soul_energy");
  if (NULL == soul_energy_node) {
    LOG_ERROR("Config file(%s) /data/soul_energy not found.", file.c_str());
    return false;
  }
  if (soul_energy_node->Attribute("soul_energy_interval_msecs",
        (int *)(&this->soul_energy_interval_msecs_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/soul_energy/soul_energy_interval_msecs not found.",
        file.c_str());
    return false;
  }
  if (soul_energy_node->Attribute("soul_energy_decrement",
        (int *)&this->soul_energy_decrement_) == NULL) {
    LOG_ERROR("Config file(%s) /data/soul_energy/soul_energy_decrement not found.",
        file.c_str());
    return false;
  }
  if (soul_energy_node->Attribute("soul_energy_explosion_buff",
        (int *)&this->soul_energy_explosion_buff_) == NULL) {
    LOG_ERROR("Config file(%s) /data/soul_energy/soul_energy_explosion_buff not found.",
        file.c_str());
    return false;
  }
  if (soul_energy_node->Attribute("soul_energy_kill_npc_energy",
        (int *)&this->soul_energy_kill_npc_energy_) == NULL) {
    LOG_ERROR("Config file(%s) /data/soul_energy/soul_energy_kill_npc_energy not found.",
        file.c_str());
    return false;
  }
  if (soul_energy_node->Attribute("soul_energy_kill_npc_level_diff",
        (int *)&this->soul_energy_kill_npc_level_diff_) == NULL) {
    LOG_ERROR("Config file(%s) /data/soul_energy/soul_energy_kill_npc_level_diff not found.",
        file.c_str());
    return false;
  }
  if (soul_energy_node->Attribute("soul_energy_kill_actor_energy",
        (int *)&this->soul_energy_kill_actor_energy_) == NULL) {
    LOG_ERROR("Config file(%s) /data/soul_energy/soul_energy_kill_actor_energy_ not found.",
        file.c_str());
    return false;
  }
  if (soul_energy_node->Attribute("soul_energy_kill_actor_level_diff",
        (int *)&this->soul_energy_kill_actor_level_diff_) == NULL) {
    LOG_ERROR("Config file(%s) /data/soul_energy/soul_energy_kill_actor_level_diff not found.",
        file.c_str());
    return false;
  }

  // NODE: scene
  TiXmlElement *scene_node = data_node->FirstChildElement("scene");
  if (NULL == scene_node) {
    LOG_ERROR("Config file(%s) /data/scene not found.", file.c_str());
    return false;
  }
  std::vector<std::string> splits;
  std::vector<core::int32> splits1;
  const char *str = scene_node->Attribute("world_boss_appear_time");
  if(str == NULL) {
    LOG_ERROR("Config file(%s) /data/scene/world_boss_appear_time not found.",
        file.c_str());
    return false;
  }
  global::StringSplit(str, ",", splits);
  for(size_t time_loop = 0; time_loop < splits.size(); ++ time_loop) {
    global::StringSplit(splits[time_loop].c_str(), ":", splits1);
    if(splits1.size() != 3) {
      LOG_ERROR("Config file(%s) /data/scene/world_boss_appear_time 配置不正确", file.c_str());
      return false;
    }
    core::int32 temp_time = splits1[0]*3600 + splits1[1]*60 + splits1[2];
    this->worldboss_appear_time_vecs_.push_back(temp_time);
  }
  std::sort(this->worldboss_appear_time_vecs_.begin(), this->worldboss_appear_time_vecs_.end());
  if(scene_node->Attribute("consecutive_kill_level", (int *)&this->consecutive_kill_level_) == NULL) {
    LOG_ERROR("Config file(%s) /data/scene/consecutive_kill_level no found.", file.c_str());
    return false;
  }

  // NODE: scene
  TiXmlElement *talisman_node = data_node->FirstChildElement("talisman");
  if (NULL == talisman_node) {
    LOG_ERROR("Config file(%s) /data/talisman not found.", file.c_str());
    return false;
  }
  if(talisman_node->Attribute("hp_talisman_auto_span_time",
        (int *)&this->hp_talisman_auto_span_time_) == NULL) {
    LOG_ERROR("Config file(%s) /data/talisman/hp_talisman_auto_span_time not found.", file.c_str());
    return false;
  }
  if(talisman_node->Attribute("mp_talisman_auto_span_time",
        (int *)&this->mp_talisman_auto_span_time_) == NULL) {
    LOG_ERROR("Config file(%s) /data/talisman/mp_talisman_auto_span_time not found.", file.c_str());
    return false;
  }
  if(talisman_node->Attribute("hp_under_percent", (int *)&this->hp_under_percent_) == NULL) {
    LOG_ERROR("Config file(%s) /data/talisman/hp_under_percent not found.", file.c_str());
    return false;
  }
  if(talisman_node->Attribute("mp_under_percent", (int *)&this->mp_under_percent_) == NULL) {
    LOG_ERROR("Config file(%s) /data/talisman/mp_under_percent not found.", file.c_str());
    return false;
  }
  if(talisman_node->Attribute("hp_talisman_max_value", (int *)&this->hp_talisman_max_value_) == NULL) {
    LOG_ERROR("Config file(%s) /data/talisman/hp_talisman_max_value not found.", file.c_str());
    return false;
  }
  if(talisman_node->Attribute("mp_talisman_max_value", (int *)&this->mp_talisman_max_value_) == NULL) {
    LOG_ERROR("Config file(%s) /data/talisman/mp_talisman_max_value not found.", file.c_str());
    return false;
  }

  // NODE: zazen
  TiXmlElement *zazen_node = data_node->FirstChildElement("zazen");
  if (NULL == zazen_node) {
    LOG_ERROR("Config file(%s) /data/zazen not found.", file.c_str());
    return false;
  }
  if(zazen_node->Attribute("zazen_revenue_time",
        (int *)&this->zazen_revenue_time_) == NULL) {
    LOG_ERROR("Config file(%s) /data/zazen/zazen_revenue_time not found.", file.c_str());
    return false;
  }

  // NODE: playing
  TiXmlElement *playing_node = data_node->FirstChildElement("playing");
  if (NULL == playing_node) {
    LOG_ERROR("config file(%s) /data/playing not found.", file.c_str());
    return false;
  }
  if (playing_node->Attribute("playing_team_ready_secs",
        (int *)&this->playing_team_ready_secs_) == NULL) {
    LOG_ERROR("Confi file(%s) /data/playing/playing_team_ready_secs.", file.c_str());
    return false;
  }

  // NODE: equip_inherit
  TiXmlElement *equip_inherit_node = data_node->FirstChildElement("equip_inherit");
  if (NULL == equip_inherit_node) {
    LOG_ERROR("Config file(%s) /data/equip_inherit not found.", file.c_str());
    return false;
  }
  if (equip_inherit_node->Attribute("spend_item_id",
          (int *)(&this->equip_inherit_spend_item_id_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[spend_item_id] not found.", file.c_str());
    return false;
  }
  if (equip_inherit_node->Attribute("spend_item_count",
          (int *)(&this->equip_inherit_spend_item_count_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[spend_item_count] not found.", file.c_str());
    return false;
  }
  if (equip_inherit_node->Attribute("min_level",
          (int *)(&this->equip_inherit_min_level_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[min_level] not found.", file.c_str());
    return false;
  }
  if (equip_inherit_node->Attribute("min_src_quality",
          (int *)(&this->equip_inherit_min_src_quality_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[min_src_quality] not found.", file.c_str());
    return false;
  }
  if (equip_inherit_node->Attribute("min_dest_quality",
          (int *)(&this->equip_inherit_min_dest_quality_)) == NULL) {
    LOG_ERROR("Config file (%s) /data/maze[min_dest_quality] not found.", file.c_str());
    return false;
  }

  // NODE: transport
  TiXmlElement *transport_node = data_node->FirstChildElement("transport");
  if (NULL == transport_node) {
    LOG_ERROR("config file(%s) /data/transport not found.", file.c_str());
    return false;
  }
  if (transport_node->Attribute("transport_destory_check_sec",
        (int *)(&this->transport_destory_check_sec_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/transport[transport_destory_check_sec] not found.",
        file.c_str());
    return false;
  }
  if (transport_node->Attribute("transport_destory_sec",
        (int *)(&this->transport_destory_sec_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/transport[transport_destory_sec] not found.",
        file.c_str());
    return false;
  }

  // NODE: transport_task
  TiXmlElement *transport_task_node = data_node->FirstChildElement("transport_task");
  if (NULL == transport_task_node) {
    LOG_ERROR("config file(%s) /data/transport_task not found.", file.c_str());
    return false;
  }
  if (transport_task_node->Attribute("transport_task_count",
        (int *)(&this->transport_task_count_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_count] not found.",
        file.c_str());
    return false;
  }
  if (transport_task_node->Attribute("transport_task_rob_count",
        (int *)(&this->transport_task_rob_count_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_rob_count] not found.",
        file.c_str());
    return false;
  }
  if (transport_task_node->Attribute("transport_task_refresh_spend_item",
        (int *)(&this->transport_task_refresh_spend_item_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_refresh_spend_item] not found.",
        file.c_str());
    return false;
  }
  if (transport_task_node->Attribute("transport_task_default_id",
        (int *)(&this->transport_task_default_id_)) == NULL) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_default_id] not found.",
        file.c_str());
    return false;
  }
  const char *transport_task_accept_npc_str =
    transport_task_node->Attribute("transport_task_accept_npc");
  if (NULL == transport_task_accept_npc_str) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_accept_npc] not found.",
        file.c_str());
    return false;
  }
  std::vector<core::int32> transport_task_accept_npc_vector;
  global::StringSplit(transport_task_accept_npc_str, "+", transport_task_accept_npc_vector);
  if (transport_task_accept_npc_vector.size() != 4) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_accept_npc] error.",
        file.c_str());
    return false;
  }
  this->transport_task_accept_npc_ = transport_task_accept_npc_vector[0];
  this->transport_task_accept_npc_map_ = transport_task_accept_npc_vector[1];
  this->transport_task_accept_npc_x_ = transport_task_accept_npc_vector[2];
  this->transport_task_accept_npc_y_ = transport_task_accept_npc_vector[3];
  const char *transport_task_finish_npc_str =
    transport_task_node->Attribute("transport_task_finish_npc");
  if (NULL == transport_task_finish_npc_str) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_finish_npc] not found.",
        file.c_str());
    return false;
  }
  std::vector<core::int32> transport_task_finish_npc_vector;
  global::StringSplit(transport_task_finish_npc_str, "+", transport_task_finish_npc_vector);
  if (transport_task_finish_npc_vector.size() != 4) {
    LOG_ERROR("Config file(%s) /data/transport_task[transport_task_finish_npc] error.",
        file.c_str());
    return false;
  }
  this->transport_task_finish_npc_ = transport_task_finish_npc_vector[0];

  // NODE: map
  TiXmlElement *map_node = data_node->FirstChildElement("map");
  if (NULL == map_node) {
    LOG_ERROR("config file(%s) /data/map not found.", file.c_str());
    return false;
  }
  if (map_node->Attribute("map_branch_max_number",
        (int *)(&this->map_branch_max_number_)) == NULL) {
    LOG_ERROR("config file(%s) /data/map[map_branch_max_number] not found.");
    return false;
  }
  if (map_node->Attribute("map_branch_max_actor_number",
        (int *)(&this->map_branch_max_actor_number_)) == NULL) {
    LOG_ERROR("config file(%s) /data/map[map_branch_max_actor_number] not found.");
    return false;
  }

  return true;
}

}  // namespace configure

}  // namespace global

