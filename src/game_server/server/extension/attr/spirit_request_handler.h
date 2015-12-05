#ifndef __GAME__SERVER__ATTR__SPIRIT__REQUEST__HANDLER__H
#define __GAME__SERVER__ATTR__SPIRIT__REQUEST__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class SpiritRequestHandler : public core::Noncopyable {
 public:
  SpiritRequestHandler();
  ~SpiritRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestAttrAddSpiritFacade(void *message, size_t size);
  int OnRequestAttrGetSpiritLevel(void *message, size_t size);
  int OnRequestAttrGetSpiritFacadeType(void *message, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__SPIRIT__REQUEST__HANDLER__H

