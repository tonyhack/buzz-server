namespace cpp game.server.request
namespace py ccrequest

enum RequestType {
  REQUEST_MIN = 0,

  ////////////////////
  // 冷却相关
  REQUEST_CHECK_IN_COOLING,                   // 是否在冷却中
  REQUEST_START_COOLING,                      // 开启冷却
  ////////////////////

  ////////////////////
  // 场景相关
  REQUEST_SCENE_GET_AROUND_ACTORS,            // 得到周围的玩家
  REQUEST_SCENE_SUMMON_NPC,                   // 请求召唤NPC
  REQUEST_SCENE_CLEAR_NPC,                    // 请求清除NPC
  REQUEST_SCENE_SELECT_TARGETS,               // 选择场景上的目标集
  REQUEST_SCENE_ROLE_MOVE,                    // 角色移动
  REQUEST_SCENE_JUMP,                         // 场景跳转
  REQUEST_SCENE_RELOCATION,                   // 重置坐标
  REQUEST_SCENE_DAYNIGHT_INFO,                // 获得昼夜信息
  REQUEST_SCENE_ROUTER,                       // 寻路
  REQUEST_SCENE_PK_INFO,                      // 获得场景PK信息
  REQUEST_SCENE_IS_SAFE,                      // 是否安全区
  REQUEST_SCENE_PK_CHECK,                     // 检测PK条件
  REQUEST_SCENE_ACTOR_BROADCAST,              // 请求场景玩家广播
  REQUEST_SCENE_CREATE_MAP,                   // 请求创建场景地图
  REQUEST_SCENE_DESTORY_MAP,                  // 请求销毁场景地图
  REQUEST_SCENE_CLEAR_ALL_NPC,                // 清除场景中所有的NPC
  REQUEST_SCENE_REVIVE,                       // 请求复活
  REQUEST_SCENE_FORBID_DROP_ITEM,             // 设置禁止掉落
  REQUEST_SCENE_GET_NPCS,                     // 获取场景所有NPC
  ////////////////////

  ////////////////////
  // 效果相关
  REQUEST_EFFECT_START,                       // 开启一个效果
  REQUEST_EFFECT_END,                         // 关闭一个效果
  REQUEST_EFFECT_COMMON_STATUS_START,         // 开启一个效果
  REQUEST_EFFECT_COMMON_STATUS_END,           // 关闭一个效果
  ////////////////////

  ////////////////////
  // BUF 相关
  REQUEST_BUFF_START,                         // 开启一个BUF
  REQUEST_BUFF_END,                           // 关闭一个BUF
  REQUEST_BUFF_CLEAR,                         // 清除BUFF
  REQUEST_BUFF_CLEAR_GROUP,                   // 清除BUFF组
  REQUEST_BUFF_SET_FORBID_GROUP,              // 设置禁止BUFF组
  REQUEST_BUFF_RESET_FORBID_GROUP,            // 重置禁止BUFF组
  REQUEST_BUFF_GET_EXIST,                     // 请求获取BUFF是否存在
  ////////////////////

  ////////////////////
  // 道具相关
  REQUEST_ADD_ITEM,                           // 增加道具
  REQUEST_ADD_ITEMS,                          // 批量增加道具
  REQUEST_GET_PAKCET_ITEM_NUMBER,             // 得到背包道具数量
  REQUEST_REMOVE_ITEM,                        // 删除道具
  REQUEST_REMOVE_ITEM_LOCATION_ID,            // 删除道具
  REQUEST_GET_PAKCET_ITEM_TEMPLATE,           // 得到道具模板
  REQUEST_ITEM_WEARED_EQUIP_INFO,             // 获取装备栏信息
  ////////////////////

  ////////////////////
  // 技能相关
  REQUEST_USE_SKILL,                          // 使用技能
  REQUEST_ADD_SKILL,                          // 增加技能(如果技能已存在，把该技能升级到N级)
  REQUEST_REMOVE_SKILL,                       // 移除技能
  REQUEST_UPGRADE_SKILL,                      // 技能升级(技能升N级使用这个接口)
  REQUEST_SKILL_STAND,                        // 技能启用
  REQUEST_SKILL_SIT,                          // 技能收回
  REQUEST_ADD_SKILL_GLOBAL_BUFF,              // 增加技能全局BUFF
  REQUEST_REMOVE_SKILL_GLOBAL_BUFF,           // 删除技能全局BUFF
  REQUEST_SKILL_HURT,                         // 请求技能伤害
  REQUEST_SKILL_ADD_TEMP_SKILL,               // 增加临时技能
  REQUEST_SKILL_REMOVE_TEMP_SKILL,            // 删除临时技能
  REQUEST_SKILL_CHANGE_SKILL_FORM,            // 改变技能形态(普通/临时)
  REQUEST_SKILL_INCREASE_ATTRIBUTE,           // 改变技能属性
  REQUEST_SKILL_SET_FIGHTING_STATUS,          // 设置战斗状态
  ////////////////////

  ////////////////////
  // 英灵相关
  REQUEST_CHECK_SOUL_EXIST,                   // 英灵是否存在
  REQUEST_ADD_SOUL,                           // 增加英灵
  REQUEST_SOUL_GET,                           // 获取玩家英灵基本信息
  REQUEST_SOUL_GET_ENERGY_STATUS,             // 获取玩家能量状态
  ////////////////////

  ////////////////////
  // 属性相关
  REQUEST_ATTR_CHECK_SWORD_OPEN,              // 检测是否开启魔剑
  REQUEST_ATTR_OPEN_SWORD,                    // 开启魔剑
  REQUEST_ATTR_GET_SWORD_STEP,                // 获取魔剑阶级
  REQUEST_ATTR_ADD_SPIRIT_FACADE,             // 增加精灵外观
  REQUEST_ATTR_GET_SPIRIT_LEVEL,              // 获取精灵等级
  REQUEST_ATTR_GET_NOBLE_LEVEL,               // 获取贵族(爵位)等级
  REQUEST_ATTR_GET_SPIRIT_FACADE_TYPE,        // 获取精灵外观类型
  ////////////////////
  
  /////////////////////
  // 邮件相关
  REQUEST_MAIL_SEND,                          // 邮件发送
  /////////////////////

  /////////////////////
  // 副本相关
  REQUEST_PLAYING_INITIALIZE_ACTOR,           // 请求初始化玩家数据
  REQUEST_PLAYING_SYNCHRONIZE_SCORE,          // 请求副本得分
  REQUEST_PLAYING_SYNCHRONIZE_RANKING,        // 请求同步排行榜
  REQUEST_PLAYING_COMPLETE,                   // 请求副本完成
  REQUEST_PLAYING_ACTOR_COMPLETE,             // 请求某玩家副本完成
  REQUEST_PLAYING_FAILURE,                    // 请求副本失败
  REQUEST_PLAYING_UPDATE_RECORD,              // 请求更新副本记录
  REQUEST_PLAYING_UPDATE_RESULT,              // 请求更新副本结果
  REQUEST_PLAYING_ADD_AWARD,                  // 请求增加副本奖励
  REQUEST_PLAYING_CLEAR_COMPLETE,             // 请求清空副本完成次数
  REQUEST_PLAYING_INCREASE_COMPLETE,          // 请求增加副本完成次数
  REQUEST_PLAYING_ACTOR_INCREASE_COMPLETE,    // 请求增加单个玩家副本完成次数
  REQUEST_PLAYING_AWARD_ACTOR,                // 请求奖励副本玩家
  REQUEST_PLAYING_ACTOR_PLAYING_ID,           // 请求玩家所在的副本ID
  /////////////////////

  /////////////////////
  // 任务相关
  REQUEST_TASK_ACCEPT,                        // 请求发送任务
  REQUEST_TASK_CHECK_GATHER,                  // 请求任务中有没有该采集NPC
  REQUEST_TASK_MAX_LAYER,                     // 请求最大的纹章ID
  REQUEST_TASK_CONDITION_COORDINATE,          // 获取任务条件坐标
  REQUEST_TASK_ACCEPT_AND_FINISH_COORDINATE,  // 获取任务接取完成坐标
  /////////////////////

  /////////////////////
  // AI相关
  REQUEST_AI_ADD_THREAT,                      // 请求增加仇恨
  REQUEST_AI_SET_TARGET,                      // 设置AI目标
  REQUEST_AI_SET_AI,                          // 设置AI
  /////////////////////

  /////////////////////
  // 公会相关
  REQUEST_GUILD_GET_ACTOR_GUILD,              // 请求获取玩家所在公会
  REQUEST_GUILD_GET_ACTOR_GUILD_NAME,         // 请求获取玩家所在公会名
  /////////////////////

  /////////////////////
  // 组队相关
  REQUEST_TEAM_GET_ACTOR_TEAM,                // 请求获取玩家所在队伍
  /////////////////////
  
  ////////////////////
  // 进度条
  REQUEST_BAR_START,                          // 开启进度条
  REQUEST_BAR_CHECK,                          // 检查进度条
  ////////////////////

  /////////////////////
  // 福利相关
  REQUEST_WELFARE_GET_TITLE_ID,               // 获取玩家称号ID
  /////////////////////

  /////////////////////
  // 护送相关
  REQUEST_TRANSPORT_SUMMON,                   // 请求召唤护送
  REQUEST_TRANSPORT_REMOVE,                   // 请求清除护送
  REQUEST_TRANSPORT_CURRENT_NPC,              // 请求当前护送NPC信息
  REQUEST_TRANSPORT_AROUND,                   // 请求检测护送NPC是否在周围
  REQUEST_TRANSPORT_GET_SNAPSHOT,             // 请求获取护送快照
  REQUEST_TRANSPORT_SUMMON_FROM_SNAPSHOT,     // 请求根据快照召唤护送
  /////////////////////

  REQUEST_MAX,
}

