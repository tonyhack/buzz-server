//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 19:25:04.
// File name: check_birth_place_condition_allocator.cc
//
// Description:
// Define class CheckBirthPlaceConditionAllocator.
//

#include "game_server/server/extension/ai/check_birth_place_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckBirthPlaceConditionAllocator::CheckBirthPlaceConditionAllocator() {}
CheckBirthPlaceConditionAllocator::~CheckBirthPlaceConditionAllocator() {}

CheckBirthPlaceCondition *CheckBirthPlaceConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  // 分配
  CheckBirthPlaceCondition *condition =
    new (std::nothrow) CheckBirthPlaceCondition();
  if(condition == NULL) {
    global::LogError("%s:%d (%s) 分配 CheckBirthPlaceCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(condition->Initialize(parent, node) == false) {
    delete condition;
    global::LogError("%s:%d (%s) 初始化 CheckBirthPlaceCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  return condition;
}

void CheckBirthPlaceConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}


}  // namespace ai

}  // namespace server

}  // namespace game

