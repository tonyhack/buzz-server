#include "game_server/server/extension/mail/mail_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/mail/configure.h"
#include "game_server/server/extension/mail/mail_actor_pool.h"
#include "game_server/server/extension/mail/mail_actor_manager.h"
#include "game_server/server/extension/mail/mail_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace game {

namespace server {

namespace mail {

MailExtension::MailExtension() {}
MailExtension::~MailExtension() {}

bool MailExtension::Initialize() {
  if(Configure::GetInstance() == NULL) {
    LOG_ERROR("初始化邮件配置文件失败");
    return false;
  }
  if(MailActorManager::GetInstance() == NULL) {
    LOG_ERROR("分配 MailActorManager 失败");
    return false;
  }
  if(MailPool::GetInstance() == NULL) {
    LOG_ERROR("分配 MailPool 失败");
    return false;
  }
  if(MailActorPool::GetInstance() == NULL) {
    LOG_ERROR("分配 MailActorPool 失败");
    return false;
  }

  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if(information == NULL) {
    global::LogError("%s:%d (%s) 获取当前服务器配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载配置文件
  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetMailFile()) == false) {
    global::LogError("%s:%d (%s) 加载 mail 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(MailPool::GetInstance()->Initialize(information->user_pool_initial_ * 5,
        information->user_pool_extend_) == false) {
    LOG_ERROR("初始化 MailPool 失败");
    return false;
  }

  if(MailActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("初始化邮件玩家对象池失败");
  }

  if(response_.Initialize() == false) {
    LOG_ERROR("初始化 MailDatabaseResponse 失败");
    return false;
  }
  if(event_.Initialize() == false) {
    LOG_ERROR("初始化 MailEventHandler 失败");
    return false;
  }
  if(message_.Initialize() == false) {
    LOG_ERROR("初始化 MailMessageHandler 失败");
    return false;
  }
  if(session_message_.Initialize() == false) {
    LOG_ERROR("初始化 SessionMessageHandler 失败");
    return false;
  }
  if(request_.Initialize() == false) {
    LOG_ERROR("初始化 RequestMailHandler 失败");
    return false;
  }

  return true;
}

void MailExtension::Finalize() {
  response_.Finalize();
  event_.Finalize();
  message_.Finalize();
  session_message_.Finalize();
  request_.Finalize();

  MailActorPool::GetInstance()->Finalize();
  MailPool::GetInstance()->Finalize();

  MailActorPool::ReleaseInstance();
  MailPool::ReleaseInstance();
  MailActorManager::ReleaseInstance();
  Configure::ReleaseInstance();
}

void MailExtension::Start() {
   
}

void MailExtension::Stop() {

}

} // namespace mail

} // namespace server 

} // namespace game
