//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-15 11:08:07.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__CONFIGURE__H
#define __GAME__SERVER__CONFIGURE__H

#include "entity/functionality_types.h"
#include "game_server/server/according_level_configure.h"
#include "game_server/server/load_attr_convert_configure.h"
#include "game_server/server/load_attr_fight_score_configure.h"
#include "game_server/server/load_camp_configure.h"
#include "game_server/server/load_functionality_configure.h"
#include "game_server/server/load_drop_configure.h"
#include "game_server/server/load_vocation_growth_configure.h"
#include "game_server/server/npc_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool Initialize();
  void Finalize();

  inline const AccordingLevelConfigure &GetAccordingLevelConfigure() const {
    return this->according_level_;
  }
  inline const NpcConfigure &GetNpcConfigure() const {
    return this->npc_;
  }
  inline const AttrConvertCell *GetAttrConvertCell(
      entity::RoleClientFields::type type) const {
    return this->load_attr_convert_.GetCell(type);
  }
  inline const AttrFightScoreCell *GetAttrFightScoreCell() const {
    return this->load_attr_fight_score_.GetCell();
  }
  inline const VocationGrowthCell *GetVocationGrowthCell(
      entity::VocationType::type type) const {
    return this->load_vocation_growth_.GetCell(type);
  }
  inline entity::CampRelationType::type GetCampRelation(core::int32 camp1, 
      core::int32 camp2) const {
    return this->load_camp_.GetRelation(camp1, camp2);
  }
  inline LoadFunctionalityConfigure::OpenCells& GetFunctionalityCell(
      entity::FunctionalityOpenType::type type) {
    return this->load_functionality_.GetCell(type);
  }
  entity::FunctionalityType::type StrConvertToFunctionality(const char *str);
  entity::FunctionalityType::type GetServerDefineByFunctionality(core::int32 functionality_id);
  const char *GetFunctionality(core::int32 id);
  inline const LoadDropConfigure::DropElementVector &Drop(core::int32 id) const {
    return this->load_drop_.Drop(id);
  }

  static const core::int32 kInitialActorAttr_ = 20;
  static const core::int32 kGridPixelWidth_ = 48;

 private:
  Configure() {}
  ~Configure() {}

  AccordingLevelConfigure according_level_;
  LoadAttrConvertConfigure load_attr_convert_;
  LoadAttrFightScoreConfigure load_attr_fight_score_;
  LoadDropConfigure load_drop_;
  LoadVocationGrowthConfigure load_vocation_growth_;
  LoadCampConfigure load_camp_;
  NpcConfigure npc_;
  LoadFunctionalityConfigure load_functionality_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CONFIGURE__H

