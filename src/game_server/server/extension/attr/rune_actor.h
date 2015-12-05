//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-21 20:41:27.
// File name: rune_actor.h
//
// Description:
//
#ifndef __GAME__SERVER__ATTR__RUNE__ACTOR__H
#define __GAME__SERVER__ATTR__RUNE__ACTOR__H

#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/constants_constants.h"
#include "entity/vocation_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"

namespace database { namespace protocol {
class StorageAttrActorField;
}}

namespace game {

namespace server {

namespace attr {

class AttrActor;

class RuneActor : public core::Noncopyable {
 public:
  RuneActor();
  ~RuneActor();

  bool Initialize(AttrActor *attr_actor, 
      const database::protocol::StorageAttrActorField &db_field);
  void Finalize();

  // 初始化下发玩家
  void InitSynchronize(gateway::protocol::MessageAttrSpiritInitialize &message);
  // 下线保存
  void Save(database::protocol::StorageAttrActorField &db_field);
  // 同步槽数据
  void SlotDataSynchronize(core::uint8 location);  
  // 检查指定的槽位是否可以放置
  bool CheckPlaceSlot(core::uint32 template_id, core::uint8 location);
  // 检查槽位的职业
  bool CheckVocation(core::uint8 location, entity::VocationType::type type);
  // 得到槽的符文
  core::uint32 GetSlotTemplateID(core::uint8 location); 
  // 设置槽符文，sync 是否同步玩家属性数据
  bool SetRune(core::uint8 location, core::uint32 template_id, bool sync);
  // 获取玩家当前的职业
  entity::VocationType::type GetVocation();
  // 玩家更换职业
  void OnVocation(entity::VocationType::type src_vocation);
  // 获取符文数据
  void GetRuneSlots(std::vector<gateway::protocol::SpiritRuneData> &out);
  // 更新符文战斗力
  void UpdateRuneFightScore(entity::VocationType::type vocation, bool sync);

 private:
  // 增加属性
  void AddAttribute(core::uint32 template_id, bool sync);
  // 删除属性
  void DesAttribute(core::uint32 template_id, bool sync);

  AttrActor *attr_actor_;
  // 每个职业对应的符文槽数目
  static const core::int32 kMaxVocationSlotNum = 2;
  // 符文槽总数
  static const size_t kMaxRuneSlotNum = kMaxVocationSlotNum * 
    (core::int32)entity::VocationType::MAX;
  // 符文槽数据
  core::uint32 slots_[RuneActor::kMaxRuneSlotNum];
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__RUNE__ACTOR__H
