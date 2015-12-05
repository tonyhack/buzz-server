//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:32:18.
// File name: set_var_action.h
//
// Description:
// Define class SetVarAction.
//

#ifndef __GAME__SERVER__AI__SET__VAR__ACTION__H 
#define __GAME__SERVER__AI__SET__VAR__ACTION__H

#include <string>

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class SetVarAction : public ActionNode {
 public:
  SetVarAction();
  virtual ~SetVarAction();

  virtual const std::string& GetTypeName() const {
    return SetVarAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  std::string var_;
  core::int64 value_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SET__VAR__ACTION__H

