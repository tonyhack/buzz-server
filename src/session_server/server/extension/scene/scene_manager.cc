//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 17:29:21.
// File name: scene_manager.cc
//
// Description:
// Define class SceneManager.
//

#include "session_server/server/extension/scene/scene_manager.h"

#include "entity/inform_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/game_misc.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "global/configure/load_world_boss_configure.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/scene/scene.h"
#include "session_server/server/extension/scene/scene_pool.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_server.h"

namespace session {

namespace server {

namespace scene {

bool SceneManager::Initialize() {
  this->StartExpireTimer(SUMMON_BOSS);
  return true;
}

void SceneManager::Finalize() {
  SceneHashmap::iterator iterator = this->scenes_.begin();
  for(; iterator != this->scenes_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      ScenePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  SessionServerSingleton::GetInstance().RemoveTimer(SUMMON_BOSS, this);
  this->scenes_.clear();
}

bool SceneManager::AddScene(Scene *scene) {
  if(scene == NULL) {
    return false;
  }

  core::uint64 id = scene->GetID();

  SceneHashmap::iterator iterator = this->scenes_.find(id);
  if(iterator != this->scenes_.end()) {
    return false;
  }

  if (scene->CheckDynamic() == false &&
      this->AddBranchScene(scene->GetMapID(), scene->GetID()) == false) {
    return false;
  }

  this->scenes_.insert(std::make_pair(id, scene));

  return true;
}

Scene *SceneManager::RemoveScene(core::uint64 id) {
  Scene *scene = NULL;

  SceneHashmap::iterator iterator = this->scenes_.find(id);
  if(iterator != this->scenes_.end()) {
    scene = iterator->second;
    this->scenes_.erase(iterator);
    if (scene->CheckDynamic() == false) {
      this->RemoveBranchScene(scene->GetMapID(), scene->GetID());
    }
  }

  return scene;
}

Scene *SceneManager::GetScene(core::uint64 id) {
  SceneHashmap::iterator iterator = this->scenes_.find(id);
  if(iterator == this->scenes_.end()) {
    return NULL;
  }
  return iterator->second;
}

Scene *SceneManager::GetBranchScene(core::uint16 map, core::uint16 branch) {
  BranchSceneMap::iterator iterator = this->branches_.find(map);
  if (iterator == this->branches_.end()) {
    return NULL;
  }

  BranchSceneSet::iterator iterator2 = iterator->second.begin();
  for (; iterator2 != iterator->second.end(); ++iterator2) {
    if (global::GameMisc::GetSceneSequenceID(*iterator2) == branch) {
      return this->GetScene(*iterator2);
    }
  }

  return NULL;
}

void SceneManager::TryBranchScene(core::uint16 map) {
  BranchSceneMap::iterator iterator = this->branches_.find(map);
  if (iterator == this->branches_.end()) {
    return;
  }

  if (iterator->second.size() > (core::uint32)MISC_CONF()->map_branch_max_number_) {
    return;
  }

  BranchSceneSet::iterator iterator2 = iterator->second.begin();
  for (; iterator2 != iterator->second.end(); ++iterator2) {
  Scene *scene = this->GetScene(*iterator2);
    if (scene == NULL) {
      LOG_ERROR("获取 Scene(%lu) 失败", *iterator2);
      return;
    }
    if (scene->GetActorSize() < (core::uint32)MISC_CONF()->map_branch_max_actor_number_) {
      return;
    }
  }

  session::protocol::MessageSceneBranchMapSynchronize message;
  message.__set_map_(map);
  message.__set_branch_(iterator->second.size());
  SessionTerminal *terminal = SessionServerSingleton::GetInstance().AllocateGameServer();
  if (terminal == NULL) {
    LOG_ERROR("分配 Gameserver 失败");
    return;
  }
  terminal->SendGameMessage(message,
      session::protocol::GameMessageType::MESSAGE_SCENE_BRANCH_MAP_SYNCHRONIZE);

  LOG_DEBUG("MessageSceneBranchMapSynchronize map=%d, branch=%d", message.map_, message.branch_);
}

bool SceneManager::AddBranchScene(core::uint16 map, core::uint64 scene) {
  BranchSceneMap::iterator iterator = this->branches_.find(map);
  if (iterator != this->branches_.end()) {
    if (iterator->second.find(scene) != iterator->second.end()) {
      return false;
    }
    iterator->second.insert(scene);
  } else {
    BranchSceneSet branch_set;
    branch_set.insert(scene);
    this->branches_.insert(std::make_pair(map, branch_set));
  }

  return true;
}

void SceneManager::RemoveBranchScene(core::uint16 map, core::uint64 scene) {
  BranchSceneMap::iterator iterator = this->branches_.find(map);
  if (iterator != this->branches_.end()) {
    iterator->second.erase(scene);
  }
}

Scene *SceneManager::AllocateBranchScene(core::uint16 map) {
  BranchSceneMap::iterator iterator = this->branches_.find(map);
  if (iterator == this->branches_.end()) {
    return NULL;
  }

  Scene *ret_scene = NULL;

  BranchSceneSet::iterator iterator2 = iterator->second.begin();
  for (; iterator2 != iterator->second.end(); ++iterator2) {
    Scene *scene = this->GetScene(*iterator2);
    if (scene == NULL) {
      LOG_ERROR("获取 Scene(%lu) 失败", *iterator2);
    } else {
      if (scene->GetActorSize() < (core::uint32)MISC_CONF()->map_branch_max_actor_number_) {
        ret_scene = scene;
        break;
      } else if (ret_scene == NULL || scene->GetActorSize() < ret_scene->GetActorSize()) {
        ret_scene = scene;
      }
    }
  }

  return ret_scene;
}

const SceneManager::BranchSceneSet *SceneManager::GetBranches(core::uint16 map) {
  BranchSceneMap::const_iterator iterator = this->branches_.find(map);
  if (iterator == this->branches_.end()) {
    return NULL;
  }

  return &iterator->second;
}

void SceneManager::RemoveScenes(core::uint32 game_server) {
  SceneHashmap::iterator iterator = this->scenes_.begin();
  for(; iterator != this->scenes_.end();) {
    Scene *scene = iterator->second;
    if(scene == NULL) {
      continue;
    }
    if(scene->GetGameServer() != game_server) {
      ++iterator;
      continue;
    }
    this->scenes_.erase(iterator++);
    // 回收对象
    scene->Finalize();
    ScenePool::GetInstance()->Deallocate(scene);
  }
}

void SceneManager::StartExpireTimer(core::uint32 type) {
  if(type == SUMMON_BOSS) {
    time_t zero_time = GlobalTimeTick::GetInstance()->GetZeroTime();
    time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    if(now_time < zero_time) {
      LOG_ERROR("时间错误[%lu],[%lu]", now_time, zero_time);
      return ;
    }

    LOG_INFO("召唤世界BOSS计时开始");

    // 获取召唤世界boss时间
    const global::configure::MiscConfigure::WorldBossTimeVecs &times = MISC_CONF()->worldboss_appear_time_vecs_;
    if(times.size() == 0) {
      LOG_INFO("无世界boss可召唤");
      return ;
    }

    core::int32 pass_time_of_day = now_time - zero_time;
    for(size_t i = 0; i < times.size(); ++i) {
      if(times[i] > pass_time_of_day) {
        // 安装定时器
        LOG_INFO("安装召唤世界BOSS定时器[%lu]", times[i] - pass_time_of_day);
        SessionServerSingleton::GetInstance().AddTimer(SUMMON_BOSS, 
            (times[i] - pass_time_of_day)*1000, -1, this,
            "SceneManager::StartExpireTimer");
        return ;
      }
    }

    // 当前的时间已经是召唤BOSS的最后时间
    time_t set_time = zero_time + 24*3600 - now_time + times[0];
    if(set_time > 0) {
      LOG_INFO("安装召唤世界BOSS定时器[%lu]", set_time);
      SessionServerSingleton::GetInstance().AddTimer(SUMMON_BOSS,
          set_time*1000, -1, this, "SceneManager::StartExpireTimer");
    } else {
      LOG_INFO("世界boss召唤时间计算失败");
    }
  }
}

void SceneManager::OnTimer(core::uint32 id) {
  if(id == SUMMON_BOSS) {
    LOG_INFO("召唤世界BOSS出现");
    // 召唤BOSS
    for(SceneHashmap::iterator itr = this->scenes_.begin(); itr != this->scenes_.end(); ++itr) {
      // 获取场景ID
      core::uint16 map_id = itr->second->GetMapID();
      const global::configure::WorldBossCell *rand_cell = 
        WORLD_BOSS_CONF()->GetCell((core::int32)map_id);
      if(rand_cell == NULL) {
        LOG_INFO("场景(%d)不存在世界boss", map_id);
        continue;
      }
      core::uint32 rand = ExtensionManager::GetInstance()->GetRandom().Random(4);
      if(rand_cell == NULL) {
        LOG_ERROR("随机配置出错");
        return ;
      }
      itr->second->GetWorldBoss().BroadCastSummonBoss(rand_cell->template_id_, rand_cell->coordinates_[rand].x_, 
          rand_cell->coordinates_[rand].y_, rand_cell->coordinates_[rand].width_, rand_cell->coordinates_[rand].heigth_);
    }

    // 世界boss公告
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::SCENE_BOSS_SUMMON);
    SessionServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

    SessionServerSingleton::GetInstance().RemoveTimer(SUMMON_BOSS, this);
    this->StartExpireTimer(SUMMON_BOSS);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace session

