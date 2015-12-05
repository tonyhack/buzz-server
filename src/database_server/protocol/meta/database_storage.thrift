namespace cpp database.protocol

enum StorageType {
  STORAGE_START = 0,

  STORAGE_TEST_ECHO = 0,
  STORAGE_TEST_SUM,

  STORAGE_CHECK_SESSION,
  STORAGE_ACTOR_ONLINE,

  STORAGE_ACTOR_LOGOUT_CENTER,

  STORAGE_WORLD_DATA_SAVE,            // 世界数据保存

  STORAGE_ACTOR_LOGIN,                // 玩家数据登陆
  STORAGE_ACTOR_SAVE,                 // 玩家数据保存

  STORAGE_COOLING_LOGIN,              // 冷却数据登陆
  STORAGE_COOLING_SAVE,               // 冷却数据保存
  
  STORAGE_SKILL_LOGIN,                // 技能数据登陆
  STORAGE_SKILL_SAVE,                 // 技能数据保存

  STORAGE_BUFF_LOGIN,                 // BUFF数据登陆
  STORAGE_BUFF_SAVE,                  // BUFF数据保存

  STORAGE_ITEM_ACTOR_LOGIN,           // 道具玩家数据登陆
  STORAGE_ITEM_ACTOR_SAVE,            // 道具玩家数据保存
  STORAGE_ITEM_LOGIN,                 // 道具数据读取
  STORAGE_ITEM_SAVE,                  // 道具数据保存
  STORAGE_GEM_LOGIN,                  // 宝石数据读取
  STORAGE_GEM_SAVE,                   // 宝石数据保存

  STORAGE_SOUL_LOGIN,                 // 英灵数据读取
  STORAGE_SOUL_SAVE,                  // 英灵数据保存
  STORAGE_SOUL_ACTOR_LOGIN,           // 英灵玩家数据读取
  STORAGE_SOUL_ACTOR_SAVE,            // 英灵玩家数据保存

  STORAGE_TASK_LOGIN,                 // 任务数据登陆
  STORAGE_TASK_SAVE,                  // 任务数据保存
  STORAGE_TASK_ACTOR_LOGIN,           // 任务玩家数据保存
  STORAGE_TASK_ACTOR_SAVE,            // 任务玩家数据保存

  STORAGE_ATTR_ACTOR_LOGIN,           // 属性玩家数据登陆
  STORAGE_ATTR_ACTOR_SAVE,            // 属性玩家数据保存

  STORAGE_SPIRIT_FACADE_LOGIN,        // 精灵外观数据登陆
  STORAGE_SPIRIT_FACADE_SAVE,         // 精灵外观数据保存
  
  STORAGE_MAIL_LOGIN,                 // 邮件玩家数据登录
  STORAGE_MAIL_ADD,                   // 添加邮件
  STORAGE_MAIL_DEL,                   // 删除邮件
  STORAGE_MAIL_PICK_AFFIX,            // 提取附件
  STORAGE_MAIL_READ,                  // 读取邮件
  STORAGE_MAIL_AFFIX_STATE,           // 变更邮件状态

  STORAGE_SOCIAL_ACTOR_LOGIN,         // 社交玩家数据登陆
  STORAGE_SOCIAL_ACTOR_SAVE,          // 社交玩家数据保存

  STORAGE_GUILD_DEL,                  // 删除公会
  STORAGE_GUILD_SAVE,                 // 公会数据保存
  STORAGE_GUILD_ACTOR_LOGIN,          // 公会玩家数据登陆
  STORAGE_GUILD_ACTOR_SAVE,           // 公会玩家数据保存

  STORAGE_PLAYING_ACTOR_LOGIN,        // 副本玩家数据登陆
  STORAGE_PLAYING_ACTOR_SAVE,         // 副本玩家数据保存

  STORAGE_AUCTION_ACTOR_LOGIN,        // 拍卖玩家数据登陆
  STORAGE_AUCTION_ACTOR_SAVE,         // 拍卖玩家数据保存
  STORAGE_AUCTION_TRANSACTION_DEL,    // 删除拍卖交易
  STORAGE_AUCTION_TRANSACTION_SAVE,   // 拍卖交易数据保存

  // 福利系统
  STORAGE_ACHIEVE_LOGIN,              // 成就列表登陆
  STORAGE_ACHIEVE_SAVE,               // 成就保存
  STORAGE_WELFARE_LOAD,               // 福利玩家登陆
  STORAGE_WELFARE_SAVE,               // 福利玩家保存

  STORAGE_MAX,
}


// type: STORAGE_TEST_ECHO
struct StorageTestEchoRequest {
  1 : required i32 argu1_;
  2 : required string argu2_;
}
struct StorageTestEchoResponse {
  1 : required i32 retvalue1_;
  2 : required string retvalue2_;
}


// type: STORAGE_TEST_SUM
struct StorageTestSumRequest {
  1 : required i32 argu1_;
  2 : required i32 argu2_;
}
struct StorageTestSumResponse {
  1 : required i32 sum_;
}

