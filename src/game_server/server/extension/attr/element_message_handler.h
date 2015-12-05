#ifndef __GAME__SERVER__ATTR__ELEMENT__MESSAGE__HANDLER
#define __GAME__SERVER__ATTR__ELEMENT__MESSAGE__HANDLER

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class ElementMessageHandler : public core::Noncopyable {
 public:
  ElementMessageHandler();
  ~ElementMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 元素升级请求
  void OnMessageAttrElementUpgradeRequest(core::uint64 id, const char *data, size_t size);
  // 元素对比请求
  void OnMessageAttrElementCompareRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ELEMENT__MESSAGE__HANDLER

