//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 14:18:56.
// File name: npc_create_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/buff/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace buff {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  // Follow 所有 npc 创建完成事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ExtensionLoadEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ExtensionLoadEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ExtensionLoadEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorExtensionLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  // 场景扩展设置为加载成功
  //npc->GetLoadProgress().LoadExtensionFinish(ExtensionType::BUFF);
  if (event->extension_type_ == ExtensionType::EFFECT) {
    global::LogDebug("收到EFFECT模块完成消息，即将启动buff模块");


    // buff玩家对象是否已存在管理器中
    BuffRole *role = BuffRoleManager::GetInstance()->GetActor(event->id_);
    if(role) {
      global::LogError("%s:%d (%s) 玩家(%lu) buff扩展已存在 BuffRoleManager",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }
    // 数据库请求
    database::protocol::StorageBuffLoginRequest request;
    request.__set_id_(event->id_);
    size_t request_size = 0;
    const char *req = global::GlobalPacket::GetPacket()->Serialize(request, request_size);
    if(req == NULL) {
      global::LogError("%s:%d (%s) 消息序列化失败", __FILE__, __LINE__, __FUNCTION__);
      return ;
    }

    // 请求
    if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          database::protocol::StorageType::STORAGE_BUFF_LOGIN,
          event->id_, req, request_size) == false) {
      global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
  }
}

}  // namespace buff

}  // namespace server

}  // namespace game

