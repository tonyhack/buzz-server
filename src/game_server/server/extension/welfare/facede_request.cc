#include "game_server/server/extension/welfare/facede_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/request/game_attr_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

FacedeRequest::FacedeRequest() {}
FacedeRequest::~FacedeRequest() {}

core::uint32 FacedeRequest::GetNpcTemplateID(core::uint64 id) {
  GameNpc *npc = GameNpcManager::GetInstance()->Get(id);
  if(npc == NULL) {
    LOG_ERROR("NPC (%lu)没有找到", id);
    return 0;
  }
  return npc->GetTemplateID();
}


bool FacedeRequest::GetBuffExist(core::uint64 id, core::uint32 buff_id) {
  request::RequestBuffGetExist request;
  request.__set_id_(id);
  request.__set_buff_id_(buff_id);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_BUFF_GET_EXIST,
      &request, sizeof(request)) == -1) {
    LOG_ERROR("请求失败(%d)", buff_id);
    return false;
  }

  return request.exist_;
}


core::int32 FacedeRequest::RequestActorEquipNum(core::uint64 id, entity::AchieveWearEquipType::type type,
    core::uint32 num) {
  request::RequestItemWearedEquipInfo request;
  request.__set_actor_id_(id);
  request.__set_type_(type);
  request.__set_param1_((core::int32)num);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ITEM_WEARED_EQUIP_INFO,
        &request, sizeof(request)) == -1) {
    return 0;
  }

  return request.num_;
}

bool FacedeRequest::AddItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, bool bind) {
  if(actor == 0 || item_template == 0 || number == 0) {
    return false;
  }
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind ? entity::BindRequireType::BIND : entity::BindRequireType::NOT_BIND);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ADD_ITEM, &request,
        sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacedeRequest::Awards(core::uint64 actor, const Award *awards, core::int32 num) {
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(actor);
  if(game_actor == NULL) {
    LOG_ERROR("玩家没有找到[%lu]", actor);
    return false;
  }
  request::RequestAddItems request;
  entity::AddItemFields field;
  for(core::int32 i = 0; i < num; ++i) {
    if(awards[i].id_ == 0) {
      continue ;
    }
    if(awards[i].type_ == entity::ResourceType::ITEM) {
      field.__set_bind_(awards[i].bind_ == 0 ? false : true);
      field.__set_item_template_(awards[i].id_);
      field.__set_number_(awards[i].number_);
      request.items_.push_back(field);
    }
  }
  request.__set_actor_id_(actor);
  request.__set_simulate_(false);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ADD_ITEMS, &request,
        sizeof(request)) != 0 || request.result_ != request::AddItemResult::SUCCESS) {
    LOG_ERROR("加入道具失败");
    return false;
  }
  for(core::int32 i = 0; i < num; ++i) {
    if(awards[i].type_ == entity::ResourceType::RESOURCE) {
      game_actor->AddResource((entity::ResourceID::type)awards[i].id_, awards[i].number_); 
    }
  }

  return true;
}

core::int32 FacedeRequest::GetSpiritFacadeType(core::uint64 actor) {
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(actor);
  if(game_actor == NULL) {
    LOG_ERROR("玩家没有找到[%lu]", actor);
    return false;
  }
  request::RequestAttrGetSpiritFacadeType request;
  request.__set_actor_(actor);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_ATTR_GET_SPIRIT_FACADE_TYPE, &request,
        sizeof(request)) != 0 ) {
    LOG_ERROR("请求精灵外观类型失败");
    return false;
  }
  return request.type_;
}

}  // namespace welfare

}  // namespace server

}  // namespace game
