//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 16:26:59.
// File name: search_target_action.h
//
// Description:
// Define class SearchTargetAction.
//

#ifndef __GAME__SERVER__AI__SEARCH__TARGET__ACTION__H
#define __GAME__SERVER__AI__SEARCH__TARGET__ACTION__H

#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class SearchTargetAction : public ActionNode {
 public:
  SearchTargetAction();
  virtual ~SearchTargetAction();

  virtual const std::string& GetTypeName() const {
    return SearchTargetAction::kTypeName_;
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

#endif  // __GAME__SERVER__AI__SEARCH__TARGET__ACTION__H

