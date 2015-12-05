//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 19:02:32.
// File name: check_birth_place_condition.h
//
// Description:
// Define class CheckBirthPlaceCondition.
//

#ifndef __GAME__SERVER__AI__CHECK__BIRTH__PLACE__CONDITION__H
#define __GAME__SERVER__AI__CHECK__BIRTH__PLACE__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"

namespace game {

namespace server {

namespace ai {

class CheckBirthPlaceCondition : public ConditionNode {
 public:
  typedef CheckBirthPlaceCondition Type;

  CheckBirthPlaceCondition();
  virtual ~CheckBirthPlaceCondition();

  virtual const std::string& GetTypeName() const {
    return CheckBirthPlaceCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__BIRTH__PLACE__CONDITION__H

