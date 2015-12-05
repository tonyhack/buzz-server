//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-12 16:54:53.
// File name: role_attribute_update.cc
//
// Description:
// Define class RoleAttributeUpdate.
//

#include "game_server/server/role_attribute_update.h"

#include "game_server/server/according_level_configure.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_role.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

const static core::int32 kPercentDenom = 10000;

RoleAttributeUpdate::RoleAttributeUpdate(GameRole *role) : role_(role) {}
RoleAttributeUpdate::~RoleAttributeUpdate() {}

bool RoleAttributeUpdate::Initialize() {
  return true;
}

void RoleAttributeUpdate::Finalize() {}

void RoleAttributeUpdate::UpdateAll(bool synchronize) {
  this->UpdateIgnoreDefence(synchronize);
  this->UpdateCrit(synchronize);
  this->UpdateDodge(synchronize);
  this->UpdateMagicDefence(synchronize);
  this->UpdateMagicAttack(synchronize);
  this->UpdatePhysicsDefence(synchronize);
  this->UpdatePhysicsAttack(synchronize);
  this->UpdateSpirit(synchronize);
  this->UpdateAgile(synchronize);
  this->UpdateIntelligence(synchronize);
  this->UpdateStrength(synchronize);
  this->UpdatePhysique(synchronize);
  this->UpdateMoveSpeed(synchronize);
  this->UpdateHp(synchronize);
  this->UpdateMp(synchronize);
  this->UpdateMaxHp(synchronize);
  this->UpdateMaxMp(synchronize);
  this->UpdateCurrentEnergy(synchronize);
  this->UpdateMaxEnergy(synchronize);
  this->UpdateEnergyIncrement(synchronize);
}

void RoleAttributeUpdate::Update(entity::RoleAoiFields::type type, bool synchronize) {
  switch(type) {
    case entity::RoleAoiFields::CURRENT_HP:
      this->UpdateStatus(synchronize);
      break;
    case entity::RoleAoiFields::MAX_HP:
      this->UpdateHp(synchronize);
      break;
    case entity::RoleAoiFields::MAX_MP:
      this->UpdateMp(synchronize);
      break;
    case entity::RoleAoiFields::LEVEL:
      this->UpdateSpirit(synchronize);
      this->UpdateAgile(synchronize);
      this->UpdateIntelligence(synchronize);
      this->UpdateStrength(synchronize);
      this->UpdatePhysique(synchronize);
      this->UpdateMoveSpeed(synchronize);
      this->UpdateMaxEnergy(synchronize);
      break;
    case entity::RoleAoiFields::VOCATION:
      this->UpdateSpirit(synchronize);
      this->UpdateAgile(synchronize);
      this->UpdateIntelligence(synchronize);
      this->UpdateStrength(synchronize);
      this->UpdatePhysique(synchronize);
      this->UpdateMoveSpeed(synchronize);
      break;
    default:
      break;
  }
}

void RoleAttributeUpdate::Update(entity::RoleClientFields::type type,
    bool synchronize) {
  switch(type) {
    case entity::RoleClientFields::PHYSIQUE:
    case entity::RoleClientFields::STRENGTH:
    case entity::RoleClientFields::INTELLIGENCE:
    case entity::RoleClientFields::AGILE:
    case entity::RoleClientFields::SPIRIT:
      this->UpdateIgnoreDefence(synchronize);
      this->UpdateCrit(synchronize);
      this->UpdateDodge(synchronize);
      this->UpdateMagicDefence(synchronize);
      this->UpdateMagicAttack(synchronize);
      this->UpdatePhysicsDefence(synchronize);
      this->UpdatePhysicsAttack(synchronize);
      this->UpdateMoveSpeed(synchronize);
      this->UpdateMaxHp(synchronize);
      this->UpdateMaxMp(synchronize);
      break;
    case entity::RoleClientFields::MAX_ENERGY:
      this->UpdateCurrentEnergy(synchronize);
      break;
    default:
      break;
  }
}

void RoleAttributeUpdate::Update(entity::RoleServerFields::type type,
    bool synchronize) {
  switch(type) {
    case entity::RoleServerFields::MAX_HP_ADD_VALUE:
    case entity::RoleServerFields::MAX_HP_ADD_SECOND_VALUE:
    case entity::RoleServerFields::MAX_HP_ADD_PERCENT:
      this->UpdateMaxHp(synchronize);
      break;
    case entity::RoleServerFields::MAX_MP_ADD_VALUE:
    case entity::RoleServerFields::MAX_MP_ADD_SECOND_VALUE:
    case entity::RoleServerFields::MAX_MP_ADD_PERCENT:
      this->UpdateMaxMp(synchronize);
      break;
    case entity::RoleServerFields::MOVE_SPEED_ADD_VALUE:
    case entity::RoleServerFields::MOVE_SPEED_ADD_SECOND_VALUE:
    case entity::RoleServerFields::MOVE_SPEED_ADD_PERCENT:
      this->UpdateMoveSpeed(synchronize);
      break;
    case entity::RoleServerFields::PHYSIQUE_ADD_VALUE:
    case entity::RoleServerFields::PHYSIQUE_ADD_SECOND_VALUE:
    case entity::RoleServerFields::PHYSIQUE_ADD_PERCENT:
      this->UpdatePhysique(synchronize);
      break;
    case entity::RoleServerFields::STRENGTH_ADD_VALUE:
    case entity::RoleServerFields::STRENGTH_ADD_SECOND_VALUE:
    case entity::RoleServerFields::STRENGTH_ADD_PERCENT:
      this->UpdateStrength(synchronize);
      break;
    case entity::RoleServerFields::INTELLIGENCE_ADD_VALUE:
    case entity::RoleServerFields::INTELLIGENCE_ADD_SECOND_VALUE:
    case entity::RoleServerFields::INTELLIGENCE_ADD_PERCENT:
      this->UpdateIntelligence(synchronize);
      break;
    case entity::RoleServerFields::AGILE_ADD_VALUE:
    case entity::RoleServerFields::AGILE_ADD_SECOND_VALUE:
    case entity::RoleServerFields::AGILE_ADD_PERCENT:
      this->UpdateAgile(synchronize);
      break;
    case entity::RoleServerFields::SPIRIT_ADD_VALUE:
    case entity::RoleServerFields::SPIRIT_ADD_SECOND_VALUE:
    case entity::RoleServerFields::SPIRIT_ADD_PERCENT:
      this->UpdateSpirit(synchronize);
      break;
    case entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE:
    case entity::RoleServerFields::PHYSICS_ATTACK_ADD_SECOND_VALUE:
    case entity::RoleServerFields::PHYSICS_ATTACK_ADD_PERCENT:
      this->UpdatePhysicsAttack(synchronize);
      break;
    case entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE:
    case entity::RoleServerFields::PHYSICS_DEFENCE_ADD_SECOND_VALUE:
    case entity::RoleServerFields::PHYSICS_DEFENCE_ADD_PERCENT:
      this->UpdatePhysicsDefence(synchronize);
      break;
    case entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE:
    case entity::RoleServerFields::MAGIC_ATTACK_ADD_SECOND_VALUE:
    case entity::RoleServerFields::MAGIC_ATTACK_ADD_PERCENT:
      this->UpdateMagicAttack(synchronize);
      break;
    case entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE:
    case entity::RoleServerFields::MAGIC_DEFENCE_ADD_SECOND_VALUE:
    case entity::RoleServerFields::MAGIC_DEFENCE_ADD_PERCENT:
      this->UpdateMagicDefence(synchronize);
      break;
    case entity::RoleServerFields::DODGE_ADD_VALUE:
    case entity::RoleServerFields::DODGE_ADD_SECOND_VALUE:
    case entity::RoleServerFields::DODGE_ADD_PERCENT:
      this->UpdateDodge(synchronize);
      break;
    case entity::RoleServerFields::CRIT_ADD_VALUE:
    case entity::RoleServerFields::CRIT_ADD_SECOND_VALUE:
    case entity::RoleServerFields::CRIT_ADD_PERCENT:
      this->UpdateCrit(synchronize);
      break;
    case entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE:
    case entity::RoleServerFields::IGNORE_DEFENCE_ADD_SECOND_VALUE:
    case entity::RoleServerFields::IGNORE_DEFENCE_ADD_PERCENT:
      this->UpdateIgnoreDefence(synchronize);
      break;
    case entity::RoleServerFields::MAX_ENERGY_ADD_VALUE:
    case entity::RoleServerFields::MAX_ENERGY_ADD_SECOND_VALUE:
    case entity::RoleServerFields::MAX_ENERGY_ADD_PERCENT:
      this->UpdateMaxEnergy(synchronize);
      break;
    case entity::RoleServerFields::ENERGY_INCREMENT_ADD_VALUE:
    case entity::RoleServerFields::ENERGY_INCREMENT_ADD_SECOND_VALUE:
    case entity::RoleServerFields::ENERGY_INCREMENT_ADD_PERCENT:
      this->UpdateEnergyIncrement(synchronize);
      break;
    default:
      break;
  }
}

void RoleAttributeUpdate::UpdateStatus(bool synchronize) {
  // 血量为0且非死亡状态时，设置为死亡状态
  if(this->role_->GetAttribute(entity::RoleAoiFields::CURRENT_HP) == 0 &&
      this->role_->GetBasicStatus() != entity::RoleBasicStatus::STATUS_DIE) {
    this->role_->SetBasicStatus(entity::RoleBasicStatus::STATUS_DIE);
  }
}

void RoleAttributeUpdate::UpdateHp(bool synchronize) {
  // 更新不能超过最大血量
  core::int32 max_hp = this->role_->GetAttribute(entity::RoleAoiFields::MAX_HP);
  if(this->role_->GetAttribute(entity::RoleAoiFields::CURRENT_HP) > max_hp) {
    this->role_->SetAttribute(entity::RoleAoiFields::CURRENT_HP, max_hp, synchronize);
  }
}

void RoleAttributeUpdate::UpdateMp(bool synchronize) {
  // 更新不能超过最大魔法
  core::int32 max_mp = this->role_->GetAttribute(entity::RoleAoiFields::MAX_MP);
  if(this->role_->GetAttribute(entity::RoleAoiFields::CURRENT_MP) > max_mp) {
    this->role_->SetAttribute(entity::RoleAoiFields::CURRENT_MP, max_mp, synchronize);
  }
}

void RoleAttributeUpdate::UpdateMaxHp(bool synchronize) {
  // 最大生命值 = (一级属性最大生命) * (1 + 加成比) + 加成值
  // 一级属性最大生命值 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->max_hp_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->max_hp_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->max_hp_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->max_hp_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->max_hp_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::MAX_HP_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_SECOND_VALUE);
  // 最大生命值必须大于0
  if(value <= 0) {
    value = 1;
  }
  this->role_->SetAttribute(entity::RoleAoiFields::MAX_HP, value, synchronize);
}

void RoleAttributeUpdate::UpdateMaxMp(bool synchronize) {
  // 最大魔法值 = (一级属性最大魔法) * (1 + 加成比) + 加成值
  // 一级属性最大魔法值 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->max_mp_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->max_mp_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->max_mp_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->max_mp_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->max_mp_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::MAX_MP_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_SECOND_VALUE);
  // 最大魔法值必须大于0
  if(value <= 0) {
    value = 1;
  }
  this->role_->SetAttribute(entity::RoleAoiFields::MAX_MP, value, synchronize);
}

void RoleAttributeUpdate::UpdateMoveSpeed(bool synchronize) {
  // 移动速度 = (当前等级移动速度) * (1 + 加成比) + 加成值
  const AccordingLevelCell *cell =
    Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(
      this->role_->GetAttribute(entity::RoleAoiFields::LEVEL));
  if(cell == NULL) {
    return ;
  }
  core::int64 value = 0;
  if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    value += cell->speed_;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
        entity::RoleServerFields::MOVE_SPEED_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::MOVE_SPEED_ADD_SECOND_VALUE);

  // 移动速度必须在配置的速度区间范围内
  if(value > MISC_CONF()->move_speed_min_) {
    value = MISC_CONF()->move_speed_min_;
  }
  if(value < MISC_CONF()->move_speed_max_) {
    value = MISC_CONF()->move_speed_max_;
  }

  this->role_->SetAttribute(entity::RoleAoiFields::MOVE_SPEED, value, synchronize);
}

void RoleAttributeUpdate::UpdatePhysique(bool synchronize) {
  // 体质 = (当前体质) * (1 + 加成比) + 加成值
  // 当前体质 = 等级 * 职业加成倍数
  const VocationGrowthCell *cell = Configure::GetInstance()->GetVocationGrowthCell(
      (entity::VocationType::type)this->role_->GetAttribute(entity::RoleAoiFields::VOCATION));
  if(cell == NULL) {
    return ;
  }
  core::int64 value = cell->physique_ *
    this->role_->GetAttribute(entity::RoleAoiFields::LEVEL) / 100;
  value += this->role_->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
        entity::RoleServerFields::PHYSIQUE_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_SECOND_VALUE);
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::PHYSIQUE, value, synchronize);
}

void RoleAttributeUpdate::UpdateStrength(bool synchronize) {
  // 力量 = (当前力量) * (1 + 加成比) + 加成值
  // 当前力量 = 等级 * 职业加成倍数
  const VocationGrowthCell *cell = Configure::GetInstance()->GetVocationGrowthCell(
      (entity::VocationType::type)this->role_->GetAttribute(entity::RoleAoiFields::VOCATION));
  if(cell == NULL) {
    return ;
  }
  core::int64 value = cell->strength_ *
    this->role_->GetAttribute(entity::RoleAoiFields::LEVEL) / 100;
  value += this->role_->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
        entity::RoleServerFields::STRENGTH_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_SECOND_VALUE);
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::STRENGTH, value, synchronize);
}

void RoleAttributeUpdate::UpdateIntelligence(bool synchronize) {
  // 智力 = (当前智力) * (1 + 加成比) + 加成值
  // 当前智力 = 等级 * 职业加成倍数
  const VocationGrowthCell *cell = Configure::GetInstance()->GetVocationGrowthCell(
      (entity::VocationType::type)this->role_->GetAttribute(entity::RoleAoiFields::VOCATION));
  if(cell == NULL) {
    return ;
  }
  core::int64 value = cell->intelligence_ *
    this->role_->GetAttribute(entity::RoleAoiFields::LEVEL) / 100;
  value += this->role_->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::INTELLIGENCE_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_SECOND_VALUE);
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::INTELLIGENCE, value, synchronize);
}

void RoleAttributeUpdate::UpdateAgile(bool synchronize) {
  // 敏捷 = (当前敏捷) * (1 + 加成比) + 加成值
  // 当前敏捷 = 等级 * 职业加成倍数
  const VocationGrowthCell *cell = Configure::GetInstance()->GetVocationGrowthCell(
      (entity::VocationType::type)this->role_->GetAttribute(entity::RoleAoiFields::VOCATION));
  if(cell == NULL) {
    return ;
  }
  core::int64 value = cell->agile_ *
    this->role_->GetAttribute(entity::RoleAoiFields::LEVEL) / 100;
  value += this->role_->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::AGILE_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::AGILE_ADD_SECOND_VALUE);
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::AGILE, value, synchronize);
}

void RoleAttributeUpdate::UpdateSpirit(bool synchronize) {
  // 精神 = (当前精神) * (1 + 加成比) + 加成值
  // 当前精神 = 等级 * 职业加成倍数
  const VocationGrowthCell *cell = Configure::GetInstance()->GetVocationGrowthCell(
      (entity::VocationType::type)this->role_->GetAttribute(entity::RoleAoiFields::VOCATION));
  if(cell == NULL) {
    return ;
  }
  core::int64 value = cell->spirit_ *
    this->role_->GetAttribute(entity::RoleAoiFields::LEVEL) / 100;
  value += this->role_->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::SPIRIT_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_SECOND_VALUE);
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::SPIRIT, value, synchronize);
}

void RoleAttributeUpdate::UpdatePhysicsAttack(bool synchronize) {
  // 物理攻击值 = (一级属性物理攻击) * (1 + 加成比) + 加成值
  // 一级属性物理攻击 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->physical_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->physical_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->physical_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->physical_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->physical_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::PHYSICS_ATTACK_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_SECOND_VALUE);
  // 物理攻击值必须大于等于0
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::PHYSICS_ATTACK, value, synchronize);
}

void RoleAttributeUpdate::UpdatePhysicsDefence(bool synchronize) {
  // 物理防御值 = (一级属性物理防御) * (1 + 加成比) + 加成值
  // 一级属性物理防御 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->physical_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->physical_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->physical_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->physical_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->physical_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::PHYSICS_DEFENCE_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_SECOND_VALUE);
  // 物理防御值必须大于等于0
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::PHYSICS_DEFENCE, value, synchronize);
}

void RoleAttributeUpdate::UpdateMagicAttack(bool synchronize) {
  // 魔法攻击值 = (一级属性魔法攻击) * (1 + 加成比) + 加成值
  // 一级属属性魔法攻击 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->magic_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->magic_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->magic_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->magic_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->magic_attack_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::MAGIC_ATTACK_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_SECOND_VALUE);
  // 魔法攻击值必须大于等于0
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::MAGIC_ATTACK, value, synchronize);
}

void RoleAttributeUpdate::UpdateMagicDefence(bool synchronize) {
  // 魔法防御值 = (一级属性魔法防御) * (1 + 加成比) + 加成值
  // 一级属性魔法防御 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->magic_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->magic_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->magic_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->magic_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->magic_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::MAGIC_DEFENCE_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_SECOND_VALUE);
  // 魔法防御值必须大于等于0
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::MAGIC_DEFENCE, value, synchronize);
}

void RoleAttributeUpdate::UpdateDodge(bool synchronize) {
  // 闪避值 = (一级属性闪避) * (1 + 加成比) + 加成值
  // 一级属性闪避 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->dodge_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->dodge_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->dodge_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->dodge_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->dodge_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::DODGE_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::DODGE_ADD_SECOND_VALUE);
  // 闪避值必须大于等于0
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::DODGE, value, synchronize);
}

void RoleAttributeUpdate::UpdateCrit(bool synchronize) {
  // 暴击值 = (一级属性暴击) * (1 + 加成比) + 加成值
  // 一级属性暴击 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->crit_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->crit_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->crit_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->crit_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->crit_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::CRIT_ADD_SECOND_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::CRIT_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE);
  // 暴击值必须大于等于0
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::CRIT, value, synchronize);
}

void RoleAttributeUpdate::UpdateIgnoreDefence(bool synchronize) {
  // 会心值 = (一级属性会心) * (1 + 加成比) + 加成值
  // 一级属性会心 = 一级属性转换之和
  core::int64 value = 0;
  // 体质加成
  const AttrConvertCell *physique_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::PHYSIQUE);
  if(physique_cell) {
    value += physique_cell->ignore_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::PHYSIQUE) / 100;
  }
  // 力量加成
  const AttrConvertCell *strength_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::STRENGTH);
  if(strength_cell) {
    value += strength_cell->ignore_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::STRENGTH) / 100;
  }
  // 智力加成
  const AttrConvertCell *intelligence_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::INTELLIGENCE);
  if(intelligence_cell) {
    value += intelligence_cell->ignore_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::INTELLIGENCE) / 100;
  }
  // 敏捷加成
  const AttrConvertCell *agile_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::AGILE);
  if(agile_cell) {
    value += agile_cell->ignore_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::AGILE) / 100;
  }
  // 精神加成
  const AttrConvertCell *spirit_cell = Configure::GetInstance()->GetAttrConvertCell(
      entity::RoleClientFields::SPIRIT);
  if(spirit_cell) {
    value += spirit_cell->ignore_defence_ *
      this->role_->GetAttribute(entity::RoleClientFields::SPIRIT) / 100;
  }
  value += this->role_->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_SECOND_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::IGNORE_DEFENCE_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE);
  // 会心值必须大于等于0
  if(value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::IGNORE_DEFENCE, value, synchronize);
}

void RoleAttributeUpdate::UpdateCurrentEnergy(bool synchronize) {
  core::int32 max_energy = this->role_->GetAttribute(entity::RoleClientFields::MAX_ENERGY);
  if(this->role_->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY) > max_energy) {
    this->role_->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY, max_energy, synchronize);
  }
}

void RoleAttributeUpdate::UpdateMaxEnergy(bool synchronize) {
  core::int64 value = 0;
  const AccordingLevelCell *cell =
    Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(
      this->role_->GetAttribute(entity::RoleAoiFields::LEVEL));
  if (cell == NULL) {
    return ;
  }
  value += cell->max_energy_;
  value += this->role_->GetAttribute(entity::RoleServerFields::MAX_ENERGY_ADD_SECOND_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::MAX_ENERGY_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::MAX_ENERGY_ADD_VALUE);
  if (value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::MAX_ENERGY, value, synchronize);
}

void RoleAttributeUpdate::UpdateEnergyIncrement(bool synchronize) {
  core::int64 value = 0;
  const AccordingLevelCell *cell =
    Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(
      this->role_->GetAttribute(entity::RoleAoiFields::LEVEL));
  if (cell == NULL) {
    return ;
  }
  value += cell->energy_increment_;
  value += this->role_->GetAttribute(entity::RoleServerFields::ENERGY_INCREMENT_ADD_SECOND_VALUE);
  value = value * (kPercentDenom + this->role_->GetAttribute(
      entity::RoleServerFields::ENERGY_INCREMENT_ADD_PERCENT)) / kPercentDenom;
  value += this->role_->GetAttribute(entity::RoleServerFields::ENERGY_INCREMENT_ADD_VALUE);
  if (value < 0) {
    value = 0;
  }
  this->role_->SetAttribute(entity::RoleClientFields::ENERGY_INCREMENT, value, synchronize);
}

}  // namespace server

}  // namespace game

