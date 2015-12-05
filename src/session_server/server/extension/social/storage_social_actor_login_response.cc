#include "session_server/server/extension/social/storage_social_actor_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_social_actor_login_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"
#include "session_server/server/extension/social/social_actor_pool.h"

namespace session {

namespace server {

namespace social {

StorageSocialActorLoginResponse::StorageSocialActorLoginResponse() {}
StorageSocialActorLoginResponse::~StorageSocialActorLoginResponse() {}

bool StorageSocialActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_SOCIAL_ACTOR_LOGIN,
      boost::bind(&StorageSocialActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageSocialActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_SOCIAL_ACTOR_LOGIN);
}

void StorageSocialActorLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageSocialActorLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize StorageSocialActorLoginRequest failed.");
    return;
  }
  database::protocol::StorageSocialActorLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize StorageSocialActorLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      response.result_ != database::protocol::SocialActorLoginResult::SUCCESS) {
    LOG_ERROR("Call StorageSocialActorLoginRequest failed");
    return;
  }

  // 是否是离线加载
  bool offline_load = (request.__isset.offline_load_ && request.offline_load_ == true);

  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(request.actor_id_);
  if (NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", request.actor_id_);
    return;
  }
  SessionActor *session_actor = NULL;
  SocialActor *social_actor = NULL;

  if (!offline_load) {
    // 从管理器中获取玩家对象
    session_actor = SessionActorManager::GetInstance()->GetActor(request.actor_id_);
    if (NULL == session_actor) {
      LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", request.actor_id_);
      return;
    }
    // 社交扩展是否已存在
    social_actor = SocialActorManager::GetInstance()->Get(request.actor_id_);
    if (social_actor != NULL) {
      LOG_ERROR("SocialActor(%lu) already exists.", request.actor_id_);
      return;
    }
  } else {
    // 离线加载检查内存中是否已存在, 若存在直接回调离线任务
    social_actor = SocialActorManager::GetInstance()->GetFromAll(request.actor_id_);
    if (social_actor != NULL) {
      // 回调离线任务 
      if (request.__isset.offline_task_id_) {
        SocialActorManager::OfflineTask task =
          SocialActorManager::GetInstance()->RemoveOfflineTask(request.offline_task_id_);
        if (task) {
          task();
        }
      }
      return;
    }
  }

  // 分配社交玩家对象
  social_actor = SocialActorPool::GetInstance()->Allocate();
  if (NULL == social_actor) {
    LOG_ERROR("Allocate SocialActor(%lu) from SocialActorPool failed.", request.actor_id_);
    return;
  }

  // 初始化
  if (social_actor->Initialize(brief_actor, response.field_) == false) {
    SocialActorPool::GetInstance()->Deallocate(social_actor);
    LOG_ERROR("Init SocialActor(%lu) failed.", request.actor_id_);
    return;
  }

  if (offline_load) {
    // 加入离线列表
    if (SocialActorManager::GetInstance()->AddToCache(social_actor) == false) {
      social_actor->Finalize();
      SocialActorPool::GetInstance()->Deallocate(social_actor);
      LOG_ERROR("Add SocialActor(%lu) to SocialActorManager failed.", request.actor_id_); 
      return;
    }
  } else {
    // 加入在线列表
    if (SocialActorManager::GetInstance()->Add(social_actor) == false) {
      social_actor->Finalize();
      SocialActorPool::GetInstance()->Deallocate(social_actor);
      LOG_ERROR("Add SocialActor(%lu) to SocialActorManager failed.", request.actor_id_); 
      return;
    }
  }

  // 每日清理
  time_t zero_time = GlobalTimeTick::GetInstance()->GetZeroTime();
  if (zero_time >= social_actor->GetLastSaveTime()) {
    social_actor->DailyClean();
    if (offline_load) {
      social_actor->Save();
    }
  }

  if (offline_load) {
    // 回调离线任务
    if (request.__isset.offline_task_id_) {
      SocialActorManager::OfflineTask task =
        SocialActorManager::GetInstance()->RemoveOfflineTask(request.offline_task_id_);
      if (task) {
        task();
      }
    }
  } else {
    // 上线
    social_actor->Online(session_actor);
    // 发送加载完成事件
    social_actor->SendLoadFinishEvent();
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

