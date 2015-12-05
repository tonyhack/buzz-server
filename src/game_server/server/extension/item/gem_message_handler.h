#ifndef __GAME__SERVER__ITEM__GEM__MESSAGE__HANDLER__H
#define __GAME__SERVER__ITEM__GEM__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class GemMessageHandler : public core::Noncopyable {
 public:
  GemMessageHandler();
  ~GemMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageGemGatherRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageGemSkipGatheringPointRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageGemMoveRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageGemPromoteRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageGemDecomposeRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageGemLockRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageGemLockAllRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GEM__MESSAGE__HANDLER__H

