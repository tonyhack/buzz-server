//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 17:34:17.
// File name: check_threat_condition.h
//
// Description:
// Define class CheckThreatCondition.
//

#ifndef __GAME__SERVER__AI__CHECK__THREAT__CONDITION__H
#define __GAME__SERVER__AI__CHECK__THREAT__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"

namespace game {

namespace server {

namespace ai {

class CheckThreatCondition : public ConditionNode {
 public:
  typedef CheckThreatCondition Type;

  CheckThreatCondition();
  virtual ~CheckThreatCondition();

  virtual const std::string& GetTypeName() const {
    return CheckThreatCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__THREAT__CONDITION__H

