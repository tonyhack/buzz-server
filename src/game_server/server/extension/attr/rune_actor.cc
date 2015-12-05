//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-21 21:05:22.
// File name: rune_actor.cc
//
// Description:
//
#include "game_server/server/extension/attr/rune_actor.h"

#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/game_actor.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace attr {

RuneActor::RuneActor() {}
RuneActor::~RuneActor() {}

bool RuneActor::Initialize(AttrActor *attr_actor, 
    const database::protocol::StorageAttrActorField &db_field) {
  if(attr_actor == NULL) {
    LOG_ERROR("attr_actor 为空");
    return false;
  }
  for(core::int32 i = 0; i < (core::int32)RuneActor::kMaxRuneSlotNum; ++i) {
    this->slots_[i] = 0;
  }
  // 初始化
  this->attr_actor_ = attr_actor;
  if(db_field.rune_slot_data_.size() != 0) {
    std::vector<std::string> results;
    std::vector<core::int32> values;
    core::int32 num = global::StringSplit(db_field.rune_slot_data_.c_str(), ",", results);
    if(num == 0 || (core::int32)results.size() != num) {
      LOG_ERROR("数据存储存在问题 num[%d], len[%d]", num, 
          db_field.rune_slot_data_.size());
      return false;
    }
    for(size_t i = 0; i < results.size(); ++i) {
      num = global::StringSplit(results[i].c_str(), "+", values);
      if(num != 2) {
        LOG_ERROR("长度不对num[%d]", num);
        return false;
      }
      if(values[0] >= (core::int32)RuneActor::kMaxRuneSlotNum || values[0] < 0) {
        LOG_ERROR("符文槽位置错误[%d]", values[0]);
        return false;
      }
      this->SetRune(values[0], values[1], false);
      values.clear();
    }
  }

  return true;
}

void RuneActor::InitSynchronize(gateway::protocol::MessageAttrSpiritInitialize &message) {
  this->GetRuneSlots(message.runes_);
}

void RuneActor::Finalize() {
  this->attr_actor_ = NULL;
  for(core::int32 i = 0; i < (core::int32)RuneActor::kMaxRuneSlotNum; ++i) {
    this->slots_[i] = 0;
  }
}

void RuneActor::Save(database::protocol::StorageAttrActorField &db_field) {
  char buffer[256] = "";
  size_t count = 0;
  for(core::int32 i = 0; i < (core::int32)RuneActor::kMaxRuneSlotNum; ++i) {
    if(this->slots_[i] != 0) {
      count += snprintf(buffer+count, sizeof(buffer) - count,"%d+%d,", i, this->slots_[i]); 
    }
  }
  db_field.__set_rune_slot_data_(buffer);
}

void RuneActor::SlotDataSynchronize(core::uint8 location) {
  if(location >= RuneActor::kMaxRuneSlotNum) {
    LOG_ERROR("输入的参数不正确");
    return ;
  }
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("玩家 AttrActor 对象没有找到");
    return ;
  }
  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家GameActor没有找到");
    return ;
  }
  gateway::protocol::MessageRuneSynchronize message;
  gateway::protocol::SpiritRuneData slot;
  slot.__set_location_(core::int8(location));
  slot.__set_template_id_(this->slots_[location]);
  message.__set_data_(slot);
  game_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_SYNCHRONIZE);

}

bool RuneActor::CheckPlaceSlot(core::uint32 template_id, core::uint8 location) {
  if(location >= RuneActor::kMaxRuneSlotNum) {
    LOG_ERROR("参数错误[%d]", location);
    return false;
  }
  //得到相近的符文槽
  core::uint8 near = 0;
  if(location%RuneActor::kMaxVocationSlotNum == 0) {
    near = location+1;
  } else {
    near = location-1;
  }
  if(this->slots_[near] == 0) {
    return true;
  }
  // 获取配置
  const RuneCell *cell1 = Configure::GetInstance()->GetRuneCell(template_id);
  if(cell1 == NULL) {
    LOG_ERROR("配置没有找到[%d]", template_id);
    return false;
  }
  const RuneCell *cell2 = Configure::GetInstance()->GetRuneCell(this->slots_[near]);
  if(cell2 == NULL) {
    LOG_ERROR("配置没有找到[%d]", this->slots_[near]);
    return false;
  }
  if(cell1->type_1_ == cell2->type_1_) {
    LOG_DEBUG("相同的属性符文不能装备在同一种职业的槽位中");
    return false;
  }
  return true;
}

core::uint32 RuneActor::GetSlotTemplateID(core::uint8 location) {
  if(location >= RuneActor::kMaxRuneSlotNum) {
    LOG_ERROR("参数错误[%d]", location);
    return false;
  }
  return this->slots_[location];
}

bool RuneActor::CheckVocation(core::uint8 location, entity::VocationType::type type) {
  // 检查槽位职业
  entity::VocationType::type location_type = entity::VocationType::MAX;
  if(location < RuneActor::kMaxRuneSlotNum) {
    location_type = (entity::VocationType::type)(location/RuneActor::kMaxVocationSlotNum); 
  }
  if(location_type == entity::VocationType::MAX) {
    LOG_ERROR("位置错误[%d]", location);
    return false;
  }
  if(location_type == type) {
    return true;
  }
  return false;
}

entity::VocationType::type RuneActor::GetVocation() {
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("属性对象没有找到");
    return entity::VocationType::MAX;
  }
  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return entity::VocationType::MAX;
  }
  return (entity::VocationType::type)game_actor->GetAttribute(
      entity::RoleAoiFields::VOCATION);
}

bool RuneActor::SetRune(core::uint8 location, core::uint32 template_id, bool sync) {
  if(location >= RuneActor::kMaxRuneSlotNum) {
    LOG_ERROR("参数错误[%d]", location);
    return false;
  }
  if(this->slots_[location] != 0 && this->CheckVocation(location,
        this->GetVocation()) == true) {
    this->DesAttribute(this->slots_[location], sync);
  }
  this->slots_[location] = template_id;
  if(this->slots_[location] != 0 && this->CheckVocation(location,
        this->GetVocation()) == true) {
    this->AddAttribute(this->slots_[location], sync);
  }

  // 更新战斗力
  this->UpdateRuneFightScore(this->GetVocation(), sync);

  return true;
}

void RuneActor::AddAttribute(core::uint32 template_id, bool sync) {
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("属性对象没有找到");
    return ;
  }
  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  const RuneCell *cell = Configure::GetInstance()->GetRuneCell(
      template_id);
  if(cell == NULL) {
    LOG_ERROR("配置没有找到[%d]", template_id);
    return ;
  }
  switch((entity::RuneAttributeType::type)cell->type_1_) {
    case entity::RuneAttributeType::PHYSIQUE:
      game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE)
          + cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::STRENGTH:
      game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE)
          + cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::INTELLIGENCE:
      game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE)
          + cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::AGILE:
      game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE)
          + cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::SPIRIT:
      game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE)
          + cell->attr_1_, sync);
      break;
    default:
      break;
  } 
}

void RuneActor::DesAttribute(core::uint32 template_id, bool sync) {
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("属性对象没有找到");
    return ;
  }
  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  const RuneCell *cell = Configure::GetInstance()->GetRuneCell(
      template_id);
  if(cell == NULL) {
    LOG_ERROR("配置没有找到[%d]", template_id);
    return ;
  }
  switch((entity::RuneAttributeType::type)cell->type_1_) {
    case entity::RuneAttributeType::PHYSIQUE:
      game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE)
          - cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::STRENGTH:
      game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE)
          - cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::INTELLIGENCE:
      game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE)
          - cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::AGILE:
      game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE)
          - cell->attr_1_, sync);
      break;
    case entity::RuneAttributeType::SPIRIT:
      game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE)
          - cell->attr_1_, sync);
      break;
    default:
      break;
  } 
}

void RuneActor::OnVocation(entity::VocationType::type src_vocation) {
  for(core::int32 i = 0; i < (core::int32)RuneActor::kMaxRuneSlotNum; ++i) {
    if(this->slots_[i] != 0) {
      if(this->CheckVocation(i, src_vocation) == true) {
        // 删除原来的属性 
        this->DesAttribute(this->slots_[i], true); 
      }
    }
    if(this->slots_[i] != 0) {
      if(this->CheckVocation(i, this->GetVocation()) == true) {
        this->AddAttribute(this->slots_[i], true);
      }
    }
  }
}

void RuneActor::GetRuneSlots(std::vector<gateway::protocol::SpiritRuneData> &out) {
  for(core::int32 i = 0; i < (core::int32)RuneActor::kMaxRuneSlotNum; ++i) {
    if(this->slots_[i] != 0) {
      gateway::protocol::SpiritRuneData  data;
      data.__set_location_(i);
      data.__set_template_id_(this->slots_[i]);
      out.push_back(data);
    }
  }
}

static void AddAttributeToCalculator(FightScoreCalculator *calc,
    entity::RuneAttributeType::type type, core::int32 value) {
  switch(type) {
    case entity::RuneAttributeType::PHYSIQUE:
      calc->physique_ += value;
      break;
    case entity::RuneAttributeType::STRENGTH:
      calc->strength_ += value;
      break;
    case entity::RuneAttributeType::INTELLIGENCE:
      calc->intelligence_ += value;
      break;
    case entity::RuneAttributeType::AGILE:
      calc->agile_ += value;
      break;
    case entity::RuneAttributeType::SPIRIT:
      calc->spirit_ += value;
      break;
    default:
      break;
  }
}

void RuneActor::UpdateRuneFightScore(entity::VocationType::type vocation, bool sync) {
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("属性对象没有找到");
    return ;
  }
  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  FightScoreCalculator calc;
  
  for(core::int32 i = 0; i < (core::int32)RuneActor::kMaxRuneSlotNum; ++i) {
    if(this->slots_[i] != 0 && this->CheckVocation(i, vocation)) {
      const RuneCell *cell = Configure::GetInstance()->GetRuneCell(this->slots_[i]);
      if(cell == NULL) {
        LOG_ERROR("配置没有找到[%d]", this->slots_[i]);
        return ;
      }

      AddAttributeToCalculator(&calc, cell->type_1_, cell->attr_1_);
    }
  }

  entity::SubFightScoreType::type fs_type;
  if(vocation == entity::VocationType::AXE) {
    fs_type = entity::SubFightScoreType::AXE_RUNE;
  } else if (vocation == entity::VocationType::SHOOT) {
    fs_type = entity::SubFightScoreType::SHOOT_RUNE;
  } else if (vocation == entity::VocationType::MAGIC) {
    fs_type = entity::SubFightScoreType::MAGIC_RUNE;
  } else {
    LOG_ERROR("无效的职业[%d]", vocation);
    return ;
  }

  game_actor->SetSubFightScore(fs_type, calc.GetFightScore(), sync);
}

} // namespace attr

} // namespace server

} // namespace game

