//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-12 02:13:07.
// File name: load_task_transport_configure.cc
//
// Description:
// Define class LoadTaskTransportConfigure.
//

#include "game_server/server/extension/task/load_task_transport_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace task {

LoadTaskTransportConfigure::LoadTaskTransportConfigure() {}

LoadTaskTransportConfigure::~LoadTaskTransportConfigure() {}

bool LoadTaskTransportConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s) 失败", file.c_str());
    return false;
  }

  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (data_node == NULL) {
    LOG_ERROR("找不到结点 [data]，文件(%s)", file.c_str());
    return false;
  }

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    TaskTransportCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("找不到属性 [cell-id], 文件(%s)", file.c_str());
      return false;
    }

    if (cell_node->Attribute("npc_id", (int *)&cell.npc_id_) == false) {
      LOG_ERROR("找不到属性 [cell-npc_id], 文件(%s)", file.c_str());
      return false;
    }

    if (cell_node->Attribute("odds", (int *)&cell.odds_) == false) {
      LOG_ERROR("找不到属性 [cell-odds], 文件(%s)", file.c_str());
      return false;
    }

    if (cell_node->Attribute("force_success_spend",
          (int *)&cell.force_success_spend_) == false) {
      LOG_ERROR("找不到属性 [cell-force_success_spend], 文件(%s)", file.c_str());
      return false;
    }

    std::vector<core::int32> awards_vector;
    std::vector<std::string> awards_str_vector;

    const char *success_awards_str = cell_node->Attribute("success_awards");
    if (success_awards_str == NULL) {
      LOG_ERROR("找不到属性 [cell-success_awards], 文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(success_awards_str, "+", awards_str_vector);
    for (size_t pos = 0; pos < awards_str_vector.size(); ++pos) {
      global::StringSplit(awards_str_vector[pos].c_str(), ":", awards_vector);
      if (awards_vector.size() != 4) {
        LOG_ERROR("transport(%d) success_awards 错误", cell.id_);
        return false;
      }
      TaskAward award;
      award.type_ = awards_vector[0];
      award.id_ = awards_vector[1];
      award.number_ = awards_vector[2];
      award.bind_ = awards_vector[3];
      cell.success_awards_.push_back(award);
    }

    const char *failure_awards_str = cell_node->Attribute("failure_awards");
    if (failure_awards_str == NULL) {
      LOG_ERROR("找不到属性 [cell-failure_awards], 文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(failure_awards_str, "+", awards_str_vector);
    for (size_t pos = 0; pos < awards_str_vector.size(); ++pos) {
      global::StringSplit(awards_str_vector[pos].c_str(), ":", awards_vector);
      if (awards_vector.size() != 4) {
        LOG_ERROR("transport(%d) failure_awards 错误", cell.id_);
        return false;
      }
      TaskAward award;
      award.type_ = awards_vector[0];
      award.id_ = awards_vector[1];
      award.number_ = awards_vector[2];
      award.bind_ = awards_vector[3];
      cell.failure_awards_.push_back(award);
    }

    const char *rob_awards_str = cell_node->Attribute("rob_awards");
    if (rob_awards_str == NULL) {
      LOG_ERROR("找不到属性 [cell-rob_awards], 文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(rob_awards_str, "+", awards_str_vector);
    for (size_t pos = 0; pos < awards_str_vector.size(); ++pos) {
      global::StringSplit(awards_str_vector[pos].c_str(), ":", awards_vector);
      if (awards_vector.size() != 4) {
        LOG_ERROR("transport(%d) rob_awards 错误", cell.id_);
        return false;
      }
      TaskAward award;
      award.type_ = awards_vector[0];
      award.id_ = awards_vector[1];
      award.number_ = awards_vector[2];
      award.bind_ = awards_vector[3];
      cell.rob_awards_.push_back(award);
    }

    this->transports_.push_back(cell);

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const TaskTransportCell *LoadTaskTransportConfigure::GetTransport(core::int32 id) const {
  TransportVector::const_iterator iterator = this->transports_.begin();
  for (; iterator != this->transports_.end(); ++iterator) {
    if (iterator->id_ == id) {
      return &(*iterator);
    }
  }

  return NULL;
}

const TaskTransportCell *LoadTaskTransportConfigure::Random() const {
  const TaskTransportCell *cell = NULL;
  core::int32 odds =
    ExtensionManager::GetInstance()->GetRandom().Random() % global::kPercentDenom;
  TransportVector::const_iterator iterator = this->transports_.begin();
  for (; iterator != this->transports_.end(); ++iterator) {
    cell = &(*iterator);
    if (iterator->odds_ >= odds) {
      break;
    }
    odds -= iterator->odds_;
  }

  return cell;
}

}  // namespace task

}  // namespace server

}  // namespace game

