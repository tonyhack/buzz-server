//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 10:07:35.
// File name: item_actor.h
//
// Description:
// Define class ItemActor.
//

#ifndef __GAME__SERVER__ITEM__ITEM__ACTOR__H
#define __GAME__SERVER__ITEM__ITEM__ACTOR__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/storage_item_actor_login_types.h"
#include "database_server/protocol/storage_item_login_types.h"
#include "database_server/protocol/storage_gem_login_types.h"
#include "entity/constants_constants.h"
#include "entity/mall_types.h"
#include "entity/item_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/item/equipment_container.h"
#include "game_server/server/extension/item/gem_container.h"
#include "game_server/server/extension/item/item_container.h"
#include "game_server/server/extension/item/packet_container.h"
#include "game_server/server/extension/item/storage_container.h"
#include "game_server/server/extension/item/shop_buyback_container.h"
#include "game_server/server/extension/item/treasure_container.h"

namespace game {

namespace server {

namespace item {

class EquipRefineData {
  typedef std::vector<core::uint8> AttrTypeVector;
  typedef std::vector<core::int32> AttrValueVector;
 public:
  EquipRefineData() {}

  bool Initialize() {
    iseffect_ = false;
    equip_id_ = 0;
    rand_attr_type_.resize(entity::g_constants_constants.kMaxRandomAttriNum, 0);
    rand_attr_value_.resize(entity::g_constants_constants.kMaxRandomAttriNum, 0);
    return true;
  }
  void Finalize() {
  }

  AttrTypeVector rand_attr_type_;
  AttrValueVector rand_attr_value_;
  core::uint64 equip_id_;
  bool iseffect_;
};

class ItemActor : public core::Noncopyable {
  typedef std::vector<database::protocol::StorageItemField> ItemFieldVector;
  typedef std::vector<database::protocol::StorageGemField> GemFieldVector;
  typedef std::vector<core::int32> ShortcutVector;
  typedef std::map<core::int32, entity::MallReasureBuyNum> MallBuyNumMap;

 public:
  ItemActor();
  ~ItemActor();

  bool Initialize(GameActor *actor, const database::protocol::StorageItemActorField &db_field);
  void Finalize();

  bool InitializeItem(entity::ItemContainerType::type container,
      const ItemFieldVector &items);
  bool InitializeGem(const GemFieldVector &gems);

  bool CheckLoadFinish() const;
  void SendLoadFinishEvent();

  void Save();

  inline entity::VocationType::type GetActorVocation() const {
    if(this->actor_) {
      return (entity::VocationType::type)this->actor_->GetAttribute(
          entity::RoleAoiFields::VOCATION);
    }
    return entity::VocationType::MIN;
  }

  inline GameActor *GetActor() {
    return this->actor_;
  }

  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return -1;
  }

  inline const std::string GetStringID() const {
    if(this->actor_) {
      return this->actor_->GetStringID();
    }
    return std::string();
  }

  inline const std::string GetName() const {
    if(this->actor_) {
      return this->actor_->GetName();
    }
    return std::string();
  }

  // 发送消息
  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if(this->actor_) {
      this->actor_->SendMessage(message, type);
    }
  }

  // 发送消息到周围
  template <typename T>
  void SendMessageAround(const T &message, gateway::protocol::MessageType::type type) {
    if(this->actor_) {
      this->actor_->SendMessageAround(message, type);
    }
  }

  inline ItemContainer *GetContainer(entity::ItemContainerType::type type,
      entity::VocationType::type vocation = entity::VocationType::MAX) {
    if(vocation < entity::VocationType::MIN || vocation >= entity::VocationType::MAX) {
      vocation = this->GetActorVocation();
    }
    switch(type) {
      case entity::ItemContainerType::PACKET:
        return &this->packet_;
      case entity::ItemContainerType::EQUIP:
        return &this->equip_[vocation];
      case entity::ItemContainerType::STORAGE:
        return &this->storage_;
      case entity::ItemContainerType::SHOP:
        return &this->buyback_;
      case entity::ItemContainerType::TREASURE:
        return &this->treasure_;
      default:
        break;
    }
    return NULL;
  }

  inline EquipRefineData *GetEquipRefineData() {
    return &equip_refine_;
  }

  inline GemContainer *GetGemContainer() {
    return &this->gem_;
  }

  inline void SetPvpStatus(bool status) {
    this->pvp_status_ = status;
  }
  inline bool GetPvpStatus() const {
    return this->pvp_status_;
  }

  inline void SetFightingStatus(bool status) {
    this->fighting_status_ = status;
  }
  inline bool GetFightingStatus() const {
    return this->fighting_status_;
  }
  inline core::int32 GetVipGiftsPickNum() const {
    return this->gifts_pick_num_;
  }
  inline void SetVipGiftsPickNum(core::int32 num) {
    this->gifts_pick_num_ = num;
  }
  void DailyClean(core::int32 days);

  bool SetShortcut(size_t index, core::int32 value);

  core::int32 GetBuyNum(core::int32 template_id) const;

  void AddBuyNum(core::int32 template_id, core::int32 num);

 private:
  GameActor *actor_;
  database::protocol::StorageItemActorField db_field_;

  // 背包
  PacketContainer packet_;
  // 仓库
  StorageContainer storage_;
  // 装备栏
  EquipmentContainer equip_[entity::VocationType::MAX];
  // 宝石
  GemContainer gem_;
  // 洗练装备临时保存
  EquipRefineData equip_refine_;
  // 商店回购列表
  ShopBuybackContainer buyback_;  
  // 礼品仓库
  TreasureContainer treasure_;

  // PVP状态
  bool pvp_status_;
  // 战斗状态
  bool fighting_status_;
  // vip 礼包(tue:领过了 false 没领取过)
  core::int32 gifts_pick_num_;

  ShortcutVector shortcuts_;
  // 玩家购买藏宝阁数据
  MallBuyNumMap buy_nums_map_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__ACTOR__H

