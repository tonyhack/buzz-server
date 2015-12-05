#include "game_server/server/extension/attr/spirit_message_handler.h"

#include <boost/bind.hpp>

#include "entity/inform_types.h"
#include "global/types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "game_server/server/game_server.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/facade_request.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/spirit_actor.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "game_server/server/request/game_bar_request_types.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace attr {

SpiritMessageHandler::SpiritMessageHandler() {}
SpiritMessageHandler::~SpiritMessageHandler() {}

bool SpiritMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_UPGRADE_REQUEST,
      boost::bind(&SpiritMessageHandler::OnMessageAttrSpiritUpgradeRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_REQUEST,
      boost::bind(&SpiritMessageHandler::OnMessageAttrSpiritTransformFacadeRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_COMPARE_REQUEST,
      boost::bind(&SpiritMessageHandler::OnMessageAttrSpiritCompareRequest, this, _1, _2, _3));

  return true;
}

void SpiritMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_UPGRADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_COMPARE_REQUEST);
}

static void SendMessageAttrSpiritUpgradeResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrSpiritUpgradeResponse response;
  response.__set_result_(false);
  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_UPGRADE_RESPONSE);
}

static void SendMessageAttrSpiritTransformFacadeResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrSpiritTransformFacadeResponse response;
  response.__set_result_(false);
  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_RESPONSE);
}

static void SendMessageAttrSpiritCompareResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrSpiritCompareResponse response;
  response.__set_result_(false);
  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_COMPARE_RESPONSE);
}

void SpiritMessageHandler::OnMessageAttrSpiritUpgradeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  SpiritActor *spirit_actor = attr_actor->GetSpiritActor();

  // 消息解析
  gateway::protocol::MessageAttrSpiritUpgradeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrSpiritUpgradeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_SPIRIT_UPGRADE_REQUEST from actor(%lu, %s)",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false ||
      game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_UPGRADE) == false) {
    LOG_WARNING("SPIRIT_MODULE or SPIRIT_UPGRADE is disable.");
    SendMessageAttrSpiritUpgradeResponseFailed(attr_actor);
    return;
  }

  const SpiritCell *current_cell =
    Configure::GetInstance()->GetSpirit(spirit_actor->GetCurrentLevel());
  if (NULL == current_cell) {
    LOG_ERROR("SpiritCell(%d) not found in config.", spirit_actor->GetCurrentLevel());
    SendMessageAttrSpiritUpgradeResponseFailed(attr_actor);
    return;
  }

  const SpiritCell *next_cell =
    Configure::GetInstance()->GetSpirit(spirit_actor->GetCurrentLevel() + 1);
  if (NULL == next_cell) {
    LOG_WARNING("Spirit is top level.");
    SendMessageAttrSpiritUpgradeResponseFailed(attr_actor);
    return;
  }

  // 检查消耗
  // --检查道具
  if (FacadeRequest::GetInstance()->CheckPacketItemExist(id,
          current_cell->upgrade_spend_item_id_, current_cell->upgrade_spend_item_count_,
          BindType::BOTH) == false) {
    LOG_WARNING("Item(%u) is not enough.", current_cell->upgrade_spend_item_id_);
    SendMessageAttrSpiritUpgradeResponseFailed(attr_actor);
    return;
  }

  // 扣除消耗
  core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(id,
      current_cell->upgrade_spend_item_id_, current_cell->upgrade_spend_item_count_,
      BindType::BOTH);
  if (delete_number != (core::uint32)current_cell->upgrade_spend_item_count_) {
    LOG_ERROR("Remove item(%d) failed, req_count=[%d], del_count=[%d].",
              current_cell->upgrade_spend_item_id_, current_cell->upgrade_spend_item_count_,
              delete_number);
    SendMessageAttrSpiritUpgradeResponseFailed(attr_actor);
    return;
  }

  // 计算本次升阶成功率
  // 成功率 = (当前幸运 - int(幸运上限*系数)) / (幸运上限 - int(幸运上限*系数))
  core::int32 odds = 0;
  core::int32 f = current_cell->max_lucky_ * current_cell->lucky_coe_ / global::kPercentDenom;
  if (current_cell->max_lucky_ - f != 0) {
    odds = (spirit_actor->GetLuckyValue() - f) * global::kPercentDenom /
        (current_cell->max_lucky_ - f);
    if (odds < 0) {
      odds = 0;
    }
  }
  odds += current_cell->upgrade_odds_;

  core::int32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  LOG_DEBUG("SPIRIT_UPGRADE odds=%d random=%d", odds, random);

  // 进阶失败, 增加幸运值
  if (random >= odds) {
    if (spirit_actor->GetLuckyValue() < current_cell->max_lucky_) {
      core::int32 new_lucky_value = std::min(current_cell->max_lucky_,
          random % current_cell->lucky_ + 1 + spirit_actor->GetLuckyValue());
      spirit_actor->SetLuckyValue(new_lucky_value);
    }
    SendMessageAttrSpiritUpgradeResponseFailed(attr_actor);
    return;
  }

  // 进阶成功
  spirit_actor->SetCurrentLevel(spirit_actor->GetCurrentLevel() + 1);

  if (spirit_actor->GetCurrentLevel() >= 6 && spirit_actor->GetCurrentLevel() <= 9) {
    // 世界公告: 精灵升级到高等级(6~9级)
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::ATTR_SPIRIT_HIGH_LEVEL);
    inform.params_.push_back(attr_actor->GetStringID());
    inform.params_.push_back(attr_actor->GetName());
    inform.params_.push_back(global::ToString(spirit_actor->GetCurrentLevel()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
    
  } else if (Configure::GetInstance()->GetSpirit(spirit_actor->GetCurrentLevel() + 1) == NULL) {
    // 世界公告: 精灵升到顶级
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::ATTR_SPIRIT_TOP_LEVEL);
    inform.params_.push_back(attr_actor->GetStringID());
    inform.params_.push_back(attr_actor->GetName());
    inform.params_.push_back(global::ToString(spirit_actor->GetCurrentLevel()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }

  gateway::protocol::MessageAttrSpiritUpgradeResponse response;
  response.__set_result_(true);
  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_UPGRADE_RESPONSE);

  // 发送进阶事件
  event::EventAttrSpiritUpgrade event_level;
  event_level.__set_actor_id_(id);
  event_level.__set_old_level_(spirit_actor->GetCurrentLevel() - 1);
  event_level.__set_new_level_(spirit_actor->GetCurrentLevel());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ATTR_SPIRIT_UPGRADE, id, 
      event::ChannelType::CHANNEL_ACTOR, &event_level, sizeof(event_level));
}

void SpiritMessageHandler::OnMessageAttrSpiritTransformFacadeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  SpiritActor *spirit_actor = attr_actor->GetSpiritActor();

  // 消息解析
  gateway::protocol::MessageAttrSpiritTransformFacadeRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrSpiritTransformFacadeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_REQUEST from actor(%lu, %s), facade_id_=[%d]",
           game_actor->GetID(), game_actor->GetName().c_str(), message.facade_id_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    LOG_WARNING("SPIRIT_MODULE is disable.");
    SendMessageAttrSpiritTransformFacadeResponseFailed(attr_actor);
    return;
  }

  // 请求进度条
  request::RequestBarStart request;
  request.__set_actor_id_(id);
  request.__set_bar_id_(MISC_CONF()->spirit_bar_id_);
  request.__set_type_(entity::BarModuleType::BAR_TYPE_SPIRIT_TRANSFORMED);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BAR_START, &request, sizeof(request)) != 0) {
    LOG_ERROR("安装进度条失败[%d]", MISC_CONF()->spirit_bar_id_);
    return ;
  }
  spirit_actor->SetFacadeID(message.facade_id_);
}

void SpiritMessageHandler::OnMessageAttrSpiritCompareRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  SpiritActor *spirit_actor = attr_actor->GetSpiritActor();

  // 消息解析
  gateway::protocol::MessageAttrSpiritCompareRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrSpiritCompareRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_SPIRIT_COMPARE_REQUEST from actor(%lu, %s), actor_id_=[%s]",
           game_actor->GetID(), game_actor->GetName().c_str(), request.actor_id_.c_str());

  // 获取被比较者属性玩家对象
  core::uint64 compare_attr_actor_id = atol(request.actor_id_.c_str());
  AttrActor *compare_attr_actor = AttrActorManager::GetInstance()->Get(compare_attr_actor_id);
  if (NULL == compare_attr_actor) {
    LOG_WARNING("Get Compare AttrActor(%lu) from AttrActorManager failed.",
        compare_attr_actor_id);
    SendMessageAttrSpiritCompareResponseFailed(attr_actor);
    return;
  }
  SpiritActor *compare_spirit_actor = compare_attr_actor->GetSpiritActor();
  RuneActor *rune_actor = compare_attr_actor->GetRuneActor();

  gateway::protocol::MessageAttrSpiritCompareResponse response;
  response.__set_result_(true);
  response.__set_actor_id_(request.actor_id_);
  response.__set_level_(compare_spirit_actor->GetCurrentLevel());
  response.__set_current_facade_type_(
      (entity::SpiritFacadeType::type)compare_spirit_actor->GetCurrentFacadeType());
  response.__set_horse_facade_id_(compare_spirit_actor->GetHorseFacadeID());
  response.__set_wing_facade_id_(compare_spirit_actor->GetWingFacadeID());
  std::vector<gateway::protocol::SpiritRuneData> val;
  rune_actor->GetRuneSlots(val);
  response.__set_runes_(val);
  for (size_t i = 0; i < compare_spirit_actor->GetFacades().size(); ++i) {
    const SpiritFacade &facade = compare_spirit_actor->GetFacades()[i];
    if (facade.CheckExist() == false) {
      continue;
    }

    gateway::protocol::SpiritFacadeData data;
    data.__set_id_(facade.GetID());
    data.__set_expire_time_(facade.GetExpireTime());

    response.facades_.push_back(data);
  }
  response.__isset.facades_ = true;

  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_COMPARE_RESPONSE);
}

}  // namespace attr

}  // namespace server

}  // namespace game

