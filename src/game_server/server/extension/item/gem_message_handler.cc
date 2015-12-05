#include "game_server/server/extension/item/gem_message_handler.h"

#include <boost/bind.hpp>

#include "entity/inform_types.h"
#include "global/types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "game_server/server/game_server.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/gem_container.h"
#include "game_server/server/event/game_item_event_types.h"

namespace game {

namespace server {

namespace item {

GemMessageHandler::GemMessageHandler() {}
GemMessageHandler::~GemMessageHandler() {}

bool GemMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_GATHER_REQUEST,
      boost::bind(&GemMessageHandler::OnMessageGemGatherRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_SKIP_GATHERING_POINT_REQUEST,
      boost::bind(&GemMessageHandler::OnMessageGemSkipGatheringPointRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_MOVE_REQUEST,
      boost::bind(&GemMessageHandler::OnMessageGemMoveRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_PROMOTE_REQUEST,
      boost::bind(&GemMessageHandler::OnMessageGemPromoteRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_DECOMPOSE_REQUEST,
      boost::bind(&GemMessageHandler::OnMessageGemDecomposeRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_REQUEST,
      boost::bind(&GemMessageHandler::OnMessageGemLockRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_ALL_REQUEST,
      boost::bind(&GemMessageHandler::OnMessageGemLockAllRequest, this, _1, _2, _3));

  return true;
}

void GemMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_GATHER_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_SKIP_GATHERING_POINT_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_MOVE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_PROMOTE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_DECOMPOSE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_ALL_REQUEST);
}

static void SendMessageGemGatherResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageGemGatherResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_GATHER_RESPONSE);
}

static void SendMessageGemSkipGatheringPointResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageGemSkipGatheringPointResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_SKIP_GATHERING_POINT_RESPONSE);
}

static void SendOnMessageGemMoveResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageGemMoveResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_MOVE_RESPONSE);
}

static void SendMessageGemPromoteResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageGemPromoteResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_PROMOTE_RESPONSE);
}

static void SendMessageGemDecomposeResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageGemDecomposeResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_DECOMPOSE_RESPONSE);
}

static void SendMessageGemLockResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageGemLockResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_RESPONSE);
}

static void SendMessageGemLockAllResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageGemLockAllResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_ALL_RESPONSE);
}

static const GemGatheringResultCell *GetRandomGemGatheringResultCell(
    const GemGatheringPointCell *point_cell) {
  core::uint32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  core::uint32 odds = 0;
  const GemGatheringResultCell *result_cell = NULL;

  for (size_t i = 0; i < point_cell->results.size(); ++i) {
    odds += point_cell->results[i].odds_;
    if (odds >= random) {
      result_cell = &point_cell->results[i];
      break;
    }
  }

  return result_cell;
}

void GemMessageHandler::OnMessageGemGatherRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", id);
    return;
  }
  GameActor *game_actor = item_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  GemContainer *gem_container = item_actor->GetGemContainer();

  // 消息解析
  gateway::protocol::MessageGemGatherRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGemGatherRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GEM_GATHER_REQUEST from actor(%lu, %s)",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    LOG_WARNING("GEM_MODULE is disable.");
    SendMessageGemGatherResponseFailed(item_actor);
    return;
  }

  // 获取激活点配置
  core::int32 gathering_point = gem_container->GetGatheringPoint();
  const GemGatheringPointCell *point_cell =
    Configure::GetInstance()->GetNextGemGatheringPoint(gathering_point);
  if (NULL == point_cell) {
    LOG_ERROR("gathering_point=[next %d] not found in config.", gathering_point);
    SendMessageGemGatherResponseFailed(item_actor);
    return;
  }

  // 获取激活点随机结果
  const GemGatheringResultCell *result_cell = GetRandomGemGatheringResultCell(point_cell);
  if (NULL == result_cell) {
    LOG_ERROR("Get Gathering result failed.");
    SendMessageGemGatherResponseFailed(item_actor);
    return;
  }

  if (result_cell->bonus_id_ != GemGatheringResultCell::BonusType::GOLD &&
      result_cell->bonus_id_ != GemGatheringResultCell::BonusType::GEM_SHARD) {
    // 检查宝石仓库是否有空位
    if (gem_container->CheckGemStorageFull()) {
      LOG_WARNING("Gem storage is full.");
      SendMessageGemGatherResponseFailed(item_actor);
      return;
    }
  }

  // 检查消耗
  // --检查金币
  if (game_actor->GetAttribute(entity::ActorClientFields::GOLD) < point_cell->spend_gold_) {
    LOG_WARNING("Gold is not enough.");
    SendMessageGemGatherResponseFailed(item_actor);
    return;
  }

  // 扣除消耗
  if (point_cell->spend_gold_ > 0) {
    game_actor->AddResource(entity::ResourceID::GOLD, 0 - point_cell->spend_gold_);
  }

  core::int32 bonus_id = result_cell->bonus_id_;

  if (result_cell->bonus_id_ == GemGatheringResultCell::BonusType::GEM_SHARD) {
    // 增加宝石碎片
    game_actor->AddResource(entity::ResourceID::GEM_SHARD,result_cell->bonus_count_);
  } else if (result_cell->bonus_id_ == GemGatheringResultCell::BonusType::GOLD) {
    // 增加金币
    game_actor->AddResource(entity::ResourceID::GOLD, result_cell->bonus_count_);
  } else {
    // 增加宝石
    if (result_cell->bonus_id_ == GemGatheringResultCell::BonusType::GREEN_QUALITY_GEM) {
      bonus_id = Configure::GetInstance()->GetRandomGemIDByQuality(GemCell::QualityType::GREEN);
    } else if (result_cell->bonus_id_ == GemGatheringResultCell::BonusType::BLUE_QUALITY_GEM) {
      bonus_id = Configure::GetInstance()->GetRandomGemIDByQuality(GemCell::QualityType::BLUE);
    } else if (result_cell->bonus_id_ == GemGatheringResultCell::BonusType::PURPLE_QUALITY_GEM) {
      bonus_id = Configure::GetInstance()->GetRandomGemIDByQuality(GemCell::QualityType::PURPLE);
    } else {
      bonus_id = Configure::GetInstance()->GetRandomGemIDByQuality(GemCell::QualityType::ORANGE);
    }
    gem_container->Add(bonus_id);
  }

  // 是否进入下一级
  core::uint32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  if (point_cell->enter_next_point_odds_ >= random) {
    if (point_cell != Configure::GetInstance()->GetLastGemGatheringPoint()) {
      gem_container->SetGatheringPoint(point_cell->id_);
    } else {
      gem_container->SetGatheringPoint(0);
    }
  } else {
    const GemGatheringPointCell *prev_point_cell =
      Configure::GetInstance()->GetPrevGemGatheringPoint(gathering_point);
    if (prev_point_cell != NULL) {
      gem_container->SetGatheringPoint(prev_point_cell->id_);
    } else {
      gem_container->SetGatheringPoint(0);
    }
  }

  gateway::protocol::MessageGemGatherResponse response;
  response.__set_result_(true);
  response.__set_gathering_point_(gem_container->GetGatheringPoint());
  response.__set_bonus_id_(bonus_id);
  response.__set_bonus_count_(result_cell->bonus_count_);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_GATHER_RESPONSE);

  // 世界公告: 获得橙色宝石
  if (GemGatheringResultCell::BonusType::ORANGE_QUALITY_GEM == result_cell->bonus_id_) {
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::GEM_GET_ORANGE_GEM);
    inform.params_.push_back(item_actor->GetStringID());
    inform.params_.push_back(item_actor->GetName());
    inform.params_.push_back(global::ToString(bonus_id));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }

  // 发送事件通知
  event::EventItemDivineGem event;
  event.__set_id_(id);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ITEM_DIVINE_GEM, game_actor->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void GemMessageHandler::OnMessageGemSkipGatheringPointRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", id);
    return;
  }
  GameActor *game_actor = item_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  GemContainer *gem_container = item_actor->GetGemContainer();

  // 消息解析
  gateway::protocol::MessageGemSkipGatheringPointRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGemSkipGatheringPointRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GEM_SKIP_GATHERING_POINT_REQUEST from actor(%lu, %s)",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    LOG_WARNING("GEM_MODULE is disable.");
    SendMessageGemSkipGatheringPointResponseFailed(item_actor);
    return;
  }

  const GemGatheringPointCell *point_cell = Configure::GetInstance()->GetLastGemGatheringPoint();
  if (NULL == point_cell) {
    LOG_ERROR("last_gathering_point not found in config.");
    SendMessageGemSkipGatheringPointResponseFailed(item_actor);
    return;
  }
  const GemGatheringPointCell *prev_point_cell =
    Configure::GetInstance()->GetPrevGemGatheringPoint(point_cell->id_);
  if (NULL == prev_point_cell) {
    LOG_ERROR("gathering_point=[prev %d] not found in config.", point_cell->id_);
    SendMessageGemSkipGatheringPointResponseFailed(item_actor);
    return;
  }

  if (gem_container->GetGatheringPoint() == prev_point_cell->id_) {
    LOG_ERROR("Already in last point.");
    SendMessageGemSkipGatheringPointResponseFailed(item_actor);
    return;
  }

  // 检查消耗
  // --检查钻石
  if (game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) <
      point_cell->spend_freedom_dollars_) {
    LOG_WARNING("Freedom dollars is not enough.");
    SendMessageGemSkipGatheringPointResponseFailed(item_actor);
    return;
  }

  // 扣除消耗
  if (point_cell->spend_freedom_dollars_ > 0) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - point_cell->spend_freedom_dollars_);
  }

  gem_container->SetGatheringPoint(prev_point_cell->id_);

  gateway::protocol::MessageGemSkipGatheringPointResponse response;
  response.__set_result_(true);
  response.__set_gathering_point_(gem_container->GetGatheringPoint());
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_SKIP_GATHERING_POINT_RESPONSE);
}

void GemMessageHandler::OnMessageGemMoveRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", id);
    return;
  }
  GameActor *game_actor = item_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  GemContainer *gem_container = item_actor->GetGemContainer();

  // 消息解析
  gateway::protocol::MessageGemMoveRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGemMoveRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GEM_MOVE_REQUEST from actor(%lu, %s), "
           "src_location_type_=[%d], src_location_index_=[%d] "
           "dest_location_type_=[%d], dest_location_index_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.src_location_type_, request.src_location_index_,
           request.dest_location_type_, request.dest_location_index_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    LOG_WARNING("GEM_MODULE is disable.");
    SendOnMessageGemMoveResponseFailed(item_actor);
    return;
  }

  bool result = gem_container->Move(
      GemContainer::Location(request.src_location_type_, request.src_location_index_),
      GemContainer::Location(request.dest_location_type_, request.dest_location_index_));

  if (false == result) {
    LOG_WARNING("Move Failed");
  }

  gateway::protocol::MessageGemMoveResponse response;
  response.__set_result_(result);

  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_MOVE_RESPONSE);
}

void GemMessageHandler::OnMessageGemPromoteRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", id);
    return;
  }
  GameActor *game_actor = item_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  GemContainer *gem_container = item_actor->GetGemContainer();

  // 消息解析
  gateway::protocol::MessageGemPromoteRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGemPromoteRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GEM_PROMOTE_REQUEST from actor(%lu, %s), "
           "location_type_=[%d], location_index_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.location_type_, request.location_index_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    LOG_WARNING("GEM_MODULE is disable.");
    SendMessageGemPromoteResponseFailed(item_actor);
    return;
  }

  GemContainer::Location location(request.location_type_, request.location_index_);

  const GemContainer::Gem *gem = gem_container->GetGemInfo(location);
  if (NULL == gem) {
    LOG_WARNING("Location is invalid.");
    SendMessageGemPromoteResponseFailed(item_actor);
    return;
  }

  const GemCell *cell = Configure::GetInstance()->GetGem(gem->id_, gem->level_);
  if (NULL == cell) {
    LOG_ERROR("gem_id=[%d], gem_level=[%d] not found in config.",
              gem->id_, gem->level_);
    SendMessageGemPromoteResponseFailed(item_actor);
    return;
  }

  const GemCell *next_cell = Configure::GetInstance()->GetGem(gem->id_, gem->level_ + 1);
  if (NULL == next_cell) {
    LOG_WARNING("Gem is top level.");
    SendMessageGemPromoteResponseFailed(item_actor);
    return;
  }

  // 检查消耗
  // --检查宝石碎片
  if (game_actor->GetAttribute(entity::ActorClientFields::GEM_SHARD) <
      cell->promote_spend_shard_count_) {
    LOG_WARNING("Gem shard is not enough.");
    SendMessageGemPromoteResponseFailed(item_actor);
    return;
  }

  // 扣除消耗
  if (cell->promote_spend_shard_count_ > 0) {
    game_actor->AddResource(entity::ResourceID::GEM_SHARD, 0 - cell->promote_spend_shard_count_);
  }

  gem_container->AddExp(location, cell->promote_exp_reward_);

  gateway::protocol::MessageGemPromoteResponse response;
  response.__set_result_(true);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_PROMOTE_RESPONSE);
}

void GemMessageHandler::OnMessageGemDecomposeRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", id);
    return;
  }
  GameActor *game_actor = item_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  GemContainer *gem_container = item_actor->GetGemContainer();

  // 消息解析
  gateway::protocol::MessageGemDecomposeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGemDecomposeRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GEM_DECOMPOSE_REQUEST from actor(%lu, %s)",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    LOG_WARNING("GEM_MODULE is disable.");
    SendMessageGemDecomposeResponseFailed(item_actor);
    return;
  }

  bool result = true;
  core::int32 shard_count = gem_container->Decompose();

  if (-1 == shard_count) {
    LOG_WARNING("Decompose failed.");
    result = false;
  }

  gateway::protocol::MessageGemDecomposeResponse response;
  response.__set_result_(result);
  response.__set_shard_count_(shard_count);

  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_DECOMPOSE_RESPONSE);
}

void GemMessageHandler::OnMessageGemLockRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", id);
    return;
  }
  GameActor *game_actor = item_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  GemContainer *gem_container = item_actor->GetGemContainer();

  // 消息解析
  gateway::protocol::MessageGemLockRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGemLockRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GEM_LOCK_REQUEST from actor(%lu, %s), "
           "location_type_=[%d], location_index_=[%d], locked=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.location_type_, request.location_index_, request.locked_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    LOG_WARNING("GEM_MODULE is disable.");
    SendMessageGemLockResponseFailed(item_actor);
    return;
  }

  bool result = gem_container->Lock(
      GemContainer::Location(request.location_type_, request.location_index_),
      request.locked_);

  if (false == result) {
    LOG_WARNING("Lock failed.");
  }

  gateway::protocol::MessageGemLockResponse response;
  response.__set_result_(result);

  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_RESPONSE);
}

void GemMessageHandler::OnMessageGemLockAllRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", id);
    return;
  }
  GameActor *game_actor = item_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  GemContainer *gem_container = item_actor->GetGemContainer();

  // 消息解析
  gateway::protocol::MessageGemLockAllRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageGemLockAllRequest falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GEM_LOCK_ALL_REQUEST from actor(%lu, %s), "
           "locked=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.locked_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::GEM_MODULE) == false) {
    LOG_WARNING("GEM_MODULE is disable.");
    SendMessageGemLockAllResponseFailed(item_actor);
    return;
  }

  bool result = gem_container->LockAll(request.locked_);

  if (false == result) {
    LOG_WARNING("Lock all failed.");
  }

  gateway::protocol::MessageGemLockAllResponse response;
  response.__set_result_(result);

  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GEM_LOCK_ALL_RESPONSE);
}

}  // namespace item

}  // namespace server

}  // namespace game

