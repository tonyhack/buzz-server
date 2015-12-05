//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 10:39:20.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/soul/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_attr_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_skill_request_types.h"

namespace game {

namespace server {

namespace soul {

bool FacadeRequest::CheckPacketItemExist(core::uint64 actor,
    core::uint32 item_template, core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestGetPacketItemNumber request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER, &request,
        sizeof(request)) == 0 && (core::uint32)request.number_ >= number) {
    return true;
  }
  return false;
}

core::uint32 FacadeRequest::RemovePacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestRemoveItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_REMOVE_ITEM, &request,
        sizeof(request)) == 0) {
    return request.remove_number_;
  }
  return 0;
}

bool FacadeRequest::CheckAddPacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind);
  request.__set_simulate_(true);
  if(ExtensionManager::GetInstance()->Request(
         request::RequestType::REQUEST_ADD_ITEM, &request,
         sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::AddPacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(
         request::RequestType::REQUEST_ADD_ITEM, &request,
         sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::UpgradeActorSkill(core::uint64 actor, core::uint32 skill_id,
    core::int32 upgrade) {
  if(skill_id == 0) {
    return true;
  }
  if(upgrade <= 0) {
    return false;
  }
  request::RequestUpgradeSkill request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(actor);
  request.__set_skill_id_(skill_id);
  request.__set_upgrade_(upgrade);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_UPGRADE_SKILL, &request,
        sizeof(request)) == 0 && request.result_ == request::SkillResultType::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::RemoveActorSkill(core::uint64 actor, core::uint32 skill_id) {
  if(skill_id == 0) {
    return true;
  }
  request::RequestRemoveSkill request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(actor);
  request.__set_skill_id_(skill_id);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_REMOVE_SKILL, &request, sizeof(request)) == 0 &&
      request.result_ == request::SkillResultType::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::AddActorSkill(core::uint64 actor, core::uint32 skill_id,
    core::uint32 level, bool stand) {
  if(skill_id == 0) {
    return true;
  }
  request::RequestAddSkill request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(actor);
  request.__set_skill_id_(skill_id);
  request.__set_level_(level);
  request.__set_stand_(stand);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ADD_SKILL, &request, sizeof(request)) == 0 &&
      request.result_ == request::SkillResultType::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::StandActorSkill(core::uint64 actor, core::uint32 skill_id) {
  if(skill_id == 0) {
    return true;
  }
  request::RequestSkillStand request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(actor);
  request.__set_skill_id_(skill_id);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SKILL_STAND,
        &request, sizeof(request)) == 0 && request.result_ == 
        request::SkillResultType::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::SitActorSkill(core::uint64 actor, core::uint32 skill_id) {
  if(skill_id == 0) {
    return true;
  }
  request::RequestSkillSit request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(actor);
  request.__set_skill_id_(skill_id);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SKILL_SIT,
        &request, sizeof(request)) == 0 && request.result_ == 
        request::SkillResultType::SUCCESS) {
    return true;
  }
  return false;
}

core::int32 FacadeRequest::GetSwordStep(core::uint64 actor) {
  request::RequestAttrGetSwordStep request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ATTR_GET_SWORD_STEP, &request,
        sizeof(request)) != 0) {
    return -1;
  }
  return request.step_;
}

core::int32 FacadeRequest::GetSpiritLevel(core::uint64 actor) {
  request::RequestAttrGetSpiritLevel request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ATTR_GET_SPIRIT_LEVEL, &request,
        sizeof(request)) != 0) {
    return -1;
  }
  return request.level_;
}

core::int32 FacadeRequest::GetNobleLevel(core::uint64 actor) {
  request::RequestAttrGetNobleLevel request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ATTR_GET_NOBLE_LEVEL, &request,
        sizeof(request)) != 0) {
    return -1;
  }
  return request.level_;
}

bool FacadeRequest::AddBuff(core::uint64 actor, core::uint32 buff) {
  request::RequestBuffStart request;
  request.__set_src_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_src_entity_id_(actor);
  request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_dest_entity_id_(actor);
  request.__set_buff_id_(buff);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_START, &request,
        sizeof(request)) == 0 && request.succeed_ == true) {
    return true;
  }
  return false;
}

bool FacadeRequest::RemoveBuff(core::uint64 actor, core::uint32 buff) {
  request::RequestBuffEnd request;
  request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_dest_entity_id_(actor);
  request.__set_buff_id_(buff);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_END, &request,
        sizeof(request)) == 0 && request.succeed_ == true) {
    return true;
  }
  return false;
}

}  // namespace soul

}  // namespace server

}  // namespace game

