//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 15:43:38.
// File name: container_message_handler.cc
//
// Description:
// Define class ContainerMessageHandler.
//

#include "game_server/server/extension/item/container_message_handler.h"

#include <algorithm>

#include <boost/bind.hpp>

#include "entity/gender_types.h"
#include "entity/item_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/facade_request.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_container.h"
#include "game_server/server/extension/item/medicine.h"
#include "game_server/server/event/game_item_event_types.h"
#include "game_server/server/error_code.h"
#include "game_server/server/configure.h"
#include "game_server/server/load_drop_configure.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace item {

ContainerMessageHandler::ContainerMessageHandler() {}
ContainerMessageHandler::~ContainerMessageHandler() {}

bool ContainerMessageHandler::Initialize() {
  // MESSAGE_ITEM_MOVE
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_MOVE,
      boost::bind(&ContainerMessageHandler::OnMessageItemMove, this, _1, _2, _3));

  // MESSAGE_ITEM_REMOVE
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_REMOVE,
      boost::bind(&ContainerMessageHandler::OnMessageItemRemove, this, _1, _2, _3));

  // MESSAGE_ITEM_USE
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_USE,
      boost::bind(&ContainerMessageHandler::OnMessageItemUse, this, _1, _2, _3));

  // MESSAGE_ITEM_SPLIT
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_SPLIT,
      boost::bind(&ContainerMessageHandler::OnMessageItemSplit, this, _1, _2, _3));

  // MESSAGE_ITEM_ARRANGE
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_ARRANGE,
      boost::bind(&ContainerMessageHandler::OnMessageItemArrange, this, _1, _2, _3));

  // MESSAGE_EXTEND_CONTAINER
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EXTEND_CONTAINER,
      boost::bind(&ContainerMessageHandler::OnMessageExtendContainer, this, _1, _2, _3));

  // MESSAGE_SHORTCUT_SET_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHORTCUT_SET_REQUEST,
      boost::bind(&ContainerMessageHandler::OnMessageShortcutSetRequest, this, _1, _2, _3));

  return true;
}

void ContainerMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_MOVE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_REMOVE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_USE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_SPLIT);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ITEM_ARRANGE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EXTEND_CONTAINER);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHORTCUT_SET_REQUEST);
}

void ContainerMessageHandler::OnMessageItemMove(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageItemMove message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息 MessageItemMove 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ITEM_MOVE from actor(%lu, %s), src_container_(%d), src_location_(%d), src_vocation_(%d), dest_container_(%d), dest_location_(%d), dest_vocation_(%d), item_id_(%s)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.src_container_, message.src_location_, message.src_vocation_,
      message.dest_container_, message.dest_location_, message.dest_vocation_, message.item_id_.c_str());

  // 位置不变
  if(message.src_container_ == message.dest_container_ &&
      message.src_location_ == message.dest_location_) {
    return ;
  }

  entity::VocationType::type src_vocation_type = entity::VocationType::MAX;
  entity::VocationType::type dest_vocation_type = entity::VocationType::MAX;

  if(message.__isset.src_vocation_ == true) {
    src_vocation_type = message.src_vocation_;
  }
  if(message.__isset.dest_vocation_ == true) {
    dest_vocation_type = message.dest_vocation_;
  }

  ItemContainer *src_container = actor->GetContainer(message.src_container_, src_vocation_type);
  ItemContainer *dest_container = actor->GetContainer(message.dest_container_, dest_vocation_type);

  // 容器是否合法
  if(src_container == NULL || dest_container == NULL) {
    global::LogError("%s:%d (%s) 源容器或目标容器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取出源容器相应位置的道具
  Item *src_item = src_container->GetItem(message.src_location_);
  if(src_item == NULL || src_item->GetStringID() != message.item_id_) {
    global::LogError("%s:%d (%s) 道具不存在或ID不匹配",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取出目的容器相应位置的道具
  Item *dest_item = dest_container->GetItem(message.dest_location_);

  const ItemCell *src_cell = Configure::GetInstance()->GetItem(src_item->GetTemplateID());
  if(src_cell == NULL) {
    LOG_ERROR("找不到道具(%u)配置", src_item->GetTemplateID());
    return ;
  }

  // 如果往装备栏放
  if(dest_container->GetType() == entity::ItemContainerType::EQUIP) {
    const EquipmentCell *equip_cell = (const EquipmentCell *)src_cell;
    if(((Equipment *)src_item)->ExpireCheck() == true) {
      LOG_ERROR("装备已过期，不能再装备");
      return ;
    }
    if (game_actor->GetAttribute(entity::RoleAoiFields::VOCATION) != dest_vocation_type) {
      LOG_ERROR("装备职业与当前职业不符，不能装备");
      return;
    }
    if(equip_cell->vocation_ != dest_vocation_type) {
      LOG_ERROR("装备职业不符，不能装备");
      return ;
    }
    if(equip_cell->gender_ != entity::GenderType::BOTH && equip_cell->gender_ != 
        game_actor->GetAttribute(entity::ActorAoiFields::GENDER)) {
      LOG_ERROR("装备性别(%d)不符(%d)，不能装备", equip_cell->gender_,
          game_actor->GetAttribute(entity::ActorAoiFields::GENDER));
      return ;
    }
  }

  // 如果是交换
  if(dest_item != NULL) {
    const ItemCell* cell = Configure::GetInstance()->GetItem(dest_item->GetTemplateID());
    if(cell == NULL) {
      LOG_ERROR("找不到道具(%u)配置", dest_item->GetTemplateID());
      return ;
    }
    core::int32 src_stack_count =
      src_item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
    core::int32 dest_stack_count =
      dest_item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
    // 两个道具ID相同且目标道具叠加数量小于配置最大叠加数据时，才进行叠加
    if(dest_item->GetTemplateID() == src_item->GetTemplateID() &&
        (core::uint32)dest_stack_count < cell->stack_count_) {
      core::int32 bind = 0;
      if(dest_item->GetAttribute(entity::ItemClientFields::BIND_TYPE) !=
          src_item->GetAttribute(entity::ItemClientFields::BIND_TYPE)) {
        bind = 1;
      } else {
        bind = dest_item->GetAttribute(entity::ItemClientFields::BIND_TYPE);
      }
      if((core::uint32)(dest_stack_count + src_stack_count) <= cell->stack_count_) {
        if(src_container->CheckRemoveItem(src_item) == false ||
          src_container->RemoveItem(src_item) == false) {
          LOG_ERROR("堆叠操作时，无法删除源道具");
          return ;
        }
        dest_item->SetAttribute(entity::ItemClientFields::STACK_COUNT,
            dest_stack_count + src_stack_count);
      } else {
        dest_item->SetAttribute(entity::ItemClientFields::STACK_COUNT,
            cell->stack_count_);
        src_item->SetAttribute(entity::ItemClientFields::STACK_COUNT,
            src_stack_count - (cell->stack_count_ - dest_stack_count));
      }
      // 设置绑定状态
      if(dest_item->GetAttribute(entity::ItemClientFields::BIND_TYPE) != bind) {
        dest_item->SetAttribute(entity::ItemClientFields::BIND_TYPE, bind);
      }
      return ;
    } else {
      // 道具位置交换处理
      if(src_container->CheckRemoveItem(src_item) == false ||
          dest_container->CheckRemoveItem(dest_item) == false) {
        global::LogError("%s:%d (%s) CheckRemoveItem 失败", __FILE__, __LINE__, __FUNCTION__);
        return ;
      }
      if(src_container->RemoveItem(src_item, false) == false) {
        global::LogError("%s:%d (%s) 删除源容器道具失败", __FILE__, __LINE__, __FUNCTION__);
        return ;
      }
      if(dest_container->RemoveItem(dest_item, false) == false) {
        global::LogError("%s:%d (%s) 删除源容器道具失败", __FILE__, __LINE__, __FUNCTION__);
        if(src_container->AddItem(src_item, message.src_location_, false) == false) {
          global::LogError("%s:%d (%s) 玩家(%lu) 移动道具失败，造成丢失道具(%u)",
              __FILE__, __LINE__, __FUNCTION__, id, src_item->GetTemplateID());
          ItemBuilder::GetInstance()->Destory(src_item);
        }
        return ;
      }
      if(src_container->CheckAddItem(dest_item, message.src_location_) == false ||
          dest_container->CheckAddItem(src_item, message.dest_location_) == false) {
        LOG_ERROR("CheckAddItem 失败");
        if(src_container->AddItem(src_item, message.src_location_, false) == false) {
          global::LogError("%s:%d (%s) 玩家(%lu) 移动道具失败，造成丢失道具(%u)",
              __FILE__, __LINE__, __FUNCTION__, id, src_item->GetTemplateID());
          ItemBuilder::GetInstance()->Destory(src_item);
        }
        if(dest_container->AddItem(dest_item, message.dest_location_, false) == false) {
          global::LogError("%s:%d (%s) 玩家(%lu) 移动道具失败，造成丢失道具(%u)",
              __FILE__, __LINE__, __FUNCTION__, id, dest_item->GetTemplateID());
          ItemBuilder::GetInstance()->Destory(dest_item);
        }
        return ;
      }
      if(src_container->AddItem(dest_item, message.src_location_, false) == false) {
        global::LogError("%s:%d (%s) 添加源容器道具失败", __FILE__, __LINE__, __FUNCTION__);
        if(src_container->AddItem(src_item, message.src_location_, false) == false) {
          global::LogError("%s:%d (%s) 玩家(%lu) 移动道具失败，造成丢失道具(%u)",
              __FILE__, __LINE__, __FUNCTION__, id, src_item->GetTemplateID());
          ItemBuilder::GetInstance()->Destory(src_item);
        }
        if(dest_container->AddItem(dest_item, message.dest_location_, false) == false) {
          global::LogError("%s:%d (%s) 玩家(%lu) 移动道具失败，造成丢失道具(%u)",
              __FILE__, __LINE__, __FUNCTION__, id, dest_item->GetTemplateID());
          ItemBuilder::GetInstance()->Destory(dest_item);
        }
        return ;
      }
      if(dest_container->AddItem(src_item, message.dest_location_, false) == false) {
        global::LogError("%s:%d (%s) 玩家(%lu) 移动道具失败，造成丢失道具(%u)",
            __FILE__, __LINE__, __FUNCTION__, id, src_item->GetTemplateID());
        ItemBuilder::GetInstance()->Destory(src_item);
        return ;
      }
      // 先把目标物品移动到源物品位置
      gateway::protocol::MessageItemMove dest_move_message;
      dest_move_message.__set_item_id_(dest_item->GetStringID());
      dest_move_message.__set_src_vocation_(message.dest_vocation_);
      dest_move_message.__set_src_container_(message.dest_container_);
      dest_move_message.__set_src_location_(message.dest_location_);
      dest_move_message.__set_dest_vocation_(message.src_vocation_);
      dest_move_message.__set_dest_container_(message.src_container_);
      dest_move_message.__set_dest_location_(message.src_location_);
      actor->SendMessage(dest_move_message, gateway::protocol::MessageType::MESSAGE_ITEM_MOVE);
    }
  } else {
    if(src_container->CheckRemoveItem(src_item) == false ||
        dest_container->CheckAddItem(src_item, message.dest_location_) == false) {
      global::LogError("%s:%d (%s) CheckRemoveItem/CheckAddItem 失败",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    if(src_container->RemoveItem(src_item, false) == false) {
      global::LogError("%s:%d (%s) 删除源容器道具失败", __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    if(dest_container->AddItem(src_item, message.dest_location_, false) == false) {
      global::LogError("%s:%d (%s) 添加目的容器道具失败", __FILE__, __LINE__, __FUNCTION__);
      if(src_container->AddItem(src_item, message.src_location_, false) == false) {
        global::LogError("%s:%d (%s) 玩家(%lu) 移动道具失败，造成丢失道具(%u)",
            __FILE__, __LINE__, __FUNCTION__, id, src_item->GetTemplateID());
        ItemBuilder::GetInstance()->Destory(src_item);
      }
      return ;
    }
  }

  // 更新战斗力
  if (src_container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)src_container;
    equipment_container->UpdateEquipmentFightScore();
  }
  if (dest_container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)dest_container;
    equipment_container->UpdateEquipmentFightScore();
  }

  // 把源物品放到目标位置
  actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_ITEM_MOVE);
}

void ContainerMessageHandler::OnMessageItemRemove(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%llu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageItemRemove message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息 MessageItemRemove 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ITEM_REMOVE from actor(%lu, %s), container_(%d), item_id_(%s), location_(%d), vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.container_, message.item_id_.c_str(), message.location_, message.vocation_);

  entity::VocationType::type vocation_type = entity::VocationType::MAX;
  if(message.__isset.vocation_ == true) {
    vocation_type = message.vocation_;
  }

  ItemContainer *container = actor->GetContainer(message.container_, vocation_type);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 容器不存在", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  Item *item = container->GetItem(message.location_);
  if(item == NULL || item->GetStringID() != message.item_id_) {
    global::LogError("%s:%d (%s) 道具不存在或道具ID不匹配",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(container->CheckRemoveItem(item) == false) {
    global::LogError("%s:%d (%s) CheckRemoveItem 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(container->RemoveItem(item, true) == false) {
    global::LogError("%s:%d (%s) 从容器中删除道具失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  ItemBuilder::GetInstance()->Destory(item);

  // 更新战斗力
  if(container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)container;
    equipment_container->UpdateEquipmentFightScore();
  }
}

void ContainerMessageHandler::OnMessageItemUse(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%llu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }
  
  // 得到玩家对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到[%lu]", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageItemUse message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息 MessageItemUse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ITEM_USE from actor(%lu, %s), container_(%d), item_id_(%s), location_(%d), number_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.container_, message.item_id_.c_str(), message.location_, message.number_);

  ItemContainer *container = actor->GetContainer(message.container_);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 容器不存在", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(container->GetType() != entity::ItemContainerType::PACKET) {
    LOG_ERROR("使用道具时，类型[%d]不是背包", message.container_);
    return ;
  }

  Item *item = container->GetItem(message.location_);
  if(item == NULL || item->GetStringID() != message.item_id_) {
    global::LogError("%s:%d (%s) 道具不存在或道具ID不匹配",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 查看道具类型
  const ItemCell* cell = Configure::GetInstance()->GetItem(item->GetTemplateID());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 道具不存在或道具模板ID不匹配",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 查看是否过期
  time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  if(cell->expire_absolute_time_ != -1 && now >= cell->expire_absolute_time_)
    return;
  // 验证道具使用等级
  if(cell->level_ > (core::uint32)game_actor->GetAttribute(
        entity::RoleAoiFields::LEVEL)) {
    LOG_ERROR("等级不够[%d]，不能使用该道具", cell->level_);
    ErrorCode::Send(game_actor, entity::ErrorType::ACTOR_LEVEL_LOW);
    return ;
  }
  if(item->GetAttribute(entity::ItemClientFields::STACK_COUNT) <
      message.number_) {
    LOG_ERROR("道具不足[%d]", message.number_);
    return ;
  }
  // 验证批量使用时，冷却时间
  if(cell->cooling_id_ != 0 && message.number_ != 1) {
    LOG_ERROR("冷却时间不为0的道具不能批量使用");
    return ;
  }

  core::int32 ret = 0;
  core::int32 needdelete = 0;
  for(core::int32 i = 0; i < message.number_; ++i) {
    // TODO 根据道具类型调用使用效果,暂时只有medicine
    switch(item->GetItemType()) {
      case entity::ItemType::MEDICINE: 
        ret = ((Medicine*)item)->UseResult(cell);
        break;
      case entity::ItemType::CARD:
        break;
      case entity::ItemType::MATERIAL:
        break;
      default:
        return ;
    }
    if(ret) 
      return ;

    ++needdelete;
    ret = 0;
    // 事件通知
    event::EventItemUse event;
    event.__set_template_id_(item->GetTemplateID());
    event.__set_id_(id);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ITEM_USE, id,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }

  // 物品堆叠大于1时，堆叠数量减1，否则删除这个物品对象
  if(needdelete > 0) {
    core::int32 stack_count = item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
    if(stack_count > needdelete) {
      item->SetAttribute(entity::ItemClientFields::STACK_COUNT, stack_count - needdelete);
    } else {
      if(container->RemoveItem(item, true) == false) {
        global::LogError("%s:%d (%s) 从容器中删除道具失败",
            __FILE__, __LINE__, __FUNCTION__);
        return ;
      }
      ItemBuilder::GetInstance()->Destory(item);
    }
  }
}

void ContainerMessageHandler::OnMessageItemSplit(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  // 得到玩家对象
  GameActor *game_actor = 
    GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象[%lu]没有找到", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageItemSplit message;
  if(global::GlobalPacket::GetPacket()->Deserialize(
        message, data, size) == false) {
    LOG_ERROR("解析消息 MessageItemSplit 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ITEM_SPLIT from actor(%lu, %s), dest_location_(%d), item_id_(%s), number_(%d), src_location_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.dest_location_, message.item_id_.c_str(), message.number_, message.src_location_);

  // 获取背包容器
  ItemContainer *container = actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(container == NULL) {
    LOG_ERROR("获取玩家(%lu)背包容器失败", id);
    return ;
  }

  // 获取相应位置上的道具
  Item *item = container->GetItem(message.src_location_);
  if(item == NULL || item->GetStringID() != message.item_id_) {
    LOG_ERROR("玩家(%lu)拆分时，不存在id", id);
    return ;
  }

  // 获取配置文件
  const ItemCell *cell = Configure::GetInstance()->GetItem(
      item->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("道具配置不存在[%d]", item->GetTemplateID());
    return ;
  }
  if(cell->type_ == entity::ItemType::EQUIPMENT) {
    // 分解装备
    EquipmentCell *equip_cell = (EquipmentCell *)cell;
    const LoadDropConfigure::DropElementVector &resources = 
      game::server::Configure::GetInstance()->Drop(equip_cell->split_id_);

    // 检查是不是可以添加道具
    std::vector<entity::AddItemFields> lists;
    entity::AddItemFields field;
    for(size_t i = 0; i < resources.size(); ++i) {
      if(resources[i].resource_type_ == entity::ResourceType::ITEM) {
        field.item_template_ = resources[i].resource_id_;
        field.number_ = resources[i].number_;
        field.bind_ = item->GetBind();
        lists.push_back(field);
      }
    }
    if(container->CheckAddItems(lists) == false) {
      LOG_ERROR("背包已经满了，无法添加道具");
      return ;
    }

    // 添加资源
    for(size_t i = 0; i < resources.size(); ++i) {
      if(resources[i].resource_type_ == entity::ResourceType::RESOURCE) {
        game_actor->AddResource((entity::ResourceID::type)resources[i].resource_id_,
            resources[i].number_);
      }
    }
    // 添加道具
    for(size_t i = 0; i < lists.size(); ++i) {
      if(container->AddItems(lists[i].item_template_, lists[i].number_,
            lists[i].bind_, 0) == false) {
        LOG_ERROR("添加道具[%d]失败 number[%d] bind[%d] 失败", lists[i].item_template_,
            lists[i].number_, lists[i].bind_);
        return ;
      }
    }

    // 删除装备
    if(container->RemoveItem(item) == false) {
      LOG_ERROR("删除装备失败");
      return ;
    }
  } else {
    // 位置相同，不处理
    if(message.src_location_ == message.dest_location_) {
      return ;
    }
    core::int32 stack_count = item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
    if(stack_count <= message.number_) {
      LOG_ERROR("玩家(%lu)拆分道具时，拆分个数大于堆叠数量", id);
      return ;
    }

    // 目的位置上是否已存在道具
    if(container->CheckLocationEmpty(message.dest_location_) == false) {
      LOG_ERROR("玩家(%lu)拆分道具时，目的位置上已存在道具", id);
      return ;
    }

    // 添加道具
    if(container->AddItemsByLocation(message.dest_location_, item->GetTemplateID(),
          message.number_, false, item->GetBind(), 0) == false, 0) {
      LOG_ERROR("添加道具[%d]失败", item->GetTemplateID());
      return ;
    }

    // 设置原道具的数量
    item->SetAttribute(entity::ItemClientFields::STACK_COUNT, stack_count - message.number_);
  }
}


void ContainerMessageHandler::OnMessageItemArrange(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageItemArrange message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageItemArrange 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ITEM_ARRANGE from actor(%lu, %s), container_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.container_);

  // 获取背包容器
  ItemContainer *container = actor->GetContainer(message.container_);
  if(container == NULL) {
    LOG_ERROR("获取玩家(%lu)背包容器失败", id);
    return ;
  }

  // 容器整理
  container->Arrange();
}


void ContainerMessageHandler::OnMessageExtendContainer(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  // 获取 GameActor 对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%lu) GameActor 对象失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageExtendContainer message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageItemArrange 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_EXTEND_CONTAINER from actor(%lu, %s), container_(%d), cost_type_(%d), extend_number_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.container_, message.cost_type_, message.extend_number_);

  if(message.container_ != entity::ItemContainerType::PACKET &&
      message.container_ != entity::ItemContainerType::STORAGE) {
    LOG_ERROR("只有背包和仓库才可以做格子扩展");
    return ;
  }

  // 获取背包容器
  ItemContainer *container = actor->GetContainer(message.container_);
  if(container == NULL) {
    LOG_ERROR("获取玩家(%lu)容器失败", id);
    return ;
  }

  // 扩展数量
  core::int32 can_extend = container->GetMaxCapacity() - container->GetCapacity();
  core::int32 extend_number = message.extend_number_ < can_extend ?
    message.extend_number_ : can_extend;

  if(message.cost_type_ == 1) {
    // 消耗FREEDOM_DOLLARS
    core::int32 free_dollars =
      game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
    if(free_dollars < MISC_CONF()->extend_container_gold_ * extend_number) {
      LOG_ERROR("获取扩展容器失败，FREEDOM_DOLLARS 不足");
      return ;
    }
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - MISC_CONF()->extend_container_gold_ * extend_number);
  } else {
    // 消耗RESTRICT_DOLLARS
    core::int32 restrict_dollars =
      game_actor->GetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS);
    if(restrict_dollars < MISC_CONF()->extend_container_lockedgold_ * extend_number) {
      LOG_ERROR("获取扩展容器失败，RESTRICT_DOLLARS 不足");
      return ;
    }
    game_actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, 0 - MISC_CONF()->extend_container_lockedgold_ * extend_number);
  }

  container->ExtendCapacity(extend_number);
}

void ContainerMessageHandler::OnMessageShortcutSetRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageShortcutSetRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageShortcutSetRequest 失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_SHORTCUT_SET_REQUEST from actor(%lu, %s), index_(%d), item_template_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.index_, message.item_template_);

  gateway::protocol::MessageShortcutSetResponse response;
  if(actor->SetShortcut(message.index_, message.item_template_) == false) {
    response.__set_result_(false);
  } else {
    response.__set_result_(true);
    response.__set_index_(message.index_);
    response.__set_item_template_(message.item_template_);
  }

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SHORTCUT_SET_RESPONSE);
}

}  // namespace item

}  // namespace server

}  // namespace game

