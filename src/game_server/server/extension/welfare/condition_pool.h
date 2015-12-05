
#ifndef __GAME__SERVER__WELFARE__CONDITION__POOL__H
#define __GAME__SERVER__WELFARE__CONDITION__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace welfare {

class Condition;

class ConditionPool : public global::SingletonFactory<ConditionPool> {
  friend class global::SingletonFactory<ConditionPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Condition *Allocate();
  void Deallocate(Condition *condition);

 private:
  ConditionPool() {}
  ~ConditionPool() {}

  global::PoolTemplate<Condition> conditions_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__CONDITION__POOL__H

