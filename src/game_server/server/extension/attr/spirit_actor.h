#ifndef __GAME__SERVER__ATTR__SPIRIT__ACTOR__H
#define __GAME__SERVER__ATTR__SPIRIT__ACTOR__H

#include <ctime>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/spirit_types.h"
#include "database_server/protocol/storage_attr_actor_login_types.h"
#include "database_server/protocol/storage_spirit_facade_login_types.h"

namespace game {

namespace server {

namespace attr {

class AttrActor;

class SpiritFacade {
 public:
  SpiritFacade(core::int32 id, time_t expire_time)
    : id_(id), expire_time_(expire_time), exist_(true) {}

  core::int32 GetID() const { return id_; }
  time_t GetExpireTime() const { return expire_time_; }
  void SetExpireTime(time_t expire_time) { this->expire_time_ = expire_time; }
  bool CheckExist() const { return this->exist_; }
  void SetExist(bool exist) { this->exist_ = exist; }

 private:
  core::int32 id_;
  time_t expire_time_;
  bool exist_;
};

class SpiritActor : public core::Noncopyable {
 public:
  typedef std::vector<database::protocol::StorageSpiritFacadeField> SpiritFacadeFieldVector;
  typedef std::vector<SpiritFacade> SpiritFacadeVector;
  typedef __gnu_cxx::hash_map<core::int32, size_t> SpiritFacadeIndexHashmap;

  SpiritActor();
  ~SpiritActor();

  bool Initialize(AttrActor *attr_actor,
      const database::protocol::StorageAttrActorField &db_field,
      const SpiritFacadeFieldVector &facades);
  void Finalize();
  bool CheckLoadFinish() const { return this->load_finish_; }
  void SetLoadFinish(bool finish = true) { this->load_finish_ = finish; }
  void Save(database::protocol::StorageAttrActorField &db_field);

  // 开启野兽形态功能
  bool EnableHorseFacadeFunc(bool synchronize = true);
  // 开启翅膀形态功能
  bool EnableWingFacadeFunc(bool synchronize = true);

  // 当前精灵形态ID
  core::int32 GetCurrentFacadeID() const;
  core::int32 GetCurrentLevel() const { return this->level_; }
  core::int32 GetCurrentFacadeType() const { return this->current_facade_type_; }
  core::int32 GetHorseFacadeID() const { return this->horse_facade_id_; }
  core::int32 GetWingFacadeID() const { return this->wing_facade_id_; }
  const SpiritFacadeVector &GetFacades() const { return this->facades_store_; }

  // 精灵进阶
  void SetCurrentLevel(core::int32 new_level, bool synchronize = true);
  core::int32 GetLuckyValue() const { return this->lucky_value_; }
  void SetLuckyValue(core::int32 lucky_value, bool synchronize = true);
  // 新增精灵外观
  bool AddFacade(core::int32 facade_id, core::int32 duration_time = 0,
      bool synchronize = true);
  // 改变当前精灵外观
  bool TransformFacade(core::int32 facade_id, bool synchronize = true);

  // 每日清理
  void DailyClean();

  // 精灵变身ID设置
  void SetFacadeID(core::int32 id) {
    this->facade_id_ = id;
  }
  // 获取精灵变身的ID
  core::int32 GetFacadeID() const {
    return this->facade_id_;
  }

  void UpdateSpiritFightScore(bool synchronize = true);

 private:
  void SendInitToClient();
  bool AddSpiritAttr(bool synchronize = true);
  bool AddFacadeBuff(core::int32 facade_type);
  bool RemoveFacadeBuff(core::int32 facade_type);
  void RemoveExpiredFacade(bool synchronize = true);
  
  bool load_finish_;
  AttrActor *attr_actor_;
  core::int32 level_;
  core::int32 lucky_value_;
  core::int32 current_facade_type_;
  core::int32 horse_facade_id_;
  core::int32 wing_facade_id_;
  core::int32 facade_id_;                             // 精灵转换的外观id
  SpiritFacadeIndexHashmap facades_index_;
  SpiritFacadeVector facades_store_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__SPIRIT__ACTOR__H

