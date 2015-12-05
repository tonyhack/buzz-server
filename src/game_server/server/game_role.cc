//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 16:17:00.
// File name: game_role.cc
//
// Description:
//

#include "game_server/server/game_role.h"

#include "coresh/communicator.h"
#include "game_server/server/configure.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

GameRole::GameRole() : attribute_update_(this) {}

GameRole::~GameRole() {}

void GameRole::Allocate() {
  GameEntity::Allocate();
  this->aoi_attribute_.resize(entity::RoleAoiFields::AOI_END, 0);
  this->client_attribute_.resize(entity::RoleClientFields::CLIENT_END, 0);
  this->server_attribute_.resize(entity::RoleServerFields::SERVER_END, 0);
  this->attribute_update_.Initialize();
}

void GameRole::Deallocate() {
  GameEntity::Deallocate();
  this->attribute_update_.Finalize();
  this->aoi_attribute_.assign(this->aoi_attribute_.size(), 0);
  this->client_attribute_.assign(this->client_attribute_.size(), 0);
  this->server_attribute_.assign(this->server_attribute_.size(), 0);
}

void GameRole::Initialize() {
  GameEntity::Initialize();
  this->SetScene(0);
  this->SetPosition(0, 0);
  this->SetPosOffset(0, 0);
}

void GameRole::Finalize() {
  GameEntity::Initialize();
}

core::int32 GameRole::GetAttribute(entity::RoleAoiFields::type type) const {
  if(type < entity::RoleAoiFields::AOI_END) {
    return this->aoi_attribute_[type];
  }
  return 0;
}

bool GameRole::SetAttribute(entity::RoleAoiFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::RoleAoiFields::AOI_END) {
    // 血量/魔法值不能超过上限
    if(type == entity::RoleAoiFields::CURRENT_HP) {
      if(value > this->GetAttribute(entity::RoleAoiFields::MAX_HP)) {
        value = this->GetAttribute(entity::RoleAoiFields::MAX_HP);
      }
    } else if(type == entity::RoleAoiFields::CURRENT_MP) {
      if(value > this->GetAttribute(entity::RoleAoiFields::MAX_MP)) {
        value = this->GetAttribute(entity::RoleAoiFields::MAX_MP);
      }
    }
    // 原属性
    core::int32 src_value = this->aoi_attribute_[type];
    // 设置属性
    this->aoi_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    // 更新相关属性
    this->attribute_update_.Update(type, synchronize);
    if(synchronize) {
      this->Synchronize(type);
      // 生命、最大生命、魔法、最大魔法改变时发事件
      if(type == entity::RoleAoiFields::CURRENT_HP) {
        event::EventRoleChangeHp event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_value_(this->GetAttribute(type));
        this->BroadcastEvent(event::EventType::EVENT_ROLE_CHANGE_HP,
            &event, sizeof(event::EventRoleChangeHp));
      } else if(type == entity::RoleAoiFields::MAX_HP) {
        event::EventRoleChangeMaxHp event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_value_(this->GetAttribute(type));
        this->BroadcastEvent(event::EventType::EVENT_ROLE_CHANGE_MAX_HP,
            &event, sizeof(event::EventRoleChangeMaxHp));
      } else if(type == entity::RoleAoiFields::CURRENT_MP) {
        event::EventRoleChangeMp event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_value_(this->GetAttribute(type));
        this->BroadcastEvent(event::EventType::EVENT_ROLE_CHANGE_MP,
            &event, sizeof(event::EventRoleChangeMp));
      } else if(type == entity::RoleAoiFields::MAX_MP) {
        event::EventRoleChangeMaxMp event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_value_(this->GetAttribute(type));
        this->BroadcastEvent(event::EventType::EVENT_ROLE_CHANGE_MAX_MP,
            &event, sizeof(event::EventRoleChangeMaxMp));
      } else if(type == entity::RoleAoiFields::VOCATION) {
        event::EventRoleChangeVocation event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_src_vocation_((entity::VocationType::type)src_value);
        this->BroadcastEvent(event::EventType::EVENT_ROLE_CHANGE_VOCATION,
            &event, sizeof(event::EventRoleChangeVocation));
      } else if(type == entity::RoleAoiFields::COMMON_STATUS) {
        event::EventRoleCommonStatusChange event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_from_(src_value);
        event.__set_to_(this->GetAttribute(type));
        this->BroadcastEvent(event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE,
            &event, sizeof(event::EventRoleCommonStatusChange));
      }
    }

    return true;
  }
  return false;
}

bool GameRole::ChangeAttribute(entity::RoleAoiFields::type type,
    core::int32 increment, bool synchronize) {
  return this->SetAttribute(type, this->aoi_attribute_[type] + increment, synchronize);
}

core::int32 GameRole::GetAttribute(entity::RoleClientFields::type type) const {
  if(type < entity::RoleClientFields::CLIENT_END) {
    return this->client_attribute_[type];
  }
  return 0;
}

bool GameRole::SetAttribute(entity::RoleClientFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::RoleClientFields::CLIENT_END) {
    if (type == entity::RoleClientFields::CURRENT_ENERGY) {
      if (value > this->GetAttribute(entity::RoleClientFields::MAX_ENERGY)) {
        value = this->GetAttribute(entity::RoleClientFields::MAX_ENERGY);
      }
    }
    // 原属性
    core::int32 src_value = this->client_attribute_[type];
    // 设置属性
    this->client_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    if (type == entity::RoleClientFields::MAX_ENERGY) {
      core::int32 current_energy = this->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY);
      if (current_energy == src_value) {
        this->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY, value, synchronize);
      }
    }
    // 更新相关属性
    this->attribute_update_.Update(type, synchronize);
    if(synchronize) {
      this->Synchronize(type);
      if (type == entity::RoleClientFields::CURRENT_ENERGY) {
        event::EventRoleChangeCurrentEnergy event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_value_(src_value);
        this->BroadcastEvent(event::EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY,
            &event, sizeof(event::EventRoleChangeCurrentEnergy));
      } else if (type == entity::RoleClientFields::MAX_ENERGY) {
        event::EventRoleChangeMaxEnergy event;
        event.__set_type_(this->GetType());
        event.__set_id_(this->GetID());
        event.__set_value_(src_value);
        this->BroadcastEvent(event::EventType::EVENT_ROLE_CHANGE_MAX_ENERGY,
            &event, sizeof(event::EventRoleChangeMaxEnergy));
      }
    }
    return true;
  }
  return false;
}

core::int32 GameRole::GetAttribute(entity::RoleServerFields::type type) const {
  if(type < entity::RoleServerFields::SERVER_END) {
    return this->server_attribute_[type];
  }
  return 0;
}

bool GameRole::SetAttribute(entity::RoleServerFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::RoleServerFields::SERVER_END) {
    // 设置属性
    this->server_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    // 更新相关属性
    this->attribute_update_.Update(type, synchronize);
    return true;
  }
  return false;
}

void GameRole::SetBasicStatus(entity::RoleBasicStatus::type type,
    bool synchronize) {
  entity::RoleBasicStatus::type from = this->GetBasicStatus();
  if(from == type) {
    return ;
  }
  // 设置状态属性
  this->SetAttribute(entity::RoleAoiFields::STATUS,
      ((this->GetAttribute(entity::RoleAoiFields::STATUS)) & (~core::kuint8max)) |
      (core::uint32)(type & core::kuint8max), synchronize);
  // 不同步时也不发送事件
  if(synchronize == false) {
    return ;
  }
  // 状态改变发事件
  event::EventBasicStatusChange event;
  event.__set_type_(this->GetType());
  event.__set_id_(this->GetID());
  event.__set_from_(from);
  event.__set_to_(type);
  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(this->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(this->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  } else {
    global::LogError("%s:%d (%s) 设置基本状态时，实体类型错误(%d, %lu)",
        __FILE__, __LINE__, __FUNCTION__, this->GetType(), this->GetID());
  }

  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, this->GetID(), channel_type,
      &event, sizeof(event));
}

core::int32 GameRole::GetGridMoveSpeed() const {
  // 速度单位转换: 移动速度[ms/格]=1000/移动速度[像素/秒] * 32
  core::int32 speed = this->GetAttribute(entity::RoleAoiFields::MOVE_SPEED);
  return 1000 * Configure::kGridPixelWidth_ / speed;
}

bool GameRole::RemoveDieProtectBuff() {
  if(this->CheckCommonStatus(entity::RoleCommonStatus::FORBID_BEATEN_MAP) == 
      entity::RoleAfterDieStatus::PROTECT) {

    request::RequestBuffEnd request;
    request.__set_dest_entity_type_(this->GetType());
    request.__set_dest_entity_id_(this->GetID());
    request.__set_buff_id_(MISC_CONF()->protect_buff_id_);

    int ret_request = ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_END, &request, sizeof(request));
    if(ret_request != 0) {
      global::LogError("%s:%d (%s) 请求关闭buff失败",
          __FILE__, __LINE__, __FUNCTION__); 
      return false;
    }

    return request.succeed_;
  }
  return true;
}

void GameRole::ReviseAttribute(entity::RoleAoiFields::type type) {
  static AttributeReviseFunctor functions[entity::RoleAoiFields::AOI_END] = {
    NonNegativeRevise,          // 等级
    NonNegativeRevise,          // 当前职业
    NonNegativeRevise,          // 当前血量
    NonNegativeRevise,          // 最大血量
    NonNegativeRevise,          // 当前魔法值
    NonNegativeRevise,          // 最大魔法值
    NonNegativeRevise,          // 移动速度
    NULL,                       // 状态
    NULL,                       // 常见状态
    NULL,                       // 阵营
  };
  // 修正
  if(functions[type]) {
    functions[type](this->aoi_attribute_[type]);
  }
}

void GameRole::ReviseAttribute(entity::RoleClientFields::type type) {
  static AttributeReviseFunctor functions[entity::RoleClientFields::CLIENT_END] = {
    NonNegativeRevise,          // 体质
    NonNegativeRevise,          // 力量
    NonNegativeRevise,          // 智力
    NonNegativeRevise,          // 敏捷
    NonNegativeRevise,          // 精神
    NonNegativeRevise,          // 物理攻击
    NonNegativeRevise,          // 物理防御
    NonNegativeRevise,          // 魔法攻击
    NonNegativeRevise,          // 魔法防御
    NonNegativeRevise,          // 闪避等级
    NonNegativeRevise,          // 暴击等级
    NonNegativeRevise,          // 会心等级(忽视防御)
    NonNegativeRevise,          // 当前能量值
    NonNegativeRevise,          // 最大能量值
    NonNegativeRevise,          // 单位时间能量恢复值
  };
  // 修正
  if(functions[type]) {
    functions[type](this->client_attribute_[type]);
  }
}

void GameRole::ReviseAttribute(entity::RoleServerFields::type type) {
  static AttributeReviseFunctor functions[entity::RoleServerFields::SERVER_END] = {
    NULL,                       // 最大血量加成值
    NULL,                       // 最大血量加成比(万分之)
    NULL,                       // 最大魔法加成值
    NULL,                       // 最大魔法加成比(万分之)
    NULL,                       // 移动速度加成值
    NULL,                       // 移动速度加成比(万分之)
    NULL,                       // 体质加成值
    NULL,                       // 体质加成比(万分之)
    NULL,                       // 力量加成值
    NULL,                       // 力量加成比(万分之)
    NULL,                       // 智力加成值
    NULL,                       // 智力加成比(万分之)
    NULL,                       // 敏捷加成值
    NULL,                       // 敏捷加成比(万分之)
    NULL,                       // 精神加成值
    NULL,                       // 精神加成比(万分之)
    NULL,                       // 物理攻击加成值
    NULL,                       // 物理攻击加成比(万分之)
    NULL,                       // 物理防御加成值
    NULL,                       // 物理防御加成比(万分之)
    NULL,                       // 魔法攻击加成值
    NULL,                       // 魔法攻击加成比(万分之)
    NULL,                       // 魔法防御加成值
    NULL,                       // 魔法防御加成比(万分之)
    NULL,                       // 闪避等级加成值
    NULL,                       // 闪避等级加成比(万分之)
    NULL,                       // 闪避率加成值(万分之)
    NULL,                       // 暴击等级加成值
    NULL,                       // 暴击等级加成比(万分之)
    NULL,                       // 暴击率加成值(万分之)
    NULL,                       // 暴击效果加成值(万分之)
    NULL,                       // 会心等级加成值
    NULL,                       // 会心等级加成比(万分之)
    NULL,                       // 会心率加成值(万分之)
    NULL,                       // 物伤累计减免率(万分之)
    NULL,                       // 法伤累计减免率(万分之)
    NULL,                       // 伤害累计减免率(万分之)
    NULL,                       // 伤害浮动比加成值(万分之)
    NULL,                       // 治疗加成比(万分之)
    NULL,                       // 最大能量加成值
    NULL,                       // 最大能量二级加成值
    NULL,                       // 最大能量加成比(万分之)
    NULL,                       // 能量恢复加成值
    NULL,                       // 能量恢复二级加成值
    NULL,                       // 能量恢复加成比(万分之)
  };
  // 修正
  if(functions[type]) {
    functions[type](this->server_attribute_[type]);
  }
}

void GameRole::BroadcastEvent(event::EventType::type type, const void *event, size_t size) {
  core::uint8 channel_type = event::ChannelType::CHANNEL_MAX;
  if(this->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(this->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(type,
      this->GetID(), channel_type, event, size);
}

}  // namespace server

}  // namespace game

