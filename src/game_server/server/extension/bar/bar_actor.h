//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-30 18:12:41.
// File name: bar_actor.h
//
// Description:
//
#ifndef __GAME__SERVER__BAR__BAR__ACTOR__H
#define __GAME__SERVER__BAR__BAR__ACTOR__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/bar/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/global_time_tick.h"
#include "gateway_server/protocol/gateway_bar_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "core/base/types.h"
#include "entity/bar_types.h"

namespace game {

namespace server {

namespace bar {

class BarActor : public core::Noncopyable {
 public:
  BarActor() {}
  ~BarActor() {}

  bool Initialize(GameActor *actor) {
    if(actor == NULL) {
      LOG_ERROR("读条ID没有找到");
      return false;
    }
    this->actor_ = actor;
    this->id_ = 0;
    this->start_time_ = 0;
    this->type_ = entity::BarModuleType::BAR_INVALID;
    return true;
  }

  void Finalize() {
    this->actor_ = 0;
    this->id_ = 0;
    this->start_time_ = 0;
    this->type_ = entity::BarModuleType::BAR_INVALID;
  }

  core::uint64 GetID() const {
    if(this->actor_ != NULL) {
      return this->actor_->GetActorID();
    }
    return 0;
  }

  GameActor *GetActor() const {
    return this->actor_;
  }

  // 获取进度条有没有完成
  bool CheckBar(core::int32 bar_id) {
    if(bar_id != this->id_) {
      LOG_ERROR("安装的进度条是[%d], 请求的进度条是[%d]",
          this->id_, bar_id);
      return false;
    }
    // 获取进度条配置
    const BarCell *cell = Configure::GetInstance()->GetCell(bar_id);
    if(cell == NULL) {
      LOG_ERROR("配置没有找到[%d]", bar_id);
      return false;
    }
    // 验证时间
    time_t cr = GlobalTimeTick::GetInstance()->TimeTick().GetRealMsec();
    if((cr - this->start_time_) < (cell->time_-100)) {
      LOG_ERROR("时间不足[%d]", cell->time_-100);
      return false;
    }
    return true;
  }

  // 安装进度条
  bool StarBar(core::int32 bar_id, entity::BarModuleType::type type) {
    if(this->actor_ == NULL || type == entity::BarModuleType::BAR_INVALID) {
      LOG_ERROR("安装进度条失败type[%d]", type);
      return false;
    }

    // 获取进度条配置
    const BarCell *cell = Configure::GetInstance()->GetCell(bar_id);
    if(cell == NULL) {
      LOG_ERROR("配置没有找到[%d]", bar_id);
      return false;
    }
    this->id_ = bar_id;
    this->type_ = type;

    // 设定时间
    this->start_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealMsec();

    gateway::protocol::MessageBarStartSynchoronize message;
    message.__set_bar_id_(this->id_);
    this->actor_->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_BAR_START_SYNCHRONIZE);
    return true;
  }

  void StopBar() {
    this->id_ = 0;
    this->type_ = entity::BarModuleType::BAR_INVALID;
    this->start_time_ = 0;
  }

  // 取消进度条
  void CancelBar(entity::BarEventType::type type, bool send_msg = true) {
    const BarCell *cell = Configure::GetInstance()->GetCell(this->id_);
    if(cell == NULL) {
      LOG_ERROR("进度条配置没有找到 bar[%d]", this->id_);
      return ;
    }
    if(cell->events_.size() != 0) {
      for(size_t i = 0; i < cell->events_.size(); ++i) {
        if(cell->events_[i] == (core::int32)type) {
          if(send_msg == true && actor_ != NULL) {
            this->StopBar();
            gateway::protocol::MessageBarInterrupt message;
            message.__set_interrupt_type_(type);
            actor_->SendMessage(message,
                gateway::protocol::MessageType::MESSAGE_BAR_INTERRUPT);
          }
          return ;
        }
      }
      return ;
    }

    LOG_DEBUG("当前的进度条[%d]被事件[%d]打断", this->id_, type);

    this->StopBar();

    if(send_msg == true && actor_ != NULL) {
      gateway::protocol::MessageBarInterrupt message;
      message.__set_interrupt_type_(type);
      actor_->SendMessage(message,
          gateway::protocol::MessageType::MESSAGE_BAR_INTERRUPT);
    }
  }

  bool BarExist() {
    return this->id_ == 0 ? false : true;
  }

  inline entity::BarModuleType::type GetType() const {
    return this->type_;
  }

 private:
  // 玩家对象指针
  GameActor *actor_;

  // 玩家进度条ID
  core::int32 id_;

  // 进度条所属类型
  entity::BarModuleType::type type_;

  // 读条开始事件
  core::uint64 start_time_;
};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__BAR__ACTOR__H
