#include "game_server/server/extension/attr/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_skill_request_types.h"

namespace game {

namespace server {

namespace attr {

bool FacadeRequest::CheckPacketItemExist(core::uint64 actor,
    core::uint32 item_template, core::uint32 number, BindType::type bind) {
  request::RequestGetPacketItemNumber request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_bind_((entity::BindRequireType::type)bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER, &request,
        sizeof(request)) == 0 && (core::uint32)request.number_ >= number) {
    return true;
  }
  return false;
}

core::uint32 FacadeRequest::RemovePacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, BindType::type bind) {
  request::RequestRemoveItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_((entity::BindRequireType::type)bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_REMOVE_ITEM, &request,
        sizeof(request)) == 0) {
    return request.remove_number_;
  }
  return 0;
}

bool FacadeRequest::AddSkillGlobalDebuff(core::uint64 actor,
    const SwordCell::BuffVector &buffs) {
  if(buffs.empty() == false) {
    request::RequestAddSkillGlobalBuff request;
    request.__set_type_(entity::EntityType::TYPE_ACTOR);
    request.__set_id_(actor);
    request::SkillBuff buff;
    SwordCell::BuffVector::const_iterator iterator = buffs.begin();
    for(; iterator != buffs.end(); ++iterator) {
      buff.__set_buff_(iterator->buff_);
      buff.__set_odds_(iterator->buff_odds_);
      buff.__set_cooling_(iterator->buff_cooling_);
      buff.__set_target_type_(iterator->target_type_);
      request.buffs_.push_back(buff);
    }
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_ADD_SKILL_GLOBAL_BUFF,
          &request, sizeof(request)) != 0) {
      return false;
    }
  }
  return true;
}

bool FacadeRequest::RemoveSkillGlobalDebuff(core::uint64 actor,
    const SwordCell::BuffVector &buffs) {
  if(buffs.empty() == false) {
    request::RequestRemoveSkillGlobalBuff request;
    request.__set_type_(entity::EntityType::TYPE_ACTOR);
    request.__set_id_(actor);
    request::SkillBuff buff;
    SwordCell::BuffVector::const_iterator iterator = buffs.begin();
    for(; iterator != buffs.end(); ++iterator) {
      buff.__set_buff_(iterator->buff_);
      buff.__set_odds_(iterator->buff_odds_);
      buff.__set_cooling_(iterator->buff_cooling_);
      request.buffs_.push_back(buff);
    }
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_REMOVE_SKILL_GLOBAL_BUFF,
          &request, sizeof(request)) != 0) {
      return false;
    }
  }
  return true;
}

bool FacadeRequest::AddBuff(core::uint64 actor, core::uint32 buff) {
  request::RequestBuffStart request;
  request.__set_src_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_src_entity_id_(actor);
  request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_dest_entity_id_(actor);
  request.__set_buff_id_(buff);
  if(ExtensionManager::GetInstance()->Request(
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
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_END, &request,
        sizeof(request)) == 0 && request.succeed_ == true) {
    return true;
  }
  return false;
}

}  // namespace attr

}  // namespace server

}  // namespace game
