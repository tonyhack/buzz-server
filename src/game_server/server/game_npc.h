//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-17 17:11:39.
// File name: game_npc.h
//
// Description:
// Define class GameNpc.
//

#ifndef __GAME__SERVER__GAME__NPC__H
#define __GAME__SERVER__GAME__NPC__H

#include <vector>

#include "core/base/types.h"
#include "entity/npc_types.h"
#include "game_server/protocol/game_protocol_types.h"
#include "game_server/server/game_role.h"
#include "game_server/server/load_progress.h"
#include "gateway_server/protocol/gateway_protocol_types.h"

namespace game {

namespace server {

class GameNpc : public GameRole {
  typedef std::vector<int32_t> AttributeFields;

  typedef void (*AttributeReviseFunctor)(core::int32 &value);

 public:
  using GameRole::SetAttribute;
  using GameRole::GetAttribute;

  GameNpc();
  virtual ~GameNpc();

  void Allocate(core::uint64 id);
  void Deallocate();

  bool Initialize(core::uint32 template_id, entity::DirectionType::type dir,
      bool need_reborn = true);
  void Finalize();

  // Aoi 属性
  core::int32 GetAttribute(entity::NpcAoiFields::type type) const;
  bool SetAttribute(entity::NpcAoiFields::type type,
      core::int32 value, bool synchronize = true);

  // 私有可见属性
  core::int32 GetAttribute(entity::NpcClientFields::type type) const;
  bool SetAttribute(entity::NpcClientFields::type type,
      core::int32 value, bool synchronize = true);

  inline const AttributeFields &GetNpcAoiAttributes() const {
    return this->aoi_attribute_;
  }
  inline const AttributeFields &GetNpcClientAttributes() const {
    return this->client_attribute_;
  }

  // 得到 npc 模板ID
  inline core::uint32 GetTemplateID() const {
    return this->template_id_;
  }

  // 得到加载进度
  inline LoadProgress &GetLoadProgress() {
    return this->load_progress_;
  }

  // 是否需要重生
  inline bool CheckNeedReborn() const {
    return this->need_reborn_;
  }

  // NPC类型
  inline entity::NpcKind::type GetNpcType() const {
    return this->npc_type_;
  }

  virtual entity::EntityType::type GetType() const {
    return entity::EntityType::TYPE_NPC;
  }

  // 获取出生坐标
  inline void GetBornPosition(core::uint32 &x, core::uint32 &y) {
    x = this->born_x_;
    y = this->born_y_;
  }
  inline void SetBornPosition(core::uint32 x, core::uint32 y) {
    this->born_x_ = x;
    this->born_y_ = y;
  }

  virtual void Synchronize();
  virtual void Synchronize(entity::NpcAoiFields::type type);
  virtual void Synchronize(entity::RoleAoiFields::type type);
  virtual void Synchronize(entity::RoleClientFields::type type);
  virtual void Synchronize(entity::EntityClientFields::type type);

 private:
  // 修正属性
  void ReviseAttribute(entity::NpcAoiFields::type type);
  void ReviseAttribute(entity::NpcClientFields::type type);

  // npc 模板ID
  core::uint32 template_id_;

  AttributeFields aoi_attribute_;
  AttributeFields client_attribute_;

  // 加载进度
  LoadProgress load_progress_;

  // 是否需要重生
  bool need_reborn_;

  // NPC类型
  entity::NpcKind::type npc_type_;

  // 出生点坐标
  core::uint32 born_x_;
  core::uint32 born_y_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__NPC__H

