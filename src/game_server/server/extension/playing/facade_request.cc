//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 10:53:14.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/playing/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_transport_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace playing {

core::uint64 FacadeRequest::CreateSceneMap(core::uint32 map) {
  request::RequestSceneCreateMap request;
  request.__set_map_(map);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_CREATE_MAP,
      &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_SCENE_CREATE_MAP(%u) 失败", map);
    return 0;
  }
  return request.scene_;
}

bool FacadeRequest::DestorySceneMap(core::uint64 scene) {
  request::RequestSceneDestoryMap request;
  request.__set_scene_(scene);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_DESTORY_MAP,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_SCENE_DESTORY_MAP(%lu) 失败", scene);
    return false;
  }

  return true;
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

bool FacadeRequest::CheckTransport(core::uint64 actor) {
  request::RequestTransportCurrentNpc request;
  request.__set_actor_(actor);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TRANSPORT_CURRENT_NPC, &request,
        sizeof(request)) == -1) {
    LOG_ERROR("请求 RequestTransportCurrentNpc 失败");
    return false;
  }

  return request.transport_npc_ != 0;
}

}  // namespace playing

}  // namespace server

}  // namespace game

