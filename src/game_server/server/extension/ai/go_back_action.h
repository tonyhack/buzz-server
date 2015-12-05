//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 10:35:43.
// File name: go_back_action.h
//
// Description:
// Define class GoBackAction
//

#ifndef __GAME__SERVER__AI__GO__GACK__ACTION__H
#define __GAME__SERVER__AI__GO__GACK__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class GoBackAction : public ActionNode {
 public:
  GoBackAction();
  virtual ~GoBackAction();

  virtual const std::string& GetTypeName() const {
    return GoBackAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  core::int32 radius_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__GO__GACK__ACTION__H

