//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 16:09:47.
// File name: game_role.h
//
// Description:
// Define class GameRole.
//

#ifndef __GAME__SERVER__GAME__ROLE__H
#define __GAME__SERVER__GAME__ROLE__H

#include <vector>

#include "core/base/types.h"
#include "entity/role_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_entity.h"
#include "game_server/server/game_server.h"
#include "game_server/server/role_attribute_update.h"
#include "game_server/server/event/game_event_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

class GameRole : public GameEntity {
  typedef std::vector<int32_t> AttributeFields;

  typedef void (*AttributeReviseFunctor)(core::int32 &value);

 public:
  using GameEntity::SetAttribute;
  using GameEntity::GetAttribute;

  GameRole();
  virtual ~GameRole();

  void Allocate();
  void Deallocate();

  void Initialize();
  void Finalize();

  // 获取属性集
  inline const AttributeFields &GetRoleAoiAttributes() const {
    return this->aoi_attribute_;
  }
  inline const AttributeFields &GetRoleClientAttributes() const {
    return this->client_attribute_;
  }
  inline const AttributeFields &GetRoleServerAttributes() const {
    return this->server_attribute_;
  }

  // Aoi属性
  core::int32 GetAttribute(entity::RoleAoiFields::type type) const;
  bool SetAttribute(entity::RoleAoiFields::type type,
      core::int32 value, bool synchronize = true);
  bool ChangeAttribute(entity::RoleAoiFields::type type,
      core::int32 increment, bool synchronize = true);

  // 私有可见属性
  core::int32 GetAttribute(entity::RoleClientFields::type type) const;
  bool SetAttribute(entity::RoleClientFields::type type,
      core::int32 value, bool synchronize = true);

  // 服务器端属性
  core::int32 GetAttribute(entity::RoleServerFields::type type) const;
  bool SetAttribute(entity::RoleServerFields::type type,
      core::int32 value, bool synchronize = true);

  // 同步属性到AOI/客户端
  virtual void Synchronize() = 0;
  virtual void Synchronize(entity::RoleAoiFields::type type) = 0;
  virtual void Synchronize(entity::RoleClientFields::type type) = 0;
  virtual void Synchronize(entity::EntityClientFields::type type) = 0;

  // 基本状态
  inline entity::RoleBasicStatus::type GetBasicStatus() const {
    return (entity::RoleBasicStatus::type)(this->GetAttribute(
          entity::RoleAoiFields::STATUS) & core::kuint8max);
  }
  void SetBasicStatus(entity::RoleBasicStatus::type type,
      bool synchronize = true);

  // 是否处于某种常见状态
  inline bool CheckCommonStatus(entity::RoleCommonStatus::type type) const {
    return (this->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) & type) != 0;
  }

  // 获取格子移动速度
  core::int32 GetGridMoveSpeed() const;

  // 移除角色死亡保护
  bool RemoveDieProtectBuff();

 protected:
  // 修正属性
  void ReviseAttribute(entity::RoleAoiFields::type type);
  void ReviseAttribute(entity::RoleClientFields::type type);
  void ReviseAttribute(entity::RoleServerFields::type type);

  void BroadcastEvent(event::EventType::type type, const void *event, size_t size);

  // 公共可见属性(需要广播)
  AttributeFields aoi_attribute_;
  // core::uint32 aoi_attribute_[entity::RoleAoiFields::AOI_END];
  // 私有可见属性(同步给客户端)
  AttributeFields client_attribute_;
  // core::uint32 client_attribute_[entity::RoleClientFields::CLIENT_END];
  // 服务器端属性(客户端不可见)
  AttributeFields server_attribute_;
  // core::uint32 server_attribute_[entity::RoleServerFields::SERVER_END];

  // 属性更新
  RoleAttributeUpdate attribute_update_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__ROLE__H

