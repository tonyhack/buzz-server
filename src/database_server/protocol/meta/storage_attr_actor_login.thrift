namespace cpp database.protocol

enum AttrActorField {
  NOBLE_LEVEL,
  NOBLE_USED_ITEM_COUNT,
  NOBLE_MAX_HP_ADD_VALUE,
  NOBLE_MAX_MP_ADD_VALUE,
  NOBLE_PHYSICS_ATTACK_ADD_VALUE,
  NOBLE_PHYSICS_DEFENCE_ADD_VALUE,
  NOBLE_MAGIC_ATTACK_ADD_VALUE,
  NOBLE_MAGIC_DEFENCE_ADD_VALUE,
  NOBLE_DODGE_ADD_VALUE,
  NOBLE_CRIT_ADD_VALUE,
  NOBLE_IGNORE_DEFENCE_ADD_VALUE,
  SWORD_AVAIL_STEP,
  SWORD_CURRENT_STEP,
  SWORD_LUCKY_VALUE,
  SWORD_GAIN_TIME,
  ELEMENT_LEVEL,
  SPIRIT_LEVEL,
  SPIRIT_LUCKY_VALUE,
  SPIRIT_CURRENT_FACADE_TYPE,
  SPIRIT_HORSE_FACADE_ID,
  SPIRIT_WING_FACADE_ID,
  RUNES,
  LEGENDARY_WEAPON_PIECES,

  MAX,
}

const list<string> kAttrActorFieldStr = [
  "noble_level",                                   // 贵族等级
  "noble_used_item_count",                         // 突破阶段已使用的道具数量
  "noble_max_hp_add_value",                        // 贵族提升的最大血量加成值
  "noble_max_mp_add_value",                        // 贵族提升的最大魔法加成值
  "noble_physics_attack_add_value",                // 贵族提升的物理攻击加成值
  "noble_physics_defence_add_value",               // 贵族提升的物理防御加成值
  "noble_magic_attack_add_value",                  // 贵族提升的魔法攻击加成值
  "noble_magic_defence_add_value",                 // 贵族提升的魔法防御加成值
  "noble_dodge_add_value",                         // 贵族提升的闪避强度加成
  "noble_crit_add_value",                          // 贵族提升的暴击强度加成值
  "noble_ignore_defence_add_value",                // 贵族提升的忽视防御比(万分之)
  "sword_avail_step",                              // 魔剑可用的最大阶数
  "sword_current_step",                            // 魔剑当前的使用阶数
  "sword_lucky_value",                             // 魔剑升阶幸运值
  "sword_gain_time",                               // 魔剑获取时间
  "element_level",                                 // 元素等级
  "spirit_level",                                  // 精灵等级
  "spirit_lucky_value",                            // 精灵升阶幸运值
  "spirit_current_facade_type",                    // 精灵当前的外观形态
  "spirit_horse_facade_id",                        // 精灵野兽形态外观ID
  "spirit_wing_facade_id",                         // 精灵翅膀形态外观ID
  "runes",                                         // 符文
  "legendary_weapon_pieces",                       // 名武碎片
]

enum AttrActorLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageAttrActorField {
  2 : required i32 noble_level_;
  3 : required i32 noble_used_item_count_;
  4 : required i32 noble_max_hp_add_value_;
  5 : required i32 noble_max_mp_add_value_;
  6 : required i32 noble_physics_attack_add_value_;
  7 : required i32 noble_physics_defence_add_value_;
  8 : required i32 noble_magic_attack_add_value_;
  9 : required i32 noble_magic_defence_add_value_;
  10: required i32 noble_dodge_add_value_;
  11: required i32 noble_crit_add_value_;
  12: required i32 noble_ignore_defence_add_value_;
  13: required i32 sword_avail_step_;
  14: required i32 sword_current_step_;
  15: required i32 sword_lucky_value_;
  16: required i32 sword_gain_time_;
  17: required i32 element_level_;
  18: required i32 spirit_level_;
  19: required i32 spirit_lucky_value_;
  20: required i32 spirit_current_facade_type_;
  21: required i32 spirit_horse_facade_id_;
  22: required i32 spirit_wing_facade_id_;
  23: required string rune_slot_data_;
  24: required string legendary_weapon_pieces_;
}

// type STORAGE_ATTR_ACTOR_LOGIN

struct StorageAttrActorLoginRequest {
  1 : required i64 actor_id_;                      // 玩家ID
}

struct StorageAttrActorLoginResponse {
  1 : required AttrActorLoginResult result_;       // 结果
  2 : required StorageAttrActorField field_;       // 属性玩家字段
}

