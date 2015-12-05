//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 10:13:39.
// File name: item_actor.cc
//
// Description:
// Define class ItemActor.
//

#include "game_server/server/extension/item/item_actor.h"

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_item_actor_save_types.h"
#include "database_server/protocol/storage_item_actor_login_types.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_mall_protocol_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace item {

ItemActor::ItemActor() : actor_(NULL) {}
ItemActor::~ItemActor() {}

bool ItemActor::Initialize(GameActor *actor, const database::protocol::StorageItemActorField &db_field) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 不合法",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->actor_ = actor;
  this->db_field_ = db_field;

  this->fighting_status_ = false;
  this->pvp_status_ = false;
  this->gifts_pick_num_ = 0;

  // 背包和仓库容量要在配置范围内
  core::uint32 packet_capacity = this->actor_->GetAttribute(
      entity::ActorClientFields::PACKET_CAPACITY);
  core::uint32 storage_capacity = this->actor_->GetAttribute(
      entity::ActorClientFields::STORAGE_CAPACITY);

  if(packet_capacity < MISC_CONF()->packet_min_capacity_) {
    this->actor_->SetAttribute(entity::ActorClientFields::PACKET_CAPACITY,
        MISC_CONF()->packet_min_capacity_, true);
  } else if(packet_capacity > MISC_CONF()->packet_max_capacity_) {
    this->actor_->SetAttribute(entity::ActorClientFields::PACKET_CAPACITY,
        MISC_CONF()->packet_max_capacity_, true);
  }

  if(storage_capacity < MISC_CONF()->storage_min_capacity_) {
    this->actor_->SetAttribute(entity::ActorClientFields::STORAGE_CAPACITY,
        MISC_CONF()->storage_min_capacity_, true);
  } else if(storage_capacity > MISC_CONF()->storage_max_capacity_) {
    this->actor_->SetAttribute(entity::ActorClientFields::STORAGE_CAPACITY,
        MISC_CONF()->storage_max_capacity_, true);
  }

  // 背包初始化
  if(this->packet_.Initialize(this, this->actor_->GetAttribute(
          entity::ActorClientFields::PACKET_CAPACITY), 
        MISC_CONF()->packet_max_capacity_) == false) {
    global::LogError("%s:%d (%s) 初始化 玩家(%lu) 背包失败",
        __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
    return false;
  }

  // 仓库初始化
  if(this->storage_.Initialize(this, this->actor_->GetAttribute(
          entity::ActorClientFields::STORAGE_CAPACITY),
        MISC_CONF()->storage_max_capacity_) == false) {
    global::LogError("%s:%d (%s) 初始化 玩家(%lu) 仓库失败",
        __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
    return false;
  }

  for(size_t pos = entity::VocationType::MIN; pos < entity::VocationType::MAX; ++pos) {
    // 装备栏初始化
    if(this->equip_[pos].Initialize(this, entity::EquipmentType::MAX,
          entity::EquipmentType::MAX, (entity::VocationType::type)pos) == false) {
      global::LogError("%s:%d (%s) 初始化 玩家(%lu) 装备栏失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
      return false;
    }
  }
  // 宝石初始化
  if(this->gem_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) 初始化 玩家(%lu) 宝石失败",
        __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
    return false;
  }
  
  // 临时洗练对象初始化
  if(this->equip_refine_.Initialize() == false) {
    LOG_ERROR("初始化 玩家(%lu) EquipRefineData 失败", this->actor_->GetID());
    return false;
  }

  // 商店回购列表初始化
  if(this->buyback_.Initialize(this,
        MISC_CONF()->buyback_capacity_) == false) {
    global::LogError("%s:%d (%s) 初始化 玩家(%lu) 商店回购列表失败",
        __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID());
    return false;
  }

  // 礼品仓库初始化
  if(this->treasure_.Initialize(this, MISC_CONF()->treasure_storage_capacity_,
        MISC_CONF()->treasure_storage_capacity_) == false) {
    LOG_ERROR("初始化玩家(%lu)礼品仓库失败", this->actor_->GetActorID());
    return false;
  }

  this->shortcuts_.push_back(db_field.shortcut1);
  this->shortcuts_.push_back(db_field.shortcut2);
  this->shortcuts_.push_back(db_field.shortcut3);
  this->shortcuts_.push_back(db_field.shortcut4);
  // 初始化道具快捷栏
  gateway::protocol::MessageShortcutInitialize shortcut_initialize;
  for(size_t pos = 0; pos < this->shortcuts_.size(); ++pos) {
    shortcut_initialize.shortcuts_.push_back(this->shortcuts_[pos]);
  }

  // 初始化VIP礼包协议
  gateway::protocol::MessageVipGiftsSynchronize message;
  if(this->actor_->GetAttribute(entity::ActorAoiFields::VIP) == (core::int32)
      entity::VipType::INVALID) {
    this->gifts_pick_num_ = 0;
    message.__set_status_(false);
  } else {
    this->gifts_pick_num_ = db_field.gifts_pick_num_;
    message.__set_status_(this->gifts_pick_num_ > 0 ? false : true);
  }

  // 初始化玩家购买藏宝阁数据
  gateway::protocol::MessageMallItemBuyNumInitialize buy_message;
  std::vector<std::string> buys_str_vec;
  std::vector<core::int32> temp_buy;
  global::StringSplit(db_field_.item_buy_num_.c_str(), ":", buys_str_vec);
  for(size_t i = 0; i < buys_str_vec.size(); ++i) {
    global::StringSplit(buys_str_vec[i].c_str(), ",", temp_buy);
    if(temp_buy.size() != 2) {
      return false;
    }
    entity::MallReasureBuyNum buy;
    buy.__set_item_id_(temp_buy[0]);
    buy.__set_num_(temp_buy[1]);
    this->buy_nums_map_.insert(MallBuyNumMap::value_type(temp_buy[0], buy));
    buy_message.buys_.push_back(buy);
    temp_buy.clear();
  }


  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_VIP_GIFTS_SYNCHRONIZE);

  this->SendMessage(shortcut_initialize,
      gateway::protocol::MessageType::MESSAGE_SHORTCUT_INITIALIZE);

  this->SendMessage(buy_message,
      gateway::protocol::MessageType::MESSAGE_MALL_ITEM_BUY_NUM_INITIALIZE);

  return true;
}

void ItemActor::Finalize() {
  this->actor_ = NULL;

  this->packet_.Finalize();
  this->storage_.Finalize();
  this->treasure_.Finalize();

  for(size_t pos = entity::VocationType::MIN; pos < entity::VocationType::MAX; ++pos) {
    this->equip_[pos].Finalize();
  }

  this->equip_refine_.Finalize();
  this->gem_.Finalize();
  this->buyback_.Finalize();

  for(ShortcutVector::iterator iterator = this->shortcuts_.begin();
      iterator != this->shortcuts_.end(); ++iterator) {
    *iterator = 0;
  }
  this->gifts_pick_num_ = true;
  this->buy_nums_map_.clear();
}

bool ItemActor::InitializeItem(entity::ItemContainerType::type container,
    const ItemFieldVector &items) {
  if(container == entity::ItemContainerType::PACKET) {
    if(this->packet_.InitializeItems(items) == false) {
      global::LogError("%s:%d (%s) 初始化背包容器道具失败",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->packet_.SetLoadFinish();
  } else if(container == entity::ItemContainerType::EQUIP) {
    for(size_t pos = entity::VocationType::MIN; pos < entity::VocationType::MAX; ++pos) {
      if(this->equip_[pos].InitializeItems(items) == false) {
        global::LogError("%s:%d (%s) 初始化装备容器道具失败",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
      this->equip_[pos].SetLoadFinish();
    }
  } else if(container == entity::ItemContainerType::STORAGE) {
    if(this->storage_.InitializeItems(items) == false) {
      global::LogError("%s:%d (%s) 初始化仓库容器道具失败",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->storage_.SetLoadFinish();
  } else if(container == entity::ItemContainerType::TREASURE) {
    if(this->treasure_.InitializeItems(items) == false) {
      global::LogError("%s:%d (%s) 初始化礼品仓库容器失败",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->treasure_.SetLoadFinish();
  }

  return true;
}

bool ItemActor::InitializeGem(const GemFieldVector &gems) {
  if(this->gem_.InitializeGems(this->db_field_, gems) == false) {
    global::LogError("%s:%d (%s) 初始化宝石失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->gem_.SetLoadFinish();

  return true;
}

bool ItemActor::CheckLoadFinish() const {
  for(size_t pos = entity::VocationType::MIN; pos < entity::VocationType::MAX; ++pos) {
    if(this->equip_[pos].CheckLoadFinish() == false) {
      return false;
    }
  }

  return this->packet_.CheckLoadFinish() &&
    this->storage_.CheckLoadFinish() &&
    this->gem_.CheckLoadFinish() &&
    this->treasure_.CheckLoadFinish();
}

void ItemActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetID());
  load_finish.__set_extension_type_(ExtensionType::ITEM);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
  global::LogInfo("玩家(%lu) Item扩展加载成功", this->GetID());
}

void ItemActor::Save() {
  if (this->actor_ == NULL || this->CheckLoadFinish() == false) {
    return;
  }

  this->packet_.Save();
  this->storage_.Save();
  this->gem_.Save(this->db_field_);
  this->treasure_.Save();

  database::protocol::StorageItemSaveRequest item_request;
  item_request.__set_id_(this->GetID());
  item_request.__set_contain_type_(entity::ItemContainerType::EQUIP);
  for(size_t pos = entity::VocationType::MIN; pos < entity::VocationType::MAX; ++pos) {
    this->equip_[pos].ExportDatabase(item_request);
    this->equip_[pos].ExpireCheck();
    global::LogDebug("玩家(%lu) 保存装备栏(%u) 道具信息",
        this->actor_->GetID(), pos);
  }
  // 发送消息
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        item_request, database::protocol::StorageType::STORAGE_ITEM_SAVE,
        this->actor_->GetID()) == false) {
    LOG_ERROR("玩家(%lu) 保存装备容器 道具信息失败",
        this->actor_->GetID());
    return ;
  }

  if(this->shortcuts_.size() >= 4) {
    this->db_field_.__set_shortcut1(this->shortcuts_[0]);
    this->db_field_.__set_shortcut2(this->shortcuts_[1]);
    this->db_field_.__set_shortcut3(this->shortcuts_[2]);
    this->db_field_.__set_shortcut4(this->shortcuts_[3]);
  }
  this->db_field_.__set_gifts_pick_num_(this->gifts_pick_num_);

  MallBuyNumMap::const_iterator buy_iter = this->buy_nums_map_.begin();
  while(buy_iter != this->buy_nums_map_.end()) {
    this->db_field_.item_buy_num_.append(global::ToString(buy_iter->second.item_id_));
    this->db_field_.item_buy_num_.append(",");
    this->db_field_.item_buy_num_.append(global::ToString(buy_iter->second.num_));
    this->db_field_.item_buy_num_.append(":");
    ++buy_iter;
  }
  database::protocol::StorageItemActorSaveRequest request;
  request.__set_actor_id_(this->GetID());
  request.__set_field_(this->db_field_);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_ITEM_ACTOR_SAVE, this->GetID()) == false) {
    LOG_ERROR("Send StorageItemSaveRequest to database server failed.");
    return;
  }
}

bool ItemActor::SetShortcut(size_t index, core::int32 value) {
  if(index >= this->shortcuts_.size()) {
    return false;
  }

  this->shortcuts_[index] = value;
  return true;
}

void ItemActor::DailyClean(core::int32 days) {
  // 处理 VIP 礼包
  this->gifts_pick_num_ = 0;
  gateway::protocol::MessageVipGiftsSynchronize message;
  if(entity::VipType::INVALID == this->actor_->GetAttribute(
        entity::ActorAoiFields::VIP)) {
    message.__set_status_(false);
  } else {
    message.__set_status_(true);
  }
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_VIP_GIFTS_SYNCHRONIZE);

  // 清除玩家购买限制次数
  this->buy_nums_map_.clear(); 
  // 初始化玩家购买藏宝阁数据
  gateway::protocol::MessageMallItemBuyNumInitialize buy_message;
  this->SendMessage(buy_message,
      gateway::protocol::MessageType::MESSAGE_MALL_ITEM_BUY_NUM_INITIALIZE);
}

core::int32 ItemActor::GetBuyNum(core::int32 template_id) const {
  MallBuyNumMap::const_iterator iter = this->buy_nums_map_.find(template_id);
  if(iter == this->buy_nums_map_.end()) {
    return 0;
  }
  return iter->second.num_;
}

void ItemActor::AddBuyNum(core::int32 template_id, core::int32 num) {
  MallBuyNumMap::iterator iter = this->buy_nums_map_.find(template_id);
  if(iter == this->buy_nums_map_.end()) {
    entity::MallReasureBuyNum buy;
    buy.__set_item_id_(template_id);
    buy.__set_num_(num);
    this->buy_nums_map_.insert(MallBuyNumMap::value_type(template_id, buy));
    return ;
  }
  iter->second.num_ += num;
  return ;
}

}  // namespace item

}  // namespace server

}  // namespace game

