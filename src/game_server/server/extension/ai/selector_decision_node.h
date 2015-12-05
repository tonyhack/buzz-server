//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 13:37:42.
// File name: selector_decision_node.h
//
// Description:
// Define class SelectorDecisionNode.
//

#ifndef __GAME__SERVER__AI__SELECTOR__DECISION__NODE__H
#define __GAME__SERVER__AI__SELECTOR__DECISION__NODE__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/behaviour_node.h"

namespace game {

namespace server {

namespace ai {

class SelectorDecisionNode : public BehaviourNode {
 public:
  typedef SelectorDecisionNode Type;

  SelectorDecisionNode();
  virtual ~SelectorDecisionNode();

  virtual bool Initialize(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate();

  virtual bool CheckEvaluate(BlackBoard *black_board) const;
  virtual void OnTransition(BlackBoard *black_board);
  virtual RunningStatus::type OnRunning(BlackBoard *black_board);

  inline core::uint32 GetID() const {
    return this->id_;
  }

  inline void SetID(core::uint32 id) {
    this->id_ = id;
  }

 private:
  mutable size_t current_child_index_;
  size_t last_child_index_;

  core::uint32 id_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SELECTOR__DECISION__NODE__H

