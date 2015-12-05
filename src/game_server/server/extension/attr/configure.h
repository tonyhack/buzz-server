#ifndef __GAME__SERVER__ATTR__CONFIGURE__H
#define __GAME__SERVER__ATTR__CONFIGURE__H

#include <string>

#include "core/base/types.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/attr/attr_configure.h"
#include "game_server/server/extension/attr/load_attr_bonus_configure.h"
#include "game_server/server/extension/attr/load_element_configure.h"
#include "game_server/server/extension/attr/load_legendary_weapon_configure.h"
#include "game_server/server/extension/attr/load_noble_configure.h"
#include "game_server/server/extension/attr/load_rune_configure.h"
#include "game_server/server/extension/attr/load_spirit_configure.h"
#include "game_server/server/extension/attr/load_sword_configure.h"

namespace game {

namespace server {

namespace attr {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  // 属性奖励配置
  inline const AttrBonusCell *GetAttrBonus(core::int32 id) const {
    return this->load_attr_bonus_.GetAttrBonus(id);
  }

  // 贵族配置
  inline const NobleCell *GetNoble(core::int32 id) const {
    return this->load_noble_.GetNoble(id);
  }
  inline const NobleCell *GetFirstNoble() const {
    return this->load_noble_.GetFirstNoble();
  }
  inline const NobleCell *GetNextNoble(core::int32 id) const {
    return this->load_noble_.GetNextNoble(id);
  }

  // 魔剑配置
  inline const SwordCell *GetSword(core::int32 step) const {
    return this->load_sword_.GetSword(step);
  }
  inline const SwordLevelCell *GetSwordLevel(core::int32 level) const {
    return this->load_sword_.GetLevel(level);
  }

  // 元素配置
  inline const ElementCell *GetElement(core::int32 id) const {
    return this->load_element_.GetElement(id);
  }
  inline const ElementCell *GetNextElement(core::int32 id) const {
    return this->load_element_.GetNextElement(id);
  }

  // 精灵配置
  inline const SpiritCell *GetSpirit(core::int32 id) const {
    return this->load_spirit_.GetSpirit(id);
  }
  inline const SpiritFacadeCell *GetSpiritFacade(core::int32 id) const {
    return this->load_spirit_.GetSpiritFacade(id);
  }
  inline const RuneCell *GetRuneCell(core::int32 template_id) const {
    return this->load_rune_.GetRune(template_id);
  }

  // 名武配置
  inline const LegendaryWeaponCell *GetLegendaryWeapon(core::int32 id) const {
    return this->load_legendary_weapon_.GetLegendaryWeapon(id);
  }

 private:
  Configure() {}
  ~Configure() {}

  LoadAttrBonusConfigure load_attr_bonus_;
  LoadNobleConfigure load_noble_;
  LoadSwordConfigure load_sword_;
  LoadElementConfigure load_element_;
  LoadSpiritConfigure load_spirit_;
  LoadRuneConfigure load_rune_;
  LoadLegendaryWeaponConfigure load_legendary_weapon_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__CONFIGURE__H

