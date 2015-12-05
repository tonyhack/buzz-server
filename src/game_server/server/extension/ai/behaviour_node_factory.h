//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 16:53:08.
// File name: behaviour_node_factory.h
//
// Description:
// Define class BehaviourNodeFactory.
//

#ifndef __GAME__SERVER__AI__BEHAVIOUR__NODE__FACTORY__H
#define __GAME__SERVER__AI__BEHAVIOUR__NODE__FACTORY__H

#include "core/tinyxml/tinyxml.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class BehaviourNode;

class BehaviourNodeFactory : public global::SingletonFactory<BehaviourNodeFactory> {
  friend class global::SingletonFactory<BehaviourNodeFactory>;

 public:
  // BehaviourNode *Allocate(const char *type1, const char *type2, const char *name);
  BehaviourNode *Allocate(const char *type1, const char *type2, const char *name,
      BehaviourNode *parent_node, TiXmlElement *node);

 private:
  BehaviourNodeFactory() {}
  virtual ~BehaviourNodeFactory() {}

  static const char *kDecisionTypeStr_;
  static const size_t kStrlenDecision_;

  static const char *kBehaviourTypeStr_;
  static const size_t kStrlenBehaviour_;

  static const char *kConditionTypeStr_;
  static const size_t kStrlenCondition_;

  static const char *kActionTypeStr_;
  static const size_t kStrlenAction_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BEHAVIOUR__NODE__FACTORY__H

