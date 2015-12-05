//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 17:20:13.
// File name: reset_interval_seconds_action.h
//
// Description:
// Define class ResetIntervalSecondsAction.
//

#ifndef __GAME__SERVER__AI__RESET__INTERVAL__SECONDS__ACTION__H
#define __GAME__SERVER__AI__RESET__INTERVAL__SECONDS__ACTION__H

#include <string>

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class ResetIntervalSecondsAction : public ActionNode {
 public:
  ResetIntervalSecondsAction();
  virtual ~ResetIntervalSecondsAction();

  virtual const std::string& GetTypeName() const {
    return ResetIntervalSecondsAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  std::string var_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__RESET__INTERVAL__SECONDS__ACTION__H

