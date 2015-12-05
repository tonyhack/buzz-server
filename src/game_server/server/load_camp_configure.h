//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-17 02:56:10.
// File name: load_camp_configure.h
//
// Description:
//

#ifndef __GAME__SERVER___LOAD__CAMP__CONFIGURE__H
#define __GAME__SERVER___LOAD__CAMP__CONFIGURE__H

#include <map>
#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/camp_types.h"
#include "entity/vocation_types.h"

namespace game {

namespace server {

class CampKey {
 public:
  CampKey() : type1_(0), type2_(0) {}
  CampKey(core::uint32 type1, core::uint32 type2)
    : type1_(type1), type2_(type2) {}

  bool operator==(const CampKey &key) const {
    return this->type1_ == key.type1_ && this->type2_ == key.type2_;
  }
  bool operator<(const CampKey &key) const {
    if(this->type1_ != key.type1_) {
      return this->type1_ < key.type1_;
    } else {
      return this->type2_ < key.type2_;
    }
  } 

  core::int32 type1_;
  core::int32 type2_;
};

class HashCampFunction {
  public:
    size_t operator()(const CampKey &key) const {
      size_t hash = key.type1_;
      return (hash << 32) + key.type2_;
    }
};

class CampCell {
 public:
  core::int32 camp_type_src_;
  core::int32 camp_type_dest_;
  entity::CampRelationType::type relation_;
};

class LoadCampConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<CampKey, CampCell, HashCampFunction> CampHashMap;

 public:
  LoadCampConfigure();
  ~LoadCampConfigure();

  bool LoadConfigure(const std::string &file);

  // 获取阵营关系表
  entity::CampRelationType::type GetRelation(core::int32 camp1, core::int32 camp2) const;

 private:
  // 阵营关系表
  CampHashMap camps_;

};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER___LOAD__CAMP__CONFIGURE__H

