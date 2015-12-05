//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 21:43:35.
// File name: buff_role.cc
//
// Description:
// Define class BuffRole.
//

#include "game_server/server/extension/buff/buff_role.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_buff_save_types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/buff/configure.h"
#include "game_server/server/extension/buff/buff.h"
#include "game_server/server/extension/buff/buff_pool.h"
#include "game_server/server/global_time_tick.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

BuffRole::BuffRole() : role_(NULL) {}
BuffRole::~BuffRole() {}

bool BuffRole::Initialize(GameRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 参数 role 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->role_ = role;

  return true;
}

void BuffRole::Finalize() {
  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end(); ++iterator) {
    if(iterator->second) {
      // iterator->second->End();
      iterator->second->Finalize();
      BuffPool::GetInstance()->Deallocate(iterator->second);
    }
  }

  this->buffs_.clear();
  this->forbids_.clear();

  this->role_ = NULL;
}

bool BuffRole::InitializeBuffs(const BuffVector &buffs) {
  BuffVector::const_iterator iterator = buffs.begin();
  for(; iterator != buffs.end(); ++iterator) {
    if(this->StartBuff(entity::EntityType::TYPE_MAX, 0,
          iterator->buff_id_, iterator->left_time_) == false) {
      global::LogError("%s:%d (%s) 初始时Buff数据时，开启buff(%u) 失败",
          __FILE__, __LINE__, __FUNCTION__, iterator->buff_id_);
      continue;
    }
    global::LogDebug("初始化buff数据 buff:%d, lefttime:%d",
        iterator->buff_id_, iterator->left_time_);
  }

  return true;
}

void BuffRole::ExportBuffs(std::vector<gateway::protocol::BuffField> &buffs) {
  gateway::protocol::BuffField field;
  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end(); ++iterator) {
    if(iterator->second) {
      field.__set_buff_(iterator->second->GetBuffID());
      field.__set_left_time_(iterator->second->GetLeftTime());
      field.__set_stack_count_(iterator->second->GetStackCount());
      buffs.push_back(field);
    }
  }
}

bool BuffRole::StartBuff(entity::EntityType::type source_type, core::int64 source_id,
    core::uint32 buff_id, core::uint32 during_time) {
  // 获取配置
  const BuffCell *cell = Configure::GetInstance()->GetBuffCell(buff_id);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 找不到 buff(%u) 配置",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  // 检测是否禁止当前BUFF组
  if(this->CheckForbidGroup(cell->clear_group_)) {
    return false;
  }

  // 如果BUFF存在，直接开启
  Buff *buff = this->GetBuff(buff_id);
  if(buff) {
    global::LogDebug("增加buff(%u)时，已存在，叠加并重置BUFF时间", buff_id);
    return buff->Start();
  } else {
    // 同组BUFF处理
    BuffMap::iterator iterator = this->buffs_.begin();
    for(; iterator != this->buffs_.end(); ++iterator) {
      if(iterator->second == NULL) {
        global::LogError("%s:%d (%s) 开启BUFF(%u)时，列表中存在空指针",
            __FILE__, __LINE__, __FUNCTION__, buff_id);
        return false;
      }
      if((core::uint32)iterator->second->GetBuffID() != buff_id) {
        const BuffCell *buff_cell = iterator->second->GetBuffCell();
        if(buff_cell == NULL) {
          global::LogError("%s:%d (%s) 开启BUFF(%u)时，配置错误",
              __FILE__, __LINE__, __FUNCTION__, buff_id);
          return false;
        }
        // 同组BUFF时，小于已存在的同组BUFF时，忽略本次BUFF
        if(cell->group_ != 0 && cell->group_ == buff_cell->group_) {
          if(cell->group_level_ < buff_cell->group_level_) {
            return true;
          }
          // 大于等于已存在的同组BUFF时，停止这个BUFF
          this->EndBuff(iterator->second->GetBuffID());
          break;
        }
      }
    }
  }

  // 分配 BUFF 对象
  buff = BuffPool::GetInstance()->Allocate();
  if(buff == NULL) {
    global::LogError("%s:%d (%s) 分配 BUFF(%u) 对象失败",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  // 初始化 BUFF 对象
  if(buff->Initialize(this, buff_id, during_time, source_type, source_id) == false) {
    BuffPool::GetInstance()->Deallocate(buff);
    global::LogError("%s:%d (%s) 初始化 BUFF(%u) 对象失败",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  // 开启 BUFF
  if(buff->Start() == false) {
    buff->Finalize();
    BuffPool::GetInstance()->Deallocate(buff);
    global::LogError("%s:%d (%s) 开启 BUFF(%u) 对象失败",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  // 加入 BUFF列表
  if(this->AddBuff(buff) == false) {
    buff->End();
    buff->Finalize();
    BuffPool::GetInstance()->Deallocate(buff);
    global::LogError("%s:%d (%s) 加入 BUFF(%u) 对象到列表失败",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  global::LogDebug("ROLE(%u,%lu) BUFF(%u) 开启",
      this->role_->GetType(), this->role_->GetID(), buff_id);

  return true;
}

bool BuffRole::EndBuff(core::uint32 buff_id) {
  Buff *buff = this->RemoveBuff(buff_id);
  if(buff == NULL) {
    global::LogError("%s:%d (%s) 删除BUFF(%u)对象失败",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  if(buff->End() == false) {
    global::LogError("%s:%d (%s) 结束BUFF(%u)失败",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  buff->Finalize();
  BuffPool::GetInstance()->Deallocate(buff);

  global::LogDebug("ROLE(%u,%lu) BUFF(%u) 结束",
      this->role_->GetType(), this->role_->GetID(), buff_id);

  return true;
}

void BuffRole::IncreaseForbidGroup(core::int32 group) {
  ForbidGroupMap::iterator iterator = this->forbids_.find(group);
  if(iterator != this->forbids_.end()) {
    ++iterator->second;
  } else {
    this->forbids_.insert(std::make_pair(group, 1));
  }
}

void BuffRole::DecreaseForbidGroup(core::int32 group) {
  ForbidGroupMap::iterator iterator = this->forbids_.find(group);
  if(iterator != this->forbids_.end()) {
    if(iterator->second > 0) {
      --iterator->second;
    }
  }
}

bool BuffRole::CheckForbidGroup(core::int32 group) const {
  ForbidGroupMap::const_iterator iterator = this->forbids_.find(group);
  if(iterator != this->forbids_.end()) {
    return iterator->second > 0;
  }

  return false;
}

bool BuffRole::CheckBuffConditionGroupExist(core::int32 group) const {
  BuffMap::const_iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end(); ++iterator) {
    Buff *buff = iterator->second;
    if(buff && buff->GetBuffCell() && buff->GetBuffCell()->condition_group_ == group) {
      return true;
    }
  }

  return false;
}

void BuffRole::ClearGroupBuff(core::int32 group) {
  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end();) {
    BuffMap::iterator iterator2 = iterator;
    ++iterator2;

    Buff *buff = iterator->second;
    if(buff == NULL) {
      LOG_ERROR("Role(%u, %lu) BUFF列表中有空指针",
          this->GetType(), this->GetID());
      continue;
    }

    const BuffCell *cell = buff->GetBuffCell();
    if(cell == NULL) {
      LOG_ERROR("Role(%u, %lu) GetBuffCell 为空",
          this->GetType(), this->GetID());
      continue;
    }

    if(cell->clear_group_ == group) {
      this->EndBuff(buff->GetBuffID());
    }

    iterator = iterator2;
  }
}

void BuffRole::OnDeath() {
  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end();) {
    BuffMap::iterator iterator2 = iterator;
    ++iterator2;

    Buff *buff = iterator->second;
    if(buff == NULL) {
      LOG_ERROR("Role(%u, %lu) BUFF列表中有空指针",
          this->GetType(), this->GetID());
      continue;
    }

    const BuffCell *cell = buff->GetBuffCell();
    if(cell == NULL) {
      LOG_ERROR("Role(%u, %lu) GetBuffCell 为空",
          this->GetType(), this->GetID());
      continue;
    }

    if(cell->end_conditions_ & entity::BuffEndConditionType::DEAD) {
      this->EndBuff(buff->GetBuffID());
    }

    iterator = iterator2;
  }
}

void BuffRole::OnHurt() {
  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end();) {
    BuffMap::iterator iterator2 = iterator;
    ++iterator2;

    Buff *buff = iterator->second;
    if(buff == NULL) {
      LOG_ERROR("Role(%u, %lu) BUFF列表中有空指针",
          this->GetType(), this->GetID());
      continue;
    }

    const BuffCell *cell = buff->GetBuffCell();
    if(cell == NULL) {
      LOG_ERROR("Role(%u, %lu) GetBuffCell 为空",
          this->GetType(), this->GetID());
      continue;
    }

    if(cell->end_conditions_ & entity::BuffEndConditionType::HURT) {
      this->EndBuff(buff->GetBuffID());
    }

    iterator = iterator2;
  }
}

void BuffRole::OnCurrentEnergyDepleted() {
  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end();) {
    BuffMap::iterator iterator2 = iterator;
    ++iterator2;

    Buff *buff = iterator->second;
    if(buff == NULL) {
      LOG_ERROR("Role(%u, %lu) BUFF列表中有空指针",
          this->GetType(), this->GetID());
      continue;
    }

    const BuffCell *cell = buff->GetBuffCell();
    if(cell == NULL) {
      LOG_ERROR("Role(%u, %lu) GetBuffCell 为空",
          this->GetType(), this->GetID());
      continue;
    }

    if(cell->end_conditions_ & entity::BuffEndConditionType::CURRENT_ENERGY_DEPLETED) {
      this->EndBuff(buff->GetBuffID());
    }

    iterator = iterator2;
  }
}

void BuffRole::OnEvent(event::EventType::type type, const void *data, size_t size) {
  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end();) {
    BuffMap::iterator iterator2 = iterator;
    ++iterator2;

    Buff *buff = iterator->second;
    if(buff == NULL) {
      LOG_ERROR("Role(%u, %lu) BUFF列表中有空指针",
          this->GetType(), this->GetID());
      continue;
    }

    buff->OnEvent(type, data, size);

    iterator = iterator2;
  }
}

void BuffRole::Save() {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) Role(%u,%lu) 为空",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID());
    return ;
  }

  database::protocol::StorageBuffSaveRequest request;
  request.__set_id_(this->role_->GetID());

  size_t number = 0;

  BuffMap::iterator iterator = this->buffs_.begin();
  for(; iterator != this->buffs_.end(); ++iterator) {
    if(number >= BuffRole::max_save_number_) {
      // 发送消息
      if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
            request, database::protocol::StorageType::STORAGE_BUFF_SAVE,
            this->role_->GetID()) == false) {
        global::LogError("%s:%d (%s) Role(%u,%lu) 保存BUFF信息失败",
            __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
            this->role_->GetID());
        return ;
      }
      request.fields_.clear();
      request.__isset.fields_ = false;
      number = 0;
    }
    if(iterator->second) {
      if(!iterator->second->IsNeedSaveDB()) {
        continue;
      }
      ++number;
      database::protocol::StorageBuffField field;
      field.__set_buff_id_(iterator->first);
      field.__set_left_time_(iterator->second->GetLeftTime());
      field.__set_stack_count_(iterator->second->GetStackCount());
      request.fields_.push_back(field);
      request.__isset.fields_ = true;
    }
  }

  // 发送消息
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_BUFF_SAVE,
        this->role_->GetID()) == false) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 保存BUF信息失败",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID());
    return ;
  }

  global::LogInfo("玩家(%lu) 保存buff信息", this->role_->GetID());
}

Buff *BuffRole::GetBuff(core::uint32 buff_id) {
  BuffMap::iterator iterator = this->buffs_.find(buff_id);
  if(iterator != this->buffs_.end()) {
    return iterator->second;
  }

  return NULL;
}

bool BuffRole::AddBuff(Buff *buff) {
  if(buff == NULL) {
    return false;
  }

  BuffMap::iterator iterator = this->buffs_.find(buff->GetBuffID());
  if(iterator != this->buffs_.end()) {
    return false;
  }

  this->buffs_.insert(std::make_pair(buff->GetBuffID(), buff));

  return true;
}

Buff *BuffRole::RemoveBuff(core::uint32 buff_id) {
  Buff *buff = NULL;

  BuffMap::iterator iterator = this->buffs_.find(buff_id);
  if(iterator != this->buffs_.end()) {
    buff = iterator->second;
    this->buffs_.erase(iterator);
  }

  return buff;
}

}  // namespace buff

}  // namespace server

}  // namespace game

