namespace cpp database.protocol

enum ItemField {
  LOCATION = 0,
  VOCATION,
  TEMPLATE_ID,
  NUMBER,
  BIND,
  INTENSIFY_LEVEL,
  MAX_INTENSIFY_LEVEL,
  RANDOM_ATTR1,
  RANDOM_ATTR_VALUE1,
  RANDOM_ATTR2,
  RANDOM_ATTR_VALUE2,
  RANDOM_ATTR3,
  RANDOM_ATTR_VALUE3,
  RANDOM_ATTR4,
  RANDOM_ATTR_VALUE4,
  RANDOM_ATTR5,
  RANDOM_ATTR_VALUE5,
  RANDOM_ATTR6,
  RANDOM_ATTR_VALUE6,
  FIRST_USE_TIME,
  UPGRADE_LUCKY,

  MAX,
}

const list<string> kItemFieldStr = [
  "location",                                   // 容器内位置
  "vocation",                                   // 职业
  "template_id",                                // 模板ID
  "number"                                      // 数量
  "bind",                                       // 是否绑定
  "intensify_level",                            // 强化等级
  "max_intensify_level",                        // 最高已强化等级
  "random_attr1",                               // 随机属性1
  "random_attr_value1",                         // 随机属性效果1
  "random_attr2",                               // 随机属性2
  "random_attr_value2",                         // 随机属性效果2
  "random_attr3",                               // 随机属性3
  "random_attr_value3",                         // 随机属性效果3
  "random_attr4",                               // 随机属性4
  "random_attr_value4",                         // 随机属性效果4
  "random_attr5",                               // 随机属性5
  "random_attr_value5",                         // 随机属性效果5
  "random_attr6",                               // 随机属性6
  "random_attr_value6",                         // 随机属性效果6
  "first_use_time",                             // 首次使用时间
  "upgrade_lucky",                              // 升阶幸运值
]

enum ItemLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageItemField {
  1 : required i16 location_;
  2 : required i32 vocation_;
  3 : required i32 template_id_;
  4 : required i32 number_;
  5 : required byte bind_;
  6 : required byte intensify_level_;
  7 : required byte max_intensify_level_;
  8 : required byte random_attr1_;
  9 : required i32 random_attr_value1_;
  10: required byte random_attr2_;
  11: required i32 random_attr_value2_;
  12: required byte random_attr3_;
  13: required i32 random_attr_value3_;
  14: required byte random_attr4_;
  15: required i32 random_attr_value4_;
  16: required byte random_attr5_;
  17: required i32 random_attr_value5_;
  18: required byte random_attr6_;
  19: required i32 random_attr_value6_;
  20: required i64 first_use_time_;
  21: required i32 upgrade_lucky_;
}

// type: STORAGE_ITEM_LOGIN

struct StorageItemLoginRequest {
  1 : required i64 actor_id_;                   // 角色ID
  2 : required byte contain_type_;              // 容器类型
}

struct StorageItemLoginResponse {
  1 : required ItemLoginResult result_;         // 结果
  2 : optional list<StorageItemField> fields_   // 结果集
}

