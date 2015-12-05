//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-22 18:09:52.
// File name: team_message_handler.cc
//
// Description:
// Define class TeamMessageHandler.
//

#include "session_server/server/extension/team/team_message_handler.h"

#include <boost/bind.hpp>

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_team_protocol_types.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "session_server/server/error_code.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/team/team.h"
#include "session_server/server/extension/team/team_actor.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_manager.h"
#include "session_server/server/extension/team/team_pool.h"
#include "session_server/server/extension/team/team_types.h"

namespace session {

namespace server {

namespace team {

TeamMessageHandler::TeamMessageHandler() {}
TeamMessageHandler::~TeamMessageHandler() {}

bool TeamMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_CREATE_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamCreateRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_JOIN_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamJoinRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_JOIN_DISPOSE_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamJoinDisposeRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_INVITE_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamInviteRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_INVITE_DISPOSE_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamInviteDisposeRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_DISMISS_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamDismissRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_QUIT_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamQuitRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_KICKOUT_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamKickoutRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_SET_CAPTAIN_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamSetCaptainRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_MAP_TEAMS_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamMapTeamsRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_MAP_ACTORS_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamMapActorsRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_REQUEST,
      boost::bind(&TeamMessageHandler::OnMessageTeamMemberRequest,
        this, _1, _2, _3));

  return true;
}

void TeamMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_CREATE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_JOIN_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_JOIN_DISPOSE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_INVITE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_INVITE_DISPOSE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_DISMISS_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_QUIT_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_KICKOUT_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_SET_CAPTAIN_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_MAP_TEAMS_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_MAP_ACTORS_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_REQUEST);
}

void TeamMessageHandler::OnMessageTeamCreateRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_CREATE_REQUEST from actor(%lu, %s)",
      session_actor->GetActorID(), session_actor->GetName().c_str());

  // 是否已组队
  if(actor->GetTeamID() != 0) {
    LOG_WARNING("玩家(%lu) 已组队(%lu)", id, actor->GetTeamID());
    return ;
  }

  // 分配队伍对象
  Team *team = TeamPool::GetInstance()->Allocate();
  if(team == NULL) {
    LOG_ERROR("玩家(%lu) 创建队伍时，分配 Team对象 失败", id);
    return ;
  }

  // 初始化队伍失败
  if(team->Initialize() == false) {
    TeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("玩家(%lu) 创建队伍时，初始化 Team对象 失败", id);
    return ;
  }

  // 设置队列ID为队长ID
  team->SetID(id);

  // 增加自己到队列列表
  if(team->AddMember(actor) == false) {
    team->Finalize();
    TeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("玩家(%lu) 创建队伍时，增加队员失败", id);
    return ;
  }

  // 加入队伍管理器
  if(TeamManager::GetInstance()->Add(team) == false) {
    team->Finalize();
    TeamPool::GetInstance()->Deallocate(team);
    LOG_ERROR("玩家(%lu) 创建队伍时，加入队伍管理器失败", id);
    return ;
  }


  gateway::protocol::MessageTeamCreateResponse response;
  response.__set_result_(true);
  response.__set_team_(global::ToString(team->GetID()));

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_CREATE_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamJoinRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamJoinRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamJoinRequest 失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_JOIN_REQUESTfrom actor(%lu, %s), team_(%s)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.team_.c_str());

  // 是否已组队
  if(actor->GetTeamID() != 0) {
    LOG_WARNING("玩家(%lu) 已组队(%lu)", id, actor->GetTeamID());
    return ;
  }

  // 获取队伍对象
  Team *team = TeamManager::GetInstance()->Get(atol(request.team_.c_str()));
  if(team == NULL) {
    LOG_WARNING("玩家(%lu) 请求失败，找不到 Team(%s)", id, request.team_.c_str());
    return ;
  }

  // 队伍是否满
  if(team->CheckMemberFull() == true) {
    // TODO 提示队伍成员已满
    LOG_ERROR("队伍人数已满");
    return ;
  }

  // 获取队长
  TeamActor *captain = TeamActorManager::GetInstance()->Get(team->GetID());
  if(captain == NULL) {
    LOG_WARNING("玩家(%lu) 请求失败，找不到 队长(%lu)", id, team->GetID());
    return ;
  }

  // 把请求消息插入命令列表
  if(captain->GetCommands().Add(id, TeamCommandType::JOIN) == false) {
    LOG_WARNING("玩家(%lu) 请求失败，插入队长(%lu)命令列表失败",
        id, team->GetID());
    return ;
  }

  // 回应请求
  gateway::protocol::MessageTeamJoinResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_JOIN_RESPONSE);

  // 通知队长有人申请加入
  gateway::protocol::MessageTeamJoinNotify notify;
  notify.__set_actor_(global::ToString(id));
  notify.__set_name_(session_actor->GetName());
  notify.__set_level_(session_actor->GetLevel());
  captain->SendMessage(notify,
      gateway::protocol::MessageType::MESSAGE_TEAM_JOIN_NOTIFY);
}

void TeamMessageHandler::OnMessageTeamJoinDisposeRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamJoinDisposeRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamJoinDisposeRequest 失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_JOIN_DISPOSE_REQUEST from actor(%lu, %s) ,actor_(%s),result_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.actor_.c_str(), request.result_);

  core::uint64 request_actor_id = atol(request.actor_.c_str());

  // 从队列中取出命令
  if(actor->GetCommands().Remove(request_actor_id,
        TeamCommandType::JOIN) == false) {
    LOG_WARNING("玩家(%lu) 处理加入队伍命令请求(%lu)时已失效",
        id, request_actor_id);
    return ;
  }

  // 获取请求加入队伍的玩家对象
  TeamActor *request_actor =
    TeamActorManager::GetInstance()->Get(request_actor_id);
  if(request_actor == NULL) {
    LOG_ERROR("获取请求玩家 TeamActor(%lu) 失败", request_actor_id);
    return ;
  }

  // 玩家是否是队长
  if(actor->GetTeamID() == 0 || actor->GetTeamID() != id) {
    LOG_WARNING("玩家(%lu) 不是队长，不能处理加入队伍命令请求", id);
    return ;
  }

  // 获取 Team
  Team *team = TeamManager::GetInstance()->Get(actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", actor->GetTeamID());
    return ;
  }

  // 队伍人数是否已满
  if(team->CheckMemberFull() == true) {
    LOG_WARNING("队伍人数已满");
    return ;
  }

  // 拒绝对方请求
  if(request.result_ == false) {
    LOG_WARNING("玩家(%lu) 拒绝了加入队伍命令请求(%lu)", id, request_actor_id);
    return ;
  }

  // 加入队伍
  if(team->AddMember(request_actor) == false) {
    LOG_WARNING("玩家(%lu) 加入队伍(%lu) 失败", request_actor_id, id);
    return ;
  }

  // 回应
  gateway::protocol::MessageTeamJoinDisposeResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_JOIN_DISPOSE_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamInviteRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamInviteRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamInviteRequest 失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_INVITE_REQUEST from actor(%lu, %s), actor_(%s), name_(%s)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.actor_.c_str(), request.name_.c_str());

  // 不在队伍或不是队长
  if(actor->GetTeamID() == 0 || actor->GetTeamID() != id) {
    LOG_WARNING("玩家(%lu) 请求失败，不是队长", id);
    return ;
  }

  Team *team = TeamManager::GetInstance()->Get(actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", actor->GetTeamID());
    return ;
  }

  if(team->CheckMemberFull() == true) {
    LOG_WARNING("队伍人数已满");
    return ;
  }

  core::uint64 request_actor_id = atol(request.actor_.c_str());

  // 得到被邀请玩家
  TeamActor *invite_actor = NULL;
  if(request.__isset.actor_ == true) {
    invite_actor = TeamActorManager::GetInstance()->Get(request_actor_id);
  } else if(request.__isset.name_ == true) {
    SessionActor *session_actor =
      SessionActorManager::GetInstance()->GetActor(request.name_);
    if(session_actor) {
      invite_actor =
        TeamActorManager::GetInstance()->Get(session_actor->GetActorID());
    }
  }
  if(invite_actor == NULL) {
    LOG_ERROR("获取被邀请玩家 TeamActor 失败");
    return ;
  }

  // 不能邀请已组队玩家
  if(invite_actor->GetTeamID() != 0) {
    ErrorCode::Send(id, entity::ErrorType::TEAM_HAVE_TEAM);
    LOG_ERROR("不能邀请已组队(%lu)玩家(%lu)", invite_actor->GetTeamID(),
        invite_actor->GetID());
    return ;
  }

  // 加入命令列表
  if(invite_actor->GetCommands().Add(id, TeamCommandType::INVITE) == false) {
    LOG_WARNING("玩家(%lu) 请求失败，插入被邀请者(%lu)命令列表失败",
        id, invite_actor->GetID());
    return ;
  }

  // 回应
  gateway::protocol::MessageTeamInviteResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_INVITE_RESPONSE);

  // 通知被邀请者
  gateway::protocol::MessageTeamInviteNotify notify;
  notify.__set_actor_(global::ToString(id));
  notify.__set_name_(session_actor->GetName());
  notify.__set_level_(session_actor->GetLevel());
  invite_actor->SendMessage(notify,
      gateway::protocol::MessageType::MESSAGE_TEAM_INVITE_NOTIFY);
}

void TeamMessageHandler::OnMessageTeamInviteDisposeRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamInviteDisposeRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamInviteDisposeRequest 失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_INVITE_DISPOSE_REQUEST from actor(%lu, %s) ,actor_(%s),result_(%d)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.actor_.c_str(), request.result_);

  core::uint64 request_actor_id = atol(request.actor_.c_str());

  // 从队列中取出命令
  if(actor->GetCommands().Remove(request_actor_id,
        TeamCommandType::INVITE) == false) {
    LOG_WARNING("玩家(%lu) 处理邀请加入队伍命令请求(%lu)时已失效",
        id, request_actor_id);
    return ;
  }

  // 拒绝对方请求
  if(request.result_ == false) {
    LOG_WARNING("玩家(%lu) 拒绝了邀请加入队伍命令请求(%lu)",
        id, request_actor_id);
    return ;
  }

  // 获取邀请加入队伍的玩家对象
  TeamActor *request_actor =
    TeamActorManager::GetInstance()->Get(request_actor_id);
  if(request_actor == NULL) {
    LOG_ERROR("获取请求玩家 TeamActor(%lu) 失败", request_actor_id);
    return ;
  }

  // 玩家是否是队长
  if(request_actor->GetTeamID() == 0 ||
      request_actor->GetTeamID() != request_actor_id) {
    LOG_WARNING("玩家(%lu) 不是队长，不能处理加入队伍命令请求", request_actor_id);
    return ;
  }

  // 获取 Team
  Team *team = TeamManager::GetInstance()->Get(request_actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", request_actor->GetTeamID());
    return ;
  }

  // 队伍人数是否已满
  if(team->CheckMemberFull() == true) {
    LOG_WARNING("队伍人数已满");
    return ;
  }

  // 加入队伍
  if(team->AddMember(actor) == false) {
    LOG_WARNING("玩家(%lu) 加入队伍(%lu) 失败", request_actor_id, id);
    return ;
  }

  // 回应
  gateway::protocol::MessageTeamInviteDisposeResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_INVITE_DISPOSE_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamDismissRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_DISMISS_REQUEST from actor(%lu, %s)",
      session_actor->GetActorID(), session_actor->GetName().c_str());

  // 是否队长
  if(actor->GetTeamID() != id) {
    LOG_WARNING("玩家(%lu) 解散队伍失败，不是队长", id);
    return ;
  }

  // 获取组队对象
  Team *team = TeamManager::GetInstance()->Get(id);
  if(team == NULL) {
    LOG_ERROR("玩家(%lu) 解散队伍失败，获取队伍对象失败", id);
    return ;
  }

  gateway::protocol::MessageTeamDismissResponse response;
  response.__set_result_(false);

  // 解散队伍
  if(team->RemoveAll() == true) {
    // 回收对象
    team->Finalize();
    TeamPool::GetInstance()->Deallocate(team);
    response.__set_result_(true);
  }

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_DISMISS_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamQuitRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_QUIT_REQUEST from actor(%lu, %s)",
      session_actor->GetActorID(), session_actor->GetName().c_str());

  // 玩家是否组队中
  if(actor->GetTeamID() == 0) {
    LOG_WARNING("玩家(%lu) 当前不在组队状态", id);
    return ;
  }

  // 获取 Team 对象
  Team *team = TeamManager::GetInstance()->Get(actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", actor->GetTeamID());
    return ;
  }

  // 从队伍中移除自己
  if(team->RemoveMember(actor) == false) {
    LOG_ERROR("玩家(%lu) 退出队伍失败", id);
    return ;
  }

  gateway::protocol::MessageTeamQuitResponse response;
  response.__set_result_(true);

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_QUIT_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamKickoutRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamKickoutRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamKickoutRequest 失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_KICKOUT_RESPONSE from actor(%lu, %s) ,actor_(%s)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.actor_.c_str());

  // 玩家是否组队中
  if(actor->GetTeamID() != id) {
    LOG_WARNING("玩家(%lu) 不是队长，不能踢人", id);
    return ;
  }

  // 被踢出玩家对象
  TeamActor *kickout_actor =
    TeamActorManager::GetInstance()->Get(atol(request.actor_.c_str()));
  if(kickout_actor == NULL) {
    LOG_ERROR("获取 被踢出玩家 TeamActor(%s) 失败", request.actor_.c_str());
    return ;
  }

  // 是否是同一队的
  if(kickout_actor->GetTeamID() != actor->GetTeamID()) {
    LOG_WARNING("玩家(%lu) 与 被踢玩家(%lu) 不在同一个队伍", id, kickout_actor->GetID());
    return ;
  }

  // 获取 Team 对象
  Team *team = TeamManager::GetInstance()->Get(actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", actor->GetTeamID());
    return ;
  }

  // 踢出队伍
  if(team->RemoveMember(kickout_actor) == false) {
    LOG_ERROR("踢出 队伍玩家(%lu) 失败", kickout_actor->GetID());
    return ;
  }

  gateway::protocol::MessageTeamKickoutResponse response;
  response.__set_result_(true);

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_KICKOUT_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamSetCaptainRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  TeamActor *actor = TeamActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", id);
    return ;
  }

  // 获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(id);
  if(session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamKickoutRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamKickoutRequest 失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_SET_CAPTAIN_REQUEST from actor(%lu, %s) ,actor_(%s)",
      session_actor->GetActorID(), session_actor->GetName().c_str(), request.actor_.c_str());

  // 玩家是否组队中
  if(actor->GetTeamID() != id) {
    LOG_WARNING("玩家(%lu) 不是队长，不能踢人", id);
    return ;
  }

  // 被踢出玩家对象
  TeamActor *new_captain =
    TeamActorManager::GetInstance()->Get(atol(request.actor_.c_str()));
  if(new_captain == NULL) {
    LOG_ERROR("获取 新队长 TeamActor(%s) 失败", request.actor_.c_str());
    return ;
  }

  // 是否是同一队的
  if(new_captain->GetTeamID() != actor->GetTeamID()) {
    LOG_WARNING("玩家(%lu) 与 新队长(%lu) 不在同一个队伍", id, new_captain->GetID());
    return ;
  }

  // 获取 Team 对象
  Team *team = TeamManager::GetInstance()->Get(actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", actor->GetTeamID());
    return ;
  }

  // 设置新队长
  if(team->SetNewCaptain(new_captain) == false) {
    LOG_ERROR("玩家(%lu) 设置新队长时出错", id);
    return ;
  }

  gateway::protocol::MessageTeamSetCaptainResponse response;
  response.__set_result_(true);

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_SET_CAPTAIN_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamMapTeamsRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamMapTeamsRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamMapTeamsRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_MAP_TEAMS_REQUEST from actor(%lu, %s)",
      actor->GetActorID(), actor->GetName().c_str());

  this->request_scene_get_actors_.actors_.clear();
  this->request_scene_get_actors_.__set_scene_(actor->GetScene());

  // 请求
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_GET_ACTORS,
        &this->request_scene_get_actors_,
        sizeof(this->request_scene_get_actors_)) != 0) {
    LOG_ERROR("请求 场景(%lu)玩家列表失败", actor->GetScene());
    return ;
  }

  this->message_team_map_teams_.teams_.clear();
  gateway::protocol::TeamFields field;

  for(std::vector<int64_t>::iterator iterator = this->request_scene_get_actors_.actors_.begin();
      iterator != this->request_scene_get_actors_.actors_.end(); ++iterator) {
    Team *team = TeamManager::GetInstance()->Get(*iterator);
    if(team) {
      TeamActor *captain = TeamActorManager::GetInstance()->Get(*iterator);
      if(captain) {
        field.__set_team_(global::ToString(team->GetID()));
        field.__set_name_(captain->GetName());
        field.__set_number_(team->GetSize());
        field.__set_captain_level_(captain->GetLevel());
        field.__set_average_level_(team->GetAverageLevel());
        this->message_team_map_teams_.teams_.push_back(field);
      }
    }
  }

  actor->SendMessage(this->message_team_map_teams_,
      gateway::protocol::MessageType::MESSAGE_TEAM_MAP_TEAMS_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamMapActorsRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamMapActorsRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamMapActorsRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_MAP_ACTORS_REQUEST from actor(%lu, %s)",
      actor->GetActorID(), actor->GetName().c_str());

  this->request_scene_get_actors_.actors_.clear();
  this->request_scene_get_actors_.__set_scene_(actor->GetScene());

  // 请求
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_GET_ACTORS,
        &this->request_scene_get_actors_,
        sizeof(this->request_scene_get_actors_)) != 0) {
    LOG_ERROR("请求 场景(%lu)玩家列表失败", actor->GetScene());
    return ;
  }

  this->message_team_map_actors_.actors_.clear();
  gateway::protocol::TeamActorFields field;

  // 获取场景所有玩家
  for(std::vector<int64_t>::iterator iterator = this->request_scene_get_actors_.actors_.begin();
      iterator != this->request_scene_get_actors_.actors_.end(); ++iterator) {
    if(*iterator != (core::int64)id) {
      TeamActor *team_actor = TeamActorManager::GetInstance()->Get(*iterator);
      if(team_actor) {
        field.__set_actor_(global::ToString(team_actor->GetID()));
        field.__set_name_(team_actor->GetName());
        field.__set_level_(team_actor->GetLevel());
        field.__set_team_(global::ToString(team_actor->GetTeamID()));
        this->message_team_map_actors_.actors_.push_back(field);
      }
    }
  }

  actor->SendMessage(this->message_team_map_actors_,
      gateway::protocol::MessageType::MESSAGE_TEAM_MAP_ACTORS_RESPONSE);
}

void TeamMessageHandler::OnMessageTeamMemberRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    return ;
  }

  // 获取玩家对象
  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTeamMemberRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request,
        data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamMemberRequest 失败");
    return ;
  }

  // 获取 Team 对象
  Team *team = TeamManager::GetInstance()->Get(atol(request.team_.c_str()));
  if(team == NULL) {
    LOG_ERROR("获取 Team(%s) 失败", request.team_.c_str());
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TEAM_MEMBER_REQUEST from actor(%lu, %s)",
      actor->GetActorID(), actor->GetName().c_str());

  gateway::protocol::MessageTeamMemberResponse response;
  gateway::protocol::TeamMemberFields field;
  response.__set_team_(request.team_);

  const Team::MemberVector &members = team->GetMembers();
  for(Team::MemberVector::const_iterator iterator = members.begin();
      iterator != members.end(); ++iterator) {
    const TeamActor *team_actor = *iterator;
    if(actor) {
      field.__set_actor_(global::ToString(team_actor->GetID()));
      field.__set_name_(team_actor->GetName());
      field.__set_level_(team_actor->GetLevel());
      field.__set_map_(team_actor->GetMap());
      response.members_.push_back(field);
    }
  }

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_RESPONSE);
}

}  // namespace team

}  // namespace server

}  // namespace session

