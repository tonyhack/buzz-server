//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-07 23:29:32.
// File name: effect_type.cc
//
// Description:
// Define class effect type.
//

#include "game_server/server/extension/effect/effect_type.h"

namespace game {

namespace server {

namespace effect {

bool IsConsumingAttribute(EffectAttributeType::type type) {
  if(type == EffectAttributeType::LEVEL ||
      type == EffectAttributeType::CURRENT_HP ||
      type == EffectAttributeType::CURRENT_MP ||
      type == EffectAttributeType::CURRENT_EXP ||
      type == EffectAttributeType::NIMBUS ||
      type == EffectAttributeType::GOLD ||
      type == EffectAttributeType::FREEDOM_DOLLARS ||
      type == EffectAttributeType::RESTRICT_DOLLARS ||
      type == EffectAttributeType::HONOUR ||
      type == EffectAttributeType::WAR_SOUL) {
    return true;
  }
  return false;
}

bool IsPassivityAttribute(EffectAttributeType::type type) {
  if(type == EffectAttributeType::VOCATION ||
      type == EffectAttributeType::MAX_HP ||
      type == EffectAttributeType::MAX_MP ||
      type == EffectAttributeType::MOVE_SPEED ||
      type == EffectAttributeType::STATUS ||
      type == EffectAttributeType::COMMON_STATUS ||
      type == EffectAttributeType::CAMP ||
      type == EffectAttributeType::PHYSIQUE ||
      type == EffectAttributeType::STRENGTH ||
      type == EffectAttributeType::INTELLIGENCE ||
      type == EffectAttributeType::AGILE ||
      type == EffectAttributeType::SPIRIT ||
      type == EffectAttributeType::PHYSICS_ATTACK ||
      type == EffectAttributeType::PHYSICS_DEFENCE ||
      type == EffectAttributeType::MAGIC_ATTACK ||
      type == EffectAttributeType::MAGIC_DEFENCE ||
      type == EffectAttributeType::DODGE ||
      type == EffectAttributeType::CRIT ||
      type == EffectAttributeType::IGNORE_DEFENCE ||
      type == EffectAttributeType::GENDER ||
      type == EffectAttributeType::APPEARANCE ||
      type == EffectAttributeType::BROWN_NAME ||
      type == EffectAttributeType::CURRENT_SWORD ||
      type == EffectAttributeType::CURRENT_SPIRIT_FACADE ||
      type == EffectAttributeType::PACKET_CAPACITY ||
      type == EffectAttributeType::STORAGE_CAPACITY ||
      type == EffectAttributeType::PK_MODE) {
    return true;
  }
  return false;
}

}  // namespace effect

}  // namespace server

}  // namespace game

