
#include "game_server/server/extension/welfare/welfare_actor_pool.h"

#include "game_server/server/extension/welfare/welfare_actor.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

bool WelfareActorPool::Initialize(size_t initial_number, size_t extend_number) {
  if(this->actors_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 WelfareActor 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void WelfareActorPool::Finalize() {
  this->actors_.Finalize();
}

WelfareActor *WelfareActorPool::Allocate() {
  return this->actors_.Allocate();
}

void WelfareActorPool::Deallocate(WelfareActor *actor) {
  if(actor == NULL) {
    return ;
  }
  this->actors_.Deallocate(actor);
}

}  // namespace welfare

}  // namespace server

}  // namespace game

