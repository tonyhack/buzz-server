//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 16:22:50.
// File name: behaviour_tree.h
//
// Description:
// Define class BehaviourTree.
//

#ifndef __GAME__SERVER__AI__BEHAVIOUR__TREE__H
#define __GAME__SERVER__AI__BEHAVIOUR__TREE__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/ai/behaviour_node.h"

namespace game {

namespace server {

namespace ai {

class BlackBoard;

class BehaviourTree : public core::Noncopyable {
 public:
  BehaviourTree();
  ~BehaviourTree();

  bool Initialize(core::uint32 id, const std::string &file);
  void Deallocate();

  inline core::uint32 GetID() const {
    return this->id_;
  }

  void Running(BlackBoard *black_board);

 private:
  bool LoadConfigure(const std::string &file);

  BehaviourNode *root_;

  core::uint32 id_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BEHAVIOUR__TREE__H

