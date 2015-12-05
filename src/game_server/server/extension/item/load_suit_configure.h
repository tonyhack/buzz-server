#ifndef __GAME__SERVER__ITEM__LOAD__SUIT__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__SUIT__CONFIGURE__H

#include <map>
#include <vector>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class SuitEffectCell {
 public:
   int effect_id_;    // 效果ID
   int suit_num_;
};

class LoadSuitConfigure : public core::Noncopyable {
 public:
  typedef std::vector<SuitEffectCell> EffectVecs;
  typedef std::map<int, EffectVecs> SuitMap;

  LoadSuitConfigure();
  ~LoadSuitConfigure();

  bool LoadConfigure(const std::string &file);

  const LoadSuitConfigure::EffectVecs *GetCell(core::int32 suit_id) const;

 private:
  SuitMap suits_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__SUIT__CONFIGURE__H

