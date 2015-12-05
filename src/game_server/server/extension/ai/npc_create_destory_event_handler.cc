//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 18:36:22.
// File name: npc_create_destory_event_handler.cc
//
// Description:
// Define class NpcCreateDestoryEventHandler.
//

#include "game_server/server/extension/ai/npc_create_destory_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/extension/ai/black_board_manager.h"
#include "game_server/server/extension/ai/black_board_pool.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace ai {

NpcCreateDestoryEventHandler::NpcCreateDestoryEventHandler() {}
NpcCreateDestoryEventHandler::~NpcCreateDestoryEventHandler() {}

bool NpcCreateDestoryEventHandler::Initialize() {
  // Follow 所有 npc 创建事件
  this->connection_create_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_CREATE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(
        &NpcCreateDestoryEventHandler::OnCreateEvent, this, _1, _2, _3, _4));
  this->connection_destory_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_DESTROY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(
        &NpcCreateDestoryEventHandler::OnDestoryEvent, this, _1, _2, _3, _4));

  return true;
}

void NpcCreateDestoryEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_create_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_destory_);
}

void NpcCreateDestoryEventHandler::OnCreateEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcCreate) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcCreate *event = (event::EventNpcCreate *)message;

  // 从 NPC 管理器中获取 NPC 对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(event->id_);
  if(npc == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 获取 GameNpc 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // BlackBoard 是否已存在
  BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(event->id_);
  if(black_board) {
    global::LogError("%s:%d (%s) Npc(%lu) BlackBoard 已存在",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 分配一个 BlackBoard
  black_board = BlackBoardPool::GetInstance()->Allocate();
  if(black_board == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) BlackBoard 分配失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 初始化 BlackBoard 对象
  if(black_board->Initialize(npc) == false) {
    black_board->Finalize();
    BlackBoardPool::GetInstance()->Deallocate(black_board);
    global::LogError("%s:%d (%s) Npc(%lu) BlackBoard 初始化失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 加入 BlackBoard 管理器
  if(BlackBoardManager::GetInstance()->AddNpc(black_board) == false) {
    black_board->Finalize();
    BlackBoardPool::GetInstance()->Deallocate(black_board);
    global::LogError("%s:%d (%s) Npc(%lu) BlackBoard 加入管理器失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // Python 扩展设置为加载成功
  npc->GetLoadProgress().LoadExtensionFinish(ExtensionType::AI);
}

void NpcCreateDestoryEventHandler::OnDestoryEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcDestory) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcDestory *event = (event::EventNpcDestory *)message;

  // 从管理器中删除 BlackBoard
  BlackBoard *black_board = BlackBoardManager::GetInstance()->RemoveNpc(event->id_);
  if(black_board == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 管理器删除 BlackBoard 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 放回对象池
  black_board->Finalize();
  BlackBoardPool::GetInstance()->Deallocate(black_board);
}

}  // namespace ai

}  // namespace server

}  // namespace game

