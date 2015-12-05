
#include "game_server/server/extension/welfare/load_checkins_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

LoadCheckinsConfigure::LoadCheckinsConfigure() {}
LoadCheckinsConfigure::~LoadCheckinsConfigure() {}

bool LoadCheckinsConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  const char *str = NULL;
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    CheckinsCell cell;
    std::vector<core::int32> awards;
    if(cell_node->Attribute("id", &cell.id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sort", &cell.sort_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sort]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 奖励1
    str = cell_node->Attribute("reward1");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.award_[0].type_ = awards[0];
      cell.award_[0].id_   = awards[1];
      cell.award_[0].number_ = awards[2];
      cell.award_[0].bind_ = awards[3];
    }
    // 奖励2
    str = cell_node->Attribute("reward2");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.award_[1].type_ = awards[0];
      cell.award_[1].id_   = awards[1];
      cell.award_[1].number_ = awards[2];
      cell.award_[1].bind_ = awards[3];
    }
    // 奖励3
    str = cell_node->Attribute("reward3");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.award_[2].type_ = awards[0];
      cell.award_[2].id_   = awards[1];
      cell.award_[2].number_ = awards[2];
      cell.award_[2].bind_ = awards[3];
    }
    // 奖励4
    str = cell_node->Attribute("reward4");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.award_[3].type_ = awards[0];
      cell.award_[3].id_   = awards[1];
      cell.award_[3].number_ = awards[2];
      cell.award_[3].bind_ = awards[3];
    }
    // 奖励5
    str = cell_node->Attribute("reward5");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.award_[4].type_ = awards[0];
      cell.award_[4].id_   = awards[1];
      cell.award_[4].number_ = awards[2];
      cell.award_[4].bind_ = awards[3];
    }


    // 奖励1
    str = cell_node->Attribute("vip_reward1");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.vip_award_[0].type_ = awards[0];
      cell.vip_award_[0].id_   = awards[1];
      cell.vip_award_[0].number_ = awards[2];
      cell.vip_award_[0].bind_ = awards[3];
    }
    // 奖励2
    str = cell_node->Attribute("vip_reward2");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.vip_award_[1].type_ = awards[0];
      cell.vip_award_[1].id_   = awards[1];
      cell.vip_award_[1].number_ = awards[2];
      cell.vip_award_[1].bind_ = awards[3];
    }
    // 奖励3
    str = cell_node->Attribute("vip_reward3");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.vip_award_[2].type_ = awards[0];
      cell.vip_award_[2].id_   = awards[1];
      cell.vip_award_[2].number_ = awards[2];
      cell.vip_award_[2].bind_ = awards[3];
    }
    // 奖励4
    str = cell_node->Attribute("vip_reward4");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.vip_award_[3].type_ = awards[0];
      cell.vip_award_[3].id_   = awards[1];
      cell.vip_award_[3].number_ = awards[2];
      cell.vip_award_[3].bind_ = awards[3];
    }
    // 奖励5
    str = cell_node->Attribute("vip_reward5");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.vip_award_[4].type_ = awards[0];
      cell.vip_award_[4].id_   = awards[1];
      cell.vip_award_[4].number_ = awards[2];
      cell.vip_award_[4].bind_ = awards[3];
    }

    this->checkins_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

}  // namespace welfare

}  // namespace server

}  // namespace game

