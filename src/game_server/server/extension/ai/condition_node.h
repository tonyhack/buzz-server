//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 14:11:11.
// File name: condition_node.h
//
// Description:
// Define class ConditionNode.
//

#ifndef __GAME__SERVER__AI__CONDITION__NODE__H
#define __GAME__SERVER__AI__CONDITION__NODE__H

#include <string>

#include "game_server/server/extension/ai/behaviour_node.h"

namespace game {

namespace server {

namespace ai {

class ConditionNode : public BehaviourNode {
 public:
  typedef ConditionNode Type;

  ConditionNode();
  virtual ~ConditionNode();

  bool Initialize(BehaviourNode *parent, TiXmlElement *node);
  void Deallocate();

  virtual const std::string &GetTypeName() const = 0;

protected:
  // 加载配置
  virtual bool LoadConfigure(TiXmlElement *node) = 0;

  virtual bool CheckEvaluate(BlackBoard *black_board) const {
    if(negation_) {
      return !this->DoCheckEvalute(black_board);
    } else {
      return this->DoCheckEvalute(black_board);
    }
  }

  virtual bool DoCheckEvalute(BlackBoard *black_board) const = 0;

  bool negation_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CONDITION__NODE__H

