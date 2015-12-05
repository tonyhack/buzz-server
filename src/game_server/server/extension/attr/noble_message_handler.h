#ifndef __GAME__SERVER__ATTR__NOBLE__MESSAGE__HANDLER__H
#define __GAME__SERVER__ATTR__NOBLE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class NobleMessageHandler : public core::Noncopyable {
 public:
  NobleMessageHandler();
  ~NobleMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 贵族提升请求
  void OnMessageAttrNoblePromoteRequest(core::uint64 id, const char *data, size_t size);
  // 贵族突破请求
  void OnMessageAttrNobleBreakThroughRequest(core::uint64 id, const char *data, size_t size);
  // 贵族对比请求
  void OnMessageAttrNobleCompareRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__NOBLE__MESSAGE__HANDLER__H

