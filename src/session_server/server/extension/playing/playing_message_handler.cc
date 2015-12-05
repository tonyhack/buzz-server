//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 19:05:25.
// File name: playing_message_handler.cc
//
// Description:
// Define class PlayingMessageHandler.
//

#include "session_server/server/extension/playing/playing_message_handler.h"

#include <boost/bind.hpp>

#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_activity_controller.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_manager.h"
#include "session_server/server/extension/playing/playing_team.h"
#include "session_server/server/extension/playing/playing_team_manager.h"
#include "session_server/server/extension/playing/playing_team_pool.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"
#include "session_server/server/request/session_scene_request_types.h"
#include "session_server/server/request/session_team_request_types.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_protocol_types.h"

namespace session {

namespace server {

namespace playing {

PlayingMessageHandler::PlayingMessageHandler() {}
PlayingMessageHandler::~PlayingMessageHandler() {}

bool PlayingMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_ENTER_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingEnterRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_LIST_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingTeamListRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_CREATE_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingTeamCreateRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_JOIN_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingTeamJoinRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_QUIT_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingTeamQuitRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_READY_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingTeamReadyRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_KICKOUT_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingTeamKickoutRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_CAPACITY_SET_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingTeamCapacitySetRequest,
        this, _1, _2, _3));

  return true;
}

void PlayingMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_ENTER_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_LIST_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_CREATE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_JOIN_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_QUIT_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_READY_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_KICKOUT_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_CAPACITY_SET_REQUEST);
}

static void TryEnterGuildPlaying(PlayingActor *playing_actor, Playing *playing) {
  if (playing_actor->GetPlaying() != 0) {
    return;
  }

  if (playing_actor->GetPlayingAvailableCount(playing->GetTemplateID()) <= 0) {
    return;
  }

  request::RequestGuildGetCurrentPlaying request;
  request.__set_actor_id_(playing_actor->GetID());
  if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_GUILD_GET_CURRENT_PLAYING,
          &request, sizeof(request)) != 0) {
    LOG_ERROR("请求获取当前正在进行中的公会副本信息失败");
    return;
  }
  if (0 == request.playing_id_) {
    return;
  }

  playing->AddActor(playing_actor->GetID());
  playing_actor->SetPlaying(playing->GetID());
}

void PlayingMessageHandler::OnMessagePlayingEnterRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  gateway::protocol::MessagePlayingEnterRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_ENTER_REQUEST from actor(%lu, %s), playing_(%s), template_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.playing_.c_str(), request.template_);

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        request.template_);
  if (cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", request.template_);
    return;
  }

  core::uint64 playing_id = 0;

  if (cell->camp_type_ == entity::PlayingCampType::ACTIVITY) {
    playing_id = PlayingActivityController::GetInstance()->GetActivity(request.template_);
  } else {
    playing_id = atol(request.playing_.c_str());
  }

  Playing *playing = PlayingManager::GetInstance()->Get(playing_id);
  if (playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", playing_id);
    return;
  }

  if (actor->CheckFunctionalityOpen(playing->GetTemplateID()) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  // 判断副本的状态
  if (playing->GetStatus() != StatusType::RUN) {
    LOG_ERROR("玩家(%lu) 进入副本失败，副本不是 RUN状态");
    return;
  }

  if (entity::PlayingCampType::GUILD == cell->camp_type_) {
    // 尝试加入公会副本
    TryEnterGuildPlaying(actor, playing);
  }

  if (cell->camp_type_ != entity::PlayingCampType::ACTIVITY &&
      actor->GetPlaying() != playing_id) {
    LOG_ERROR("玩家进入非所在副本(%lu,%lu)", actor->GetPlaying(), playing_id);
    return;
  }

  if (cell->camp_type_ == entity::PlayingCampType::ROOM) {
    LOG_ERROR("副本(%d) 阵营类型为 entity::PlayingCampType::ROOM");
    return;
  }

  if (cell->camp_type_ != entity::PlayingCampType::ACTIVITY &&
      playing->CheckJoin(actor->GetID()) == false) {
    LOG_ERROR("检测 PlayingActor(%lu) 加入失败", actor->GetID());
    return;
  }

  core::int32 x = 0, y = 0;
  if (cell->GetEnterPosition(0, x, y) == false) {
    LOG_ERROR("获取 进入副本(%u)坐标 失败", playing->GetTemplateID());
    return;
  }

  request::RequestSceneJump request_jump;
  request_jump.__set_actor_(actor->GetID());
  request_jump.__set_scene_(playing->GetScene());
  request_jump.__set_x_(x);
  request_jump.__set_y_(y);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_JUMP,
        &request_jump, sizeof(request_jump)) != 0) {
    LOG_ERROR("请求跳转场景(%lu) 失败", playing->GetScene());
    return;
  }

  PlayingManager::GetInstance()->AddActorPlaying(actor->GetID(), playing->GetID());

  gateway::protocol::MessagePlayingEnterResponse response;
  response.__set_template_(playing->GetTemplateID());
  response.__set_result_(true);
  session_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_ENTER_RESPONSE);
}

void PlayingMessageHandler::OnMessagePlayingTeamListRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  if (actor->CheckFunctionalityOpen(entity::PlayingCampType::ROOM) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }


  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  gateway::protocol::MessagePlayingTeamListRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_TEAM_LIST_REQUEST from actor(%lu, %s), playing_template_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.playing_template_);

  gateway::protocol::MessagePlayingTeamListResponse response;
  response.__set_playing_template_(request.playing_template_);

  const PlayingTeamManager::TeamSet *teams =
    PlayingTeamManager::GetInstance()->GetPlayingTeams(request.playing_template_);
  if (teams != NULL) {
    entity::PlayingTeamField team_field;
    PlayingTeamManager::TeamSet::const_iterator iterator = teams->begin();
    for (; iterator != teams->end(); ++iterator) {
      PlayingTeam *team = PlayingTeamManager::GetInstance()->Get(*iterator);
      if (team != NULL) {
        team_field.__set_id_(global::ToString(team->GetID()));
        team_field.__set_name_(team->GetName());
        team_field.__set_min_fight_score_(team->GetMinFightScore());
        team_field.__set_auto_open_(team->GetAutoOpen());
        team_field.__set_member_number_(team->GetSize());
        team_field.__set_status_(team->GetStatus());
        team_field.__set_capacity_(team->GetCapacity());
        response.teams_.push_back(team_field);
      }
    }
  }

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_LIST_RESPONSE);
}

void PlayingMessageHandler::OnMessagePlayingTeamCreateRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  if (actor->CheckFunctionalityOpen(entity::PlayingCampType::ROOM) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  if (actor->CheckPlaying() == true) {
    LOG_ERROR("玩家(%lu) 正在副本中");
    return;
  }

  PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", id);
    return;
  }

  if (team_actor->GetTeam() != 0) {
    LOG_ERROR("玩家(%lu) 已在副本队伍中", id);
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  gateway::protocol::MessagePlayingTeamCreateRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_TEAM_CREATE_REQUEST from actor(%lu, %s), playing_template_(%d), min_fight_score_(%d), auto_open_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.playing_template_,
      request.auto_open_);

  if (actor->GetPlayingAvailableCount(request.playing_template_) <= 0) {
    LOG_ERROR("玩家(%lu) 副本(%d) 次数已耗尽", id, request.playing_template_);
    return;
  }

  PlayingTeam *team = PlayingTeamPool::GetInstance()->Allocate();
  if (team == NULL) {
    LOG_ERROR("分配 PlayingTeam 失败");
    return;
  }

  if (team->Initialize(request.playing_template_, actor->GetID()) == false) {
    PlayingTeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("初始化 PlayingTeam 失败");
    return;
  }

  team->SetMinFightScore(request.min_fight_score_);
  team->SetAutoOpen(request.auto_open_);

  if (team->AddMember(team_actor) == false) {
    team->Finalize();
    PlayingTeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("增加队员(%lu) 失败", actor->GetID());
    return;
  }

  if (PlayingTeamManager::GetInstance()->Add(team) == false) {
    team->RemoveMember(team_actor);
    team->Finalize();
    PlayingTeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("PlayingTeam 加入管理器失败");
    return;
  }

  request::RequestTeamGetMembers request_team;
  request_team.__set_actor_(id);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TEAM_GET_MEMBERS,
        &request_team, sizeof(request_team)) != 0) {
    team->RemoveMember(team_actor);
    team->Finalize();
    PlayingTeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("请求获取队员信息失败");
    return;
  }

  if (actor->GetID() == (core::uint64)request_team.captain_) {
    for (size_t pos = 0; pos < request_team.members_.size(); ++pos) {
      if ((core::uint64)request_team.members_[pos] != id) {
        PlayingActor *actor =
          PlayingActorManager::GetInstance()->Get(request_team.members_[pos]);
        if (actor == NULL) {
          LOG_ERROR("获取 PlayingActor(%lu) 失败", request_team.members_[pos]);
          continue;
        }
        if (actor->CheckPlaying() == true) {
          LOG_ERROR("增加队员(%lu) 已在副本中", id);
          continue;
        }
        PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
        if (team_actor == NULL) {
          LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", request_team.members_[pos]);
          continue;
        }
        if (team_actor->GetTeam() != 0) {
          LOG_ERROR("增加 队员(%lu) 已有副本队伍", id);
          continue;
        }
        if (actor->GetPlayingAvailableCount(request.playing_template_) <= 0) {
          LOG_DEBUG("actor(%lu) GetPlayingAvailableCount() <= 0", actor->GetID());
          continue;
        }
        if (team->AddMember(team_actor) == false) {
          LOG_ERROR("增加 队员(%lu) 失败", id);
          continue;
        }
      }
    }
  }

  gateway::protocol::MessagePlayingTeamCreateResponse response;
  response.__set_result_(true);
  response.__set_playing_template_(request.playing_template_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_CREATE_RESPONSE);

  team->Synchronize();
}

void PlayingMessageHandler::OnMessagePlayingTeamJoinRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  if (actor->CheckFunctionalityOpen(entity::PlayingCampType::ROOM) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  if (actor->CheckPlaying() == true) {
    LOG_ERROR("玩家(%lu) 正在副本中");
    return;
  }

  PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", id);
    return;
  }

  if (team_actor->GetTeam() != 0) {
    LOG_ERROR("玩家(%lu) 已在副本队伍中", id);
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  gateway::protocol::MessagePlayingTeamJoinRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_TEAM_JOIN_REQUEST from actor(%lu, %s), team_id_(%s)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.team_id_.c_str());

  PlayingTeam *team = PlayingTeamManager::GetInstance()->Get(atol(request.team_id_.c_str()));
  if (team == NULL) {
    LOG_ERROR("获取 PlayingTeam(%s) 失败", request.team_id_.c_str());
    return;
  }

  if (team->CheckMemberFull() == true) {
    LOG_ERROR("队伍人数已满");
    return;
  }

  if (actor->GetPlayingAvailableCount(team->GetPlayingTemplate()) <= 0) {
    LOG_ERROR("玩家(%lu) 副本(%d) 次数已耗尽", id, team->GetPlayingTemplate());
    return;
  }

  BriefActor *brief_actor = session_actor->GetBriefActor();
  if (brief_actor == NULL) {
    LOG_ERROR("获取 BriefActor(%lu) 失败", id);
    return;
  }

  if (brief_actor->GetCurrentFightScore() < team->GetMinFightScore()) {
    LOG_ERROR("玩家(%lu) 战斗力(%lu) 不足(%lu)",
        id, brief_actor->GetCurrentFightScore(), team->GetMinFightScore());
    return;
  }

  if (team->AddMember(team_actor) == false) {
    LOG_ERROR("队伍增加队员(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessagePlayingTeamJoinResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_JOIN_RESPONSE);

  team->Synchronize(actor);
}

void PlayingMessageHandler::OnMessagePlayingTeamQuitRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  if (actor->CheckFunctionalityOpen(entity::PlayingCampType::ROOM) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", id);
    return;
  }

  if (team_actor->GetTeam() == 0) {
    LOG_ERROR("玩家(%lu) 已在副本队伍中", id);
    return;
  }

  PlayingTeam *team = PlayingTeamManager::GetInstance()->Get(team_actor->GetTeam());
  if (team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", team_actor->GetTeam());
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_TEAM_QUIT_REQUEST from actor(%lu, %s)",
      session_actor->GetActorID(), session_actor->GetName().c_str());

  if (team->RemoveMember(team_actor) == false) {
    LOG_ERROR("移除 PlayingTeamActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessagePlayingTeamQuitResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_QUIT_RESPONSE);
}

void PlayingMessageHandler::OnMessagePlayingTeamReadyRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  if (actor->CheckFunctionalityOpen(entity::PlayingCampType::ROOM) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  if (actor->GetPlaying() != 0) {
    LOG_ERROR("玩家(%lu) 正在副本中");
    return;
  }

  PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", id);
    return;
  }

  if (team_actor->GetTeam() == 0) {
    LOG_ERROR("玩家(%lu) 已在副本队伍中", id);
    return;
  }

  PlayingTeam *playing_team =
    PlayingTeamManager::GetInstance()->Get(team_actor->GetTeam());
  if (playing_team == NULL) {
    LOG_ERROR("获取 PlayingTeam(%lu) 失败", team_actor->GetTeam());
    return;
  }

  if (playing_team->GetStatus() != entity::PlayingTeamStatus::WAITING) {
    LOG_ERROR("副本队伍非处于 WAITING 状态");
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  gateway::protocol::MessagePlayingTeamReadyRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_TEAM_READY_REQUEST from actor(%lu, %s), ready_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.ready_);

  team_actor->SetReady(request.ready_);

  gateway::protocol::MessagePlayingTeamReadyResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_READY_RESPONSE);

  // 同步队员信息
  gateway::protocol::MessagePlayingTeamMemberSynchronize synchronize;
  synchronize.__set_actor_(global::ToString(id));
  synchronize.__set_ready_(team_actor->GetReady());
  playing_team->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_MEMBER_SYNCHRONIZE);
}

void PlayingMessageHandler::OnMessagePlayingTeamKickoutRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  if (actor->CheckFunctionalityOpen(entity::PlayingCampType::ROOM) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", id);
    return;
  }

  if (team_actor->GetTeam() == 0) {
    LOG_ERROR("玩家(%lu) 已在副本队伍中", id);
    return;
  }

  PlayingTeam *playing_team =
    PlayingTeamManager::GetInstance()->Get(team_actor->GetTeam());
  if (playing_team == NULL) {
    LOG_ERROR("获取 PlayingTeam(%lu) 失败", team_actor->GetTeam());
    return;
  }

  if (playing_team->GetStatus() != entity::PlayingTeamStatus::WAITING) {
    LOG_ERROR("副本队伍非处于 WAITING 状态");
    return;
  }

  if (actor->GetID() != playing_team->GetID()) {
    LOG_ERROR("玩家不是队长，不能踢出队员");
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  gateway::protocol::MessagePlayingTeamKickoutRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_TEAM_KICKOUT_REQUEST from actor(%lu, %s), actor_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.actor_.c_str());

  core::uint64 kickout_actor = atol(request.actor_.c_str());
  PlayingActor *kickout_playing_actor =
    PlayingActorManager::GetInstance()->Get(kickout_actor);
  if (kickout_playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", kickout_actor);
    return;
  }

  PlayingTeamActor *kickout_team_actor = kickout_playing_actor->GetPlayingTeamActor();
  if (kickout_team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", kickout_actor);
    return;
  }

  if (kickout_team_actor->GetTeam() != playing_team->GetID()) {
    LOG_ERROR("踢出玩家(%lu) 不在当前队伍", kickout_actor);
    return;
  }

  if (playing_team->RemoveMember(kickout_team_actor) == false) {
    LOG_ERROR("踢出玩家不在当前队伍中");
    return;
  }

  gateway::protocol::MessagePlayingTeamKickoutResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_KICKOUT_RESPONSE);
}

void PlayingMessageHandler::OnMessagePlayingTeamCapacitySetRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  if (actor->CheckFunctionalityOpen(entity::PlayingCampType::ROOM) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  PlayingTeamActor *team_actor = actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", id);
    return;
  }

  if (team_actor->GetTeam() == 0) {
    LOG_ERROR("玩家(%lu) 已在副本队伍中", id);
    return;
  }

  PlayingTeam *playing_team =
    PlayingTeamManager::GetInstance()->Get(team_actor->GetTeam());
  if (playing_team == NULL) {
    LOG_ERROR("获取 PlayingTeam(%lu) 失败", team_actor->GetTeam());
    return;
  }

  if (playing_team->GetStatus() != entity::PlayingTeamStatus::WAITING) {
    LOG_ERROR("副本队伍非处于 WAITING 状态");
    return;
  }

  if (actor->GetID() != playing_team->GetID()) {
    LOG_ERROR("玩家不是队长");
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if(session_actor == NULL) {
    LOG_ERROR("玩家的对象没有找到(%lu)", id);
    return ;
  }

  gateway::protocol::MessagePlayingTeamCapacitySetRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_TEAM_CAPACITY_SET_REQUEST from actor(%lu, %s), capacity_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.capacity_);

  playing_team->SetCapacity(request.capacity_);

  gateway::protocol::MessagePlayingTeamSynchronize synchronize;
  synchronize.__set_id_(global::ToString(playing_team->GetID()));
  synchronize.__set_capacity_(playing_team->GetCapacity());
  playing_team->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_SYNCHRONIZE);

  gateway::protocol::MessagePlayingTeamCapacitySetResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_CAPACITY_SET_RESPONSE);
}

}  // namespace playing

}  // namespace server

}  // namespace session

