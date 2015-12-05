//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 10:08:18.
// File name: behaviour_tree_manager.h
//
// Description:
// Define class BehaviourTreeManager.
//

#ifndef __GAME__SERVER__AI__BEHAVIOUR__TREE__MANAGER__H
#define __GAME__SERVER__AI__BEHAVIOUR__TREE__MANAGER__H

#include <string>

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class BehaviourTree;

class BehaviourTreeManager : public global::SingletonFactory<BehaviourTreeManager> {
  friend class global::SingletonFactory<BehaviourTreeManager>;

  typedef __gnu_cxx::hash_map<core::uint32, BehaviourTree *> BehaviourTreeHashmap;

 public:
  bool Initialize(const std::string &file);
  void Finalize();

  BehaviourTree *Get(core::uint32 id);
  bool Add(BehaviourTree *tree);
  BehaviourTree *Remove(core::uint32 id);

 private:
  BehaviourTreeManager() {}
  ~BehaviourTreeManager() {}

  void RegistAllocator();
  void CancelRegistAllocator();

  bool Parse(const std::string &file);

  BehaviourTreeHashmap behaviour_trees_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BEHAVIOUR__TREE__MANAGER__H

