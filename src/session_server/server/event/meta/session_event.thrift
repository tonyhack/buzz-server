namespace cpp session.server.event

enum EventType {
  EVENT_MIN = 0,

  /////////////////////////
  // 系统相关事件
  EVENT_SYSTEM_SERVER_LOGIN,                          // 服务器登陆
  EVENT_SYSTEM_SERVER_LOGOUT,                         // 服务器登出
  EVENT_SYSTEM_DATABASE_SERVER_LOGINED,               // 已连接到数据库服务器
  /////////////////////////

  /////////////////////////
  // 玩家相关事件
  EVENT_ACTOR_LOGIN,                                  // 玩家登陆Session
  EVENT_ACTOR_LOGOUT,                                 // 玩家登出Session
  EVENT_ACTOR_SAVE,                                   // 玩家数据保存
  EVENT_ACTOR_EXTENSION_LOAD_FINISH,                  // 扩展模块加载完成(每个扩展加载完成后发送的事件)
  EVENT_ACTOR_LOAD_FINISH,                            // 玩家数据加载完成(所有扩展加载完成后发送的事件)
  EVENT_ACTOR_DAILY_CLEAN,                            // 玩家每日清理事件
  EVENT_ACTOR_WEEKLY_CLEAN,                           // 玩家每周清理事件
  EVENT_ACTOR_LEVEL_UP,                               // 玩家升级事件
  EVENT_ACTOR_FUNCTIONALITY_ENABLE,                   // 玩家系统功能开启事件
  EVENT_ACTOR_CHANGE_VOCATION,                        // 玩家切换职业事件
  EVENT_ACTOR_CHANGE_FIGHT_SCORE,                     // 玩家战斗力变化事件
  EVENT_ACTOR_JUMP_LOGIN,                             // 玩家跳场景登陆开始
  EVENT_ACTOR_JUMP_SCENE,                             // 玩家跳场景
  /////////////////////////

  /////////////////////////
  // 社交相关事件
  EVENT_SOCIAL_CONTACTS_INFO_UPDATE,                  // 联系人信息更新事件
  EVENT_SOCIAL_ENEMY_INFO_UPDATE,                     // 仇人信息更新事件
  /////////////////////////

  /////////////////////////
  // 家园相关事件
  EVENT_HOMELAND_FRIEND_INFO_UPDATE,                  // 家园好友信息更新事件
  EVENT_HOMELAND_INVADER_INFO_UPDATE,                 // 家园侵入者信息更新事件
  /////////////////////////

  /////////////////////////
  // 公会相关事件
  EVENT_GUILD_DAILY_CLEAN,                            // 公会每日清理事件
  EVENT_GUILD_WEEKLY_CLEAN,                           // 公会每周清理事件
  /////////////////////////

  /////////////////////////
  // 副本相关事件
  EVENT_PLAYING_END,                                  // 副本结束事件
  EVENT_PLAYING_DESTROY,                              // 副本销毁事件
  /////////////////////////

  EVENT_MAX,
}

// 定义事件通道类型
enum ChannelType {
  CHANNEL_SYSTEM = 0,                                 // 系统类型
  CHANNEL_ACTOR,                                      // 玩家类型
  CHANNEL_PLAYING,                                    // 副本类型

  CHANNEL_MAX,
}

