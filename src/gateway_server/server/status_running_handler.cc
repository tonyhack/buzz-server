//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 15:01:41.
// File name: status_running_handler.cc
//
// Description:
// Define class StatusRunningHandler.
//

#include "gateway_server/server/status_running_handler.h"

#include "game_server/client/game_client.h"
#include "game_server/protocol/game_protocol_types.h"
#include "gateway_server/server/gateway_user.h"
#include "gateway_server/server/session_channel.h"
#include "session_server/protocol/session_gateway_protocol_types.h"

namespace gateway {

namespace server {

StatusRunningHandler::StatusRunningHandler() {}
StatusRunningHandler::~StatusRunningHandler() {}

bool StatusRunningHandler::Initialize(GatewayUser *user) {
  if(StatusHandler::Initialize(user) == false) {
    global::LogError("%s:%d (%s) StatusHandler initialize failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void StatusRunningHandler::Finalize() {
  StatusHandler::Finalize();
}

UserStatus::Type StatusRunningHandler::GetStatus() {
  return UserStatus::RUNNING_STATUS;
}

void StatusRunningHandler::Start() {
}

void StatusRunningHandler::End() {
}

void StatusRunningHandler::GotoNextStatus() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
}

void StatusRunningHandler::OnMessage(const char *message, size_t size) {
  global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Get thrift packet failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message2;
  if(thrift->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(message2.type_) {
    case protocol::MessageType::MESSAGE_LOGOUT_REQUEST:
      this->OnMessageLogoutRequest(message2.message_.data(), message2.message_.size());
      break;
    case protocol::MessageType::MESSAGE_ACTOR_MOVE:
    case protocol::MessageType::MESSAGE_ACTOR_FUNCTIONALITY_OPEN_REQUEST:
    case protocol::MessageType::MESSAGE_ACTOR_USE_TASLIMAN_REQUEST:
    case protocol::MessageType::MESSAGE_ITEM_MOVE:
    case protocol::MessageType::MESSAGE_ITEM_REMOVE:
    case protocol::MessageType::MESSAGE_ITEM_USE:
    case protocol::MessageType::MESSAGE_ITEM_SPLIT:
    case protocol::MessageType::MESSAGE_ITEM_ARRANGE:
    case protocol::MessageType::MESSAGE_VIP_GIFTS_PICK_REQUEST:
    case protocol::MessageType::MESSAGE_EQUIP_COMPARE_REQUEST:
    case protocol::MessageType::MESSAGE_EXTEND_CONTAINER:
    case protocol::MessageType::MESSAGE_ITEM_COMPOSE_REQUEST:
    case protocol::MessageType::MESSAGE_EQUIP_INTENSIFY_REQUEST:
    case protocol::MessageType::MESSAGE_EQUIP_REFINE_REQUEST:
    case protocol::MessageType::MESSAGE_EQUIP_REFINE_CONFIRM_REQUEST:
    case protocol::MessageType::MESSAGE_EQUIP_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_EQUIP_INHERIT_REQUEST:
    case protocol::MessageType::MESSAGE_GEM_GATHER_REQUEST:
    case protocol::MessageType::MESSAGE_GEM_SKIP_GATHERING_POINT_REQUEST:
    case protocol::MessageType::MESSAGE_GEM_MOVE_REQUEST:
    case protocol::MessageType::MESSAGE_GEM_PROMOTE_REQUEST:
    case protocol::MessageType::MESSAGE_GEM_DECOMPOSE_REQUEST:
    case protocol::MessageType::MESSAGE_GEM_LOCK_REQUEST:
    case protocol::MessageType::MESSAGE_GEM_LOCK_ALL_REQUEST:
    case protocol::MessageType::MESSAGE_SHORTCUT_SET_REQUEST:
    case protocol::MessageType::MESSAGE_GM_ADD_ITEM:
    case protocol::MessageType::MESSAGE_GM_SCENE_JUMP:
    case protocol::MessageType::MESSAGE_GM_OPEN_SWORD:
    case protocol::MessageType::MESSAGE_GM_COMMAND:
    case protocol::MessageType::MESSAGE_SCENE_REQUEST_JUMP:
    case protocol::MessageType::MESSAGE_SCENE_GATHER:
    case protocol::MessageType::MESSAGE_SCENE_PICK_UP_REQUEST:
    case protocol::MessageType::MESSAGE_SCENE_JUMP_REQUEST:
    case protocol::MessageType::MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_REQUEST:
    case protocol::MessageType::MESSAGE_SCENE_ZAZEN_REQUEST:
    case protocol::MessageType::MESSAGE_SCENE_CANCEL_ZAZEN_REQUEST:
    case protocol::MessageType::MESSAGE_SKILL_OPERATE:
    case protocol::MessageType::MESSAGE_SKILL_HIT:
    case protocol::MessageType::MESSAGE_SOUL_UNLOCK_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_UPGRADE_STEP_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_UPGRADE_LEVEL_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_SET_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_RESET_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_SETTING_STAND_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_SKILL_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_ALTAR_SUMMON_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_ALTAR_TAKE_REQUEST:
    case protocol::MessageType::MESSAGE_SOUL_ALTAR_RESET_REQUEST:
    case protocol::MessageType::MESSAGE_ACTOR_CHOOSE_PK_MODE:
    case protocol::MessageType::MESSAGE_TASK_ACCEPT:
    case protocol::MessageType::MESSAGE_TASK_FINISH:
    case protocol::MessageType::MESSAGE_TASK_ABANDON:
    case protocol::MessageType::MESSAGE_TASK_TALK_NPC:
    case protocol::MessageType::MESSAGE_TASK_CIRCLE_OPT_REQUEST:
    case protocol::MessageType::MESSAGE_TASK_TRIP_OPT_REQUEST:
    case protocol::MessageType::MESSAGE_TASK_SUMMON_NPC_REQUEST:
    case protocol::MessageType::MESSAGE_TASK_DAILY_REQUEST:
    case protocol::MessageType::MESSAGE_TASK_DAILY_AWARD_REQUEST:
    case protocol::MessageType::MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_REQUEST:
    case protocol::MessageType::MESSAGE_TASK_TRANSPORT_ACCEPT_REQUEST:
    case protocol::MessageType::MESSAGE_TASK_TRANSPORT_FINISH_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_NOBLE_PROMOTE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_NOBLE_BREAKTHROUGH_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_NOBLE_COMPARE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_SWORD_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_SWORD_EQUIP:
    case protocol::MessageType::MESSAGE_ATTR_SWORD_UNEQUIP:
    case protocol::MessageType::MESSAGE_ATTR_SWORD_COMPARE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_ELEMENT_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_SPIRIT_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_SPIRIT_COMPARE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_RUNE_COMPOSE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_RUNE_MOVE_REQUEST:
    case protocol::MessageType::MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_REQUEST:
    case protocol::MessageType::MESSAGE_CHAT_REQUEST:
    case protocol::MessageType::MESSAGE_MAIL_DEL_REQUEST:
    case protocol::MessageType::MESSAGE_MAIL_READ_REQUEST:
    case protocol::MessageType::MESSAGE_MAIL_PICK_AFFIX_REQUEST:
    case protocol::MessageType::MESSAGE_MALL_BUY_REQUEST:
    case protocol::MessageType::MESSAGE_MAIL_SEND_REQUEST:
    case protocol::MessageType::MESSAGE_SHOP_BUYBACK_REQUEST:
    case protocol::MessageType::MESSAGE_SHOP_BUY_REQUEST:
    case protocol::MessageType::MESSAGE_SHOP_SELL_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_CREATE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_DONATE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_PLAYING_AWARD_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_CREATE_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_COMPLETE_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_LEAVE_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_AWARD_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_AUTO_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_AUTO_FINISH_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_PAID_AWARD_REQUEST:
    case protocol::MessageType::MESSAGE_MAZE_MOVE_REQUEST:
    case protocol::MessageType::MESSAGE_MAZE_CONFIRM_MOVE_REQUEST:
    case protocol::MessageType::MESSAGE_MAZE_AWARD_REQUEST:
    case protocol::MessageType::MESSAGE_BAR_FINISH_REQUEST:
    case protocol::MessageType::MESSAGE_ACHIEVE_AWARD_REQUEST:
    case protocol::MessageType::MESSAGE_ACHIEVE_CHANGE_TITLE_REQUEST:
    case protocol::MessageType::MESSAGE_CHECKINS_OPT_REQUEST:
    case protocol::MessageType::MESSAGE_ONLINE_AWARD_ITEM_REQUEST:
    case protocol::MessageType::MESSAGE_ONLINE_AWARD_RESOURCE_REQUEST:
    case protocol::MessageType::MESSAGE_ITEM_TREASURE_HUNT_REQUEST:
      this->OnMessageForward(message2);
      break;
    case protocol::MessageType::MESSAGE_ACTOR_FACADE_REQUEST:
    case protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_REQUEST:
    case protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_ADD_REQUEST:
    case protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_REMOVE_REQUEST:
    case protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_MOVE_REQUEST:
    case protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_RECOMMEND_REQUEST:
    case protocol::MessageType::MESSAGE_SOCIAL_ENEMY_ADD_REQUEST:
    case protocol::MessageType::MESSAGE_SOCIAL_ENEMY_REMOVE_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_VISIT_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_POOL_HARVEST_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_CHURCH_PRAY_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TREE_WATER_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TREE_STEAL_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TREE_HARVEST_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST:
    case protocol::MessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_CREATE_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_JOIN_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_JOIN_DISPOSE_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_INVITE_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_INVITE_DISPOSE_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_DISMISS_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_QUIT_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_KICKOUT_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_SET_CAPTAIN_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_MAP_TEAMS_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_MAP_ACTORS_REQUEST:
    case protocol::MessageType::MESSAGE_TEAM_MEMBER_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_LIST_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_SUPPRESS_JOIN_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_JOIN_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_CANCEL_JOIN_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_AUDIT_PENDING_MEMBER_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_INVITE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_INVITE_DISPOSE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_LEAVE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_KICK_MEMBER_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_GIVE_UP_POSITION_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_SET_MEMBER_POSITION_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_EDIT_ANNOUNCEMENT_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_SHOP_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_COLLEGE_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_COLLEGE_LEARN_SKILL_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_IDOL_PRAY_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_BARRACK_UPGRADE_REQUEST:
    case protocol::MessageType::MESSAGE_GUILD_PLAYING_GROUP_OPEN_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_ENTER_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_TEAM_LIST_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_TEAM_CREATE_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_TEAM_JOIN_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_TEAM_QUIT_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_TEAM_READY_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_TEAM_KICKOUT_REQUEST:
    case protocol::MessageType::MESSAGE_PLAYING_TEAM_CAPACITY_SET_REQUEST:
    case protocol::MessageType::MESSAGE_AUCTION_QUERY_TRANSACTION_REQUEST:
    case protocol::MessageType::MESSAGE_AUCTION_SEARCH_REQUEST:
    case protocol::MessageType::MESSAGE_AUCTION_SELL_REQUEST:
    case protocol::MessageType::MESSAGE_AUCTION_CANCEL_SELL_REQUEST:
    case protocol::MessageType::MESSAGE_AUCTION_BID_REQUEST:
    case protocol::MessageType::MESSAGE_AUCTION_BUYOUT_REQUEST:
    case protocol::MessageType::MESSAGE_AUCTION_WITHDRAW_REQUEST:
    case protocol::MessageType::MESSAGE_SCENE_BRANCH_SWITCH_REQUEST:
    case protocol::MessageType::MESSAGE_SCENE_BRANCH_STATUS_REQUEST:
      this->OnMessageForwardSession(message2);
      break;
    default:
      global::LogError("%s:%d (%s) Error message type[%d]",
          __FILE__, __LINE__, __FUNCTION__, message2.type_);
      break;
  }
}

void StatusRunningHandler::OnDisconnect() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  } 

  if(SessionChannel::CheckSessionConnection() == true) {
    session::protocol::GateMessageUserLogout message;
    message.__set_actor_id_(this->user_->GetActorID().GetID());
    SessionChannel::SendMessage(message,
        session::protocol::GatewayMessageType::MESSAGE_USER_LOGOUT);
  } else {
    game::client::GameSessionPtr session =
      game::client::GameClientSingleton::GetInstance().GetSession(this->user_->GetGameServer());
    if(session.get() == NULL) {
      global::LogError("%s:%d (%s) Game session pointer is null.",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    game::protocol::MessageUserLogout message;
    message.__set_actor_id_(this->user_->GetActorID().GetID());
    session->SendMessage(message,
        game::protocol::MessageType::MESSAGE_USER_LOGOUT);
  }
}

void StatusRunningHandler::OnMessageLogoutRequest(const char *message, size_t size) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(SessionChannel::CheckSessionConnection() == true) {
    // Notice session logout.
    session::protocol::GateMessageUserLogout logout_message;
    logout_message.__set_actor_id_(this->user_->GetActorID().GetID());
    SessionChannel::SendMessage(logout_message,
        session::protocol::GatewayMessageType::MESSAGE_USER_LOGOUT);
  } else {
    game::client::GameSessionPtr session =
      game::client::GameClientSingleton::GetInstance().GetSession(this->user_->GetGameServer());
    if(session.get() == NULL) {
      global::LogError("%s:%d (%s) Game session pointer is null.",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    game::protocol::MessageUserLogout message;
    message.__set_actor_id_(this->user_->GetActorID().GetID());
    session->SendMessage(message,
        game::protocol::MessageType::MESSAGE_USER_LOGOUT);
  }

  this->GotoNextStatus();
}

void StatusRunningHandler::OnMessageForward(protocol::Message &message) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;              
  }

  game::client::GameSessionPtr session =
    game::client::GameClientSingleton::GetInstance().GetSession(this->user_->GetGameServer());

  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Game session pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  game::protocol::MessageUserMessageForward forward;
  forward.__set_message_(message.message_);
  forward.__set_type_(message.type_);
  forward.__set_actor_(this->user_->GetActorID().GetID());

  session->SendMessage(forward, game::protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD);
}

void StatusRunningHandler::OnMessageForwardSession(protocol::Message &message) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  session::protocol::MessageUserMessageForward forward;
  forward.__set_message_(message.message_);
  forward.__set_type_(message.type_);
  forward.__set_actor_(this->user_->GetActorID().GetID());

  session::client::SessionClientPtr session =
    GatewayServerSingleton::GetInstance().GetSessionClient();
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) session is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  session->SendMessage(forward,
      session::protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD);
}

}  // namespace server

}  // namespace gateway

