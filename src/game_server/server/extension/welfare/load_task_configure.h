#ifndef __GAME__SERVER__WELFARE__LOAD__TASK__CONFIGURE__H
#define __GAME__SERVER__WELFARE__LOAD__TASK__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace welfare {

class ConditionCell {
 public:
  ConditionCell() {
    this->param1_ = 0;
    this->param2_ = 0;
    this->param3_ = 0;
  }
  core::int32 type_;
  core::uint32 param1_;
  core::uint32 param2_;
  core::uint32 param3_;
};

class Award {
 public:
  Award() : type_(0), id_(0), number_(0) {}

  // 类型
  core::uint32 type_;

  // ID/数量
  core::uint32 id_;
  core::uint32 number_;

  // 是否绑定(0:非绑, 1:绑定)
  core::uint32 bind_;
};

class AchieveTaskCell {
 public:
  core::int32 id_;
  core::int32 achieve_point_;
  Award award_;
  core::int32 physique_;
  core::int32 strength_;
  core::int32 intelligence_;
  core::int32 agile_;
  core::int32 spirit_;
  core::int32 title_;
  std::vector<ConditionCell> conditions_;
};

class LoadTaskConfigure : public core::Noncopyable {
 public:
  typedef std::map<core::int32, AchieveTaskCell> AchieveMap;

  LoadTaskConfigure();
  ~LoadTaskConfigure();

  bool LoadConfigure(const std::string &file);

  const AchieveMap &GetCells() const {
    return this->achieve_map_;
  }

  const AchieveTaskCell *GetCell(core::int32 id) const {
    LoadTaskConfigure::AchieveMap::const_iterator itr = this->achieve_map_.find(id);
    if(itr == this->achieve_map_.end()) {
      return NULL;
    }
    return &(itr->second);
  }

 private:
  AchieveMap achieve_map_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__LOAD__TASK__CONFIGURE__H

