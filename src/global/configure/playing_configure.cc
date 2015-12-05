//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-17 10:24:39.
// File name: playing_configure.cc
//
// Description:
// Define class PlayingConfigure.
//

#include "global/configure/playing_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "entity/playing_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace global {

namespace configure {

PlayingConfigure::PlayingConfigure() {}
PlayingConfigure::~PlayingConfigure() {}

bool PlayingConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load Configure file(%s) failed.", file.c_str());
    return false;
  }

  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    LOG_ERROR("Config file(%s) /configure not found.", file.c_str());
    return false;
  }

  TiXmlElement *playing_pool_node = configure_node->FirstChildElement("playing_pool");
  if(playing_pool_node == NULL) {
    LOG_ERROR("Config file(%s) /configure/playing_pool not found.", file.c_str());
    return false;
  }

  if(playing_pool_node->Attribute("initial_number",
        (int *)&this->playing_pool_initial_) == false) {
    LOG_ERROR("Config file(%s) /configure/playing_pool/initial_number not found.",
        file.c_str());
    return false;
  }

  if(playing_pool_node->Attribute("extend_number",
        (int *)&this->playing_pool_extend_) == false) {
    LOG_ERROR("Config file(%s) /configure/playing_pool/extend_number not found.",
        file.c_str());
    return false;
  }

  TiXmlElement *playing_node = configure_node->FirstChildElement("playing");
  if(playing_node == NULL) {
    LOG_ERROR("Config file(%s) /configure/playing not found.", file.c_str());
    return false;
  }

  const char *playing_file = playing_node->Attribute("file");
  if(playing_file == NULL) {
    LOG_ERROR("Config file(%s) /configure/playing/file not found.", file.c_str());
    return false;
  }

  this->playing_file_ =
    ServerConfigureSingleton::GetInstance().GetConfigueDir() + playing_file;
  if(this->LoadPlayingConfigure(this->playing_file_) == false) {
    LOG_ERROR("Load playing configire failed.");
    return false;
  }

  return true;
}

bool PlayingConfigure::LoadPlayingConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load Configure file(%s) failed.", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("Config file(%s) /data not found.", file.c_str());
    return false;
  }

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    int type = 0;
    int camp_type = 0;
    PlayingCell cell;
    if(cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file(%s) /data/id not found.", file.c_str());
      return false;
    }
    if(this->playings_.find(cell.id_) != this->playings_.end()) {
      LOG_ERROR("Config file(%s) /data/id(%d) repeated found.",
          file.c_str(), cell.id_);
      return false;
    }

    if (cell_node->Attribute("group", (int *)&cell.group_) == false) {
      LOG_ERROR("Config file(%s) /data/group not found.", file.c_str());
      return false;
    }

    if (cell_node->Attribute("depend", (int *)&cell.depend_) == false) {
      LOG_ERROR("Config file(%s) /data/depend not found.", file.c_str());
      return false;
    }

    if(cell_node->Attribute("camp_type", &camp_type) == false) {
      LOG_ERROR("Config file(%s) /data/camp_type not found.", file.c_str());
      return false;
    }
    if(camp_type <= entity::PlayingCampType::MIN ||
        type >= entity::PlayingCampType::MAX) {
      LOG_ERROR("Config file(%s) /data/camp_type(%d) invalid.",
          file.c_str(), camp_type);
      return false;
    }
    cell.camp_type_ = (entity::PlayingCampType::type)camp_type;
    if(cell_node->Attribute("camp_number", (int *)&cell.camp_number_) == false) {
      LOG_ERROR("Config file(%s) /data/camp_number not found.", file.c_str());
      return false;
    }

    if(cell_node->Attribute("map", (int *)&cell.map_) == false) {
      LOG_ERROR("Config file(%s) /data/map not found.", file.c_str());
      return false;
    }

    core::int32 auto_type = 0;
    if (cell_node->Attribute("auto_type", (int *)&auto_type) == false) {
      LOG_ERROR("Config file(%s) /data/auto_type not found.", file.c_str());
      return false;
    }
    if (auto_type < entity::PlayingAutoType::MIN ||
        auto_type >= entity::PlayingAutoType::MAX) {
      LOG_ERROR("Config file(%s) /data/auto_type(%d) invalid.",
          file.c_str(), auto_type);
      return false;
    }
    cell.auto_type_ = (entity::PlayingAutoType::type)auto_type;
    if (cell_node->Attribute("spend_freedom_dollars",
          (int *)&cell.auto_spend_freedom_dollars_) == false) {
      LOG_ERROR("Config file(%s) /data/spend_freedom_dollars not found.", file.c_str());
      return false;
    }

    if(cell_node->Attribute("min_level", (int *)&cell.min_level_) == false) {
      LOG_ERROR("Config file(%s) /data/min_level not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_level", (int *)&cell.max_level_) == false) {
      LOG_ERROR("Config file(%s) /data/max_level not found.", file.c_str());
      return false;
    }

    core::int32 period_type = 0;
    if (cell_node->Attribute("period_type", (int *)&cell.period_type_) == false) {
      LOG_ERROR("Config file(%s) /data/period_type not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("period_count", (int *)&cell.period_count_) == false) {
      LOG_ERROR("Config file(%s) /data/daily_count not found.", file.c_str());
      return false;
    }

    if (cell_node->Attribute("period_day", (int *)&cell.period_day_) == false) {
      LOG_ERROR("Config file(%s) /data/period_day not found.", file.c_str());
      return false;
    }

    const char *period_begin_time_str = cell_node->Attribute("period_begin_time");
    if (period_begin_time_str == NULL) {
      LOG_ERROR("Config file(%s) /data/period_begin_time not found.", file.c_str());
      return false;
    }
    std::vector<core::int32> period_begin_time_vector;
    global::StringSplit(period_begin_time_str, ":", period_begin_time_vector);
    if (period_begin_time_vector.size() == 3) {
      cell.period_begin_time_ = period_begin_time_vector[0] * 60 * 60 +
        period_begin_time_vector[1] * 60 + period_begin_time_vector[2];
    }

    if(cell_node->Attribute("min_actor", (int *)&cell.min_actor_) == false) {
      LOG_ERROR("Config file(%s) /data/min_actor not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_actor", (int *)&cell.max_actor_) == false) {
      LOG_ERROR("Config file(%s) /data/max_actor not found.", file.c_str());
      return false;
    }

    std::vector<std::string> award_string_vector;
    std::vector<core::int32> award_int_vector;

    const char *first_pass_awards_str = cell_node->Attribute("first_pass_awards");
    if (first_pass_awards_str == NULL) {
      LOG_ERROR("Config file(%s) /data/first_pass_awards not found.", file.c_str());
      return false;
    }
    global::StringSplit(first_pass_awards_str, "+", award_string_vector);
    for (size_t pos = 0; pos < award_string_vector.size(); ++pos) {
      global::StringSplit(award_string_vector[pos].c_str(), ":", award_int_vector);
      if (award_int_vector.size() != 3) {
        LOG_ERROR("Config file(%s) /data/first_pass_awards invalid.", file.c_str());
        return false;
      }
      PlayingAward award;
      award.type_ = (entity::ResourceType::type)award_int_vector[0];
      award.id_ = award_int_vector[1];
      award.number_ = award_int_vector[2];
      cell.first_pass_awards_.push_back(award);
    }

    const char *paid_award_str = cell_node->Attribute("buy_award");
    if (paid_award_str == NULL) {
      LOG_ERROR("Config file(%s) /data/paid_award not found.", file.c_str());
      return false;
    }
    global::StringSplit(paid_award_str, "+", award_string_vector);
    for (size_t pos = 0; pos < award_string_vector.size(); ++pos) {
      global::StringSplit(award_string_vector[pos].c_str(), ":", award_int_vector);
      if (award_int_vector.size() != 3) {
        LOG_ERROR("Config file(%s) /data/buy_award invalid.", file.c_str());
        return false;
      }
      PlayingAward award;
      award.type_ = (entity::ResourceType::type)award_int_vector[0];
      award.id_ = award_int_vector[1];
      award.number_ = award_int_vector[2];
      cell.paid_awards_.push_back(award);
    }
    if (cell_node->Attribute("buy_award_period_count",
          (int *)&cell.paid_award_period_count_) == false) {
      LOG_ERROR("Config file(%s) /data/buy_award_period_count not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("buy_award_spend_freedom_dollars",
          (int *)&cell.paid_award_spend_freedom_dollars_) == false) {
      LOG_ERROR("Config file(%s) /data/buy_award_spend_freedom_dollars not found.", file.c_str());
      return false;
    }

    std::vector<std::string> string_vector;
    std::vector<core::int32> temp_vector;

    const char *str = cell_node->Attribute("position_enter");
    if(str == NULL) {
      LOG_ERROR("Config file(%s) /data/position_enter not found.", file.c_str());
      return false;
    }
    global::StringSplit(str, "+", string_vector);
    for(size_t pos = 0; pos < string_vector.size(); ++pos) {
      global::StringSplit(string_vector[pos].c_str(), ":", temp_vector);
      if(temp_vector.size() != 2) {
        LOG_ERROR("Config file(%s) /data/position_enter invalid", file.c_str());
        return false;
      }
      cell.enter_posx_.push_back(temp_vector[0]);
      cell.enter_posy_.push_back(temp_vector[1]);
    }

    string_vector.clear();
    temp_vector.clear();
    str = cell_node->Attribute("position_exit");
    if(str == NULL) {
      LOG_ERROR("Config file(%s) /data/position_exit not found.", file.c_str());
      return false;
    }
    global::StringSplit(str, "+", string_vector);
    for(size_t pos = 0; pos < string_vector.size(); ++pos) {
      global::StringSplit(string_vector[pos].c_str(), ":", temp_vector);
      if(temp_vector.size() != 3) {
        LOG_ERROR("Config file(%s) /data/position_exit invalid", file.c_str());
        return false;
      }
      cell.exit_map_.push_back(temp_vector[0]);
      cell.exit_posx_.push_back(temp_vector[1]);
      cell.exit_posy_.push_back(temp_vector[2]);
    }

    this->playings_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const PlayingCell *PlayingConfigure::GetPlaying(core::int32 id) const {
  PlayingMap::const_iterator iterator = this->playings_.find(id);
  if(iterator != this->playings_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace configure

}  // namespace global

