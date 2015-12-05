namespace cpp session.server.request

enum RequestType {
  REQUEST_MIN = 0,

  ////////////////////////////////
  // 场景相关
  REQUEST_GET_MAP_SCENE_ID,                           // 请求获取静态地图的场景ID
  REQUEST_SCENE_GET_ACTORS,                           // 请求获取某地图上的玩家集合
  REQUEST_SCENE_GET_GAME_SERVER,                      // 请求获取场景所在GameServer
  REQUEST_SCENE_JUMP,                                 // 请求跳场景地图
  ////////////////////////////////

  ////////////////////////////////
  // 社交相关
  REQUEST_SOCIAL_CHECK_IN_BLACKLIST,                  // 检查是否在黑名单中
  ////////////////////////////////
  
  ////////////////////////////////
  // 邮件相关
  REQUEST_MAIL_SEND_MAIL,                             // 发送邮件
  ////////////////////////////////

  ////////////////////////////////
  // 组队相关
  REQUEST_TEAM_GET_MEMBERS,                           // 获取组队队友列表
  REQUEST_TEAM_ACTOR_QUIT,                            // 请求离开队列
  REQUEST_TEAM_CREATE,                                // 请求创建一个队伍
  ////////////////////////////////

  ////////////////////////////////
  // 公会相关
  REQUEST_GUILD_GET_ID                                // 获取玩家公会ID
  REQUEST_GUILD_ADD_CONTRIBUTION                      // 增加公会贡献
  REQUEST_GUILD_CHECK_AUTHORITY_CHAT,                 // 检查是否有公会聊天权限
  REQUEST_GUILD_ADD_ATTRIBUTE,                        // 增加公会属性
  REQUEST_GUILD_GET_MEMBERS,                          // 获取公会成员列表
  REQUEST_GUILD_CHECK_CAN_CREATE_PLAYING,             // 检查是否可以创建公会副本
  REQUEST_GUILD_GET_CURRENT_PLAYING,                  // 获取当前正在进行中的公会副本信息
  REQUEST_GUILD_SET_CURRENT_PLAYING,                  // 设置当前正在进行中的公会副本
  ////////////////////////////////

  ////////////////////////////////
  // 副本相关
  REQUEST_PLAYING_ACTOR_CONNECT_INFO,                 // 获取玩家关联的副本信息
  ////////////////////////////////

  REQUEST_MAX,
} 
