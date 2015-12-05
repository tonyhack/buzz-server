#include "session_server/server/extension/guild/guild_message_handler.h"

#include <cstdio>
#include <vector>
#include <boost/bind.hpp>

#include "entity/guild_types.h"
#include "entity/inform_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/guild/guild.h"
#include "session_server/server/extension/guild/guild_manager.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"
#include "session_server/server/extension/guild/guild_event.h"

#define MAX_GUILD_ONCE 100

namespace session {

namespace server {

namespace guild {

GuildMessageHandler::GuildMessageHandler() {}
GuildMessageHandler::~GuildMessageHandler() {}

bool GuildMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_LIST_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildListRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SUPPRESS_JOIN_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildSuppressJoinRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_JOIN_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildJoinRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_CANCEL_JOIN_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildCancelJoinRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_AUDIT_PENDING_MEMBER_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildAuditPendingMemberRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_INVITE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildInviteRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_INVITE_DISPOSE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildInviteDisposeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_LEAVE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildLeaveRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_KICK_MEMBER_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildKickMemberRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildGiveUpLeaderPositionRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_POSITION_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildGiveUpPositionRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SET_MEMBER_POSITION_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildSetMemberPositionRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_EDIT_ANNOUNCEMENT_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildEditAnnouncementRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_UPGRADE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_UPGRADE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildShopUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_UPGRADE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildCollegeUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_LEARN_SKILL_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildCollegeLearnSkillRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_IDOL_PRAY_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildIdolPrayRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_BARRACK_UPGRADE_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildBarrackUpgradeRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_GROUP_OPEN_REQUEST,
      boost::bind(&GuildMessageHandler::OnMessageGuildPlayingGroupOpenRequest,
          this, _1, _2, _3));

  return true;
}

void GuildMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_LIST_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SUPPRESS_JOIN_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_JOIN_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_CANCEL_JOIN_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_AUDIT_PENDING_MEMBER_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_INVITE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_INVITE_DISPOSE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_LEAVE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_KICK_MEMBER_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_POSITION_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SET_MEMBER_POSITION_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_EDIT_ANNOUNCEMENT_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_LEARN_SKILL_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_IDOL_PRAY_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_BARRACK_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_GROUP_OPEN_REQUEST);
}

static void SendMessageGuildListResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildListResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_LIST_RESPONSE);
}

static void SendMessageGuildSuppressJoinResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildSuppressJoinResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SUPPRESS_JOIN_RESPONSE);
}

static void SendMessageGuildJoinResponse(GuildActor *guild_actor,
    gateway::protocol::GuildErrorCode::type error_code) {
  gateway::protocol::MessageGuildJoinResponse response;
  response.__set_result_(error_code);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_JOIN_RESPONSE);
}

static void SendMessageGuildCancelJoinResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildCancelJoinResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_CANCEL_JOIN_RESPONSE);
}

static void SendMessageGuildAuditPendingMemberResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildAuditPendingMemberResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_AUDIT_PENDING_MEMBER_RESPONSE);
}

static void SendMessageGuildInviteResponse(GuildActor *guild_actor,
    gateway::protocol::GuildErrorCode::type error_code) {
  gateway::protocol::MessageGuildInviteResponse response;
  response.__set_result_(error_code);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_INVITE_RESPONSE);
}

static void SendMessageGuildInviteDisposeResponse(GuildActor *guild_actor,
    gateway::protocol::GuildErrorCode::type error_code) {
  gateway::protocol::MessageGuildInviteDisposeResponse response;
  response.__set_result_(error_code);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_INVITE_DISPOSE_RESPONSE);
}

static void SendMessageGuildLeaveResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildLeaveResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_LEAVE_RESPONSE);
}

static void SendMessageGuildKickMemberResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildKickMemberResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_KICK_MEMBER_RESPONSE);
}

static void SendMessageGuildGiveUpLeaderPositionResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildGiveUpLeaderPositionResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_RESPONSE);
}

static void SendMessageGuildGiveUpPositionResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildGiveUpPositionResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_POSITION_RESPONSE);
}

static void SendMessageGuildSetMemberPositionResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildSetMemberPositionResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SET_MEMBER_POSITION_RESPONSE);
}

static void SendMessageGuildEditAnnouncementResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildEditAnnouncementResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_EDIT_ANNOUNCEMENT_RESPONSE);
}

static void SendMessageGuildUpgradeResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildUpgradeResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_UPGRADE_RESPONSE);
}

static void SendMessageGuildShopUpgradeResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildShopUpgradeResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_UPGRADE_RESPONSE);
}

static void SendMessageGuildCollegeUpgradeResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildCollegeUpgradeResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_UPGRADE_RESPONSE);
}

static void SendMessageGuildBarrackUpgradeResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildBarrackUpgradeResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_BARRACK_UPGRADE_RESPONSE);
}

static void SendMessageGuildCollegeLearnSkillResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildCollegeLearnSkillResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_LEARN_SKILL_RESPONSE);
}

static void SendMessageGuildIdolPrayResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildIdolPrayResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_IDOL_PRAY_RESPONSE);
}

static void SendMessageGuildPlayingGroupOpenResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildPlayingGroupOpenResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_GROUP_OPEN_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildListRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildListRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildListRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_LIST_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildListResponseFailed(guild_actor);
    return;
  }


  // 发送回复
  gateway::protocol::MessageGuildListResponse response;
  response.__set_result_(true);
  response.__isset.guilds_ = true;

  const GuildManager::GuildMap &guilds = GuildManager::GetInstance()->GetGuilds();
  size_t guild_count = 0;

  GuildManager::GuildMap::const_iterator iter = guilds.begin();
  for (; iter != guilds.end(); ++iter) {
    Guild *guild = iter->second;
    const GuildActor *leader = guild->GetLeader();
    if (NULL == leader) {
      LOG_ERROR("Guild(%lu) has no leader.", guild->GetID());
      continue;
    }

    gateway::protocol::GuildListData data;
    data.__set_guild_id_(guild->GetStringID());
    data.__set_guild_name_(guild->GetName());
    data.__set_guild_level_(guild->GetLevel());
    data.__set_member_num_(guild->GetMemberNum());
    data.__set_leader_actor_id_(leader->GetStringID());
    data.__set_leader_actor_name_(leader->GetName());
    data.__set_leader_online_(leader->CheckOnline());
    data.__set_total_fight_score_(global::ToString(guild->GetTotalFightScore()));
    data.__set_suppress_join_(guild->CheckSuppressJoin());
    data.__set_joined_(guild_actor->CheckJoinedGuild(guild));

    response.guilds_.push_back(data);

    if (response.guilds_.size() >= MAX_GUILD_ONCE) {
      guild_actor->SendMessage(response,
          gateway::protocol::MessageType::MESSAGE_GUILD_LIST_RESPONSE);
      response.guilds_.clear();
    }
  }

  if (!response.guilds_.empty()) {
    guild_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_GUILD_LIST_RESPONSE);
  }
}

void GuildMessageHandler::OnMessageGuildSuppressJoinRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildSuppressJoinRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildSuppressJoinRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_SUPPRESS_JOIN_REQUEST from actor(%lu, %s), "
           "suppress_join_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.suppress_join_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildSuppressJoinResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildSuppressJoinResponseFailed(guild_actor);
    return;
  }

  // 检查是否有更改权限
  const global::configure::GuildPositionCell *cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildSuppressJoinResponseFailed(guild_actor);
    return;
  }
  if (!cell->authority_suppress_join_) {
    LOG_WARNING("No authority_suppress_join.");
    SendMessageGuildSuppressJoinResponseFailed(guild_actor);
    return;
  }

  guild->SetSuppressJoin(request.suppress_join_);

  gateway::protocol::MessageGuildSuppressJoinResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SUPPRESS_JOIN_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildJoinRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildJoinRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildJoinRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_JOIN_REQUEST from actor(%lu, %s), "
           "guild_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.guild_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查是否有公会
  if (guild_actor->GetGuild() != NULL) {
    LOG_WARNING("Already in a guild.");
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查同时申请公会的上限
  if (guild_actor->GetJoinedGuildNum() >=
      MISC_CONF()->guild_max_join_capacity_) {
    LOG_WARNING("Join too many guilds.");
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查公会是否存在
  Guild *guild = GuildManager::GetInstance()->GetGuild(atol(request.guild_id_.c_str()));
  if (NULL == guild) {
    LOG_WARNING("Guild(%s) does not exist.", request.guild_id_.c_str());
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查是否已申请该公会
  if (guild_actor->CheckJoinedGuild(guild)) {
    LOG_WARNING("already joined this guild.");
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查公会是否禁止申请
  if (guild->CheckSuppressJoin()) {
    LOG_WARNING("Guild(%lu) suppress join.", guild->GetID());
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查公会待审核成员数量上限
  if (guild->CheckPendingMemberFull()) {
    LOG_WARNING("Guild(%lu) pending member is full.", guild->GetID());
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::PENDING_MEMBER_FULL);
    return;
  }

  // 检查公会成员数量上限
  if (guild->CheckMemberFull()) {
    LOG_WARNING("Guild(%lu) member is full.", guild->GetID());
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }
  
  // 申请该公会 
  if (guild_actor->AddJoinedGuild(guild) == false) {
    LOG_ERROR("GuildActor(%lu) add Guild(%lu) failed.", id, guild->GetID());
    SendMessageGuildJoinResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  SendMessageGuildJoinResponse(guild_actor, gateway::protocol::GuildErrorCode::SUCCESS);
}

void GuildMessageHandler::OnMessageGuildCancelJoinRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildCancelJoinRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildCancelJoinRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_CANCEL_JOIN_REQUEST from actor(%lu, %s), "
           "guild_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.guild_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildCancelJoinResponseFailed(guild_actor);
    return;
  }

  // 检查公会是否存在
  Guild *guild = GuildManager::GetInstance()->GetGuild(atol(request.guild_id_.c_str()));
  if (NULL == guild) {
    LOG_WARNING("Guild(%s) does not exist.", request.guild_id_.c_str());
    SendMessageGuildCancelJoinResponseFailed(guild_actor);
    return;
  }

  // 检查是否已申请该公会
  if (guild_actor->CheckJoinedGuild(guild) == false) {
    LOG_WARNING("Do not join this guild.");
    SendMessageGuildCancelJoinResponseFailed(guild_actor);
    return;
  }

  // 撤销申请该公会
  if (guild_actor->RemoveJoinedGuild(guild) == false) {
    LOG_ERROR("RemoveJoinedGuild failed.");
    SendMessageGuildCancelJoinResponseFailed(guild_actor);
    return;
  }

  gateway::protocol::MessageGuildCancelJoinResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_CANCEL_JOIN_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildAuditPendingMemberRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildAuditPendingMemberRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildAuditPendingMemberRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_AUDIT_PENDING_MEMBER_REQUEST from actor (%lu, %s), "
           "actor_id_=[%s], audit_result_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str(), request.audit_result_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
    return;
  }

  // 检查是否有审核权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_audit_pending_member_) {
    LOG_WARNING("No authority_audit_pending_member.");
    SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
    return;
  }

  // 取待审核公会玩家对象
  GuildActor *target_guild_actor =
    GuildActorManager::GetInstance()->GetFromAll(atol(request.actor_id_.c_str()));
  if (NULL == target_guild_actor) {
    LOG_ERROR("Get target GuildActor(%s) from GuildActorManager failed.", request.actor_id_.c_str());
    SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
    return;
  }

  // 检查目标玩家是否已申请该公会
  if (target_guild_actor->CheckJoinedGuild(guild) == false) {
    LOG_WARNING("Target GuildActor(%lu) do not join this guild.", target_guild_actor->GetActorID());
    SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
    return;
  }

  if (request.audit_result_) {
    // 检查公会成员数是否已满
    if (guild->CheckMemberFull()) {
      LOG_WARNING("Guild(%lu) member is full.", guild->GetID());
      SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
      return;
    }
    // 撤销所有申请
    target_guild_actor->RemoveAllJoinedGuild();

    // 添加成员
    if (guild->AddMember(target_guild_actor) == false) {
      LOG_ERROR("Add target GuildActor(%lu) to guild failed.", target_guild_actor->GetActorID());
      SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
      return;
    }
    // 加入公会事件
    GuildEvent event(entity::GuildEventType::JOIN_GUILD);
    event.PushEventVar(target_guild_actor->GetActorID());
    guild->AddEvent(event);
    // 加入公会公告
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GUILD_JOIN);
    inform.params_.push_back(target_guild_actor->GetStringID());
    inform.params_.push_back(target_guild_actor->GetName());
    guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  } else {
    // 撤销申请
    if (target_guild_actor->RemoveJoinedGuild(guild) == false) {
      LOG_ERROR("Target GuildActor(%lu) RemoveJoinedGuild(%lu) failed.",
                target_guild_actor->GetActorID(), guild->GetID());
      SendMessageGuildAuditPendingMemberResponseFailed(guild_actor);
      return;
    }
    // 加入军团申请被拒绝公告
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GUILD_REFUSE_JOIN);
    inform.params_.push_back(guild->GetStringID());
    inform.params_.push_back(guild->GetName());
    target_guild_actor->SendMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }

  gateway::protocol::MessageGuildAuditPendingMemberResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_AUDIT_PENDING_MEMBER_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildInviteRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildInviteRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildInviteRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_INVITE_REQUEST from actor(%lu, %s), "
           "actor_name_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_name_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查是否有邀请权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }
  if (!position_cell->authority_invite_) {
    LOG_WARNING("No authority_invite.");
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查公会成员是否已满
  if (guild->CheckMemberFull()) {
    LOG_WARNING("Guild(%lu) member is full.", guild->GetID());
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查被邀请的玩家是否在线 
  SessionActor *target_session_actor =
    SessionActorManager::GetInstance()->GetActor(request.actor_name_);
  if (NULL == target_session_actor) {
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::TARGET_OFFLINE);
    return;
  }
  GuildActor *target_guild_actor =
    GuildActorManager::GetInstance()->Get(target_session_actor->GetActorID());
  if (NULL == target_guild_actor) {
    LOG_ERROR("Get target GuildActor(%lu) from GuildActorManager failed.",
              target_guild_actor->GetActorID());
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查被邀请的玩家是否已有公会功能
  if (target_session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::TARGET_GUILD_MODULE_DISABLED);
    return;
  }

  // 检查被邀请的玩家是否已有公会
  if (target_guild_actor->GetGuild() != NULL) {
    SendMessageGuildInviteResponse(guild_actor,
        gateway::protocol::GuildErrorCode::TARGET_ALREADY_IN_GUILD);
    return;
  }

  // 加入申请列表
  guild->AddInviteMember(target_guild_actor->GetActorID());

  // 给被邀请的玩家发送通知
  gateway::protocol::MessageGuildInviteNotify notify;
  notify.__set_guild_id_(guild->GetStringID());
  target_guild_actor->SendMessage(notify,
      gateway::protocol::MessageType::MESSAGE_GUILD_INVITE_NOTIFY);

  SendMessageGuildInviteResponse(guild_actor, gateway::protocol::GuildErrorCode::SUCCESS);
}

void GuildMessageHandler::OnMessageGuildInviteDisposeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildInviteDisposeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildInviteDisposeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_INVITE_DISPOSE_REQUEST from actor(%lu, %s), "
           "guild_id_=[%s], result_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.guild_id_.c_str(), request.result_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildInviteDisposeResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查玩家是否已有公会
  if (guild_actor->GetGuild() != NULL) {
    LOG_WARNING("Already in a guild.");
    SendMessageGuildInviteDisposeResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查公会是否存在
  Guild *guild = GuildManager::GetInstance()->GetGuild(atol(request.guild_id_.c_str()));
  if (NULL == guild) {
    LOG_WARNING("Guild(%s) does not exist.", request.guild_id_.c_str());
    SendMessageGuildInviteDisposeResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查是否被邀请
  if (guild->CheckInvitedMember(guild_actor->GetActorID()) == false) {
    LOG_WARNING("Not be invited by Guild(%lu)", guild->GetID());
    SendMessageGuildInviteDisposeResponse(guild_actor,
        gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 删除邀请
  guild->RemoveInviteMember(guild_actor->GetActorID());

  if (request.result_) {
    // 检查公会成员是否已满
    if (guild->CheckMemberFull()) {
      SendMessageGuildInviteDisposeResponse(guild_actor,
          gateway::protocol::GuildErrorCode::MEMBER_FULL);
      return;
    }
    // 撤销所有申请
    guild_actor->RemoveAllJoinedGuild();

    // 添加成员
    if (guild->AddMember(guild_actor) == false) {
      LOG_ERROR("Add GuildActor(%lu) to Guild(%lu) failed.",
                guild_actor->GetActorID(), guild->GetID());
      SendMessageGuildInviteDisposeResponse(guild_actor,
          gateway::protocol::GuildErrorCode::ERROR_UNKNOWN);
      return;
    }
    // 加入公会事件
    GuildEvent event(entity::GuildEventType::JOIN_GUILD);
    event.PushEventVar(guild_actor->GetActorID());
    guild->AddEvent(event);
    // 加入公会公告
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GUILD_JOIN);
    inform.params_.push_back(guild_actor->GetStringID());
    inform.params_.push_back(guild_actor->GetName());
    guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  } else {
    // 邀请加入军团被拒绝公告
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GUILD_REFUSE_INVITE);
    inform.params_.push_back(guild_actor->GetStringID());
    inform.params_.push_back(guild_actor->GetName());

    const Guild::GuildActorSet &members = guild->GetMembers(); 
    for (Guild::GuildActorSet::iterator iter = members.begin();
         iter != members.end(); ++iter) {
      GuildActor *member = *iter;
      if (NULL == member) {
        continue;
      }

      const global::configure::GuildPositionCell *position_cell =
        GUILD_CONF()->GetGuildPosition(member->GetGuildPosition());
      if (NULL == position_cell) {
        LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
        continue;
      }
      if (position_cell->authority_invite_) {
        member->SendMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
      }
    }
  }

  SendMessageGuildInviteDisposeResponse(guild_actor, gateway::protocol::GuildErrorCode::SUCCESS);
}

void GuildMessageHandler::OnMessageGuildLeaveRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildLeaveRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildLeaveRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_LEAVE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildLeaveResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildLeaveResponseFailed(guild_actor);
    return;
  }

  if (guild_actor->GetGuildPosition() == entity::GuildPositionType::LEADER) {
    // 检查会长退出公会不是最后一人
    if (guild->GetMemberNum() != 1) {
      LOG_WARNING("Leader can not leave guild.");
      SendMessageGuildLeaveResponseFailed(guild_actor);
      return;
    }
  } else {
    // 检查是否有离开权限
    const global::configure::GuildPositionCell *position_cell =
      GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
    if (NULL == position_cell) {
      LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
      SendMessageGuildLeaveResponseFailed(guild_actor);
      return;
    }
    if (!position_cell->authority_leave_guild_) {
      LOG_WARNING("No authority_leave_guild.");
      SendMessageGuildLeaveResponseFailed(guild_actor);
      return;
    }
  }

  // 退出公会
  if (guild->RemoveMember(guild_actor) == false) {
    LOG_ERROR("Remove GuildActor(%lu) from Guild(%lu) failed.",
              guild_actor->GetActorID(), guild->GetID());
    SendMessageGuildLeaveResponseFailed(guild_actor);
    return;
  }
  // 离开公会事件
  GuildEvent event(entity::GuildEventType::LEAVE_GUILD);
  event.PushEventVar(guild_actor->GetActorID());
  guild->AddEvent(event);
  // 离开公会公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_LEAVE);
  inform.params_.push_back(guild_actor->GetStringID());
  inform.params_.push_back(guild_actor->GetName());
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  if (guild->GetMemberNum() == 0) {
    GuildManager::GetInstance()->DismissGuild(guild->GetID());
  }

  gateway::protocol::MessageGuildLeaveResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_LEAVE_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildKickMemberRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildKickMemberRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildKickMemberRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_KICK_MEMBER_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }

  // 检查是否有踢人权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_kick_member_) {
    LOG_WARNING("No authority_kick_member.");
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }

  // 取被踢公会玩家对象
  GuildActor *target_guild_actor =
    GuildActorManager::GetInstance()->GetFromAll(atol(request.actor_id_.c_str()));
  if (NULL == target_guild_actor) {
    LOG_ERROR("Get target GuildActor(%s) from GuildActorManager failed.", request.actor_id_.c_str());
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }

  // 不能踢自己
  if (guild_actor == target_guild_actor) {
    LOG_WARNING("Can not kick self.");
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }

  // 是否是同一公会
  if (guild_actor->GetGuild() != target_guild_actor->GetGuild()) {
    LOG_WARNING("Not in same guild.");
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }

  // 只能踢出成员
  if (target_guild_actor->GetGuildPosition() != entity::GuildPositionType::MEMBER) {
    LOG_WARNING("Can only kick member position.");
    SendMessageGuildKickMemberResponseFailed(guild_actor);
    return;
  }

  // 踢出公会
  if (guild->RemoveMember(target_guild_actor) == false) {
    LOG_ERROR("Remove GuildActor(%lu) from Guild(%lu) failed.",
              target_guild_actor->GetActorID(), guild->GetID());
    return;
  }
  // 离开公会事件
  GuildEvent event(entity::GuildEventType::LEAVE_GUILD);
  event.PushEventVar(target_guild_actor->GetActorID());
  guild->AddEvent(event);
  // 离开公会公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_LEAVE);
  inform.params_.push_back(target_guild_actor->GetStringID());
  inform.params_.push_back(target_guild_actor->GetName());
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  // 给被踢的玩家发送通知
  gateway::protocol::MessageGuildKickMemberNotify notify;
  target_guild_actor->SendMessage(notify,
      gateway::protocol::MessageType::MESSAGE_GUILD_KICK_MEMBER_NOTIFY);

  gateway::protocol::MessageGuildKickMemberResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_KICK_MEMBER_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildGiveUpLeaderPositionRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildGiveUpLeaderPositionRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildGiveUpLeaderPositionRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_REQUEST from actor(%lu, %s), "
           "new_leader_actor_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.new_leader_actor_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildGiveUpLeaderPositionResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildGiveUpLeaderPositionResponseFailed(guild_actor);
    return;
  }

  // 检查是否是会长
  if (guild_actor->GetGuildPosition() != entity::GuildPositionType::LEADER) {
    LOG_WARNING("Not a leader.");
    SendMessageGuildGiveUpLeaderPositionResponseFailed(guild_actor);
    return;
  }

  // 取新会长公会玩家对象
  GuildActor *target_guild_actor =
    GuildActorManager::GetInstance()->GetFromAll(atol(request.new_leader_actor_id_.c_str()));
  if (NULL == target_guild_actor) {
    LOG_ERROR("Get target GuildActor(%s) from GuildActorManager failed.",
              request.new_leader_actor_id_.c_str());
    SendMessageGuildGiveUpLeaderPositionResponseFailed(guild_actor);
    return;
  }

  // 不能转让给自己
  if (guild_actor == target_guild_actor) {
    LOG_WARNING("New leader is same as the old one.");
    SendMessageGuildGiveUpLeaderPositionResponseFailed(guild_actor);
    return;
  }

  // 是否是同一公会
  if (guild_actor->GetGuild() != target_guild_actor->GetGuild()) {
    LOG_WARNING("Not in same guild.");
    SendMessageGuildGiveUpLeaderPositionResponseFailed(guild_actor);
    return;
  }

  // 转让会长
  guild->SetLeader(target_guild_actor);

  // 放弃会长职位事件
  GuildEvent event(entity::GuildEventType::GIVE_UP_LEADER_POSITION);
  event.PushEventVar(guild_actor->GetActorID());
  event.PushEventVar(target_guild_actor->GetActorID());
  guild->AddEvent(event);
  // 放弃会长职位公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_GIVE_UP_LEADER_POSITION);
  inform.params_.push_back(guild_actor->GetStringID());
  inform.params_.push_back(guild_actor->GetName());
  inform.params_.push_back(target_guild_actor->GetStringID());
  inform.params_.push_back(target_guild_actor->GetName());
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  gateway::protocol::MessageGuildGiveUpLeaderPositionResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildGiveUpPositionRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_GIVE_UP_POSITION_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildGiveUpPositionResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildGiveUpPositionResponseFailed(guild_actor);
    return;
  }

  // 检查玩家不是成员或者会长
  if (guild_actor->GetGuildPosition() == entity::GuildPositionType::LEADER ||
      guild_actor->GetGuildPosition() == entity::GuildPositionType::MEMBER) {
    LOG_WARNING("Leader or member can not give up position.");
    SendMessageGuildGiveUpPositionResponseFailed(guild_actor);
    return;
  }

  // 检查是否有放弃职位权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildGiveUpPositionResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_give_up_position_) {
    LOG_WARNING("No authority_give_up_position.");
    SendMessageGuildGiveUpPositionResponseFailed(guild_actor);
    return;
  }

  // 设置玩家职位为成员
  core::int32 old_position = guild_actor->GetGuildPosition();
  bool result = guild->SetGuildPosition(guild_actor, entity::GuildPositionType::MEMBER);

  // 放弃职位事件
  GuildEvent event(entity::GuildEventType::GIVE_UP_POSITION);
  event.PushEventVar(guild_actor->GetActorID());
  event.PushEventVar(old_position);
  guild->AddEvent(event);
  // 放弃职位公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_GIVE_UP_POSITION);
  inform.params_.push_back(guild_actor->GetStringID());
  inform.params_.push_back(guild_actor->GetName());
  inform.params_.push_back(global::ToString(old_position));
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  gateway::protocol::MessageGuildGiveUpPositionResponse response;
  response.__set_result_(result);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_GIVE_UP_POSITION_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildSetMemberPositionRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildSetMemberPositionRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildSetMemberPositionRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_SET_MEMBER_POSITION_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s], guild_position_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str(), request.guild_position_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 检查是否有设置职位权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_set_member_position_) {
    LOG_WARNING("No authority_set_member_position.");
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 取被设置职位的公会玩家对象
  GuildActor *target_guild_actor =
    GuildActorManager::GetInstance()->GetFromAll(atol(request.actor_id_.c_str()));
  if (NULL == target_guild_actor) {
    LOG_ERROR("Get target GuildActor(%s) from GuildActorManager failed.", request.actor_id_.c_str());
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 不能设置自己
  if (guild_actor == target_guild_actor) {
    LOG_WARNING("Can not set self position.");
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 是否是同一公会
  if (guild_actor->GetGuild() != target_guild_actor->GetGuild()) {
    LOG_WARNING("Not in same guild.");
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 玩家的职位必须高于被设置玩家的职位
  const global::configure::GuildPositionCell *target_position_cell =
    GUILD_CONF()->GetGuildPosition(target_guild_actor->GetGuildPosition());
  if (NULL == target_position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", target_guild_actor->GetGuildPosition());
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  if (position_cell->seq_ <= target_position_cell->seq_) {
    LOG_WARNING("Guild position is too low.");
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 设置的职位不能超过玩家自己的职位
  const global::configure::GuildPositionCell *target_new_position_cell =
    GUILD_CONF()->GetGuildPosition(request.guild_position_);
  if (NULL == target_new_position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", request.guild_position_);
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  if (position_cell->seq_ <= target_new_position_cell->seq_) {
    LOG_WARNING("Guild position is too low.");
    SendMessageGuildSetMemberPositionResponseFailed(guild_actor);
    return;
  }

  // 设置玩家职位
  bool result = guild->SetGuildPosition(target_guild_actor, request.guild_position_);

  if (request.guild_position_ != entity::GuildPositionType::MEMBER) {
    // 职位任命事件
    GuildEvent event(entity::GuildEventType::SET_MEMBER_POSITION);
    event.PushEventVar(guild_actor->GetActorID());
    event.PushEventVar(target_guild_actor->GetActorID());
    event.PushEventVar(request.guild_position_);
    guild->AddEvent(event);
    // 职位任命公告
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GUILD_SET_MEMBER_POSITION);
    inform.params_.push_back(guild_actor->GetStringID());
    inform.params_.push_back(guild_actor->GetName());
    inform.params_.push_back(target_guild_actor->GetStringID());
    inform.params_.push_back(target_guild_actor->GetName());
    inform.params_.push_back(global::ToString(request.guild_position_));
    guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  } else {
    // 职位解除任命事件
    GuildEvent event(entity::GuildEventType::RESET_MEMBER_POSITION);
    event.PushEventVar(guild_actor->GetActorID());
    event.PushEventVar(target_guild_actor->GetActorID());
    event.PushEventVar(request.guild_position_);
    guild->AddEvent(event);
    // 职位解除任命公告
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GUILD_RESET_MEMBER_POSITION);
    inform.params_.push_back(guild_actor->GetStringID());
    inform.params_.push_back(guild_actor->GetName());
    inform.params_.push_back(target_guild_actor->GetStringID());
    inform.params_.push_back(target_guild_actor->GetName());
    inform.params_.push_back(global::ToString(target_position_cell->id_));
    guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }

  gateway::protocol::MessageGuildSetMemberPositionResponse response;
  response.__set_result_(result);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SET_MEMBER_POSITION_RESPONSE);
} 

void GuildMessageHandler::OnMessageGuildEditAnnouncementRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildEditAnnouncementRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildEditAnnouncement failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_EDIT_ANNOUNCEMENT_REQUEST from actor(%lu, %s), "
           "guild_announcement_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.guild_announcement_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildEditAnnouncementResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildEditAnnouncementResponseFailed(guild_actor);
    return;
  }

  // 检查是否有设置公告权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildEditAnnouncementResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_edit_announcement_) {
    LOG_WARNING("No authority_edit_announcement.");
    SendMessageGuildEditAnnouncementResponseFailed(guild_actor);
    return;
  }

  // 检查公告长度
  ssize_t visual_length = global::GetStrVisualLengthUTF8(request.guild_announcement_.c_str());
  if (visual_length < 0 || visual_length > MISC_CONF()->guild_max_announcement_length_) {
    LOG_WARNING("Guild announcement is invalid.");
    SendMessageGuildEditAnnouncementResponseFailed(guild_actor);
    return;
  }

  // 关键字过滤
  SessionServerSingleton::GetInstance().GetWordFilter()->Block(&request.guild_announcement_);

  guild->SetAnnouncement(request.guild_announcement_);

  gateway::protocol::MessageGuildEditAnnouncementResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_EDIT_ANNOUNCEMENT_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有升级公会设施的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_upgrade_facility_) {
    LOG_WARNING("No authority_upgrade_facility.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否已经顶级
  const global::configure::GuildCell *next_level_cell =
    GUILD_CONF()->GetGuild(guild->GetLevel() + 1);
  if (NULL == next_level_cell) {
    LOG_WARNING("Guild is already in top level.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }

  const global::configure::GuildCell *level_cell =
    GUILD_CONF()->GetGuild(guild->GetLevel());
  if (NULL == level_cell) {
    LOG_ERROR("Guild level(%d) is not found in config.", guild->GetLevel());
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查公会设施是否已经升满
  if (guild->GetShopLevel() < level_cell->max_shop_level_ ||
      guild->GetCollegeLevel() < level_cell->max_college_level_ ||
      guild->GetBarrackLevel() < level_cell->max_barrack_level_) {
    LOG_WARNING("Guild facility is not in max level.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查升级消耗
  // --检查公会金币
  if (guild->GetGold() < level_cell->spend_gold_) {
    LOG_WARNING("Guild gold is not enough.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }
  // --检查光晶石
  if (guild->GetLightCrystal() < level_cell->spend_light_crystal_) {
    LOG_WARNING("Guild light crystal is not enough.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }
  // --检查暗晶石
  if (guild->GetDarkCrystal() < level_cell->spend_dark_crystal_) {
    LOG_WARNING("Guild dark crystal is not enough.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }
  // --检查圣晶石
  if (guild->GetHolyCrystal() < level_cell->spend_holy_crystal_) {
    LOG_WARNING("Guild holy crystal is not enough.");
    SendMessageGuildUpgradeResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  if (level_cell->spend_gold_ > 0) {
    guild->SetGold(guild->GetGold() - level_cell->spend_gold_);
  }
  if (level_cell->spend_light_crystal_ > 0) {
    guild->SetLightCrystal(guild->GetLightCrystal() - level_cell->spend_light_crystal_);
  }
  if (level_cell->spend_dark_crystal_ > 0) {
    guild->SetDarkCrystal(guild->GetDarkCrystal() - level_cell->spend_dark_crystal_);
  }
  if (level_cell->spend_holy_crystal_ > 0) {
    guild->SetHolyCrystal(guild->GetHolyCrystal() - level_cell->spend_holy_crystal_);
  }

  // 公会升级
  guild->SetLevel(guild->GetLevel() + 1);

  // 公会升级事件
  GuildEvent event(entity::GuildEventType::GUILD_UPGRADE);
  event.PushEventVar(guild->GetLevel());
  guild->AddEvent(event);
  // 公会升级公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_UPGRADE);
  inform.params_.push_back(global::ToString(guild->GetLevel()));
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  // 世界公告: 公会等级大于等于3
  if (guild->GetLevel() >= 3) {
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GUILD_HIGH_LEVEL);
    inform.params_.push_back(guild->GetStringID());
    inform.params_.push_back(guild->GetName());
    inform.params_.push_back(global::ToString(guild->GetLevel()));
    SessionServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }

  gateway::protocol::MessageGuildUpgradeResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_UPGRADE_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildShopUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildShopUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildShopUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_SHOP_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有升级公会设施的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_upgrade_facility_) {
    LOG_WARNING("No authority_upgrade_facility.");
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否已经顶级
  const global::configure::GuildShopCell *next_level_cell =
    GUILD_CONF()->GetGuildShop(guild->GetShopLevel() + 1);
  if (NULL == next_level_cell) {
    LOG_WARNING("Guild shop is already in top level.");
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否已到当前公会等级所限制的最高商店等级
  const global::configure::GuildCell *guild_level_cell =
    GUILD_CONF()->GetGuild(guild->GetLevel());
  if (NULL == guild_level_cell) {
    LOG_ERROR("Guild level(%d) is not found in config.", guild->GetLevel());
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }
  if (guild->GetShopLevel() >= guild_level_cell->max_shop_level_) {
    LOG_WARNING("GuildShop level(%d) is max for current guild level.", guild->GetShopLevel());
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查升级消耗
  // --检查公会金币
  if (guild->GetGold() < next_level_cell->spend_gold_) {
    LOG_WARNING("Guild gold is not enough.");
    SendMessageGuildShopUpgradeResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  if (next_level_cell->spend_gold_ > 0) {
    guild->SetGold(guild->GetGold() - next_level_cell->spend_gold_);
  }

  // 公会商店升级
  guild->SetShopLevel(guild->GetShopLevel() + 1);

  // 公会商店升级事件
  GuildEvent event(entity::GuildEventType::GUILD_SHOP_UPGRADE);
  event.PushEventVar(guild->GetShopLevel());
  guild->AddEvent(event);
  // 公会商店升级公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_SHOP_UPGRADE);
  inform.params_.push_back(global::ToString(guild->GetShopLevel()));
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  gateway::protocol::MessageGuildShopUpgradeResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_UPGRADE_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildCollegeUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildCollegeUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildCollegeUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_COLLEGE_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有升级公会设施的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_upgrade_facility_) {
    LOG_WARNING("No authority_upgrade_facility.");
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否已经顶级
  const global::configure::GuildCollegeCell *next_level_cell =
    GUILD_CONF()->GetGuildCollege(guild->GetCollegeLevel() + 1);
  if (NULL == next_level_cell) {
    LOG_WARNING("Guild college is already in top level.");
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否已到当前公会等级所限制的最高学院等级
  const global::configure::GuildCell *guild_level_cell =
    GUILD_CONF()->GetGuild(guild->GetLevel());
  if (NULL == guild_level_cell) {
    LOG_ERROR("Guild level(%d) is not found in config.", guild->GetLevel());
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }
  if (guild->GetCollegeLevel() >= guild_level_cell->max_college_level_) {
    LOG_WARNING("GuildCollege level(%d) is max for current guild level.", guild->GetCollegeLevel());
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查升级消耗
  // --检查公会金币
  if (guild->GetGold() < next_level_cell->spend_gold_) {
    LOG_WARNING("Guild gold is not enough.");
    SendMessageGuildCollegeUpgradeResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  if (next_level_cell->spend_gold_ > 0) {
    guild->SetGold(guild->GetGold() - next_level_cell->spend_gold_);
  }

  // 公会学院升级
  guild->SetCollegeLevel(guild->GetCollegeLevel() + 1);

  // 公会学院升级事件
  GuildEvent event(entity::GuildEventType::GUILD_COLLEGE_UPGRADE);
  event.PushEventVar(guild->GetCollegeLevel());
  guild->AddEvent(event);
  // 公会学院升级公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_COLLEGE_UPGRADE);
  inform.params_.push_back(global::ToString(guild->GetCollegeLevel()));
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  gateway::protocol::MessageGuildCollegeUpgradeResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_UPGRADE_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildCollegeLearnSkillRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildCollegeLearnSkillRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildCollegeLearnSkillRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_COLLEGE_LEARN_SKILL_REQUEST from actor(%lu, %s), "
           "guild_skill_id_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.guild_skill_id_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildCollegeLearnSkillResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildCollegeLearnSkillResponseFailed(guild_actor);
    return;
  }

  // 检查是否有学习公会技能的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildCollegeLearnSkillResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_college_lean_skill_) {
    LOG_WARNING("No authority_college_lean_skill.");
    SendMessageGuildCollegeLearnSkillResponseFailed(guild_actor);
    return;
  }

  core::int32 guild_skill_id = request.guild_skill_id_;
  core::int32 guild_skill_level =
    guild_actor->GetGuildSkillLevel(guild_skill_id);

  // 检查公会技能是否存在, 或者已经顶级
  const global::configure::GuildSkillCell *next_level_cell =
    GUILD_CONF()->GetGuildSkill(guild_skill_id, guild_skill_level + 1);
  if (NULL == next_level_cell) {
    LOG_WARNING("GuildSkill(%d) not exists or is already in top level.");
    SendMessageGuildCollegeLearnSkillResponseFailed(guild_actor);
    return;
  }

  // 检查学院等级
  if (guild->GetCollegeLevel() < next_level_cell->required_guild_college_level_) {
    LOG_WARNING("Guild college level is too low.");
    SendMessageGuildCollegeLearnSkillResponseFailed(guild_actor);
    return;
  }

  // 检查消耗
  // --检查贡献值
  if (guild_actor->GetCurrentContributionValue() < next_level_cell->spend_contribution_) {
    LOG_WARNING("Contribution is not enough.");
    SendMessageGuildCollegeLearnSkillResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  if (next_level_cell->spend_contribution_ > 0) {
    guild_actor->DelContribution(next_level_cell->spend_contribution_);
  }

  // 设置等级
  guild_actor->SetGuildSkillLevel(guild_skill_id, guild_skill_level + 1);

  gateway::protocol::MessageGuildCollegeLearnSkillResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_COLLEGE_LEARN_SKILL_RESPONSE);
}

static core::int32 GetRandomGuildBuffID(GuildActor *guild_actor, Guild *guild) {
  const global::configure::GuildConfigure::GuildBuffHashmap &guild_buffs =
    GUILD_CONF()->GetAllGuildBuff();
  std::vector<core::int32> guild_buff_pool;

  global::configure::GuildConfigure::GuildBuffHashmap::const_iterator iter =
    guild_buffs.begin();
  for (; iter != guild_buffs.end(); ++iter) {
    const global::configure::GuildBuffCell *cell = &iter->second;

    if (guild_actor->CheckGuildBuffExists(cell->id_)) {
      continue;
    }
    if (guild->GetLightAttribute() < cell->required_light_attribute_ ||
        guild->GetDarkAttribute() < cell->required_dark_attribute_ ||
        guild->GetHolyAttribute() < cell->required_holy_attribute_) {
      continue;
    }

    guild_buff_pool.push_back(cell->id_);
  }

  if (guild_buff_pool.empty()) {
    return -1;
  } else {
    std::random_shuffle(guild_buff_pool.begin(), guild_buff_pool.end());
    return guild_buff_pool[0];
  }
}

void GuildMessageHandler::OnMessageGuildIdolPrayRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildIdolPrayRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildIdolPrayRequest failed.");
    return;
  }
  
  LOG_INFO("Receive MESSAGE_GUILD_IDOL_PRAY_REQUEST from actor(%lu, %s), ",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 检查是否有神像祈祷的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_idol_pray_) {
    LOG_WARNING("No authority_idol_pray.");
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 随机出一个 BUFF_ID
  core::int32 guild_buff_id = GetRandomGuildBuffID(guild_actor, guild);
  if (-1 == guild_buff_id) {
    LOG_WARNING("can not get a random guild buff id.");
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 检查 BUFF 是否存在
  const global::configure::GuildBuffCell *buff_cell =
    GUILD_CONF()->GetGuildBuff(guild_buff_id);
  if (NULL == buff_cell) {
    LOG_WARNING("request.guild_buff_id_ is invalid.");
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 检查玩家是否已获得该 BUFF
  if (guild_actor->CheckGuildBuffExists(guild_buff_id)) {
    LOG_WARNING("Already get this GuildBuff(%d).", guild_buff_id);
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 检查公会属性是否满足
  if (guild->GetLightAttribute() < buff_cell->required_light_attribute_ ||
      guild->GetDarkAttribute() < buff_cell->required_dark_attribute_ ||
      guild->GetHolyAttribute() < buff_cell->required_holy_attribute_) {
    LOG_WARNING("Guild attribute is too low.");
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 检查消耗
  const global::configure::GuildPrayCell *pray_cell =
    GUILD_CONF()->GetGuildPray(guild_actor->GetGuildBuffNum() + 1);
  if (NULL == pray_cell) {
    LOG_ERROR("GuildPray(%d) not found in config.", guild_actor->GetGuildBuffNum() + 1);
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // --检查贡献值
  if (guild_actor->GetCurrentContributionValue() < pray_cell->spend_contribution_) {
    LOG_WARNING("Contribution is not enough.");
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  if (pray_cell->spend_contribution_ > 0) {
    guild_actor->DelContribution(pray_cell->spend_contribution_);
  }

  // 增加 BUFF
  if (guild_actor->AddGuildBuff(guild_buff_id) == false) {
    LOG_ERROR("Add GuildBuff(%d) failed.", guild_buff_id);
    SendMessageGuildIdolPrayResponseFailed(guild_actor);
    return;
  }

  gateway::protocol::MessageGuildIdolPrayResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_IDOL_PRAY_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildBarrackUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildBarrackUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildBarrackUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_BARRACK_UPGRADE_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false) {
    LOG_WARNING("GUILD_MODULE is disable.");
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否有升级公会设施的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_upgrade_facility_) {
    LOG_WARNING("No authority_upgrade_facility.");
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否已经顶级
  const global::configure::GuildBarrackCell *next_level_cell =
    GUILD_CONF()->GetGuildBarrack(guild->GetBarrackLevel() + 1);
  if (NULL == next_level_cell) {
    LOG_WARNING("Guild barrack is already in top level.");
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查是否已到当前公会等级所限制的最高军营等级
  const global::configure::GuildCell *guild_level_cell =
    GUILD_CONF()->GetGuild(guild->GetLevel());
  if (NULL == guild_level_cell) {
    LOG_ERROR("Guild level(%d) is not found in config.", guild->GetLevel());
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }
  if (guild->GetBarrackLevel() >= guild_level_cell->max_barrack_level_) {
    LOG_WARNING("GuildBarrack level(%d) is max for current guild level.", guild->GetBarrackLevel());
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }

  // 检查升级消耗
  // --检查公会金币
  if (guild->GetGold() < next_level_cell->spend_gold_) {
    LOG_WARNING("Guild gold is not enough.");
    SendMessageGuildBarrackUpgradeResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  if (next_level_cell->spend_gold_ > 0) {
    guild->SetGold(guild->GetGold() - next_level_cell->spend_gold_);
  }

  // 公会军营升级
  guild->SetBarrackLevel(guild->GetBarrackLevel() + 1);

  // 公会军营升级事件
  GuildEvent event(entity::GuildEventType::GUILD_BARRACK_UPGRADE);
  event.PushEventVar(guild->GetBarrackLevel());
  guild->AddEvent(event);
  // 公会军营升级公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_BARRACK_UPGRADE);
  inform.params_.push_back(global::ToString(guild->GetBarrackLevel()));
  guild->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  gateway::protocol::MessageGuildBarrackUpgradeResponse response;
  response.__set_result_(true);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_BARRACK_UPGRADE_RESPONSE);
}

void GuildMessageHandler::OnMessageGuildPlayingGroupOpenRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(id);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = guild_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGuildPlayingGroupOpenRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildPlayingGroupOpenRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_PLAYING_GROUP_OPEN_REQUEST from actor(%lu, %s), "
           "playing_group_id_=[%d].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.playing_group_id_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::GUILD_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::MULTI_PLAYING) == false) {
    LOG_WARNING("GUILD_MODULE or MULTI_PLAYING is disable.");
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("Not in a guild.");
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }

  // 检查是否有解封副本组的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }
  if (!position_cell->authority_open_playing_) {
    LOG_WARNING("No authority_open_playing.");
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }

  // 检查副本组是否已开启
  if (guild->CheckPlayingGroupOpened(request.playing_group_id_)) {
    LOG_WARNING("Playing group is already opened.");
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }

  const global::configure::GuildPlayingGroupCell *playing_group_cell =
    GUILD_CONF()->GetGuildPlayingGroup(request.playing_group_id_);
  if (NULL == playing_group_cell) {
    LOG_ERROR("GuildPlayingGroupCell(%d) not found in config.", request.playing_group_id_);
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }

  // 检查公会等级
  if (guild->GetLevel() < playing_group_cell->required_guild_level_) {
    LOG_WARNING("Guild level is not enough.");
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }

  // 检查消耗
  // --检查公会金币
  if (guild->GetGold() < playing_group_cell->spend_gold_) {
    LOG_WARNING("Guild gold is not enough.");
    SendMessageGuildPlayingGroupOpenResponseFailed(guild_actor);
    return;
  }

  // 扣除消耗
  if (playing_group_cell->spend_gold_ > 0) {
    guild->SetGold(guild->GetGold() - playing_group_cell->spend_gold_);
  }

  // 开启副本组
  guild->OpenPlayingGroup(request.playing_group_id_);
}

}  // namespace guild

}  // namespace server

}  // namespace session

