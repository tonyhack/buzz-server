#include "session_server/server/extension/social/homeland_battle_message_handler.h"

#include <boost/bind.hpp>

#include "entity/homeland_types.h"
#include "global/types.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_social_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"
#include "session_server/server/extension/social/homeland_actor.h"
#include "session_server/server/extension/social/homeland_event.h"
#include "session_server/server/extension/social/homeland_battle_manager.h"

namespace session {

namespace server {

namespace social {

HomelandBattleMessageHandler::HomelandBattleMessageHandler() {}
HomelandBattleMessageHandler::~HomelandBattleMessageHandler() {}

bool HomelandBattleMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST,
      boost::bind(&HomelandBattleMessageHandler::OnMessageHomelandBattleMatchRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST,
      boost::bind(&HomelandBattleMessageHandler::OnMessageHomelandBattleStrikeBackRequest,
          this, _1, _2, _3));

  return true;
}

void HomelandBattleMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST);
}

static void SendMessageHomelandBattleMatchResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandBattleMatchResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_MATCH_RESPONSE);
}

static void SendMessageHomelandBattleStrikeBackResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageHomelandBattleStrikeBackResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_RESPONSE);
}

static bool DoHomelandGoldMineRobbery(
    SocialActor *attacker_social_actor, SocialActor *defender_social_actor,
    HomelandBattleResult *battle_result, core::int32 *gold_robbed) {
  HomelandActor *attacker_homeland_actor = attacker_social_actor->GetHomelandActor();
  HomelandActor *defender_homeland_actor = defender_social_actor->GetHomelandActor();

  const global::configure::HomelandGoldmineCell *cell =
    SOCIAL_CONF()->GetHomelandGoldmine(defender_homeland_actor->GetGoldmineLevel());
  if (NULL == cell) {
    LOG_ERROR("HomelandGoldmineCell(%d) not found in config.",
              defender_homeland_actor->GetGoldmineLevel());
    return false;
  }

  // 检查掠夺次数
  if (attacker_homeland_actor->GetGoldmineRobTimes() >=
      MISC_CONF()->homeland_max_goldmine_rob_times_) {
    LOG_WARNING("Attacker's goldmine rob times is full.");
    return false;
  }
  // 检查掠夺冷却时间
  if (attacker_homeland_actor->GetGoldmineRobCooling() != 0) {
    LOG_WARNING("Attacker in goldmine rob cooling.");
    return false;
  }
  // 进行战斗
  if (HomelandBattleManager::GetInstance()->DoBattle(
          attacker_social_actor, defender_social_actor, battle_result) == false) {
    LOG_ERROR("Do HomelandBattle (%lu vs %lu) failed.",
              attacker_social_actor->GetActorID(), defender_social_actor->GetActorID());
    return false;
  }
  // 增加掠夺次数
  attacker_homeland_actor->AddGoldmineRobTimes();
  // 设置掠夺冷却时间
  attacker_homeland_actor->SetGoldmineRobCooling(MISC_CONF()->homeland_goldmine_rob_cooling_);

  if (battle_result->result_ != entity::HomelandBattleResultType::LOSE) {
    // 计算损失的金币数量
    core::int32 loss_rate = 0;
    if (entity::HomelandBattleResultType::TINY_WIN == battle_result->result_) {
      loss_rate = cell->tiny_win_gold_loss_rate_;
    } else if (entity::HomelandBattleResultType::GREAT_WIN == battle_result->result_) {
      loss_rate = cell->great_win_gold_loss_rate_;
    } else if (entity::HomelandBattleResultType::PERFECT_WIN == battle_result->result_) {
      loss_rate = cell->perfect_win_gold_loss_rate_;
    }
    *gold_robbed = cell->harvest_gold_reward_ *  loss_rate / 100;

    // 增加被掠夺次数
    defender_homeland_actor->AddGoldmineRobbedTimes();
    // 增加金矿损失比例
    defender_homeland_actor->AddGoldmineLossRate(loss_rate);
    // 加入侵入者列表
    defender_homeland_actor->AddInvader(attacker_social_actor->GetActorID());

    // 攻防战进攻成功事件
    {
      HomelandEvent event(entity::HomelandEventType::BATTLE_ATTACK_SUCCESS);
      event.PushEventVar(defender_social_actor->GetActorID());
      event.PushEventVar(*gold_robbed);
      attacker_homeland_actor->AddEvent(event);
    }
    // 攻防战防守失败事件
    {
      HomelandEvent event(entity::HomelandEventType::BATTLE_DEFENCE_FAILED);
      event.PushEventVar(attacker_social_actor->GetActorID());
      event.PushEventVar(*gold_robbed);
      defender_homeland_actor->AddEvent(event);
    }
  } else {
    // 攻防战进攻失败事件
    {
      HomelandEvent event(entity::HomelandEventType::BATTLE_ATTACK_FAILED);
      event.PushEventVar(defender_social_actor->GetActorID());
      attacker_homeland_actor->AddEvent(event);
    }
    // 攻防战防守成功事件
    {
      HomelandEvent event(entity::HomelandEventType::BATTLE_DEFENCE_SUCCESS);
      event.PushEventVar(attacker_social_actor->GetActorID());
      defender_homeland_actor->AddEvent(event);
    }
  }

  return true;
}

static void HomelandBattleMatchRequest(core::uint64 actor_id, core::uint64 target_actor_id) {
  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", actor_id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", actor_id);
    return;
  }

  // 获取目标社交玩家对象
  SocialActor *target_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(target_actor_id);
  if (NULL == target_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", target_actor_id);
    SendMessageHomelandBattleMatchResponseFailed(social_actor);
    return;
  }

  // 掠夺金矿
  HomelandBattleResult battle_result;
  core::int32 gold_robbed = 0;
  if (DoHomelandGoldMineRobbery(social_actor, target_social_actor,
          &battle_result, &gold_robbed) == false) {
    SendMessageHomelandBattleMatchResponseFailed(social_actor);
    return;
  }

  // 保存离线数据
  if (target_social_actor->CheckOnline() == false) {
    target_social_actor->Save();
  }

  // 发送成功回复
  session::protocol::MessageHomelandBattleMatchReqeust game_request;
  game_request.__set_actor_id_(actor_id);
  game_request.__set_target_actor_name_(target_social_actor->GetName());
  game_request.__set_gold_(gold_robbed);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST);
}

void HomelandBattleMessageHandler::OnMessageHomelandBattleMatchRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandBattleMatchRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandBattleMatchRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST from actor(%lu, %s).",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_GOLDMINE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_GOLDMINE is disable.");
    SendMessageHomelandBattleMatchResponseFailed(social_actor);
    return;
  }

  // 取匹配的玩家
  core::uint64 target_actor_id =
    HomelandBattleManager::GetInstance()->GetBattleTarget(social_actor->GetActorID());
  if (0 == target_actor_id) {
    LOG_WARNING("Do not found battle target for Actor(%lu).", social_actor->GetActorID());
    SendMessageHomelandBattleMatchResponseFailed(social_actor);
    return;
  }

  if (SocialActorManager::GetInstance()->GetFromAll(target_actor_id) == NULL) {
    // 离线加载
    SocialActorManager::GetInstance()->OfflineLoad(target_actor_id,
        boost::bind(&HomelandBattleMatchRequest, id, target_actor_id));
  } else {
    HomelandBattleMatchRequest(id, target_actor_id);
  }
}

static void HomelandBattleStrikeBackRequest(core::uint64 actor_id, core::uint64 target_actor_id) {
  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", actor_id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", actor_id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 获取目标社交玩家对象
  SocialActor *target_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(target_actor_id);
  if (NULL == target_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", target_actor_id);
    SendMessageHomelandBattleStrikeBackResponseFailed(social_actor);
    return;
  }
  HomelandActor *target_homeland_actor = social_actor->GetHomelandActor();

  // 检查目标是否在侵入者列表
  if (homeland_actor->IsInvader(target_actor_id) == false) {
    LOG_WARNING("Actor(%lu) is not a invader for Actor(%lu).", target_actor_id, actor_id);
    SendMessageHomelandBattleStrikeBackResponseFailed(social_actor);
    return;
  }

  // 检查目标是否可掠夺
  if (target_homeland_actor->CheckGoldmineCanRob() == false) {
    LOG_WARNING("Can not rob Actor(%lu).", target_actor_id);
    SendMessageHomelandBattleStrikeBackResponseFailed(social_actor);
    return;
  }

  // 掠夺金矿
  HomelandBattleResult battle_result;
  core::int32 gold_robbed = 0;
  if (DoHomelandGoldMineRobbery(social_actor, target_social_actor,
          &battle_result, &gold_robbed) == false) {
    SendMessageHomelandBattleStrikeBackResponseFailed(social_actor);
    return;
  }

  // 将目标玩家从侵入者列表中删除
  if (battle_result.result_ != entity::HomelandBattleResultType::LOSE) {
    homeland_actor->RemoveInvader(target_actor_id);
  }
 
  // 保存离线数据
  if (target_social_actor->CheckOnline() == false) {
    target_social_actor->Save();
  }

  // 转发 game_server
  session::protocol::MessageHomelandBattleStrikeBackRequest game_request;
  game_request.__set_actor_id_(actor_id);
  game_request.__set_gold_(gold_robbed);
  social_actor->SendGameMessage(game_request,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST);
}

void HomelandBattleMessageHandler::OnMessageHomelandBattleStrikeBackRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  HomelandActor *homeland_actor = social_actor->GetHomelandActor();

  // 消息解析
  gateway::protocol::MessageHomelandBattleStrikeBackRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageHomelandBattleStrikeBackRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_MODULE) == false ||
      session_actor->CheckFunctionalityState(entity::FunctionalityType::HOMELAND_GOLDMINE) == false) {
    LOG_WARNING("HOMELAND_MODULE or HOMELAND_GOLDMINE is disable.");
    SendMessageHomelandBattleStrikeBackResponseFailed(social_actor);
    return;
  }

  core::uint64 target_actor_id = atol(request.actor_id_.c_str());

  // 检查 target_actor_id 是否存在
  if (SessionActorManager::GetInstance()->GetBriefActor(target_actor_id) == NULL) {
    LOG_WARNING("target_actor_id(%lu) is invalid.", target_actor_id);
    SendMessageHomelandBattleStrikeBackResponseFailed(social_actor);
    return;
  }

  if (SocialActorManager::GetInstance()->GetFromAll(target_actor_id) == NULL) {
    // 离线加载
    SocialActorManager::GetInstance()->OfflineLoad(target_actor_id,
        boost::bind(&HomelandBattleStrikeBackRequest, id, target_actor_id));
  } else {
    HomelandBattleStrikeBackRequest(id, target_actor_id);
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

