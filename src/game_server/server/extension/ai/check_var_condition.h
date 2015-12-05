//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:14:17.
// File name: check_var_condition.h
//
// Description:
// Define class CheckVarCondition.
//

#ifndef __GAME__SERVER__AI__CHECK__VAR__CONDITION__H
#define __GAME__SERVER__AI__CHECK__VAR__CONDITION__H

#include <string>

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"
#include "game_server/server/extension/ai/ai_types.h"

namespace game {

namespace server {

namespace ai {

class CheckVarCondition : public ConditionNode {
 public:
  typedef CheckVarCondition Type;

  CheckVarCondition();
  virtual ~CheckVarCondition();

  virtual const std::string& GetTypeName() const {
    return CheckVarCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;

  std::string var_;
  RelationalOperator::type operator_type_;
  core::int32 value_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__VAR__CONDITION__H

