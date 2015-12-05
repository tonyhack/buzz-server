//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 20:19:52.
// File name: move_target_action.h
//
// Description:
// Define class MoveTargetAction.
//

#ifndef __GAME__SERVER__AI__MOVE__TARGET__ACTION__H
#define __GAME__SERVER__AI__MOVE__TARGET__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class MoveTargetAction : public ActionNode {
 public:
  MoveTargetAction();
  virtual ~MoveTargetAction();

  virtual const std::string& GetTypeName() const {
    return MoveTargetAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  core::int32 max_path_;
  core::int32 move_target_radius_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__MOVE__TARGET__ACTION__H

