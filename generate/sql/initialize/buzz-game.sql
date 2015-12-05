/*
MySQL Data Transfer
Source Host: 192.168.0.208
Source Database: buzz-game-hantao
Target Host: 192.168.0.208
Target Database: buzz-game-hantao
Date: 2013-8-5 下午 06:21:28
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for actor-0
-- ----------------------------
DROP TABLE IF EXISTS `actor-0`;
CREATE TABLE `actor-0` (
  `id` bigint(20) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `level` int(11) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `gender` tinyint(4) NOT NULL DEFAULT '0',
  `head` tinyint(4) NOT NULL DEFAULT '0',
  `hair` tinyint(4) NOT NULL DEFAULT '0',
  `current_hp` int(11) NOT NULL DEFAULT '0',
  `current_mp` int(11) NOT NULL DEFAULT '0',
  `current_exp` int(11) NOT NULL DEFAULT '0',
  `nimbus` int(11) NOT NULL DEFAULT '0',
  `gold` int(11) NOT NULL DEFAULT '0',
  `freedom_dollars` int(11) NOT NULL DEFAULT '0',
  `restrict_dollars` int(11) NOT NULL DEFAULT '0',
  `honour` int(11) NOT NULL DEFAULT '0',
  `war_soul` int(11) NOT NULL DEFAULT '0',
  `gem_shard` int(11) NOT NULL DEFAULT '0',
  `justice` int(11) NOT NULL DEFAULT '0',
  `miracle_integral` int(11) NOT NULL DEFAULT '0',
  `legend_integral` int(11) NOT NULL DEFAULT '0',
  `arena_integral` int(11) NOT NULL DEFAULT '0',
  `map` smallint(6) NOT NULL DEFAULT '0',
  `x` int(11) NOT NULL DEFAULT '0',
  `y` int(11) NOT NULL DEFAULT '0',
  `pre_map` smallint(6) NOT NULL DEFAULT '0',
  `pre_x` int(11) NOT NULL DEFAULT '0',
  `pre_y` int(11) NOT NULL DEFAULT '0',
  `brown_name` int(11) NOT NULL DEFAULT '0',
  `pk_mode` int(11) NOT NULL DEFAULT '0',
  `functionality_state` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `packet_capacity` int(11) NOT NULL DEFAULT '0',
  `storage_capacity` int(11) NOT NULL DEFAULT '0',
  `axe_fight_score` int(11) NOT NULL DEFAULT '0',
  `shoot_fight_score` int(11) NOT NULL DEFAULT '0',
  `magic_fight_score` int(11) NOT NULL DEFAULT '0',
  `last_logout_time` bigint(20) NOT NULL DEFAULT '0',
  `vip_type` tinyint(4) NOT NULL DEFAULT '0',
  `vip_remainder_time` int(11) NOT NULL DEFAULT '0',
  `world_jump_vip_num` int(11) NOT NULL DEFAULT '0',
  `fcm_type` int(11) NOT NULL DEFAULT '0',
  `fcm_online_time` int(11) NOT NULL DEFAULT '0',
  `create_time` bigint(20) NOT NULL DEFAULT '0',
  `hp_tasliman` int(11) NOT NULL DEFAULT '0',
  `mp_tasliman` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Table structure for actor-1
-- ----------------------------
DROP TABLE IF EXISTS `actor-1`;
CREATE TABLE `actor-1` (
  `id` bigint(20) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `level` int(11) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `gender` tinyint(4) NOT NULL DEFAULT '0',
  `head` tinyint(4) NOT NULL DEFAULT '0',
  `hair` tinyint(4) NOT NULL DEFAULT '0',
  `current_hp` int(11) NOT NULL DEFAULT '0',
  `current_mp` int(11) NOT NULL DEFAULT '0',
  `current_exp` int(11) NOT NULL DEFAULT '0',
  `nimbus` int(11) NOT NULL DEFAULT '0',
  `gold` int(11) NOT NULL,
  `freedom_dollars` int(11) NOT NULL DEFAULT '0',
  `restrict_dollars` int(11) NOT NULL DEFAULT '0',
  `honour` int(11) NOT NULL DEFAULT '0',
  `war_soul` int(11) NOT NULL DEFAULT '0',
  `gem_shard` int(11) NOT NULL DEFAULT '0',
  `justice` int(11) NOT NULL DEFAULT '0',
  `miracle_integral` int(11) NOT NULL DEFAULT '0',
  `legend_integral` int(11) NOT NULL DEFAULT '0',
  `arena_integral` int(11) NOT NULL DEFAULT '0',
  `map` smallint(6) NOT NULL DEFAULT '0',
  `x` int(11) NOT NULL DEFAULT '0',
  `y` int(11) NOT NULL DEFAULT '0',
  `pre_map` smallint(6) NOT NULL DEFAULT '0',
  `pre_x` int(11) NOT NULL DEFAULT '0',
  `pre_y` int(11) NOT NULL DEFAULT '0',
  `brown_name` int(11) NOT NULL DEFAULT '0',
  `pk_mode` int(11) NOT NULL DEFAULT '0',
  `functionality_state` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `packet_capacity` int(11) NOT NULL DEFAULT '0',
  `storage_capacity` int(11) NOT NULL DEFAULT '0',
  `axe_fight_score` int(11) NOT NULL DEFAULT '0',
  `shoot_fight_score` int(11) NOT NULL DEFAULT '0',
  `magic_fight_score` int(11) NOT NULL DEFAULT '0',
  `last_logout_time` bigint(20) NOT NULL DEFAULT '0',
  `vip_type` tinyint(4) NOT NULL DEFAULT '0',
  `vip_remainder_time` int(11) NOT NULL DEFAULT '0',
  `world_jump_vip_num` int(11) NOT NULL DEFAULT '0',
  `fcm_type` int(11) NOT NULL DEFAULT '0',
  `fcm_online_time` int(11) NOT NULL DEFAULT '0',
  `create_time` bigint(20) NOT NULL DEFAULT '0',
  `hp_tasliman` int(11) NOT NULL DEFAULT '0',
  `mp_tasliman` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Table structure for attr-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `attr-actor-0`;
CREATE TABLE `attr-actor-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0', 
  `noble_level` int(11) NOT NULL DEFAULT '0', 
  `noble_used_item_count` int(11) NOT NULL DEFAULT '0', 
  `noble_max_hp_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_max_mp_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_physics_attack_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_physics_defence_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_magic_attack_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_magic_defence_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_dodge_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_crit_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_ignore_defence_add_value` int(11) NOT NULL DEFAULT '0', 
  `sword_avail_step` int(11) NOT NULL DEFAULT '0',
  `sword_current_step` int(11) NOT NULL DEFAULT '0',
  `sword_lucky_value` int(11) NOT NULL DEFAULT '0',
  `sword_gain_time` bigint(20) NOT NULL DEFAULT '0',
  `element_level` int(11) NOT NULL DEFAULT '0', 
  `spirit_level` int(11) NOT NULL DEFAULT '0', 
  `spirit_lucky_value` int(11) NOT NULL DEFAULT '0', 
  `spirit_current_facade_type` int(11) NOT NULL DEFAULT '0', 
  `spirit_horse_facade_id` int(11) NOT NULL DEFAULT '0', 
  `spirit_wing_facade_id` int(11) NOT NULL DEFAULT '0', 
  `runes` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `legendary_weapon_pieces` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for attr-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `attr-actor-1`;
CREATE TABLE `attr-actor-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0', 
  `noble_level` int(11) NOT NULL DEFAULT '0', 
  `noble_used_item_count` int(11) NOT NULL DEFAULT '0', 
  `noble_max_hp_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_max_mp_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_physics_attack_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_physics_defence_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_magic_attack_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_magic_defence_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_dodge_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_crit_add_value` int(11) NOT NULL DEFAULT '0', 
  `noble_ignore_defence_add_value` int(11) NOT NULL DEFAULT '0', 
  `sword_avail_step` int(11) NOT NULL DEFAULT '0',
  `sword_current_step` int(11) NOT NULL DEFAULT '0',
  `sword_lucky_value` int(11) NOT NULL DEFAULT '0',
  `sword_gain_time` bigint(20) NOT NULL DEFAULT '0',
  `element_level` int(11) NOT NULL DEFAULT '0', 
  `spirit_level` int(11) NOT NULL DEFAULT '0', 
  `spirit_lucky_value` int(11) NOT NULL DEFAULT '0', 
  `spirit_current_facade_type` int(11) NOT NULL DEFAULT '0', 
  `spirit_horse_facade_id` int(11) NOT NULL DEFAULT '0', 
  `spirit_wing_facade_id` int(11) NOT NULL DEFAULT '0',
  `runes` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `legendary_weapon_pieces` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auction-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `auction-actor-0`;
CREATE TABLE `auction-actor-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0', 
  `sell_items` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `buy_items` varchar(8192) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `history` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auction-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `auction-actor-1`;
CREATE TABLE `auction-actor-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0', 
  `sell_items` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `buy_items` varchar(8192) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `history` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auction-transaction-0
-- ----------------------------
DROP TABLE IF EXISTS `auction-transaction-0`;
CREATE TABLE `auction-transaction-0` (
  `id` varchar(32) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `item_template_id` int(11) NOT NULL DEFAULT '0',
  `item_number` int(11) NOT NULL DEFAULT '0',
  `item_extra_info` varchar(512) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `auction_time_id` int(11) NOT NULL DEFAULT '0',
  `start_bid_time` bigint(20) NOT NULL DEFAULT '0',
  `end_bid_time` bigint(20) NOT NULL DEFAULT '0',
  `owner` bigint(20) NOT NULL DEFAULT '0',
  `buyer` bigint(20) NOT NULL DEFAULT '0',
  `start_bid_price` int(11) NOT NULL DEFAULT '0',
  `last_bid_price` int(11) NOT NULL DEFAULT '0',
  `buyout_price` int(11) NOT NULL DEFAULT '0',
  `finished` tinyint(4) NOT NULL DEFAULT '0',
  `money_withdrawn` tinyint(4) NOT NULL DEFAULT '0',
  `item_withdrawn` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for mail-0
-- ----------------------------
DROP TABLE IF EXISTS `mail-0`;
CREATE TABLE `mail-0` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `mail_id` int(11) NOT NULL DEFAULT '0',
  `type` tinyint(4) NOT NULL DEFAULT '0',
  `affix_state` tinyint(4) NOT NULL DEFAULT '0',
  `read_state` tinyint(4) NOT NULL DEFAULT '0',
  `title` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `create_time` bigint(20) NOT NULL DEFAULT '0',
  `context` text NOT NULL,
  `addresser` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `item1` int(11) NOT NULL DEFAULT '0',
  `item1_num` int(11) NOT NULL DEFAULT '0',
  `item2` int(11) NOT NULL DEFAULT '0',
  `item2_num` int(11) NOT NULL DEFAULT '0',
  `item3` int(11) NOT NULL DEFAULT '0',
  `item3_num` int(11) NOT NULL DEFAULT '0',
  `item4` int(11) NOT NULL DEFAULT '0',
  `item4_num` int(11) NOT NULL DEFAULT '0',
  `item5` int(11) NOT NULL DEFAULT '0',
  `item5_num` int(11) NOT NULL DEFAULT '0',
  `gold` int(11) NOT NULL DEFAULT '0',
  `freedom_dollars` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`,`mail_id`),
  KEY `actor+type` (`actor`,`type`),
  KEY `actor+type+create_time` (`actor`,`type`,`create_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for mail-1
-- ----------------------------
DROP TABLE IF EXISTS `mail-1`;
CREATE TABLE `mail-1` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `mail_id` int(11) NOT NULL DEFAULT '0',
  `type` tinyint(4) NOT NULL DEFAULT '0',
  `affix_state` tinyint(4) NOT NULL DEFAULT '0',
  `read_state` tinyint(4) NOT NULL DEFAULT '0',
  `title` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `create_time` bigint(20) NOT NULL DEFAULT '0',
  `context` text NOT NULL,
  `addresser` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `item1` int(11) NOT NULL DEFAULT '0',
  `item1_num` int(11) NOT NULL DEFAULT '0',
  `item2` int(11) NOT NULL DEFAULT '0',
  `item2_num` int(11) NOT NULL DEFAULT '0',
  `item3` int(11) NOT NULL DEFAULT '0',
  `item3_num` int(11) NOT NULL DEFAULT '0',
  `item4` int(11) NOT NULL DEFAULT '0',
  `item4_num` int(11) NOT NULL DEFAULT '0',
  `item5` int(11) NOT NULL DEFAULT '0',
  `item5_num` int(11) NOT NULL DEFAULT '0',
  `gold` int(11) NOT NULL DEFAULT '0',
  `freedom_dollars` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`,`mail_id`),
  KEY `actor+type` (`actor`,`type`),
  KEY `actor+type+create_time` (`actor`,`type`,`create_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for buff-0
-- ----------------------------
DROP TABLE IF EXISTS `buff-0`;
CREATE TABLE `buff-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `buff_id` int(11) NOT NULL DEFAULT '0',
  `left_time` int(11) NOT NULL DEFAULT '0',
  `stack_count` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`buff_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for buff-1
-- ----------------------------
DROP TABLE IF EXISTS `buff-1`;
CREATE TABLE `buff-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `buff_id` int(11) NOT NULL DEFAULT '0',
  `left_time` int(11) NOT NULL DEFAULT '0',
  `stack_count` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`buff_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for cooling-0
-- ----------------------------
DROP TABLE IF EXISTS `cooling-0`;
CREATE TABLE `cooling-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `cooling_id` int(11) NOT NULL DEFAULT '0',
  `cooling_time` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`cooling_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for cooling-1
-- ----------------------------
DROP TABLE IF EXISTS `cooling-1`;
CREATE TABLE `cooling-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `cooling_id` int(11) NOT NULL DEFAULT '0',
  `cooling_time` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`cooling_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for equip-item-0
-- ----------------------------
DROP TABLE IF EXISTS `equip-item-0`;
CREATE TABLE `equip-item-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for equip-item-1
-- ----------------------------
DROP TABLE IF EXISTS `equip-item-1`;
CREATE TABLE `equip-item-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for gem-0
-- ----------------------------
DROP TABLE IF EXISTS `gem-0`;
CREATE TABLE `gem-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location_type` int(11) NOT NULL DEFAULT '0',
  `location_index` smallint(6) NOT NULL DEFAULT '0',
  `gem_id` int(11) NOT NULL DEFAULT '0',
  `gem_level` int(11) NOT NULL DEFAULT '0',
  `gem_exp` int(11) NOT NULL DEFAULT '0',
  `locked` tinyint(4) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location_type`,`location_index`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for gem-1
-- ----------------------------
DROP TABLE IF EXISTS `gem-1`;
CREATE TABLE `gem-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location_type` int(11) NOT NULL DEFAULT '0',
  `location_index` smallint(6) NOT NULL DEFAULT '0',
  `gem_id` int(11) NOT NULL DEFAULT '0',
  `gem_level` int(11) NOT NULL DEFAULT '0',
  `gem_exp` int(11) NOT NULL DEFAULT '0',
  `locked` tinyint(4) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location_type`,`location_index`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for guild-0
-- ----------------------------
DROP TABLE IF EXISTS `guild-0`;
CREATE TABLE `guild-0` (
  `id` bigint(20) NOT NULL DEFAULT '0',
  `name` varchar(255) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `logo` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '0',
  `suppress_join` int(11) NOT NULL DEFAULT '0',
  `pending_members` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `members` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `announcement` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `events` varchar(10240) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `gold` int(11) NOT NULL DEFAULT '0',
  `light_crystal` int(11) NOT NULL DEFAULT '0',
  `dark_crystal` int(11) NOT NULL DEFAULT '0',
  `holy_crystal` int(11) NOT NULL DEFAULT '0',
  `light_attribute` int(11) NOT NULL DEFAULT '0',
  `dark_attribute` int(11) NOT NULL DEFAULT '0',
  `holy_attribute` int(11) NOT NULL DEFAULT '0',
  `shop_level` int(11) NOT NULL DEFAULT '0',
  `college_level` int(11) NOT NULL DEFAULT '0',
  `barrack_level` int(11) NOT NULL DEFAULT '0',
  `opened_playing_groups` varchar(128) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `finished_playings` varchar(256) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `last_save_time` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for guild-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `guild-actor-0`;
CREATE TABLE `guild-actor-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `guild_id` bigint(20) NOT NULL DEFAULT '0',
  `guild_position` int(11) NOT NULL DEFAULT '0',
  `current_contribution_value` int(11) NOT NULL DEFAULT '0',
  `total_contribution_value` int(11) NOT NULL DEFAULT '0',
  `gold_contribution` int(11) NOT NULL DEFAULT '0',
  `freedom_dollars_contribution` int(11) NOT NULL DEFAULT '0',
  `light_crystal_contribution` int(11) NOT NULL DEFAULT '0',
  `dark_crystal_contribution` int(11) NOT NULL DEFAULT '0',
  `holy_crystal_contribution` int(11) NOT NULL DEFAULT '0',
  `guild_skills` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `guild_buffs` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `awarded_guild_playings` varchar(256) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for guild-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `guild-actor-1`;
CREATE TABLE `guild-actor-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `guild_id` bigint(20) NOT NULL DEFAULT '0',
  `guild_position` int(11) NOT NULL DEFAULT '0',
  `current_contribution_value` int(11) NOT NULL DEFAULT '0',
  `total_contribution_value` int(11) NOT NULL DEFAULT '0',
  `gold_contribution` int(11) NOT NULL DEFAULT '0',
  `freedom_dollars_contribution` int(11) NOT NULL DEFAULT '0',
  `light_crystal_contribution` int(11) NOT NULL DEFAULT '0',
  `dark_crystal_contribution` int(11) NOT NULL DEFAULT '0',
  `holy_crystal_contribution` int(11) NOT NULL DEFAULT '0',
  `guild_skills` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `guild_buffs` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `awarded_guild_playings` varchar(256) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for item-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `item-actor-0`;
CREATE TABLE `item-actor-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `gem_gathering_point` int(11) NOT NULL DEFAULT '0',
  `shortcut1` int(11) NOT NULL DEFAULT '0',
  `shortcut2` int(11) NOT NULL DEFAULT '0',
  `shortcut3` int(11) NOT NULL DEFAULT '0',
  `shortcut4` int(11) NOT NULL DEFAULT '0',
  `gifts_pick_num` int(11) NOT NULL DEFAULT '0',
  `item_buy_num` text NOT NULL,
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for item-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `item-actor-1`;
CREATE TABLE `item-actor-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `gem_gathering_point` int(11) NOT NULL DEFAULT '0',
  `shortcut1` int(11) NOT NULL DEFAULT '0',
  `shortcut2` int(11) NOT NULL DEFAULT '0',
  `shortcut3` int(11) NOT NULL DEFAULT '0',
  `shortcut4` int(11) NOT NULL DEFAULT '0',
  `gifts_pick_num` int(11) NOT NULL DEFAULT '0',
  `item_buy_num` text NOT NULL,
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for packet-item-0
-- ----------------------------
DROP TABLE IF EXISTS `packet-item-0`;
CREATE TABLE `packet-item-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for packet-item-1
-- ----------------------------
DROP TABLE IF EXISTS `packet-item-1`;
CREATE TABLE `packet-item-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for playing-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `playing-actor-0`;
CREATE TABLE `playing-actor-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `records` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `results` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `auto_playing` int(11) NOT NULL DEFAULT '0',
  `auto_playing_group` int(11) NOT NULL DEFAULT '0',
  `auto_finish_time` bigint(20) NOT NULL DEFAULT '0',
  `maze_ap` int(11) NOT NULL DEFAULT '0',
  `maze_last_ap_recover_time` bigint(20) NOT NULL DEFAULT '0',
  `maze_floor` int(11) NOT NULL DEFAULT '0',
  `maze_explored_area` varchar(128) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `maze_recent_position` varchar(128) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `maze_gift_number` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for playing-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `playing-actor-1`;
CREATE TABLE `playing-actor-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `records` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `results` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `auto_playing` int(11) NOT NULL DEFAULT '0',
  `auto_playing_group` int(11) NOT NULL DEFAULT '0',
  `auto_finish_time` bigint(20) NOT NULL DEFAULT '0',
  `maze_ap` int(11) NOT NULL DEFAULT '0',
  `maze_last_ap_recover_time` bigint(20) NOT NULL DEFAULT '0',
  `maze_floor` int(11) NOT NULL DEFAULT '0',
  `maze_explored_area` varchar(128) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `maze_recent_position` varchar(128) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `maze_gift_number` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for skill-0
-- ----------------------------
DROP TABLE IF EXISTS `skill-0`;
CREATE TABLE `skill-0` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `actor_id` bigint(20) NOT NULL,
  `skill_id` int(11) NOT NULL,
  `get_at` datetime NOT NULL DEFAULT '2000-01-01 00:00:00' COMMENT '技能学习时间',
  `current_level` smallint(6) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `NewIndex1` (`actor_id`,`skill_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for skill-1
-- ----------------------------
DROP TABLE IF EXISTS `skill-1`;
CREATE TABLE `skill-1` (
  `actor_id` bigint(20) NOT NULL,
  `skill_id` int(11) NOT NULL,
  `get_at` datetime NOT NULL DEFAULT '2000-01-01 00:00:00' COMMENT '技能学习时间',
  `current_level` smallint(6) NOT NULL DEFAULT '1',
  PRIMARY KEY (`actor_id`,`skill_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for social-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `social-actor-0`;
CREATE TABLE `social-actor-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `signature` varchar(256) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `friends` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `blacklist` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `enemies` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_upgrade_start_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_upgrade_finish_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_temple_level` int(11) NOT NULL DEFAULT '0',
  `homeland_temple_harvest_times` int(11) NOT NULL DEFAULT '0',
  `homeland_temple_force_harvest_times` int(11) NOT NULL DEFAULT '0',
  `homeland_temple_next_harvest_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_goldmine_level` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_robbed_times` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_loss_rate` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_next_harvest_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_goldmine_rob_times` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_next_rob_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_pool_level` int(11) NOT NULL DEFAULT '0',
  `homeland_pool_last_harvest_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_church_free_pray_times` int(11) NOT NULL DEFAULT '0',
  `homeland_church_paid_pray_times` int(11) NOT NULL DEFAULT '0',
  `homeland_church_free_refresh_times` int(11) NOT NULL DEFAULT '0',
  `homeland_church_chest_type` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_level` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_harvest_times` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_stage` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_watered_times` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_next_water_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_tree_growing_finish_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_tree_assist_water_times` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_watered_actors` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_tree_stolen_actors` varchar(256) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_tower1_level` int(11) NOT NULL DEFAULT '0',
  `homeland_tower2_level` int(11) NOT NULL DEFAULT '0',
  `homeland_stronghold_level` int(11) NOT NULL DEFAULT '0',
  `homeland_stronghold_next_call_support_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_stronghold_supported_souls` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_stronghold_support_souls` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_stronghold_support_times` int(11) NOT NULL DEFAULT '0',
  `homeland_invaders` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_events` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `last_save_time` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for social-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `social-actor-1`;
CREATE TABLE `social-actor-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `signature` varchar(256) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `friends` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `blacklist` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `enemies` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_upgrade_start_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_upgrade_finish_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_temple_level` int(11) NOT NULL DEFAULT '0',
  `homeland_temple_harvest_times` int(11) NOT NULL DEFAULT '0',
  `homeland_temple_force_harvest_times` int(11) NOT NULL DEFAULT '0',
  `homeland_temple_next_harvest_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_goldmine_level` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_robbed_times` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_loss_rate` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_next_harvest_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_goldmine_rob_times` int(11) NOT NULL DEFAULT '0',
  `homeland_goldmine_next_rob_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_pool_level` int(11) NOT NULL DEFAULT '0',
  `homeland_pool_last_harvest_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_church_free_pray_times` int(11) NOT NULL DEFAULT '0',
  `homeland_church_paid_pray_times` int(11) NOT NULL DEFAULT '0',
  `homeland_church_free_refresh_times` int(11) NOT NULL DEFAULT '0',
  `homeland_church_chest_type` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_level` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_harvest_times` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_stage` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_watered_times` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_next_water_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_tree_growing_finish_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_tree_assist_water_times` int(11) NOT NULL DEFAULT '0',
  `homeland_tree_watered_actors` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_tree_stolen_actors` varchar(256) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_tower1_level` int(11) NOT NULL DEFAULT '0',
  `homeland_tower2_level` int(11) NOT NULL DEFAULT '0',
  `homeland_stronghold_level` int(11) NOT NULL DEFAULT '0',
  `homeland_stronghold_next_call_support_time` bigint(20) NOT NULL DEFAULT '0',
  `homeland_stronghold_supported_souls` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_stronghold_support_souls` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_stronghold_support_times` int(11) NOT NULL DEFAULT '0',
  `homeland_invaders` varchar(2048) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `homeland_events` varchar(4096) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `last_save_time` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for soul-0
-- ----------------------------
DROP TABLE IF EXISTS `soul-0`;
CREATE TABLE `soul-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `id` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '0',
  `step` int(11) NOT NULL DEFAULT '0',
  `common_skill_level` int(11) NOT NULL DEFAULT '0',
  `appear_skill_level` int(11) NOT NULL DEFAULT '0',
  `soul_skill_level` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`, `id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for soul-1
-- ----------------------------
DROP TABLE IF EXISTS `soul-1`;
CREATE TABLE `soul-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `id` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '0',
  `step` int(11) NOT NULL DEFAULT '0',
  `common_skill_level` int(11) NOT NULL DEFAULT '0',
  `appear_skill_level` int(11) NOT NULL DEFAULT '0',
  `soul_skill_level` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`, `id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for soul-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `soul-actor-0`;
CREATE TABLE `soul-actor-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `axe_setting` int(11) NOT NULL DEFAULT '0',
  `shoot_setting` int(11) NOT NULL DEFAULT '0',
  `magic_setting` int(11) NOT NULL DEFAULT '0',
  `settings` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `altar_summon_paid` tinyint(4) NOT NULL DEFAULT '0',
  `altar_free_summon_times` int(11) NOT NULL DEFAULT '0',
  `altar_cards` varchar(128) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `altar_card_bind` tinyint(4) NOT NULL DEFAULT '0',
  `current_energy` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for soul-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `soul-actor-1`;
CREATE TABLE `soul-actor-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `axe_setting` int(11) NOT NULL DEFAULT '0',
  `shoot_setting` int(11) NOT NULL DEFAULT '0',
  `magic_setting` int(11) NOT NULL DEFAULT '0',
  `settings` varchar(1024) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `altar_summon_paid` tinyint(4) NOT NULL DEFAULT '0',
  `altar_free_summon_times` int(11) NOT NULL DEFAULT '0',
  `altar_cards` varchar(128) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `altar_card_bind` tinyint(4) NOT NULL DEFAULT '0',
  `current_energy` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for spirit-facade-0
-- ----------------------------
DROP TABLE IF EXISTS `spirit-facade-0`;
CREATE TABLE `spirit-facade-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `spirit_facade_id` int(11) NOT NULL DEFAULT '0',
  `expire_time` bigint(20) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`spirit_facade_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for spirit-facade-1
-- ----------------------------
DROP TABLE IF EXISTS `spirit-facade-1`;
CREATE TABLE `spirit-facade-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `spirit_facade_id` int(11) NOT NULL DEFAULT '0',
  `expire_time` bigint(20) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`spirit_facade_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for storage-item-0
-- ----------------------------
DROP TABLE IF EXISTS `storage-item-0`;
CREATE TABLE `storage-item-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for storage-item-1
-- ----------------------------
DROP TABLE IF EXISTS `storage-item-1`;
CREATE TABLE `storage-item-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for treasure-item-0
-- ----------------------------
DROP TABLE IF EXISTS `treasure-item-0`;
CREATE TABLE `treasure-item-0` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for treasure-item-1
-- ----------------------------
DROP TABLE IF EXISTS `treasure-item-1`;
CREATE TABLE `treasure-item-1` (
  `actor_id` bigint(20) NOT NULL DEFAULT '0',
  `location` smallint(6) NOT NULL DEFAULT '0',
  `vocation` int(11) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `bind` tinyint(4) NOT NULL DEFAULT '0',
  `intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `max_intensify_level` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr1` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value1` int(11) NOT NULL DEFAULT '0',
  `random_attr2` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value2` int(11) NOT NULL DEFAULT '0',
  `random_attr3` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value3` int(11) NOT NULL DEFAULT '0',
  `random_attr4` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value4` int(11) NOT NULL DEFAULT '0',
  `random_attr5` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value5` int(11) NOT NULL DEFAULT '0',
  `random_attr6` tinyint(4) NOT NULL DEFAULT '0',
  `random_attr_value6` int(11) NOT NULL DEFAULT '0',
  `exist` tinyint(4) NOT NULL DEFAULT '0',
  `first_use_time` bigint(20) NOT NULL DEFAULT '0',
  `upgrade_lucky` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor_id`,`location`, `vocation`),
  KEY `actor_id` (`actor_id`),
  KEY `actor_id+exist` (`actor_id`,`exist`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for task-0
-- ----------------------------
DROP TABLE IF EXISTS `task-0`;
CREATE TABLE `task-0` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `task` int(11) NOT NULL DEFAULT '0',
  `flag` tinyint(4) NOT NULL DEFAULT '0',
  `cond_type1` tinyint(4) NOT NULL DEFAULT '0',
  `var1` int(11) NOT NULL DEFAULT '0',
  `value1` int(11) NOT NULL DEFAULT '0',
  `cond_type2` tinyint(4) NOT NULL DEFAULT '0',
  `var2` int(11) NOT NULL DEFAULT '0',
  `value2` int(11) NOT NULL DEFAULT '0',
  `cond_type3` tinyint(4) NOT NULL DEFAULT '0',
  `var3` int(11) NOT NULL DEFAULT '0',
  `value3` int(11) NOT NULL DEFAULT '0',
  `task_type` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`,`task`,`task_type`),
  KEY `actor+flag` (`actor`,`flag`,`task_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for task-1
-- ----------------------------
DROP TABLE IF EXISTS `task-1`;
CREATE TABLE `task-1` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `task` int(11) NOT NULL DEFAULT '0',
  `flag` tinyint(4) NOT NULL DEFAULT '0',
  `cond_type1` tinyint(4) NOT NULL DEFAULT '0',
  `var1` int(11) NOT NULL DEFAULT '0',
  `value1` int(11) NOT NULL DEFAULT '0',
  `cond_type2` tinyint(4) NOT NULL DEFAULT '0',
  `var2` int(11) NOT NULL DEFAULT '0',
  `value2` int(11) NOT NULL DEFAULT '0',
  `cond_type3` tinyint(4) NOT NULL DEFAULT '0',
  `var3` int(11) NOT NULL DEFAULT '0',
  `value3` int(11) NOT NULL DEFAULT '0',
  `task_type` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`,`task`,`task_type`),
  KEY `actor+flag` (`actor`,`flag`,`task_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for achive-0
-- ----------------------------
DROP TABLE IF EXISTS `achieve-0`;
CREATE TABLE `achieve-0` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `achieve_id` int(11) NOT NULL DEFAULT '0',
  `flag` tinyint(4) NOT NULL DEFAULT '0',
  `condition1_param1` tinyint(4) NOT NULL DEFAULT '0',
  `condition1_param2` int(11) NOT NULL DEFAULT '0',
  `condition1_value` int(11) NOT NULL DEFAULT '0',
  `condition2_param1` int(11) NOT NULL DEFAULT '0',
  `condition2_param2` int(11) NOT NULL DEFAULT '0',
  `condition2_value` int(11) NOT NULL DEFAULT '0',
  `finish_time` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`,`achieve_id`),
  KEY `actor+flag` (`actor`,`flag`,`achieve_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for achive-1
-- ----------------------------
DROP TABLE IF EXISTS `achieve-1`;
CREATE TABLE `achieve-1` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `achieve_id` int(11) NOT NULL DEFAULT '0',
  `flag` tinyint(4) NOT NULL DEFAULT '0',
  `condition1_param1` tinyint(4) NOT NULL DEFAULT '0',
  `condition1_param2` int(11) NOT NULL DEFAULT '0',
  `condition1_value` int(11) NOT NULL DEFAULT '0',
  `condition2_param1` int(11) NOT NULL DEFAULT '0',
  `condition2_param2` int(11) NOT NULL DEFAULT '0',
  `condition2_value` int(11) NOT NULL DEFAULT '0',
  `finish_time`  bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`,`achieve_id`),
  KEY `actor+flag` (`actor`,`flag`,`achieve_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- Table structure for welfare-actor-0
-- ----------------------------
DROP TABLE IF EXISTS `welfare-actor-0`;
CREATE TABLE `welfare-actor-0` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `title_id` int(11) NOT NULL DEFAULT '0',
  `daily_checkins` int(11) NOT NULL DEFAULT '0',
  `daily_award` tinyint(4) NOT NULL DEFAULT '0',
  `general_checkins` int(11) NOT NULL DEFAULT '0',
  `vip_checkins` int(11) NOT NULL DEFAULT '0',
  `last_month_resource` int(11) NOT NULL DEFAULT '0',
  `current_month_resource` int(11) NOT NULL DEFAULT '0',
  `last_month_award_resource` tinyint(4) NOT NULL DEFAULT '0',
  `daily_time` int(11) NOT NULL DEFAULT '0',
  `total_time` int(11) NOT NULL DEFAULT '0',
  `replenish_checkins_num` int(11) NOT NULL DEFAULT '0',
  `online_award_items` varchar(20) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `respenish_days` varchar(20) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- Table structure for welfare-actor-1
-- ----------------------------
DROP TABLE IF EXISTS `welfare-actor-1`;
CREATE TABLE `welfare-actor-1` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `title_id` int(11) NOT NULL DEFAULT '0',
   `daily_checkins` int(11) NOT NULL DEFAULT '0',
  `daily_award` tinyint(4) NOT NULL DEFAULT '0',
  `general_checkins` int(11) NOT NULL DEFAULT '0',
  `vip_checkins` int(11) NOT NULL DEFAULT '0',
  `last_month_resource` int(11) NOT NULL DEFAULT '0',
  `current_month_resource` int(11) NOT NULL DEFAULT '0',
  `last_month_award_resource` tinyint(4) NOT NULL DEFAULT '0',
  `daily_time` int(11) NOT NULL DEFAULT '0',
  `total_time` int(11) NOT NULL DEFAULT '0',
  `replenish_checkins_num` int(11) NOT NULL DEFAULT '0',
  `online_award_items` varchar(20) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `respenish_days` varchar(20) BINARY CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`actor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for task_actor-0
-- ----------------------------
DROP TABLE IF EXISTS `task_actor-0`;
CREATE TABLE `task_actor-0` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `circle_flag` tinyint(4) NOT NULL DEFAULT '0',
  `round_num` int(11) NOT NULL DEFAULT '0',
  `total_round` int(11) NOT NULL DEFAULT '0',
  `circle_num` int(11) NOT NULL DEFAULT '0',
  `lv_stage` int(11) NOT NULL DEFAULT '0',
  `condition_id` int(11) NOT NULL DEFAULT '0',
  `trip_num` int(11) NOT NULL DEFAULT '0',
  `exploit_award` int(11) NOT NULL DEFAULT '0',
  `daily_exploit` int(11) NOT NULL DEFAULT '0',
  `transport_finished_count` int(11) NOT NULL DEFAULT '0',
  `transport_free_refreshed_count` int(11) NOT NULL DEFAULT '0',
  `transport_robed_count` int(11) NOT NULL DEFAULT '0',
  `transport_current_transport` int(11) NOT NULL DEFAULT '0',
  `transport_status` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for task_actor-1
-- ----------------------------
DROP TABLE IF EXISTS `task_actor-1`;
CREATE TABLE `task_actor-1` (
  `actor` bigint(20) NOT NULL DEFAULT '0',
  `circle_flag` tinyint(4) NOT NULL DEFAULT '0',
  `round_num` int(11) NOT NULL DEFAULT '0',
  `total_round` int(11) NOT NULL DEFAULT '0',
  `circle_num` int(11) NOT NULL DEFAULT '0',
  `lv_stage` int(11) NOT NULL DEFAULT '0',
  `condition_id` int(11) NOT NULL DEFAULT '0',
  `trip_num` int(11) NOT NULL DEFAULT '0',
  `exploit_award` int(11) NOT NULL DEFAULT '0',
  `daily_exploit` int(11) NOT NULL DEFAULT '0',
  `transport_finished_count` int(11) NOT NULL DEFAULT '0',
  `transport_free_refreshed_count` int(11) NOT NULL DEFAULT '0',
  `transport_robed_count` int(11) NOT NULL DEFAULT '0',
  `transport_current_transport` int(11) NOT NULL DEFAULT '0',
  `transport_status` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`actor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for world-data
-- ----------------------------
DROP TABLE IF EXISTS `world-data`;
CREATE TABLE `world-data` (
  `id` int(11) NOT NULL DEFAULT '0', 
  `data` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Procedure structure for GAME_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ACTOR_LOGIN`(IN `id` bigint)
BEGIN

  DECLARE max_table_number int DEFAULT 2;

  SET @param_id = id;

  SET @ret_code = 0;
  SET @ret_name = NULL;
  SET @ret_level = 0;
  SET @ret_vocation = 0;
  SET @ret_gender = 0;
  SET @ret_head = 0;
  SET @ret_hair = 0;
  SET @ret_current_hp = 0;
  SET @ret_current_mp = 0;
  SET @ret_current_exp = 0;
  SET @ret_nimbus = 0;
  SET @ret_gold = 0;
  SET @ret_freedom_dollars = 0;
  SET @ret_restrict_dollars = 0;
  SET @ret_honour = 0;
  SET @ret_war_soul = 0;
  SET @ret_gem_shard = 0;
  SET @ret_justice = 0;
  SET @ret_miracle_integral = 0;
  SET @ret_legend_integral = 0;
  SET @ret_arena_integral = 0;
  SET @ret_map = 0;
  SET @ret_x = 0;
  SET @ret_y = 0;
  SET @ret_pre_map = 0;
  SET @ret_pre_x = 0;
  SET @ret_pre_y = 0;
  SET @ret_brown_name = 0;
  SET @ret_pk_mode = 0;
  SET @ret_functionality_state = NULL;
  SET @ret_packet_capacity = 0;
  SET @ret_storage_capacity = 0;
  SET @ret_axe_fight_score = 0;
  SET @ret_shoot_fight_score = 0;
  SET @ret_magic_fight_score = 0;
  SET @ret_last_logout_time = 0;
  SET @ret_vip_type = 0;
  SET @ret_vip_remainder_time = 0;
  SET @ret_world_jump_vip_num = 0;
  SET @ret_fcm_type = 0;
  SET @ret_fcm_online_time = 0;
  SET @ret_create_time = 0;
  SET @ret_hp_tasliman = 0;
  SET @ret_mp_tasliman = 0;


  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);

  START TRANSACTION;

  SET @sql = CONCAT('select count(1), ',
                    '`name`, ',
                    '`level`, ',
                    '`vocation`, ',
                    '`gender`, ',
                    '`head`, ',
                    '`hair`, ',
                    '`current_hp`, ',
                    '`current_mp`, ',
                    '`current_exp`, ',
                    '`nimbus`, ',
                    '`gold`, ',
                    '`freedom_dollars`, ',
                    '`restrict_dollars`, ',
                    '`honour`, ',
                    '`war_soul`, ',
                    '`gem_shard`, ',
                    '`justice`, ',
                    '`miracle_integral`, ',
                    '`legend_integral`, ',
                    '`arena_integral`, ',
                    '`map`, ',
                    '`x`, ',
                    '`y`, ',
                    '`pre_map`, ',
                    '`pre_x`, ',
                    '`pre_y`, ',
                    '`brown_name`, ',
                    '`pk_mode`, ',
                    '`functionality_state`, ',
                    '`packet_capacity`, ',
                    '`storage_capacity`, ',
                    '`axe_fight_score`, ',
                    '`shoot_fight_score`, ',
                    '`magic_fight_score`, ',
                    '`last_logout_time`, ',
                    '`vip_type`, ',
                    '`vip_remainder_time`, ',
                    '`world_jump_vip_num`, ',
                    '`fcm_type`, ',
                    '`fcm_online_time`, ',
                    '`create_time`, ',
                    '`hp_tasliman`, ',
                    '`mp_tasliman`',
                    ' into @count, ',
                    '@ret_name, ',
                    '@ret_level, ',
                    '@ret_vocation, ',
                    '@ret_gender, ',
                    '@ret_head, ',
                    '@ret_hair, ',
                    '@ret_current_hp, ',
                    '@ret_current_mp, ',
                    '@ret_current_exp, ',
                    '@ret_nimbus, ',
                    '@ret_gold, ',
                    '@ret_freedom_dollars, ',
                    '@ret_restrict_dollars, ',
                    '@ret_honour, ',
                    '@ret_war_soul, ',
                    '@ret_gem_shard, ',
                    '@ret_justice, ',
                    '@ret_miracle_integral, ',
                    '@ret_legend_integral, ',
                    '@ret_arena_integral, ',
                    '@ret_map, ',
                    '@ret_x, ',
                    '@ret_y, ',
                    '@ret_pre_map, ',
                    '@ret_pre_x, ',
                    '@ret_pre_y, ',
                    '@ret_brown_name, ',
                    '@ret_pk_mode, ',
                    '@ret_functionality_state, ',
                    '@ret_packet_capacity, ',
                    '@ret_storage_capacity, ',
                    '@ret_axe_fight_score, ',
                    '@ret_shoot_fight_score, ',
                    '@ret_magic_fight_score, ',
                    '@ret_last_logout_time, ',
                    '@ret_vip_type, ',
                    '@ret_vip_remainder_time, ',
                    '@ret_world_jump_vip_num, ',
                    '@ret_fcm_type, ',
                    '@ret_fcm_online_time, ',
                    '@ret_create_time, ',
                    '@ret_hp_tasliman, ',
                    '@ret_mp_tasliman',
                    ' from `',
      @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  COMMIT;

  IF @count = 1 THEN
    SET @ret_code = 0;
    SELECT @ret_code,
           @ret_name,
           @ret_level,
           @ret_vocation,
           @ret_gender,
           @ret_head,
           @ret_hair,
           @ret_current_hp,
           @ret_current_mp,
           @ret_current_exp,
           @ret_nimbus,
           @ret_gold,
           @ret_freedom_dollars,
           @ret_restrict_dollars,
           @ret_honour,
           @ret_war_soul,
           @ret_gem_shard,
           @ret_justice,
           @ret_miracle_integral,
           @ret_legend_integral,
           @ret_arena_integral,
           @ret_map,
           @ret_x,
           @ret_y,
           @ret_pre_map,
           @ret_pre_x,
           @ret_pre_y,
           @ret_brown_name,
           @ret_pk_mode,
           @ret_functionality_state,
           @ret_packet_capacity,
           @ret_storage_capacity,
           @ret_axe_fight_score,
           @ret_shoot_fight_score,
           @ret_magic_fight_score,
           @ret_last_logout_time,
           @ret_vip_type,
           @ret_vip_remainder_time,
           @ret_world_jump_vip_num,
           @ret_fcm_type,
           @ret_fcm_online_time,
           @ret_create_time,
           @ret_hp_tasliman,
           @ret_mp_tasliman;
  ELSE
    SET @ret_code = -1;
    SELECT @ret_code;
  END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ACTOR_SAVE`(IN `id` bigint,
    IN `level` int,
    IN `vocation` int,
    IN `gender` tinyint,
    IN `head` tinyint,
    IN `hair` tinyint,
    IN `current_hp` int,
    IN `current_mp` int,
    IN `current_exp` int,
    IN `nimbus` int,
    IN `gold` int,
    IN `freedom_dollars` int,
    IN `restrict_dollars` int,
    IN `honour` int,
    IN `war_soul` int,
    IN `gem_shard` int,
    IN `justice` int,
    IN `miracle_integral` int,
    IN `legend_integral` int,
    IN `arena_integral` int,
    IN `map` int,
    IN `x` int,
    IN `y` int,
    IN `pre_map` int,
    IN `pre_x` int,
    IN `pre_y` int,
    IN `brown_name` int,
    IN `pk_mode` int,
    IN `functionality_state` varchar(255),
    IN `packet_capacity` int,
    IN `storage_capacity` int,
    IN `axe_fight_score` int,
    IN `shoot_fight_score` int,
    IN `magic_fight_score` int,
    IN `last_logout_time` int,
    IN `vip_type` tinyint,
    IN `vip_remainder_time` int,
    IN `world_jump_vip_num` int,
    IN `fcm_type` int,
    IN `fcm_online_time` int,
	IN `create_time` bigint,
	IN `hp_tasliman` int,
	IN `mp_tasliman` int)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_level = level;
  SET @param_vocation = vocation;
  SET @param_gender = gender;
  SET @param_head = head;
  SET @param_hair = hair;
  SET @param_current_hp = current_hp;
  SET @param_current_mp = current_mp;
  SET @param_current_exp = current_exp;
  SET @param_nimbus = nimbus;
  SET @param_gold = gold;
  SET @param_freedom_dollars = freedom_dollars;
  SET @param_restrict_dollars = restrict_dollars;
  SET @param_honour = honour;
  SET @param_war_soul = war_soul;
  SET @param_gem_shard = gem_shard;
  SET @param_justice = justice;
  SET @param_miracle_integral = miracle_integral;
  SET @param_legend_integral = legend_integral;
  SET @param_arena_integral = arena_integral;
  SET @param_map = map;
  SET @param_x = x;
  SET @param_y = y;
  SET @param_pre_map = pre_map;
  SET @param_pre_x = pre_x;
  SET @param_pre_y = pre_y;
  SET @param_brown_name = brown_name;
  SET @param_pk_mode = pk_mode;
  SET @param_functionality_state = functionality_state;
  SET @param_packet_capacity = packet_capacity;
  SET @param_storage_capacity = storage_capacity;
  SET @param_axe_fight_score = axe_fight_score;
  SET @param_shoot_fight_score = shoot_fight_score;
  SET @param_magic_fight_score = magic_fight_score;
  SET @param_last_logout_time = last_logout_time;
  SET @param_vip_type = vip_type;
  SET @param_vip_remainder_time = vip_remainder_time;
  SET @param_world_jump_vip_num = world_jump_vip_num;
  SET @param_fcm_type = fcm_type;
  SET @param_fcm_online_time = fcm_online_time;
  SET @param_create_time = create_time;
  SET @param_hp_tasliman = hp_tasliman;
  SET @param_mp_tasliman = mp_tasliman;
  SET @ret_code = 0;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
  IF @count = 0 THEN
    SET @ret_code = -1;
  ELSE
    SET @sql = CONCAT('update `', @table_name, '` set ',
                      '`level` = ?, ',
                      '`vocation` = ?, ',
                      '`gender` = ?, ',
                      '`head` = ?, ',
                      '`hair` = ?, ',
                      '`current_hp` = ?, ',
                      '`current_mp` = ?, ',
                      '`current_exp` = ?, ',
                      '`nimbus` = ?, ',
                      '`gold` = ?, ',
                      '`freedom_dollars` = ?, ',
                      '`restrict_dollars` = ?, ',
                      '`honour` = ?, ',
                      '`war_soul` = ?, ',
                      '`gem_shard` = ?, ',
                      '`justice` = ?, ',
                      '`miracle_integral` = ?, ',
                      '`legend_integral` = ?, ',
                      '`arena_integral` = ?, ',
                      '`map` = ?, ',
                      '`x` = ?, ',
                      '`y` = ?, ',
                      '`pre_map` = ?, ',
                      '`pre_x` = ?, ',
                      '`pre_y` = ?, ',
                      '`brown_name` = ?, ',
                      '`pk_mode` = ? , ',
                      '`functionality_state` = ? , ',
                      '`packet_capacity` = ?, ',
                      '`storage_capacity` = ?,',
                      '`axe_fight_score` = ?,',
                      '`shoot_fight_score` = ?,',
                      '`magic_fight_score` = ?,',
                      '`last_logout_time` = ?,',
                      '`vip_type` = ?, ',
                      '`vip_remainder_time` = ?, ',
                      '`world_jump_vip_num` = ?, ',
                      '`fcm_type` = ?, ',
                      '`fcm_online_time` = ?, ',
                      '`create_time` = ?, ',
                      '`hp_tasliman` = ?, ',
                      '`mp_tasliman` = ?',
                      ' where `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_level,
                       @param_vocation,
                       @param_gender,
                       @param_head,
                       @param_hair,
                       @param_current_hp,
                       @param_current_mp,
                       @param_current_exp,
                       @param_nimbus,
                       @param_gold,
                       @param_freedom_dollars,
                       @param_restrict_dollars,
                       @param_honour,
                       @param_war_soul,
                       @param_gem_shard,
                       @param_justice,
                       @param_miracle_integral,
                       @param_legend_integral,
                       @param_arena_integral,
                       @param_map,
                       @param_x,
                       @param_y,
                       @param_pre_map,
                       @param_pre_x,
                       @param_pre_y,
                       @param_brown_name,
                       @param_pk_mode,
                       @param_functionality_state,
                       @param_packet_capacity,
                       @param_storage_capacity,
                       @param_axe_fight_score,
                       @param_shoot_fight_score,
                       @param_magic_fight_score,
                       @param_last_logout_time,
                       @param_vip_type,
                       @param_vip_remainder_time,
                       @param_world_jump_vip_num,
                       @param_fcm_type,
                       @param_fcm_online_time,
                       @param_create_time,
                       @param_hp_tasliman,
                       @param_mp_tasliman,
                       @param_id;
    SET @ret_code = 0;
  END IF;

  COMMIT;

  SELECT @ret_code;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ATTR_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ATTR_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ATTR_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('attr-actor-', @table_number);

  SET @field_list = CONCAT('`noble_level`, ',
                           '`noble_used_item_count`, ',
                           '`noble_max_hp_add_value`, ',
                           '`noble_max_mp_add_value`, ',
                           '`noble_physics_attack_add_value`, ',
                           '`noble_physics_defence_add_value`, ',
                           '`noble_magic_attack_add_value`, ',
                           '`noble_magic_defence_add_value`, ',
                           '`noble_dodge_add_value`, ',
                           '`noble_crit_add_value`, ',
                           '`noble_ignore_defence_add_value`, ',
                           '`sword_avail_step`, ',
                           '`sword_current_step`, ',
                           '`sword_lucky_value`, ',
                           '`sword_gain_time`, ',
                           '`element_level`, ',
                           '`spirit_level`, ',
                           '`spirit_lucky_value`, ',
                           '`spirit_current_facade_type`, ',
                           '`spirit_horse_facade_id`, ',
                           '`spirit_wing_facade_id`, ',
                           '`runes`, ',
                           '`legendary_weapon_pieces`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ATTR_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ATTR_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ATTR_ACTOR_SAVE`(IN `id` bigint,
    IN `noble_level` int,
    IN `noble_used_item_count` int,
    IN `noble_max_hp_add_value` int,
    IN `noble_max_mp_add_value`int,
    IN `noble_physics_attack_add_value` int,
    IN `noble_physics_defence_add_value` int,
    IN `noble_magic_attack_add_value` int,
    IN `noble_magic_defence_add_value` int,
    IN `noble_dodge_add_value` int,
    IN `noble_crit_add_value` int,
    IN `noble_ignore_defence_add_value` int,
    IN `sword_avail_step` int,
    IN `sword_current_step` int,
    IN `sword_lucky_value` int,
    IN `sword_gain_time` int,
    IN `element_level` int,
    IN `spirit_level` int,
    IN `spirit_lucky_value` int,
    IN `spirit_current_facade_type` int,
    IN `spirit_horse_facade_id` int,
    IN `spirit_wing_facade_id` int,
    IN `runes` varchar(255),
    IN `legendary_weapon_pieces` varchar(1024))
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('attr-actor-', @table_number);

  SET @param_noble_level = noble_level;
  SET @param_noble_used_item_count = noble_used_item_count;
  SET @param_noble_max_hp_add_value = noble_max_hp_add_value;
  SET @param_noble_max_mp_add_value = noble_max_mp_add_value;
  SET @param_noble_physics_attack_add_value = noble_physics_attack_add_value;
  SET @param_noble_physics_defence_add_value = noble_physics_defence_add_value;
  SET @param_noble_magic_attack_add_value = noble_magic_attack_add_value;
  SET @param_noble_magic_defence_add_value = noble_magic_defence_add_value;
  SET @param_noble_dodge_add_value = noble_dodge_add_value;
  SET @param_noble_crit_add_value = noble_crit_add_value;
  SET @param_noble_ignore_defence_add_value = noble_ignore_defence_add_value;
  SET @param_sword_avail_step = sword_avail_step;
  SET @param_sword_current_step = sword_current_step;
  SET @param_sword_lucky_value = sword_lucky_value;
  SET @param_sword_gain_time = sword_gain_time;
  SET @param_element_level = element_level;
  SET @param_spirit_level = spirit_level;
  SET @param_spirit_lucky_value = spirit_lucky_value;
  SET @param_spirit_current_facade_type = spirit_current_facade_type;
  SET @param_spirit_horse_facade_id = spirit_horse_facade_id;
  SET @param_spirit_wing_facade_id = spirit_wing_facade_id;
  SET @param_runes = runes;
  SET @param_legendary_weapon_pieces = legendary_weapon_pieces;
 
  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor_id`, ',
                             '`noble_level`, ',
                             '`noble_used_item_count`, ',
                             '`noble_max_hp_add_value`, ',
                             '`noble_max_mp_add_value`, ',
                             '`noble_physics_attack_add_value`, ',
                             '`noble_physics_defence_add_value`, ',
                             '`noble_magic_attack_add_value`, ',
                             '`noble_magic_defence_add_value`, ',
                             '`noble_dodge_add_value`, ',
                             '`noble_crit_add_value`, ',
                             '`noble_ignore_defence_add_value`, ',
                             '`sword_avail_step`, ',
                             '`sword_current_step`, ',
                             '`sword_lucky_value`, ',
                             '`sword_gain_time`, ',
                             '`element_level`, ',
                             '`spirit_level`, ',
                             '`spirit_lucky_value`, ',
                             '`spirit_current_facade_type`, ',
                             '`spirit_horse_facade_id`, ',
                             '`spirit_wing_facade_id`, ',
                             '`runes`, ',
                             '`legendary_weapon_pieces`');

    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list,
                      ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                               '?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                               '?, ?, ?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_noble_level,
                       @param_noble_used_item_count,
                       @param_noble_max_hp_add_value,
                       @param_noble_max_mp_add_value,
                       @param_noble_physics_attack_add_value,
                       @param_noble_physics_defence_add_value,
                       @param_noble_magic_attack_add_value,
                       @param_noble_magic_defence_add_value,
                       @param_noble_dodge_add_value,
                       @param_noble_crit_add_value,
                       @param_noble_ignore_defence_add_value,
                       @param_sword_avail_step,
                       @param_sword_current_step,
                       @param_sword_lucky_value,
                       @param_sword_gain_time,
                       @param_element_level,
                       @param_spirit_level,
                       @param_spirit_lucky_value,
                       @param_spirit_current_facade_type,
                       @param_spirit_horse_facade_id,
                       @param_spirit_wing_facade_id,
                       @param_runes,
                       @param_legendary_weapon_pieces;
  ELSE
    SET @update_list = CONCAT('`noble_level` = ?, ',
                              '`noble_used_item_count` = ?, ',
                              '`noble_max_hp_add_value` = ?, ',
                              '`noble_max_mp_add_value` = ?, ',
                              '`noble_physics_attack_add_value` = ?, ',
                              '`noble_physics_defence_add_value` = ?, ',
                              '`noble_magic_attack_add_value` = ?, ',
                              '`noble_magic_defence_add_value` = ?, ',
                              '`noble_dodge_add_value` = ?, ',
                              '`noble_crit_add_value` = ?, ',
                              '`noble_ignore_defence_add_value` = ?, ',
                              '`sword_avail_step` = ?, ',
                              '`sword_current_step` = ?, ',
                              '`sword_lucky_value` = ?, ',
                              '`sword_gain_time` = ?, ',
                              '`element_level` = ?, ',
                              '`spirit_level` = ?, ',
                              '`spirit_lucky_value` = ?, ',
                              '`spirit_current_facade_type` = ?, ',
                              '`spirit_horse_facade_id` = ?, ',
                              '`spirit_wing_facade_id` = ?, ',
                              '`runes` = ?, ',
                              '`legendary_weapon_pieces` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `actor_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_noble_level,
                       @param_noble_used_item_count,
                       @param_noble_max_hp_add_value,
                       @param_noble_max_mp_add_value,
                       @param_noble_physics_attack_add_value,
                       @param_noble_physics_defence_add_value,
                       @param_noble_magic_attack_add_value,
                       @param_noble_magic_defence_add_value,
                       @param_noble_dodge_add_value,
                       @param_noble_crit_add_value,
                       @param_noble_ignore_defence_add_value,
                       @param_sword_avail_step,
                       @param_sword_current_step,
                       @param_sword_lucky_value,
                       @param_sword_gain_time,
                       @param_element_level,
                       @param_spirit_level,
                       @param_spirit_lucky_value,
                       @param_spirit_current_facade_type,
                       @param_spirit_horse_facade_id,
                       @param_spirit_wing_facade_id,
                       @param_runes,
                       @param_legendary_weapon_pieces,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_AUCTION_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_AUCTION_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_AUCTION_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('auction-actor-', @table_number);

  SET @field_list = CONCAT('`sell_items`, ',
                           '`buy_items`, ',
                           '`history`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_AUCTION_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_AUCTION_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_AUCTION_ACTOR_SAVE`(IN `id` bigint,
    IN `sell_items` varchar(2048),
    IN `buy_items` varchar(8192),
    IN `history` varchar(4096))
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('auction-actor-', @table_number);

  SET @param_sell_items = sell_items;
  SET @param_buy_items = buy_items;
  SET @param_history = history;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor_id`, ',
                             '`sell_items`, ',
                             '`buy_items`, ',
                             '`history`');
    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list, ') values(?, ?, ?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_sell_items,
                       @param_buy_items,
                       @param_history;
  ELSE
    SET @update_list = CONCAT('`sell_items` = ?, ',
                              '`buy_items` = ?, ',
                              '`history` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `actor_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_sell_items,
                       @param_buy_items,
                       @param_history,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_AUCTION_TRANSACTION_DEL
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_AUCTION_TRANSACTION_DEL`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_AUCTION_TRANSACTION_DEL`(IN `id` varchar(32))
BEGIN
  DECLARE max_table_number int DEFAULT 1;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('auction-transaction-', @table_number);
  SET @ret_code = 0;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @ret_code = -1;
  ELSE
    SET @sql = CONCAT('delete from `', @table_name, '` where `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id;
    SET @ret_code = 0;
  END IF;

  COMMIT;

  SELECT @ret_code;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_AUCTION_TRANSACTION_LOAD
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_AUCTION_TRANSACTION_LOAD`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_AUCTION_TRANSACTION_LOAD`()
BEGIN
  DECLARE max_table_number int DEFAULT 1;
  SET @idx = 0;
  SET @sql = '';

  WHILE @idx < max_table_number DO
    SET @table_name = CONCAT('auction-transaction-', @idx);
    SET @field_list = CONCAT('`id`, ',
                             '`item_template_id`, ',
                             '`item_number`, ',
                             '`item_extra_info`, ',
                             '`auction_time_id`, ',
                             '`start_bid_time`, ',
                             '`end_bid_time`, ',
                             '`owner`, ',
                             '`buyer`, ',
                             '`start_bid_price`, ',
                             '`last_bid_price`, ',
                             '`buyout_price`, ',
                             '`finished`, ',
                             '`money_withdrawn`, ',
                             '`item_withdrawn`');
    SET @sql = CONCAT(@sql, 'select ', @field_list, ' from `', @table_name, '` ');
    IF @idx <> max_table_number - 1 THEN
      SET @sql = CONCAT(@sql, 'union ');
    END IF;
    SET @idx = @idx + 1;
  END WHILE;

  PREPARE stmt FROM @sql;
  EXECUTE stmt;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_AUCTION_TRANSACTION_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_AUCTION_TRANSACTION_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_AUCTION_TRANSACTION_SAVE`(IN `id` varchar(32),
    IN `item_template_id` int,
    IN `item_number` int,
    IN `item_extra_info` varchar(512),
    IN `auction_time_id` int,
    IN `start_bid_time` bigint,
    IN `end_bid_time` bigint,
    IN `owner` bigint,
    IN `buyer` bigint,
    IN `start_bid_price` int,
    IN `last_bid_price` int,
    IN `buyout_price` int,
    IN `finished` tinyint,
    IN `money_withdrawn` tinyint,
    IN `item_withdrawn` tinyint)
BEGIN
  DECLARE max_table_number int DEFAULT 1;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('auction-transaction-', @table_number);

  SET @param_item_template_id = item_template_id;
  SET @param_item_number = item_number;
  SET @param_item_extra_info = item_extra_info;
  SET @param_auction_time_id = auction_time_id;
  SET @param_start_bid_time = start_bid_time;
  SET @param_end_bid_time = end_bid_time;
  SET @param_owner = owner;
  SET @param_buyer = buyer;
  SET @param_start_bid_price = start_bid_price;
  SET @param_last_bid_price = last_bid_price;
  SET @param_buyout_price = buyout_price;
  SET @param_finished = finished; 
  SET @param_money_withdrawn = money_withdrawn;
  SET @param_item_withdrawn = item_withdrawn;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`id`, ',
                             '`item_template_id`, ',
                             '`item_number`, ',
                             '`item_extra_info`, ',
                             '`auction_time_id`, ',
                             '`start_bid_time`, ',
                             '`end_bid_time`, ',
                             '`owner`, ',
                             '`buyer`, ',
                             '`start_bid_price`, ',
                             '`last_bid_price`, ',
                             '`buyout_price`, ',
                             '`finished`, ',
                             '`money_withdrawn`, ',
                             '`item_withdrawn`');
    SET @sql = CONCAT('insert into `', @table_name,
        '` (', @field_list, ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                                     '?, ?, ?, ?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_item_template_id,
                       @param_item_number,
                       @param_item_extra_info,
                       @param_auction_time_id,
                       @param_start_bid_time,
                       @param_end_bid_time,
                       @param_owner,
                       @param_buyer,
                       @param_start_bid_price,
                       @param_last_bid_price,
                       @param_buyout_price,
                       @param_finished,
                       @param_money_withdrawn,
                       @param_item_withdrawn;
  ELSE
    SET @update_list = CONCAT('`item_template_id`= ?, ',
                              '`item_number` = ?, ',
                              '`item_extra_info` = ?, ',
                              '`auction_time_id` = ?, ',
                              '`start_bid_time` = ?, ',
                              '`end_bid_time` = ?, ',
                              '`owner` = ?, ',
                              '`buyer` = ?, ',
                              '`start_bid_price` = ?, ',
                              '`last_bid_price` = ?, ',
                              '`buyout_price` = ?, ',
                              '`finished` = ?, ',
                              '`money_withdrawn` = ?, ',
                              '`item_withdrawn` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_item_template_id,
                       @param_item_number,
                       @param_item_extra_info,
                       @param_auction_time_id,
                       @param_start_bid_time,
                       @param_end_bid_time,
                       @param_owner,
                       @param_buyer,
                       @param_start_bid_price,
                       @param_last_bid_price,
                       @param_buyout_price,
                       @param_finished,
                       @param_money_withdrawn,
                       @param_item_withdrawn,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_BRIEF_ACTOR_LOAD
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_BRIEF_ACTOR_LOAD`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_BRIEF_ACTOR_LOAD`()
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @idx = 0;
  SET @sql = '';

  WHILE @idx < max_table_number DO
    SET @table_name = CONCAT('actor-', @idx);
    SET @sql = CONCAT(@sql,
      'select `id`, ',
      '`name`, ',
      '`level`, ',
      '`vocation`, ',
      '`gender`, ',
      '`map`, ',
      '`functionality_state`, ',
      '`axe_fight_score`, ',
      '`shoot_fight_score`, ',
      '`magic_fight_score`, ',
      '`last_logout_time`, ',
      '`vip_type`, ',
      '`vip_remainder_time` from `', @table_name, '` ');
    IF @idx <> max_table_number - 1 THEN
      SET @sql = CONCAT(@sql, 'union ');
    END IF;
    SET @idx = @idx + 1;
  END WHILE;

  PREPARE stmt FROM @sql;
  EXECUTE stmt;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_BUFF_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_BUFF_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_BUFF_LOGIN`(IN `id` bigint)
BEGIN
   DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('buff-', @table_number);
  SET @sql = CONCAT('select `buff_id`, `left_time`, `stack_count` from `', @table_name, '` where actor_id = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_BUFF_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_BUFF_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_BUFF_SAVE`(IN id BIGINT, IN buff_count INT, IN buffer VARCHAR(10240))
BEGIN

  DECLARE max_table_number INT DEFAULT 2;
  SET @actor_id = id;
  SET @param_count = buff_count;
  SET @table_number = @actor_id % max_table_number;
  SET @table_name = CONCAT('buff-', @table_number);
  SET @pair = '';
  SET @buff_id = 0;
  SET @left_time = 0;
  SET @buff_id_str = '';
  SET @buff_left_time_str = '';
  SET @idx = 1;

  START TRANSACTION;

  SET @SQL = CONCAT('delete from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @SQL;
  EXECUTE stmt USING @actor_id;

  SET @content = CONCAT('insert into `', @table_name, '`(`actor_id`,`buff_id`, `left_time`, `stack_count`) values');

  WHILE @idx <= @param_count DO
    SET @pair = SUBSTRING_INDEX(SUBSTRING_INDEX(buffer, ',', @idx), ',', -1);
    SET @buff_id_str = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@pair, ':', 1), ':', -1));
    SET @buff_left_time_str = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@pair, ':', 2), ':', -1));
    SET @buff_stack_cout_str = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@pair, ':', 3), ':', -1));
    SET @buff_id = CAST(@buff_id_str AS SIGNED);
    SET @left_time = CAST(@buff_left_time_str AS SIGNED);
    SET @stack_cout = CAST(@buff_stack_cout_str AS SIGNED);

    SET @content = concat(@content, '(', @actor_id, ',', @buff_id_str, ',', @buff_left_time_str, ',', @stack_cout,'),');

    SET @idx = @idx + 1;

  END WHILE;

  if @param_count > 0 then

    SET @content = left(@content, length(@content) - 1);
    PREPARE stmt FROM @content;
    execute stmt;

  end if;

  COMMIT;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_COOLING_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_COOLING_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_COOLING_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('cooling-', @table_number);
  SET @sql = CONCAT('select `cooling_id`, `cooling_time` from `', @table_name, '` where actor_id = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_COOLING_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_COOLING_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_COOLING_SAVE`(IN `id` bigint, IN `cooling_count` int, IN `buffer` varchar(10240))
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_count = cooling_count;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('cooling-', @table_number);
  SET @pair = '';
  SET @cooling_id = 0;
  SET @cooling_time = 0;
  SET @cooling_id_str = '';
  SET @cooling_time_str = '';
  SET @idx = 1;
  WHILE @idx <= @param_count DO
    SET @pair = SUBSTRING_INDEX(SUBSTRING_INDEX(buffer, ',', @idx), ',', -1);
    SET @cooling_id_str = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@pair, ':', 1), ':', -1));
    SET @cooling_time_str = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@pair, ':', 2), ':', -1));
    SET @cooling_id = CAST(@cooling_id_str as signed);
    SET @cooling_time = CAST(@cooling_time_str as signed);
    START TRANSACTION;
    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ? and `cooling_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @cooling_id;
    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`actor_id`, `cooling_id`, `cooling_time`) values (?, ?, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @cooling_id, @cooling_time;
    ELSE
      SET @sql = CONCAT('update `', @table_name, '` set `cooling_time` = ? where `actor_id` = ? and `cooling_id` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @cooling_time, @param_id, @cooling_id;
    END IF;
    COMMIT;
    SET @idx = @idx + 1;
  END WHILE;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_CREATE_ACTOR
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_CREATE_ACTOR`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_CREATE_ACTOR`(IN `id` bigint, IN `name` varchar(255), IN `hair` tinyint, IN `gender` tinyint, IN `vocation` int)
BEGIN

  DECLARE max_table_number int DEFAULT 2;

  SET @param_id = id;
  SET @param_name = name;
  SET @param_hair = hair;
  SET @param_gender = gender;
  SET @param_vocation = vocation;
  SET @param_now_time = UNIX_TIMESTAMP();

  SET @ret_code = 0;

  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('actor-', @table_number);

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @sql = CONCAT('insert into `', @table_name, '` (`id`, `name`, `vocation`, `gender`, `hair`, `level`, `current_hp`, `current_mp`, `map`, `x`, `y`, `create_time`) values(?, ?, ?, ?, ?, 1, 1000, 1000, 1001, 18, 71, ?)' );
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @param_name, @param_vocation, @param_gender, @param_hair, @param_now_time;
    SET @ret_code = 0;
  ELSE
    SET @ret_code = -1;
  END IF;

  COMMIT;

  SELECT @ret_code;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_GEM_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_GEM_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_GEM_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('gem-', @table_number);

  SET @field_list = CONCAT('`location_type`, ',
                           '`location_index`, ',
                           '`gem_id`, ',
                           '`gem_level`, ',
                           '`gem_exp`, ',
                           '`locked`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '` where `actor_id` = ? and exist = 1');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_GEM_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_GEM_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_GEM_SAVE`(IN `id` bigint, IN `gem_count` int, IN `buffer` varchar(10240))
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_count = gem_count;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('gem-', @table_number);

  SET @gem_info = '';
  SET @location_type = 0;
  SET @location_index = 0;
  SET @gem_id = 0;
  SET @gem_level = 0;
  SET @gem_exp = 0;
  SET @locked = 0;

  SET @idx = 1;

  START TRANSACTION;

  SET @sql = CONCAT('update `', @table_name, '` set `exist` = 0 where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  WHILE @idx <= @param_count DO
    SET @gem_info = SUBSTRING_INDEX(SUBSTRING_INDEX(buffer, ',', @idx), ',', -1);
    SET @location_type = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@gem_info, ':', 1), ':', -1));
    SET @location_index = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@gem_info, ':', 2), ':', -1));
    SET @gem_id = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@gem_info, ':', 3), ':', -1));
    SET @gem_level = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@gem_info, ':', 4), ':', -1));
    SET @gem_exp = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@gem_info, ':', 5), ':', -1));
    SET @locked = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@gem_info, ':', 6), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name,
                      '` where `actor_id` = ? and `location_type` = ? and `location_index` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @location_type, @location_index;

    IF @count = 0 THEN
      SET @field_list = CONCAT('`actor_id`, ',
                               '`location_type`, ',
                               '`location_index`, ',
                               '`gem_id`, ',
                               '`gem_level`, ',
                               '`gem_exp`, ',
                               '`locked`, ',
                               '`exist`');
      SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list, ') values(?, ?, ?, ?, ?, ?, ?, 1);');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id,
                         @location_type,
                         @location_index,
                         @gem_id,
                         @gem_level,
                         @gem_exp,
                         @locked;
    ELSE
      SET @update_list = CONCAT('`gem_id` = ?, ',
                                '`gem_level` = ?, ',
                                '`gem_exp` = ?, ',
                                '`locked` = ?, ',
                                '`exist` = 1');
      SET @sql = CONCAT('update `', @table_name, '` set ', @update_list,
                        ' where `actor_id` = ? and `location_type` = ? and `location_index` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @gem_id,
                         @gem_level,
                         @gem_exp,
                         @locked,
                         @param_id,
                         @location_type,
                         @location_index;
    END IF;

    SET @idx = @idx + 1;
  END WHILE;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_GUILD_DEL
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_GUILD_DEL`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_GUILD_DEL`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 1;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('guild-', @table_number);
  SET @ret_code = 0;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @ret_code = -1;
  ELSE
    SET @sql = CONCAT('delete from `', @table_name, '` where `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id;
    SET @ret_code = 0;
  END IF;

  COMMIT;

  SELECT @ret_code;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_GUILD_LOAD
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_GUILD_LOAD`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_GUILD_LOAD`()
BEGIN
  DECLARE max_table_number int DEFAULT 1;
  SET @idx = 0;
  SET @sql = '';

  WHILE @idx < max_table_number DO
    SET @table_name = CONCAT('guild-', @idx);
    SET @field_list = CONCAT('`id`, ',
                             '`name`, ',
                             '`logo`, ',
                             '`level`, ',
                             '`suppress_join`, ',
                             '`pending_members`, ',
                             '`members`, ',
                             '`announcement`, ',
                             '`events`, ',
                             '`gold`, ',
                             '`light_crystal`, ',
                             '`dark_crystal`, ',
                             '`holy_crystal`, ',
                             '`light_attribute`, ',
                             '`dark_attribute`, ',
                             '`holy_attribute`, ',
                             '`shop_level`, ',
                             '`college_level`, ',
                             '`barrack_level`, ',
                             '`opened_playing_groups`, ',
                             '`finished_playings`, ',
                             '`last_save_time`');
    SET @sql = CONCAT(@sql, 'select ', @field_list, ' from `', @table_name, '` ');
    IF @idx <> max_table_number - 1 THEN
      SET @sql = CONCAT(@sql, 'union ');
    END IF;
    SET @idx = @idx + 1;
  END WHILE;

  PREPARE stmt FROM @sql;
  EXECUTE stmt;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_GUILD_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_GUILD_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_GUILD_SAVE`(IN `id` bigint,
    IN `name` varchar(255),
    IN `logo` int,
    IN `level` int,
    IN `suppress_join` int,
    IN `pending_members` varchar(4096),
    IN `members` varchar(1024),
    IN `announcement` varchar(1024),
    IN `events` varchar(10240),
    IN `gold` int,
    IN `light_crystal` int,
    IN `dark_crystal` int,
    IN `holy_crystal` int,
    IN `light_attribute` int,
    IN `dark_attribute` int,
    IN `holy_attribute` int,
    IN `shop_level` int,
    IN `college_level` int,
    IN `barrack_level` int,
    IN `opened_playing_groups` varchar(128),
    IN `finished_playings` varchar(256),
    IN `last_save_time` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 1;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('guild-', @table_number);

  SET @param_name = name;
  SET @param_logo = logo;
  SET @param_level = level;
  SET @param_suppress_join = suppress_join;
  SET @param_pending_members = pending_members;
  SET @param_members = members;
  SET @param_announcement = announcement;
  SET @param_events = events;
  SET @param_gold = gold;
  SET @param_light_crystal = light_crystal;
  SET @param_dark_crystal = dark_crystal;
  SET @param_holy_crystal = holy_crystal;
  SET @param_light_attribute = light_attribute;
  SET @param_dark_attribute = dark_attribute;
  SET @param_holy_attribute = holy_attribute;
  SET @param_shop_level = shop_level;
  SET @param_college_level = college_level;
  SET @param_barrack_level = barrack_level;
  SET @param_opened_playing_groups = opened_playing_groups;
  SET @param_finished_playings = finished_playings;
  SET @param_last_save_time = last_save_time;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`id`, ',
                             '`name`, ',
                             '`logo`, ',
                             '`level`, ',
                             '`suppress_join`, ',
                             '`pending_members`, ',
                             '`members`, ',
                             '`announcement`, ',
                             '`events`, ',
                             '`gold`, ',
                             '`light_crystal`, ',
                             '`dark_crystal`, ',
                             '`holy_crystal`, ',
                             '`light_attribute`, ',
                             '`dark_attribute`, ',
                             '`holy_attribute`, ',
                             '`shop_level`, ',
                             '`college_level`, ',
                             '`barrack_level`, ',
                             '`opened_playing_groups`, ',
                             '`finished_playings`, ',
                             '`last_save_time`');
    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list,
                      ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                               '?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                               '?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_name,
                       @param_logo,
                       @param_level,
                       @param_suppress_join,
                       @param_pending_members,
                       @param_members,
                       @param_announcement,
                       @param_events,
                       @param_gold,
                       @param_light_crystal,
                       @param_dark_crystal,
                       @param_holy_crystal,
                       @param_light_attribute,
                       @param_dark_attribute,
                       @param_holy_attribute,
                       @param_shop_level,
                       @param_college_level,
                       @param_barrack_level,
                       @param_opened_playing_groups,
                       @param_finished_playings,
                       @param_last_save_time;
  ELSE
    SET @update_list = CONCAT('`level`= ?, ',
                              '`suppress_join` = ?, ',
                              '`pending_members` = ?, ',
                              '`members` = ?, ',
                              '`announcement` = ?, ',
                              '`events` = ?, ',
                              '`gold` = ?, ',
                              '`light_crystal` = ?, ',
                              '`dark_crystal` = ?, ',
                              '`holy_crystal` = ?, ',
                              '`light_attribute` = ?, ',
                              '`dark_attribute` = ?, ',
                              '`holy_attribute` = ?, ',
                              '`shop_level` = ?, ',
                              '`college_level` = ?, ',
                              '`barrack_level` = ?, ',
                              '`opened_playing_groups` = ?, ',
                              '`finished_playings` = ?, ',
                              '`last_save_time` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_level,
                       @param_suppress_join,
                       @param_pending_members,
                       @param_members,
                       @param_announcement,
                       @param_events,
                       @param_gold,
                       @param_light_crystal,
                       @param_dark_crystal,
                       @param_holy_crystal,
                       @param_light_attribute,
                       @param_dark_attribute,
                       @param_holy_attribute,
                       @param_shop_level,
                       @param_college_level,
                       @param_barrack_level,
                       @param_opened_playing_groups,
                       @param_finished_playings,
                       @param_last_save_time,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_GUILD_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_GUILD_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_GUILD_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('guild-actor-', @table_number);

  SET @field_list = CONCAT('`guild_id`, ',
                           '`guild_position`, ',
                           '`current_contribution_value`, ',
                           '`total_contribution_value`, ',
                           '`gold_contribution`, ',
                           '`freedom_dollars_contribution`, ',
                           '`light_crystal_contribution`, ',
                           '`dark_crystal_contribution`, ',
                           '`holy_crystal_contribution`, ',
                           '`guild_skills`, ',
                           '`guild_buffs`, ',
                           '`awarded_guild_playings`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_GUILD_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_GUILD_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_GUILD_ACTOR_SAVE`(IN `id` bigint,
    IN `guild_id` bigint,
    IN `guild_position` int,
    IN `current_contribution_value` int,
    IN `total_contribution_value` int,
    IN `gold_contribution` int,
    IN `freedom_dollars_contribution` int,
    IN `light_crystal_contribution` int,
    IN `dark_crystal_contribution` int,
    IN `holy_crystal_contribution` int,
    IN `guild_skills` varchar(1024),
    IN `guild_buffs` varchar(1024),
    IN `awarded_guild_playings` varchar(256))
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('guild-actor-', @table_number);

  SET @param_guild_id = guild_id;
  SET @param_guild_position = guild_position;
  SET @param_current_contribution_value = current_contribution_value;
  SET @param_total_contribution_value = total_contribution_value;
  SET @param_gold_contribution = gold_contribution;
  SET @param_freedom_dollars_contribution = freedom_dollars_contribution;
  SET @param_light_crystal_contribution = light_crystal_contribution;
  SET @param_dark_crystal_contribution = dark_crystal_contribution;
  SET @param_holy_crystal_contribution = holy_crystal_contribution;
  SET @param_guild_skills = guild_skills;
  SET @param_guild_buffs = guild_buffs;
  SET @param_awarded_guild_playings = awarded_guild_playings;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor_id`, ',
                             '`guild_id`, ',
                             '`guild_position`, ',
                             '`current_contribution_value`, ',
                             '`total_contribution_value`, ',
                             '`gold_contribution`, ',
                             '`freedom_dollars_contribution`, ',
                             '`light_crystal_contribution`, ',
                             '`dark_crystal_contribution`, ',
                             '`holy_crystal_contribution`, ',
                             '`guild_skills`, ',
                             '`guild_buffs`, ',
                             '`awarded_guild_playings`');

    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list,
                      ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                               '?, ?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_guild_id,
                       @param_guild_position,
                       @param_current_contribution_value,
                       @param_total_contribution_value,
                       @param_gold_contribution,
                       @param_freedom_dollars_contribution,
                       @param_light_crystal_contribution,
                       @param_dark_crystal_contribution,
                       @param_holy_crystal_contribution,
                       @param_guild_skills,
                       @param_guild_buffs,
                       @param_awarded_guild_playings;
  ELSE
    SET @update_list = CONCAT('`guild_id` = ?, ',
                              '`guild_position` = ?, ',
                              '`current_contribution_value` = ?, ',
                              '`total_contribution_value` = ?, ',
                              '`gold_contribution` = ?, ',
                              '`freedom_dollars_contribution` = ?, ',
                              '`light_crystal_contribution` = ?, ',
                              '`dark_crystal_contribution` = ?, ',
                              '`holy_crystal_contribution` = ?, ',
                              '`guild_skills` = ?, ',
                              '`guild_buffs` = ?, ',
                              '`awarded_guild_playings` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `actor_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_guild_id,
                       @param_guild_position,
                       @param_current_contribution_value,
                       @param_total_contribution_value,
                       @param_gold_contribution,
                       @param_freedom_dollars_contribution,
                       @param_light_crystal_contribution,
                       @param_dark_crystal_contribution,
                       @param_holy_crystal_contribution,
                       @param_guild_skills,
                       @param_guild_buffs,
                       @param_awarded_guild_playings,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ITEM_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ITEM_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ITEM_LOGIN`(IN `id` bigint, IN `container` tinyint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_contain_type = container;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = '';
  IF @param_contain_type = 0 THEN
    SET @table_name = CONCAT('packet-item-', @table_number);
  ELSEIF @param_contain_type = 1 THEN
    SET @table_name = CONCAT('equip-item-', @table_number);
  ELSEIF @param_contain_type = 4 THEN
    SET @table_name = CONCAT('treasure-item-', @table_number);
  ELSEIF @param_contain_type = 3 THEN
    SET @table_name = CONCAT('storage-item-', @table_number);
  END IF;

  SET @sql = CONCAT('select `location`, `vocation`, `template_id`, `number`, `bind`, `intensify_level`, `max_intensify_level`, `random_attr1`, `random_attr_value1`, `random_attr2`, `random_attr_value2`, `random_attr3`, `random_attr_value3`, `random_attr4`, `random_attr_value4`, `random_attr5`, `random_attr_value5`, `random_attr6`, `random_attr_value6`, `first_use_time`, `upgrade_lucky` from `', @table_name, '` where actor_id = ? and exist = 1');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ITEM_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ITEM_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ITEM_SAVE`(IN `id` bigint, IN `container` tinyint, IN `item_count` int, IN `buffer` varchar(10240))
BEGIN

  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_contain_type = container;
  SET @param_count = item_count;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = '';
  IF @param_contain_type = 0 THEN
    SET @table_name = CONCAT('packet-item-', @table_number);
  ELSEIF @param_contain_type = 1 THEN
    SET @table_name = CONCAT('equip-item-', @table_number);
  ELSEIF @param_contain_type = 4 THEN
    SET @table_name = CONCAT('treasure-item-', @table_number);
  ELSEIF @param_contain_type = 3 THEN
    SET @table_name = CONCAT('storage-item-', @table_number);
  END IF;

  SET @item_info = '';
  SET @item_template = 0;
  SET @item_location = 0;
  SET @item_vocation = 0;
  SET @item_number = 0;
  SET @item_bind = 0;
  SET @item_intensify_level = 0;
  SET @item_max_intensify_level = 0;
  SET @item_random_attr1 = 0;
  SET @item_random_attr_value1 = 0;
  SET @item_random_attr2 = 0;
  SET @item_random_attr_value2 = 0;
  SET @item_random_attr3 = 0;
  SET @item_random_attr_value3 = 0;
  SET @item_random_attr4 = 0;
  SET @item_random_attr_value4 = 0;
  SET @item_random_attr5 = 0;
  SET @item_random_attr_value5 = 0;
  SET @item_random_attr6 = 0;
  SET @item_random_attr_value6 = 0;
  SET @item_first_use_time = 0;
  SET @item_upgrade_lucky = 0;

  SET @idx = 1;

  START TRANSACTION;

  SET @sql = CONCAT('update `', @table_name, '` set `exist` = 0 where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  WHILE @idx <= @param_count DO
    SET @item_info = SUBSTRING_INDEX(SUBSTRING_INDEX(buffer, ',', @idx), ',', -1);
    SET @item_location = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 1), ':', -1));
    SET @item_vocation = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 2), ':', -1));
    SET @item_template = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 3), ':', -1));
    SET @item_number = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 4), ':', -1));
    SET @item_bind = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 5), ':', -1));
    SET @item_intensify_level = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 6), ':', -1));
    SET @item_max_intensify_level = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 7), ':', -1));
    SET @item_random_attr1 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 8), ':', -1));
    SET @item_random_attr_value1 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 9), ':', -1));
    SET @item_random_attr2 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 10), ':', -1));
    SET @item_random_attr_value2 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 11), ':', -1));
    SET @item_random_attr3 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 12), ':', -1));
    SET @item_random_attr_value3 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 13), ':', -1));
    SET @item_random_attr4 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 14), ':', -1));
    SET @item_random_attr_value4 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 15), ':', -1));
    SET @item_random_attr5 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 16), ':', -1));
    SET @item_random_attr_value5 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 17), ':', -1));
    SET @item_random_attr6 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 18), ':', -1));
    SET @item_random_attr_value6 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 19), ':', -1));
    SET @item_first_use_time = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 20), ':', -1));
    SET @item_upgrade_lucky = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@item_info, ':', 21), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ? and `location` = ? and `vocation` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @item_location, @item_vocation;

    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`actor_id`, `location`, `vocation`, `template_id`, `number`, `bind`, `intensify_level`, `max_intensify_level`, `random_attr1`, `random_attr_value1`, `random_attr2`, `random_attr_value2`, `random_attr3`, `random_attr_value3`, `random_attr4`, `random_attr_value4`, `random_attr5`, `random_attr_value5`, `random_attr6`, `random_attr_value6`, `exist`, `first_use_time`, `upgrade_lucky`) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 1, ?, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @item_location, @item_vocation, @item_template, @item_number, @item_bind, @item_intensify_level, @item_max_intensify_level, @item_random_attr1, @item_random_attr_value1, @item_random_attr2, @item_random_attr_value2, @item_random_attr3, @item_random_attr_value3, @item_random_attr4, @item_random_attr_value4, @item_random_attr5, @item_random_attr_value5, @item_random_attr6, @item_random_attr_value6, @item_first_use_time, @item_upgrade_lucky;
    ELSE
      SET @sql = CONCAT('update `', @table_name, '` set `template_id` = ?, `number` = ?, `bind` = ?, `intensify_level` = ?, `max_intensify_level` = ?, `random_attr1` = ?, `random_attr_value1` = ?, `random_attr2` = ?, `random_attr_value2` = ?, `random_attr3` = ?, `random_attr_value3` = ?, `random_attr4` = ?, `random_attr_value4` = ?, `random_attr5` = ?, `random_attr_value5` = ?, `random_attr6` = ?, `random_attr_value6` = ?, `exist` = 1, `first_use_time` = ?, `upgrade_lucky` = ? where `actor_id` = ? and `location` = ? and `vocation` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @item_template, @item_number, @item_bind, @item_intensify_level, @item_max_intensify_level, @item_random_attr1, @item_random_attr_value1, @item_random_attr2, @item_random_attr_value2, @item_random_attr3, @item_random_attr_value3, @item_random_attr4, @item_random_attr_value4, @item_random_attr5, @item_random_attr_value5, @item_random_attr6, @item_random_attr_value6, @item_first_use_time, @item_upgrade_lucky, @param_id, @item_location, @item_vocation;
    END IF;

    SET @idx = @idx + 1;
  END WHILE;

  COMMIT;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_PLAYING_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_PLAYING_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_PLAYING_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('playing-actor-', @table_number);

  SET @field_list = CONCAT('`records`,',
                           '`results`,',
                           '`auto_playing`,',
                           '`auto_playing_group`,',
                           '`auto_finish_time`,',
                           '`maze_ap`,',
                           '`maze_last_ap_recover_time`,',
                           '`maze_floor`,',
                           '`maze_explored_area`,',
                           '`maze_recent_position`,',
                           '`maze_gift_number`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;


-- ----------------------------
-- Procedure structure for GAME_PLAYING_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_PLAYING_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_PLAYING_ACTOR_SAVE`(IN `actor_id` bigint,
    IN `records` varchar(4096),
    IN `results` varchar(4096),
    IN `auto_playing` int,
    IN `auto_playing_group` int,
    IN `auto_finish_time` bigint,
    IN `maze_ap` int,
    IN `maze_last_ap_recover_time` bigint,
    IN `maze_floor` int,
    IN `maze_explored_area` varchar(128),
    IN `maze_recent_position` varchar(128),
    IN `maze_gift_number` int)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('playing-actor-', @table_number);

  SET @param_actor_id = actor_id;
  SET @param_records = records;
  SET @param_results = results;
  SET @param_auto_playing = auto_playing;
  SET @param_auto_playing_group = auto_playing_group;
  SET @param_auto_finish_time = auto_finish_time;
  SET @param_maze_ap = maze_ap;
  SET @param_maze_last_ap_recover_time = maze_last_ap_recover_time;
  SET @param_maze_floor = maze_floor;
  SET @param_maze_explored_area = maze_explored_area;
  SET @param_maze_recent_position = maze_recent_position;
  SET @param_maze_gift_number = maze_gift_number;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor_id`, ',
                             '`records`, ',
                             '`results`, ',
                             '`auto_playing`, ',
                             '`auto_playing_group`, ',
                             '`auto_finish_time`, ',
                             '`maze_ap`, ',
                             '`maze_last_ap_recover_time`, ',
                             '`maze_floor`, ',
                             '`maze_explored_area`, ',
                             '`maze_recent_position`, ',
                             '`maze_gift_number`');

    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list,
                      ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                               '?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_actor_id,
                       @param_records,
                       @param_results,
                       @param_auto_playing,
                       @param_auto_playing_group,
                       @param_auto_finish_time,
                       @param_maze_ap,
                       @param_maze_last_ap_recover_time,
                       @param_maze_floor,
                       @param_maze_explored_area,
                       @param_maze_recent_position,
                       @param_maze_gift_number;
  ELSE
    SET @update_list = CONCAT('`records` = ?, ',
                              '`results` = ?, ',
                              '`auto_playing` = ?, ',
                              '`auto_playing_group` = ?, ',
                              '`auto_finish_time` = ?, ',
                              '`maze_ap` = ?, ',
                              '`maze_last_ap_recover_time` = ?, ',
                              '`maze_floor` = ?, ',
                              '`maze_explored_area` = ?, ',
                              '`maze_recent_position` = ?, ',
                              '`maze_gift_number` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `actor_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_records,
                       @param_results,
                       @param_auto_playing,
                       @param_auto_playing_group,
                       @param_auto_finish_time,
                       @param_maze_ap,
                       @param_maze_last_ap_recover_time,
                       @param_maze_floor,
                       @param_maze_explored_area,
                       @param_maze_recent_position,
                       @param_maze_gift_number,
                       @param_actor_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;


-- ----------------------------
-- Procedure structure for GAME_MAIL_ADD
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_MAIL_ADD`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_MAIL_ADD`(IN `addressee_id` bigint(20), IN `addresser_id` bigint(20), IN `addresser_name` varchar(255),
    IN `title` varchar(255), IN `mail_type` tinyint(4), IN `affix_state` tinyint(4),
    IN `context` text, IN `item1` int(11), IN `item1_num` int(11), IN `item2` int(11), IN `item2_num` int(11), IN `item3` int(11),
    IN `item3_num` int(11), IN `item4` int(11), IN `item4_num` int(11), IN `item5` int(11), IN `item5_num` int(11),
    IN `gold` int(11), IN `freedom_dollars` int(11))
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  DECLARE max_mail_number int DEFAULT 50;
  SET @table_id = 0;
  SET @actor_id = addressee_id;
  SET @user_sys_type = 3;
  
  SET @param_addresser_id = addresser_id;
  SET @param_addresser_name = addresser_name;
  SET @param_title = title;
  SET @param_mail_type = mail_type;
  SET @param_affix_state = affix_state;
  SET @param_create_time = unix_timestamp(now());
  SET @param_context = context;
  SET @param_item1 = item1;
  SET @param_item1_num = item1_num;
  SET @param_item2 = item2;
  SET @param_item2_num = item2_num;
  SET @param_item3 = item3;
  SET @param_item3_num = item3_num;
  SET @param_item4 = item4;
  SET @param_item4_num = item4_num;
  SET @param_item5 = item5;
  SET @param_item5_num = item5_num;
  SET @param_gold = gold;
  SET @param_freedom_dollars = freedom_dollars;
  SET @mail_max_num = 50;
  SET @ret_mail_id = 0;
  IF @param_mail_type = @user_sys_type THEN
     SET @mail_max_num = 20;
  ELSE
     SET @mail_max_num = 30;
  END IF;
       
  START TRANSACTION;
  
  #IF @actor_id = 0 THEN
   #WHILE @table_id < max_table_number AND @actor_id = 0 DO
    #SET @table_name = CONCAT('actor-', @table_id);
    #SET @sql = CONCAT('select `id` into @actor_id from `', @table_name,
    #  '` where `name` = ? LIMIT 1');
    #PREPARE stmt FROM @sql;
    #EXECUTE stmt USING @param_name;
    #SET @table_id = @table_id + 1;
   #END WHILE;
  #END IF;
  
  SET @type_mail_num = 0;
  SET @mail_table_name = '';
  IF @actor_id != 0 THEN
    SET @table_mail = @actor_id % max_table_number;
    SET @mail_table_name = CONCAT('mail-', @table_mail);
  SET @sql_count = '';
  IF @param_mail_type = @user_sys_type THEN
        SET @sql_count = CONCAT('SELECT count(*) into @type_mail_num from `', @mail_table_name,
        '` where `actor` = ? AND `type`=3');
  ELSE
      SET @sql_count = CONCAT('SELECT count(*) into @type_mail_num from `', @mail_table_name,
        '` where `actor` = ? AND `type`=1 OR `type`=2');
  END IF;
    PREPARE stmt FROM @sql_count;
    EXECUTE stmt USING @actor_id;

    SET @update_mail_id = 0;
  IF @mail_max_num > @type_mail_num THEN
         SET @sql_invalied_mail = CONCAT('SELECT `mail_id` into @update_mail_id from `', @mail_table_name,'` where `actor`=? AND `type` = 0 LIMIT 1');
           PREPARE stmt FROM @sql_invalied_mail;
           EXECUTE stmt USING @actor_id;
  END IF;
    SET @is_insert = 0;
  IF @update_mail_id = 0 THEN   #### 没有找到已经删除的邮件，按照规则
      ### 统计邮件数目
    SET @total_mail_num = 0;
    SET @sql_count = CONCAT('SELECT count(*) into @total_mail_num from `', @mail_table_name,
        '` where `actor` = ?');
        PREPARE stmt FROM @sql_count;
        EXECUTE stmt USING @actor_id;
    IF @type_mail_num < @mail_max_num THEN
        SET @update_mail_id = @total_mail_num + 1;
            SET @is_insert = 1;
    ELSE ###只能顶邮件
      SET @temp_time = 0;
      SET @sql_invalied_mail='';
        IF @param_mail_type = @user_sys_type THEN
                SET @sql_invalied_mail = CONCAT('SELECT `mail_id` into @update_mail_id from `', @mail_table_name,'` where `actor`=? AND `type`=3 order by create_time asc limit 1');
      ELSE
                SET @sql_invalied_mail = CONCAT('SELECT `mail_id` into @update_mail_id from `', @mail_table_name,'` where `actor`=? AND `type`=2 OR `type`=1 order by create_time asc limit 1');
      END IF;
      PREPARE stmt FROM @sql_invalied_mail;
            EXECUTE stmt USING @actor_id;
    END IF;
    END IF;
  SET @param_read_state = 0;
  IF @is_insert = 1 THEN
      ### 插入
      SET @sql_insert = CONCAT('INSERT INTO `',@mail_table_name,'` (actor, mail_id, type, affix_state, read_state, title, create_time, context,
         addresser, item1, item1_num, item2, item2_num, item3, item3_num, item4, item4_num, item5, item5_num, gold, freedom_dollars) VALUES
         (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)');
        PREPARE stmt FROM @sql_insert;
        EXECUTE stmt USING @actor_id,
                            @update_mail_id,
                            @param_mail_type,
                            @param_affix_state,
                            @param_read_state,
                            @param_title,
                            @param_create_time,
                            @param_context,
                            @param_addresser_name,
                            @param_item1,
              @param_item1_num,
                            @param_item2,
                            @param_item2_num,
                            @param_item3,
                            @param_item3_num,
                            @param_item4,
                            @param_item4_num,
                            @param_item5,
                            @param_item5_num,
                            @param_gold,
                            @param_freedom_dollars;
      
  ELSE
      ### 更新
      SET @sql_update = CONCAT('UPDATE `',@mail_table_name,'` SET type=?, affix_state=?, read_state=?, title=?, create_time=?,
      context=?, addresser=?, item1=?, item1_num=?, item2=?, item2_num=?, item3=?, item3_num=?, item4=?, item4_num=?, item5=?, item5_num=?,
      gold=?, freedom_dollars=? where `actor`=? AND `mail_id`=?');
        PREPARE stmt FROM @sql_update;
        EXECUTE stmt USING @param_mail_type,
                           @param_affix_state,
                           @param_read_state,
                           @param_title,
                           @param_create_time,
                           @param_context,
                           @param_addresser_name,
                           @param_item1,
                           @param_item1_num,
                           @param_item2,
                           @param_item2_num,
                           @param_item3,
                           @param_item3_num,
                           @param_item4,
                           @param_item4_num,
                           @param_item5,
                           @param_item5_num,
                           @param_gold,
                           @param_freedom_dollars,
                           @actor_id,
                           @update_mail_id;
  END IF;
  SET @ret_mail_id = @update_mail_id;
    
  END IF;
  
  COMMIT;
  
  SELECT @ret_mail_id,@actor_id,@param_create_time;
END;;
DELIMITER ;


-- ----------------------------
-- Procedure structure for GAME_MAIL_AFFIX
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_MAIL_AFFIX`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_MAIL_AFFIX`( IN `affix_state` tinyint(4), IN `mail_id` int , IN `id` bigint(20))
BEGIN
   DECLARE max_table_number int DEFAULT 2;
   SET @param_id = id;
   SET @table_mail = @param_id % max_table_number;
   SET @mail_table_name = CONCAT('mail-', @table_mail);

   SET @param_mail_id = mail_id;
   SET @param_affix_state = affix_state;
   
   SET @sql = CONCAT('SELECT item1, item1_num, item2, item2_num, item3, item3_num, item4, item4_num, item5, item5_num,
      gold, freedom_dollars from `', @mail_table_name,'` where `actor` = ? AND `mail_id`=?');
  
   PREPARE stmt FROM @sql;
   EXECUTE stmt USING @param_id,
                      @param_mail_id;
END;;
DELIMITER ;


-- ----------------------------
-- Procedure structure for GAME_MAIL_AFFIX_STATE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_MAIL_AFFIX_STATE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_MAIL_AFFIX_STATE`( IN `affix_state` tinyint(4), IN `mail_id` int , IN `id` bigint(20))
BEGIN
   DECLARE max_table_number int DEFAULT 2;
   SET @param_id = id;
   SET @table_mail = @param_id % max_table_number;
   SET @mail_table_name = CONCAT('mail-', @table_mail);

   SET @param_mail_id = mail_id;
   SET @param_affix_state = affix_state;

   SET @sql = CONCAT('UPDATE  `', @mail_table_name,'` SET affix_state=? where `actor` = ? AND `mail_id`=?');
      
   PREPARE stmt FROM @sql;
   EXECUTE stmt USING @param_affix_state,
                      @param_id,
                      @param_mail_id;
            
   PREPARE stmt FROM @sql;
   EXECUTE stmt USING @param_id,
                      @param_mail_id;
END;;
DELIMITER ;


-- ----------------------------
-- Procedure structure for GAME_MAIL_DEL
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_MAIL_DEL`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_MAIL_DEL`( IN `del_count` int , IN `buffer` varchar(1024), IN `id` bigint(20))
BEGIN
   DECLARE max_table_number int DEFAULT 2;
   SET @param_id = id;
   SET @table_mail = @param_id % max_table_number;
   SET @mail_table_name = CONCAT('mail-', @table_mail);
   
   SET @temp_loop = 0;
   SET @param_del_count = del_count;
   
   START TRANSACTION;
   
   SET @state = 0;
   WHILE @temp_loop <= @param_del_count DO
      SET @mail_id = SUBSTRING_INDEX(SUBSTRING_INDEX(buffer, ',', @temp_loop), ',', -1);
      SET @sql = CONCAT('UPDATE `', @mail_table_name,'` SET type=?  where `actor` = ? AND `mail_id`=?');
      
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @state,
                         @param_id,
                         @mail_id;
      SET @temp_loop = @temp_loop + 1;
   END WHILE;
   
   COMMIT;
   
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_MAIL_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_MAIL_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_MAIL_LOGIN`(IN `addressee_id` bigint(20))
BEGIN
   DECLARE max_table_number int DEFAULT 2;
   SET @param_addressee_id = addressee_id;
   SET @table_mail = @param_addressee_id % max_table_number;
   SET @mail_table_name = CONCAT('mail-', @table_mail);
   SET @sql_login = CONCAT('SELECT mail_id, type, affix_state, read_state, title, create_time,',
         'addresser from `', @mail_table_name,'` where `actor` = ? AND `type` != 0');
   PREPARE stmt FROM @sql_login;
   EXECUTE stmt USING @param_addressee_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_MAIL_READ
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_MAIL_READ`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_MAIL_READ`( IN `mail_read_state` int , IN `mail_id` int , IN `id` bigint(20))
BEGIN
   DECLARE max_table_number int DEFAULT 2;
   SET @param_id = id;
   SET @table_mail = @param_id % max_table_number;
   SET @mail_table_name = CONCAT('mail-', @table_mail);

   SET @param_mail_id = mail_id;
   SET @param_mail_read_state = mail_read_state;

   SET @sql = CONCAT('UPDATE  `', @mail_table_name,'` SET read_state=? where `actor` = ? AND `mail_id`=?');
      
   PREPARE stmt FROM @sql;
   EXECUTE stmt USING @param_mail_read_state,
                      @param_id,
                      @param_mail_id;
                         

   SET @sql = CONCAT('SELECT context, item1, item1_num, item2, item2_num, item3, item3_num, item4, item4_num, item5, item5_num,
      gold, freedom_dollars from `', @mail_table_name,'` where `actor` = ? AND `mail_id`=?');
  
   PREPARE stmt FROM @sql;
   EXECUTE stmt USING @param_id,
                      @param_mail_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ITEM_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ITEM_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ITEM_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('item-actor-', @table_number);

  SET @field_list = CONCAT('`gem_gathering_point`,',
                           '`shortcut1`,',
                           '`shortcut2`,',
                           '`shortcut3`, ',
                           '`shortcut4`, ',
                           '`gifts_pick_num`, ',
                           '`item_buy_num`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ITEM_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ITEM_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ITEM_ACTOR_SAVE`(IN `id` bigint,
    IN `gem_gathering_point` int,
    IN `shortcut1` int,
    IN `shortcut2` int,
    IN `shortcut3` int,
    IN `shortcut4` int,
    IN `gifts_pick_num` int,
    IN `item_buy_num` text)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('item-actor-', @table_number);

  SET @param_gem_gathering_point = gem_gathering_point;
  SET @param_shortcut1 = shortcut1;
  SET @param_shortcut2 = shortcut2;
  SET @param_shortcut3 = shortcut3;
  SET @param_shortcut4 = shortcut4;
  SET @param_gifts_pick_num = gifts_pick_num;
  SET @param_item_buy_num = item_buy_num;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor_id`, ',
                             '`gem_gathering_point`, ',
                             '`shortcut1`, ',
                             '`shortcut2`, ',
                             '`shortcut3`, ',
                             '`shortcut4`, ',
                             '`gifts_pick_num`, ',
                             '`item_buy_num`');

    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list, ') values(?, ?, ?, ?, ?, ?, ?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_gem_gathering_point,
                       @param_shortcut1,
                       @param_shortcut2,
                       @param_shortcut3,
                       @param_shortcut4,
                       @param_gifts_pick_num,
                       @param_item_buy_num;
  ELSE
    SET @update_list = CONCAT('`gem_gathering_point` = ?, ',
                              '`shortcut1` = ?, ',
                              '`shortcut2` = ?, ',
                              '`shortcut3` = ?, ',
                              '`shortcut4` = ?, ',
                              '`gifts_pick_num` = ?, ',
                              '`item_buy_num` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `actor_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_gem_gathering_point,
                       @param_shortcut1,
                       @param_shortcut2,
                       @param_shortcut3,
                       @param_shortcut4,
                       @param_gifts_pick_num,
                       @param_item_buy_num,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SKILL_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SKILL_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SKILL_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('skill-', @table_number);
  
  SET @sql = CONCAT('select `skill_id`, `current_level` from `', @table_name, '` where actor_id = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
  
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SKILL_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SKILL_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SKILL_SAVE`(IN `id` bigint, IN `skill_id` int, in `operate_type` smallint, in `currentlevel` smallint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @actor_id = id;
  SET @table_number = @actor_id % max_table_number;
  SET @table_name = CONCAT('skill-', @table_number);
  
  SET @skill_id = skill_id;
  SET @operate_type = operate_type;
  SET @currentlevel = currentlevel;
  
  if @operate_type = 1 then 
    SET @SQL = CONCAT('insert into  `', @table_name, '`(`actor_id`, `skill_id`,`current_level`) values(?,?,?)');
    PREPARE stmt FROM @SQL;
    EXECUTE stmt USING @actor_id, @skill_id, @currentlevel;
  elseif @operate_type = 2 THEN
    SET @SQL = CONCAT('update `', @table_name, '` set `current_level`= ? where `actor_id` = ? and `skill_id` = ?');
    PREPARE stmt FROM @SQL;
    EXECUTE stmt USING @currentlevel, @actor_id, @skill_id;
  ELSEIF @operate_type = 3 THEN
    SET @SQL = CONCAT('delete from `', @table_name, '` where `actor_id` = ? and `skill_id` = ?');
    PREPARE stmt FROM @SQL;
    EXECUTE stmt USING @actor_id, @skill_id;
  end if;
  
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SOCIAL_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SOCIAL_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SOCIAL_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_social_actor_table_number int DEFAULT 2;
  DECLARE max_soul_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @social_actor_table_name = CONCAT('social-actor-', @param_id % max_social_actor_table_number);
  SET @soul_table_name = CONCAT('soul-', @param_id % max_soul_table_number);

  SET @sql = CONCAT('select group_concat(`id`, ":", `level`) into @soul_data from `', @soul_table_name,
                    '` where actor_id = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @soul_data IS NULL THEN
    SET @soul_data = '';
  END IF;

  SET @field_list = CONCAT('`signature`, ',
                           '`friends`, ',
                           '`blacklist`, '
                           '`enemies`, '
                           '`homeland_upgrade_start_time`, ',
                           '`homeland_upgrade_finish_time`, ',
                           '`homeland_temple_level`, ',
                           '`homeland_temple_harvest_times`, ',
                           '`homeland_temple_force_harvest_times`, ',
                           '`homeland_temple_next_harvest_time`, ',
                           '`homeland_goldmine_level`, ',
                           '`homeland_goldmine_robbed_times`, ',
                           '`homeland_goldmine_loss_rate`, ',
                           '`homeland_goldmine_next_harvest_time`, ',
                           '`homeland_goldmine_rob_times`, ',
                           '`homeland_goldmine_next_rob_time`, ',
                           '`homeland_pool_level`, ',
                           '`homeland_pool_last_harvest_time`, ',
                           '`homeland_church_free_pray_times`, ',
                           '`homeland_church_paid_pray_times`, ',
                           '`homeland_church_free_refresh_times`, ',
                           '`homeland_church_chest_type`, ',
                           '`homeland_tree_level`, ',
                           '`homeland_tree_harvest_times`, ',
                           '`homeland_tree_stage`, ',
                           '`homeland_tree_watered_times`, ',
                           '`homeland_tree_next_water_time`, ',
                           '`homeland_tree_growing_finish_time`, ',
                           '`homeland_tree_assist_water_times`, ',
                           '`homeland_tree_watered_actors`, ',
                           '`homeland_tree_stolen_actors`, ',
                           '`homeland_tower1_level`, ',
                           '`homeland_tower2_level`, ',
                           '`homeland_stronghold_level`, ',
                           '`homeland_stronghold_next_call_support_time`, ',
                           '`homeland_stronghold_supported_souls`, ',
                           '`homeland_stronghold_support_souls`, ',
                           '`homeland_stronghold_support_times`, ',
                           '`homeland_invaders`, ',
                           '`homeland_events`, ',
                           '`last_save_time`, ',
                           '"', @soul_data, '" as `soul_data`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @social_actor_table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SOCIAL_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SOCIAL_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SOCIAL_ACTOR_SAVE`(IN `id` bigint,
    IN `signature` varchar(256),
    IN `friends` varchar(2048),
    IN `blacklist` varchar(1024),
    IN `enemies` varchar(1024),
    IN `homeland_upgrade_start_time` bigint,
    IN `homeland_upgrade_finish_time` bigint,
    IN `homeland_temple_level` int,
    IN `homeland_temple_harvest_times` int,
    IN `homeland_temple_force_harvest_times` int,
    IN `homeland_temple_next_harvest_time` bigint,
    IN `homeland_goldmine_level` int,
    IN `homeland_goldmine_robbed_times` int,
    IN `homeland_goldmine_loss_rate` int,
    IN `homeland_goldmine_next_harvest_time` bigint,
    IN `homeland_goldmine_rob_times` int,
    IN `homeland_goldmine_next_rob_time` bigint,
    IN `homeland_pool_level` int,
    IN `homeland_pool_last_harvest_time` bigint,
    IN `homeland_church_free_pray_times` int,
    IN `homeland_church_paid_pray_times` int,
    IN `homeland_church_free_refresh_times` int,
    IN `homeland_church_chest_type` int,
    IN `homeland_tree_level` int,
    IN `homeland_tree_harvest_times` int,
    IN `homeland_tree_stage` int,
    IN `homeland_tree_watered_times` int,
    IN `homeland_tree_next_water_time` bigint,
    IN `homeland_tree_growing_finish_time` bigint,
    IN `homeland_tree_assist_water_times` int,
    IN `homeland_tree_watered_actors` varchar(2048),
    IN `homeland_tree_stolen_actors` varchar(256),
    IN `homeland_tower1_level` int,
    IN `homeland_tower2_level` int,
    IN `homeland_stronghold_level` int,
    IN `homeland_stronghold_next_call_support_time` bigint,
    IN `homeland_stronghold_supported_souls` varchar(1024),
    IN `homeland_stronghold_support_souls` varchar(1024),
    IN `homeland_stronghold_support_times` int,
    IN `homeland_invaders` varchar(2048),
    IN `homeland_events` varchar(4096),
    IN `last_save_time` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('social-actor-', @table_number);

  SET @param_signature = signature;
  SET @param_friends = friends;
  SET @param_blacklist = blacklist;
  SET @param_enemies = enemies;
  SET @param_homeland_upgrade_start_time = homeland_upgrade_start_time;
  SET @param_homeland_upgrade_finish_time = homeland_upgrade_finish_time;
  SET @param_homeland_temple_level = homeland_temple_level;
  SET @param_homeland_temple_harvest_times = homeland_temple_harvest_times;
  SET @param_homeland_temple_force_harvest_times = homeland_temple_force_harvest_times;
  SET @param_homeland_temple_next_harvest_time = homeland_temple_next_harvest_time;
  SET @param_homeland_goldmine_level = homeland_goldmine_level;
  SET @param_homeland_goldmine_robbed_times = homeland_goldmine_robbed_times;
  SET @param_homeland_goldmine_loss_rate = homeland_goldmine_loss_rate;
  SET @param_homeland_goldmine_next_harvest_time = homeland_goldmine_next_harvest_time;
  SET @param_homeland_goldmine_rob_times = homeland_goldmine_rob_times;
  SET @param_homeland_goldmine_next_rob_time = homeland_goldmine_next_rob_time;
  SET @param_homeland_pool_level = homeland_pool_level;
  SET @param_homeland_pool_last_harvest_time = homeland_pool_last_harvest_time;
  SET @param_homeland_church_free_pray_times = homeland_church_free_pray_times;
  SET @param_homeland_church_paid_pray_times = homeland_church_paid_pray_times;
  SET @param_homeland_church_free_refresh_times = homeland_church_free_refresh_times;
  SET @param_homeland_church_chest_type = homeland_church_chest_type;
  SET @param_homeland_tree_level = homeland_tree_level;
  SET @param_homeland_tree_harvest_times = homeland_tree_harvest_times;
  SET @param_homeland_tree_stage = homeland_tree_stage;
  SET @param_homeland_tree_watered_times = homeland_tree_watered_times;
  SET @param_homeland_tree_next_water_time = homeland_tree_next_water_time;
  SET @param_homeland_tree_growing_finish_time = homeland_tree_growing_finish_time;
  SET @param_homeland_tree_assist_water_times = homeland_tree_assist_water_times;
  SET @param_homeland_tree_watered_actors = homeland_tree_watered_actors;
  SET @param_homeland_tree_stolen_actors = homeland_tree_stolen_actors;
  SET @param_homeland_tower1_level = homeland_tower1_level;
  SET @param_homeland_tower2_level = homeland_tower2_level;
  SET @param_homeland_stronghold_level = homeland_stronghold_level;
  SET @param_homeland_stronghold_next_call_support_time = homeland_stronghold_next_call_support_time;
  SET @param_homeland_stronghold_supported_souls = homeland_stronghold_supported_souls;
  SET @param_homeland_stronghold_support_souls = homeland_stronghold_support_souls;
  SET @param_homeland_stronghold_support_times = homeland_stronghold_support_times;
  SET @param_homeland_invaders = homeland_invaders;
  SET @param_homeland_events = homeland_events;
  SET @param_last_save_time = last_save_time;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor_id`, ',
                             '`signature`, ',
                             '`friends`, ',
                             '`blacklist`, ',
                             '`enemies`, '
                             '`homeland_upgrade_start_time`, '
                             '`homeland_upgrade_finish_time`, ',
                             '`homeland_temple_level`, ',
                             '`homeland_temple_harvest_times`, ',
                             '`homeland_temple_force_harvest_times`, ',
                             '`homeland_temple_next_harvest_time`, ',
                             '`homeland_goldmine_level`, ',
                             '`homeland_goldmine_robbed_times`, ',
                             '`homeland_goldmine_loss_rate`, ',
                             '`homeland_goldmine_next_harvest_time`, ',
                             '`homeland_goldmine_rob_times`, ',
                             '`homeland_goldmine_next_rob_time`, ',
                             '`homeland_pool_level`, ',
                             '`homeland_pool_last_harvest_time`, ',
                             '`homeland_church_free_pray_times`, ',
                             '`homeland_church_paid_pray_times`, ',
                             '`homeland_church_free_refresh_times`, ',
                             '`homeland_church_chest_type`, ',
                             '`homeland_tree_level`, ',
                             '`homeland_tree_harvest_times`, ',
                             '`homeland_tree_stage`, ',
                             '`homeland_tree_watered_times`, ',
                             '`homeland_tree_next_water_time`, ',
                             '`homeland_tree_growing_finish_time`, ',
                             '`homeland_tree_assist_water_times`, ',
                             '`homeland_tree_watered_actors`, ',
                             '`homeland_tree_stolen_actors`, ',
                             '`homeland_tower1_level`, ',
                             '`homeland_tower2_level`, ',
                             '`homeland_stronghold_level`, ',
                             '`homeland_stronghold_next_call_support_time`, ',
                             '`homeland_stronghold_supported_souls`, ',
                             '`homeland_stronghold_support_souls`, ',
                             '`homeland_stronghold_support_times`, ',
                             '`homeland_invaders`, ',
                             '`homeland_events`, ',
                             '`last_save_time`');

    SET @sql = CONCAT('insert into `', @table_name,
        '` (', @field_list, ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                                     '?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                                     '?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                                     '?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ',
                                     '?, ?)');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_signature,
                       @param_friends,
                       @param_blacklist,
                       @param_enemies,
                       @param_homeland_upgrade_start_time,
                       @param_homeland_upgrade_finish_time,
                       @param_homeland_temple_level,
                       @param_homeland_temple_harvest_times,
                       @param_homeland_temple_force_harvest_times,
                       @param_homeland_temple_next_harvest_time,
                       @param_homeland_goldmine_level,
                       @param_homeland_goldmine_robbed_times,
                       @param_homeland_goldmine_loss_rate,
                       @param_homeland_goldmine_next_harvest_time,
                       @param_homeland_goldmine_rob_times,
                       @param_homeland_goldmine_next_rob_time,
                       @param_homeland_pool_level,
                       @param_homeland_pool_last_harvest_time,
                       @param_homeland_church_free_pray_times,
                       @param_homeland_church_paid_pray_times,
                       @param_homeland_church_free_refresh_times,
                       @param_homeland_church_chest_type,
                       @param_homeland_tree_level,
                       @param_homeland_tree_harvest_times,
                       @param_homeland_tree_stage,
                       @param_homeland_tree_watered_times,
                       @param_homeland_tree_next_water_time,
                       @param_homeland_tree_growing_finish_time,
                       @param_homeland_tree_assist_water_times,
                       @param_homeland_tree_watered_actors,
                       @param_homeland_tree_stolen_actors,
                       @param_homeland_tower1_level,
                       @param_homeland_tower2_level,
                       @param_homeland_stronghold_level,
                       @param_homeland_stronghold_next_call_support_time,
                       @param_homeland_stronghold_supported_souls,
                       @param_homeland_stronghold_support_souls,
                       @param_homeland_stronghold_support_times,
                       @param_homeland_invaders,
                       @param_homeland_events,
                       @param_last_save_time;
  ELSE
    SET @update_list = CONCAT('`signature` = ?, ',
                              '`friends`= ?, ',
                              '`blacklist` = ?, ',
                              '`enemies` = ?, ',
                              '`homeland_upgrade_start_time` = ?, ',
                              '`homeland_upgrade_finish_time` = ?, ',
                              '`homeland_temple_level` = ?, ',
                              '`homeland_temple_harvest_times` = ?, ',
                              '`homeland_temple_force_harvest_times` = ?, ',
                              '`homeland_temple_next_harvest_time` = ?, ',
                              '`homeland_goldmine_level` = ?, ',
                              '`homeland_goldmine_robbed_times` = ?, ',
                              '`homeland_goldmine_loss_rate` = ?, ',
                              '`homeland_goldmine_next_harvest_time` = ?, ',
                              '`homeland_goldmine_rob_times` = ?, ',
                              '`homeland_goldmine_next_rob_time` = ?, ',
                              '`homeland_pool_level` = ?, ',
                              '`homeland_pool_last_harvest_time` = ?, ',
                              '`homeland_church_free_pray_times` = ?, ',
                              '`homeland_church_paid_pray_times` = ?, ',
                              '`homeland_church_free_refresh_times` = ?, ',
                              '`homeland_church_chest_type` = ?, ',
                              '`homeland_tree_level` = ?, ',
                              '`homeland_tree_harvest_times` = ?, ',
                              '`homeland_tree_stage` = ?, ',
                              '`homeland_tree_watered_times` = ?, ',
                              '`homeland_tree_next_water_time` = ?, ',
                              '`homeland_tree_growing_finish_time` = ?, ',
                              '`homeland_tree_assist_water_times` = ?, ',
                              '`homeland_tree_watered_actors` = ?, ',
                              '`homeland_tree_stolen_actors` = ?, ',
                              '`homeland_tower1_level` = ?, ',
                              '`homeland_tower2_level` = ?, ',
                              '`homeland_stronghold_level` = ?, ',
                              '`homeland_stronghold_next_call_support_time` = ?, ',
                              '`homeland_stronghold_supported_souls` = ?, ',
                              '`homeland_stronghold_support_souls` = ?, ',
                              '`homeland_stronghold_support_times` = ?, ',
                              '`homeland_invaders` = ?, ',
                              '`homeland_events` = ?, ',
                              '`last_save_time` = ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `actor_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_signature,
                       @param_friends,
                       @param_blacklist,
                       @param_enemies,
                       @param_homeland_upgrade_start_time,
                       @param_homeland_upgrade_finish_time,
                       @param_homeland_temple_level,
                       @param_homeland_temple_harvest_times,
                       @param_homeland_temple_force_harvest_times,
                       @param_homeland_temple_next_harvest_time,
                       @param_homeland_goldmine_level,
                       @param_homeland_goldmine_robbed_times,
                       @param_homeland_goldmine_loss_rate,
                       @param_homeland_goldmine_next_harvest_time,
                       @param_homeland_goldmine_rob_times,
                       @param_homeland_goldmine_next_rob_time,
                       @param_homeland_pool_level,
                       @param_homeland_pool_last_harvest_time,
                       @param_homeland_church_free_pray_times,
                       @param_homeland_church_paid_pray_times,
                       @param_homeland_church_free_refresh_times,
                       @param_homeland_church_chest_type,
                       @param_homeland_tree_level,
                       @param_homeland_tree_harvest_times,
                       @param_homeland_tree_stage,
                       @param_homeland_tree_watered_times,
                       @param_homeland_tree_next_water_time,
                       @param_homeland_tree_growing_finish_time,
                       @param_homeland_tree_assist_water_times,
                       @param_homeland_tree_watered_actors,
                       @param_homeland_tree_stolen_actors,
                       @param_homeland_tower1_level,
                       @param_homeland_tower2_level,
                       @param_homeland_stronghold_level,
                       @param_homeland_stronghold_next_call_support_time,
                       @param_homeland_stronghold_supported_souls,
                       @param_homeland_stronghold_support_souls,
                       @param_homeland_stronghold_support_times,
                       @param_homeland_invaders,
                       @param_homeland_events,
                       @param_last_save_time,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SOUL_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SOUL_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SOUL_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('soul-', @table_number);

  SET @sql = CONCAT('select `id`, `level`, `step`, `common_skill_level`, `appear_skill_level`, `soul_skill_level` from `',
      @table_name, '` where actor_id = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SOUL_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SOUL_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SOUL_SAVE`(IN `id` bigint, IN `soul_count` int, IN `buffer` varchar(10240))
BEGIN

  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_count = soul_count;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('soul-', @table_number);

  SET @soul_info = '';
  SET @soul_id = 0;
  SET @soul_level = 0;
  SET @soul_step = 0;
  SET @soul_common_skill_level = 0;
  SET @soul_appear_skill_level = 0;
  SET @soul_soul_skill_level = 0;

  SET @idx = 1;

  WHILE @idx <= @param_count DO
    SET @soul_info = SUBSTRING_INDEX(SUBSTRING_INDEX(buffer, ',', @idx), ',', -1);
    SET @soul_id = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@soul_info, ':', 1), ':', -1));
    SET @soul_level = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@soul_info, ':', 2), ':', -1));
    SET @soul_step = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@soul_info, ':', 3), ':', -1));
    SET @soul_common_skill_level = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@soul_info, ':', 4), ':', -1));
    SET @soul_appear_skill_level = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@soul_info, ':', 5), ':', -1));
    SET @soul_soul_skill_level = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@soul_info, ':', 6), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ? and `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @soul_id;

    START TRANSACTION;

    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name,
          '` (`actor_id`, `id`, `level`, `step`, `common_skill_level`, `appear_skill_level`, `soul_skill_level`) values (?, ?, ?, ?, ?, ?, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @soul_id, @soul_level, @soul_step, @soul_common_skill_level, @soul_appear_skill_level, @soul_soul_skill_level;
    ELSE
      SET @sql = CONCAT('update `', @table_name,
          '` set `level` = ?, `step` = ?, `common_skill_level` = ?, `appear_skill_level` = ?, `soul_skill_level` = ? where `actor_id` = ? and `id` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @soul_level, @soul_step, @soul_common_skill_level, @soul_appear_skill_level, @soul_soul_skill_level, @param_id, @soul_id;
    END IF;

    COMMIT;

    SET @idx = @idx + 1;
  END WHILE;


END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SOUL_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SOUL_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SOUL_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('soul-actor-', @table_number);

  SET @sql = CONCAT('select ',
                    '`axe_setting`, ',
                    '`shoot_setting`, ',
                    '`magic_setting`, ',
                    '`settings`, ',
                    '`altar_summon_paid`, ',
                    '`altar_free_summon_times`, ',
                    '`altar_cards`, ',
                    '`altar_card_bind`, ',
                    '`current_energy` ',
                    'from `', @table_name, '` where `actor_id` = ?');

  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SOUL_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SOUL_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SOUL_ACTOR_SAVE`(IN `id` bigint,
    IN `axe_setting` int,
    IN `shoot_setting` int,
    IN `magic_setting` int,
    IN `settings` varchar(1024),
    IN `altar_summon_paid` tinyint,
    IN `altar_free_summon_times` int,
    IN `altar_cards` varchar(128),
    IN `altar_card_bind` tinyint,
    IN `current_energy` int)
BEGIN

  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_axe_setting = axe_setting;
  SET @param_shoot_setting = shoot_setting;
  SET @param_magic_setting = magic_setting;
  SET @param_settings = settings;
  SET @param_altar_summon_paid = altar_summon_paid;
  SET @param_altar_free_summon_times = altar_free_summon_times;
  SET @param_altar_cards = altar_cards;
  SET @param_altar_card_bind = altar_card_bind;
  SET @param_current_energy = current_energy;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('soul-actor-', @table_number);

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor_id`, ',
                             '`axe_setting`, ',
                             '`shoot_setting`, ',
                             '`magic_setting`, ',
                             '`settings`, ',
                             '`altar_summon_paid`, ',
                             '`altar_free_summon_times`, ',
                             '`altar_cards`, ',
                             '`altar_card_bind`,',
                             '`current_energy` ');

    SET @sql = CONCAT('insert into `', @table_name,
        '` (', @field_list, ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_axe_setting,
                       @param_shoot_setting,
                       @param_magic_setting,
                       @param_settings,
                       @param_altar_summon_paid,
                       @param_altar_free_summon_times,
                       @param_altar_cards,
                       @param_altar_card_bind,
                       @param_current_energy;
  ELSE
    SET @update_list = CONCAT('`axe_setting` = ?, ',
                              '`shoot_setting` = ?, ',
                              '`magic_setting` = ?, ',
                              '`settings`= ?, ',
                              '`altar_summon_paid`= ?, ',
                              '`altar_free_summon_times`= ?, ',
                              '`altar_cards`= ?, ',
                              '`altar_card_bind`= ?, ',
                              '`current_energy` = ? ');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `actor_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_axe_setting,
                       @param_shoot_setting,
                       @param_magic_setting,
                       @param_settings,
                       @param_altar_summon_paid,
                       @param_altar_free_summon_times,
                       @param_altar_cards,
                       @param_altar_card_bind,
                       @param_current_energy,
                       @param_id;
  END IF;

  COMMIT;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SPIRIT_FACADE_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SPIRIT_FACADE_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SPIRIT_FACADE_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('spirit-facade-', @table_number);

  SET @field_list = CONCAT('`spirit_facade_id`, ',
                           '`expire_time`');

  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '` where `actor_id` = ? and exist = 1');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_SPIRIT_FACADE_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_SPIRIT_FACADE_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_SPIRIT_FACADE_SAVE`(IN `id` bigint, IN `spirit_facade_count` int, IN `buffer` varchar(10240))
BEGIN

  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_count = spirit_facade_count;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('spirit-facade-', @table_number);

  SET @info = '';
  SET @spirit_facade_id = 0;
  SET @expire_time = 0;

  SET @idx = 1;

  START TRANSACTION;

  SET @sql = CONCAT('update `', @table_name, '` set `exist` = 0 where `actor_id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  WHILE @idx <= @param_count DO
    SET @info = SUBSTRING_INDEX(SUBSTRING_INDEX(buffer, ',', @idx), ',', -1);
    SET @spirit_facade_id = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@info, ':', 1), ':', -1));
    SET @expire_time = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@info, ':', 2), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name,
                      '` where `actor_id` = ? and `spirit_facade_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @spirit_facade_id;

    IF @count = 0 THEN
      SET @field_list = CONCAT('`actor_id`, ',
                               '`spirit_facade_id`, ',
                               '`expire_time`, ',
                               '`exist`');
      SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list, ') values(?, ?, ?, 1);');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id,
                         @spirit_facade_id,
                         @expire_time;
    ELSE
      SET @update_list = CONCAT('`expire_time` = ?, ',
                                '`exist` = 1');
      SET @sql = CONCAT('update `', @table_name, '` set ', @update_list,
                        ' where `actor_id` = ? and `spirit_facade_id` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @expire_time,
                         @param_id,
                         @spirit_facade_id;
    END IF;

    SET @idx = @idx + 1;
  END WHILE;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_TASK_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_TASK_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_TASK_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('task-', @table_number);

  SET @sql = CONCAT('select `task`, `flag`, `cond_type1`, `var1`, `value1`, `cond_type2`, `var2`, `value2`, `cond_type3`, `var3`, `value3`, `task_type` from `', @table_name, '` where `actor` = ? and `flag` in (1, 2)');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

END;;
DELIMITER ;


-- ----------------------------
-- Procedure structure for GAME_ACHIEVE_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ACHIEVE_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ACHIEVE_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('achieve-', @table_number);

  SET @sql = CONCAT('select `achieve_id`, `flag`, `condition1_param1`, `condition1_param2`, `condition1_value`, `condition2_param1`, `condition2_param2`, `condition2_value`, `finish_time` from `', @table_name, '` where `actor` = ? and `flag` in (1, 2)');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

END;;
DELIMITER ;


-- ----------------------------
-- Procedure structure for GAME_WELFARE_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_WELFARE_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_WELFARE_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('welfare-actor-', @table_number);
  
  SET @sql = CONCAT('select `title_id`, `daily_checkins`, `daily_award`, `general_checkins`, `vip_checkins`, `last_month_resource`, `current_month_resource`, `last_month_award_resource`, `daily_time`, `total_time`, `replenish_checkins_num`, `online_award_items`, `respenish_days`  from `', @table_name, '` where `actor` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_TASK_ACTOR_LOGIN
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_TASK_ACTOR_LOGIN`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_TASK_ACTOR_LOGIN`(IN `id` bigint)
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('task_actor-', @table_number);

  SET @sql = CONCAT('select `circle_flag`, `round_num`, `total_round`, `circle_num`, `lv_stage`, `condition_id`, `trip_num`, `exploit_award`, `daily_exploit`, `transport_finished_count`, `transport_free_refreshed_count`, `transport_robed_count`, `transport_current_transport`, `transport_status` from `', @table_name, '` where `actor` = ? ');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_TASK_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_TASK_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_TASK_ACTOR_SAVE`(IN `id` bigint, IN `circle_flag` tinyint(1), IN `circle_round_num` int,IN `circle_total_round_num` int,
		IN `circle_num` int,IN `circle_level_stage` int,IN `circle_condition` int,IN `trip_num` int, IN `exploit_award` int, IN `daily_exploit` int,
    IN `transport_finished_count` int, IN `transport_free_refreshed_count` int, IN `transport_robed_count` int, IN `transport_current_transport` int, IN `transport_status` int)
BEGIN

  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('task_actor-', @table_number);

  SET @param_circle_flag = circle_flag;
  SET @param_circle_round_num = circle_round_num;
  SET @param_circle_total_round_num = circle_total_round_num;
  SET @param_circle_num = circle_num;
  SET @param_circle_level_stage = circle_level_stage;
  SET @param_circle_condition = circle_condition;
  SET @param_trip_num = trip_num;
  SET @param_exploit_award = exploit_award;
  SET @param_daily_exploit = daily_exploit;
  SET @param_transport_finished_count = transport_finished_count;
  SET @param_transport_free_refreshed_count = transport_free_refreshed_count;
  SET @param_transport_robed_count = transport_robed_count;
  SET @param_transport_current_transport = transport_current_transport;
  SET @param_transport_status = transport_status;

  START TRANSACTION;

  SET @num = 0;
  SET @sql = CONCAT('select count(*) into @num from `', @table_name, '` where `actor` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;
  IF @num = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`actor`, `circle_flag`, `round_num`, `total_round`, `circle_num`, `lv_stage`, `condition_id`, `trip_num`, ',
          '`exploit_award`, `daily_exploit`, `transport_finished_count`, `transport_free_refreshed_count`, `transport_robed_count`, `transport_current_transport`, ',
          '`transport_status`) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @param_circle_flag, @param_circle_round_num,
          @param_circle_total_round_num, @param_circle_num, @param_circle_level_stage,
          @param_circle_condition, @param_trip_num, @param_exploit_award, @param_daily_exploit,
          @param_transport_finished_count, @param_transport_free_refreshed_count,
          @param_transport_robed_count, @param_transport_current_transport, @param_transport_status;
  ELSE
      SET @sql =  CONCAT('update `', @table_name, '` set `circle_flag` = ?, `round_num` = ?, `total_round` = ?, `circle_num` = ?, `lv_stage` = ?, `condition_id` = ?, ',
          '`trip_num` = ?, `exploit_award` = ?, `daily_exploit` = ?, `transport_finished_count` = ?, `transport_free_refreshed_count` = ?, `transport_robed_count` = ?, ',
          '`transport_current_transport` = ?, `transport_status` = ? where `actor` = ? ');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_circle_flag, @param_circle_round_num, @param_circle_total_round_num,
          @param_circle_num, @param_circle_level_stage, @param_circle_condition, @param_trip_num,
          @param_exploit_award, @param_daily_exploit, @param_transport_finished_count,
          @param_transport_free_refreshed_count, @param_transport_robed_count, @param_transport_current_transport,
          @param_transport_status, @param_id;
  END IF;
  
  COMMIT;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_TASK_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_TASK_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_TASK_SAVE`(IN `id` bigint, IN `active_count` int, IN `actives` varchar(10240), IN `finish_count` int, IN `finishes` varchar(10240))
BEGIN

  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_active_count = active_count;
  SET @param_finish_count = finish_count;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('task-', @table_number);

  SET @task_info = '';
  SET @task_id = 0;
  SET @task_cond_type1 = 0;
  SET @task_var1 = 0;
  SET @task_value1 = 0;
  SET @task_cond_type2 = 0;
  SET @task_var2 = 0;
  SET @task_value2 = 0;
  SET @task_cond_type3 = 0;
  SET @task_var3 = 0;
  SET @task_value3 = 0;
  SET @param_task_type = 0;

  START TRANSACTION;

  SET @sql = CONCAT('update `', @table_name, '` set `flag` = 0 where `actor` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  SET @idx = 1;

  WHILE @idx <= @param_active_count DO
    SET @task_info = SUBSTRING_INDEX(SUBSTRING_INDEX(actives, ',', @idx), ',', -1);
    SET @task_id = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 1), ':', -1));
    SET @task_cond_type1 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 2), ':', -1));
    SET @task_var1 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 3), ':', -1));
    SET @task_value1 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 4), ':', -1));
    SET @task_cond_type2 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 5), ':', -1));
    SET @task_var2 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 6), ':', -1));
    SET @task_value2 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 7), ':', -1));
    SET @task_cond_type3 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 8), ':', -1));
    SET @task_var3 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 9), ':', -1));
    SET @task_value3 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 10), ':', -1));
	SET @param_task_type = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 11), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor` = ? and `task` = ? and `task_type` = ? ');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @task_id, @param_task_type;

    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`actor`, `task`, `flag`, `cond_type1`, `var1`, `value1`, `cond_type2`, `var2`, `value2`, `cond_type3`, `var3`, `value3`, `task_type`) values (?, ?, 1, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @task_id, @task_cond_type1, @task_var1, @task_value1, @task_cond_type2, @task_var2, @task_value2, @task_cond_type3, @task_var3, @task_value3, @param_task_type;
    ELSE
      SET @sql =  CONCAT('update `', @table_name, '` set `flag` = 1, `cond_type1` = ?, `var1` = ?, `value1` = ?, `cond_type2` = ?, `var2` = ?, `value2` = ?, `cond_type3` = ?, `var3` = ?, `value3` = ? where `actor` = ? and `task` = ? and `task_type` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @task_cond_type1, @task_var1, @task_value1, @task_cond_type2, @task_var2, @task_value2, @task_cond_type3, @task_var3, @task_value3, @param_id, @task_id, @param_task_type;
    END IF;

    SET @idx = @idx + 1;

  END WHILE;

  set @idx = 1;
  WHILE @idx <= @param_finish_count DO
    SET @task_info = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(finishes, ',', @idx), ',', -1));
	SET @task_id = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 1), ':', -1));
    SET @task_type = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@task_info, ':', 2), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor` = ? and `task` = ? and `task_type` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @task_id, @task_type;

    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`actor`, `task`, `flag`, `task_type`) values (?, ?, 2, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @task_id, @task_type;
    ELSE
      SET @sql =  CONCAT('update `', @table_name, '` set `flag` = 2 where `actor` = ? and `task` = ? and `task_type` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @task_id, @task_type;
    END IF;

    SET @idx = @idx + 1;

  END WHILE;

  COMMIT;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_ACHIEVE_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_ACHIEVE_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_ACHIEVE_SAVE`(IN `id` bigint, IN `active_count` int, IN `actives` varchar(10240), IN `finish_count` int, IN `finishes` varchar(10240))
BEGIN

  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @param_active_count = active_count;
  SET @param_finish_count = finish_count;
  SET @table_number = @param_id % max_table_number;

  SET @table_name = CONCAT('achieve-', @table_number);

  SET @achieve_info = '';
  SET @achieve_id = 0;
  SET @condition1_param1 = 0;
  SET @condition1_param2 = 0;
  SET @condition1_value = 0;
  SET @task_cond_type2 = 0;
  SET @condition2_param1 = 0;
  SET @condition2_param2 = 0;
  SET @condition2_value = 0;
  SET @finish_time = 0;

  START TRANSACTION;

  SET @sql = CONCAT('update `', @table_name, '` set `flag` = 0 where `actor` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  SET @idx = 1;

  WHILE @idx <= @param_active_count DO
    SET @achieve_info = SUBSTRING_INDEX(SUBSTRING_INDEX(actives, ',', @idx), ',', -1);
    SET @achieve_id = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 1), ':', -1));
    SET @condition1_param1 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 2), ':', -1));
    SET @condition1_param2 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 3), ':', -1));
    SET @condition1_value = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 4), ':', -1));
    SET @condition2_param1 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 5), ':', -1));
    SET @condition2_param2 = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 6), ':', -1));
    SET @condition2_value = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 7), ':', -1));
	SET @finish_time = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 8), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor` = ? and `achieve_id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @achieve_id;

    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`actor`, `achieve_id`, `flag`, `condition1_param1`, `condition1_param2`, `condition1_value`, `condition2_param1`, `condition2_param2`, `condition2_value`, `finish_time`) values (?, ?, 1, ?, ?, ?, ?, ?, ?, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @achieve_id, @condition1_param1, @condition1_param2, @condition1_value, @condition2_param1, @condition2_param2, @condition2_value, @finish_time;
    ELSE
      SET @sql =  CONCAT('update `', @table_name, '` set `flag` = 1, `condition1_param1` = ?, `condition1_param2` = ?, `condition1_value` = ?, `condition2_param1` = ?, `condition2_param2` = ?, `condition2_value` = ?, `finish_time` = ? where `actor` = ? and `achieve_id` = ?' );
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @condition1_param1, @condition1_param2, @condition1_value, @condition2_param1, @condition2_param2, @condition2_value, @finish_time, @param_id, @achieve_id;
    END IF;

    SET @idx = @idx + 1;

  END WHILE;
  
  set @idx = 1;
  WHILE @idx <= @param_finish_count DO
    SET @achieve_info = SUBSTRING_INDEX(SUBSTRING_INDEX(finishes, ',', @idx), ',', -1);
	SET @achieve_id = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 1), ':', -1));
	SET @finish_time = CONCAT(SUBSTRING_INDEX(SUBSTRING_INDEX(@achieve_info, ':', 2), ':', -1));

    SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor` = ? and `achieve_id` = ? ');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id, @achieve_id;

    IF @count = 0 THEN
      SET @sql = CONCAT('insert into `', @table_name, '` (`actor`, `achieve_id`, `flag`, `finish_time`) values (?, ?, 2, ?)');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING @param_id, @achieve_id, @finish_time;
    ELSE
      SET @sql =  CONCAT('update `', @table_name, '` set `flag` = 2, `finish_time`=? where `actor` = ? and `achieve_id` = ?');
      PREPARE stmt FROM @sql;
      EXECUTE stmt USING  @finish_time, @param_id, @achieve_id;
    END IF;

    SET @idx = @idx + 1;

  END WHILE;
  COMMIT;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_WELFARE_ACTOR_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_WELFARE_ACTOR_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_WELFARE_ACTOR_SAVE`(IN `id` bigint,
    IN `title_id` int, IN `daily_checkins` int, IN `daily_award` tinyint, IN `general_checkins` int, IN `vip_checkins` int,
    IN `last_month_resource` int, IN `current_month_resource` int, IN `last_month_award_resource` tinyint, IN `daily_time` int, IN `total_time` int, IN `replenish_checkins_num` int, IN `online_award_items` varchar(20), IN `respenish_days` varchar(20))
BEGIN
  DECLARE max_table_number int DEFAULT 2;
  SET @param_id = id;
  SET @table_number = @param_id % max_table_number;
  SET @table_name = CONCAT('welfare-actor-', @table_number);

  SET @title_id = title_id;
  SET @daily_checkins = daily_checkins;
  SET @daily_award = daily_award;
  SET @general_checkins = general_checkins;
  SET @vip_checkins = vip_checkins;
  SET @online_award_items = online_award_items;
  SET @last_month_resource = last_month_resource;
  SET @current_month_resource = current_month_resource;
  SET @last_month_award_resource = last_month_award_resource;
  SET @daily_time = daily_time;
  SET @total_time = total_time;
  SET @replenish_checkins_num = replenish_checkins_num;
  set @respenish_days = respenish_days;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `actor` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`actor`, ',
                             '`title_id`, ',
                             '`daily_checkins`, ',
                             '`daily_award`, ',
                             '`general_checkins`, ',
                             '`vip_checkins`, ',
                             '`last_month_resource`, ',
                             '`current_month_resource`, ',
                             '`last_month_award_resource`, ',
                             '`daily_time`, ',
                             '`total_time`, ',
                             '`replenish_checkins_num`, ',
                             '`online_award_items`, ',
							 '`respenish_days`');
	select @field_list;
    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list, ') values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @title_id,
                       @daily_checkins,
                       @daily_award,
                       @general_checkins,
                       @vip_checkins,
                       @last_month_resource,
                       @current_month_resource,
                       @last_month_award_resource,
                       @daily_time,
                       @total_time,
                       @replenish_checkins_num,
                       @online_award_items,
					   @respenish_days;
  ELSE
    SET @sql = CONCAT('update `', @table_name, '` set `title_id`= ?, `daily_checkins`=?, `daily_award`=?, `general_checkins`=?, `vip_checkins`=? , `last_month_resource`=?, `current_month_resource`=?, `last_month_award_resource`=?, `daily_time`=?, `total_time`=?, `replenish_checkins_num`=?, `online_award_items`=?, `respenish_days`=? where `actor` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @title_id,
                       @daily_checkins,
                       @daily_award,
                       @general_checkins,
                       @vip_checkins,
                       @last_month_resource,
                       @current_month_resource,
                       @last_month_award_resource,
                       @daily_time,
                       @total_time,
                       @replenish_checkins_num,
                       @online_award_items,
					   @respenish_days,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_WORLD_DATA_LOAD
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_WORLD_DATA_LOAD`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_WORLD_DATA_LOAD`()
BEGIN
  SET @table_name = 'world-data';
  SET @field_list = CONCAT('`id`, ',
                           '`data`');
  SET @sql = CONCAT('select ', @field_list, ' from `', @table_name, '`');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for GAME_WORLD_DATA_SAVE
-- ----------------------------
DROP PROCEDURE IF EXISTS `GAME_WORLD_DATA_SAVE`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `GAME_WORLD_DATA_SAVE`(IN `id` int,
    IN `data` text)
BEGIN
  SET @table_name = 'world-data';
  SET @param_id = id;
  SET @param_data = data;

  START TRANSACTION;

  SET @sql = CONCAT('select count(1) into @count from `', @table_name, '` where `id` = ?');
  PREPARE stmt FROM @sql;
  EXECUTE stmt USING @param_id;

  IF @count = 0 THEN
    SET @field_list = CONCAT('`id`, ',
                             '`data`');
    SET @sql = CONCAT('insert into `', @table_name, '` (', @field_list, ') values(?, ?);');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_id,
                       @param_data;
  ELSE
    SET @update_list = CONCAT('`data`= ?');
    SET @sql = CONCAT('update `', @table_name, '` set ', @update_list, ' where `id` = ?');
    PREPARE stmt FROM @sql;
    EXECUTE stmt USING @param_data,
                       @param_id;
  END IF;

  COMMIT;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for STORAGE_TEST_ECHO
-- ----------------------------
DROP PROCEDURE IF EXISTS `STORAGE_TEST_ECHO`;
DELIMITER ;;
CREATE DEFINER=`buzz`@`%` PROCEDURE `STORAGE_TEST_ECHO`(IN `argu1` int, IN `argu2` varchar(255))
BEGIN

  SELECT argu1, argu2;

END;;
DELIMITER ;

