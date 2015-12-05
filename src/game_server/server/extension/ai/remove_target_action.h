//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 17:30:23.
// File name: remove_target_action.h
//
// Description:
// Define class RemoveTargetAction.
//

#ifndef __GAME__SERVER__AI__REMOVE__TARGET__ACTION__H
#define __GAME__SERVER__AI__REMOVE__TARGET__ACTION__H

#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class RemoveTargetAction : public ActionNode {
 public:
  typedef RemoveTargetAction Type;

  RemoveTargetAction();
  virtual ~RemoveTargetAction();

  virtual const std::string& GetTypeName() const {
    return RemoveTargetAction::kTypeName_;
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

#endif  // __GAME__SERVER__AI__REMOVE__TARGET__ACTION__H

