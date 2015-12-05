//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-10 14:17:40.
// File name: server_configure.h
//
// Description:
// Define class ServerConfigure.
//

#include <map>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "core/tinyxml/tinyxml.h"
#include "global/server_informations.h"

namespace global {

class ServerConfigure : public core::Noncopyable {
  typedef std::map<core::uint32, ServerInformation> ServerInformationMap;
  typedef std::map<core::uint32, DatabaseServerInformation> DBServerInformationMap;
  typedef std::map<core::uint32, GatewayServerInformation> GatewayServerInformationMap;
  typedef std::map<core::uint32, GameServerInformation> GameServerInformationMap;

 public:
  ServerConfigure();
  ~ServerConfigure();

  inline const DatabaseServerInformation *GetDatabaseServers(core::uint32 id) const {
    DBServerInformationMap::const_iterator iterator =
      this->database_servers_.find(id);
    if(iterator != this->database_servers_.end()) {
      return &iterator->second;
    }
    return NULL;
  }
  inline const GameServerInformation *GetGameServers(core::uint32 id) const {
    GameServerInformationMap::const_iterator iterator =
      this->game_servers_.find(id);
    if(iterator != this->game_servers_.end()) {
      return &iterator->second;
    }
    return NULL;
  }
  inline const GatewayServerInformation *GetGatewayServers(core::uint32 id) const {
    GatewayServerInformationMap::const_iterator iterator =
      this->gateway_servers_.find(id);
    if(iterator != this->gateway_servers_.end()) {
      return &iterator->second;
    }
    return NULL;
  }

  inline const ServerInformation *GetScheduleServer() const {
    return &this->schedule_server_;
  }
  inline const SessionServerInformation *GetSessionServer() const {
    return &this->session_server_;
  }

  inline const GameDatabaseInformation *GetGameDatabaseInformation() const {
    return &this->game_databases_;
  }

  inline core::uint16 GetZoneID() const {
    return this->zone_id_;
  }

  inline const std::string &GetQuestion() const {
    return this->question_;
  }

  inline const bool CheckGmCommandEnable() const {
    return this->gm_command_enable_;
  }

  inline const bool CheckMoveCheckEnable() const {
    return this->move_check_enable_;
  }

  inline const std::string &GetConfigueDir() const {
    return this->configure_dir_;
  }

  inline const std::string &GetAccordingLevelFile() const {
    return this->according_level_file_;
  }

  inline const std::string &GetAttrConvertFile() const {
    return this->attr_convert_file_;
  }

  inline const std::string &GetAttrFightScoreFile() const {
    return this->attr_fight_score_file_;
  }

  inline const std::string &GetVocationGrowthFile() const {
    return this->vocation_growth_file_;
  }

  inline const std::string &GetNpcFile() const {
    return this->npc_file_;
  }

  inline const std::string &GetItemFile() const {
    return this->item_file_;
  }

  inline const std::string &GetPythonFile() const {
    return this->python_file_;
  }

  inline const std::string &GetAiFile() const {
    return this->ai_file_;
  }

  inline const std::string &GetCoolingDir() const {
    return this->cooling_file_;
  }

  inline const std::string &GetEffectDir() const {
    return this->effect_file_;
  }

  inline const std::string &GetScenesFile() const {
    return this->scenes_file_;
  }

  inline const std::string &GetSkillsFile() const {
    return this->skills_file_;
  }

  inline const std::string &GetSkillFile() const {
    return this->skill_file_;
  }

  inline const std::string &GetSoulFile() const {
    return this->soul_file_;
  }

  inline const std::string &GetDirectHurtFile() const {
    return this->directhurt_file_;
  }

  inline const std::string &GetBuffFile() const {
    return this->buff_file_;
  }

  inline const std::string &GetTaskFile() const {
    return this->task_file_;
  }

  inline const std::string &GetAttrFile() const {
    return this->attr_file_;
  }

  inline const std::string &GetChatFile() const {
    return this->chat_file_;
  }

  inline const AutoSaveInformation &GetAutoSave() const {
    return this->auto_save_;
  }

  inline const std::string &GetMailFile() const {
    return this->mail_file_;
  }

  inline const std::string &GetTeamFile() const {
    return this->team_file_;
  }
  
  inline const std::string &GetBarFile() const {
    return this->bar_file_;
  }

  inline const std::string &GetCampFile() const {
    return this->camp_file_;
  }

  inline const std::string &GetFunctionalityFile() {
    return this->functionality_open_file_;
  }

  inline const std::string &GetDropFile() const {
    return this->drop_file_;
  }

  inline const std::string &GetSubDropFile() const {
    return this->sub_drop_file_;
  }

  inline const std::string &GetWelfarefile() const {
    return this->welfare_file_;
  }

  inline const core::uint64 GetOpenServiceTime() const {
    return this->open_service_time_;
  }

  // Load and parse configure file.
  bool LoadFile(const char *file);
  bool LoadCoreshFile(const char *file);

 private:
  bool LoadDatabaseServers(TiXmlElement *configure_node, const std::string &file);
  bool LoadGameServers(TiXmlElement *configure_node, const std::string &file);
  bool LoadGatewayServers(TiXmlElement *configure_node, const std::string &file);

  bool LoadScheduleServer(TiXmlElement *configure_node, const std::string &file);
  bool LoadSessionServer(TiXmlElement *configure_node, const std::string &file);

  // Directory of configure file.
  std::string configure_dir_;

  // configure file.
  std::string according_level_file_;

  std::string attr_convert_file_;

  std::string attr_fight_score_file_;

  std::string vocation_growth_file_;

  std::string npc_file_;

  std::string item_file_;

  std::string python_file_;

  std::string ai_file_;

  // Directory of cooling configure file.
  std::string cooling_file_;

  // Directory of effect configure file.
  std::string effect_file_;

  // Directory of scenes configure file.
  std::string scenes_file_;

  // Directroy of skills configure files.
  std::string skill_file_;
  std::string skills_file_;
  std::string directhurt_file_;

  // Directory of soul configure file.
  std::string soul_file_;

  // Directory of buff configure files.
  std::string buff_file_;

  // Directory of task configure file.
  std::string task_file_;

  // Directory of attr configure file.
  std::string attr_file_;

  // Directory of attr configure file.
  std::string chat_file_;

  // Directory of mail configure file.
  std::string mail_file_;

  // Directory of team configure file.
  std::string team_file_;

  // Directory of bar configure file.
  std::string bar_file_;

  // Directory of camp configure file.
  std::string camp_file_;

  // Directory of functionality configure file.
  std::string functionality_open_file_;

  // Directory of drop configure file.
  std::string drop_file_;
  // Directory of sub drop configure file.
  std::string sub_drop_file_;
  
  // Directory of welfare configure file
  std::string welfare_file_;

  // open service time
  core::int64 open_service_time_;

  DBServerInformationMap database_servers_;
  GameDatabaseInformation game_databases_;

  GameServerInformationMap game_servers_;
  GatewayServerInformationMap gateway_servers_;

  ServerInformation schedule_server_;
  SessionServerInformation session_server_;

  core::uint16 zone_id_;
  std::string question_;
  bool gm_command_enable_;
  bool move_check_enable_;

  AutoSaveInformation auto_save_;
};

class ServerConfigureSingleton : public core::Noncopyable {
 public:
  ServerConfigureSingleton() {}
  ~ServerConfigureSingleton() {}

  static inline ServerConfigure &GetInstance() {
    return ServerConfigureSingleton::instance_;
  }

 private:
  static ServerConfigure instance_;
};

}  // namespace global

