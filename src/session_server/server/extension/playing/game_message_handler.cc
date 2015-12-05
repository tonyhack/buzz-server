//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-17 19:10:47.
// File name: game_message_handler.cc
//
// Description:
// Define class GameMessageHandler.
//

#include "session_server/server/extension/playing/game_message_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/inform_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_activity_controller.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_load_balance.h"
#include "session_server/server/extension/playing/playing_manager.h"
#include "session_server/server/extension/playing/playing_page.h"
#include "session_server/server/extension/playing/playing_page_pool.h"
#include "session_server/server/extension/playing/playing_pool.h"
#include "session_server/server/extension/playing/playing_team.h"
#include "session_server/server/extension/playing/playing_team_manager.h"
#include "session_server/server/extension/playing/playing_team_pool.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_playing_event_types.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"
#include "session_server/server/request/session_team_request_types.h"

namespace session {

namespace server {

namespace playing {

GameMessageHandler::GameMessageHandler() {}
GameMessageHandler::~GameMessageHandler() {}

bool GameMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_REQUEST,
      boost::bind(&GameMessageHandler::OnMessagePlayingCreateRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_REQUEST,
      boost::bind(&GameMessageHandler::OnMessagePlayingCreateFinishRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_REQUEST,
      boost::bind(&GameMessageHandler::OnMessagePlayingDestoryRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_END_REQUEST,
      boost::bind(&GameMessageHandler::OnMessagePlayingEndRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_ACTOR_PAGE_SYNCHRONIZE,
      boost::bind(&GameMessageHandler::OnMessagePlayingActorPageSynchronize,
        this, _1, _2, _3));

  return true;
}

void GameMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_END_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_PLAYING_ACTOR_PAGE_SYNCHRONIZE);
}

void GameMessageHandler::OnMessagePlayingCreateRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingCreateRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("消息解析 MessagePlayingCreateRequest 失败");
    return ;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request.actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request.actor_);
    return ;
  }

  PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", request.actor_);
    return;
  }

  // 玩家当前所在副本
  if (actor->GetPlaying() != 0) {
    LOG_ERROR("玩家 PlayingActor(%lu) 处于副本状态中", request.actor_);
    return;
  }

  core::uint64 team_id = team_actor->GetTeam();
  if (team_id != 0) {
    PlayingTeam *playing_team = PlayingTeamManager::GetInstance()->Get(team_id);
    if (playing_team && playing_team->GetPlayingTemplate() != request.template_) {
      LOG_ERROR("玩家 PlayingActor(%lu) 不能创建非当前房间副本", request.actor_);
      return;
    }
  }

  // 获取副本配置
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(request.template_);
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 配置失败", request.template_);
    return ;
  }

  LOG_DEBUG("收到场景消息 MessagePlayingCreateRequest");

  session::protocol::MessagePlayingCreateResponse response;
  response.__set_actor_(request.actor_);
  response.__set_template_(request.template_);
  response.__set_result_(true);

  Playing *playing = NULL;

  switch (cell->camp_type_) {
    case entity::PlayingCampType::PLAYER:
      playing = this->CreateActorPlaying(request.actor_, cell);
      break;
    case entity::PlayingCampType::TEAM:
      playing = this->CreateTeamPlaying(request.actor_, cell);
      break;
    case entity::PlayingCampType::GUILD:
      playing = this->CreateGuildPlaying(request.actor_, cell);
      break;
    case entity::PlayingCampType::ROOM:
      playing = this->CreateRoomPlaying(request.actor_, cell);
      break;
    default:
      break;
  }

  if(playing == NULL) {
    response.__set_result_(false);
    LOG_ERROR("创建 Playing对象 失败");
  } else {
    response.__set_playing_(playing->GetID());

    // 副本负载均衡到 GameServer
    core::uint32 game_server_id = PlayingLoadBalance::GetInstance()->Allocate();
    SessionTerminal *terminal =
      SessionServerSingleton::GetInstance().GetTerminal(
          global::protocol::ServerType::GAME_SERVER, game_server_id);
    if(terminal != NULL) {
      playing->SetGameServer(game_server_id);
      session::protocol::MessagePlayingCreateSynchronize synchronize;
      synchronize.__set_playing_(playing->GetID());
      synchronize.__set_template_(request.template_);
      terminal->SendGameMessage(synchronize,
          session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_SYNCHRONIZE);
    } else {
      PlayingLoadBalance::GetInstance()->Deallocate(game_server_id);
      PlayingManager::GetInstance()->Remove(playing->GetID());
      PlayingTeamManager::GetInstance()->RemoveReadyTeam(playing->GetID());
      playing->Finalize();
      PlayingPool::GetInstance()->Deallocate(playing);
      response.__set_result_(false);
    }
  }

  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_RESPONSE);
}

void GameMessageHandler::OnMessagePlayingCreateFinishRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingCreateFinishRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  session::protocol::MessagePlayingCreateFinishResponse response;
  response.__set_playing_(request.playing_);
  response.__set_result_(false);

  // 获取 Playing 对象
  Playing *playing = PlayingManager::GetInstance()->Get(request.playing_);
  if(playing == NULL) {
    LOG_ERROR("获取 Playing对象 失败");
    terminal->SendGameMessage(response,
        session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_RESPONSE);
    return ;
  }

  // 获取副本配置
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(playing->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 配置失败", playing->GetTemplateID());
    return ;
  }

  if(request.__isset.failure_ == true) {
    PlayingLoadBalance::GetInstance()->Deallocate(playing->GetGameServer());
    PlayingManager::GetInstance()->Remove(playing->GetID());
    PlayingTeamManager::GetInstance()->RemoveReadyTeam(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("创建副本完成失败");
    return ;
  }

  LOG_DEBUG("收到场景消息 MessagePlayingCreateFinishRequest");

  if (cell->camp_type_ == entity::PlayingCampType::ROOM) {
    PlayingTeam *playing_team =
      PlayingTeamManager::GetInstance()->GetReadyTeam(playing->GetID());
    if (playing_team == NULL || playing_team->GetStatus() != entity::PlayingTeamStatus::WAITING) {
      PlayingLoadBalance::GetInstance()->Deallocate(playing->GetGameServer());
      PlayingManager::GetInstance()->Remove(playing->GetID());
      PlayingTeamManager::GetInstance()->RemoveReadyTeam(playing->GetID());
      playing->Finalize();
      PlayingPool::GetInstance()->Deallocate(playing);
      LOG_ERROR("获取 PlayingTeam 失败, 创建副本完成失败");
      return;
    }

    // 变为 READY 状态
    playing_team->GotoReady();
  }

  playing->SetScene(request.scene_);
  playing->SetStatus(StatusType::RUN);

  response.__set_result_(true);
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_FINISH_RESPONSE);

  // 通知客户端副本创建完成
  playing->BroadcastCreateFinish();
}

void GameMessageHandler::OnMessagePlayingDestoryRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingDestoryRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  session::protocol::MessagePlayingDestoryResponse response;
  response.__set_playing_(request.playing_);
  response.__set_result_(false);

  // 获取 Playing 对象
  Playing *playing = PlayingManager::GetInstance()->Remove(request.playing_);
  if(playing == NULL) {
    LOG_ERROR("删除 Playing(%lu) 失败", request.playing_);
    terminal->SendGameMessage(response,
        session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_RESPONSE);
    return ;
  }

  // 获取副本配置
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(playing->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 配置失败", playing->GetTemplateID());
    return ;
  }

  if (cell->camp_type_ == entity::PlayingCampType::ACTIVITY) {
    PlayingActivityController::GetInstance()->RemoveActivity(playing->GetTemplateID());
  }

  LOG_DEBUG("收到场景消息 MessagePlayingDestoryRequest");

  // 发送副本销毁事件
  {
    event::EventPlayingDestroy event;
    event.__set_playing_id_(playing->GetID());
    event.__set_playing_template_id_(playing->GetTemplateID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_PLAYING_DESTROY, playing->GetID(),
        event::ChannelType::CHANNEL_PLAYING, &event, sizeof(event));
  }

  PlayingLoadBalance::GetInstance()->Deallocate(playing->GetGameServer());
  playing->Finalize();
  PlayingPool::GetInstance()->Deallocate(playing);

  response.__set_result_(true);
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_PLAYING_DESTORY_RESPONSE);
}

void GameMessageHandler::OnMessagePlayingEndRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessagePlayingEndRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return ;
  }

  Playing *playing = PlayingManager::GetInstance()->Get(request.playing_);
  if(playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", request.playing_);
    return ;
  }

  playing->SetStatus(StatusType::END);

  session::protocol::MessagePlayingEndResponse response;
  response.__set_playing_(request.playing_);
  response.__set_result_(true);
  terminal->SendGameMessage(response,
      session::protocol::GameMessageType::MESSAGE_PLAYING_END_RESPONSE);

  // 发送副本结束事件
  {
    event::EventPlayingEnd event;
    event.__set_playing_id_(playing->GetID());
    event.__set_playing_template_id_(playing->GetTemplateID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_PLAYING_END, playing->GetID(),
        event::ChannelType::CHANNEL_PLAYING, &event, sizeof(event));
  }
}

void GameMessageHandler::OnMessagePlayingActorPageSynchronize(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (terminal == NULL || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessagePlayingActorPageSynchronize message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("消息解析失败");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(message.actor_);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", message.actor_);
    return;
  }

  for (size_t pos = 0; pos < message.pages_.size(); ++pos) {
    PlayingPage *page = actor->GetPage(message.pages_[pos].template_);
    if (page) {
      page->SetRecord(message.pages_[pos].record_);
      page->SetResult(message.pages_[pos].result_);
    } else {
      PlayingPage *page = PlayingPagePool::GetInstance()->Allocate();
      if (page == NULL) {
        LOG_ERROR("玩家(%lu) 分配 PlayingPage(%d) 失败",
            message.actor_, message.pages_[pos].template_);
        return;
      }
      if (page->Initialize(actor, message.pages_[pos].template_) == false) {
        LOG_ERROR("玩家(%lu) 初始化 PlayingPage(%d) 失败",
            message.actor_, message.pages_[pos].template_);
        PlayingPagePool::GetInstance()->Deallocate(page);
        return;
      }
      if (actor->AddPage(page) == false) {
        LOG_ERROR("玩家(%lu) 增加 PlayingPage(%d) 失败",
            message.actor_, message.pages_[pos].template_);
        page->Finalize();
        PlayingPagePool::GetInstance()->Deallocate(page);
        return;
      }
      page->SetRecord(message.pages_[pos].record_);
      page->SetResult(message.pages_[pos].result_);
    }
  }
}

Playing *GameMessageHandler::CreateActorPlaying(core::uint64 actor,
    const global::configure::PlayingCell *cell) {
  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(actor);
  if(playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", actor);
    return NULL;
  }

  // 分配 Playing 对象
  Playing *playing = PlayingPool::GetInstance()->Allocate();
  if(playing == NULL) {
    LOG_ERROR("分配 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // 初始化 Playing 对象
  if(playing->Initialize(cell->id_) == false) {
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("初始化 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // Playing对象 加入管理器
  if(PlayingManager::GetInstance()->Add(playing) == false) {
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Playing(%d) 加入管理器失败", cell->id_);
    return NULL;
  }

  // 加入玩家
  if(playing->AddActor(actor) == false) {
    LOG_ERROR("Actor(%lu)  加入副本玩家表失败", actor);
    PlayingManager::GetInstance()->Remove(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    return NULL;
  }

  playing_actor->SetPlaying(playing->GetID());

  return playing;
}

static void CreateTeamPlayingFailed(PlayingActor *actor, entity::InformType::type type) {
  gateway::protocol::MessageInformNotify message;
  message.__set_id_(type);
  actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
}

Playing *GameMessageHandler::CreateTeamPlaying(core::uint64 actor,
    const global::configure::PlayingCell *cell) {
  PlayingActor *playing_actor =
    PlayingActorManager::GetInstance()->Get(actor);
  if (playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", actor);
    return NULL;
  }

  request::RequestTeamGetMembers request;
  request.__set_actor_(actor);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TEAM_GET_MEMBERS,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("请求获取队员信息失败");
    return NULL;
  }

  // 成员数量是否满足要求
  size_t member_size = request.members_.size();
  if(member_size < (size_t)cell->min_actor_ || member_size > (size_t)cell->max_actor_) {
    LOG_ERROR("成员数量(%lu)不满足要求(%d,%d)", member_size,
        cell->min_actor_, cell->max_actor_);
    CreateTeamPlayingFailed(playing_actor, entity::InformType::PLAYING_MEMBER_NUMBER_ERROR);
    return NULL;
  }

  for(size_t pos = 0; pos < request.members_.size(); ++pos) {
    PlayingActor *playing_actor_member = PlayingActorManager::GetInstance()->Get(request.members_[pos]);
    if(playing_actor_member == NULL) {
      LOG_ERROR("获取 PlayingActor(%lu) 失败", request.members_[pos]);
      return NULL;
    }
    if (playing_actor_member->CheckPlaying() == true) {
      LOG_ERROR("成员(%lu) 已处于Playing(%lu) 状态中", request.members_[pos],
          playing_actor_member->GetPlaying());
      CreateTeamPlayingFailed(playing_actor, entity::InformType::PLAYING_MEMBER_PLAYING_ERROR);
      return NULL;
    }

    SessionActor *session_actor = playing_actor_member->GetActor();
    if(session_actor == NULL) {
      LOG_ERROR("获取 SessionActor(%lu) 失败", request.members_[pos]);
      return NULL;
    }

    BriefActor *brief_actor = session_actor->GetBriefActor();
    if (brief_actor == NULL) {
      LOG_ERROR("获取 BriefActor(%lu) 失败", request.members_[pos]);
      return NULL;
    }

    core::int32 vip_period_count = 0;
    const global::configure::VipCell *vip_cell =
      brief_actor->GetVipPrivilege(entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, cell->id_);
    if (vip_cell != NULL) {
      vip_period_count = vip_cell->value2_;
    }

    if(session_actor->GetLevel() < cell->min_level_ ||
        session_actor->GetLevel() > cell->max_level_) {
      LOG_ERROR("成员等级(%d)不满足要求(%d,%d)", session_actor->GetLevel(),
          cell->min_level_, cell->max_level_);
      CreateTeamPlayingFailed(playing_actor, entity::InformType::PLAYING_MEMBER_LEVEL_ERROR);
      return NULL;
    }
    const PlayingPage *page = playing_actor_member->GetPage(cell->id_);
    if (page) {
      const entity::PlayingRecordField &record = page->GetRecord();
      if (record.period_count_ >= cell->period_count_ + vip_period_count) {
        LOG_WARNING("玩家(%lu) 副本(%d) 次数(%d) 超过上限(%d) vip次数(%d)",
            playing_actor_member->GetID(), cell->id_, record.period_count_, cell->period_count_, vip_period_count);
        CreateTeamPlayingFailed(playing_actor, entity::InformType::PLAYING_MEMBER_PERIOD_COUNT_ERROR);
        return NULL;
      }
      if(cell->depend_ != 0) {
        const PlayingPage *depend_page = playing_actor_member->GetPage(cell->depend_);
        if (depend_page == NULL || depend_page->GetRecord().spend_secs_ <= 0) {
          LOG_WARNING("玩家(%lu) 副本(%d) 的前置副本(%d) 没有通关过",
              playing_actor_member->GetID(), cell->id_, cell->depend_);
          return NULL;
        } 
      }
    }
  }

  // 分配 Playing 对象
  Playing *playing = PlayingPool::GetInstance()->Allocate();
  if(playing == NULL) {
    LOG_ERROR("分配 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // 初始化 Playing 对象
  if(playing->Initialize(cell->id_) == false) {
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("初始化 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // Playing对象 加入管理器
  if(PlayingManager::GetInstance()->Add(playing) == false) {
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Playing(%d) 加入管理器失败", cell->id_);
    return NULL;
  }

  // 设置加入
  for(size_t pos = 0; pos < request.members_.size(); ++pos) {
    PlayingActor *playing_actor_member =
      PlayingActorManager::GetInstance()->Get(request.members_[pos]);
    if(playing_actor_member && playing->AddActor(request.members_[pos])) {
      playing_actor_member->SetPlaying(playing->GetID());
    }
  }

  return playing;
}

Playing *GameMessageHandler::CreateGuildPlaying(core::uint64 actor,
    const global::configure::PlayingCell *cell) {
  if (cell == NULL) {
    LOG_ERROR("参数错误");
    return NULL;
  }

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(actor);
  if (NULL == playing_actor) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", actor);
    return NULL;
  }

  // 检查是否可以创建公会副本
  {
    request::RequestGuildCheckCanCreatePlaying request;
    request.__set_actor_id_(actor);
    request.__set_playing_template_id_(cell->id_);
    if (ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_GUILD_CHECK_CAN_CREATE_PLAYING,
            &request, sizeof(request)) != 0) {
      LOG_ERROR("请求检查是否可以创建公会副本失败");
      return NULL;
    }
    if (false == request.result_) {
      return NULL;
    }
  }

  // 分配 Playing 对象
  Playing *playing = PlayingPool::GetInstance()->Allocate();
  if (NULL == playing) {
    LOG_ERROR("分配 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // 初始化 Playing 对象
  if (playing->Initialize(cell->id_) == false) {
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("初始化 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // Playing对象 加入管理器
  if (PlayingManager::GetInstance()->Add(playing) == false) {
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Playing(%d) 加入管理器失败", cell->id_);
    return NULL;
  }

  // 将创建副本的玩家加入副本
  if (playing->AddActor(actor) == false) {
    PlayingManager::GetInstance()->Remove(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Actor(%lu) 加入副本玩家失败", actor);
    return NULL;
  }
  playing_actor->SetPlaying(playing->GetID());

  // 设置公会当前正在进行中的副本
  {
    request::RequestGuildSetCurrentPlaying request;
    request.__set_actor_id_(actor);
    request.__set_playing_id_(playing->GetID());
    request.__set_playing_template_id_(playing->GetTemplateID());
    if (ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_GUILD_SET_CURRENT_PLAYING,
            &request, sizeof(request)) != 0) {
      playing_actor->SetPlaying(0);
      PlayingManager::GetInstance()->Remove(playing->GetID());
      playing->Finalize();
      PlayingPool::GetInstance()->Deallocate(playing);
      LOG_ERROR("请求设置公会当前正在进行中的副本失败");
      return NULL;
    }
  }

  return playing;
}

Playing *GameMessageHandler::CreateRoomPlaying(core::uint64 actor,
    const global::configure::PlayingCell *cell) {
  if (cell == NULL) {
    LOG_ERROR("参数错误");
    return NULL;
  }

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(actor);
  if (playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", actor);
    return NULL;
  }

  PlayingTeamActor *team_actor = playing_actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", actor);
    return NULL;
  }

  PlayingTeam *playing_team =
    PlayingTeamManager::GetInstance()->Get(team_actor->GetTeam());
  if (playing_team == NULL) {
    LOG_ERROR("获取 PlayingTeam(%lu) 失败", team_actor->GetTeam());
    return NULL;
  }

  if (playing_team->GetID() != actor) {
    LOG_ERROR("非队长不能开启副本");
    return NULL;
  }

  if (playing_team->CheckReady() == false) {
    LOG_ERROR("队伍中有玩家没有ready");
    return NULL;
  }

  // 分配 Playing 对象
  Playing *playing = PlayingPool::GetInstance()->Allocate();
  if(playing == NULL) {
    LOG_ERROR("分配 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // 初始化 Playing 对象
  if(playing->Initialize(cell->id_) == false) {
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("初始化 Playing(%d) 失败", cell->id_);
    return NULL;
  }

  // Playing对象 加入管理器
  if(PlayingManager::GetInstance()->Add(playing) == false) {
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Playing(%d) 加入管理器失败", cell->id_);
    return NULL;
  }

  playing_team->SetPlaying(playing->GetID());

  // 加入 ReadyTeam 表
  if (PlayingTeamManager::GetInstance()->AddReadyTeam(playing_team) == false) {
    PlayingManager::GetInstance()->Remove(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Playing(%d) 加入管理器失败", cell->id_);
    return NULL;
  }

  const PlayingTeam::MemberVector &members = playing_team->GetMembers();
  PlayingTeam::MemberVector::const_iterator iterator = members.begin();
  for (; iterator != members.end(); ++iterator) {
    PlayingTeamActor *team_actor_member = *iterator;
    if (team_actor_member == NULL) {
      continue;
    }
    PlayingActor *playing_actor_member = team_actor_member->GetActor();
    if (playing_actor_member == NULL) {
      continue;
    }
    if (playing->AddActor(playing_actor_member->GetID())) {
      playing_actor_member->SetPlaying(playing->GetID());
    }
  }

  // 同步状态广播到客户端
  gateway::protocol::MessagePlayingTeamSynchronize team_sync;
  team_sync.__set_id_(global::ToString(playing_team->GetID()));
  team_sync.__set_status_(playing_team->GetStatus());
  playing_team->BroadcastMessage(team_sync,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);

  return playing;
}

}  // namespace playing

}  // namespace server

}  // namespace session

