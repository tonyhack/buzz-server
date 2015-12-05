
#include "game_server/server/extension/welfare/welfare_actor.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_achieve_save_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace welfare {

WelfareActor::WelfareActor() {}

WelfareActor::~WelfareActor() {}

bool WelfareActor::Initialize(StorageAchieveFields &achieves, 
    StorageAchieveFinishField &field) {
  if(this->actor_ == NULL) {
    LOG_ERROR("福利加载时，游戏玩家对象没有找到");
    return false;
  }
  this->achieve_.Initialize(achieves, field);

  // 初始化福利系统
  gateway::protocol::MessageWelfareInitilize message;
  message.__set_title_id_(this->achieve_.GetTitleID());
  message.__set_service_time_(global::ToString(global::ServerConfigureSingleton::GetInstance().GetOpenServiceTime()));

  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_WELFARE_INITILIZE);

  // 同步签到数据
  this->checkins_.Synchronize();
  return true;
}

bool WelfareActor::InitializeActor(GameActor *actor, DatabaseWelfareField &field) {
  if(actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }
  this->actor_ = actor;
  this->achieve_.InitializeActor(actor, field);
  this->checkins_.InitializeActor(actor, field);
  return true;
}

void WelfareActor::Finalize() {
  this->achieve_.Finalize();
  this->checkins_.Finalize();
}

void WelfareActor::Save() {
  database::protocol::StorageWelfareSave actor_save;
  actor_save.__set_actor_(this->GetActorID());
  this->achieve_.SaveActor(actor_save.field_);
  this->checkins_.SaveActor(actor_save.field_);

  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(actor_save,
        database::protocol::StorageType::STORAGE_WELFARE_SAVE, this->GetActorID()) == false) {
    LOG_ERROR("保存成就列表失败");
  }

  this->achieve_.Save();
}

void WelfareActor::OnEvent(event::EventType::type type, const void *data, size_t size) {
  this->achieve_.OnEvent(type, data, size);
}

void WelfareActor::DailyClean(core::int32 days) {
  this->checkins_.DailyClean(days);
}

}  // namespace welfare

}  // namespace server

}  // namespace game
