//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 18:10:34.
// File name: set_back_state_action.h
//
// Description:
// Define class SetBackStateAction.
//

#ifndef __GAME__SERVER__AI__SET__BACK__STATE__ACTION__H
#define __GAME__SERVER__AI__SET__BACK__STATE__ACTION__H

#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class SetBackStateAction : public ActionNode {
 public:
  SetBackStateAction();
  virtual ~SetBackStateAction();

  virtual const std::string& GetTypeName() const {
    return SetBackStateAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  bool status_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SET__BACK__STATE__ACTION__H

