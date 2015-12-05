namespace cpp game.server.event
namespace py ccevent

enum EventType {
  EVENT_MIN = 0,

  /////////////////////
  // 系统相关事件
  EVENT_SYSTEM_LOGIN_SESSION = 0,               // 登陆Session服务器成功
  /////////////////////

  /////////////////////
  // 玩家相关事件
  EVENT_ACTOR_LOGIN,                            // 玩家上线
  EVENT_ACTOR_LOGOUT,                           // 玩家下线
  EVENT_ACTOR_SAVE,                             // 玩家数据保存
  EVENT_ACTOR_EXTENSION_LOAD_FINISH,            // 扩展模块加载完成(每个扩展加载完成后发送的事件)
  EVENT_ACTOR_LOAD_FINISH,                      // 玩家数据加载完成(所有扩展加载完成后发送的事件)
  EVENT_ACTOR_DAILY_CLEAN,                      // 玩家每日清理事件
  EVENT_ACTOR_WEEKLY_CLEAN,                     // 玩家每周清理事件
  EVENT_ACTOR_TALK_NPC,                         // 与NPC对话
  EVENT_ACTOR_LEVEL_UP,                         // 玩家升级事件
  EVENT_ACTOR_FUNCTIONALITY_ENABLE,             // 玩家系统功能开启事件
  EVENT_ACTOR_KILL_NPC,                         // 玩家杀死NPC事件
  EVENT_ACTOR_COMPARE,                          // 玩家对比事件
  EVENT_ACTOR_VIP_CHANGE,                       // 玩家VIP变化
  EVENT_ACTOR_RESOURCE_CHANGE,                  // 玩家资源变化
  EVENT_ACTOR_RELIVE,                           // 玩家复活事件
  EVENT_ACTOR_FCM_STATUS_CHANGE,                // 玩家防沉迷状态变化
  /////////////////////

  /////////////////////
  // Role相关事件
  EVENT_BASIC_STATUS_CHANGE,                    // 状态改变
  EVENT_ROLE_COMMON_STATUS_CHANGE,              // 常见状态改变
  EVENT_ROLE_KILLED,                            // 角色被杀死事件
  EVENT_ROLE_CHANGE_HP,                         // 生命值改变
  EVENT_ROLE_CHANGE_MAX_HP,                     // 生命上限改变
  EVENT_ROLE_CHANGE_MP,                         // 魔法值改变
  EVENT_ROLE_CHANGE_MAX_MP,                     // 魔法上限改变
  EVENT_ROLE_ENTER_FIGHTING_STATUS,             // 进入战斗状态
  EVENT_ROLE_LEAVE_FIGHTING_STATUS,             // 离开战斗状态
  EVENT_ROLE_ENTER_PVP_STATUS,                  // 进入PVP状态
  EVENT_ROLE_LEAVE_PVP_STATUS,                  // 离开PVP状态
  EVENT_ROLE_CHANGE_VOCATION,                   // 切换职业
  EVENT_ROLE_CHANGE_CURRENT_ENERGY,             // 能量值改变
  EVENT_ROLE_CHANGE_MAX_ENERGY,                 // 能量上限改变
  /////////////////////

  /////////////////////
  // NPC相关事件
  EVENT_NPC_CREATE,                             // NPC创建事件
  EVENT_NPC_DESTROY,                            // NPC销毁事件
  /////////////////////

  /////////////////////
  // 场景相关事件
  EVENT_SCENE_BROADCAST_CREATE_ROLE,            // 广播角色创建到玩家集
  EVENT_SCENE_CREATE_AOI_ROLES,                 // 创建视野内的角色集
  EVENT_SCENE_ROLE_COMING,                      // 角色靠近
  EVENT_SCENE_ROLE_LEAVING,                     // 角色远离
  EVENT_SCENE_ROLE_CHANGE_LOCATION,             // 角色改变位置
  EVENT_SCENE_JUMP_MAP,                         // 角色跳转地图
  EVENT_SCENE_GATHER,                           // 场景采集事件
  /////////////////////
  
  /////////////////////
  // 技能事件
  EVENT_SKILL_USE_SKILL,                        // 使用技能
  EVENT_SKILL_ATTACKED_SKILL,                   // 受到技能攻击
  EVENT_SKILL_USE_APPEAR,                       // 使用幻化技能
  EVENT_SKILL_ATTACK_HIT,                       // 攻击命中
  EVENT_SKILL_ATTACKED_HIT,                     // 受到攻击命中
  /////////////////////

  /////////////////////
  // BUFF事件
  EVENT_BUFF_START,                             // BUFF开启
  EVENT_BUFF_END,                               // BUFF关闭
  /////////////////////

  /////////////////////
  // 副本事件
  EVENT_PLAYING_CREATE,                         // 副本创建
  EVENT_PLAYING_DESTORY,                        // 副本销毁
  EVENT_PLAYING_ACTOR_ENTER,                    // 玩家进入副本
  EVENT_PLAYING_ACTOR_LEAVE,                    // 玩家离开副本
  EVENT_PLAYING_ACTOR_REQUEST_FINISH,           // 玩家请求完成副本
  EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,         // 玩家请求完成副本
  EVENT_PLAYING_REQUEST_UPDATE_RECORD,          // 请求更新记录
  EVENT_PLAYING_REQUEST_AUTO_FINISH,            // 请求完成自动副本
  EVENT_PLAYING_REQUEST_ADD_AWARD,              // 请求增加副本奖励(脚本专用)
  EVENT_PLAYING_ACTOR_FINISH,                   // 玩家完成副本事件
  EVENT_PLAYING_ACTOR_REQUEST_AWARD,            // 玩家请求领取奖励
  /////////////////////

  ////////////////////
  // 道具相关
  EVENT_ITEM_USE,                               // 使用道具
  EVENT_ITEM_EQUIP_STRENGTH,                    // 装备强化
  EVENT_ITEM_EQUIP_REFINE,                      // 装备洗练
  EVENT_ITEM_EQUIP_INLAY,                       // 宝石镶嵌
  EVENT_ITEM_WEAR_EQUIP,                        // 穿装备
  EVENT_ITEM_DIVINE_GEM,                        // 购买宝石(占星)
  ///////////////////

  ///////////////////
  // attr相关
  EVENT_ATTR_SPIRIT_FEED,                       // 精灵培养
  EVENT_ATTR_CONSTELLATION_UP,                  // 提升之力
  EVENT_ATTR_SPIRIT_UPGRADE,                    // 精灵进阶
  EVENT_ATTR_SWORD_UPGRADE,                     // 魔剑进阶
  EVENT_ATTR_NOBLE_UPGRADE,                     // 贵族(爵位)进阶
  ///////////////////

  //////////////////
  // 英灵相关
  EVENT_SOUL_ADD,                               // 增加英灵
  EVENT_SOUL_SET,                               // 英灵设置
  EVENT_SOUL_UPGRADE_STEP,                      // 英灵升阶
  EVENT_SOUL_UPGRADE_LEVEL,                     // 英灵升级
  //////////////////

  /////////////////////
  // AI事件
  EVENT_AI_THREAT_START,                        // 仇恨开始(进入仇恨列表)
  EVENT_AI_THREAT_END,                          // 仇恨结束(从仇恨列表清除)
  /////////////////////

  /////////////////////
  // bar事件
  EVENT_BAR_FINISH,                             // 进度条完成事件
  /////////////////////

  /////////////////////
  // task事件
  EVENT_TASK_FINISH,                            // 任务完成事件
  EVENT_TASK_ACCEPT,                            // 任务接取事件
  /////////////////////

  /////////////////////
  // 社交 事件
  EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE,          // 家园掠夺
  EVENT_SOCIAL_HOMELAND_TREE_WATER,             // 家园浇水
  /////////////////////

  /////////////////////
  // 护送事件
  EVENT_TRANSPORT_FAILURE,                      // 护送失败
  /////////////////////

  EVENT_MAX,
}

// 定义事件通道类型
enum ChannelType {
  CHANNEL_SYSTEM = 0,                           // 系统类型
  CHANNEL_ACTOR,                                // 玩家类型
  CHANNEL_NPC,                                  // NPC类型
  CHANNEL_SCENE,                                // 场景类型
  CHANNEL_PLAYING,                              // 副本类型

  CHANNEL_MAX,
}

