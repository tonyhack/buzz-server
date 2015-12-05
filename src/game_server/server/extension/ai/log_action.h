//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-12 14:47:34.
// File name: log_action.h
//
// Description:
// Define class LogAction.
//

#ifndef __GAME__SERVER__AI__LOG__ACTION__H
#define __GAME__SERVER__AI__LOG__ACTION__H

#include <string>

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class LogAction : public ActionNode {
 public:
  LogAction();
  virtual ~LogAction();

  virtual const std::string& GetTypeName() const {
    return LogAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  std::string log_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__LOG__ACTION__H

