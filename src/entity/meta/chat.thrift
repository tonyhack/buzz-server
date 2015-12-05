namespace cpp entity
namespace py ccentity.chat
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 聊天频道类型定义
enum ChatChannelType {
  MIN = 0,

  WORLD = 0,                          // 世界
  SCENE,                              // 场景
  GUILD,                              // 公会
  TEAM,                               // 组队
  WHISPER,                            // 私聊
  WORLD_VIP,                          // 喇叭
  SYSTEM,                             // 系统公告
  
  MAX,
}
