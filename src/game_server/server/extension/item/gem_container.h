#ifndef __GAME__SERVER__ITEM__GEM__CONTAINER__H
#define __GAME__SERVER__ITEM__GEM__CONTAINER__H

#include <vector>
#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/gem_types.h"
#include "database_server/protocol/storage_item_actor_login_types.h"
#include "database_server/protocol/storage_gem_login_types.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class GemContainer : public core::Noncopyable {
 public:
  struct Location {
    Location(core::int32 type, core::int16 index)
      : type_(type), index_(index) {}

    bool operator<(const Location &other) const {
      if (this->type_ != other.type_) {
        return this->type_ < other.type_;
      }
      return this->index_ < other.index_;
    }

    core::int32 type_;
    core::int16 index_;
  };

  struct Gem {
    Gem() {}
    Gem(core::int32 id, core::int32 level,
        core::int32 exp, bool locked, bool exist = true)
      : id_(id), level_(level),
        exp_(exp), locked_(locked), exist_(exist) {}

    core::int32 id_;
    core::int32 level_;
    core::int32 exp_;
    bool locked_;
    bool exist_;
  };

  typedef std::map<Location, Gem> GemMap;
  typedef std::vector< std::vector<Gem *> > GemIndexVector;
  typedef std::vector<database::protocol::StorageGemField> GemFieldVector;

  GemContainer();
  ~GemContainer();

  bool Initialize(ItemActor *item_actor);
  void Finalize();

  bool InitializeGems(const database::protocol::StorageItemActorField &db_item_actor_field,
      const GemFieldVector &gems);
  void Save(database::protocol::StorageItemActorField &db_item_actor_field);

  inline bool CheckLoadFinish() const {
    return this->load_finish_;
  }
  inline void SetLoadFinish(bool finish = true) {
    this->load_finish_ = finish;
  }

  // 开启功能
  bool EnableGemFunc(bool synchronize = true);
  bool EnableGemSlot2Func();
  bool EnableGemSlot3Func();
  bool EnableGemSlot4Func();
  bool EnableGemSlot5Func();
  bool EnableGemSlot6Func();
  bool EnableGemSlot7Func();
  bool EnableGemSlot8Func();
  bool EnableGemSlot9Func();

  const Gem *GetGemInfo(const Location &location) const;

  inline core::int32 GetGatheringPoint() const {
    return this->gathering_point_;
  }
  inline void SetGatheringPoint(core::int32 point) {
    this->gathering_point_ = point;
  }

  bool CheckGemStorageFull() const;
  bool Add(core::int32 gem_id, bool synchronize = true);
  bool Move(const Location &src, const Location &dest, bool synchronize = true);
  bool Lock(const Location &location, bool locked, bool synchronize = true);
  bool LockAll(bool locked, bool synchronize = true);
  core::int32 Decompose(bool synchronize = true);
  void AddExp(const Location &location, core::int32 exp, bool synchronize = true);

  void UpdateGemFightScore(bool synchronize = true);

 private:
  void SendInitToClient();
  bool CheckLocationValid(const Location &location) const;
  bool CheckGemAttrTypeDuplicate(core::int16 except_index,
      entity::GemLocationType::type location_type,
      core::int32 gem_id, core::int32 gem_level) const;
  Gem *GetGem(const Location &location) const;
  void AddGem(const Location &location, const Gem &gem, bool synchronize);
  void RemoveGem(const Location &location, bool synchronize);
  void SwapGem(const Location &lhs, const Location &rhs, bool synchronize);
  void SynchronizeGem(const Location &location);
  void AddGemAttr(core::int32 gem_id, core::int32 gem_level, bool synchronize);
  void RemoveGemAttr(core::int32 gem_id, core::int32 gem_level, bool synchronize);

  bool load_finish_;
  ItemActor *item_actor_;
  GemMap gems_store_;
  GemIndexVector gems_index_;
  core::int16 storage_min_empty_index_;
  core::int32 gathering_point_; 
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__GEM__CONTAINER__H

