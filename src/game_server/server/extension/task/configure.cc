//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 16:14:10.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/task/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace task {

const core::uint32 Configure::kTalkDistance_ = 5;

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s) 出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 condition_pool 结点
  TiXmlElement *condition_pool_node = configure_node->FirstChildElement("condition_pool");
  if(condition_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-condition_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(condition_pool_node->Attribute("initial_number",
        (int *)(&this->condition_initial_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [condition_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(condition_pool_node->Attribute("extend_number",
        (int *)(&this->condition_extend_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [condition_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 task_pool 结点
  TiXmlElement *task_pool_node = configure_node->FirstChildElement("task_pool");
  if(task_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-task_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(task_pool_node->Attribute("initial_number",
        (int *)(&this->task_initial_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [task_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(task_pool_node->Attribute("extend_number",
        (int *)(&this->task_extend_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [task_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  const char *str = NULL;
  // 加载任务配置
  TiXmlElement *task_node = configure_node->FirstChildElement("task");
  if(task_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-task]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = task_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[task-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_task_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 task 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  
  // 加载杀怪收集配置
  TiXmlElement *kill_collect_node = configure_node->FirstChildElement("kill_collect");
  if(kill_collect_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-kill_collect]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = kill_collect_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[kill_collect-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_kill_collect_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 kill_collect 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  
  // 加载循环任务配置
  TiXmlElement *circle_condition_node = configure_node->FirstChildElement("circle_condition");
  if(circle_condition_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-circle_condition]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = circle_condition_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[circle_condition-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_circle_condition_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 circle_condition 配置失败");
    return false;
  }

  // 加载循环任务主配置
  TiXmlElement *circle_node = configure_node->FirstChildElement("circle");
  if(circle_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure_circle]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = circle_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[circle-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_circle_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 circle 配置失败");
    return false;
  }

  // 加载历险任务主配置
  TiXmlElement *trip_node = configure_node->FirstChildElement("trip");
  if(trip_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [trip_circle]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = trip_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[trip-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_trip_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 circle 配置失败");
    return false;
  }

  // 加载纹章配置
  TiXmlElement *layer_node = configure_node->FirstChildElement("layer");
  if(layer_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [layer]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = layer_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[layer-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_layer_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 layer 配置失败");
    return false;
  }

  // 加载召唤NPC配置
  TiXmlElement *summon_npc_node = configure_node->FirstChildElement("summon_npc");
  if(summon_npc_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [summon_npc]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = summon_npc_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[summon_npc-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_summon_npc_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 summon_npc 配置失败");
    return false;
  }

  // 加载每日任务配置
  TiXmlElement *daily_node = configure_node->FirstChildElement("task_daily");
  if(daily_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [task_daily]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = daily_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[task_daily-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_daily_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 task_daily 配置失败");
    return false;
  }

  // 加载副本任务条件配置
  TiXmlElement *task_playing = configure_node->FirstChildElement("task_playing");
  if(task_playing == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [task_playing]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = task_playing->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[task_playing-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_playing_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 task_playing 配置失败");
    return false;
  }

  // 加载召唤NPC配置
  TiXmlElement *expliot_award_node = configure_node->FirstChildElement("exploit_award");
  if(expliot_award_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [exploit_award]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = expliot_award_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[exploit_award-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_exploit_award_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 exploit_award 配置失败");
    return false;
  }

  // 加载召唤NPC配置
  TiXmlElement *exploit_attr_node = configure_node->FirstChildElement("exploit_attr");
  if(exploit_attr_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [exploit_attr]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = exploit_attr_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[exploit_attr-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->exploit_attr_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false ) {
    LOG_ERROR("加载 exploit_attr 配置失败");
    return false;
  }

  // 加载运送配置
  TiXmlElement *task_transport_node = configure_node->FirstChildElement("task_transport");
  if (task_transport_node == NULL) {
    LOG_ERROR("找不到结点 [task_transport], 文件(%s)", file.c_str());
    return false;
  }

  str = task_transport_node->Attribute("file");
  if (str == NULL) {
    LOG_ERROR("找不到属性[task_transport-file], 文件(%s)", file.c_str());
    return false;
  }

  if (this->transport_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    LOG_ERROR("加载 transport 配置失败");
    return false;
  }

  return true;
}

const TaskCell *Configure::GetTaskConfigure(core::uint32 id) {
  return this->load_task_.GetTask(id);
}

const KillCollectCell *Configure::GetKillCollect(const core::uint32 key) {
  return this->load_kill_collect_.GetKillCollect(key);
}

const TaskCircleCell *Configure::GetCircleCell(core::int32 lv_stage, core::int32 circle_num) {
  TaskCircleKey key;
  key.circle_num_ = circle_num;
  key.id_ = lv_stage;
  return this->load_circle_.GetTaskCircleCell(key);
}

const ConditionCell *Configure::GetConditionCell(core::int32 id) const {
  return this->load_circle_condition_.GetCondition(id);
}

const ConditionCell *Configure::RandConditionCell(core::int32 lv_stage) const {
  return this->load_circle_condition_.RandCondition(lv_stage);
}

const LoadTripConfigure::RandConditionVec *Configure::GetRandCondition(core::int32 stage, 
    core::int32 type) const {
  return this->load_trip_.GetRandCondition(stage, type);
}

const TripTaskCell *Configure::GetTripConditionCell(core::int32 id) const {
  return this->load_trip_.GetCondition(id);
}

const TaskLayerCell *Configure::GetLayerCell(core::uint32 task_id) const {
  return this->load_layer_.GetCell(task_id);
}

const SummonNpcCell *Configure::GetSummonNpcCell(core::uint32 npc_template) const {
  return this->load_summon_npc_.GetCell((core::int32)npc_template);
}

bool Configure::CheckPlayingCondition(core::int32 id, core::int32 map_id) const {
  return this->load_playing_.Check(id, map_id);
}

const TaskDailyCell *Configure::GetTaskDailyCell(core::int32 id) const {
  return this->load_daily_.GetCell(id);
}

const ExploitAwardCell *Configure::GetTaskDailyAwardCell(core::int32 id) const {
  return this->load_exploit_award_.GetCell(id);
}

}  // namespace task

}  // namespace server

}  // namespace game

