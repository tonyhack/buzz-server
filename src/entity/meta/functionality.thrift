namespace cpp entity
namespace py ccentity.functionality
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 系统功能类型定义
enum FunctionalityType {
  MIN = 0,

  //////////////////////////////
  // 道具
  ITEM_COMPOSE = 0,                                        // 道具合成
  EQUIP_INTENSIFY,                                         // 装备强化
  HUNT_TREASURE,                                           // 寻宝
  TREASURE,                                                // 藏宝阁

  //////////////////////////////
  // 宝石
  GEM_MODULE,                                              // 宝石模块开启
  GEM_SLOT_2,                                              // 宝石槽2个开启
  GEM_SLOT_3,                                              // 宝石槽3个开启
  GEM_SLOT_4,                                              // 宝石槽4个开启
  GEM_SLOT_5,                                              // 宝石槽5个开启
  GEM_SLOT_6,                                              // 宝石槽6个开启
  GEM_SLOT_7,                                              // 宝石槽7个开启
  GEM_SLOT_8,                                              // 宝石槽8个开启
  GEM_SLOT_9,                                              // 宝石槽9个开启

  //////////////////////////////
  // 英灵
  SOUL_MODULE,                                             // 英灵模块开启
  SOUL_ALTAR,                                              // 英灵祭坛开启
  SOUL_SETING,                                             // 幻化槽设置开启

  //////////////////////////////
  // 魔剑
  SWORD_MODULE,                                            // 魔剑模块开启

  //////////////////////////////
  // 贵族
  NOBLE_MODULE,                                            // 贵族模块开启

  //////////////////////////////
  // 元素
  ELEMENT_MODULE,                                          // 元素模块开启

  //////////////////////////////
  // 精灵
  SPIRIT_MODULE,                                           // 精灵模块开启
  SPIRIT_UPGRADE,                                          // 精灵进阶开启
  SPIRIT_WING_FACADE,                                      // 精灵翅膀外观开启

  //////////////////////////////
  // 名武
  LEGENDARY_WEAPON_MODULE,                                 // 名武模块开启

  //////////////////////////////
  // 邮件
  MAIL_MODULE,                                             // 邮件模块开启

  //////////////////////////////
  // 好友
  CONTACTS_MODULE,                                         // 好友模块开启

  //////////////////////////////
  // 家园
  HOMELAND_MODULE,                                         // 家园模块开启
  HOMELAND_GOLDMINE,                                       // 家园金矿开启
  HOMELAND_POOL,                                           // 家园神秘之源开启
  HOMELAND_CHURCH,                                         // 家园祈福堂开启
  HOMELAND_TREE,                                           // 家园神树开启
  HOMELAND_TOWER,                                          // 家园炮塔开启
  HOMELAND_STRONGHOLD,                                     // 家园军团据点开启

  //////////////////////////////
  // 组队
  TEAM_MODULE,                                             // 组队模块开启

  //////////////////////////////
  // 公会
  GUILD_MODULE,                                            // 公会模块开启
  
  //////////////////////////////
  // 任务
  TASK_CIRCLE,                                             // 循环任务功能开启
  TASK_RISK,                                               // 历险任务功能开启
  TASK_TRIP,                                               // 奇遇任务功能开启

  //////////////////////////////
  // 职业设置
  VOCATION_SET,                                            // 职业设置功能开启

  //////////////////////////////
  // 拍卖
  AUCTION_MODULE,                                          // 拍卖模块开启

  //////////////////////////////
  // 地宫
  MAZE_MODULE,                                             // 地宫模块开启

  //////////////////////////////
  // 副本
  PLAYING_MODULE,                                          // 副本模块开启
  MULTI_PLAYING,                                           // 多人副本开户

  MAX,
}

// 功能开启定义
enum FunctionalityOpenType {
  MIN = 0,
  ACTOR_LEVEL,                                             // 玩家升级
  TASK_ACCEPT,                                             // 任务接取
  TASK_FINISH,                                             // 任务完成
  MALL,                                                    // 商城
  SPIRIT_LEVEL,                                            // 精灵升级
  USE_ITEM,                                                // 使用道具
  CLIENT_REQUEST,                                          // 客户端请求
  HOMELAND_TEMPLE_LEVEL,                                   // 家园神殿等级
  MAX,
}
