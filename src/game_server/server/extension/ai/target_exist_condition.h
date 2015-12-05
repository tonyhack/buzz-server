//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 18:09:50.
// File name: target_exist_condition.h
//
// Description:
// Define class TargetExistCondition.
//

#ifndef __GAME__SERVER__AI__TARGET__EXIT__CONDITION__H
#define __GAME__SERVER__AI__TARGET__EXIT__CONDITION__H

#include "game_server/server/extension/ai/condition_node.h"

namespace game {

namespace server {

namespace ai {

class TargetExistCondition : public ConditionNode {
 public:
  typedef TargetExistCondition Type;

  TargetExistCondition();
  virtual ~TargetExistCondition();

  virtual const std::string& GetTypeName() const {
    return TargetExistCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__TARGET__EXIT__CONDITION__H

