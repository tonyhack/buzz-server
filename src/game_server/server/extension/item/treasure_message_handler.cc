#include "game_server/server/extension/item/treasure_message_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "entity/inform_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/load_drop_configure.h"
#include "game_server/server/configure.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/configure/configure.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/types.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/game_item_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace item {

TreasureMessageHandler::TreasureMessageHandler() {}
TreasureMessageHandler::~TreasureMessageHandler() {}

bool TreasureMessageHandler::Initialize() {
  // MESSAGE_ITEM_TREASURE_HUNT_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_TREASURE_HUNT_REQUEST,
      boost::bind(&TreasureMessageHandler::OnMessageTreasureHunt, this, _1, _2, _3));

  return true;
}

void TreasureMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_TREASURE_HUNT_REQUEST);
}

void TreasureMessageHandler::OnMessageTreasureHunt(core::uint64 id, const char *data,
    size_t size) {
  // 获取道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  // 获取 GameActor 对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%lu) GameActor 失败", id);
    return ;
  }

  // 验证功能是否开启
  if(game_actor->CheckFunctionalityState(entity::FunctionalityType::HUNT_TREASURE) == false) {
    LOG_ERROR("寻宝功能未开启，无法操作");
    return ;
  }

  gateway::protocol::MessageItemTreasureHuntRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("%s:%d (%s) 解析消息 MessageItemTreasureHuntRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ITEM_TREASURE_HUNT_REQUEST from actor(%lu, %s), num_(%d), tab_id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.num_, message.tab_id_);

  // 获取相应容器
  ItemContainer *container = actor->GetContainer(entity::ItemContainerType::PACKET, entity::VocationType::MAX);
  if(container == NULL) {
    return ;
  }
  PacketContainer *pack = (PacketContainer *)container;

  // 检查消耗
  if(message.tab_id_ > 2) {
    LOG_ERROR("不支持超过2的标签");
    return ;
  }
  const TreasureSpendCell *spend_cell = Configure::GetInstance()->GetSpendCell(message.tab_id_, message.num_);
  if(spend_cell == NULL) {
    LOG_ERROR("玩家消息tab_id(%d)(%d)错误", message.tab_id_, message.num_);
    return ;
  }
  bool isbind = true;
  if(spend_cell->item_ != 0) {
    if(pack->GetItemNumber(spend_cell->item_, entity::BindRequireType::BIND) < (core::uint32)spend_cell->item_num_) {
      if(pack->GetItemNumber(spend_cell->item_, entity::BindRequireType::NOT_BIND) < (core::uint32)spend_cell->item_num_) {
        LOG_ERROR("道具不足(%d)", spend_cell->item_);
        return ;
      }
      isbind = false;
    }
  }

  // 检查礼品仓库
  TreasureContainer *treasure_container = (TreasureContainer *)actor->GetContainer(
      entity::ItemContainerType::TREASURE, entity::VocationType::MAX);
  if(treasure_container == NULL) {
    LOG_ERROR("玩家礼品仓库没有找到");
    return ;
  }

  core::int32 drop_id = 0;
  std::vector<game::server::DropElement> temp_drops;
  std::vector<entity::AddItemFields> temp_drops_item; 
  entity::AddItemFields item_field;
  gateway::protocol::MessageItemTreasureHuntResponse response;
  for(core::int32 i = 0; i < message.num_; ++i) {
    HuntTreasureRandCell *cell_rand = Configure::GetInstance()->GetTreasureRandDrop(message.tab_id_);
    if(cell_rand == 0) {
      continue ;
    }
    response.rand_pos_.push_back(cell_rand->position_);
    // 得到掉落
    const LoadDropConfigure::DropElementVector &drops =
      game::server::Configure::GetInstance()->Drop(cell_rand->drop_);
    for(size_t j = 0; j < drops.size(); ++j) {
      if(drops[j].resource_type_ == entity::ResourceType::ITEM) {
        item_field.item_template_ = drops[j].resource_id_;
        item_field.number_ = drops[j].number_;
        item_field.intensify_level_ = drops[j].intensify_level_;
        item_field.bind_ = isbind;
        temp_drops_item.push_back(item_field);
      } else {
        temp_drops.push_back(drops[j]);
      }
    }
  }

  if(treasure_container->CheckAddItems(temp_drops_item) == false) {
    LOG_ERROR("礼品仓库空间不足");
    return ;
  }

  for(size_t i = 0; i < temp_drops_item.size(); ++i) {
    if(treasure_container->AddItems(temp_drops_item[i].item_template_, temp_drops_item[i].number_,
          temp_drops_item[i].bind_, temp_drops_item[i].intensify_level_, NULL) == false) {
      LOG_ERROR("加入道具(%d)num(%d)bind(%d)", temp_drops_item[i].item_template_,
          temp_drops_item[i].number_, temp_drops_item[i].bind_);
      return ;
    }
    response.template_id_list_.push_back(temp_drops_item[i].item_template_);
  }
  for(size_t i = 0; i < temp_drops.size(); ++i) {
    game_actor->AddResource((entity::ResourceID::type)temp_drops[i].resource_id_, temp_drops[i].number_);
  }
  entity::BindRequireType::type bind_type = isbind == true ? entity::BindRequireType::BIND : entity::BindRequireType::NOT_BIND;
  if(pack->RemoveItem((core::uint32)spend_cell->item_, (core::uint32)spend_cell->item_num_, bind_type) == false){
    LOG_ERROR("删除消耗的道具失败(%d)num(%d)bind(%d)", spend_cell->item_, spend_cell->item_num_, entity::BindRequireType::BOTH);
    return ;
  }

  session::protocol::MessageItemTreasureHuntSynchronize synchronize;
  for(size_t i = 0; i < response.template_id_list_.size(); ++i) {
    const ItemCell *item_cell = Configure::GetInstance()->GetItem(response.template_id_list_[i]);
    if(item_cell == NULL) {
      LOG_ERROR("配置(%d)没有找到",response.template_id_list_[i]);
    } else {
      if(item_cell->quality_ >= 3) {
        // 公告session
        std::string info;
        info.append(global::ToString(id));
        info.append(",");
        info.append(game_actor->GetName());
        info.append(",");
        info.append(global::ToString(response.template_id_list_[i]));
        synchronize.info_.push_back(info);
      }
    }
  }

  // 积分
  if(game_actor->GetAttribute(entity::ActorAoiFields::VIP) > (core::int32)entity::VipType::INVALID) {
    IntegralType::type integral_type = Configure::GetInstance()->GetItegralType(message.tab_id_);
    if(IntegralType::MIRACLE == integral_type) {
      game_actor->AddResource(entity::ResourceID::MIRACLE_INTEGRAL, spend_cell->integral_num_);
    }
    if(IntegralType::LEGEND == integral_type) {
      game_actor->AddResource(entity::ResourceID::LEGEND_INTEGRAL, spend_cell->integral_num_);
    }
  }

  if(synchronize.info_.size() > 0) {
    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_ITEM_TREASURE_HUNT_SYNCHRONIZE);
  }

  // 响应玩家
  response.__set_tab_id_(message.tab_id_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ITEM_TREASURE_HUNT_RESPONSE);

}

}  // namespace item

}  // namespace server

}  // namespace game

