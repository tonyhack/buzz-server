//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 19:13:59.
// File name: allocator_template.h
//
// Description:
// Define class AllocatorTemplate.
//

#ifndef __GAME__SERVER__AI__ALLOCATOR__TEMPLATE__H
#define __GAME__SERVER__AI__ALLOCATOR__TEMPLATE__H

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension/ai/behaviour_node.h"

namespace game {

namespace server {

namespace ai {

template <typename T>
class AllocatorTemplate {
 public:
  typedef typename T::Type Type;

  AllocatorTemplate() {}
  virtual ~AllocatorTemplate() {}

  virtual Type *Allocate(BehaviourNode *parent, TiXmlElement *node) = 0;
  virtual void Deallocate(BehaviourNode *node) = 0;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ALLOCATOR__TEMPLATE__H

