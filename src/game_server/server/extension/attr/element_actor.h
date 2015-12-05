#ifndef __GAME__SERVER__ATTR__ELEMENT__ACTOR__H
#define __GAME__SERVER__ATTR__ELEMENT__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace database { namespace protocol {
class StorageAttrActorField;
}}

namespace game {

namespace server {

namespace attr {

class AttrActor;

class ElementActor : public core::Noncopyable {
 public:
  ElementActor();
  ~ElementActor();

 bool Initialize(AttrActor *attr_actor, const database::protocol::StorageAttrActorField &db_field);
 void Finalize();
 void Save(database::protocol::StorageAttrActorField &db_field);

 // 开启功能
 bool EnableElementFunc(bool synchronize = true);

 inline core::int32 GetCurrentLevel() const { return this->current_level_; }
 void SetCurrentLevel(core::int32 new_level, bool synchronize = true);

 void UpdateElementFightScore(bool synchronize = true);

 private:
  void SendInitToClient();

  AttrActor *attr_actor_;
  core::int32 current_level_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ELEMENT__ACTOR__H

