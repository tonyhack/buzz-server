//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 14:05:22.
// File name: behaviour_factory.h
//
// Description:
// Define class BehaviourFactory.
//

#ifndef __GAME__SERVER__AI__BEHAVIOUR__FACTORY__H
#define __GAME__SERVER__AI__BEHAVIOUR__FACTORY__H

#include <map>
#include <string>

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/behaviour_node.h"
#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/decision_allocator.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

template <typename K, typename A>
class BehaviourFactory : public global::SingletonFactory<BehaviourFactory<K, A> > {
  friend class global::SingletonFactory<BehaviourFactory<K, A> >;

  typedef K Key;
  typedef A Allocator;
  typedef typename std::map<Key, Allocator *> BehaviourMap;
  typedef typename BehaviourMap::iterator BehaviourMapIterator;

 public:
  BehaviourFactory() {}
  virtual ~BehaviourFactory() {}

  bool Register(const Key &key, Allocator *allocator) {
    if(allocator == NULL) {
      return false;
    }
    if(this->behaviours_.find(key) != this->behaviours_.end()) {
      return false;
    }
    this->behaviours_.insert(std::make_pair(key, allocator));
    return true;
  }

  void CancelRegister(const Key &key) {
    this->behaviours_.erase(key);
  }

  Allocator *GetAllocator(const Key &key) {
    BehaviourMapIterator iterator = this->behaviours_.find(key);
    if(iterator == this->behaviours_.end()) {
      return NULL;
    }
    return iterator->second;
  }

 private:
  BehaviourMap behaviours_;
};

typedef BehaviourFactory<std::string, ActionAllocator> ActionFactory;
typedef BehaviourFactory<std::string, ConditionAllocator> ConditionFactory;
typedef BehaviourFactory<std::string, DecisionAllocator> DecisionFactory;

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BEHAVIOUR__FACTORY__H
