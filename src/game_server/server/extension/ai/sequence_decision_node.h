//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 11:01:58.
// File name: sequence_decision_node.h
//
// Description:
// Define class SequenceDecisionNode.
//

#ifndef __GAME__SERVER__AI__SEQUENCE__DECISION__NODE__H
#define __GAME__SERVER__AI__SEQUENCE__DECISION__NODE__H

#include "game_server/server/extension/ai/behaviour_node.h"

namespace game {

namespace server {

namespace ai {

class SequenceDecisionNode : public BehaviourNode {
 public:
  typedef SequenceDecisionNode Type;

  SequenceDecisionNode();
  virtual ~SequenceDecisionNode();

  bool Initialize(BehaviourNode *parent, TiXmlElement *node);
  void Deallocate();

  virtual bool CheckEvaluate(BlackBoard *black_board) const;
  virtual void OnTransition(BlackBoard *black_board);
  virtual RunningStatus::type OnRunning(BlackBoard *black_board);
  virtual void OnAddChildNode(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SEQUENCE__DECISION__NODE__H

