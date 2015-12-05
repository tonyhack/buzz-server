//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-29 10:15:58.
// File name: session_message_handler.cc
//
// Description:
// Define class SessionMessageHandler.
//

#include "game_server/server/extension/scene/session_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/configure.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_server.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_builder.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_pool.h"
#include "game_server/server/request/game_transport_request_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/types.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace scene {

SessionMessageHandler::SessionMessageHandler() {}
SessionMessageHandler::~SessionMessageHandler() {}

bool SessionMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGIN_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessageSceneLoginResponse,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_BATCH_LOGIN_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessageSceneBatchLoginResponse,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGOUT_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessageSceneLogoutResponse,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_NOTICE,
      boost::bind(&SessionMessageHandler::OnMessageSceneJumpNotice,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_SUMMON_WORLD_BOSS,
      boost::bind(&SessionMessageHandler::OnMessageSceneSummonWorldBoss,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_MAP_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessageSceneActorJumpMapResponse,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessageSceneJumpSceneResponse,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_NPC_JUMP_SYNCHRONIZE,
      boost::bind(&SessionMessageHandler::OnMessageSceneNpcJumpSynchronize,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_BRANCH_MAP_SYNCHRONIZE,
      boost::bind(&SessionMessageHandler::OnMessageSceneBranchMapSynchronize,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_SWITCH_BRANCH_NOTICE,
      boost::bind(&SessionMessageHandler::OnMessageSceneActorSwitchBranchNotice,
        this, _1, _2));

  return true;
}

void SessionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGIN_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_BATCH_LOGIN_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGOUT_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_SUMMON_WORLD_BOSS);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_MAP_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_NPC_JUMP_SYNCHRONIZE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_BRANCH_MAP_SYNCHRONIZE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_SWITCH_BRANCH_NOTICE);
}

void SessionMessageHandler::OnMessageSceneLoginResponse(const char *data,
    size_t size) {
  if(data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 消息解析
  session::protocol::MessageSceneLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(response.scene_);
  if(scene == NULL) {
    LOG_ERROR("获取场景对象(%lu)失败", response.scene_);
    return ;
  }

  // 注册结果
  if(response.result_ == true) {
    // 获取场景对象，并设置登陆成功
    scene->SetLogined();
  } else {
    SceneBuilder::GetInstance()->Destory(scene);
  }
}

void SessionMessageHandler::OnMessageSceneBatchLoginResponse(const char *data,
    size_t size) {
  if(data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 消息解析
  session::protocol::MessageSceneBatchLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  size_t pos = 0;
  for(; pos < response.successes_.size(); ++pos) {
    // 获取场景对象，并设置登陆成功
    Scene *scene = SceneManager::GetInstance()->GetScene(response.successes_[pos]);
    if(scene == NULL) {
      LOG_ERROR("获取场景对象(%lu)失败", response.successes_[pos]);
      continue;
    }
    scene->SetLogined();
  }

  pos = 0;
  for(; pos < response.failures_.size(); ++pos) {
    Scene *scene = SceneManager::GetInstance()->GetScene(response.failures_[pos]);
    if(scene == NULL) {
      LOG_ERROR("获取场景对象(%lu)失败", response.failures_[pos]);
      continue;
    }
    SceneBuilder::GetInstance()->Destory(scene);
  }
}

void SessionMessageHandler::OnMessageSceneLogoutResponse(const char *data,
    size_t size) {
  if(data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 消息解析
  session::protocol::MessageSceneLogoutResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  LOG_DEBUG("收到会话消息 MessageSceneLogoutResponse");

  if(response.result_ == false) {
    LOG_ERROR("场景登出 session服务器 失败");
  }
}

void SessionMessageHandler::OnMessageSceneJumpNotice(const char *data, size_t size) {
  if(data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 消息解析
  session::protocol::MessageSceneJumpNotice notice;
  if(global::GlobalPacket::GetPacket()->Deserialize(notice, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(notice.actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", notice.actor_);
    return ;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", actor->GetScene());
    return ;
  }

  if(scene->JumpScene(actor, notice.scene_, notice.x_, notice.y_) == false) {
    LOG_ERROR("JumpScene(%lu,%lu,%u,%u) 失败",
        actor, notice.scene_, notice.x_, notice.y_);
    return ;
  }

  LOG_DEBUG("收到 Session 发来 MessageSceneJumpNotice");
}

void SessionMessageHandler::OnMessageSceneSummonWorldBoss(
    const char *data, size_t size) {
  if(data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 消息解析
  session::protocol::MessageSceneSummonWorldBoss request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(request.id_);
  if(scene == NULL) {
    LOG_ERROR("获取场景对象(%lu)失败", request.id_);
    return ;
  }

  core::uint64 dirction = ExtensionManager::GetInstance()->GetRandom().Random(entity::DirectionType::MAX);
  EntityBuilder &builder = scene->GetEntityBuilder();
  GameNpc* npc = builder.SummonNpc(request.template_id_, request.x_, request.y_, request.width_,
      request.heigth_, (entity::DirectionType::type)dirction);
  if(npc == NULL) {
    LOG_ERROR("创建NPC失败");
    return ;
  }
  scene->SetWorldBossID(npc->GetID());

  // 广播客户端
  gateway::protocol::MessageSceneSummonWorldBoss message;
  message.__set_template_id_(npc->GetTemplateID());
  LOG_DEBUG("召唤 BOSS【%d】", npc->GetTemplateID());
  GameServerSingleton::GetInstance().BroadcastMessage(message,
      gateway::protocol::MessageType::MESSAGE_SCENE_SUMMON_WORLD_BOSS);
}

void SessionMessageHandler::OnMessageSceneActorJumpMapResponse(const char *data, size_t size) {
  if (data == NULL || size <=0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneActorJumpMapResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return;
  }

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(response.id_);
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", response.id_);
    return;
  }

  gateway::protocol::MessageSceneActorJump message;
  message.__set_map_(response.map_);
  message.__set_x_(response.x_);
  message.__set_y_(response.y_);
  game_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_SCENE_ACTOR_JUMP);

  Scene *src_scene = SceneManager::GetInstance()->GetScene(game_actor->GetScene());
  if (src_scene != NULL && src_scene->CheckDynamic() == false) {
    game_actor->SetPreMap(src_scene->GetMapID());
    game_actor->SetPrePosition(game_actor->GetPosx(), game_actor->GetPosy());
  }

  if (response.game_cross_ == true) {
    event::EventSceneJumpMap event;
    event.__set_type_(game_actor->GetType());
    event.__set_id_(game_actor->GetID());
    event.__set_src_scene_(game_actor->GetScene());
    event.__set_dest_scene_(response.scene_);
    event.__set_dest_x_(response.x_);
    event.__set_dest_y_(response.y_);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SCENE_JUMP_MAP, game_actor->GetID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

    GameActorManager::GetInstance()->KickActor(game_actor->GetID());

    session::protocol::MessageSceneActorJumpLogoutRequest request;
    request.__set_id_(response.id_);
    request.__set_scene_(response.scene_);
    request.__set_x_(response.x_);
    request.__set_y_(response.y_);
    SessionChannel::SendMessage(request,
        session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_LOGOUT_REQUEST);
  } else {
    if (src_scene->JumpScene(game_actor, response.scene_, response.x_, response.y_) == false) {
      LOG_ERROR("玩家(%lu) 跳转场景(%lu) 失败", game_actor->GetID(), response.scene_);
      return;
    }
  }
}

void SessionMessageHandler::OnMessageSceneJumpSceneResponse(const char *data, size_t size) {
  if (data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneJumpSceneResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return;
  }

  if (response.type_ == entity::EntityType::TYPE_ACTOR) {
    GameActor *game_actor = GameActorManager::GetInstance()->GetActor(response.id_);
    if (game_actor == NULL) {
      LOG_ERROR("获取 GameActor(%lu) 失败", response.id_);
      return;
    }

    gateway::protocol::MessageSceneActorJump gate_message;
    gate_message.__set_map_(global::GameMisc::GetSceneMapID(response.scene_));
    gate_message.__set_x_(response.x_);
    gate_message.__set_y_(response.y_);
    game_actor->SendMessage(gate_message,
        gateway::protocol::MessageType::MESSAGE_SCENE_ACTOR_JUMP);

    Scene *src_scene = SceneManager::GetInstance()->GetScene(game_actor->GetScene());
    if (src_scene != NULL &&
        src_scene->GetMapID() != global::GameMisc::GetSceneMapID(response.scene_) &&
        src_scene->CheckDynamic() == false) {
      game_actor->SetPreMap(src_scene->GetMapID());
      game_actor->SetPrePosition(game_actor->GetPosx(), game_actor->GetPosy());
    }

    if (SceneManager::GetInstance()->GetScene(response.scene_) == NULL) {
      event::EventSceneJumpMap event;
      event.__set_type_(game_actor->GetType());
      event.__set_id_(game_actor->GetID());
      event.__set_src_scene_(game_actor->GetScene());
      event.__set_dest_scene_(response.scene_);
      event.__set_dest_x_(response.x_);
      event.__set_dest_y_(response.y_);
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_SCENE_JUMP_MAP, game_actor->GetID(),
          event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

      GameActorManager::GetInstance()->KickActor(game_actor->GetID());

      session::protocol::MessageSceneActorJumpLogoutRequest request;
      request.__set_id_(response.id_);
      request.__set_scene_(response.scene_);
      request.__set_x_(response.x_);
      request.__set_y_(response.y_);
      SessionChannel::SendMessage(request,
          session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_LOGOUT_REQUEST);
    }
  }
}

void SessionMessageHandler::OnMessageSceneNpcJumpSynchronize(const char *data, size_t size) {
  if (data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneNpcJumpSynchronize message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(message.scene_);
  if(scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", message.scene_);
    return;
  }

  const NpcCell *cell = game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
      message.npc_snapshot_.template_id_);
  if(cell == NULL) {
    LOG_ERROR("配置中找不到 npc(%u) ", message.npc_snapshot_.template_id_);
    return;
  }

  // TODO: 如果是护送NPC，则根据快照发送request给护送模块
  if (cell->type_ == entity::NpcKind::TRANSPORT) {
    request::RequestTransportSummonFromSnapshot request;
    request.__set_transport_npc_template_(message.npc_snapshot_.template_id_);
    request.__set_scene_(message.scene_);
    request.__set_x_(message.x_);
    request.__set_y_(message.y_);
    request.__set_snapshot_(message.npc_snapshot_.transport_);
    if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_TRANSPORT_SUMMON_FROM_SNAPSHOT,
          &request, sizeof(request)) == -1) {
      LOG_ERROR("请求 REQUEST_TRANSPORT_SUMMON_FROM_SNAPSHOT 失败");
      return;
    }
  } else {
    // 召唤NPC
    GameNpc *game_npc = scene->GetEntityBuilder().SummonNpc(
        message.npc_snapshot_.template_id_, message.x_, message.y_,
        3, 3, entity::DirectionType::NONE);
    if (game_npc == NULL) {
      LOG_ERROR("召唤 NPC(%u) 失败", message.npc_snapshot_.template_id_);
      return;
    }
  }
}

void SessionMessageHandler::OnMessageSceneBranchMapSynchronize(const char *data, size_t size) {
  if (data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneBranchMapSynchronize message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return;
  }

  Scene *scene = SceneBuilder::GetInstance()->CreateStatic(message.map_, message.branch_);
  if (scene == NULL) {
    LOG_ERROR("创建 Scene(%d, %d) 失败", message.map_, message.branch_);
    return;
  }
}

void SessionMessageHandler::OnMessageSceneActorSwitchBranchNotice(const char *data, size_t size) {
  if (data == NULL || size <= 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneActorSwitchBranchNotice message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(message.actor_);
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", message.actor_);
    return;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(game_actor->GetScene());
  if (scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", game_actor->GetScene());
    return;
  }

  scene->JumpScene(game_actor, message.scene_, game_actor->GetPosx(), game_actor->GetPosy());
}

}  // namespace scene

}  // namespace server

}  // namespace game

