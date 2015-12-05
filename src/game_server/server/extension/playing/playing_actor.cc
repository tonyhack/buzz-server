//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-17 14:38:28.
// File name: playing_actor.cc
//
// Description:
// Define class PlayingActor.
//

#include "game_server/server/extension/playing/playing_actor.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_playing_actor_save_types.h"
#include "database_server/protocol/database_storage_types.h"
#include "entity/playing_types.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/extension/playing/facade_request.h"
#include "game_server/server/extension/playing/playing_page.h"
#include "game_server/server/extension/playing/playing_page_pool.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/configure/configure.h"
#include "global/common_functions.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace playing {

PlayingActor::PlayingActor() {}
PlayingActor::~PlayingActor() {}

bool PlayingActor::Initialize(GameActor *actor,
    const database::protocol::StoragePlayingActorField &field) {
  if (actor == NULL) {
    LOG_ERROR("参数 actor 为NULL");
    return false;
  }

  this->actor_ = actor;
  this->playing_ = 0;
  this->playing_team_ = 0;

  this->auto_playing_ = field.auto_playing_;
  this->auto_playing_group_ = field.auto_playing_group_;
  this->auto_finish_time_ = field.auto_finish_time_;

  std::vector<std::string> str_vector;
  std::vector<core::int32> field_vector;

  // 记录
  entity::PlayingRecordField record;
  global::StringSplit(field.records_.c_str(), ",", str_vector);
  for (size_t pos = 0; pos < str_vector.size(); ++pos) {
    global::StringSplit(str_vector[pos].c_str(), "+", field_vector);
    size_t field_size = field_vector.size();
    if (field_size < 5) {
      return false;
    }
    size_t num = field_vector[4];
    if (field_size != num + 5) {
      LOG_ERROR("玩家(%lu) 副本 records 字段格式错误", this->GetID());
      return false;
    }
    core::int32 template_id = field_vector[0];
    record.values_.clear();
    record.__set_period_count_(field_vector[1]);
    record.__set_spend_secs_(field_vector[2]);
    record.__set_first_pass_awarded_(field_vector[3]);
    for (size_t i = 0; i < num; ++i) {
      record.values_.push_back(field_vector[i+5]);
    }
    if (this->GetPage(template_id) != NULL) {
      LOG_ERROR("玩家(%lu) 副本记录(%d) 重复", this->GetID(), template_id);
      return false;
    }
    PlayingPage *page = PlayingPagePool::GetInstance()->Allocate();
    if (page == NULL) {
      LOG_ERROR("玩家(%lu) 分配 PlayingPage 失败", this->GetID());
      return false;
    }
    if (page->Initialize(this, template_id) == false) {
      PlayingPagePool::GetInstance()->Deallocate(page);
      LOG_ERROR("玩家(%lu) 初始化 PlayingPage(%d) 失败", this->GetID());
      return false;
    }
    if (this->AddPage(page) == false) {
      page->Finalize();
      PlayingPagePool::GetInstance()->Deallocate(page);
      LOG_ERROR("玩家(%lu) 加入 PlayingPage(%d) 失败", this->GetID());
      return false;
    }
    page->SetRecord(record);
  }

  // 结果
  entity::PlayingResultField result;
  global::StringSplit(field.results_.c_str(), ",", str_vector);
  for (size_t pos = 0; pos < str_vector.size(); ++pos) {
    global::StringSplit(str_vector[pos].c_str(), "+", field_vector);
    size_t field_size = field_vector.size();
    if (field_size < 5) {
      LOG_ERROR("玩家(%lu) 副本 results 字段格式错误", this->GetID());
      return false;
    }
    size_t num = field_vector[4];
    if (field_size != num + 5) {
      LOG_ERROR("玩家(%lu) 副本 results 字段格式错误", this->GetID());
      return false;
    }
    core::int32 template_id = field_vector[0];
    PlayingPage *page = this->GetPage(template_id);
    if (page == NULL) {
      LOG_ERROR("玩家(%lu) 副本记录(%d) 不存在", this->GetID(), template_id);
      return false;
    }
    result.values_.clear();
    result.__set_awarded_(field_vector[1] != 0);
    result.__set_award_count_(field_vector[2]);
    result.__set_paid_award_count_(field_vector[3]);
    for (size_t i = 0; i < num; ++i) {
      result.values_.push_back(field_vector[i+5]);
    }
    LOG_DEBUG("副本 PlayingResultField(%d) paid_award_count_=%d",
        template_id, result.paid_award_count_);
    page->SetResult(result);
  }

  gateway::protocol::MessagePlayingInitialize message;
  if (this->auto_playing_ > 0) {
    message.__set_auto_playing_(this->auto_playing_);
  }
  if (this->auto_playing_group_ > 0) {
    message.__set_auto_playing_group_(this->auto_playing_group_);
  }
  if (this->auto_finish_time_ > 0) {
    time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    if (now > this->auto_finish_time_) {
      message.__set_auto_remain_secs_(0);
    } else {
      message.__set_auto_remain_secs_(this->auto_finish_time_ - now);
    }
  }

  session::protocol::MessagePlayingActorPageSynchronize session_synchronize;
  session::protocol::PlayingPageField session_page;
  session_synchronize.__set_actor_(this->GetID());

  gateway::protocol::PlayingPageField playing_field;
  for (PageMap::const_iterator iterator = this->pages_.begin();
      iterator != this->pages_.end(); ++iterator) {
    const PlayingPage *page = iterator->second;
    if (page == NULL) {
      LOG_ERROR("玩家(%lu) PlayingPage 列表中存在空指针", this->GetID());
      return false;
    }
    playing_field.__set_template_(page->GetTemplate());
    playing_field.__set_record_(page->GetRecord());
    playing_field.__set_result_(page->GetResult());
    message.playings_.push_back(playing_field);

    session_page.__set_template_(page->GetTemplate());
    session_page.__set_record_(page->GetRecord());
    session_page.__set_result_(page->GetResult());
    session_synchronize.pages_.push_back(session_page);
  }

  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_PLAYING_INITIALIZE);

  SessionChannel::SendMessage(session_synchronize,
      session::protocol::GameMessageType::MESSAGE_PLAYING_ACTOR_PAGE_SYNCHRONIZE);

  // 地宫初始化
  if(this->maze_actor_.Initialize(this, field) == false) {
    LOG_ERROR("Init MazeActor(%lu) failed.", this->GetID());
    return false;
  }

  return true;
}

void PlayingActor::Finalize() {
  this->maze_actor_.Finalize();

  PageMap::iterator iterator = this->pages_.begin();
  for(; iterator != this->pages_.end(); ++iterator) {
    if (iterator->second) {
      iterator->second->Finalize();
      PlayingPagePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->pages_.clear();

  this->actor_ = NULL;
  this->playing_ = 0;
}

void PlayingActor::Save() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return;
  }

  std::string record;
  std::string result;

  database::protocol::StoragePlayingActorSaveRequest request;
  request.__set_actor_id_(this->actor_->GetID());

  request.field_.__set_auto_playing_(this->auto_playing_);
  request.field_.__set_auto_playing_group_(this->auto_playing_group_);
  request.field_.__set_auto_finish_time_(this->auto_finish_time_);

  // TODO: 保存格式化字段
  PageMap::iterator iterator = this->pages_.begin();
  for(; iterator != this->pages_.end(); ++iterator) {
    PlayingPage *page = iterator->second;
    if (page == NULL) {
      LOG_ERROR("保存 玩家(%lu) PlayingPage 失败", this->actor_->GetID());
      return;
    }
    if (page->Save(record, result) == false) {
      LOG_ERROR("玩家(%lu) 保存副本(%d) 失败", this->GetID(), page->GetTemplate());
      return;
    }
    request.field_.records_ += record;
    request.field_.records_ += ",";
    request.field_.results_ += result;
    request.field_.results_ += ",";
  }

  // 地宫保存
  this->maze_actor_.Save(request.field_);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_PLAYING_ACTOR_SAVE,
        this->actor_->GetID()) == false) {
    LOG_ERROR("保存 玩家(%lu) 副本信息失败", this->actor_->GetID());
    return;
  }
}

PlayingPage *PlayingActor::GetPage(core::int32 template_id) {
  PageMap::iterator iterator = this->pages_.find(template_id);
  if (iterator != this->pages_.end()) {
    return iterator->second;
  }
  return NULL;
}

const PlayingPage *PlayingActor::GetPage(core::int32 template_id) const {
  PageMap::const_iterator iterator = this->pages_.find(template_id);
  if (iterator != this->pages_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool PlayingActor::AddPage(PlayingPage *page) {
  if (page == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  PageMap::const_iterator iterator = this->pages_.find(page->GetTemplate());
  if (iterator != this->pages_.end()) {
    return false;
  }

  this->pages_.insert(std::make_pair(page->GetTemplate(), page));

  return true;
}

PlayingPage *PlayingActor::RemovePage(core::int32 template_id) {
  PlayingPage *page = NULL;
  PageMap::iterator iterator = this->pages_.find(template_id);
  if (iterator != this->pages_.end()) {
    page = iterator->second;
    this->pages_.erase(iterator);
  }
  return page;
}

void PlayingActor::PeriodClean(entity::PlayingPeriodType::type period_type) {
  for (PageMap::const_iterator iterator = this->pages_.begin();
      iterator != this->pages_.end(); ++iterator) {
    PlayingPage *page = iterator->second;
    if (page == NULL) {
      LOG_ERROR("玩家(%lu) PlayingPage 表中有空指针", this->GetID());
      return;
    }
    const global::configure::PlayingCell *cell =
      global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(page->GetTemplate());
    if (cell == NULL) {
      LOG_ERROR("获取 PlayingCell(%u) 失败", page->GetTemplate());
      continue;
    }
    if (cell->period_type_ != period_type) {
      continue;
    }

    entity::PlayingRecordField &record = page->GetRecord();
    entity::PlayingResultField &result = page->GetResult();
    record.__set_period_count_(0);
    result.__set_awarded_(true);
    result.__set_paid_award_count_(0);
    page->Synchronize();
  }
}

void PlayingActor::SynchronizePage(core::int32 template_id) const {
  const PlayingPage *page = this->GetPage(template_id);
  if (page == NULL) {
    LOG_WARNING("玩家(%lu) 找不到 PlayingPage(%d)", this->GetID(), template_id);
    return;
  }
  page->Synchronize();
}

bool PlayingActor::CheckCreatePlaying(core::int32 template_id) const {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  if (FacadeRequest::GetInstance()->CheckTransport(this->GetID()) == true) {
    LOG_ERROR("玩家(%lu) 当前处于护送状态");
    return false;
  }

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(template_id);
  if (cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", template_id);
    return false;
  }

  if(this->GetPlaying() != 0) {
    LOG_ERROR("玩家 playingActor(%lu) 还处于副本中", this->GetID());
    return false;
  }

  if (this->auto_playing_ != 0 || this->auto_playing_group_ != 0) {
    LOG_WARNING("当前正在自动副本中(%u,%u)",
        this->auto_playing_, this->auto_playing_group_);
    return false;
  }

  core::int32 vip_period_count = 0;
  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)this->actor_->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, template_id);
  if (vip_cell) {
    vip_period_count = vip_cell->value2_;
  }

  const PlayingPage *page = this->GetPage(template_id);
  if (page != NULL) {
    const entity::PlayingRecordField &record = page->GetRecord();
    if (record.period_count_ >= cell->period_count_ + vip_period_count) {
      LOG_WARNING("玩家(%lu) 副本(%d) 次数(%d) 超过上限(%d) vip次数(%d)",
          this->GetID(), template_id, record.period_count_, cell->period_count_,
          vip_period_count);
      return false;
    }
    if(cell->depend_ != 0) {
      const PlayingPage *depend_page = this->GetPage(cell->depend_);
      if (depend_page == NULL || depend_page->GetRecord().spend_secs_ <= 0) {
        LOG_WARNING("玩家(%lu) 副本(%d) 的前置副本(%d) 没有通关过",
            this->GetID(), template_id, cell->depend_);
        return false;
      }
    }
  }

  return true;
}

bool PlayingActor::StartAutoPlaying(core::int32 template_id) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor 为空");
    return false;
  }

  if(this->GetPlaying() != 0) {
    LOG_ERROR("玩家 playingActor(%lu) 还处于副本中", this->GetID());
    return false;
  }

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(template_id);
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 失败", template_id);
    return false;
  }

  PlayingPage *page = this->GetPage(template_id);
  if (page == NULL) {
    LOG_WARNING("获取 PlayingPage(%lu) 失败", template_id);
    return false;
  }

  entity::PlayingRecordField &record = page->GetRecord();
  entity::PlayingResultField &result = page->GetResult();
  if (result.awarded_ == false) {
    LOG_WARNING("玩家(%lu) 有副本(%d) 奖励没有领取", this->GetID(), template_id);
    return false;
  }

  if (record.spend_secs_ <= 0) {
    LOG_WARNING("玩家(%lu) 副本(%d) 未通过，不能自动进行", this->GetID(), template_id);
    return false;
  }

  core::int32 vip_period_count = 0;
  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)this->actor_->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, template_id);
  if (vip_cell) {
    vip_period_count = vip_cell->value2_;
  }

  if (record.period_count_ >= cell->period_count_ + vip_period_count) {
    LOG_WARNING("玩家(%lu) 副本(%d) 完成次数(%d) 已满(%d)",
        this->GetID(), template_id, record.period_count_, cell->period_count_,
        vip_period_count);
    return false;
  }

  if (cell->auto_type_ == entity::PlayingAutoType::IMMEDIATE) {
    // 直接拿奖励
    for (core::int32 i = record.period_count_; i <= cell->period_count_ + vip_period_count; ++i) {
      event::EventPlayingActorRequestAward event;
      event.__set_actor_(this->GetID());
      event.__set_template_(template_id);
      event.__set_result_(page->GetResult());
      event.__set_record_(page->GetRecord());
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_PLAYING_ACTOR_REQUEST_AWARD, this->GetID(),
          event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
    }
    record.__set_period_count_(cell->period_count_ + vip_period_count);
    page->Synchronize();
  } else if (cell->auto_type_ == entity::PlayingAutoType::RECORD) {
    if (this->auto_playing_ != 0 || this->auto_playing_group_ != 0) {
      LOG_WARNING("有正在自动进行的副本(%d,%d)",
          this->auto_playing_, this->auto_playing_group_);
      return false;
    }
    this->auto_playing_ = template_id;
    this->auto_finish_time_ =
      GlobalTimeTick::GetInstance()->TimeTick().GetRealSec() +
      record.spend_secs_ * (cell->period_count_ + vip_period_count - record.period_count_);
  } else {
    return false;
  }

  return true;
}

bool PlayingActor::StartAutoPlayingGroup(core::int32 group) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor 为空");
    return false;
  }

  if(this->GetPlaying() != 0) {
    LOG_ERROR("玩家 playingActor(%lu) 还处于副本中", this->GetID());
    return false;
  }

  if (this->auto_playing_ != 0 || this->auto_playing_group_ != 0) {
    LOG_WARNING("有正在自动进行的副本(%d,%d)",
        this->auto_playing_, this->auto_playing_group_);
    return false;
  }

  core::int32 need_secs = 0;

  for (PageMap::const_iterator iterator = this->pages_.begin();
      iterator != this->pages_.end(); ++iterator) {
    PlayingPage *page = iterator->second;
    if (page == NULL) {
      LOG_ERROR("玩家(%lu) PlayingPage 表中有空指针", this->GetID());
      return false;
    }
    const global::configure::PlayingCell *cell =
      global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(page->GetTemplate());
    if (cell == NULL) {
      LOG_ERROR("获取 PlayingCell(%d) 失败", page->GetTemplate());
      return false;
    }
    if (cell->group_ != group) {
      continue;
    }
    const entity::PlayingRecordField &record = page->GetRecord();
    const entity::PlayingResultField &result = page->GetResult();
    if (result.awarded_ == false) {
      LOG_WARNING("玩家(%lu) 无法自动进行组副本(%d) 副本(%d)奖励未领取",
          group, page->GetTemplate());
      return false;
    }
    if (record.spend_secs_ <= 0) {
      continue;
    }

    core::int32 vip_period_count = 0;
    const global::configure::VipCell *vip_cell =
      VIP_CONF()->GetCell((entity::VipType::type)this->actor_->GetAttribute(entity::ActorAoiFields::VIP),
          entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, page->GetTemplate());
    if (vip_cell) {
      vip_period_count = vip_cell->value2_;
    }

    core::int32 remain_period_count = 0;
    if (vip_period_count + cell->period_count_ > record.period_count_) {
      remain_period_count = vip_period_count + cell->period_count_ - record.period_count_;
    }

    need_secs += record.spend_secs_ * remain_period_count;
  }

  core::int32 freedom_dollars =
    this->actor_->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);

  this->auto_playing_group_ = group;
  this->auto_finish_time_ =
    GlobalTimeTick::GetInstance()->TimeTick().GetRealSec() + need_secs;

  return true;
}

bool PlayingActor::FinishAutoPlaying(core::int32 template_id) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor 为空");
    return false;
  }

  if (this->auto_playing_ != template_id) {
    LOG_ERROR("参数(%d) 错误", template_id);
    return false;
  }

  PlayingPage *page = this->GetPage(template_id);
  if (page == NULL) {
    LOG_WARNING("获取 玩家(%lu) PlayingPage(%d) 失败", this->GetID(), template_id);
    return false;
  }

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(page->GetTemplate());
  if (cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 失败", page->GetTemplate());
    return false;
  }

  entity::PlayingRecordField &record = page->GetRecord();

  core::int32 vip_period_count = 0;
  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)this->actor_->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, template_id);
  if (vip_cell) {
    vip_period_count = vip_cell->value2_;
  }

  if (cell->period_count_ + vip_period_count <= record.period_count_) {
    LOG_ERROR("玩家(%lu) PlayingRecordField(%d) 周期次数(%d) vip次数(%d) 错误",
      this->GetID(), template_id, record.period_count_, vip_period_count);
    return false;
  }

  core::int32 finish_count = cell->period_count_ + vip_period_count - record.period_count_;

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if (this->auto_finish_time_ > now) {
    core::int32 freedom_dollars = this->actor_->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
    core::int32 need_freedom_dollars = finish_count * cell->auto_spend_freedom_dollars_;
    if (need_freedom_dollars > 0 && freedom_dollars < need_freedom_dollars) {
      LOG_ERROR("玩家(%lu) 快速完成失败，钻石(%d) 不足(%d)",
          this->GetID(), freedom_dollars, need_freedom_dollars);
      return false;
    }
    this->actor_->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - need_freedom_dollars);
  }

  // 完成自动副本
  if (page->FinishAutoPage(finish_count)) {
    // TODO: 这样在发事件前清空是有危险的，如果事件发送中出错，可能没有记录到结果
    this->auto_playing_ = 0;
    this->auto_finish_time_ = 0;
  }

  return true;
}

bool PlayingActor::FinishAutoPlayingGroup(core::int32 group) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor 为空");
    return false;
  }

  if (this->auto_playing_group_ != group) {
    LOG_ERROR("参数(%d) 错误)", group);
    return false;
  }

  bool spend_finish = false;

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if (this->auto_finish_time_ > now) {
    spend_finish = true;
  }

  for (PageMap::iterator iterator = this->pages_.begin();
      iterator != this->pages_.end(); ++iterator) {
    PlayingPage *page = iterator->second;
    if (page == NULL) {
      LOG_ERROR("玩家(%lu) PlayingPage 列表中有空指针", this->GetID());
      return false;
    }
    const global::configure::PlayingCell *cell =
      global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(page->GetTemplate());
    if (cell == NULL) {
      LOG_ERROR("获取 PlayingCell(%d) 失败", page->GetTemplate());
      return false;
    }
    if (cell->group_ != group) {
      continue;
    }
    entity::PlayingRecordField &record = page->GetRecord();
    const entity::PlayingResultField &result = page->GetResult();
    if (result.awarded_ == false) {
      LOG_WARNING("玩家(%lu) 无法自动进行组副本(%d) 副本(%d)奖励未领取",
          group, page->GetTemplate());
      return false;
    }
    core::int32 vip_period_count = 0;
    const global::configure::VipCell *vip_cell =
      VIP_CONF()->GetCell((entity::VipType::type)this->actor_->GetAttribute(entity::ActorAoiFields::VIP),
          entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, page->GetTemplate());
    if (vip_cell) {
      vip_period_count = vip_cell->value2_;
    }
    if (cell->period_count_ + vip_period_count <= record.period_count_) {
      continue;
    }
    if (record.spend_secs_ <= 0) {
      continue;
    }
    core::int32 finish_count = cell->period_count_ + vip_period_count - record.period_count_;
    // 是否要快速完成
    if (spend_finish == true) {
      core::int32 freedom_dollars = this->actor_->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
      core::int32 need_freedom_dollars = finish_count * cell->auto_spend_freedom_dollars_;
      if (need_freedom_dollars > 0 && freedom_dollars < need_freedom_dollars) {
        LOG_ERROR("玩家(%lu) 快速完成失败，钻石(%d) 不足(%d)",
            this->GetID(), freedom_dollars, need_freedom_dollars);
        return false;
      }
      this->actor_->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - need_freedom_dollars);
    }
    // 完成自动副本
    if (page->FinishAutoPage(finish_count) == false) {
      LOG_ERROR("玩家(%lu) PlayingPage(%d) 完成自动副本失败",
          this->GetID(), page->GetTemplate());
      return false;
    }
  }

  this->auto_finish_time_ = 0;
  this->auto_playing_group_ = 0;

  return true;
}

bool PlayingActor::AwardPlaying(core::int32 template_id) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  PlayingPage *page = this->GetPage(template_id);
  if (page == NULL) {
    LOG_WARNING("获取 玩家(%lu) PlayingPage(%d) 失败",
        this->GetID(), template_id);
    return false;
  }

  const entity::PlayingRecordField &record = page->GetRecord();
  const entity::PlayingResultField &result = page->GetResult();

  if (result.awarded_ != false && record.first_pass_awarded_ != false) {
    LOG_WARNING("玩家(%lu) 重复领取奖励", this->GetID());
    return false;
  }

  event::EventPlayingActorRequestAward event;
  event.__set_actor_(this->GetID());
  event.__set_template_(template_id);
  event.__set_result_(result);
  event.__set_record_(record);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_ACTOR_REQUEST_AWARD, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

  return true;
}

bool PlayingActor::AwardPlayingGroup(core::int32 group) {
  PageMap::iterator iterator = this->pages_.begin();
  for (; iterator != this->pages_.end(); ++iterator) {
    PlayingPage *page = iterator->second;
    if (page == NULL) {
      LOG_ERROR("玩家(%lu) PlayingPage 列表中有空指针", this->GetID());
      return false;
    }

    const global::configure::PlayingCell *cell =
      global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(page->GetTemplate());
    if (cell == NULL) {
      LOG_ERROR("获取 PlayingCell(%d) 失败", page->GetTemplate());
      return false;
    }
    if (cell->group_ != group) {
      continue;
    }

    const entity::PlayingRecordField &record = page->GetRecord();
    const entity::PlayingResultField &result = page->GetResult();

    if (result.awarded_ == true) {
      continue;
    }

    event::EventPlayingActorRequestAward event;
    event.__set_actor_(this->GetID());
    event.__set_template_(page->GetTemplate());
    event.__set_result_(result);
    event.__set_record_(record);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_PLAYING_ACTOR_REQUEST_AWARD, this->GetID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }

  return true;
}

}  // namespace playing

}  // namespace server

}  // namespace game

