//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 13:27:31.
// File name: random_move_action.h
//
// Description:
// Define class RandomMoveAction.
//

#ifndef __GAME__SERVER__AI__RANDOM__MOVE__ACTION__H
#define __GAME__SERVER__AI__RANDOM__MOVE__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class RandomMoveAction : public ActionNode {
 public:
  RandomMoveAction();
  virtual ~RandomMoveAction();

  virtual const std::string& GetTypeName() const {
    return RandomMoveAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  // 机率(万分之)
  core::uint32 odds_;

  // 移动间隔
  core::uint32 move_interval_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__RANDOM__MOVE__ACTION__H

