//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-14 16:46:27.
// File name: entity_enumerator.h
//
// Description:
// Define class EntityEnumberator.
//

#ifndef __GAME__SERVER__SCENE__ENTITY__ENUMBERATOR__H
#define __GAME__SERVER__SCENE__ENTITY__ENUMBERATOR__H

#include <map>
#include <set>
#include <vector>

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GameRole;

namespace scene {

class Grid;
class Scene;

class EntityFilter : public global::SingletonFactory<EntityFilter> {
  friend class global::SingletonFactory<EntityFilter>;

 public:
  typedef std::pair<core::uint64, entity::EntityType::type> EntityPair;
  typedef std::set<EntityPair> EntityFilterSet;
  typedef std::vector<EntityPair> EntityVector;

  typedef std::map<core::uint16, size_t> DirectionIndexMap;

  bool Initialize();

  inline void SetRole(GameRole *src_role) {
    this->src_role_ = src_role;
  }
  inline GameRole *GetRole() const {
    return this->src_role_;
  }

  inline void SetScene(Scene *scene) {
    this->scene_ = scene;
  }

  inline Scene *GetScene() const {
    return this->scene_;
  }

  inline void SetFilterMask(core::uint32 mask) {
    this->filter_mask_ = mask;
  }

  inline void FilterEntity(entity::EntityType::type type, core::uint64 id) {
    this->filters_.insert(std::make_pair(id, type));
  }

  inline void Clear() {
    this->filters_.clear();
    this->entities_.clear();
  }

  inline size_t GetFilterSize() const {
    return this->entities_.size();
  }

  inline size_t GetEntitySize() const {
    return this->entities_.size();
  }

  inline const EntityVector &GetEntitys() const {
    return this->entities_;
  }

  void FileterGrid(const Grid *grid);
  void AddEntity(GameRole *Role);

  inline size_t GetDirectionIndex(core::uint16 direction) {
    DirectionIndexMap::const_iterator iterator = this->dir_indexs_.find(direction);
    if(iterator != this->dir_indexs_.end()) {
      return iterator->second;
    }
    return -1;
  }

 private:
  EntityFilter() {}
  ~EntityFilter() {}

  EntityFilterSet filters_;
  EntityVector entities_;

  DirectionIndexMap dir_indexs_;

  core::uint32 filter_mask_;
  GameRole *src_role_;

  Scene *scene_;
};

class Enumberator {
 public:
  Enumberator() : filter_(NULL), max_count_(0) {}
  virtual ~Enumberator() {}

  inline void SetEntityFilter(EntityFilter *filter) {
    this->filter_ = filter;
  }
  inline void SetMaxCount(core::uint32 count) {
    this->max_count_ = count;
  }
  virtual void Execute(Scene *scene, core::uint32 x, core::uint32 y) = 0;

 protected:
  EntityFilter *filter_;
  core::uint32 max_count_;
};

class RayEnumberator : public Enumberator {
 public:
  RayEnumberator();
  virtual ~RayEnumberator();

  bool Initialize(core::uint8 length, core::uint8 width,
      core::uint16 direction);

  virtual void Execute(Scene *scene, core::uint32 x, core::uint32 y);

 private:
  // 长和宽
  core::uint8 length_;
  core::uint8 width_;
  // 方向
  core::int16 direction_;
};

class CircleEnumberator : public Enumberator {
 public:
  CircleEnumberator();
  virtual ~CircleEnumberator();

  bool Initialize(core::uint8 radius);

  virtual void Execute(Scene *scene, core::uint32 x, core::uint32 y);

 private:
  core::uint8 radius_;
};

class SectorQuarterEnumberator : public Enumberator {
 public:
  SectorQuarterEnumberator();
  virtual ~SectorQuarterEnumberator();

  bool Initialize(core::uint8 radius, entity::DirectionType::type direction);

  virtual void Execute(Scene *scene, core::uint32 x, core::uint32 y);

 private:
  core::uint8 radius_;
  entity::DirectionType::type direction_;
};

class SectorHalfEnumberator : public Enumberator {
 public:
  SectorHalfEnumberator();
  virtual ~SectorHalfEnumberator();

  bool Initialize(core::uint8 radius, entity::DirectionType::type direction);

  virtual void Execute(Scene *scene, core::uint32 x, core::uint32 y);

 private:
  core::uint8 radius_;
  entity::DirectionType::type direction_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__ENTITY__ENUMBERATOR__H

