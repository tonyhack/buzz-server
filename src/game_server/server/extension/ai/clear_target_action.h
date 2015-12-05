//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-09 13:48:07.
// File name: clear_target_action.h
//
// Description:
// Define class ClearTargetAction.
//

#ifndef __GAME__SERVER__AI__CLEAR__TARGET__ACTION__H
#define __GAME__SERVER__AI__CLEAR__TARGET__ACTION__H

#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class ClearTargetAction : public ActionNode {
 public:
  ClearTargetAction();
  virtual ~ClearTargetAction();

  virtual const std::string& GetTypeName() const {
    return ClearTargetAction::kTypeName_;
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

#endif  // __GAME__SERVER__AI__CLEAR__TARGET__ACTION__H

