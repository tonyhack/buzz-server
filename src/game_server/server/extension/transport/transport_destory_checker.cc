//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-10 01:23:57.
// File name: transport_destory_checker.cc
//
// Description:
// Define class TransportDestoryChecker.
//

#include "game_server/server/extension/transport/transport_destory_checker.h"

#include "game_server/server/game_server.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/event/game_transport_event_types.h"
#include "game_server/server/extension/transport/facade_request.h"
#include "game_server/server/extension/transport/transport_actor.h"
#include "game_server/server/extension/transport/transport_actor_manager.h"
#include "game_server/server/extension/transport/transport_npc.h"
#include "game_server/server/extension/transport/transport_npc_manager.h"
#include "game_server/server/extension/transport/transport_npc_pool.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace transport {

bool TransportDestoryChecker::Initialize() {
  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_CHECK,
      MISC_CONF()->transport_destory_check_sec_ * 1000, -1, this,
      "TransportDestoryChecker::Initialize");

  return true;
}

void TransportDestoryChecker::Finalize() {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_CHECK, this);
}

void TransportDestoryChecker::Add(core::uint64 transport_npc) {
  if (this->transports_.find(transport_npc) == this->transports_.end()) {
    this->transports_.insert(std::make_pair(transport_npc,
          GlobalTimeTick::GetInstance()->TimeTick().GetRealSec()));
  }
}

void TransportDestoryChecker::Remove(core::uint64 transport_npc) {
  this->transports_.erase(transport_npc);
}

void TransportDestoryChecker::OnTimer(core::uint32 id) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  TransportMap::iterator iterator = this->transports_.begin();
  for (; iterator != this->transports_.end();) {
    if (now > iterator->second &&
        now - iterator->second > MISC_CONF()->transport_destory_sec_) {
      core::uint64 transport_npc_id = iterator->first;

      TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(transport_npc_id);
      if (transport_npc == NULL) {
        LOG_ERROR("获取 TransportNpc(%lu) 失败", transport_npc_id);
        return;
      }

      if (FacadeRequest::GetInstance()->RemoveNpc(transport_npc_id) == false) {
        LOG_ERROR("请求删除NPC失败");
        return;
      }

      core::uint64 transport_actor_id = transport_npc->GetMasterActor();
      TransportActor *transport_actor =
        TransportActorManager::GetInstance()->Get(transport_actor_id);
      if (transport_actor != NULL) {
        // 护送失败，发事件
        event::EventTransportFailure event;
        event.__set_actor_(transport_actor->GetID());
        coresh::CommunicatorSingleton::GetInstance().Broadcast(
            event::EventType::EVENT_TRANSPORT_FAILURE, transport_actor->GetID(),
            event::ChannelType::CHANNEL_ACTOR, &event,
            sizeof(event::EventTransportFailure));
        transport_actor->SetTransportNpc(0);
      }

      TransportActorManager::GetInstance()->RemoveTransport(transport_actor_id);
      transport_npc->Finalize();
      TransportNpcManager::GetInstance()->Remove(transport_npc_id);
      TransportNpcPool::GetInstance()->Deallocate(transport_npc);

      this->transports_.erase(iterator++);

      LOG_DEBUG("TransportActor(%lu) 运送NPC被移除", transport_actor_id);
    } else {
      ++iterator;
    }
  }
}

}  // namespace transport

}  // namespace server

}  // namespace game

