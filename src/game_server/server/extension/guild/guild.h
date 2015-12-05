#ifndef __GAME__SERVER__GUILD__GUILD__H
#define __GAME__SERVER__GUILD__GUILD__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session { namespace protocol {
class GuildData;
}}

namespace game {

namespace server {

namespace guild {

class Guild : public core::Noncopyable {
 public:
  Guild();
  ~Guild();

  bool Initialize(const session::protocol::GuildData &data);
  void Finalize();

  inline core::uint64 GetID() const { return this->id_; }
  inline const std::string &GetName() const { return this->name_; }
  inline core::int32 GetLevel() const { return this->level_; }
  inline core::int32 GetLightAttribute() const { return this->light_attribute_; }
  inline core::int32 GetDarkAttribute() const { return this->dark_attribute_; }
  inline core::int32 GetHolyAttribute() const { return this->holy_attribute_; }
  inline core::int32 GetShopLevel() const { return this->shop_level_; }
  inline core::int32 GetCollegeLevel() const { return this->college_level_; }
  inline core::int32 GetBarrackLevel() const { return this->barrack_level_; }

  inline core::int32 GetAttributeSum() const {
    return this->light_attribute_ + this->dark_attribute_ + this->holy_attribute_;
  }

  inline void SetLevel(core::int32 level) {
    this->level_ = level;
  }
  inline void SetLightAttribute(core::int32 light_attribute) {
    this->light_attribute_ = light_attribute;
  }
  inline void SetDarkAttribute(core::int32 dark_attribute) {
    this->dark_attribute_ = dark_attribute;
  }
  inline void SetHolyAttribute(core::int32 holy_attribute) {
    this->holy_attribute_ = holy_attribute;
  }
  inline void SetShopLevel(core::int32 shop_level) {
    this->shop_level_ = shop_level;
  }
  inline void SetCollegeLevel(core::int32 college_level) {
    this->college_level_ = college_level;
  }
  inline void SetBarrackLevel(core::int32 barrack_level) {
    this->barrack_level_ = barrack_level;
  }

 private:
  core::uint64 id_;
  std::string name_;
  core::int32 level_;
  core::int32 light_attribute_;
  core::int32 dark_attribute_;
  core::int32 holy_attribute_;
  core::int32 shop_level_;
  core::int32 college_level_;
  core::int32 barrack_level_;
};


}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__GUILD__H

