
#include "game_server/server/extension/welfare/condition_pool.h"

#include "game_server/server/extension/welfare/condition.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

bool ConditionPool::Initialize(size_t initial_number, size_t extend_number) {
  if(this->conditions_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 Condition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void ConditionPool::Finalize() {
  this->conditions_.Finalize();
}

Condition *ConditionPool::Allocate() {
  return this->conditions_.Allocate();
}

void ConditionPool::Deallocate(Condition *condition) {
  if(condition == NULL) {
    return ;
  }
  this->conditions_.Deallocate(condition);
}

}  // namespace welfare

}  // namespace server

}  // namespace game

