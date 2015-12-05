namespace cpp database.protocol

enum SoulActorField {
  AXE_SETTING,
  SHOOT_SETTING,
  MAGIC_SETTING,
  SETTINGS,
  ALTAR_SUMMON_PAID,
  ALTAR_FREE_SUMMON_TIMES,
  ALTAR_CARDS,
  ALTAR_CARD_BIND,
  CURRENT_ENERGY,

  MAX,
}

const list<string> kSoulActorFieldStr = [
  'axe_setting',                                  // 近战设置
  'shoot_setting',                                // 远程设置
  'magic_setting',                                // 法术设置
  'settings',                                     // 设置集
  'altar_summon_paid',                            // 英灵祭坛召唤是否已支付
  'altar_free_summon_times',                      // 英灵祭坛免费召唤次数
  'altar_cards',                                  // 英灵祭坛召唤得到的英灵卡
  'altar_card_bind',                              // 英灵祭坛召唤得到的英灵卡是否绑定
  'current_energy',                               // 当前能量值
]

enum SoulActorLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageSoulActorField {
  1 : required i32 axe_setting_;                  // 近战设置
  2 : required i32 shoot_setting_;                // 远程设置
  3 : required i32 magic_setting_;                // 法术设置
  4 : required string settings_;                  // 设置集
  5 : required bool altar_summon_paid_;           // 英灵祭坛召唤是否已支付
  6 : required i32 altar_free_summon_times_;      // 英灵祭坛免费召唤次数
  7 : required string altar_cards_;               // 英灵祭坛召唤得到的英灵卡
  8 : required bool altar_card_bind_;             // 英灵祭坛召唤得到的英灵卡是否绑定
  9 : required i32 current_energy_;               // 当前能量值
}

// type: STORAGE_SOUL_ACTOR_LOGIN

struct StorageSoulActorLoginRequest {
  1 : required i64 actor_id_;                     // 玩家ID
}

struct StorageSoulActorLoginResponse {
  1 : required SoulActorLoginResult result_;      // 结果
  2 : required StorageSoulActorField field_;      // 字段
}

