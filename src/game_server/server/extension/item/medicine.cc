//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 17:52:10.
// File name: medicine.cc
//
// Description:
// Define class Medicine.
//

#include "game_server/server/extension/item/medicine.h"

#include "game_server/server/configure.h"
#include "game_server/server/error_code.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/item_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_soul_request_types.h"
#include "game_server/server/request/game_task_request_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "entity/item_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

Medicine::ItemVector Medicine::items_;

Medicine::Medicine() {}
Medicine::~Medicine() {}

void Medicine::Allocate(core::uint64 id) {
  Item::Allocate(id);
}

void Medicine::Deallocate() {
  Item::Deallocate();
}

bool Medicine::Initialize(core::uint32 template_id, ItemActor *actor) {
  return Item::Initialize(template_id, actor);
}

void Medicine::Finalize() {
  return Item::Finalize();
}

void Medicine::InitializeDatabase(const database::protocol::StorageItemField &field) {
  this->SetAttribute(entity::ItemClientFields::BIND_TYPE, field.bind_, false);
  this->SetAttribute(entity::ItemClientFields::STACK_COUNT, field.number_, false);
}

void Medicine::ExportDatabaseField(database::protocol::StorageItemField &field) {
  field.__set_location_(this->GetLocation());
  field.__set_template_id_(this->GetTemplateID());
  field.__set_number_(this->GetAttribute(entity::ItemClientFields::STACK_COUNT));
  field.__set_bind_(this->GetAttribute(entity::ItemClientFields::BIND_TYPE));
}

void Medicine::ExportInitialField(entity::ItemFields &field) const {
  field.__set_id_(this->GetStringID());
  field.__set_template_id_(this->GetTemplateID());
  field.__set_type_(this->GetItemType());
  field.__set_location_(this->GetLocation());
  field.__set_item_client_fields_(this->GetItemClientAttributes());
}

void Medicine::Synchronize() {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 同步创建时 actor_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

/*
  // 发送属性到客户端
  gateway::protocol::MessageCreateMedicine message;
  message.__set_id_(this->GetStringID());
  message.__set_template_id_(this->GetTemplateID());
  message.__set_item_client_fields_(this->GetItemClientAttributes());

  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_CREATE_MEDICINE);
 */

  // 发送属性同步到客户端
  gateway::protocol::MessageItemCreateSynchronize message;
  message.__set_id_(this->GetStringID());
  message.__set_template_id_(this->GetTemplateID());
  message.__set_type_(entity::ItemType::MEDICINE);
  message.__set_item_client_fields_(this->GetItemClientAttributes());
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ITEM_CREATE_SYNCHRONIZE);
}

// TODO 其他类型处理
core::int32 Medicine::UseResult(const ItemCell* cell) {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 使用道具时 actor_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 使用道具时 获取GameActor 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(game_actor->CheckCommonStatus(entity::RoleCommonStatus::FORBID_USE_ITEM) == true) {
    global::LogError("%s:%d (%s) 使用道具时 玩家处于禁止使用物品状态",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  const MedicineCell* medicinecell =
    Configure::GetInstance()->GetMedicineConfigure(this->GetTemplateID()); 
  if(medicinecell == NULL) {
    global::LogError("%s:%d (%s) 获取 MedicineCell(%u) 失败",
        __FILE__, __LINE__, __FUNCTION__, this->GetTemplateID());
    return -1;
  }

  // 战斗状态禁止判断
  if(medicinecell->fighting_status_forbid_ && this->actor_->GetFightingStatus()) {
    LOG_ERROR("战斗状态中，禁止使用道具");
    return -1;
  }

  // 战斗状态禁止判断
  if(medicinecell->pvp_status_forbid_ && this->actor_->GetPvpStatus()) {
    LOG_ERROR("PVP状态中，禁止使用道具");
    return -1;
  }

  // 判断玩家状态是否符合使用规则
  if(medicinecell->use_state_ == entity::ItemUseRoleStatus::LIVE) {
    if((core::uint32)game_actor->GetAttribute(entity::RoleAoiFields::STATUS) == entity::RoleBasicStatus::STATUS_DIE) {
      LOG_ERROR("玩家当前状态无法使用道具");
      return -1;
    }
  } else if(medicinecell->use_state_ == entity::ItemUseRoleStatus::DIED) {
    if((core::uint32)game_actor->GetAttribute(entity::RoleAoiFields::STATUS) != entity::RoleBasicStatus::STATUS_DIE) {
      LOG_ERROR("玩家当前不是死亡状态，无法使用道具");
      return -1;
    }
  } else {
  }

  switch(medicinecell->func_type_) {
    case UseFuncType::ADD_BUFF: 
      return this->UseAddBuff(medicinecell);
      break;
    case UseFuncType::ADD_ITEM: 
      return this->UseAddItem(medicinecell);
      break;
    case UseFuncType::ADD_SOUL:
      return this->UseAddSoul(medicinecell);
      break;
    case UseFuncType::ADD_EXP:   
      return this->UseAddEXP(medicinecell);  
      break;  
    case UseFuncType::ADD_HP:  
      return this->UseAddHP(medicinecell);   
      break; 
    case UseFuncType::ADD_MP:  
      return this->UseAddMP(medicinecell);   
      break;
    case UseFuncType::ADD_NIMBUS:
      return this->UseAddNimbus(medicinecell);
      break;
    case UseFuncType::OPEN_RISK:
      return this->UseOpenRisk(medicinecell);
      break;
    case UseFuncType::OPEN_TRIP:
      return this->UseOpenTrip(medicinecell);
      break;
    case UseFuncType::USE_VIP:
      return this->UseVip(medicinecell);
      break;
    case UseFuncType::OPEN_FUNCTIONALITY:
      return this->OpenFunctionality(medicinecell);
      break;
    case UseFuncType::ADD_ENERGY:
      return this->UseAddEnergy(medicinecell);
      break;
    case UseFuncType::ADD_HP_TASLIMAN:
      return this->UseAddHpTasliman(medicinecell);
      break;
    case UseFuncType::ADD_MP_TASLIMAN:
      return this->UseAddMpTasliman(medicinecell);
      break;
    default:
      break;
  }      
  return -1;
}

core::int32 Medicine::UseAddBuff(const MedicineCell* cell) {
  // 匹配参数(至少两个)
  if(cell == NULL || cell->func_params_.empty())
    return -1;

  request::RequestBuffStart request;

  for(MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {

    // 目标类型
    core::int32 item_target_type = iter->param1_;

    // buffid
    request.__set_buff_id_(iter->param2_);
    GameActor* self = this->actor_->GetActor();

    EntityKey temp;
    if(self) {
      const EntityKey& select_target = self->GetSelectTarget();
      temp.type_ = select_target.type_;
      temp.id_ = select_target.id_;
    } else {
      temp.id_ = 0;
    }
    switch(item_target_type) {
      case entity::ItemTargetType::SELF:
        request.__set_src_entity_type_(self->GetType());
        request.__set_src_entity_id_(self->GetID());
        request.__set_dest_entity_type_(self->GetType());
        request.__set_dest_entity_id_(self->GetID());   
        break;
      case entity::ItemTargetType::OTHER_ONE:
        request.__set_src_entity_type_(self->GetType());
        request.__set_src_entity_id_(self->GetID());
        request.__set_dest_entity_type_(temp.type_);
        request.__set_dest_entity_id_(temp.id_);   
        break;
      default:
        return -1;
    }

    int ret_request = ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_START, &request, sizeof(request));
    if(ret_request != 0) {
      global::LogError("%s:%d (%s) 请求开启buff失败",
          __FILE__, __LINE__, __FUNCTION__); 
      return -1;   
    } 
  }
  return 0;
}

core::int32 Medicine::UseAddItem(const MedicineCell* cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    LOG_ERROR("参数错误");
    return 0;
  }

  ItemActor *actor = this->actor_;
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu)对象 不存在",
        __FILE__, __LINE__, __FUNCTION__, actor->GetActor()->GetID());
    return -1;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return -1;
  }

  PacketContainer *container = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu) 背包 不存在",
        __FILE__, __LINE__, __FUNCTION__, actor->GetActor()->GetID());
    return -1;
  }

  const LoadDropConfigure::DropElementVector &drops =
    game::server::Configure::GetInstance()->Drop(cell->func_params_[0].param2_);
  if(drops.empty() == true) {
    return -1;
  }

  Medicine::items_.clear();

  entity::AddItemFields field;

  LoadDropConfigure::DropElementVector::const_iterator iterator = drops.begin();
  for(; iterator != drops.end(); ++iterator) {
    if(iterator->resource_type_ == entity::ResourceType::ITEM) {
      field.__set_item_template_(iterator->resource_id_);
      field.__set_number_(iterator->number_);
      field.__set_bind_(iterator->bind_ != 0);
      Medicine::items_.push_back(field);
    }
  }

  if(Medicine::items_.empty() == false && container->CheckAddItems(Medicine::items_) == false) {
    ErrorCode::Send(actor->GetActor(), entity::ErrorType::PACKET_FULL);
    return -1;
  }

  LoadDropConfigure::DropElementVector::const_iterator iterator2 = drops.begin();
  for(; iterator2 != drops.end(); ++iterator2) {
    if(iterator2->resource_type_ == entity::ResourceType::ITEM) {
      if(container->AddItems(iterator2->resource_id_, iterator2->number_, 
            iterator2->bind_ != 0, iterator2->intensify_level_) == false) {
        LOG_ERROR("玩家(%lu) AddItems 失败", actor->GetID());
        return -1;
      }
    } else {
      game_actor->AddResource((entity::ResourceID::type)iterator2->resource_id_, 
          iterator2->number_);
    }
  }

  return 0;

/*
  for(MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {

    core::uint16 location = container->GetEmptyPosition();
    if(location == core::kuint16max) {
      global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu) 背包已满",
          __FILE__, __LINE__, __FUNCTION__, actor->GetActor()->GetID());
      return -1;
    }

    const ItemCell* itemcell = Configure::GetInstance()->GetItem(iter->param1_);
    if(itemcell == NULL) {
      global::LogError("%s:%d (%s) 请求增加道具时，模板ID(%u) 不存在",
          __FILE__, __LINE__, __FUNCTION__, iter->param1_);
      return -1;
    }

    if((core::uint32)iter->param2_ >  itemcell->stack_count_) {
      global::LogError("%s:%d (%s) 请求增加道具时，超过可叠加数量",
          __FILE__, __LINE__, __FUNCTION__);

      return -1;
    }

    Item *item = ItemBuilder::GetInstance()->Create(actor,
        (entity::ItemType::type)itemcell->type_, iter->param1_);
    if(item == NULL) {
      global::LogError("%s:%d (%s) 请求增加道具时，分配 Item对象 失败",
          __FILE__, __LINE__, __FUNCTION__);
      return -1;
    }

    item->SetAttribute(entity::ItemClientFields::STACK_COUNT, iter->param2_, false);

    if(iter->param3_ == 0) {
      item->SetAttribute(entity::ItemClientFields::BIND_TYPE, 0, false);
    } else {
      item->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1, false);
    }

    if(container->CheckAddItem(item, location) == false) {
      ItemBuilder::GetInstance()->Destory(item);
      return -1;
    }

    if(container->AddItem(item, location, true) == false) {
      item->Finalize();
      ItemBuilder::GetInstance()->Destory(item);
      return -1;
    }
  }
  return 0;
*/
}

core::int32 Medicine::UseAddSoul(const MedicineCell *cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 使用物品增加英灵时，参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 使用物品增加英灵时，actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestAddSoul request;

  for(MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {
    request.__set_actor_(this->actor_->GetID());
    request.__set_soul_id_(iter->param2_);

    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_ADD_SOUL, &request, sizeof(request)) != 0) {
      global::LogError("%s:%d (%s) 请求增加英灵失败",
          __FILE__, __LINE__, __FUNCTION__);
      return -1;
    }
    if(request.result_ != request::SoulResultType::SUCCESS) {
      LOG_ERROR("玩家(%lu)使用物品增加英灵失败", this->actor_->GetID());
      return -1;
    }
  }

  return 0;
}

// 增加玩家经验
core::int32 Medicine::UseAddEXP(const MedicineCell *cell) { 
  if(cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 使用物品增加经验时，参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 使用物品增加经验时，actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  for(MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {
    // 目标类型
    core::int32 item_target_type = iter->param1_;

    // 得到GameActor对象
    GameActor *actor = this->actor_->GetActor();
    if(actor == NULL) {
      global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
      return -1;
    }

    EntityKey temp;
    if(actor) {
      const EntityKey& select_target = actor->GetSelectTarget();
      temp.type_ = select_target.type_;
      temp.id_ = select_target.id_;
    } else {
      temp.id_ = 0;
    }

    // 判断类型是否是玩家，非玩家不能拥有经验
    if(temp.type_ == entity::EntityType::TYPE_ACTOR) {
      switch(item_target_type) {
        case entity::ItemTargetType::SELF:
          actor->AddResource(entity::ResourceID::EXP, iter->param2_);
          break;
        case entity::ItemTargetType::OTHER_ONE:
          // 根据选中的目标玩家ID， 得到玩家对象
          actor = GameActorManager::GetInstance()->GetActor(temp.id_);
          if(actor == NULL) {
            global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
                __FILE__, __LINE__, __FUNCTION__, temp.id_);
            return -1;
          }
          // 得到选中目标当前的经验值
          actor->AddResource(entity::ResourceID::EXP,iter->param2_);
          break;
        default:
          return -1;
      }

    } else {
      return -1;
    }
  }

  return 0;
}

// 增加血量
core::int32 Medicine::UseAddHP(const MedicineCell *cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 使用物品增加血量时，参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 使用物品增加血量时，actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  for(MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {
    // 目标类型
    core::int32 item_target_type = iter->param1_;
    // 得到GameActor对象
    GameActor *actor = this->actor_->GetActor();

    if(actor == NULL) {
      global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
      return -1;
    }

    EntityKey temp;
    if(actor) {
      const EntityKey& select_target = actor->GetSelectTarget();
      temp.type_ = select_target.type_;
      temp.id_ = select_target.id_;
    } else {
      temp.id_ = 0;
    }

    // 得到当前血量值
    core::int32 cur_hp = 0;

    // 得到需要增加的血量值
    core::int32 need_add_hp = iter->param2_;

    // 总的血量值
    core::int32 total_hp = 0;

    switch(item_target_type) {
      case entity::ItemTargetType::SELF:
        // 得到自己当前的血量值
        cur_hp = actor->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
        // 总的血量值 = 当前的血量值 + 需要增加的血量值
        total_hp = cur_hp + need_add_hp;
        actor->SetAttribute(entity::RoleAoiFields::CURRENT_HP, total_hp);
        break;
      case entity::ItemTargetType::OTHER_ONE:
        // 根据选中的目标玩家ID， 得到玩家对象
        actor = GameActorManager::GetInstance()->GetActor(temp.id_);

        if(actor == NULL) {
          global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
              __FILE__, __LINE__, __FUNCTION__, temp.id_);
          return -1;
        }

        // 得到选中目标当前的血量值
        cur_hp = actor->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
        // 总的血量值 = 当前的血量值 + 需要增加的血量值
        total_hp = cur_hp + need_add_hp;
        actor->SetAttribute(entity::RoleAoiFields::CURRENT_HP, total_hp);
        break;
      default:
        return -1;
    }
  }

  return 0;

}

// 增加魔法
core::int32 Medicine::UseAddMP(const MedicineCell *cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 使用物品增加魔法时，参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 使用物品增加魔法时，actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  for(MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {
    // 目标类型
    core::int32 item_target_type = iter->param1_;
    // 得到GameActor对象
    GameActor *actor = this->actor_->GetActor();

    if(actor == NULL) {
      global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
      return -1;
    }

    EntityKey temp;
    if(actor) {
      const EntityKey& select_target = actor->GetSelectTarget();
      temp.type_ = select_target.type_;
      temp.id_ = select_target.id_;
    } else {
      temp.id_ = 0;
    }

    // 得到当前魔法值
    core::int32 cur_mp = 0;

    // 得到需要增加的魔法值
    core::int32 need_add_mp = iter->param2_;

    // 总的魔法值
    core::int32 total_mp = 0;

    switch(item_target_type) {
      case entity::ItemTargetType::SELF:
        // 得到自己当前的魔法值
        cur_mp = actor->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
        // 总的魔法值 = 当前的魔法值 + 需要增加的魔法值
        total_mp = cur_mp + need_add_mp;
        actor->SetAttribute(entity::RoleAoiFields::CURRENT_MP, total_mp);
        break;
      case entity::ItemTargetType::OTHER_ONE:
        // 根据选中的目标玩家ID， 得到玩家对象
        actor = GameActorManager::GetInstance()->GetActor(temp.id_);
        if(actor == NULL) {
          global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
              __FILE__, __LINE__, __FUNCTION__, temp.id_);
          return -1;
        }

        // 得到选中目标当前的魔法值
        cur_mp = actor->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
        // 总的魔法值 = 当前的魔法值 + 需要增加的魔法值
        total_mp = cur_mp + need_add_mp;
        actor->SetAttribute(entity::RoleAoiFields::CURRENT_MP, total_mp);
        break;
      default:
        return -1;
    }
  }

  return 0;

}

core::int32 Medicine::UseAddNimbus(const MedicineCell *cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  for(MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {
    // 目标类型
    core::int32 item_target_type = iter->param1_;
    // 得到GameActor对象
    GameActor *actor = this->actor_->GetActor();

    if(actor == NULL) {
      global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
      return -1;
    }

    EntityKey temp;
    if(actor) {
      const EntityKey& select_target = actor->GetSelectTarget();
      temp.type_ = select_target.type_;
      temp.id_ = select_target.id_;
    } else {
      temp.id_ = 0;
    }

    core::int32 cur = 0;
    core::int32 need_add = iter->param2_;
    core::int32 total = 0;

    switch(item_target_type) {
      case entity::ItemTargetType::SELF:
        cur = actor->GetAttribute(entity::ActorClientFields::NIMBUS);
        total = cur + need_add;
        actor->AddResource(entity::ResourceID::NIMBUS, total);
        break;
      case entity::ItemTargetType::OTHER_ONE:
        // 根据选中的目标玩家ID， 得到玩家对象
        actor = GameActorManager::GetInstance()->GetActor(temp.id_);
        if(actor == NULL) {
          global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
              __FILE__, __LINE__, __FUNCTION__, temp.id_);
          return -1;
        }

        cur = actor->GetAttribute(entity::ActorClientFields::NIMBUS);
        total = cur + need_add;
        actor->AddResource(entity::ResourceID::NIMBUS, need_add);
        break;
      default:
        return -1;
    }
  }

  return 0;
}

core::int32 Medicine::UseOpenRisk(const MedicineCell *cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return -1;
  }

  if(cell->func_params_.size() != 1) {
    LOG_ERROR("配置文件错误  MedicineCell [%d]", cell->func_params_.size());
    return -1;
  }
  if(game_actor->CheckFunctionalityState(entity::FunctionalityType::TASK_RISK) == false) {
    // 开启历险任务功能模块
    game_actor->SetFunctionalityState(entity::FunctionalityType::TASK_RISK);
  }
  if(game_actor->CheckFunctionalityState(entity::FunctionalityType::TASK_RISK) == true) {
    // 发送第一个历险任务
    request::RequestTaskAccept request;
    request.__set_actor_id_(game_actor->GetActorID());
    request.__set_type_(entity::TaskType::TASK_RISK);
    request.__set_task_id_(cell->func_params_[0].param2_);
    return ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TASK_ACCEPT, &request, sizeof(request));
  }
  return -1;
}

core::int32 Medicine::UseOpenTrip(const MedicineCell *cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return -1;
  }

  if(cell->func_params_.size() != 1) {
    LOG_ERROR("配置文件错误  MedicineCell [%d]", cell->func_params_.size());
    return -1;
  }
  if(game_actor->CheckFunctionalityState(entity::FunctionalityType::TASK_TRIP) == false) {
    // 开启奇遇任务功能模块
    game_actor->SetFunctionalityState(entity::FunctionalityType::TASK_TRIP);
  }
  if(game_actor->CheckFunctionalityState(entity::FunctionalityType::TASK_TRIP) == true) {
    // 发送奇遇任务
    request::RequestTaskAccept request;
    request.__set_actor_id_(game_actor->GetActorID());
    request.__set_type_(entity::TaskType::TASK_TRIP);
    request.__set_task_id_(cell->func_params_[0].param2_);
    return ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TASK_ACCEPT, &request, sizeof(request));
  }
  return 0;
}

core::int32 Medicine::UseVip(const MedicineCell *cell) {
  if(cell == NULL || cell->func_params_.empty()) {
    LOG_ERROR("配置出错或者参数为空(%d)", cell->func_params_.empty());
    return -1;
  }
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象还没有找到");
    return -1;
  }
  if(cell->func_params_.size() != 1) {
    LOG_ERROR("参数错误(%d)", cell->func_params_.size());
    return -1;
  }
  // 使用 VIP卡
  if(cell->func_params_[0].param1_>= entity::VipType::MAX ||
      cell->func_params_[0].param1_ <= 0) {
    LOG_ERROR("VIP卡(%s)配置出错(%d)", cell->name_.c_str(),
        cell->func_params_[0].param1_);
    return -1;
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return -1;
  }
  return game_actor->AlterVip((entity::VipType::type)cell->func_params_[0].param1_,
      cell->func_params_[0].param2_, true) == true ? 0 : -1;
}


core::int32 Medicine::OpenFunctionality(const MedicineCell *cell) {
  if(cell == NULL) {
    return -1;
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return -1;
  }
  // 开启功能
  LoadFunctionalityConfigure::OpenCells& cells = game::server::Configure::GetInstance()->GetFunctionalityCell(
      entity::FunctionalityOpenType::USE_ITEM);
  for(core::uint32 i = 0; i < cells.size(); ++i) {
    if(cells[i].param_ == cell->template_id_ &&
        game_actor->CheckFunctionalityState(cells[i].function_) == false) {
      game_actor->SetFunctionalityState(cells[i].function_, true, true);
    }
  }
  return 0;
}

core::int32 Medicine::UseAddEnergy(const MedicineCell *cell) {
  if (cell == NULL || cell->func_params_.empty()) {
    global::LogError("%s:%d (%s) 使用物品增加能量时，参数为空或配置错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  if (this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 使用物品增加能量时，actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  for (MedicineCell::FunctionParamsVector::const_iterator iter =
      cell->func_params_.begin(); iter != cell->func_params_.end(); ++iter) {
    // 目标类型
    core::int32 item_target_type = iter->param1_;
    // 得到GameActor对象
    GameActor *actor = this->actor_->GetActor();

    if (actor == NULL) {
      global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
      return -1;
    }

    EntityKey temp;
    if (actor) {
      const EntityKey& select_target = actor->GetSelectTarget();
      temp.type_ = select_target.type_;
      temp.id_ = select_target.id_;
    } else {
      temp.id_ = 0;
    }

    // 得到当前能量值
    core::int32 cur_energy = 0;

    // 得到需要增加的能量值
    core::int32 need_add_energy = iter->param2_;

    // 总的能量值
    core::int32 total_energy = 0;

    switch (item_target_type) {
      case entity::ItemTargetType::SELF:
        // 得到自己当前的能量值
        cur_energy = actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY);
        // 总的能量值 = 当前的能量值 + 需要增加的能量值
        total_energy = cur_energy + need_add_energy;
        actor->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY, total_energy);
        break;
      case entity::ItemTargetType::OTHER_ONE:
        // 根据选中的目标玩家ID， 得到玩家对象
        actor = GameActorManager::GetInstance()->GetActor(temp.id_);

        if(actor == NULL) {
          global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
              __FILE__, __LINE__, __FUNCTION__, temp.id_);
          return -1;
        }

        // 得到选中目标当前的能量值
        cur_energy = actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY);
        // 总的能量值 = 当前的能量值 + 需要增加的能量值
        total_energy = cur_energy + need_add_energy;
        actor->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY, total_energy);
        break;
      default:
        return -1;
    }

  }

  return 0;
}


core::int32 Medicine::UseAddHpTasliman(const MedicineCell *cell) {
  if(cell == NULL) {
    LOG_ERROR("使用血护符道具，参数错误"); 
    return -1;
  }
  GameActor *actor = this->actor_->GetActor();
  if(actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return -1;
  }
  if(cell->func_params_.size() != 1) {
    LOG_ERROR("血护符道具功能参数错误");
    return -1;
  }
  if(actor->AddHpTasliman(cell->func_params_[0].param2_) == false) {
    LOG_ERROR("增加血护符值(%d)失败", cell->func_params_[0].param2_);
    return -1;
  }

  return 0;
}

core::int32 Medicine::UseAddMpTasliman(const MedicineCell *cell) {
  if(cell == NULL) {
    LOG_ERROR("使用蓝护符道具，参数错误"); 
    return -1;
  }
  GameActor *actor = this->actor_->GetActor();
  if(actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return -1;
  }
  if(cell->func_params_.size() != 1) {
    LOG_ERROR("蓝护符道具功能参数错误");
    return -1;
  }
  if(actor->AddMpTasliman(cell->func_params_[0].param2_) == false) {
    LOG_ERROR("增加蓝护符值(%d)失败", cell->func_params_[0].param2_);
    return -1;
  }

  return 0;
}

}  // namespace item

}  // namespace server

}  // namespace game
