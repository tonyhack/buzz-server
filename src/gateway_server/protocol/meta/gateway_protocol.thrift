include "kickout_actor_type.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

enum MessageType {
  MESSAGE_MIN = 0,

  //////////////////////////////
  // 实体类消息
  MESSAGE_SYSTEM = 0,

  MESSAGE_VERIFY_QUESTION,                           // 要求验证连接
  MESSAGE_VERIFY_QUESTION_REQUEST,                   // 连接验证请求
  MESSAGE_VERIFY_QUESTION_RESPONSE,                  // 连接验证回复

  MESSAGE_TEST,                                      // 测试消息

  MESSAGE_LOGIN_REQUEST,                             // 登陆请求
  MESSAGE_LOGIN_RESPONSE,                            // 登陆回复

  MESSAGE_LOGOUT_REQUEST,                            // 登出请求
  MESSAGE_LOGOUT_RESPONSE,                           // 登出回复

  MESSAGE_KICKOUT_ACTOR,                             // 踢出玩家
  MESSAGE_CREATE_ACTOR,                              // 创建主角色(客户端为主角创建)
  MESSAGE_SYNCHRONIZE_ACTOR,                         // 玩家属性同步
  MESSAGE_SYNCHRONIZE_ACTOR_FUNCTIONALITY_STATE,     // 玩家系统功能开启状态同步
  MESSAGE_SYNCHRONIZE_ACTOR_SUB_FIGHT_SCORE,         // 玩家子战斗力同步
  MESSAGE_SYNCHRONIZE_NPC,                           // NPC属性同步
  MESSAGE_SYNCHRONIZE_SCENE_ITEM,                    // 场景道具属性同步
  MESSAGE_ACTOR_ENTER_MAP,                           // 主角色进入新地图
  MESSAGE_ACTOR_MOVE,                                // 主角色移动

  MESSAGE_ACTOR_RELOCATION,                          // 重新定位

  MESSAGE_ROLE_MOVE_SYNCHRONIZE,                     // 实体移动同步

  MESSAGE_CREATE_ACTOR_SYNCHRONIZE,                  // 创建玩家同步(同步给周围玩家，玩家进入地图、移动切屏时会同步给周围)
  MESSAGE_CREATE_NPC_SYNCHRONIZE,                    // 创建Npc同步(同步给周围玩家，玩家进入地图、移动切屏时会同步给周围)
  MESSAGE_CREATE_SCENE_ITEM_SYNCHRONIZE,             // 创建场景道具同步(同步给周围玩家，玩家进入地图、移动切屏时会同步给周围)
  MESSAGE_DESTORY_ROLE_SYNCHRONIZE,                  // 销毁实体同步(同步给周围玩家，玩家离开地图、移动切屏时会同步给周围)
  MESSAGE_ADD_EXP_SYNCHRONIZE,                       // 同步经验加成值
  MESSAGE_ACTOR_VIP_SYNCHRONIZE,                     // VIP时间同步
  MESSAGE_WORLD_JUMP_VIP_NUM_SYNCHRONIZE,            // 世界跳转次数

  MESSAGE_CREATE_AOI_ROLES,                          // 创建自己AOI中的所有实体(玩家在进入地图、移动切屏、传送时收到)
  MESSAGE_DESTORY_AOI_ROLES,                         // 销毁自己AOI中的所有实体(玩家在离开地图、移动切屏、传送时收到)

  MESSAGE_ACTOR_FACADE_REQUEST,                      // 玩家外观请求
  MESSAGE_ACTOR_FACADE_RESPONSE,                     // 玩家外观回应

  MESSAGE_ACTOR_FUNCTIONALITY_OPEN_REQUEST,          // 功能开启请求
  MESSAGE_ACTOR_FUNCTIONALITY_OPEN_RESPONSE,         // 功能开启响应

  MESSAGE_ACTOR_USE_TASLIMAN_REQUEST,                // 玩家使用护符请求
  MESSAGE_ACTOR_USE_TASLIMAN_RESPONSE,               // 玩家使用护符响应
  MESSAGE_ACTOR_TASLIMAN_SYNCHRONIZE,                // 护符值同步
  //////////////////////////////

  //////////////////////////////
  // 返回错误码
  MESSAGE_ERROR_CODE,                                // 错误信息(server->client)
  //////////////////////////////

  /////////////////////////////////
  // 公告类消息
  MESSAGE_INFORM_NOTIFY,                             // 公告信息(server->client)
  /////////////////////////////////

  //////////////////////////////
  // 技能类消息
  MESSAGE_SKILL_OPERATE,                             // 技能操作(client->server)
  MESSAGE_SKILL_HIT,                                 // 技能命中(client->server, server->client)
  MESSAGE_ACTOR_CHOOSE_PK_MODE,                      // 玩家选择PK模式(client->server)
  MESSAGE_ENTER_FIGHTING_STATUS,                     // 进入战斗状态(client->server)
  MESSAGE_LEAVE_FIGHTING_STATUS,                     // 离开战斗状态(client->server)
  MESSAGE_ENTER_PVP_STATUS,                          // 进入PVP状态(client->server)
  MESSAGE_LEAVE_PVP_STATUS,                          // 离开PVP状态(client->server)
  MESSAGE_PVP_STATUS_BROADCAST,                      // 广播PVP状态(server->client)
  MEESAGE_SKILL_BREAK_SYNCHRONIZE,                   // 技能打断(client->server)
  MESSAGE_SKILL_ATTRIBUTE_SYNCHRONIZE,               // 技能属性同步(server->cilent)
  //////////////////////////////
 
  //////////////////////////////
  // BUFF类消息
  MESSAGE_BUFF_INITIALIZE,                           // BUFF系统初始化数据(server->client)
  MESSAGE_ADD_BUFF,                                  // 增加一个BUFF(server->client)
  MESSAGE_REMOVE_BUFF,                               // 删除一个BUFF(server->client)
  //////////////////////////////
  
  //////////////////////////////
  // 道具类消息
  MESSAGE_ITEM_CONTAINER_INITIALIZE,                 // 道具容器初始化(server->client)
  MESSAGE_ITEM_CREATE_SYNCHRONIZE,                   // 创建道具同步(server->client)
  MESSAGE_SYNCHRONIZE_ITEM,                          // 道具类属性更新(server->client)
  MESSAGE_ITEM_CONTAINER_OPERATE,                    // 道具容器操作(server->client)
  MESSAGE_ITEM_MOVE,                                 // 移动容器内的道具(client->server, server->client)
  MESSAGE_ITEM_REMOVE,                               // 丢弃容器内的道具(client->server)
  MESSAGE_ITEM_USE,                                  // 使用容器内的道具(client->server)
  MESSAGE_ITEM_SPLIT,                                // 拆分容器内的道具(client->server)
  MESSAGE_ITEM_ARRANGE,                              // 整理容器内的道具(client->server)
  MESSAGE_ITEM_ARRANGE_RESULT,                       // 整理容器后的结果(server->client)
  MESSAGE_EXTEND_CONTAINER,                          // 扩展容器格子(client->server)

  MESSAGE_ITEM_COMPOSE_REQUEST,                      // 道具合成请求(client->server)
  MESSAGE_ITEM_COMPOSE_RESPONSE,                     // 道具合成回复(server->client)

  MESSAGE_CREATE_ACTOR_EQUIP_BROADCAST,              // 创建玩家装备广播(广播给需要新创建的玩家)
  MESSAGE_UPDATE_ACTOR_EQUIP_BROADCAST,              // 更新装备广播(穿脱装备时广播给周围玩家)
  MESSAGE_EQUIP_COMPARE_REQUEST,                     // 装备栏与玩家基本信息对比请求(client->server)
  MESSAGE_EQUIP_COMPARE_RESPONSE,                    // 装备栏与玩家基本信息对比回应(server->client)
  MESSAGE_EQUIP_INTENSIFY_REQUEST,                   // 装备强化请求(client->server)
  MESSAGE_EQUIP_INTENSIFY_RESPONSE,                  // 装备强化回应(server->client)
  MESSAGE_EQUIP_REFINE_REQUEST,                      // 装备洗练请求(client->server)
  MESSAGE_EQUIP_REFINE_RESPONSE,                     // 装备洗练回应（server->client)
  MESSAGE_EQUIP_REFINE_CONFIRM_REQUEST,              // 装备洗练玩家确认(client->server)
  MESSAGE_EQUIP_REFINE_CONFIRM_RESPONSE,             // 装备洗练玩家确认返回(server->client)
  MESSAGE_EQUIP_UPGRADE_REQUEST,                     // 装备升阶请求(client->server)
  MESSAGE_EQUIP_UPGRADE_RESPONSE,                    // 装备升阶回应(server->client)
  MESSAGE_EQUIP_INHERIT_REQUEST,                     // 装备继承请求(client->server)
  MESSAGE_EQUIP_INHERIT_RESPONSE,                    // 装备继承回复(server->client)

  MESSAGE_GEM_INITIALIZE,                            // 宝石数据初始化(server->client)
  MESSAGE_GEM_SYNCHRONIZE,                           // 宝石数据同步(server->client)
  MESSAGE_GEM_GATHER_REQUEST,                        // 宝石聚集请求(client->server)
  MESSAGE_GEM_GATHER_RESPONSE,                       // 宝石聚集回复(server->client)
  MESSAGE_GEM_SKIP_GATHERING_POINT_REQUEST,          // 宝石略过激活点请求(client->server)
  MESSAGE_GEM_SKIP_GATHERING_POINT_RESPONSE,         // 宝石略过激活点回复(server->client)
  MESSAGE_GEM_MOVE_REQUEST,                          // 宝石移动请求(client->server)
  MESSAGE_GEM_MOVE_RESPONSE,                         // 宝石移动回复(server->client)
  MESSAGE_GEM_PROMOTE_REQUEST,                       // 宝石提升请求(client->server)
  MESSAGE_GEM_PROMOTE_RESPONSE,                      // 宝石提升回复(server->client)
  MESSAGE_GEM_DECOMPOSE_REQUEST,                     // 宝石拆解请求(client->server)
  MESSAGE_GEM_DECOMPOSE_RESPONSE,                    // 宝石拆解回复(server->client)
  MESSAGE_GEM_LOCK_REQUEST,                          // 宝石锁定请求(client->server)
  MESSAGE_GEM_LOCK_RESPONSE,                         // 宝石锁定回复(server->client)
  MESSAGE_GEM_LOCK_ALL_REQUEST,                      // 宝石全部锁定请求(client->server)
  MESSAGE_GEM_LOCK_ALL_RESPONSE,                     // 宝石全部锁定回复(server->client)

  MESSAGE_SHORTCUT_INITIALIZE,                       // 道具快捷栏初始化(server->client)
  MESSAGE_SHORTCUT_SET_REQUEST,                      // 设置道具快捷栏请求(client->server)
  MESSAGE_SHORTCUT_SET_RESPONSE,                     // 设置道具快捷栏回应(server->client)

  MESSAGE_VIP_GIFTS_PICK_REQUEST,                    // VIP礼包领取请求(client->server)
  MESSAGE_VIP_GIFTS_PICK_RESPONSE,                   // VIP礼包领取请求回应(server->client)
  MESSAGE_VIP_GIFTS_SYNCHRONIZE,                     // vip礼包数据同步(server->client)

  MESSAGE_ITEM_TREASURE_HUNT_REQUEST,                // 寻宝请求(client->gateway)
  MESSAGE_ITEM_TREASURE_HUNT_RESPONSE,               // 寻宝响应(gateway->client)
  MESSAGE_ITEM_TREASURE_HUNT_NOTICE,                 // 寻宝公告(gateway->client)
  //////////////////////////////

  //////////////////////////////
  // GM消息
  MESSAGE_GM_ADD_ITEM,                               // 增加道具
  MESSAGE_GM_SCENE_JUMP,                             // 场景跳转
  MESSAGE_GM_OPEN_SWORD,                             // 开启魔剑
  MESSAGE_GM_COMMAND,                                // 通用GM指令
  //////////////////////////////


  //////////////////////////////
  // 场景消息
  MESSAGE_SCENE_REQUEST_JUMP,                        // 请求跳转场景(client->gateway)
  MESSAGE_SCENE_ACTOR_JUMP,                          // 玩家跳转场景(gateway->client)
  MESSAGE_SCENE_DAYNIGHT_CHANGE,                     // 场景昼夜转换(server->client)
  MESSAGE_SCENE_ROLE_KILLED,                         // 角色被杀死消息(server->client)
  MESSAGE_SCENE_GATHER,                              // 采集(client->server)
  MESSAGE_SCENE_GATHER_RESPONSE,                     // 采集响应(server->client)
  MESSAGE_SCENE_PICK_UP_REQUEST,                     // 拾取请求(client->server)
  MESSAGE_SCENE_PICK_UP_RESPONSE,                    // 拾取回应(server->client)
  MESSAGE_SCENE_JUMP_REQUEST,                        // 请求跳转(client->server)
  MESSAGE_SCENE_JUMP_RESPONSE,                       // 请求跳转返回(server->client)
  MESSAGE_SCENE_WORLD_BOSS_INITIALIZE,               // 初始化玩家场景数据(gateway->client)
  MESSAGE_SCENE_BOSS_KILLED,                         // 广播世界BOSS被杀死(gateway->client)
  MESSAGE_SCENE_SUMMON_WORLD_BOSS,                   // 召唤世界BOSS(gateway->client)
  MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_REQUEST,    // 玩家选中复活方式(client->server)
  MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_RESPONSE,   // 玩家选中复活方式(server->client)
  MESSAGE_SCENE_ZAZEN_REQUEST,                       // 请求打坐(client->gateway)
  MESSAGE_SCENE_ZAZEN_RESPONSE,                      // 请求打坐响应(gateway->client)
  MESSAGE_SCENE_CANCEL_ZAZEN_REQUEST,                // 请求取消打坐(client->gateway)
  MESSAGE_SCENE_CANCEL_ZAZEN_RESPONSE,               // 请求取消打坐返回(gateway->client)
  MESSAGE_SCENE_ZAZEN_INCOME_NOTICE,                 // 通知客户端冥想获取的收益(gateway->client)
  MESSAGE_SCENE_CONSECUTIVE_KILL_NOTICE,             // 通知客户端连斩(gateway->client)
  MESSAGE_SCENE_BRANCH_SYNCHRONIZE,                  // 分线同步(gateway->client)
  MESSAGE_SCENE_BRANCH_STATUS_REQUEST,               // 分线状态请求(client->gateway)
  MESSAGE_SCENE_BRANCH_STATUS_RESPONSE,              // 分线状态回应(gateway->client)
  MESSAGE_SCENE_BRANCH_SWITCH_REQUEST,               // 分线切换请求(client->gateway)
  MESSAGE_SCENE_BRANCH_SWITCH_RESPONSE,              // 分线切换回应(gateway->client)
  //////////////////////////////

  //////////////////////////////
  // 英灵消息
  MESSAGE_SOUL_INITIALIZE,                           // 英灵初始化(server->client)
  MESSAGE_SOUL_SYNCHRONIZE,                          // 英灵数据同步(server->client)
  MESSAGE_SOUL_UNLOCK_REQUEST,                       // 英灵解锁请求(client->server)
  MESSAGE_SOUL_UNLOCK_RESPONSE,                      // 英灵解锁回复(server->client)
  MESSAGE_SOUL_UPGRADE_STEP_REQUEST,                 // 英灵升阶请求(client->server)
  MESSAGE_SOUL_UPGRADE_STEP_RESPONSE,                // 英灵升阶回复(server->client)
  MESSAGE_SOUL_UPGRADE_LEVEL_REQUEST,                // 英灵升级请求(client->server)
  MESSAGE_SOUL_UPGRADE_LEVEL_RESPONSE,               // 英灵升级回复(server->client)
  MESSAGE_SOUL_SET_REQUEST,                          // 英灵启用请求(client->server)
  MESSAGE_SOUL_SET_RESPONSE,                         // 英灵启用回复(server->client)
  MESSAGE_SOUL_RESET_REQUEST,                        // 英灵取消请求(client->server)
  MESSAGE_SOUL_RESET_RESPONSE,                       // 英灵取消回复(server->client)
  MESSAGE_SOUL_SETTING_STAND_REQUEST,                // 英灵设置启用请求(client->server)
  MESSAGE_SOUL_SETTING_STAND_RESPONSE,               // 英灵设置启用回复(server->client)
  MESSAGE_SOUL_SKILL_UPGRADE_REQUEST,                // 英灵技能升级请求(client->server)
  MESSAGE_SOUL_SKILL_UPGRADE_RESPONSE,               // 英灵技能升级回应(server->client)

  MESSAGE_SOUL_ALTAR_INITIALIZE,                     // 英灵祭坛数据初始化(server->client)
  MESSAGE_SOUL_ALTAR_SYNCHRONIZE,                    // 英灵祭坛数据同步(server->client)
  MESSAGE_SOUL_ALTAR_CARD_SYNCHRONIZE,               // 英灵祭坛卡片数据同步(server->client)
  MESSAGE_SOUL_ALTAR_SUMMON_REQUEST,                 // 英灵祭坛召唤请求(client->server)
  MESSAGE_SOUL_ALTAR_SUMMON_RESPONSE,                // 英灵祭坛召唤回复(server->client)
  MESSAGE_SOUL_ALTAR_TAKE_REQUEST,                   // 英灵祭坛收取请求(client->server)
  MESSAGE_SOUL_ALTAR_TAKE_RESPONSE,                  // 英灵祭坛收取回复(server->client)
  MESSAGE_SOUL_ALTAR_RESET_REQUEST,                  // 英灵祭坛放弃请求(client->server)
  MESSAGE_SOUL_ALTAR_RESET_RESPONSE,                 // 英灵祭坛放弃回复(server->client)
  //////////////////////////////

  //////////////////////////////
  // 任务消息
  MESSAGE_TASK_INITIALIZE,                           // 任务数据初始化(server->client)
  MESSAGE_TASK_SYNCHRONIZE,                          // 任务数据同步(server->client)
  MESSAGE_TASK_ACCEPT,                               // 接受一个任务(client->server, server->client)
  MESSAGE_TASK_FINISH,                               // 完成一个任务(client->server, server->client)
  MESSAGE_TASK_ABANDON,                              // 放弃一个任务(client->server, server->client)
  MESSAGE_TASK_TALK_NPC,                             // 与NPC对话(client->server)
  MESSAGE_TASK_CIRCLE_INITIALIZE,                    // 循环任务初始化(server->client)
  MESSAGE_TASK_CIRCLE_OPT_REQUEST,                   // 循环任务操作类型(client->server)
  MESSAGE_TASK_CIRCLE_OPT_RESPONSE,                  // 循环任务操作响应(server->client)
  MESSAGE_TASK_TRIP_INITIALIZE,                      // 奇遇任务初始化(server->client)
  MESSAGE_TASK_TRIP_OPT_REQUEST,                     // 奇遇任务操作类型(client->server)
  MESSAGE_TASK_TRIP_OPT_RESPONSE,                    // 奇遇任务操作类型(server->client)
  MESSAGE_TASK_SUMMON_NPC_REQUEST,                   // 请求召唤NPC(client->server)
  MESSAGE_TASK_SUMMON_NPC_RESPONSE,                  // 请求召唤NPC返回(server->client)
  MESSAGE_TASK_DAILY_INITIALIZE,                     // 每日任务初始化(server->client)
  MESSAGE_TASK_DAILY_REQUEST,                        // 每日任务请求(client->server)
  MESSAGE_TASK_DAILY_RESPONSE,                       // 每日任务响应(server->client)
  MESSAGE_TASK_DAILY_AWARD_REQUEST,                  // 请求每日任务的奖励(client->server)
  MESSAGE_TASK_DAILY_AWARD_RESPONSE,                 // 请求每日任务的奖励返回(server->client)
  MESSAGE_TASK_TRANSPORT_INITIALIZE,                 // 护送任务初始化(server->client)
  MESSAGE_TASK_TRANSPORT_SYNCHRONIZE,                // 护送任务数据同步(server->client)
  MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_REQUEST,    // 护送任务刷新请求(client->server)
  MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_RESPONSE,   // 护送任务刷新回应(server->client)
  MESSAGE_TASK_TRANSPORT_ACCEPT_REQUEST,             // 护送任务接受请求(client->server)
  MESSAGE_TASK_TRANSPORT_ACCEPT_RESPONSE,            // 护送任务接受回应(server->client)
  MESSAGE_TASK_TRANSPORT_FINISH_REQUEST,             // 护送任务完成请求(client->server)
  MESSAGE_TASK_TRANSPORT_FINISH_RESPONSE,            // 护送任务完成回应(server->client)
  ////////////////////////////// 

  //////////////////////////////
  // 属性消息
  MESSAGE_ATTR_AOI_SYNCHRONIZE,                      // 属性AOI数据同步(server->client)
  MESSAGE_ATTR_SWORD_INITIALIZE,                     // 魔剑数据初始化(server->client)
  MESSAGE_ATTR_SWORD_UPGRADE_REQUEST,                // 魔剑进阶请求(client->server)
  MESSAGE_ATTR_SWORD_UPGRADE_RESPONSE,               // 魔剑进阶回复(server->client)
  MESSAGE_ATTR_SWORD_SYNCHRONIZE,                    // 魔剑属性同步(server->client)
  MESSAGE_ATTR_SWORD_EQUIP,                          // 魔剑装备请求(client->server)
  MESSAGE_ATTR_SWORD_UNEQUIP,                        // 魔剑卸下请求(client->server)
  MESSAGE_ATTR_SWORD_COMPARE_REQUEST,                // 魔剑对比请求(client->server)
  MESSAGE_ATTR_SWORD_COMPARE_RESPONSE,               // 魔剑对比回复(server->client)
  MESSAGE_ATTR_NOBLE_INITIALIZE,                     // 贵族数据初始化(server->client)
  MESSAGE_ATTR_NOBLE_PROMOTE_REQUEST,                // 贵族提升请求(client->server)
  MESSAGE_ATTR_NOBLE_PROMOTE_RESPONSE,               // 贵族提升回复(server->client)
  MESSAGE_ATTR_NOBLE_BREAKTHROUGH_REQUEST,           // 贵族突破请求(client->server)
  MESSAGE_ATTR_NOBLE_BREAKTHROUGH_RESPONSE,          // 贵族突破回复(server->client)
  MESSAGE_ATTR_NOBLE_COMPARE_REQUEST,                // 贵族对比请求(client->server)
  MESSAGE_ATTR_NOBLE_COMPARE_RESPONSE,               // 贵族对比回复(server->client)
  MESSAGE_ATTR_ELEMENT_INITIALIZE,                   // 元素数据初始化(server->client)
  MESSAGE_ATTR_ELEMENT_UPGRADE_REQUEST,              // 元素升级请求(client->server)
  MESSAGE_ATTR_ELEMENT_UPGRADE_RESPONSE,             // 元素升级回复(server->client)
  MESSAGE_ATTR_ELEMENT_COMPARE_REQUEST,              // 元素对比请求(client->server)
  MESSAGE_ATTR_ELEMENT_COMPARE_RESPONSE,             // 元素对比回复(server->client)
  MESSAGE_ATTR_SPIRIT_INITIALIZE,                    // 精灵数据初始化(server->client)
  MESSAGE_ATTR_SPIRIT_SYNCHRONIZE,                   // 精灵数据同步(server->client)
  MESSAGE_ATTR_SPIRIT_FACADE_SYNCHRONIZE,            // 精灵外观数据同步(server->client)
  MESSAGE_ATTR_SPIRIT_UPGRADE_REQUEST,               // 精灵进阶请求(client->server)
  MESSAGE_ATTR_SPIRIT_UPGRADE_RESPONSE,              // 精灵进阶回复(server->client)
  MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_REQUEST,      // 精灵转换外观请求(client->server)
  MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_RESPONSE,     // 精灵转换外观回复(server->client)
  MESSAGE_ATTR_SPIRIT_COMPARE_REQUEST,               // 精灵对比请求(client->server)
  MESSAGE_ATTR_SPIRIT_COMPARE_RESPONSE,              // 精灵对比回复(server->client)
  MESSAGE_ATTR_RUNE_SYNCHRONIZE,                     // 符文槽位数据同步(server->client)
  MESSAGE_ATTR_RUNE_COMPOSE_REQUEST,                 // 符文合成请求(client->server)
  MESSAGE_ATTR_RUNE_COMPOSE_RESPONSE,                // 符文合成响应(server->client)
  MESSAGE_ATTR_RUNE_MOVE_REQUEST,                    // 符文移动请求(client->server)
  MESSAGE_ATTR_RUNE_MOVE_RESPONSE,                   // 符文移动响应(server->client)
  MESSAGE_ATTR_LEGENDARY_WEAPON_INITIALIZE,          // 名武数据初始化(server->client)
  MESSAGE_ATTR_LEGENDARY_WEAPON_PIECE_SYNCHRONIZE,   // 名武碎片数据同步(server->client)
  MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_REQUEST,      // 名武修复请求(client->server)
  MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_RESPONSE,     // 名武修复回复(server->client)
  //////////////////////////////

  //////////////////////////////
  // 聊天消息
  MESSAGE_CHAT_REQUEST,                              // 聊天请求(client->server)
  MESSAGE_CHAT_RESPONSE,                             // 聊天回复(server->client)
  MESSAGE_CHAT_SYNCHRONIZE,                          // 聊天同步(server->client)
  //////////////////////////////

  //////////////////////////////
  // 邮件消息
  MESSAGE_MAIL_INITIALIZE,                           // 邮件数据初始化(server->client)
  MESSAGE_MAIL_ADD,                                  // 邮件下发(server->client)
  MESSAGE_MAIL_SEND_REQUEST,                         // 发送邮件请求(client->server)
  MESSAGE_MAIL_SEND_RESPONSE,                        // 发送邮件返回(server->client)
  MESSAGE_MAIL_DEL_REQUEST,                          // 删除邮件请求(client->server)
  MESSAGE_MAIL_DEL_RESPONSE,                         // 删除邮件返回(server->client)
  MESSAGE_MAIL_READ_REQUEST,                         // 读取邮件请求(client->server)
  MESSAGE_MAIL_READ_RESPONSE,                        // 读取邮件返回(server->client)
  MESSAGE_MAIL_PICK_AFFIX_REQUEST,                   // 提取附件请求(client->server)
  MESSAGE_MAIL_PICK_AFFIX_RESPONSE,                  // 提取附件返回(server->client)
  MESSAGE_MAIL_DEL,                                  // 删除邮件(server->client)
  /////////////////////////////////
  
  ////////////////////////////
  // 商店
  MESSAGE_SHOP_BUYBACK_INITIALIZE,                   // 商店回购初始化(server->client)
  MESSAGE_SHOP_BUY_REQUEST,                          // 商店购买请求(client->server)
  MESSAGE_SHOP_BUY_RESPONSE,                         // 商店购买返回(server->client)
  MESSAGE_SHOP_BUYBACK_REQUEST,                      // 商店回购请求(client->server)
  MESSAGE_SHOP_BUYBACK_RESPONSE,                     // 商店回购相应(server->client)
  MESSAGE_SHOP_SELL_REQUEST,                         // 商店出售(client->server)
  MESSAGE_SHOP_SELL_RESPONSE,                        // 商店出售返回(server->client)

  //////////////////////////////
  // 社交消息
  MESSAGE_SOCIAL_CONTACTS_INITIALIZE,                // 联系人数据初始化(server->client)
  MESSAGE_SOCIAL_CONTACTS_SYNCHRONIZE,               // 联系人数据同步(server->client)
  MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_REQUEST,    // 编辑签名请求(client->server)
  MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_RESPONSE,   // 编辑签名回复(server->client)
  MESSAGE_SOCIAL_CONTACTS_ADD_REQUEST,               // 新增联系人请求(client->server)
  MESSAGE_SOCIAL_CONTACTS_ADD_RESPONSE,              // 新增联系人回复(server->client)
  MESSAGE_SOCIAL_CONTACTS_ADD_NOTIFY,                // 新增联系人通知(server->client)
  MESSAGE_SOCIAL_CONTACTS_REMOVE_REQUEST,            // 删除联系人请求(client->server)
  MESSAGE_SOCIAL_CONTACTS_REMOVE_RESPONSE,           // 删除联系人回复(server->client)
  MESSAGE_SOCIAL_CONTACTS_MOVE_REQUEST,              // 移动联系人请求(client->server)
  MESSAGE_SOCIAL_CONTACTS_MOVE_RESPONSE,             // 移动联系人回复(server->client)
  MESSAGE_SOCIAL_CONTACTS_RECOMMEND_REQUEST,         // 推荐联系人请求(client->server)
  MESSAGE_SOCIAL_CONTACTS_RECOMMEND_RESPONSE,        // 推荐联系人回复(server->client)
  MESSAGE_SOCIAL_ENEMY_INITIALIZE,                   // 仇人信息初始化(server->client)
  MESSAGE_SOCIAL_ENEMY_SYNCHRONIZE,                  // 仇人信息同步(server->client)
  MESSAGE_SOCIAL_ENEMY_ADD_REQUEST,                  // 新增仇人请求(client->server)
  MESSAGE_SOCIAL_ENEMY_ADD_RESPONSE,                 // 新增仇人回复(server->client)
  MESSAGE_SOCIAL_ENEMY_REMOVE_REQUEST,               // 删除仇人请求(client->server)
  MESSAGE_SOCIAL_ENEMY_REMOVE_RESPONSE,              // 删除仇人回复(server->client)
  //////////////////////////////

  //////////////////////////////
  // 家园消息
  MESSAGE_HOMELAND_INITIALIZE,                            // 家园数据初始化(server->client)
  MESSAGE_HOMELAND_SYNCHRONIZE,                           // 家园数据同步(server->client)
  MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE,                    // 家园好友数据同步(server->client)
  MESSAGE_HOMELAND_INVADER_SYNCHRONIZE,                   // 家园侵入者数据同步(server->client)
  MESSAGE_HOMELAND_SUPPORTED_SOUL_SYNCHRONIZE,            // 家园被支援英灵数据同步(server->client)
  MESSAGE_HOMELAND_SUPPORT_SOUL_SYNCHRONIZE,              // 家园支援英灵数据同步(server->client)
  MESSAGE_HOMELAND_EVENT_SYNCHRONIZE,                     // 家园告示板信息数据同步(server->client)
  MESSAGE_HOMELAND_VISIT_REQUEST,                         // 访问好友家园请求(client->server)
  MESSAGE_HOMELAND_VISIT_RESPONSE,                        // 访问好友家园回复(server->client)
  MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST,         // 清除升级冷却时间请求(client->server)
  MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE,        // 清除升级冷却时间回复(server->client)
  MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST,                // 神殿升级请求(client->server)
  MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE,               // 神殿升级回复(server->client)
  MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST,                // 神殿征收请求(client->server)
  MESSAGE_HOMELAND_TEMPLE_HARVEST_RESPONSE,               // 神殿征收回复(server->client)
  MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST,  // 清除神殿征收冷却时间请求(client->server)
  MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE, // 清除神殿征收冷却时间回复(server->client)
  MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST,          // 神殿强征请求(client->server)
  MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE,         // 神殿强征回复(server->client)
  MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST,              // 金矿升级请求(client->server)
  MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE,             // 金矿升级回复(server->client)
  MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST,              // 金矿收获请求(client->server)
  MESSAGE_HOMELAND_GOLDMINE_HARVEST_RESPONSE,             // 金矿收获回复(server->client)
  MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST,                  // 神秘之源升级请求(client->server)
  MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE,                 // 神秘之源升级回复(server->client)
  MESSAGE_HOMELAND_POOL_HARVEST_REQUEST,                  // 神秘之源领取请求(client->server)
  MESSAGE_HOMELAND_POOL_HARVEST_RESPONSE,                 // 神秘之源领取回复(server->client)
  MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST,           // 神秘之源双倍领取请求(client->server)
  MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE,          // 神秘之源双倍领取回复(server->client)
  MESSAGE_HOMELAND_CHURCH_PRAY_REQUEST,                   // 祈福堂祈福请求(client->server)
  MESSAGE_HOMELAND_CHURCH_PRAY_RESPONSE,                  // 祈福堂祈福回复(server->client)
  MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST,                // 祈福堂刷新请求(client->server)
  MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE,               // 祈福堂刷新回复(server->client)
  MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST,            // 祈福堂高级刷新请求(client->server)
  MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE,           // 祈福堂高级刷新回复(server->client)
  MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST,                // 祈福堂领取奖励请求(client->server)
  MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE,               // 祈福堂领取奖励回复(server->client)
  MESSAGE_HOMELAND_TREE_WATER_REQUEST,                    // 神树浇水请求(client->server)
  MESSAGE_HOMELAND_TREE_WATER_RESPONSE,                   // 神树浇水回复(server->client)
  MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST,             // 神树帮助好友浇水请求(client->server)
  MESSAGE_HOMELAND_TREE_ASSIST_WATER_RESPONSE,            // 神树帮助好友浇水回复(server->client)
  MESSAGE_HOMELAND_TREE_STEAL_REQUEST,                    // 神树抢收请求(client->server)
  MESSAGE_HOMELAND_TREE_STEAL_RESPONSE,                   // 神树抢收回复(server->client)
  MESSAGE_HOMELAND_TREE_HARVEST_REQUEST,                  // 神树收获请求(client->server)
  MESSAGE_HOMELAND_TREE_HARVEST_RESPONSE,                 // 神树收获回复(server->client)
  MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST,                 // 炮塔升级请求(client->server)
  MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE,                // 炮塔升级回复(server->client)
  MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST,            // 军团据点升级请求(client->server)
  MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE,           // 军团据点升级回复(server->client)
  MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_REQUEST,       // 军团据点申请支援请求(client->server)
  MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_RESPONSE,      // 军团据点申请支援回复(server->client)
  MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_NOTIFY,        // 军团据点申请支援通知(server->client)
  MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_REQUEST,            // 军团据点支援请求(client->server)
  MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_RESPONSE,           // 军团据点支援回复(server->client)
  MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST,                  // 家园攻防战匹配请求(client->server)
  MESSAGE_HOMELAND_BATTLE_MATCH_RESPONSE,                 // 家园攻防战匹配回复(server->client)
  MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST,            // 家园攻防战反击请求(client->server)
  MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_RESPONSE,           // 家园攻防战反击回复(server->client)
  MESSAGE_HOMELAND_BATTLE_FIGHT_DATA_NOTIFY,              // 家园攻防战战斗数据(server->client)
  //////////////////////////////
  
  ///////////////////////////////
  // 商城
  MESSAGE_MALL_BUY_REQUEST,                          // 商城购买(client->server)
  MESSAGE_MALL_BUY_RESPONSE,                         // 商城购买返回(server->client)
  MESSAGE_MALL_SYNCHRONIZE,                          // 商城、藏宝阁推送
  MESSAGE_MALL_ITEM_BUY_NUM_INITIALIZE,              // 玩家购买藏宝阁数据初始化           
  ///////////////////////////////

  /////////////////////////////////
  // 组队消息
  MESSAGE_TEAM_CREATE_REQUEST,                       // 请求(client->server)
  MESSAGE_TEAM_CREATE_RESPONSE,                      // 回应(server->client)
  MESSAGE_TEAM_JOIN_REQUEST,                         // 入队请求(client->server)
  MESSAGE_TEAM_JOIN_RESPONSE,                        // 入队回应(server->client)
  MESSAGE_TEAM_JOIN_NOTIFY,                          // 入队消息通知(server->client)
  MESSAGE_TEAM_JOIN_DISPOSE_REQUEST,                 // 申请入队处理请求(client->server)
  MESSAGE_TEAM_JOIN_DISPOSE_RESPONSE,                // 申请入队处理回应(server->client)
  MESSAGE_TEAM_INVITE_REQUEST,                       // 邀请入队请求(client->server)
  MESSAGE_TEAM_INVITE_RESPONSE,                      // 邀请入队回应(server->client)
  MESSAGE_TEAM_INVITE_NOTIFY,                        // 邀请消息通知(server->Client)
  MESSAGE_TEAM_INVITE_DISPOSE_REQUEST,               // 邀请入队处理请求(client->server)
  MESSAGE_TEAM_INVITE_DISPOSE_RESPONSE,              // 邀请入队处理回应(server->client)
  MESSAGE_TEAM_MEMBER_ADD_SYNCHRONIZE,               // 增加队员(server->client)
  MESSAGE_TEAM_MEMBER_REMOVE_SYNCHRONIZE,            // 移除队员(server->client)
  MESSAGE_TEAM_SYNCHRONIZE,                          // 组队信息同步(server->client)
  MESSAGE_TEAM_MEMBER_SYNCHRONIZE,                   // 队员信息同步(server->client)
  MESSAGE_TEAM_DISMISS_REQUEST,                      // 解散队伍请求(client->server)
  MESSAGE_TEAM_DISMISS_RESPONSE,                     // 解散队伍回应(server->client)
  MESSAGE_TEAM_DISMISS_NOTIFY,                       // 组队解散通知(server->client)
  MESSAGE_TEAM_QUIT_REQUEST,                         // 退出请求(client->server)
  MESSAGE_TEAM_QUIT_RESPONSE,                        // 退出回应(server->client)
  MESSAGE_TEAM_KICKOUT_REQUEST,                      // 踢出请求(client->server)
  MESSAGE_TEAM_KICKOUT_RESPONSE,                     // 踢出回应(server->client)
  MESSAGE_TEAM_SET_CAPTAIN_REQUEST,                  // 设置队长请求(client->server)
  MESSAGE_TEAM_SET_CAPTAIN_RESPONSE,                 // 设置队长回应(server->client)
  MESSAGE_TEAM_MAP_TEAMS_REQUEST,                    // 地图组队信息请求(client->server)
  MESSAGE_TEAM_MAP_TEAMS_RESPONSE,                   // 地图组队信息回应(server->client)
  MESSAGE_TEAM_MAP_ACTORS_REQUEST,                   // 地图玩家信息请求(client->server)
  MESSAGE_TEAM_MAP_ACTORS_RESPONSE,                  // 地图玩家信息回应(server->client)
  MESSAGE_TEAM_MEMBER_REQUEST,                       // 队员信息请求(client->server)
  MESSAGE_TEAM_MEMBER_RESPONSE,                      // 队员信息回应(server->client)
  MESSAGE_TEAM_ACTORS_BROADCAST,                     // 广播场景玩家组队信息(server->client)
  /////////////////////////////////

  /////////////////////////////////
  // 公会消息
  MESSAGE_GUILD_AOI_SYNCHRONIZE,                     // 公会AOI数据同步(server->client)
  MESSAGE_GUILD_INITIALIZE,                          // 公会数据初始化(server->client)
  MESSAGE_GUILD_SYNCHRONIZE,                         // 公会数据同步(server->client)
  MESSAGE_GUILD_PENDING_MEMBER_SYNCHRONIZE,          // 公会待审核成员数据同步(server->client)
  MESSAGE_GUILD_MEMBER_SYNCHRONIZE,                  // 公会成员数据同步(server->client)
  MESSAGE_GUILD_EVENT_SYNCHRONIZE,                   // 公会事件数据同步(server->client)
  MESSAGE_GUILD_SKILL_SYNCHRONIZE,                   // 公会技能数据同步(server->client)
  MESSAGE_GUILD_BUFF_SYNCHRONIZE,                    // 公会BUFF数据同步(server->client)
  MESSAGE_GUILD_LIST_REQUEST,                        // 公会列表数据请求(client->server)
  MESSAGE_GUILD_LIST_RESPONSE,                       // 公会列表数据回复(server->client)
  MESSAGE_GUILD_LIST_SYNCHRONIZE,                    // 公会列表数据同步(server->client)
  MESSAGE_GUILD_CREATE_REQUEST,                      // 创建公会请求(client->server)
  MESSAGE_GUILD_CREATE_RESPONSE,                     // 创建公会回复(server->client)
  MESSAGE_GUILD_DISMISS_NOTIFY,                      // 公会解散通知(server->client)
  MESSAGE_GUILD_SUPPRESS_JOIN_REQUEST,               // 公会关闭申请请求(client->server)
  MESSAGE_GUILD_SUPPRESS_JOIN_RESPONSE,              // 公会关闭申请回复(server->client)
  MESSAGE_GUILD_JOIN_REQUEST,                        // 公会申请请求(client->server)
  MESSAGE_GUILD_JOIN_RESPONSE,                       // 公会申请回复(server->client)
  MESSAGE_GUILD_CANCEL_JOIN_REQUEST,                 // 公会取消申请请求(client->server)
  MESSAGE_GUILD_CANCEL_JOIN_RESPONSE,                // 公会取消申请回复(server->client)
  MESSAGE_GUILD_AUDIT_PENDING_MEMBER_REQUEST,        // 公会审核成员加入请求(client->server)
  MESSAGE_GUILD_AUDIT_PENDING_MEMBER_RESPONSE,       // 公会审核成员加入回复(server->client) 
  MESSAGE_GUILD_INVITE_REQUEST,                      // 公会邀请请求(client->server)
  MESSAGE_GUILD_INVITE_RESPONSE,                     // 公会邀请回复(server->client)
  MESSAGE_GUILD_INVITE_NOTIFY,                       // 公会邀请通知(server->client)
  MESSAGE_GUILD_INVITE_DISPOSE_REQUEST,              // 公会邀请处理请求(client->server)
  MESSAGE_GUILD_INVITE_DISPOSE_RESPONSE,             // 公会邀请处理回复(server->client)
  MESSAGE_GUILD_LEAVE_REQUEST,                       // 离开公会请求(client->server)
  MESSAGE_GUILD_LEAVE_RESPONSE,                      // 离开公会回复(server->client)
  MESSAGE_GUILD_KICK_MEMBER_REQUEST,                 // 踢出成员请求(client->server)
  MESSAGE_GUILD_KICK_MEMBER_RESPONSE,                // 踢出成员回复(server->client)
  MESSAGE_GUILD_KICK_MEMBER_NOTIFY,                  // 踢出成员通知(server->client)
  MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_REQUEST,     // 放弃会长职位请求(client->server)
  MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_RESPONSE,    // 放弃会长职位回复(server->client)
  MESSAGE_GUILD_GIVE_UP_POSITION_REQUEST,            // 放弃公会职位请求(client->server)
  MESSAGE_GUILD_GIVE_UP_POSITION_RESPONSE,           // 放弃公会职位回复(server->client)
  MESSAGE_GUILD_SET_MEMBER_POSITION_REQUEST,         // 公会职位任免请求(client->server)
  MESSAGE_GUILD_SET_MEMBER_POSITION_RESPONSE,        // 公会职位任免回复(server->client)
  MESSAGE_GUILD_EDIT_ANNOUNCEMENT_REQUEST,           // 编辑公会公告请求(client->server)
  MESSAGE_GUILD_EDIT_ANNOUNCEMENT_RESPONSE,          // 编辑公会公告回复(server->client) 
  MESSAGE_GUILD_UPGRADE_REQUEST,                     // 公会升级请求(client->server)
  MESSAGE_GUILD_UPGRADE_RESPONSE,                    // 公会升级回复(server->client)
  MESSAGE_GUILD_DONATE_REQUEST,                      // 公会捐赠请求(client->server)
  MESSAGE_GUILD_DONATE_RESPONSE,                     // 公会捐赠回复(server->client)
  MESSAGE_GUILD_SHOP_UPGRADE_REQUEST,                // 公会商店升级请求(client->server)
  MESSAGE_GUILD_SHOP_UPGRADE_RESPONSE,               // 公会商店升级回复(server->client)
  MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST,               // 购买公会商店道具请求(client->server)
  MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE,              // 购买公会商店道具回复(server->client)
  MESSAGE_GUILD_COLLEGE_UPGRADE_REQUEST,             // 公会学院升级请求(client->server)
  MESSAGE_GUILD_COLLEGE_UPGRADE_RESPONSE,            // 公会学院升级回复(server->client)
  MESSAGE_GUILD_COLLEGE_LEARN_SKILL_REQUEST,         // 学习公会技能请求(client->server)
  MESSAGE_GUILD_COLLEGE_LEARN_SKILL_RESPONSE,        // 学习公会技能回复(server->client)
  MESSAGE_GUILD_IDOL_PRAY_REQUEST,                   // 神像祈祷请求(client->server)
  MESSAGE_GUILD_IDOL_PRAY_RESPONSE,                  // 神像祈祷回复(server->client)
  MESSAGE_GUILD_BARRACK_UPGRADE_REQUEST,             // 公会军营升级请求(client->server)
  MESSAGE_GUILD_BARRACK_UPGRADE_RESPONSE,            // 公会军营升级回复(server->client)
  MESSAGE_GUILD_PLAYING_GROUP_OPEN_REQUEST,          // 公会副本组解封请求(client->server)
  MESSAGE_GUILD_PLAYING_GROUP_OPEN_RESPONSE,         // 公会副本组解封回复(server->client)
  MESSAGE_GUILD_PLAYING_GROUP_OPEN_NOTIFY,           // 公会副本组解封通知(server->client)
  MESSAGE_GUILD_PLAYING_FINISHED_NOTIFY,             // 公会副本完成通知(server->client)
  MESSAGE_GUILD_PLAYING_AWARD_REQUEST,               // 公会副本领取奖励请求(client->server)
  MESSAGE_GUILD_PLAYING_AWARD_RESPONSE,              // 公会副本领取奖励回复(server->client)
  MESSAGE_GUILD_PLAYING_RESET_NOTIFY,                // 公会副本重置通知(server->client)
  /////////////////////////////////

  /////////////////////////////////
  // 副本消息
  MESSAGE_PLAYING_INITIALIZE,                        // 副本数据初始化(server->client)
  MESSAGE_PLAYING_CREATE_REQUEST,                    // 创建副本请求(client->server)
  MESSAGE_PLAYING_CREATE_RESPONSE,                   // 创建副本回应(server->client)
  MESSAGE_PLAYING_CREATE_FINISH_NOTICE,              // 创建副本完成通知(server->client)
  MESSAGE_PLAYING_ENTER_REQUEST,                     // 进入副本请求(client->server)
  MESSAGE_PLAYING_ENTER_RESPONSE,                    // 进入副本回应(server->client)
  MESSAGE_PLAYING_ENTER_INITIALIZE,                  // 进入副本初始化(server->client)
  MESSAGE_PLAYING_SCORE_SYNCHRONIZE,                 // 副本得分同步(server->client)
  MESSAGE_PLAYING_RANKING_SYNCHRONIZE,               // 副本排名同步(server->client)
  MESSAGE_PLAYING_COMPLETE_REQUEST,                  // 副本完成请求(client->server)
  MESSAGE_PLAYING_COMPLETE_RESPONSE,                 // 副本完成回应(server->client)
  MESSAGE_PLAYING_COMPLETE_SYNCHRONIZE,              // 副本完成同步(server->client)
  MESSAGE_PLAYING_LEAVE_REQUEST,                     // 离开副本请求(client->server)
  MESSAGE_PLAYING_LEAVE_RESPONSE,                    // 离开副本回应(server->client)
  MESSAGE_PLAYING_LEAVE_SYNCHRONIZE,                 // 离开副本同步(server->client)
  MESSAGE_PLAYING_AWARD_REQUEST,                     // 领取奖励请求(client->server)
  MESSAGE_PLAYING_AWARD_RESPONSE,                    // 领取奖励回应(server->client)
  MESSAGE_PLAYING_RECORD_SYNCHRONIZE,                // 副本记录同步(server->cilent)
  MESSAGE_PLAYING_AUTO_REQUEST,                      // 副本自动进行请求(client->server)
  MESSAGE_PLAYING_AUTO_RESPONSE,                     // 副本自动进行回应(server->client)
  MESSAGE_PLAYING_AUTO_STOP_SYNCHRONIZE,             // 同步结束自动进行副本(server->client)
  MESSAGE_PLAYING_AUTO_FINISH_REQUEST,               // 副本自动进行完成请求(client->server)
  MESSAGE_PLAYING_AUTO_FINISH_RESPONSE,              // 副本自动进行完成回应(server->client)
  MESSAGE_PLAYING_PAID_AWARD_REQUEST,                // 副本有偿奖励请求(client->server)
  MESSAGE_PLAYING_PAID_AWARD_RESPONSE,               // 副本有偿奖励回应(server->client)
  MESSAGE_PLAYING_TEAM_LIST_REQUEST,                 // 副本队伍列表请求(client->server)
  MESSAGE_PLAYING_TEAM_LIST_RESPONSE,                // 副本队伍列表回应(server->client)
  MESSAGE_PLAYING_TEAM_CREATE_REQUEST,               // 创建副本队伍请求(client->server)
  MESSAGE_PLAYING_TEAM_CREATE_RESPONSE,              // 创建副本队伍回应(server->client)
  MESSAGE_PLAYING_TEAM_SYNCHRONIZE,                  // 组队信息同步(server->client)
  MESSAGE_PLAYING_TEAM_MEMBER_SYNCHRONIZE,           // 队员信息同步(server->client)
  MESSAGE_PLAYING_TEAM_JOIN_REQUEST,                 // 加入副本队伍请求(client->server)
  MESSAGE_PLAYING_TEAM_JOIN_RESPONSE,                // 加入副本队伍回应(server->client)
  MESSAGE_PLAYING_TEAM_JOIN_SYNCHRONIZE,             // 加入副本队伍同步(server->client)
  MESSAGE_PLAYING_TEAM_QUIT_REQUEST,                 // 退出副本队伍请求(client->server)
  MESSAGE_PLAYING_TEAM_QUIT_RESPONSE,                // 退出副本队伍回应(server->client)
  MESSAGE_PLAYING_TEAM_QUIT_SYNCHRONIZE,             // 退出副本队伍同步(server->client)
  MESSAGE_PLAYING_TEAM_READY_REQUEST,                // 准备副本请求(client->server)
  MESSAGE_PLAYING_TEAM_READY_RESPONSE,               // 准备副本回应(server->client)
  MESSAGE_PLAYING_TEAM_KICKOUT_REQUEST,              // 踢出副本队伍请求(client->server)
  MESSAGE_PLAYING_TEAM_KICKOUT_RESPONSE,             // 踢出副本队伍回应(server->client)
  MESSAGE_PLAYING_TEAM_CAPACITY_SET_REQUEST,         // 设置副本队伍容量设置请求(client->server)
  MESSAGE_PLAYING_TEAM_CAPACITY_SET_RESPONSE,        // 设置副本队伍容量设置回应(server->client)
  MESSAGE_MAZE_INITIALIZE,                           // 地宫数据初始化(server->client)
  MESSAGE_MAZE_SYNCHRONIZE,                          // 地宫数据同步(server->client)
  MESSAGE_MAZE_ENTER_NEXT_FLOOR_NOTIFY,              // 地宫进入下一层通知(server->client)
  MESSAGE_MAZE_MOVE_REQUEST,                         // 地宫移动请求(client->server)
  MESSAGE_MAZE_MOVE_RESPONSE,                        // 地宫移动回复(server->client)
  MESSAGE_MAZE_CONFIRM_MOVE_REQUEST,                 // 地宫确认移动请求(client->server)
  MESSAGE_MAZE_CONFIRM_MOVE_RESPONSE,                // 地宫确认移动回复(server->client)
  MESSAGE_MAZE_AWARD_REQUEST,                        // 地宫领取奖励请求(client->server)
  MESSAGE_MAZE_AWARD_RESPONSE,                       // 地宫领取奖励回复(server->client)
  /////////////////////////////////
  
  ///////////////////////////////////
  // 进度条
  MESSAGE_BAR_INTERRUPT,                             // 采集打断(servser->client)
  MESSAGE_BAR_FINISH_REQUEST,                        // 进度条完成请求(client->server)
  MESSAGE_BAR_FINISH_RESPONSE,                       // 进度条完成响应(server->client)
  MESSAGE_BAR_START_SYNCHRONIZE,                     // 同步客户端安装进度条(server->client)
  ///////////////////////////////////

  ///////////////////////////////////
  // 冷却
  MESSAGE_COOLING_INITIALIZE,                        // 冷却初始化(server->client)
  ///////////////////////////////////

  ///////////////////////////////////
  // 拍卖消息
  MESSAGE_AUCTION_INITIALIZE,                        // 拍卖数据初始化(server->client)
  MESSAGE_AUCTION_SELL_ITEM_SYNCHRONIZE,             // 玩家拍卖道具数据同步(server->client)
  MESSAGE_AUCTION_BUY_ITEM_SYNCHRONIZE,              // 玩家竞拍道具数据同步(server->client)
  MESSAGE_AUCTION_HISTORY_SYNCHRONIZE,               // 拍卖历史数据同步(server->client)
  MESSAGE_AUCTION_QUERY_TRANSACTION_REQUEST,         // 取交易信息请求(client->server)
  MESSAGE_AUCTION_QUERY_TRANSACTION_RESPONSE,        // 取交易信息回复(server->client)
  MESSAGE_AUCTION_SEARCH_REQUEST,                    // 搜索道具请求(client->server)
  MESSAGE_AUCTION_SEARCH_RESPONSE,                   // 搜索道具回复(server->client)
  MESSAGE_AUCTION_SELL_REQUEST,                      // 上架道具请求(client->server)
  MESSAGE_AUCTION_SELL_RESPONSE,                     // 上架道具回复(server->client)
  MESSAGE_AUCTION_CANCEL_SELL_REQUEST,               // 下架道具请求(client->server)
  MESSAGE_AUCTION_CANCEL_SELL_RESPONSE,              // 下架道具回复(server->client)
  MESSAGE_AUCTION_BID_REQUEST,                       // 竞拍道具请求(client->server)
  MESSAGE_AUCTION_BID_RESPONSE,                      // 竞拍道具回复(server->client)
  MESSAGE_AUCTION_BUYOUT_REQUEST,                    // 一口价购买道具请求(client->server)
  MESSAGE_AUCTION_BUYOUT_RESPONSE,                   // 一口价购买道具回复(server->client)
  MESSAGE_AUCTION_WITHDRAW_REQUEST,                  // 提取(道具/交易金额)请求(client->server)
  MESSAGE_AUCTION_WITHDRAW_RESPONSE,                 // 提取(道具/交易金额)回复(server->client)
  ///////////////////////////////////

  ///////////////////////////////////
  // 福利系统
  MESSAGE_WELFARE_INITILIZE,                         // 福利系统数据初始化(server->client)
  MESSAGE_ACHIEVE_INITILIZE,                         // 成就系统数据初始化(server->client)
  MESSAGE_ACHIEVE_CONDITION_SYSNCHRONIZATION,        // 成就条件数据同步(server->client)
  MESSAGE_CHECKINS_SYNCHRONIZATION,                  // 签到数据同步(client->server)
  MESSAGE_WELFARE_TITLE_BROADCAST,                   // 广播称号(gateway->server)
  MESSAGE_ACHIEVE_AWARD_REQUEST,                     // 成就领取奖励请求(client->server)
  MESSAGE_ACHIEVE_AWARD_RESPONSE,                    // 成就领取奖励返回(server->client)
  MESSAGE_ACHIEVE_CHANGE_TITLE_REQUEST,              // 请求称号(server->client)
  MESSAGE_ACHIEVE_CHANGE_TITLE_RESPONSE,             // 称号响应(client->server)
  MESSAGE_CHECKINS_OPT_REQUEST,                      // 签到操作请求(client->server)
  MESSAGE_CHECKINS_OPT_RESPONSE,                     // 签到操作响应(server->client)
  MESSAGE_ONLINE_AWARD_ITEM_REQUEST,                 // 在线奖励道具请求(client->server)
  MESSAGE_ONLINE_AWARD_ITEM_RESPONSE,                // 在线奖励道具响应(server->client)
  MESSAGE_ONLINE_AWARD_RESOURCE_REQUEST,             // 现在奖励资源请求(client->server)
  MESSAGE_ONLINE_AWARD_ITEM_RESOURCE_RESPONSE,       // 在线奖励资源请求响应(server->client)
  ///////////////////////////////////

  ///////////////////////////////////
  // 护送
  MESSAGE_TRANSPORT_AOI_SYNCHRONIZE,                 // 护送AOI数据同步(server->client)
  MESSAGE_TRANSPORT_INITIALIZE,                      // 护送数据初始化(server->client)
  MESSAGE_TRANSPORT_SYNCHRONIZE,                     // 护送数据同步(server->client)
  ///////////////////////////////////

  MESSAGE_MAX,
}

struct Message {
  1 : required MessageType type_;
  2 : required binary message_;
}



// type: MESSAGE_TEST
// dir: gateway->other
struct MessageTest {
  1 : required i32 serialize_;
}



enum MessageSystemType {
  UNKNOWN_ERROR = 0,
  USER_UPPER_LIMITED,

  MAX,
}

// type: MESSAGE_SYSTEM
// dir: gateway->other
struct MessageSystem {
  1 : required MessageSystemType type_;
  2 : optional string details_;
}



// type: MESSAGE_VERIFY_QUESTION
// dir: gateway->other
struct MessageVerifyQuestion {
  1 : required i32 code_;
  2 : required binary key_;
}

// type: MESSAGE_VERIFY_QUESTION_REQUEST
// dir: other->gateway
struct MessageVerifyQuestionRequest {
  1 : required binary value_;
}

// type: MESSAGE_VERIFY_QUESTION_RESPONSE
// dir: gateway->other
struct MessageVerifyQuestionResponse {
  1 : required bool pass_;
}



// type: MESSAGE_LOGIN_REQUEST
// dir: other->gateway
struct MessageLoginRequest {
  1 : required string actor_id_;
  2 : required string session_key_;
}

// type: MESSAGE_LOGIN_RESPONSE
// dir: gateway->other
struct MessageLoginResponse {
  1 : required bool logined_;
}



// type: MESSAGE_LOGOUT_REQUEST
// dir: other->gateway
struct MessageLogoutRequest {
}

// type: MESSAGE_LOGOUT_RESPONSE
// dir: gateway->other
struct MessageLogoutResponse {
  1 : required bool logouted_;
}




// type: MESSAGE_KICKOUT_ACTOR
// dir: gateway->other
struct MessageKickoutActor {
  1 : required kickout_actor_type.KickoutActorType type_;
}

