//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 16:48:43.
// File name: check_interval_seconds_condition.h
//
// Description:
// Define class CheckIntervalSecondsCondition.
//

#ifndef __GAME__SERVER__AI__CHECK__INTERVAL__SECONDS__CONDITION__H
#define __GAME__SERVER__AI__CHECK__INTERVAL__SECONDS__CONDITION__H

#include <string>

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"

namespace game {

namespace server {

namespace ai {

class CheckIntervalSecondsCondition : public ConditionNode {
 public:
  typedef CheckIntervalSecondsCondition Type;

  CheckIntervalSecondsCondition();
  virtual ~CheckIntervalSecondsCondition();

  virtual const std::string& GetTypeName() const {
    return CheckIntervalSecondsCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;

  // 变量
  std::string var_;
  // 时间
  core::uint32 interval_seconds_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__INTERVAL__SECONDS__CONDITION__H

