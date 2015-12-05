#ifndef __GAME__SERVER__ATTR__SPIRIT__MESSAGE__HANDLER__H
#define __GAME__SERVER__ATTR__SPIRIT__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class SpiritMessageHandler : public core::Noncopyable {
 public:
  SpiritMessageHandler();
  ~SpiritMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 精灵进阶请求
  void OnMessageAttrSpiritUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 精灵转换外观请求
  void OnMessageAttrSpiritTransformFacadeRequest(core::uint64 id, const char *data, size_t size);
  // 精灵对比请求
  void OnMessageAttrSpiritCompareRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__SPIRIT__MESSAGE__HANDLER__H

