//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-22 03:13:32.
// File name: rune_message_handler.cc
//
// Description:
//
#include "game_server/server/extension/attr/rune_message_handler.h"

#include <boost/bind.hpp>

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace attr {

RuneMessageHandler::RuneMessageHandler() {}
RuneMessageHandler::~RuneMessageHandler() {}

bool RuneMessageHandler::Initialize() {
  // MESSAGE_ATTR_RUNE_COMPOSE_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_COMPOSE_REQUEST,
      boost::bind(&RuneMessageHandler::OnMessageRuneCompose, this, _1, _2, _3));

  // MESSAGE_ATTR_RUNE_MOVE_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_MOVE_REQUEST,
      boost::bind(&RuneMessageHandler::OnMessageRuneMove, this, _1, _2, _3));
  return true;
}

void RuneMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_COMPOSE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_MOVE_REQUEST);
}

void RuneMessageHandler::OnMessageRuneMove(core::uint64 id, 
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  RuneActor *rune_actor = attr_actor->GetRuneActor();

  // 消息解析
  gateway::protocol::MessageRuneMoveRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageRuneMoveRequest falied.");
    return;
  }

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    LOG_WARNING("SPIRIT_MODULE is disable.");
    return;
  }

  // 检查移动
  if(message.src_type_ == message.dest_type_) {
    if(message.src_type_ == gateway::protocol::RuneMoveType::PACKET) {
      LOG_ERROR("不能移动[%d]", message.src_type_);
      return ;
    }
    core::uint32 dest_template = rune_actor->GetSlotTemplateID(message.dest_location_);
    core::uint32 src_template = rune_actor->GetSlotTemplateID(message.src_location_);
    if( src_template == 0) {
      LOG_ERROR("参数错误");
      return ;
    }
    if(dest_template == 0) {
      // 移动
      rune_actor->SetRune(message.src_location_, 0, false);
      if(rune_actor->CheckPlaceSlot(src_template, message.dest_location_) == false) {
        LOG_ERROR("检测不能移动");
        rune_actor->SetRune(message.src_location_, src_template, false);
        return ;
      }
      rune_actor->SlotDataSynchronize(message.src_location_);
      rune_actor->SetRune(message.dest_location_, src_template, true);
      rune_actor->SlotDataSynchronize(message.dest_location_);
    } else {
      // 替换
      rune_actor->SetRune(message.src_location_, 0, false);
      rune_actor->SetRune(message.dest_location_, 0, false);
      if(rune_actor->CheckPlaceSlot(src_template, message.dest_location_) == false) {
        LOG_ERROR("检测不能移动");
        rune_actor->SetRune(message.src_location_, src_template, false);
        rune_actor->SetRune(message.dest_location_, dest_template, false);
        return ;
      }
      if(rune_actor->CheckPlaceSlot(dest_template, message.src_location_) == false) {
        LOG_ERROR("检测不能移动");
        rune_actor->SetRune(message.src_location_, src_template, false);
        rune_actor->SetRune(message.dest_location_, dest_template, false);
        return ;
      }
      rune_actor->SetRune(message.src_location_, dest_template, false);
      rune_actor->SlotDataSynchronize(message.src_location_);
      rune_actor->SetRune(message.dest_location_, src_template, true);
      rune_actor->SlotDataSynchronize(message.dest_location_);
    }
    gateway::protocol::MessageRuneMoveResponse response;
    response.__set_result_(true);
    game_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_MOVE_RESPONSE);

    return ;
  }

  if(gateway::protocol::RuneMoveType::PACKET == message.src_type_) {
    // 获取位置上模板id
    request::RequestGetPacketItemTemplate request;
    request.__set_actor_id_(id);
    request.__set_location_(message.src_location_);
    if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GET_PAKCET_ITEM_TEMPLATE,
          &request, sizeof(request)) == -1) {
      LOG_ERROR("指定位置上的道具没有找到");
      return ;
    }
    const RuneCell *cell = Configure::GetInstance()->GetRuneCell(request.item_template_);
    if(cell == NULL) {
      LOG_ERROR("此道具[%d]不是符文", request.item_template_);
      return ;
    }
    if(rune_actor->GetSlotTemplateID(message.dest_location_) != 0) {
      LOG_ERROR("装备符文失败，指定位置存在符文");
      return ;
    } else {
      // 删除道具
      request::RequestRemoveItemLocationID request_remove;
      request_remove.__set_actor_id_(id);
      request_remove.__set_location_(message.src_location_);
      request_remove.__set_number_(1);
      if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_REMOVE_ITEM_LOCATION_ID,
            &request_remove, sizeof(request_remove)) == -1 ||
          request_remove.result_ != request::RemoveResult::SUCCESS) {
        LOG_ERROR("删除道具失败[%d]", request_remove.result_);
        return ;
      }
    }
    //替换上符文
    rune_actor->SetRune(message.dest_location_, request.item_template_, true);
    rune_actor->SlotDataSynchronize(message.dest_location_);
  } else {
    if(rune_actor->GetSlotTemplateID(message.src_location_) != 0) {
      // 脱下:如果背包中指定位置有其他物品，则不能脱下
      request::RequestAddItem request_add;
      request_add.__set_actor_id_(id);
      request_add.__set_item_template_(rune_actor->GetSlotTemplateID(message.src_location_));
      request_add.__set_number_(1);
      request_add.__set_bind_(entity::BindRequireType::BIND);
      request_add.__set_simulate_(false);
      request_add.__set_location_(message.dest_location_);
      if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
            &request_add, sizeof(request_add)) == -1 || request_add.result_
          != request::AddItemResult::SUCCESS) {
        LOG_ERROR("添加道具失败[%d]", request_add.result_);
        return ;
      }
      //卸下符文
      rune_actor->SetRune(message.src_location_, 0, true);
      rune_actor->SlotDataSynchronize(message.src_location_);
    } else {
      LOG_ERROR("指定位置不存在符文");
      return ;
    }
  }
  gateway::protocol::MessageRuneMoveResponse response;
  response.__set_result_(true);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_MOVE_RESPONSE);
}

void RuneMessageHandler::OnMessageRuneCompose(core::uint64 id, 
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  RuneActor *rune_actor = attr_actor->GetRuneActor();

  // 消息解析
  gateway::protocol::MessageRuneComposeRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageRuneComposeRequest falied.");
    return;
  }

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::SPIRIT_MODULE) == false) {
    LOG_WARNING("SPIRIT_MODULE is disable.");
    return;
  }
  // 获取位置上模板id
  request::RequestGetPacketItemTemplate request;
  request.__set_actor_id_(id);
  request.__set_item_id_(atol(message.item_id_.c_str()));
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GET_PAKCET_ITEM_TEMPLATE,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("指定ID【%lu】上的道具没有找到", atol(message.item_id_.c_str()));
    return ;
  }

  // 符文合成
  const RuneCell *cell = Configure::GetInstance()->GetRuneCell(request.item_template_);
  if(cell == NULL) {
    LOG_ERROR("合成符文失败，此道具[%d]不能合成", request.item_template_);
    return ;
  }
  if(message.num_%cell->num_ != 0) {
    LOG_ERROR("输入参数有误[%d]", message.num_);
    return ;
  }

  core::int32 success_num = 0;
  core::int32 faild_num = 0;
  for(core::int32 i = 0; i < message.num_/cell->num_; ++i) {
    if(ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom)
        > (core::uint32)cell->odds_) {
      faild_num += 1 ;
    } else {
      success_num += 1;
    }
  }
  if(success_num != 0) {
    // 尝试添加符文
    request::RequestAddItem request_add;
    request_add.__set_actor_id_(id);
    request_add.__set_item_template_(cell->fuse_template_);
    request_add.__set_number_(success_num);
    request_add.__set_bind_(request.bind_ == true ?
        entity::BindRequireType::BIND : entity::BindRequireType::NOT_BIND);
    request_add.__set_simulate_(true);
    if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
          &request_add, sizeof(request_add)) == -1 || request_add.result_
        != request::AddItemResult::SUCCESS) {
      LOG_DEBUG("添加道具失败[%d]", request_add.result_);
      return ;
    }
  }
  // 删除道具
  request::RequestRemoveItemLocationID request_remove;
  request_remove.__set_actor_id_(id);
  request_remove.__set_item_id_(message.item_id_.c_str());
  request_remove.__set_number_(message.num_);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_REMOVE_ITEM_LOCATION_ID,
        &request_remove, sizeof(request_remove)) == -1 ||
      request_remove.result_ != request::RemoveResult::SUCCESS) {
    LOG_ERROR("删除道具失败[%d]", request_remove.result_);
  }
  // 添加符文
  if(success_num != 0) {
    request::RequestAddItem add;
    add.__set_actor_id_(id);
    add.__set_item_template_(cell->fuse_template_);
    add.__set_number_(success_num);
    add.__set_bind_(request.bind_ == true ?
        entity::BindRequireType::BIND : entity::BindRequireType::NOT_BIND);
    add.__set_simulate_(false);
    if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
          &add, sizeof(add)) == -1 || add.result_
        != request::AddItemResult::SUCCESS) {
      LOG_ERROR("添加道具失败[%d]", add.result_);
    }
  }

  gateway::protocol::MessageRuneComposeResponse response;
  response.__set_template_id_(cell->fuse_template_);
  response.__set_success_num_(success_num);
  response.__set_fail_num_(faild_num);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_RUNE_COMPOSE_RESPONSE);

}

} // attr

} // server

} // game
