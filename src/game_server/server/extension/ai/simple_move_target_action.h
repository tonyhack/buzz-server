//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 12:06:56.
// File name: simple_move_target_action.h
//
// Description:
// Define class SimpleMoveTargetAction.
//

#ifndef __GAME__SERVER__AI__SIMPLE__MOVE__TARGET__ACTION__H
#define __GAME__SERVER__AI__SIMPLE__MOVE__TARGET__ACTION__H

#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class SimpleMoveTargetAction : public ActionNode {
 public:
  SimpleMoveTargetAction();
  virtual ~SimpleMoveTargetAction();

  virtual const std::string& GetTypeName() const {
    return SimpleMoveTargetAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SIMPLE__MOVE__TARGET__ACTION__H

