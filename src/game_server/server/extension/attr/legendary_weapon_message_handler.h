#ifndef __GAME__SERVER__ATTR__LEGENDARY__WEAPON__MESSAGE__HANDLER__H
#define __GAME__SERVER__ATTR__LEGENDARY__WEAPON__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class LegendaryWeaponMessageHandler : public core::Noncopyable {
 public:
  LegendaryWeaponMessageHandler();
  ~LegendaryWeaponMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 名武修复请求
  void OnMessageAttrLegendaryWeaponRepairRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LEGENDARY__WEAPON__MESSAGE__HANDLER__H

