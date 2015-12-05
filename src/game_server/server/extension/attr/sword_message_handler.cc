//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-08 10:30:45.
// File name: sword_message_handler.cc
//
// Description:
// Define SwordMessageHandler.
//

#include "game_server/server/extension/attr/sword_message_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/inform_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/facade_request.h"
#include "game_server/server/extension/attr/sword_actor.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/common_functions.h"
#include "global/types.h"

namespace game {

namespace server {

namespace attr {

SwordMessageHandler::SwordMessageHandler() {}
SwordMessageHandler::~SwordMessageHandler() {}

bool SwordMessageHandler::Initialize() {
  // MESSAGE_ATTR_SWORD_UPGRADE_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_UPGRADE_REQUEST,
      boost::bind(&SwordMessageHandler::OnMessageUpgradeRequest, this, _1, _2, _3));

  // MESSAGE_ATTR_SWORD_EQUIP
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_EQUIP,
      boost::bind(&SwordMessageHandler::OnMessageEquip, this, _1, _2, _3));

  // MESSAGE_ATTR_SWORD_UNEQUIP
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_UNEQUIP,
      boost::bind(&SwordMessageHandler::OnMessageUnequip, this, _1, _2, _3));

  // MESSAGE_ATTR_SWORD_COMPARE_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_COMPARE_REQUEST,
      boost::bind(&SwordMessageHandler::OnMessageCompareRequest, this, _1, _2, _3));

  return true;
}

void SwordMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_UPGRADE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_EQUIP);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_UNEQUIP);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_COMPARE_REQUEST);
}

void SwordMessageHandler::OnMessageUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  // 获取 AttrActor 对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if(attr_actor == NULL) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return ;
  }

  GameActor *game_actor = attr_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return ;
  }

  SwordActor *sword_actor = attr_actor->GetSwordActor();
  if(sword_actor == NULL) {
    LOG_ERROR("SwordActor(%lu) is null.", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageAttrSwordUpgradeRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrSwordUpgradeRequest failed.");
    return ;
  }

  LOG_INFO("Receive MESSAGE_ATTR_SWORD_UPGRADE_REQUEST actor(%lu, %s)",
      game_actor->GetID(), game_actor->GetName().c_str());

  // 获取当前魔剑的阶级
  core::int32 sword_step = sword_actor->GetAvailStep();

  // 获取当前阶级的魔剑配置
  const SwordCell *old_cell = Configure::GetInstance()->GetSword(sword_step);
  if(old_cell == NULL) {
    LOG_WARNING("Avail sword step[%u] is invalid.", sword_step);
    this->SendUpgradeFailed(attr_actor);
    return ;
  }

  // 获取下一级魔剑配置
  const SwordCell *new_cell = Configure::GetInstance()->GetSword(sword_step + 1);
  if(new_cell == NULL) {
    LOG_WARNING("Upgrade sword step[%u] is invalid.", sword_step + 1);
    this->SendUpgradeFailed(attr_actor);
    return ;
  }

  // 判断道具是否足够
  if(FacadeRequest::GetInstance()->CheckPacketItemExist(id, old_cell->spend_item_,
        old_cell->spend_item_number_, BindType::BOTH) == false) {
    LOG_WARNING("item[%u] number[%u] not enough", old_cell->spend_item_,
        old_cell->spend_item_number_);
    return ;
  }

  // 获取当前游戏币数量
  core::int32 gold = game_actor->GetAttribute(entity::ActorClientFields::GOLD);
  if(gold < old_cell->spend_gold_) {
    this->SendUpgradeFailed(attr_actor);
    return ;
  }

  // 扣道具
  core::uint32 number = 0;
  if((number = FacadeRequest::GetInstance()->RemovePacketItem(id, old_cell->spend_item_,
          old_cell->spend_item_number_, BindType::BOTH)) !=
      (core::uint32)old_cell->spend_item_number_) {
    LOG_WARNING("item[%u] number[%u] remove_number[%u]", old_cell->spend_item_,
        old_cell->spend_item_number_, number);
    return ;
  }

  // 扣游戏币
  game_actor->AddResource(entity::ResourceID::GOLD, 0 - old_cell->spend_gold_);

  // 成功率=(当前幸运-int(幸运上限*系数))/(幸运上限-int(幸运上限*系数))
  // 幸运值小时幸运上限*3/4时，成功率小于零，取值零
  core::int32 lucky_value = sword_actor->GetLuckyValue();
  core::int32 mid_value = old_cell->max_lucky_ * old_cell->lucky_coe_ / global::kPercentDenom;
  core::int32 numerator = lucky_value - mid_value;
  core::int32 denom = old_cell->max_lucky_ - mid_value;
  core::int32 odds = 0;
  if(denom != 0) {
    odds = numerator * global::kPercentDenom / denom;
    if(odds < 0) {
      odds = 0;
    }
  }
  odds += old_cell->upgrade_odds_;

  core::int32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  LOG_DEBUG("SWORD_UPGRADE odds=%d random=%d", odds, random);

  // 随机失败
  if(random >= odds) {
    if(lucky_value < old_cell->max_lucky_) {
      lucky_value = random % old_cell->lucky_;
      lucky_value = sword_actor->GetLuckyValue() +
        (lucky_value ? lucky_value : old_cell->lucky_);
      if(lucky_value > old_cell->max_lucky_) {
        lucky_value = old_cell->max_lucky_;
      }
      sword_actor->SetLuckyValue(lucky_value);
      // 同步幸运值
      gateway::protocol::MessageAttrSwordSynchronize sync;
      sync.__set_lucky_value_(lucky_value);
      attr_actor->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_SYNCHRONIZE);
    }
    // 发送升阶失败
    this->SendUpgradeFailed(attr_actor);
    return ;
  }

  // 设置升阶时间
  sword_actor->SetGainTime(GlobalTimeTick::GetInstance()->TimeTick().GetRealSec());

  // 设置当前最大阶数
  sword_actor->SetAvailStep(sword_step + 1);

  // 设置为装备当前最大阶数
  sword_actor->SetCurrentStep(sword_step + 1);
  // 更新战斗力
  sword_actor->UpdateSwordFightScore();

  sword_actor->SetLuckyValue(0);


  // 升阶后开始过期定时器
  sword_actor->StartExpireTimer();

  // 同步最大阶数
  gateway::protocol::MessageAttrSwordSynchronize sync;
  sync.__set_lucky_value_(0);
  sync.__set_step_(sword_step + 1);
  attr_actor->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_SYNCHRONIZE);

  // 升阶成功
  gateway::protocol::MessageAttrSwordUpgradeResponse response;
  response.__set_result_(true);
  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_UPGRADE_RESPONSE);

  if (Configure::GetInstance()->GetSword(sword_actor->GetCurrentStep() + 1) == NULL) {
    // 世界公告: 激活魔剑(神武)
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::ATTR_SWORD_TOP_LEVEL);
    inform.params_.push_back(attr_actor->GetStringID());
    inform.params_.push_back(attr_actor->GetName());
    inform.params_.push_back(global::ToString(sword_actor->GetCurrentStep()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  } else {
    // 世界公告: 激活魔剑(神武)
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::ATTR_SWORD_UPGRADE_LEVEL);
    inform.params_.push_back(attr_actor->GetStringID());
    inform.params_.push_back(attr_actor->GetName());
    inform.params_.push_back(global::ToString(sword_actor->GetCurrentStep()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }

  // 发送升阶事件
  event::EventAttrSwordUpgrade event;
  event.__set_actor_id_(id);
  event.__set_old_step_(sword_step);
  event.__set_new_step_(sword_step + 1);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ATTR_SWORD_UPGRADE, id, 
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void SwordMessageHandler::OnMessageEquip(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  // 获取 AttrActor 对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if(attr_actor == NULL) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return ;
  }

  GameActor *game_actor = attr_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return ;
  }

  SwordActor *sword_actor = attr_actor->GetSwordActor();
  if(sword_actor == NULL) {
    LOG_ERROR("SwordActor(%lu) is null.", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageAttrSwordEquip message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrSwordEquip failed.");
    return ;
  }

  LOG_INFO("Receive MESSAGE_ATTR_SWORD_EQUIP actor(%lu, %s) step(%u)",
      game_actor->GetID(), game_actor->GetName().c_str(), message.step_);

  if(message.step_ <= 0) {
    LOG_WARNING("Equip step[%d] invalid.", message.step_);
  }

  // 不能大于最高阶级
  core::int32 avail_step = sword_actor->GetAvailStep();
  if(avail_step < message.step_) {
    LOG_WARNING("Equip step[%d] invalid, avail step[%d].",
        message.step_, avail_step);
    return ;
  }

  // 如果是当前阶级，则不变
  if(avail_step == sword_actor->GetCurrentStep()) {
    LOG_WARNING("Have equiped step[%d]", message.step_);
    return ;
  }

  // 获取当前最高阶级配置
  const SwordCell *cell = Configure::GetInstance()->GetSword(avail_step);
  if(cell == NULL) {
    LOG_WARNING("Step[%d] not found in config.");
    return ;
  }

  // 是否失效
  if(cell->active_hours_ > 0) {
    time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    if(sword_actor->GetGainTime() + cell->active_hours_ * 60 * 60 < now) {
      LOG_WARNING("Actor[%lu] sword out of available time.", id);
      return ;
    }
  }

  // 设置为当前阶级
  sword_actor->SetCurrentStep(message.step_);
  // 更新战斗力
  sword_actor->UpdateSwordFightScore();
  sword_actor->StartExpireTimer();
}

void SwordMessageHandler::OnMessageUnequip(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  // 获取 AttrActor 对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if(attr_actor == NULL) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return ;
  }

  SwordActor *sword_actor = attr_actor->GetSwordActor();
  if(sword_actor == NULL) {
    LOG_ERROR("SwordActor(%lu) is null.", id);
    return ;
  }

  LOG_INFO("Receive MESSAGE_ATTR_SWORD_UNEQUIP actor(%lu)",
      attr_actor->GetID());

  sword_actor->SetCurrentStep(0);
  // 更新战斗力
  sword_actor->UpdateSwordFightScore();
}

void SwordMessageHandler::OnMessageCompareRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  // 获取 AttrActor 对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if(attr_actor == NULL) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageAttrSwordCompareRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrSwordCompareRequest failed.");
    return ;
  }

  // 获取被比较者 AttrActor 对象
  AttrActor *compare_attr_actor = AttrActorManager::GetInstance()->Get(
      atol(message.actor_id_.c_str()));
  if(compare_attr_actor == NULL) {
    LOG_ERROR("Get compared AttrActor(%s) failed", message.actor_id_.c_str());
    return ;
  }

  // 获取被比较者 SwordActor 对象
  SwordActor *compare_sword_actor = compare_attr_actor->GetSwordActor();
  if(compare_attr_actor == NULL) {
    LOG_ERROR("Get compared SwordActor(%s) failed", message.actor_id_.c_str());
    return ;
  }

  LOG_INFO("Receive MESSAGE_ATTR_SWORD_COMPARE_REQUEST actor(%lu)",
      attr_actor->GetID());

  gateway::protocol::MessageAttrSwordCompareResponse response;
  response.__set_step_(compare_sword_actor->GetAvailStep());
  response.__set_level_(compare_sword_actor->GetLevel());

  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_COMPARE_RESPONSE);
}

void SwordMessageHandler::SendUpgradeFailed(AttrActor *actor) {
  if(actor == NULL) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  gateway::protocol::MessageAttrSwordUpgradeResponse message;
  message.__set_result_(false);

  actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_UPGRADE_RESPONSE);
}

}  // namespace attr

}  // namespace server

}  // namespace game

