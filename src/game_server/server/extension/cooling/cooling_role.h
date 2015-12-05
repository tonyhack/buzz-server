//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 19:28:32.
// File name: cooling_role.h
//
// Description:
// Define class CoolingRole.
//

#ifndef __GAME__SERVER__COOLING__COOLING__ROLE__H
#define __GAME__SERVER__COOLING__COOLING__ROLE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_cooling_login_types.h"
#include "entity/role_types.h"
#include "game_server/server/game_role.h"

namespace game {

namespace server {

namespace cooling {

class Cooling;
class CoolingGroup;

class CoolingRole : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, Cooling *> CoolingHashMap;
  typedef std::map<core::uint16, CoolingGroup *> CoolingGroupMap;

 public:
  typedef std::vector<database::protocol::StorageCoolingField> CoolingVector;

  CoolingRole();
  ~CoolingRole();

  bool Initialize(GameRole *role, const CoolingVector &coolings);
  void Finalize();

  // 开始冷却
  void StartCooling(core::uint32 id, core::int64 elapse = 0,
      core::int64 add_value_ = 0, core::int32 add_percent = 0);
  // 结束冷却
  void EndCooling(core::uint32 id);

  // 检测冷却是否结束
  bool CheckCoolingEnd(core::uint32 id) const;

  // 保存
  void Save();

  // 同步冷却数据
  void Synchronize();

  // 获取类型
  inline entity::EntityType::type GetType() const {
    if(this->role_ == NULL) return entity::EntityType::TYPE_MAX;
    else return this->role_->GetType();
  }

  // 获取ID
  inline core::uint64 GetID() const {
    if(this->role_) {
      return this->role_->GetID();
    }
    return 0;
  }

 private:
  // 增加一个冷却对象
  bool AddCooling(core::uint32 id, Cooling *cooling);
  // 移除一个冷却对象
  Cooling *RemoveCooling(core::uint32 id);

  // 增加一个冷却组对象
  bool AddCoolingGroup(core::uint32 id, CoolingGroup *group);
  // 移除一个冷却组对象
  CoolingGroup *RemoveCoolingGroup(core::uint32 id);

  // 角色指针
  GameRole *role_;
  // 冷却表
  CoolingHashMap coolings_;
  // 冷却组表
  CoolingGroupMap cooling_groups_;

  static const size_t max_save_number_ = 256;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__ROLE__H

