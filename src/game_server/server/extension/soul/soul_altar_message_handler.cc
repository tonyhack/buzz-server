#include "game_server/server/extension/soul/soul_altar_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_soul_protocol_types.h"
#include "game_server/server/extension/soul/facade_request.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_altar_actor.h"

namespace game {

namespace server {

namespace soul {

SoulAltarMessageHandler::SoulAltarMessageHandler() {}
SoulAltarMessageHandler::~SoulAltarMessageHandler() {}

bool SoulAltarMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SUMMON_REQUEST,
      boost::bind(&SoulAltarMessageHandler::OnMessageSoulAltarSummonRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_TAKE_REQUEST,
      boost::bind(&SoulAltarMessageHandler::OnMessageSoulAltarTakeRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_RESET_REQUEST,
      boost::bind(&SoulAltarMessageHandler::OnMessageSoulAltarResetRequest, this, _1, _2, _3));

  return true;
}

void SoulAltarMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SUMMON_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_TAKE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_RESET_REQUEST);
}

static void SendMessageSoulAltarSummonResponseFailed(SoulActor *soul_actor) {
  gateway::protocol::MessageSoulAltarSummonResponse response;
  response.__set_result_(false);
  soul_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SUMMON_RESPONSE);
}

static void SendMessageSoulAltarTakeResponseFailed(SoulActor *soul_actor) {
  gateway::protocol::MessageSoulAltarTakeResponse response;
  response.__set_result_(false);
  soul_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_TAKE_RESPONSE);
}

static void SendMessageSoulAltarResetResponseFailed(SoulActor *soul_actor) {
  gateway::protocol::MessageSoulAltarResetResponse response;
  response.__set_result_(false);
  soul_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_RESET_RESPONSE);
}

void SoulAltarMessageHandler::OnMessageSoulAltarSummonRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(id);
  if (NULL == soul_actor) {
    LOG_ERROR("Get SoulActor(%lu) from SoulActorManager failed.", id);
    return;
  }
  GameActor *game_actor = soul_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  SoulAltarActor *soul_altar_actor = soul_actor->GetSoulAltarActor();

  // 消息解析
  gateway::protocol::MessageSoulAltarSummonRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSoulAltarSummonRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOUL_ALTAR_SUMMON_REQUEST from actor(%lu, %s), bind_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(), request.bind_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SOUL_ALTAR) == false) {
    LOG_WARNING("SOUL_ALTAR is disable.");
    SendMessageSoulAltarSummonResponseFailed(soul_actor);
    return;
  }

  // 检查是否已召唤
  if (soul_altar_actor->CheckSummonPaid()) {
    LOG_WARNING("Summon already paid.");
    SendMessageSoulAltarSummonResponseFailed(soul_actor);
    return;
  }

  bool bind = false;

  if (soul_altar_actor->GetFreeSummonTimes() < MISC_CONF()->soul_altar_max_free_summon_times_) {
    // 免费召唤
    soul_altar_actor->AddFreeSummonTimes();
    bind = true;
  } else {
    // 检查消耗
    entity::BindRequireType::type bind_type =
      (request.bind_ == false) ? entity::BindRequireType::NOT_BIND : entity::BindRequireType::BIND;
    // --检查道具
    if (FacadeRequest::GetInstance()->CheckPacketItemExist(id,
            MISC_CONF()->soul_altar_summon_spend_item_id_,
            MISC_CONF()->soul_altar_summon_spend_item_count_,
            bind_type) == false) {
      LOG_WARNING("Item(%u) is not enough.", MISC_CONF()->soul_altar_summon_spend_item_id_);
      SendMessageSoulAltarSummonResponseFailed(soul_actor);
      return;
    }

    // 扣除消耗
    core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(id,
        MISC_CONF()->soul_altar_summon_spend_item_id_,
        MISC_CONF()->soul_altar_summon_spend_item_count_, bind_type);
    if (delete_number != (core::uint32)MISC_CONF()->soul_altar_summon_spend_item_count_) {
      LOG_ERROR("Remove item(%d) failed, req_count=[%d], del_count=[%d].",
                MISC_CONF()->soul_altar_summon_spend_item_id_,
                MISC_CONF()->soul_altar_summon_spend_item_count_, delete_number);
      SendMessageSoulAltarSummonResponseFailed(soul_actor);
      return;
    }
    bind = request.bind_;
  }

  soul_altar_actor->SetSummonPaid();
  soul_altar_actor->SetCardBind(bind);

  // 发送成功回复
  gateway::protocol::MessageSoulAltarSummonResponse response;
  response.__set_result_(true);
  soul_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_SUMMON_RESPONSE);
}

void SoulAltarMessageHandler::OnMessageSoulAltarTakeRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(id);
  if (NULL == soul_actor) {
    LOG_ERROR("Get SoulActor(%lu) from SoulActorManager failed.", id);
    return;
  }
  GameActor *game_actor = soul_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  SoulAltarActor *soul_altar_actor = soul_actor->GetSoulAltarActor();

  // 消息解析
  gateway::protocol::MessageSoulAltarTakeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSoulAltarTakeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOUL_ALTAR_TAKE_REQUEST from actor(%lu, %s), card_pos_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(), request.card_pos_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SOUL_ALTAR) == false) {
    LOG_WARNING("SOUL_ALTAR is disable.");
    SendMessageSoulAltarTakeResponseFailed(soul_actor);
    return;
  }

  // 检查卡片位置
  if (request.card_pos_ < 0 || request.card_pos_ >= SoulAltarActor::kMaxCardSlotNum) {
    LOG_WARNING("request.card_pos_ is invalid.");
    SendMessageSoulAltarTakeResponseFailed(soul_actor);
    return;
  }

  // 检查是否已召唤
  if (soul_altar_actor->CheckSummonPaid() == false) {
    LOG_WARNING("Summon not paid.");
    SendMessageSoulAltarSummonResponseFailed(soul_actor);
    return;
  }

  if (soul_altar_actor->GetLeftCardCount() == 0) {
    soul_altar_actor->SummonCards(soul_altar_actor->CheckCardBind(), request.card_pos_);
  }

  // 检查卡片是否存在
  core::uint32 card_id = soul_altar_actor->GetCardID(request.card_pos_);
  if (0 == card_id) {
    LOG_WARNING("Card not exist.");
    SendMessageSoulAltarTakeResponseFailed(soul_actor);
    return;
  }

  // 检查背包空位
  entity::BindRequireType::type bind_type = (soul_altar_actor->CheckCardBind() == false) ?
    entity::BindRequireType::NOT_BIND : entity::BindRequireType::BIND;
  if (FacadeRequest::GetInstance()->CheckAddPacketItem(id, card_id, 1, bind_type) == false) {
    LOG_WARNING("Can not add item.");
    SendMessageSoulAltarTakeResponseFailed(soul_actor);
    return;
  }

  // 如果是第二张和第三张, 需要检查钻石
  core::int32 freedom_dollars = 0;
  if (soul_altar_actor->GetLeftCardCount() == 2)  {
    freedom_dollars = MISC_CONF()->soul_altar_second_card_spend_freedom_dollars_;
  } else if (soul_altar_actor->GetLeftCardCount() == 1) {
    freedom_dollars = MISC_CONF()->soul_altar_third_card_spend_freedom_dollars_;
  }

  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < freedom_dollars) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageSoulAltarTakeResponseFailed(soul_actor);
    return;
  }

  // 扣除消耗
  if (freedom_dollars > 0) {
    game_actor->SetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS,
        game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) - freedom_dollars);
  }

  soul_altar_actor->TakeCard(request.card_pos_);

  // 增加道具
  if (FacadeRequest::GetInstance()->AddPacketItem(id, card_id, 1, bind_type) == false) {
    LOG_ERROR("Add item failed.");
    SendMessageSoulAltarTakeResponseFailed(soul_actor);
    return;
  }

  // 发送成功回复
  gateway::protocol::MessageSoulAltarTakeResponse response;
  response.__set_result_(true);
  response.__set_card_id_(card_id);
  soul_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_TAKE_RESPONSE);
}

void SoulAltarMessageHandler::OnMessageSoulAltarResetRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取英灵玩家对象
  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(id);
  if (NULL == soul_actor) {
    LOG_ERROR("Get SoulActor(%lu) from SoulActorManager failed.", id);
    return;
  }
  GameActor *game_actor = soul_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  SoulAltarActor *soul_altar_actor = soul_actor->GetSoulAltarActor();

  // 消息解析
  gateway::protocol::MessageSoulAltarResetRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSoulAltarResetRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOUL_ALTAR_RESET_REQUEST from actor(%lu, %s).",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SOUL_ALTAR) == false) {
    LOG_WARNING("SOUL_ALTAR is disable.");
    SendMessageSoulAltarResetResponseFailed(soul_actor);
    return;
  }

  // 检查是否已召唤
  if (soul_altar_actor->CheckSummonPaid() == false) {
    LOG_WARNING("Summon not paid.");
    SendMessageSoulAltarSummonResponseFailed(soul_actor);
    return;
  }

  soul_altar_actor->ResetCards();

  // 发送成功回复
  gateway::protocol::MessageSoulAltarResetResponse response;
  response.__set_result_(true);
  soul_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOUL_ALTAR_RESET_RESPONSE);
}

}  // namespace soul

}  // namespace server

}  // namespace game

