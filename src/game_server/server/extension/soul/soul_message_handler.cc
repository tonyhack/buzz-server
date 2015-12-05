//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-19 17:56:01.
// File name: soul_message_handler.cc
//
// Description:
// Define class SoulMessageHandler.
//

#include "game_server/server/extension/soul/soul_message_handler.h"

#include <boost/bind.hpp>

#include "entity/inform_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/facade_request.h"
#include "game_server/server/extension/soul/soul.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "game_server/server/extension/soul/soul_pool.h"
#include "game_server/server/event/game_soul_event_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_soul_protocol_types.h"
#include "global/global_packet.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace soul {

SoulMessageHandler::SoulMessageHandler() {}
SoulMessageHandler::~SoulMessageHandler() {}

bool SoulMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_UNLOCK_REQUEST,
      boost::bind(&SoulMessageHandler::OnMessageSoulUnlockRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_UPGRADE_STEP_REQUEST,
      boost::bind(&SoulMessageHandler::OnMessageSoulUpgradeStepRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_UPGRADE_LEVEL_REQUEST,
      boost::bind(&SoulMessageHandler::OnMessageSoulUpgradeLevelRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_SET_REQUEST,
      boost::bind(&SoulMessageHandler::OnMessageSoulSetRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_RESET_REQUEST,
      boost::bind(&SoulMessageHandler::OnMessageSoulResetRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_SETTING_STAND_REQUEST,
      boost::bind(&SoulMessageHandler::OnMessageSoulSettingStandRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_SKILL_UPGRADE_REQUEST,
      boost::bind(&SoulMessageHandler::OnMessageSoulSkillUpgradeRequest, this, _1, _2, _3));

  return true;
}

void SoulMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_UNLOCK_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_UPGRADE_STEP_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_UPGRADE_LEVEL_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_SET_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_RESET_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_SETTING_STAND_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_SKILL_UPGRADE_REQUEST);
}

void SoulMessageHandler::OnMessageSoulUnlockRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", id);
    return ;
  }

  // 模块是否开启
  if (actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return;
  }

  gateway::protocol::MessageSoulUnlockRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_SOUL_UNLOCK_REQUEST from actor(%lu, %s), soul_id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.soul_id_);

  // 获取英灵配置
  const SoulCell *soul_cell =
    Configure::GetInstance()->GetSoulCell(message.soul_id_);
  if (soul_cell == NULL) {
    LOG_ERROR("获取英灵配置(%d) 失败", message.soul_id_);
    return;
  }

  if (actor->GetSoul(message.soul_id_) != NULL) {
    LOG_ERROR("不能解锁已解锁过的英灵");
    return;
  }

  // 解锁条件
  if (soul_cell->unlock_type_ == entity::SoulUnlockType::ITEM) {
    core::int32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(id,
        soul_cell->unlock_param1_, soul_cell->unlock_param2_, entity::BindRequireType::BOTH);
    if (delete_number != soul_cell->unlock_param2_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，删除道具数量(%d) 不足(%d)",
          id, message.soul_id_, delete_number, soul_cell->unlock_param2_);
      return;
    }
  } else if (soul_cell->unlock_type_ == entity::SoulUnlockType::SPIRIT_LEVEL) {
    core::int32 spirit_level = FacadeRequest::GetInstance()->GetSpiritLevel(id);
    if (spirit_level < soul_cell->unlock_param1_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，spirit_level(%d) 不足(%d)",
          id, message.soul_id_, spirit_level, soul_cell->unlock_param1_);
      return;
    }
  } else if (soul_cell->unlock_type_ == entity::SoulUnlockType::SWORD_LEVEL) {
    core::int32 sword_level = FacadeRequest::GetInstance()->GetSwordStep(id);
    if (sword_level < soul_cell->unlock_param1_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，sword_level(%d) 不足(%d)",
          id, message.soul_id_, sword_level, soul_cell->unlock_param1_);
      return;
    }
  } else if (soul_cell->unlock_type_ == entity::SoulUnlockType::NOBLE_LEVEL) {
    core::int32 noble_level = FacadeRequest::GetInstance()->GetNobleLevel(id);
    if (noble_level < soul_cell->unlock_param1_) {
      LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，noble_level(%d) 不足(%d)",
          id, message.soul_id_, noble_level, soul_cell->unlock_param1_);
      return;
    }
  } else {
    LOG_ERROR("玩家(%lu) 英灵(%lu) 解锁时，解锁类型(%d) 错误",
        id, message.soul_id_, soul_cell->unlock_type_);
    return;
  }

  Soul *soul = SoulPool::GetInstance()->Allocate();
  if (soul == NULL) {
    LOG_ERROR("玩家(%lu) 分配 Soul(%lu) 失败", id, message.soul_id_);
    return;
  }

  if (soul->Initialize(actor, message.soul_id_) == false) {
    SoulPool::GetInstance()->Deallocate(soul);
    LOG_ERROR("玩家(%lu) Soul(%lu) 初始化失败", id, message.soul_id_);
    return;
  }

  soul->SetStep(0);
  soul->SetLevel(1);

  if (soul_cell->common_skill_ > 0) {
    FacadeRequest::GetInstance()->AddActorSkill(id, soul_cell->common_skill_, 1, false);
    soul->SetCommonSkillLevel(1);
  }
  if (soul_cell->appear_skill_ > 0) {
    FacadeRequest::GetInstance()->AddActorSkill(id, soul_cell->appear_skill_, 1, false);
    soul->SetAppearSkillLevel(1);
  }
  if (soul_cell->soul_skill_ > 0) {
    FacadeRequest::GetInstance()->AddActorSkill(id, soul_cell->soul_skill_, 1, false);
    soul->SetSoulSkillLevel(1);
  }

  if (actor->AddSoul(soul) == false) {
    soul->Finalize();
    SoulPool::GetInstance()->Deallocate(soul);
    LOG_ERROR("玩家(%lu) Soul(%lu) 加入列表失败", id, message.soul_id_);
    return;
  }

  gateway::protocol::MessageSoulUnlockResponse response;
  response.__set_result_(true);
  response.__set_soul_id_(message.soul_id_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_UNLOCK_RESPONSE);

  actor->UpdateSoulFightScore();

  soul->Synchronize();

  event::EventSoulAdd event;
  event.__set_actor_(actor->GetID());
  event.__set_soul_(soul->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOUL_ADD, actor->GetID(), event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));
}


void SoulMessageHandler::OnMessageSoulUpgradeStepRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  // 模块是否开启
  if (actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return;
  }

  gateway::protocol::MessageSoulUpgradeStepRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_SOUL_UPGRADE_STEP_REQUEST from actor(%lu, %s), soul_id(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.soul_id_);

  Soul *soul = actor->GetSoul(message.soul_id_);
  if (soul == NULL) {
    LOG_ERROR("获取玩家(%lu) Soul(%lu) 失败", id, message.soul_id_);
    return;
  }

  // 获取英灵配置
  const SoulCell *soul_cell =
    Configure::GetInstance()->GetSoulCell(soul->GetID());
  if (soul_cell == NULL) {
    LOG_ERROR("获取英灵配置(%d) 失败", soul->GetID());
    return;
  }

  core::int32 old_step = soul->GetStep();

  const SoulStepCell *soul_step_cell =
    Configure::GetInstance()->GetSoulStepCell(soul_cell->step_group_,
        soul->GetLevel(), old_step);
  if (soul_step_cell == NULL) {
    LOG_ERROR("获取 SoulStepCell(%d,%d,%d) 失败", soul_cell->step_group_,
        soul->GetLevel(), old_step);
    return;
  }
  const SoulStepCell *new_soul_step_cell =
    Configure::GetInstance()->GetSoulStepCell(soul_cell->step_group_,
        soul->GetLevel(), old_step + 1);
  if (new_soul_step_cell == NULL) {
    LOG_ERROR("获取 SoulStepCell(%d,%d,%d) 失败", soul_cell->step_group_,
        soul->GetLevel(), old_step + 1);
    return;
  }

  core::int32 current_nimbus = game_actor->GetAttribute(entity::ActorClientFields::NIMBUS);
  if (current_nimbus < soul_step_cell->spend_nimbus_) {
    LOG_ERROR("英灵升阶时，nimbus 不足");
    return;
  }

  game_actor->AddResource(entity::ResourceID::NIMBUS, 0 - soul_step_cell->spend_nimbus_);

  core::int32 random = ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  if (random >= soul_step_cell->odds_) {
    gateway::protocol::MessageSoulUpgradeStepResponse response;
    response.__set_result_(false);
    response.__set_soul_id_(message.soul_id_);
    actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_SOUL_UPGRADE_STEP_RESPONSE);
    return;
  }

  soul->SetStep(old_step + 1);

  for (SoulStepCell::AttrMap::const_iterator iterator = soul_step_cell->attrs_.begin();
      iterator != soul_step_cell->attrs_.end(); ++iterator) {
    soul->AddAttr(iterator->first, 0 - iterator->second);
  }
  for (SoulStepCell::AttrMap::const_iterator iterator = new_soul_step_cell->attrs_.begin();
      iterator != new_soul_step_cell->attrs_.end(); ++iterator) {
    soul->AddAttr(iterator->first, iterator->second);
  }

  gateway::protocol::MessageSoulUpgradeStepResponse response;
  response.__set_result_(true);
  response.__set_soul_id_(message.soul_id_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_UPGRADE_STEP_RESPONSE);

  actor->UpdateSoulFightScore();

  soul->Synchronize();

  event::EventSoulUpgradeStep event;
  event.__set_actor_(actor->GetID());
  event.__set_soul_(soul->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOUL_UPGRADE_STEP, actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void SoulMessageHandler::OnMessageSoulUpgradeLevelRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  // 模块是否开启
  if (actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return;
  }

  gateway::protocol::MessageSoulUpgradeLevelRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_SOUL_UPGRADE_LEVEL_REQUEST from actor(%lu, %s), soul_id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.soul_id_);

  Soul *soul = actor->GetSoul(message.soul_id_);
  if (soul == NULL) {
    LOG_ERROR("获取玩家(%lu) Soul(%lu) 失败", id, message.soul_id_);
    return;
  }

  core::int32 current_level = soul->GetLevel();

  // 获取英灵配置
  const SoulCell *soul_cell =
    Configure::GetInstance()->GetSoulCell(soul->GetID());
  if (soul_cell == NULL) {
    LOG_ERROR("获取英灵配置(%d) 失败", soul->GetID());
    return;
  }

  const SoulStepCell *soul_step_cell =
    Configure::GetInstance()->GetSoulStepCell(soul_cell->step_group_,
        current_level, soul->GetStep() + 1);
  if (soul_step_cell != NULL) {
    LOG_ERROR("英灵(%d) step(%d) 不足, 升级失败", soul->GetStep());
    return;
  }

  const SoulLevelCell *current_level_cell =
    Configure::GetInstance()->GetSoulLevelCell(soul_cell->level_group_,
        current_level);
  if (current_level_cell == NULL) {
    LOG_ERROR("获取 SoulLevelCell(%u,%u) 失败", soul_cell->level_group_,
        current_level);
    return;
  }

  const SoulLevelCell *next_level_cell =
    Configure::GetInstance()->GetSoulLevelCell(soul_cell->level_group_,
        current_level + 1);
  if (next_level_cell == NULL) {
    LOG_ERROR("获取 SoulLevelCell(%u,%u) 失败", soul_cell->level_group_,
        current_level);
    return;
  }

  core::int32 delete_item_num = FacadeRequest::GetInstance()->RemovePacketItem(id,
      current_level_cell->spend_item_, current_level_cell->spend_number_,
      entity::BindRequireType::BOTH);
  if (delete_item_num != current_level_cell->spend_number_) {
    LOG_ERROR("删除 玩家(%lu) 道具(%d) 个数(%d) 不足(%d)", id,
        current_level_cell->spend_item_, delete_item_num, current_level_cell->spend_number_);
    return;
  }

  soul->SetLevel(current_level + 1);
  soul->SetStep(0);

  gateway::protocol::MessageSoulUpgradeLevelResponse response;
  response.__set_result_(true);
  response.__set_soul_id_(message.soul_id_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_UPGRADE_LEVEL_RESPONSE);

  soul->Synchronize();

  if (Configure::GetInstance()->GetSoulLevelCell(soul_cell->level_group_,
        soul->GetLevel() + 1) == NULL) {
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::SOUL_TOP_LEVEL);
    inform.params_.push_back(game_actor->GetStringID());
    inform.params_.push_back(game_actor->GetName());
    inform.params_.push_back(global::ToString(soul->GetID()));
    inform.params_.push_back(global::ToString(soul->GetLevel()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }

  event::EventSoulUpgradeLevel event;
  event.__set_actor_(actor->GetID());
  event.__set_soul_(soul->GetID());
  event.__set_level_(soul->GetLevel());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOUL_UPGRADE_LEVEL, actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void SoulMessageHandler::OnMessageSoulSetRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  // 模块是否开启
  if (actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return;
  }

  // 战斗状态
  if(actor->GetFightingStatus() == true) {
    LOG_ERROR("玩家(%lu) 处于战斗状态中，操作失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", id);
    return ;
  }

  gateway::protocol::MessageSoulSetRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Rcevie MESSAGE_SOUL_SET_REQUEST from actor(%lu, %s), group_(%d), position_(%d), soul_id_(%d), vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.group_, message.position_, message.soul_id_, message.vocation_);

  Soul *soul = actor->GetSoul(message.soul_id_);
  if (soul == NULL) {
    LOG_ERROR("获取玩家(%lu) Soul(%lu) 失败", id, message.soul_id_);
    return;
  }

  const SoulCell *soul_cell =
    Configure::GetInstance()->GetSoulCell(message.soul_id_);
  if (soul_cell == NULL) {
    LOG_ERROR("获取英灵配置(%d) 失败", message.soul_id_);
    return;
  }

  if (soul_cell->vocation_ == entity::VocationType::MAX &&
      game_actor->CheckFunctionalityState(entity::FunctionalityType::SOUL_MODULE) == false) {
    LOG_ERROR("幻化槽功能未开启");
    return;
  }

  // 不能在幻化中调整幻化技能
  if (soul_cell->vocation_ == entity::VocationType::MAX &&
      game_actor->CheckCommonStatus(entity::RoleCommonStatus::SOUL) == true) {
    LOG_ERROR("玩家(%lu) 处于幻化状态 调整幻化技能失败", id);
    return;
  }

  if (soul_cell->vocation_ != entity::VocationType::MAX &&
      soul_cell->vocation_ != message.vocation_) {
    LOG_ERROR("设置英灵职业(%d) 不符合(%d)", message.vocation_, soul_cell->vocation_);
    return;
  }

  if ((message.group_ == 0 && soul_cell->appear_skill_ <= 0) ||
      (message.group_ != 0 && soul_cell->appear_skill_ > 0)) {
    LOG_ERROR("设置英灵时 group_(%d) appear_skill(%d) 不符",
        message.group_, soul_cell->appear_skill_);
    return;
  }

  SoulSetting *setting = actor->GetSetting(message.vocation_, message.group_);
  if (setting == NULL) {
    LOG_ERROR("获取 SoulSetting(%d,%d) 失败", message.vocation_, message.group_);
    return;
  }

  if (setting->SetSoul(soul, message.position_) == false) {
    LOG_ERROR("设置 setting->SetSoul(%d,%d) 失败", message.soul_id_, message.position_);
    return;
  }

  gateway::protocol::MessageSoulSetResponse response;
  response.__set_result_(true);
  response.__set_vocation_(message.vocation_);
  response.__set_group_(message.group_);
  response.__set_soul_id_(message.soul_id_);
  response.__set_position_(message.position_);
  actor->SendMessage(response, gateway::protocol::MessageType::MESSAGE_SOUL_SET_RESPONSE);

  event::EventSoulSet event;
  event.__set_actor_(actor->GetID());
  event.__set_soul_(message.soul_id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOUL_SET, actor->GetID(), event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));
}

void SoulMessageHandler::OnMessageSoulResetRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", id);
    return ;
  }

  // 模块是否开启
  if (actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return;
  }

  // 战斗状态
  if(actor->GetFightingStatus() == true) {
    LOG_ERROR("玩家(%lu) 处于战斗状态中，操作失败", id);
    return ;
  }

  gateway::protocol::MessageSoulResetRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_SOUL_RESET_REQUEST from actor(%lu, %s), group_(%d), position_(%d), vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.group_, message.position_, message.vocation_);

  SoulSetting *setting = actor->GetSetting(message.vocation_, message.group_);
  if (setting == NULL) {
    LOG_ERROR("获取 SoulSetting(%d,%d) 失败", message.vocation_, message.group_);
    return;
  }

  if (message.vocation_ == entity::VocationType::MAX &&
      game_actor->CheckFunctionalityState(entity::FunctionalityType::SOUL_SETING) == false) {
    LOG_ERROR("幻化槽功能未开启");
    return;
  }

  // 不能在幻化中调整幻化技能
  if (message.vocation_ == entity::VocationType::MAX &&
      game_actor->CheckCommonStatus(entity::RoleCommonStatus::SOUL) == true) {
    LOG_ERROR("玩家(%lu) 处于幻化状态 调整幻化技能失败", id);
    return;
  }

  if (setting->ResetSoul(message.position_) == false) {
    LOG_ERROR("当前 setting ResetSoul(%d,%d,%d) 失败",
        message.vocation_, message.group_, message.position_);
    return;
  }

  gateway::protocol::MessageSoulResetResponse response;
  response.__set_result_(true);
  response.__set_vocation_(message.vocation_);
  response.__set_group_(message.group_);
  response.__set_position_(message.position_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_RESET_RESPONSE);
}

void SoulMessageHandler::OnMessageSoulSettingStandRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  // 模块是否开启
  if (actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return;
  }

  // 战斗状态
  if(actor->GetFightingStatus() == true) {
    LOG_ERROR("玩家(%lu) 处于战斗状态中，操作失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessageSoulSettingStandRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_SOUL_SETTING_STAND_REQUEST from actor(%lu, %s), group_(%d), vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.group_, message.vocation_);

  if (message.group_ == 0) {
    LOG_ERROR("message.group_ == 0, stand 失败");
    return;
  }

  SoulSetting *setting = actor->GetSetting(message.vocation_, message.group_);
  if (setting == NULL) {
    LOG_ERROR("获取 SoulSetting(%d,%d) 失败", message.vocation_, message.group_);
    return;
  }

  SoulSetting *current_setting = actor->GetCurrentSetting();
  if (current_setting == NULL) {
    LOG_ERROR("获取 current_setting 失败");
    return;
  }

  if (current_setting->Sit() == false) {
    LOG_ERROR("current_setting sit 失败");
    return;
  }

  if (setting->Stand() == false) {
    LOG_ERROR("SoulSetting(%d,%d) stand 失败", message.vocation_, message.group_);
    return;
  }

  // 幻化设置
  entity::VocationType::type old_vocation =
    (entity::VocationType::type)game_actor->GetAttribute(entity::RoleAoiFields::VOCATION);
  game_actor->SetAttribute(entity::RoleAoiFields::VOCATION, message.vocation_);
  if (old_vocation != message.vocation_) {
    SoulSetting *old_appear_setting = actor->GetSetting(old_vocation, 0);
    if (old_appear_setting) {
      old_appear_setting->Sit();
    }
    SoulSetting *appear_setting = actor->GetSetting(message.vocation_, 0);
    if (appear_setting) {
      appear_setting->Stand();
    }
  }

  actor->SetCurrentSetting(message.vocation_, message.group_);

  gateway::protocol::MessageSoulSettingStandResponse response;
  response.__set_result_(true);
  response.__set_vocation_(message.vocation_);
  response.__set_group_(message.group_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_SETTING_STAND_RESPONSE);
}

void SoulMessageHandler::OnMessageSoulSkillUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *actor = SoulActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  // 模块是否开启
  if (actor->CheckSoulFunctionStatus() == false) {
    LOG_ERROR("请求增加英灵时，英灵功能未开启");
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessageSoulSkillUpgradeRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_SOUL_SKILL_UPGRADE_REQUEST from actor(%lu, %s), skill_id_(%d), soul_id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.skill_id_, message.soul_id_);

  Soul *soul = actor->GetSoul(message.soul_id_);
  if (soul == NULL) {
    LOG_ERROR("获取 Soul(%d) 失败", message.soul_id_);
    return;
  }

  // 获取英灵配置
  const SoulCell *soul_cell =
    Configure::GetInstance()->GetSoulCell(message.soul_id_);
  if (soul_cell == NULL) {
    LOG_ERROR("获取英灵配置(%d) 失败", message.soul_id_);
    return;
  }

  core::int32 current_skill_level = 0;

  if (message.skill_id_ == soul_cell->common_skill_) {
    current_skill_level = soul->GetCommonSkillLevel();
  } else if (message.skill_id_ == soul_cell->appear_skill_) {
    current_skill_level = soul->GetAppearSkillLevel();
  } else if (message.skill_id_ == soul_cell->soul_skill_) {
    current_skill_level = soul->GetSoulSkillLevel();
  } else {
    LOG_ERROR("英灵(%d) 技能(%d) 不匹配",
        message.soul_id_, message.skill_id_);
    return;
  }

  if (soul->GetMaxSkillLevel() <= current_skill_level) {
    LOG_ERROR("英灵(%d) 技能(%d) 已为当前最高等级",
        message.soul_id_, message.skill_id_);
    return;
  }

  const SoulSkillLevelCell *soul_skill_level_cell =
    Configure::GetInstance()->GetSoulSkillLevel(current_skill_level);
  if (soul_skill_level_cell == NULL) {
    LOG_ERROR("获取 SoulLevelCell(%d) 失败", current_skill_level);
    return;
  }

  const SoulSkillLevelCell *next_soul_skill_level_cell = 
    Configure::GetInstance()->GetSoulSkillLevel(current_skill_level + 1);
  if (next_soul_skill_level_cell == NULL) {
    LOG_ERROR("获取 SoulLevelCell(%d) 失败", current_skill_level + 1);
    return;
  }

  core::int32 current_nimbus =
    game_actor->GetAttribute(entity::ActorClientFields::NIMBUS);
  core::int32 current_gold =
    game_actor->GetAttribute(entity::ActorClientFields::GOLD);

  if (current_nimbus < soul_skill_level_cell->spend_nimbus_ ||
      current_gold < soul_skill_level_cell->spend_gold_) {
    LOG_ERROR("消耗(nimbus=%d, gold=%d) 不足(%d,%d)", current_nimbus, current_gold,
        soul_skill_level_cell->spend_nimbus_, soul_skill_level_cell->spend_gold_);
    return;
  }

  game_actor->AddResource(entity::ResourceID::NIMBUS,
      0 - soul_skill_level_cell->spend_nimbus_);
  game_actor->AddResource(entity::ResourceID::GOLD,
      0 - soul_skill_level_cell->spend_gold_);

  if (FacadeRequest::GetInstance()->UpgradeActorSkill(id,
        message.skill_id_, 1) == false) {
    LOG_ERROR("升级 玩家(%lu) 英灵(%d) 技能(%d) 当前等级(%d) 失败",
        id, message.soul_id_, message.skill_id_, current_skill_level);
    return;
  }

  if (message.skill_id_ == soul_cell->common_skill_) {
    soul->SetCommonSkillLevel(current_skill_level + 1);
  } else if (message.skill_id_ == soul_cell->appear_skill_) {
    soul->SetAppearSkillLevel(current_skill_level + 1);
  } else if (message.skill_id_ == soul_cell->soul_skill_) {
    soul->SetSoulSkillLevel(current_skill_level + 1);
  } else {
    LOG_ERROR("英灵(%d) 技能(%d) 不匹配",
        message.soul_id_, message.skill_id_);
    return;
  }

  gateway::protocol::MessageSoulSkillUpgradeResponse response;
  response.__set_result_(true);
  response.__set_soul_id_(message.soul_id_);
  response.__set_skill_id_(message.skill_id_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_SKILL_UPGRADE_RESPONSE);

  actor->UpdateSoulFightScore();

  soul->Synchronize();
}

}  // namespace soul

}  // namespace server

}  // namespace game

