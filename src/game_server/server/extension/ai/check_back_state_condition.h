//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 17:49:37.
// File name: check_back_state_condition.h
//
// Description:
// Define class CheckBackStateCondition.
//

#ifndef __GAME__SERVER__AI__CHECK__BACK__STATE__CONDITION__H
#define __GAME__SERVER__AI__CHECK__BACK__STATE__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"

namespace game {

namespace server {

namespace ai {

class CheckBackStateCondition : public ConditionNode {
 public:
  typedef CheckBackStateCondition Type;

  CheckBackStateCondition();
  virtual ~CheckBackStateCondition();

  virtual const std::string& GetTypeName() const {
    return CheckBackStateCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__BACK__STATE__CONDITION__H

