//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 14:35:26.
// File name: activity_range_condition.h
//
// Description:
// Define class ActivityRangeCondition.
//

#ifndef __GAME__SERVER__AI__ACTIVITY__RANGE__CONDITION__H
#define __GAME__SERVER__AI__ACTIVITY__RANGE__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"
#include "game_server/server/extension/ai/ai_types.h"

namespace game {

namespace server {

namespace ai {

class ActivityRangeCondition : public ConditionNode {
 public:
  typedef ActivityRangeCondition Type;

  ActivityRangeCondition();
  virtual ~ActivityRangeCondition();

  virtual const std::string& GetTypeName() const {
    return ActivityRangeCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;

  // 关系运算符
  RelationalOperator::type operator_type_;
  // 距离
  core::uint32 distance_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ACTIVITY__RANGE__CONDITION__H

