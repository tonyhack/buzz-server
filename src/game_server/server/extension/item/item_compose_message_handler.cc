#include "game_server/server/extension/item/item_compose_message_handler.h" 

#include <cstdlib>
#include <map>
#include <boost/bind.hpp>

#include "entity/item_types.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/packet_container.h"

namespace game {

namespace server {

namespace item {

ItemComposeMessageHandler::ItemComposeMessageHandler() {}
ItemComposeMessageHandler::~ItemComposeMessageHandler() {}

bool ItemComposeMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_COMPOSE_REQUEST,
      boost::bind(&ItemComposeMessageHandler::OnMessageItemComposeRequest, this, _1, _2, _3));

  return true;
}

void ItemComposeMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_COMPOSE_REQUEST);
}

static void SendMessageItemComposeResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageItemComposeResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ITEM_COMPOSE_RESPONSE);
}

void ItemComposeMessageHandler::OnMessageItemComposeRequest(core::uint64 id,
    const char *data, size_t size) {
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

  // 消息解析
  gateway::protocol::MessageItemComposeRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageItemComposeRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ITEM_COMPOSE_REQUEST from actor(%lu, %s), "
           "recipe_id_=[%d], number_=[%d], bind_=[%d]",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.recipe_id_, request.number_, request.bind_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::ITEM_COMPOSE) == false) {
    LOG_WARNING("ITEM_COMPOSE is disable.");
    SendMessageItemComposeResponseFailed(item_actor);
    return;
  }

  // 检查合成配方是否存在
  const ItemComposeRecipeCell *cell =
    Configure::GetInstance()->GetItemComposeRecipe(request.recipe_id_);
  if (NULL == cell) {
    LOG_WARNING("recipe_id is invalid.");
    SendMessageItemComposeResponseFailed(item_actor);
    return;
  }

  // 获取合成数量
  core::int32 compose_number = 1;
  if (request.__isset.number_) {
    if (request.number_ <= 0) {
      LOG_WARNING("number is invalid.");
      SendMessageItemComposeResponseFailed(item_actor);
      return;
    }
    compose_number = request.number_;
  }

  if (compose_number > 1 && cell->consume_equipments_.empty() == false) {
    LOG_WARNING("number is invalid.");
    SendMessageItemComposeResponseFailed(item_actor);
    return;
  }

  // 获取背包容器
  PacketContainer *packet = (PacketContainer *)item_actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if (NULL == packet) {
    LOG_ERROR("Get packet container failed.");
    SendMessageItemComposeResponseFailed(item_actor);
    return;
  }

  // 检查资源是否足够
  if (cell->resource_id_ != 0) {
    core::int32 actor_resource = game_actor->GetResource(
        (entity::ResourceID::type)cell->resource_id_);
    if (actor_resource / compose_number < cell->resource_number_) {
      LOG_WARNING("resource(%d) is not enough.", cell->resource_id_);
      SendMessageItemComposeResponseFailed(item_actor);
      return;
    }
  }

  // 成品的绑定属性
  bool result_item_bind = false;
  if (cell->bind_ == ItemComposeRecipeCell::BindType::NOT_BIND) {
    result_item_bind = false; 
    request.bind_ = true;
  } else if (cell->bind_ == ItemComposeRecipeCell::BindType::BIND) {
    result_item_bind = true;
    request.bind_ = true;
  }

  // 消耗道具的绑定属性
  std::map<core::uint32, entity::BindRequireType::type> consume_item_bind_types;
  // 消耗装备的item_id
  std::vector<core::uint64> consume_equipments;

  // 检查合成材料是否足够
  // --检查需要的道具
  for (ItemComposeRecipeCell::ItemMap::const_iterator iter = cell->consume_items_.begin();
       iter != cell->consume_items_.end(); ++iter) {
    core::uint32 item_template_id = iter->first;
    core::uint32 item_count = iter->second;

    core::uint32 actor_not_bind_item_number =
      packet->GetItemNumber(item_template_id, entity::BindRequireType::NOT_BIND);
    core::uint32 actor_bind_item_number = 
      packet->GetItemNumber(item_template_id, entity::BindRequireType::BIND);
    core::uint32 actor_item_number = actor_not_bind_item_number;
    if (request.bind_) {
      actor_item_number += actor_bind_item_number;
    }

    if (actor_item_number / compose_number < item_count) {
      LOG_WARNING("Materail(%u) is not enough.", item_template_id);
      SendMessageItemComposeResponseFailed(item_actor);
      return;
    }

    if (request.bind_) {
      consume_item_bind_types[item_template_id] = entity::BindRequireType::BOTH;
    } else {
      consume_item_bind_types[item_template_id] = entity::BindRequireType::NOT_BIND;
    }

    if (request.bind_ && actor_bind_item_number > 0 &&
        cell->bind_ == ItemComposeRecipeCell::BindType::DEPEND_ON_MATERIAL) {
      result_item_bind = true;
    }
  }

  // --检查需要的装备
  for (size_t i = 0; i < cell->consume_equipments_.size(); ++i) {
    bool found = false;

    // 搜索item_id列表找到匹配的道具
    for (size_t j = 0; j < request.item_ids_.size(); ++j) {
      core::uint64 item_id = atol(request.item_ids_[j].c_str());

      Item *item = packet->GetItemByID(item_id);
      if (NULL == item) {
        LOG_WARNING("item_id(%lu) is invalid.", item_id);
        return;
      }

      if (cell->consume_equipments_[i] == item->GetTemplateID()) {
        found = true;
        request.item_ids_.erase(request.item_ids_.begin() + j);
        consume_equipments.push_back(item_id);
        if (item->GetBind() == true &&
            cell->bind_ == ItemComposeRecipeCell::BindType::DEPEND_ON_MATERIAL) {
          result_item_bind = true;
        }
        break;
      }
    }

    if (false == found) {
      LOG_WARNING("Materail(%u) is not enough.", cell->consume_equipments_[i]);
      SendMessageItemComposeResponseFailed(item_actor);
      return;
    }
  }

  // 检查是否可以添加成品道具
  if (packet->CheckAddItem(cell->result_item_id_,
          compose_number, result_item_bind) == false) {
    LOG_WARNING("Packet full.");
    SendMessageItemComposeResponseFailed(item_actor);
    return;
  }

  // 扣除消耗
  // --扣除资源
  if (cell->resource_id_ != 0 && cell->resource_number_ * compose_number > 0) {
    game_actor->AddResource(
        (entity::ResourceID::type)cell->resource_id_,
        0 - cell->resource_number_ * compose_number);
  }

  // --扣除道具
  for (ItemComposeRecipeCell::ItemMap::const_iterator iter = cell->consume_items_.begin();
       iter != cell->consume_items_.end(); ++iter) {
    core::uint32 item_template_id = iter->first;
    core::uint32 item_count = iter->second;
    packet->RemoveItem(item_template_id, item_count * compose_number,
        consume_item_bind_types[item_template_id]);
  }
  // --扣除装备
  for (size_t i = 0; i < consume_equipments.size(); ++i) {
    Item *item = packet->GetItemByID(consume_equipments[i]);
    if (NULL == item) {
      LOG_ERROR("Can not find item_id(%lu).", consume_equipments[i]);
      return;
    }
    packet->RemoveItem(item);
  }

  // 增加成品
  packet->AddItems(cell->result_item_id_, compose_number, result_item_bind, 0);

  // 发送成功回复
  gateway::protocol::MessageItemComposeResponse response;
  response.__set_result_(true);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ITEM_COMPOSE_RESPONSE);
}

}  // namespace item

}  // namespace server

}  // namespace game

