namespace cpp database.protocol

enum ItemActorField {
  GEM_GATHERING_POINT = 0,
  SHORTCUT1,
  SHORTCUT2,
  SHORTCUT3,
  SHORTCUT4,
  GIFTS_PICK_NUM,
  ITEM_BUY_NUM,

  MAX,
}

const list<string> kItemActorFieldStr = [
  "gem_gathering_point",                           // 宝石收集激活点
  "shortcut1",                                     // 道具快捷栏1
  "shortcut2",                                     // 道具快捷栏2
  "shortcut3",                                     // 道具快捷栏3
  "shortcut4",                                     // 道具快捷栏3
  "gifts_pick_num",                                // VIP礼包领取次数
  "item_buy_num",                                  // 道具购买次数
]

enum ItemActorLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageItemActorField {
  1 : required i32 gem_gathering_point_;
  2 : required i32 shortcut1;
  3 : required i32 shortcut2;
  4 : required i32 shortcut3;
  5 : required i32 shortcut4;
  6 : required i32 gifts_pick_num_;
  7 : required string item_buy_num_;
}

// type STORAGE_ITEM_ACTOR_LOGIN

struct StorageItemActorLoginRequest {
  1 : required i64 actor_id_;                      // 玩家ID
}

struct StorageItemActorLoginResponse {
  1 : required ItemActorLoginResult result_;       // 结果
  2 : required StorageItemActorField field_;       // 属性模块字段
}

