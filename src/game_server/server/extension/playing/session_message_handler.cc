//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:31:42.
// File name: session_message_handler.cc
//
// Description:
// Define SessionMessageHandler.
//

#include "game_server/server/extension/playing/session_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/playing/playing_page.h"
#include "game_server/server/extension/playing/playing_page_pool.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension/playing/facade_request.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_destory_checker.h"
#include "game_server/server/extension/playing/playing_builder.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "game_server/server/extension/playing/playing_pool.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "global/configure/playing_configure.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace playing {

SessionMessageHandler::SessionMessageHandler() {}
SessionMessageHandler::~SessionMessageHandler() {}

bool SessionMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessagePlayingCreateResponse, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_SYNCHRONIZE,
      boost::bind(&SessionMessageHandler::OnMessagePlayingCreateSynchronize, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessagePlayingCreateFinishResponse, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessagePlayingDestoryResponse, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_END_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessagePlayingEndResponse, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE,
      boost::bind(&SessionMessageHandler::OnMessagePlayingTeamSynchronize, this, _1, _2));

  return true;
}

void SessionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_SYNCHRONIZE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_END_RESPONSE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);
}

static void SendSessionPlayingDestory(core::uint64 playing) {
  session::protocol::MessagePlayingDestoryRequest destory_playing;
  destory_playing.__set_playing_(playing);
  SessionChannel::SendMessage(destory_playing,
      session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_REQUEST);
}

void SessionMessageHandler::OnMessagePlayingCreateResponse(const char *data,
    size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingCreateResponse message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(message.actor_);
  if(playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", message.actor_);
    return ;
  }

  if(message.result_ == true) {
    if(playing_actor->CheckCreatePlaying(message.template_) == false) {
      SendSessionPlayingDestory(message.playing_);
      LOG_ERROR("CheckCreatePlaying(%u) 失败", message.template_);
    } else {
      PlayingPage *page = playing_actor->GetPage(message.template_);
      if (page == NULL) {
        page = PlayingPagePool::GetInstance()->Allocate();
        if (page == NULL) {
          SendSessionPlayingDestory(message.playing_);
          LOG_ERROR("分配 PlayingPage(%u) 失败", message.template_);
          return;
        }
        if(page->Initialize(playing_actor, message.template_) == false) {
          SendSessionPlayingDestory(message.playing_);
          PlayingPagePool::GetInstance()->Deallocate(page);
          LOG_ERROR("初始化 PlayingPage(%d) 失败", message.template_);
          return;
        }
        if (playing_actor->AddPage(page) == false) {
          SendSessionPlayingDestory(message.playing_);
          page->Finalize();
          PlayingPagePool::GetInstance()->Deallocate(page);
          LOG_ERROR("增加 PlayingPage(%u) 失败", message.template_);
          return;
        }
      }
    }
  }

  gateway::protocol::MessagePlayingCreateResponse response;
  response.__set_template_(message.template_);
  response.__set_result_(message.result_);
  playing_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_CREATE_RESPONSE);
}

void SessionMessageHandler::OnMessagePlayingCreateSynchronize(const char *data,
    size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingCreateSynchronize synchronize;
  if(global::GlobalPacket::GetPacket()->Deserialize(synchronize, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  Playing *playing = PlayingBuilder::GetInstance()->Create(synchronize.template_,
      synchronize.playing_);
  if(playing == NULL) {
    LOG_ERROR("创建 Playing(%u) 失败", synchronize.template_);
    SendMessagePlayingCreateFinishFailed(synchronize.playing_);
    return ;
  }

  session::protocol::MessagePlayingCreateFinishRequest request;
  request.__set_playing_(synchronize.playing_);
  request.__set_scene_(playing->GetSceneID());
  SessionChannel::SendMessage(request,
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_REQUEST);
}

void SessionMessageHandler::OnMessagePlayingCreateFinishResponse(const char *data,
    size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingCreateFinishResponse message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  // 获取 Playing 对象
  Playing *playing = PlayingManager::GetInstance()->Get(message.playing_);
  if(playing == NULL) {
    LOG_ERROR("获取 Playing 失败");
    return ;
  }

  if(message.result_ == false) {
    LOG_ERROR("创建副本(%u)完成失败", playing->GetTemplateID());
    PlayingBuilder::GetInstance()->Destory(playing);
    return ;
  }
}

void SessionMessageHandler::OnMessagePlayingDestoryResponse(const char *data,
    size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingDestoryResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  if(response.result_ == false) {
    LOG_ERROR("销毁副本收到结果为失败");
    return ;
  }

  // 获取 Playing 对象
  Playing *playing = PlayingManager::GetInstance()->Get(response.playing_);
  if(playing == NULL) {
    LOG_ERROR("获取 Playing 失败");
    return ;
  }

  PlayingBuilder::GetInstance()->Destory(playing);
}

void SessionMessageHandler::OnMessagePlayingEndResponse(const char *data, size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingEndResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  if(response.result_ == false) {
    LOG_ERROR("结束副本收到结果为失败");
    return ;
  }

  // 获取 Playing 对象
  Playing *playing = PlayingManager::GetInstance()->Get(response.playing_);
  if(playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", response.playing_);
    return ;
  }

  playing->SetStatus(StatusType::END);

  PlayingDestoryChecker::GetInstance()->Add(response.playing_);

}

void SessionMessageHandler::OnMessagePlayingTeamSynchronize(const char *data, size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingTeamSynchronize response;
  if(global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(response.actor_);
  if (playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", response.actor_);
    return;
  }

  playing_actor->SetPlayingTeam(response.playing_team_);

}

void SessionMessageHandler::SendMessagePlayingCreateFinishFailed(core::uint64 id,
    core::int8 failure) {
  session::protocol::MessagePlayingCreateFinishRequest message;
  message.__set_playing_(id);
  message.__set_failure_(failure);
  SessionChannel::SendMessage(message,
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_REQUEST);
}

}  // namespace playing

}  // namespace server

}  // namespace game

