//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 16:35:54.
// File name: game_message_handler.cc
//
// Description:
// Define class GameMessageHandler.
//

#include "session_server/server/extension/scene/game_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/scene/scene.h"
#include "session_server/server/extension/scene/scene_manager.h"
#include "session_server/server/extension/scene/scene_pool.h"

namespace session {

namespace server {

namespace scene {

GameMessageHandler::GameMessageHandler() {}
GameMessageHandler::~GameMessageHandler() {}

bool GameMessageHandler::Initialize() {
  // 场景登陆消息
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGIN_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageSceneLogin, this, _1, _2, _3));

  // 场景批量登陆消息
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_BATCH_LOGIN_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageSceneBatchLogin, this, _1, _2, _3));

  // 场景登出消息
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGOUT_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageSceneLogoutRequest, this, _1, _2, _3));

  // 场景世界BOSS死亡消息
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_WORLD_BOSS_KILLED_NOTICE,
      boost::bind(&GameMessageHandler::OnMessageSceneWorldBossKilled, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageSceneJumpSceneRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_MAP_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageSceneActorJumpMapRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_LOGIN_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageSceneActorJumpLoginRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_LOGOUT_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageSceneActorJumpLogoutRequest, this, _1, _2, _3));

  return true;
}

void GameMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGIN_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_BATCH_LOGIN_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGOUT_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_WORLD_BOSS_KILLED_NOTICE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_MAP_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_LOGIN_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_LOGOUT_REQUEST);
}

void GameMessageHandler::OnMessageSceneLogin(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessageSceneLoginRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  session::protocol::MessageSceneLoginResponse response;
  response.__set_scene_(message.scene_);

  // 分配 Scene 对象
  Scene *scene = ScenePool::GetInstance()->Allocate();
  if(scene == NULL) {
    LOG_ERROR("分配 Scene 对象失败");
    response.__set_result_(false);
  } else {
    // 初始化 Scene
    if(scene->Initialize(message.scene_, message.dynamic_) == false) {
      ScenePool::GetInstance()->Deallocate(scene);
      response.__set_result_(false);
      LOG_ERROR("初始化 Scene 对象失败");
      return ;
    }

    // 加入到场景管理器中
    if(SceneManager::GetInstance()->AddScene(scene) == false) {
      scene->Finalize();
      ScenePool::GetInstance()->Deallocate(scene);
      response.__set_result_(false);
      LOG_ERROR("场景(%lu) 加入管理器失败", message.scene_);
    } else {
      response.__set_result_(true);
      scene->SetGameServer(terminal->GetSignature().id_);
    }
  }

  // 场景登陆回应
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGIN_RESPONSE);
}

void GameMessageHandler::OnMessageSceneBatchLogin(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessageSceneBatchLoginRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  session::protocol::MessageSceneBatchLoginResponse response;

  size_t pos = 0;
  for(; pos < message.scenes_.size(); ++pos) {
    // 分配 Scene 对象
    Scene *scene = ScenePool::GetInstance()->Allocate();
    if(scene == NULL) {
      response.failures_.push_back(message.scenes_[pos].scene_id_);
      LOG_ERROR("分配 Scene 对象失败");
      continue;
    }
    // 初始化 Scene
    if(scene->Initialize(message.scenes_[pos].scene_id_,
          message.scenes_[pos].dynamic_) == false) {
      ScenePool::GetInstance()->Deallocate(scene);
      response.failures_.push_back(message.scenes_[pos].scene_id_);
      LOG_ERROR("初始化 Scene 对象失败");
      continue ;
    }
    // 加入到场景管理器中
    if(SceneManager::GetInstance()->AddScene(scene) == false) {
      scene->Finalize();
      ScenePool::GetInstance()->Deallocate(scene);
      response.failures_.push_back(message.scenes_[pos].scene_id_);
      LOG_ERROR("场景(%lu) 加入管理器失败", message.scenes_[pos].scene_id_);
      continue;
    }
    scene->SetGameServer(terminal->GetSignature().id_);
    response.successes_.push_back(message.scenes_[pos].scene_id_);
  }

  // 场景登陆回应
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_SCENE_BATCH_LOGIN_RESPONSE);
}

void GameMessageHandler::OnMessageSceneLogoutRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessageSceneLogoutRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  Scene *scene = SceneManager::GetInstance()->RemoveScene(request.scene_);
  if(scene == NULL) {
    LOG_ERROR("移除 Scene(%lu)对象 失败", request.scene_);
    return ;
  }

  scene->Finalize();
  ScenePool::GetInstance()->Deallocate(scene);

  session::protocol::MessageSceneLogoutResponse response;
  response.__set_result_(true);
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_SCENE_LOGOUT_RESPONSE);
}

void GameMessageHandler::OnMessageSceneWorldBossKilled(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessageSceneWorldBossKilledNotice message;
  if(global::GlobalPacket::GetPacket()->Deserialize(
        message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  // 得到场景对象
  Scene *scene = SceneManager::GetInstance()->GetScene(message.id_);
  if(scene == NULL) {
    LOG_ERROR("场景[%lu]对象没有找到", message.id_);
    return ;
  }
  scene->GetWorldBoss().OnWorldBossDie(message.killer_);
}

void GameMessageHandler::OnMessageSceneJumpSceneRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneJumpSceneRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(
      message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  Scene *jump_scene = SceneManager::GetInstance()->GetScene(message.scene_);
  if (jump_scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", message.scene_);
    return;
  }

  if (message.type_ == entity::EntityType::TYPE_NPC) {
    SessionTerminal *game_terminal = SessionServerSingleton::GetInstance().GetTerminal(
        global::protocol::ServerType::GAME_SERVER, jump_scene->GetGameServer());
    if (game_terminal == NULL) {
      LOG_ERROR("获取 SessionTerminal(%d) 失败", jump_scene->GetGameServer());
      return;
    }
    session::protocol::MessageSceneNpcJumpSynchronize synchronize;
    synchronize.__set_scene_(message.scene_);
    synchronize.__set_x_(message.x_);
    synchronize.__set_y_(message.y_);
    synchronize.__set_npc_snapshot_(message.npc_snapshot_);
    game_terminal->SendGameMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_SCENE_NPC_JUMP_SYNCHRONIZE);
  } else if (message.type_ == entity::EntityType::TYPE_ACTOR) {
    SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(message.id_);
    if (session_actor == NULL) {
      LOG_ERROR("获取 SessionActor(%lu) 失败", message.id_);
      return;
    }

    Scene *current_scene = SceneManager::GetInstance()->GetScene(session_actor->GetScene());
    if (current_scene == NULL) {
      LOG_ERROR("场景 Scene(%lu) 对象不存在", session_actor->GetScene());
      return;
    }

    current_scene->RemoveActor(session_actor->GetActorID());

    session_actor->SetScene(message.scene_);

    BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(
        session_actor->GetActorID());
    if (brief_actor != NULL) {
      brief_actor->SetMap(global::GameMisc::GetSceneMapID(message.scene_));
    }

    if (jump_scene->AddActor(session_actor->GetActorID()) == false) {
      LOG_ERROR("Scene(%lu)::AddActor(%lu) 失败", message.scene_, message.id_);
      return;
    }

    if (jump_scene->GetGameServer() != current_scene->GetGameServer()) {
      session::protocol::MessageSceneJumpSceneResponse response;
      response.__set_type_(message.type_);
      response.__set_id_(message.id_);
      response.__set_scene_(message.scene_);
      response.__set_x_(message.x_);
      response.__set_y_(message.y_);
      terminal->SendGameMessage(response,
          session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_RESPONSE);
    } else {
      event::EventActorJumpScene event;
      event.__set_actor_(session_actor->GetActorID());
      event.__set_cross_game_server_(false);
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_ACTOR_JUMP_SCENE, session_actor->GetActorID(),
          event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
    }
  }
}

void GameMessageHandler::OnMessageSceneActorJumpMapRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneActorJumpMapRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(
        message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(message.id_);
  if (session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", message.id_);
    return;
  }

  Scene *current_scene = SceneManager::GetInstance()->GetScene(session_actor->GetScene());
  if (current_scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", session_actor->GetScene());
    return;
  }

  Scene *jump_scene = SceneManager::GetInstance()->AllocateBranchScene(message.map_);
  if (jump_scene == NULL) {
    LOG_ERROR("Allocate branch scene(map=%d) failed.", message.map_);
    return;
  }

  current_scene->RemoveActor(session_actor->GetActorID());

  session_actor->SetScene(jump_scene->GetID());

  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(
      session_actor->GetActorID());
  if (brief_actor != NULL) {
    brief_actor->SetMap(message.map_);
  }

  if (jump_scene->AddActor(session_actor->GetActorID()) == false) {
    LOG_ERROR("场景增加 玩家(%lu) 失败", message.id_);
    return;
  }

  session::protocol::MessageSceneActorJumpMapResponse response;
  response.__set_id_(message.id_);
  response.__set_map_(message.map_);
  response.__set_scene_(jump_scene->GetID());
  response.__set_x_(message.x_);
  response.__set_y_(message.y_);
  if (current_scene->GetGameServer() != jump_scene->GetGameServer()) {
    response.__set_game_cross_(true);
  } else  {
    response.__set_game_cross_(false);
  }
  terminal->SendGameMessage(response,
      protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_MAP_RESPONSE);
}

void GameMessageHandler::OnMessageSceneActorJumpLoginRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneActorJumpLoginRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  event::EventActorJumpLogin event;
  event.__set_actor_(message.id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_JUMP_LOGIN, message.id_,
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void GameMessageHandler::OnMessageSceneActorJumpLogoutRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageSceneActorJumpLogoutRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(
      message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  Scene *jump_scene = SceneManager::GetInstance()->GetScene(message.scene_);
  if (jump_scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", message.scene_);
    return;
  }
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(message.id_);
  if (session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", message.id_);
    return;
  }

  core::uint32 game_server = jump_scene->GetGameServer();

  SessionTerminal *gateway_terminal = SessionServerSingleton::GetInstance().GetTerminal(
      global::protocol::ServerType::GATEWAY_SERVER, session_actor->GetGateway());
  if (gateway_terminal == NULL) {
    session_actor->Kickout();
    LOG_ERROR("获取 GatewayServer(%lu) 失败", session_actor->GetGateway());
    return;
  }
  session::protocol::GateMessageUserSwitchGameRequest gateway_switch_game;
  gateway_switch_game.__set_actor_(message.id_);
  gateway_switch_game.__set_game_server_(game_server);
  gateway_terminal->SendGatewayMessage(gateway_switch_game,
      session::protocol::GatewayMessageType::MESSAGE_USER_SWITCH_GAME_REQUEST);

  SessionTerminal *game_terminal = SessionServerSingleton::GetInstance().GetTerminal(
      global::protocol::ServerType::GAME_SERVER, game_server);
  if (game_terminal == NULL) {
    session_actor->Kickout();
    LOG_ERROR("获取 GameServer(%d) 失败", game_server);
    return;
  }

  session_actor->SetGameServer(game_server);

  protocol::GameMessageActorLoginRequest game_request;
  game_request.__set_actor_id_(session_actor->GetActorID());
  game_request.__set_gateway_(session_actor->GetGateway());
  game_request.__set_jump_scene_(true);
  game_request.__set_scene_(message.scene_);
  game_request.__set_x_(message.x_);
  game_request.__set_y_(message.y_);
  game_terminal->SendGameMessage(game_request,
      protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_REQUEST);
}

}  // namespace scene

}  // namespace server

}  // namespace session

