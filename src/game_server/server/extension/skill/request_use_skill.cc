//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 15:37:32.
// File name: request_use_skill.cc
//
// Description:
// Define class RequestUseSkill.
//

#include "game_server/server/extension/skill/request_use_skill.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

RequestUseSkill::RequestUseSkill() {}
RequestUseSkill::~RequestUseSkill() {}

bool RequestUseSkill::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_USE_SKILL,
      boost::bind(&RequestUseSkill::OnRequest, this, _1, _2));
  return true;
}

void RequestUseSkill::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_USE_SKILL);
}

int RequestUseSkill::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestUseSkill) != size) {
    global::LogError("%s:%d (%s) 请求使用技能时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestUseSkill *request = (request::RequestUseSkill *)message;

  // 获取 SkillRole 对象
  SkillRole *role = NULL;
  switch(request->type_) {
    case entity::EntityType::TYPE_ACTOR:
      role = SkillRoleManager::GetInstance()->GetActor(request->id_);
      break;
    case entity::EntityType::TYPE_NPC:
      role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
      break;
    default:
      break;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求使用技能时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  // 获取当前技能状态
  SkillStatus *status = role->GetCurrentStatus();
  if(status == NULL) {
    global::LogError("%s:%d (%s) 请求使用技能时，Role(%d, %lu) 状态错误",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  // 执行
  request->__set_result_(status->OnExecute(request::RequestType::REQUEST_USE_SKILL,
        message, size));

  return 0;
}

}  // namespace skill

}  // namespace server

}  // namespace game


