//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-10 14:26:31.
// File name: server_configure.cc
//
// Description:
// Define class ServerConfigure.
//

#include "global/server_configure.h"

#include <iostream>

#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "coresh/variable_table.h"
#include "global/common_functions.h"

namespace global {

ServerConfigure ServerConfigureSingleton::instance_;

ServerConfigure::ServerConfigure() {}
ServerConfigure::~ServerConfigure() {}

bool ServerConfigure::LoadFile(const char *file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file) == false) {
    std::cout << "Load file(" << file << ") error." << std::endl;
    return false;
  }

  // Configure node.
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    std::cout << "Can't get node [configure], file(" << file << ")." << std::endl;
    return false;
  }

  TiXmlElement *global_node = configure_node->FirstChildElement("global");
  if(global_node == NULL) {
    std::cout << "Can't get node [global], file(" << file << ")." << std::endl;
    return false;
  }

  const char *str = NULL;
  int int_value = 0;

  TiXmlElement *zone_node = global_node->FirstChildElement("zone");
  if(zone_node == NULL) {
    std::cout << "Can't get node [global-zone], file(" << file << ")." << std::endl;
    return false;
  }

  if(zone_node->Attribute("id", (int *)(&this->zone_id_)) == NULL) {
    std::cout << "Can't get attribute [global-zone-id], file("
      << file << ")." << std::endl;
    return false;
  }

  TiXmlElement *question_node = global_node->FirstChildElement("question");
  if(question_node == NULL) {
    std::cout << "Can't get node [global-question], file(" << file << ")." << std::endl;
    return false;
  }

  str = question_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [global-question-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->question_ = str;

  TiXmlElement *gm_command_node = global_node->FirstChildElement("gm_command");
  if(gm_command_node == NULL) {
    std::cout << "Can't get node [global-gm_command], file(" << file << ")." << std::endl;
    return false;
  }
  if(gm_command_node->Attribute("enable", (int *)(&int_value)) == NULL) {
    std::cout << "Can't get attribute [global-gm_command-enable], file(" << file << ")." << std::endl;
    return false;
  }
  this->gm_command_enable_ = int_value;

  TiXmlElement *move_check_node = global_node->FirstChildElement("move_check");
  if(move_check_node == NULL) {
    std::cout << "Can't get node [global-move_check], file(" << file << ")." << std::endl;
    return false;
  }
  if(move_check_node->Attribute("enable", (int *)(&int_value)) == NULL) {
    std::cout << "Can't get attribute [global-move_check-enable], file(" << file << ")." << std::endl;
    return false;
  }
  this->move_check_enable_ = int_value;

  TiXmlElement *config_dir_node = global_node->FirstChildElement("config_dir");
  if(config_dir_node == NULL) {
    std::cout << "Can't get node [global-config_dir], file("
      << file << ")." << std::endl;
    return false;
  }

  str = config_dir_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [global-config_dir-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->configure_dir_ = str;

  TiXmlElement *auto_save_node = global_node->FirstChildElement("auto_save");
  if(auto_save_node == NULL) {
    std::cout << "Can't get node [global-auto_save], file(" << file << ")." << std::endl;
    return false;
  }

  if(auto_save_node->Attribute("actor", (int *)(&this->auto_save_.actor_)) == NULL) {
    std::cout << "Can't get attribute [global-auto_save-actor], file(" << file << ")." << std::endl;
    return false;
  }

  TiXmlElement *open_service_time_node = global_node->FirstChildElement("open_service_time");
  if(open_service_time_node == NULL) {
    std::cout << "Can't get node [global-open_service_time_node], file(" << file << ")." << std::endl;
    return false;
  }
  str = open_service_time_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [open_service_time]";
    return false;
  }
  this->open_service_time_ = global::FormatTime(str);

  time_t now = time(NULL);
  if(now <= this->open_service_time_) {
    std::cout << "开服时间不得小于当前时间";
    return false;
  }

  // Database server configure.
  return this->LoadDatabaseServers(configure_node, file) &&
    this->LoadGameServers(configure_node, file) &&
    this->LoadGatewayServers(configure_node, file) &&
    this->LoadScheduleServer(configure_node, file) &&
    this->LoadSessionServer(configure_node, file);
}

bool ServerConfigure::LoadCoreshFile(const char *file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file) == false) {
    std::cout << "Load file(" << file << ") error." << std::endl;
    return false;
  }

  // Configure node.
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    std::cout << "Can't get node [configure], file(" << file << ")." << std::endl;
    return false;
  }

  // Coresh node.
  TiXmlElement *coresh_node = configure_node->FirstChildElement("coresh");
  if(coresh_node == NULL) {
    std::cout << "Can't get node [configure-coresh], file("
      << file << ")." << std::endl;
    return false;
  }

  // Message node.
  TiXmlElement *message_node = coresh_node->FirstChildElement("message");
  if(message_node == NULL) {
    std::cout << "Can't get node [configure-coresh-message], file("
      << file << ")." << std::endl;
    return false;
  }

  const char *str = NULL;
  size_t data = 0;

  if(message_node->Attribute("max_protocol_size", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-message-max_protocol_size], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetMaxProtocolSize(data);

  if(message_node->Attribute("max_message_size", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-message-max_message_size], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetMaxMessageSize(data);

  // Message queue node.
  TiXmlElement *message_queue_node = coresh_node->FirstChildElement("message_queue");
  if(message_queue_node == NULL) {
    std::cout << "Can't get node [configure-coresh-message_queue], file("
      << file << ")." << std::endl;
    return false;
  }

  if(message_queue_node->Attribute("initial_size", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-message_queue-initial_size], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetMessageQueueInitialSize(data);

  // Connection node.
  TiXmlElement *connection_node = coresh_node->FirstChildElement("connection");
  if(connection_node == NULL) {
    std::cout << "Can't get node [configure-coresh-connection], file("
      << file << ")." << std::endl;
    return false;
  }

  if(connection_node->Attribute("pool_initial_number", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-connection-pool_initial_number], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetConnectionPoolInitialNumber(data);

  if(connection_node->Attribute("extend_number", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-connection-extend_number], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetConnectionPoolExtendNumber(data);

  // Thread node.
  TiXmlElement *thread_node = coresh_node->FirstChildElement("thread");
  if(thread_node == NULL) {
    std::cout << "Can't get node [configure-coresh-thread], file("
      << file << ")." << std::endl;
    return false;
  }

  if(thread_node->Attribute("network_number", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-thread-network_number], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetNetworkThreadNumber(data);

  // Packet node.
  TiXmlElement *packet_node = coresh_node->FirstChildElement("packet");
  if(packet_node == NULL) {
    std::cout << "Can't get node [configure-coresh-packet], file("
      << file << ")." << std::endl;
    return false;
  }

  if(packet_node->Attribute("compress", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-packet-compress], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetCompressType((coresh::CompressType::Type)data);

  if(packet_node->Attribute("encode", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-packet-encode], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetEncodeType((coresh::EncodeType::Type)data);

  return true;
}

bool ServerConfigure::LoadDatabaseServers(TiXmlElement *configure_node, const std::string &file) {
  const char *str = NULL;

  // Database node.
  TiXmlElement *database_node = configure_node->FirstChildElement("database_server");
  if(database_node == NULL) {
    std::cout << "Can't get node [configure-database_server], file("
      << file << ")." << std::endl;
    return false;
  }

  {

    // Node node.
    TiXmlElement *node_node = database_node->FirstChildElement("node");
    while(node_node) {
      DatabaseServerInformation information;

      if(node_node->Attribute("id", (int *)(&information.id_)) == NULL) {
        std::cout << "Can't get attribute [database_server-node-id], file("
          << file << ")." << std::endl;
        return false;
      }

      // Host node.
      TiXmlElement *host_node = node_node->FirstChildElement("host");
      if(host_node == NULL) {
        std::cout << "Can't get node [database_server-node-host], file("
          << file << ")." << std::endl;
        return false;
      }

      str = host_node->Attribute("ip");
      if(str == NULL) {
        std::cout << "Can't get attribute [database_server-node-host-ip], file("
          << file << ")." << std::endl;
        return false;
      }
      information.ip_ = str;

      if(host_node->Attribute("port", (int *)(&information.port_)) == NULL) {
        std::cout << "Can't get attribute [database_server-node-host-port], file("
          << file << ")." << std::endl;
        return false;
      }

      // Log node.
      TiXmlElement *log_node = node_node->FirstChildElement("log");
      if(log_node == NULL) {
        std::cout << "Can't get node [database_server-node-log], file("
          << file << ")." << std::endl;
        return false;
      }

      str = log_node->Attribute("value");
      if(str == NULL) {
        std::cout << "Can't get attribute [database_server-node-log-value], file("
          << file << ")." << std::endl;
        return false;
      }
      information.log_ = str;

      // Core node.
      TiXmlElement *core_node = node_node->FirstChildElement("core");
      if(core_node == NULL) {
        std::cout << "Can't get node [database_server-node-core], file("
          << file << ")." << std::endl;
        return false;
      }

      str = core_node->Attribute("value");
      if(str == NULL) {
        std::cout << "Can't get attribute [database_server-node-core-value], file("
          << file << ")." << std::endl;
        return false;
      }
      information.core_ = str;

      // Coresh node.
      TiXmlElement *coresh_node = node_node->FirstChildElement("coresh");
      if(coresh_node == NULL) {
        std::cout << "Can't get node [database_server-node-coresh], file("
          << file << ")." << std::endl;
        return false;
      }

      str = coresh_node->Attribute("value");
      if(str == NULL) {
        std::cout << "Can't get attribute [database_server-node-coresh-value], file("
          << file << ")." << std::endl;
        return false;
      }
      information.coresh_ = str;

      // DB node.
      TiXmlElement *db_node = node_node->FirstChildElement("db");
      if(db_node == NULL) {
        std::cout << "Can't get node [database_server-node-db], file("
          << file << ")." << std::endl;
        return false;
      }

      if(db_node->Attribute("request_queue_size", (int *)(&information.request_queue_size_)) == NULL) {
        std::cout << "Can't get attribute [database_server-node-db-request_queue_size]"
          << file << ")." << std::endl;
        return false;
      }

      if(db_node->Attribute("request_queue_reduce_waterline",
            (int *)(&information.request_queue_reduce_waterline_)) == NULL) {
        std::cout << "Can't get attribute [database_server-node-db-request_queue_reduce_waterline"
          << file << ")." << std::endl;
        return false;
      }

      if(db_node->Attribute("db_thread", (int *)(&information.db_thread_number_)) == NULL) {
        std::cout << "Can't get attribute [database_server-node-db-db_thread]"
          << file << ")." << std::endl;
        return false;
      }

      if(this->database_servers_.find(information.id_) != this->database_servers_.end()) {
        std::cout << "database server has repeat id[" << information.id_ << "]." << std::endl;
        return false;
      }
      this->database_servers_.insert(std::make_pair(information.id_, information));

      node_node = node_node->NextSiblingElement("node");
    }

  }

  // Database node.
  TiXmlElement *game_db_node = database_node->FirstChildElement("game_db");
  if(game_db_node == NULL) {
    std::cout << "Can't get node [database_server-game_db], file("
      << file << ")." << std::endl;
    return false;
  }

  // game node.
  TiXmlElement *game_node = game_db_node->FirstChildElement("game");
  if(game_node == NULL) {
    std::cout << "Can't get node [game_db-game], file("
      << file << ")." << std::endl;
    return false;
  }

  str = game_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-game-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.game_database_.ip_ = str;

  if(game_node->Attribute("port",
        (int *)(&this->game_databases_.game_database_.port_)) == NULL) {
    std::cout << "Can't get attribute [game_db-game-port], file("
      << file << ")." << std::endl;
    return false;
  }

  str = game_node->Attribute("user");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-game-user], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.game_database_.user_ = str;

  str = game_node->Attribute("passwd");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-game-passwd], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.game_database_.passwd_ = str;

  str = game_node->Attribute("db");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-game-db], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.game_database_.db_ = str;

  // center node.
  TiXmlElement *center_node = game_db_node->FirstChildElement("center");
  if(center_node == NULL) {
    std::cout << "Can't get attribute [game_db-center], file("
      << file << ")." << std::endl;
    return false;
  }

  str = center_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-center-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.center_database_.ip_ = str;

  if(center_node->Attribute("port",
        (int *)(&this->game_databases_.center_database_.port_)) == NULL) {
    std::cout << "Can't get attribute [game_db-center-port], file("
      << file << ")." << std::endl;
    return false;
  }

  str = center_node->Attribute("user");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-center-user], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.center_database_.user_ = str;

  str = center_node->Attribute("passwd");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-center-passwd], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.center_database_.passwd_ = str;

  str = center_node->Attribute("db");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_db-center-db], file("
      << file << ")." << std::endl;
    return false;
  }
  this->game_databases_.center_database_.db_ = str;

  // Procedure node.
  TiXmlElement *procedure_node = game_db_node->FirstChildElement("procedure");
  while(procedure_node) {
    int type = 0;
    if(procedure_node->Attribute("type", (int *)(&type)) == NULL) {
      std::cout << "Can't get attribute [game_db-procedure-type], file("
        << file << ")." << std::endl;
      return false;
    }
    str = procedure_node->Attribute("name");
    if(str == NULL) {
      std::cout << "Can't get attribute [game_db-procedure-name], file("
        << file << ")." << std::endl;
      return false;
    }
    if(this->game_databases_.InsertProcedure(type, str) == false) {
      std::cout << "Procedure [type=" << type << ", name=" << str
        << "] repeat, file(" << file << ")." << std::endl;
      return false;
    }
    procedure_node = procedure_node->NextSiblingElement("procedure");
  }

  return true;
}

bool ServerConfigure::LoadGameServers(TiXmlElement *configure_node, const std::string &file) {
  const char *str = NULL;

  // Game node.
  TiXmlElement *game_node = configure_node->FirstChildElement("game_server");
  if(game_node == NULL) {
    std::cout << "Can't get node [configure-game_server], file("
      << file << ")." << std::endl;
    return false;
  }

  // according level node.
  TiXmlElement *according_level_node = game_node->FirstChildElement("according_level");
  if(according_level_node == NULL) {
    std::cout << "Can't get node [configure-game_server-according_level], file("
      << file << ")." << std::endl;
    return false;
  }

  str = according_level_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-according_level-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->according_level_file_ = str;

  // attr_convert level node.
  TiXmlElement *attr_convert_node = game_node->FirstChildElement("attr_convert");
  if(attr_convert_node == NULL) {
    std::cout << "Can't get node [configure-game_server-attr_convert], file("
      << file << ")." << std::endl;
    return false;
  }

  str = attr_convert_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-attr_convert-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->attr_convert_file_ = str;

  // attr_fight_score node.
  TiXmlElement *attr_fight_score_node = game_node->FirstChildElement("attr_fight_score");
  if(attr_fight_score_node == NULL) {
    std::cout << "Can't get node [configure-game_server-attr_fight_score], file("
      << file << ")." << std::endl;
    return false;
  }

  str = attr_fight_score_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-attr_fight_score-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->attr_fight_score_file_ = str;

  // vocation_growth node.
  TiXmlElement *vocation_growth_node = game_node->FirstChildElement("vocation_growth");
  if(vocation_growth_node == NULL) {
    std::cout << "Can't get node [configure-game_server-vocation_growth], file("
      << file << ")." << std::endl;
    return false;
  }

  str = vocation_growth_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-vocation_growth-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->vocation_growth_file_ = str;

  // npc node
  TiXmlElement *npc_node = game_node->FirstChildElement("npc");
  if(npc_node == NULL) {
    std::cout << "Can't get node [game_server-npc], file("
      << file << ")." << std::endl;
    return false;
  }
  str = npc_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-npc-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->npc_file_ = str;

  // item node
  TiXmlElement *item_node = game_node->FirstChildElement("item");
  if(item_node == NULL) {
    std::cout << "Can't get node [game_server-item], file("
      << file << ")." << std::endl;
    return false;
  }
  str = item_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-item-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->item_file_ = str;

  // python node
  TiXmlElement *python_node = game_node->FirstChildElement("python");
  if(python_node == NULL) {
    std::cout << "Can't get node [game_server-python], file("
      << file << ")." << std::endl;
    return false;
  }
  str = python_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-python-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->python_file_ = str;

  // ai node
  TiXmlElement *ai_node = game_node->FirstChildElement("ai");
  if(ai_node == NULL) {
    std::cout << "Can't get node [game_server-ai], file("
      << file << ")." << std::endl;
    return false;
  }
  str = ai_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [game_server-ai-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->ai_file_ = str;

  // cooling info node.
  TiXmlElement *cooling_node = game_node->FirstChildElement("cooling");
  if(cooling_node == NULL) {
    std::cout << "Can't get node [configure-game_server-cooling], file("
      << file << ")." << std::endl;
    return false;
  }

  // cooling.
  const char *cooling = cooling_node->Attribute("value");
  if(cooling == NULL) {
    std::cout << "Can't get attribute [configure-game_server-cooling-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->cooling_file_ = this->configure_dir_ + cooling;

  // effect info node.
  TiXmlElement *effect_node = game_node->FirstChildElement("effect");
  if(effect_node == NULL) {
    std::cout << "Can't get node [configure-game_server-effect], file("
      << file << ")." << std::endl;
    return false;
  }

  // effect.
  const char *effect = effect_node->Attribute("value");
  if(effect == NULL) {
    std::cout << "Can't get attribute [configure-game_server-effect-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->effect_file_ = this->configure_dir_ + effect;

  // scenes info node.
  TiXmlElement *scenesinfo_node = game_node->FirstChildElement("scenesinfo");
  if(scenesinfo_node == NULL) {
    std::cout << "Can't get node [configure-game_server-scenesinfo], file("
      << file << ")." << std::endl;
    return false;
  }

  // scenes info.
  const char *scenesinfo = scenesinfo_node->Attribute("value");
  if(scenesinfo == NULL) {
    std::cout << "Can't get attribute [configure-game_server-scenesinfo-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->scenes_file_ = this->configure_dir_ + scenesinfo;

  // skill node.
  TiXmlElement *skill_node = game_node->FirstChildElement("skill");
  if(skill_node == NULL) {
    std::cout << "Can't get node [configure-game_server-skill], file("
      << file << ")." << std::endl;
    return false;
  }
  const char *skill_file = skill_node->Attribute("value");
  if(skill_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-skill-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->skill_file_ = skill_file;

  // soul node.
  TiXmlElement *soul_node = game_node->FirstChildElement("soul");
  if(soul_node == NULL) {
    std::cout << "Can't get node [configure-game_server-soul]，file("
      << file << ")." << std::endl;
    return false;
  }
  const char *soul_file = soul_node->Attribute("value");
  if(soul_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-soul-value]，file("
      << file << ")." << std::endl;
    return false;
  }
  this->soul_file_ = soul_file;

  // buff  info node.
  TiXmlElement *buffinfo_node = game_node->FirstChildElement("buffinfo");
  if(buffinfo_node == NULL) {
    std::cout << "Can't get node [configure-game_server-buffinfo], file("
      << file << ")." << std::endl;
    return false;
  }
  // skills info.
  const char *buffinfo = buffinfo_node->Attribute("value");
  if(buffinfo == NULL) {
    std::cout << "Can't get attribute [configure-game_server-buffinfo-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->buff_file_ = this->configure_dir_ + buffinfo;

  // task node.
  TiXmlElement *task_node = game_node->FirstChildElement("task");
  if(task_node == NULL) {
    std::cout << "Can't get node [configure-game_server-task]，file("
      << file << ")." << std::endl;
    return false;
  }
  // task file.
  const char *task_file = task_node->Attribute("value");
  if(task_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-task-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->task_file_ = this->configure_dir_ + task_file;

  // attr node.
  TiXmlElement *attr_node = game_node->FirstChildElement("attr");
  if(attr_node == NULL) {
    std::cout << "Can't get node [configure-game_server-attr]，file("
      << file << ")." << std::endl;
    return false;
  }
  // attr file.
  const char *attr_file = attr_node->Attribute("value");
  if(attr_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-attr-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->attr_file_ = this->configure_dir_ + attr_file;

  // chat node.
  TiXmlElement *chat_node = game_node->FirstChildElement("chat");
  if(attr_node == NULL) {
    std::cout << "Can't get node [configure-game_server-chat]，file("
      << file << ")." << std::endl;
    return false;
  }
  // chat file.
  const char *chat_file = chat_node->Attribute("value");
  if(chat_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-chat-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->chat_file_ = this->configure_dir_ + chat_file;

  // mail node.
  TiXmlElement *mail_node = game_node->FirstChildElement("mail");
  if(mail_node == NULL) {
    std::cout << "Can't get node [configure-game_server-mail]，file("
      << file << ")." << std::endl;
    return false;
  }

  // mail file.
  const char *mail_file = mail_node->Attribute("value");
  if(mail_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-mail-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->mail_file_ = this->configure_dir_ + mail_file;

  // team node.
  TiXmlElement *team_node = game_node->FirstChildElement("team");
  if(team_node == NULL) {
    std::cout << "Can't get node [configure-game_server-team]，file("
      << file << ")." << std::endl;
    return false;
  }
  // team file.
  const char *team_file = team_node->Attribute("value");
  if(team_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-team-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->team_file_ = this->configure_dir_ + team_file;
  
  // bar node.
  TiXmlElement *bar_node = game_node->FirstChildElement("bar");
  if(bar_node == NULL) {
    std::cout << "Can't get node [configure-game_server-bar]，file("
      << file << ")." << std::endl;
    return false;
  }

  // bar file.
  const char *bar_file = bar_node->Attribute("value");
  if(bar_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-bar-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->bar_file_ = this->configure_dir_ + bar_file;

  // camp node.
  TiXmlElement *camp_node = game_node->FirstChildElement("camp");
  if(camp_node == NULL) {
    std::cout << "Can't get node [configure-game_server-camp]，file("
      << file << ")." << std::endl;
    return false;
  }

  // camp file.
  const char *camp_file = camp_node->Attribute("value");
  if(camp_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-camp-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->camp_file_ = camp_file;

  // functionality_open node.
  TiXmlElement *functionality_node = game_node->FirstChildElement("functionality");
  if(functionality_node == NULL) {
    std::cout << "Can't get node [configure-game_server-functionality]，file("
      << file << ")." << std::endl;
    return false;
  }

  // functionality_open file.
  const char *functionality_file = functionality_node->Attribute("value");
  if(functionality_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-functionality-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->functionality_open_file_ = functionality_file;

  // drop node.
  TiXmlElement *drop_node = game_node->FirstChildElement("drop");
  if(drop_node == NULL) {
    std::cout << "Can't get attribute [configure-game_server-drop], file("
      << file << ")." << std::endl;
    return false;
  }

  // drop file.
  const char *drop_file = drop_node->Attribute("value");
  if(drop_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-drop-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->drop_file_ = drop_file;

  // sub drop node.
  TiXmlElement *sub_drop_node = game_node->FirstChildElement("sub_drop");
  if(sub_drop_node == NULL) {
    std::cout << "Can't get attribute [configure-game_server-sub_drop], file("
      << file << ")." << std::endl;
    return false;
  }

  // sub drop file.
  const char *sub_drop_file = sub_drop_node->Attribute("value");
  if(drop_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-sub_drop-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->sub_drop_file_ = sub_drop_file;

  // welfare node.
  TiXmlElement *welfare_node = game_node->FirstChildElement("welfare");
  if(welfare_node == NULL) {
    std::cout << "Can't get attribute [configure-game_server-welfare], file("
      << file << ")." << std::endl;
    return false;
  }

  const char *welfare_file = welfare_node->Attribute("value");
  if(welfare_file == NULL) {
    std::cout << "Can't get attribute [configure-game_server-welfare-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->welfare_file_ = this->configure_dir_ + welfare_file;

  // Node node.
  TiXmlElement *node_node = game_node->FirstChildElement("node");
  while(node_node) {
    if(node_node == NULL) {
      std::cout << "Can't get node [game_server-node], file("
        << file << ")." << std::endl;
      return false;
    }

    GameServerInformation information;

    if(node_node->Attribute("id", (int *)(&information.id_)) == NULL) {
      std::cout << "Can't get attribute [game_server-node-id], file("
        << file << ")." << std::endl;
      return false;
    }

    // Host node.
    TiXmlElement *host_node = node_node->FirstChildElement("host");
    if(host_node == NULL) {
      std::cout << "Can't get node [game_server-node-host], file("
        << file << ")." << std::endl;
      return false;
    }

    str = host_node->Attribute("ip");
    if(str == NULL) {
      std::cout << "Can't get attribute [game_server-node-host-ip], file("
        << file << ")." << std::endl;
      return false;
    }
    information.ip_ = str;

    if(host_node->Attribute("port", (int *)(&information.port_)) == NULL) {
      std::cout << "Can't get attribute [game_server-node-host-port], file("
        << file << ")." << std::endl;
      return false;
    }

    // Log node.
    TiXmlElement *log_node = node_node->FirstChildElement("log");
    if(log_node == NULL) {
      std::cout << "Can't get node [game_server-node-log], file("
        << file << ")." << std::endl;
      return false;
    }

    str = log_node->Attribute("value");
    if(str == NULL) {
      std::cout << "Can't get attribute [game_server-node-log-value], file("
        << file << ")." << std::endl;
      return false;
    }
    information.log_ = str;

    // Core node.
    TiXmlElement *core_node = node_node->FirstChildElement("core");
    if(core_node == NULL) {
      std::cout << "Can't get node [game_server-node-core], file("
        << file << ")." << std::endl;
      return false;
    }

    str = core_node->Attribute("value");
    if(str == NULL) {
      std::cout << "Can't get attribute [game_server-node-core-value], file("
        << file << ")." << std::endl;
      return false;
    }
    information.core_ = str;

    // Coresh node.
    TiXmlElement *coresh_node = node_node->FirstChildElement("coresh");
    if(coresh_node == NULL) {
      std::cout << "Can't get node [game_server-node-coresh], file("
        << file << ")." << std::endl;
      return false;
    }

    str = coresh_node->Attribute("value");
    if(str == NULL) {
      std::cout << "Can't get attribute [game_server-node-coresh-value], file("
        << file << ")." << std::endl;
      return false;
    }
    information.coresh_ = str;

    TiXmlElement *user_pool_node = node_node->FirstChildElement("user_pool");
    if(user_pool_node == NULL) {
      std::cout << "Can't get node [game_server-node-user_pool], file("
        << file << ")." << std::endl;
      return false;
    }

    if(user_pool_node->Attribute("initial_number", (int *)(&information.user_pool_initial_)) == NULL) {
      std::cout << "Can't get attribute [game_server-node-user_pool-initial_number], file("
        << file << ")." << std::endl;
      return false;
    }

    if(user_pool_node->Attribute("extend_number", (int *)(&information.user_pool_extend_)) == NULL) {
      std::cout << "Can't get attribute [game_server-node-user_pool-extend_number], file("
        << file << ")." << std::endl;
      return false;
    }

    TiXmlElement *npc_pool_node = node_node->FirstChildElement("npc_pool");
    if(npc_pool_node == NULL) {
      std::cout << "Can't get node [game_server-node-npc_pool], file("
        << file << ")." << std::endl;
      return false;
    }

    if(npc_pool_node->Attribute("initial_number", (int *)(&information.npc_pool_initial_)) == NULL) {
      std::cout << "Can't get attribute [game_server-node-npc_pool-initial_number], file("
        << file << ")." << std::endl;
      return false;
    }

    if(npc_pool_node->Attribute("extend_number", (int *)(&information.npc_pool_extend_)) == NULL) {
      std::cout << "Can't get attribute [game_server-node-npc_pool-extend_number], file("
        << file << ")." << std::endl;
      return false;
    }

    if(this->game_servers_.find(information.id_) != this->game_servers_.end()) {
      std::cout << "game server has repeat id[" << information.id_ << "]." << std::endl;
      return false;
    }
    this->game_servers_.insert(std::make_pair(information.id_, information));

    node_node = node_node->NextSiblingElement("node");
  }

  return true;
}

bool ServerConfigure::LoadGatewayServers(TiXmlElement *configure_node, const std::string &file) {
  const char *str = NULL;

  // Gateway node.
  TiXmlElement *gateway_node = configure_node->FirstChildElement("gateway_server");
  if(gateway_node == NULL) {
    std::cout << "Can't get node [configure-gateway_server], file("
      << file << ")." << std::endl;
    return false;
  }

  // Node node.
  TiXmlElement *node_node = gateway_node->FirstChildElement("node");
  while(node_node) {
    if(node_node == NULL) {
      std::cout << "Can't get node [gateway_server-node], file("
        << file << ")." << std::endl;
      return false;
    }

    GatewayServerInformation information;

    if(node_node->Attribute("id", (int *)(&information.id_)) == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-id], file("
        << file << ")." << std::endl;
      return false;
    }

    // Host node.
    TiXmlElement *host_node = node_node->FirstChildElement("host");
    if(host_node == NULL) {
      std::cout << "Can't get node [gateway_server-node-host], file("
        << file << ")." << std::endl;
      return false;
    }

    str = host_node->Attribute("ip");
    if(str == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-host-ip], file("
        << file << ")." << std::endl;
      return false;
    }
    information.ip_ = str;

    if(host_node->Attribute("port", (int *)(&information.port_)) == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-host-port], file("
        << file << ")." << std::endl;
      return false;
    }

    // Log node.
    TiXmlElement *log_node = node_node->FirstChildElement("log");
    if(log_node == NULL) {
      std::cout << "Can't get node [gateway_server-node-log], file("
        << file << ")." << std::endl;
      return false;
    }

    str = log_node->Attribute("value");
    if(str == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-log-value], file("
        << file << ")." << std::endl;
      return false;
    }
    information.log_ = str;

    // Core node.
    TiXmlElement *core_node = node_node->FirstChildElement("core");
    if(core_node == NULL) {
      std::cout << "Can't get node [gateway_server-node-core], file("
        << file << ")." << std::endl;
      return false;
    }

    str = core_node->Attribute("value");
    if(str == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-core-value], file("
        << file << ")." << std::endl;
      return false;
    }
    information.core_ = str;

    // Coresh node.
    TiXmlElement *coresh_node = node_node->FirstChildElement("coresh");
    if(coresh_node == NULL) {
      std::cout << "Can't get node [gateway_server-node-coresh], file("
        << file << ")." << std::endl;
      return false;
    }

    str = coresh_node->Attribute("value");
    if(str == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-coresh-value], file("
        << file << ")." << std::endl;
      return false;
    }
    information.coresh_ = str;

    if(this->gateway_servers_.find(information.id_) != this->gateway_servers_.end()) {
      std::cout << "gateway server has repeat id[" << information.id_ << "]." << std::endl;
      return false;
    }

    TiXmlElement *user_pool_node = node_node->FirstChildElement("user_pool");
    if(user_pool_node == NULL) {
      std::cout << "Can't get node [gateway_server-node-user_pool], file("
        << file << ")." << std::endl;
      return false;
    }

    if(user_pool_node->Attribute("initial_number", (int *)(&information.user_pool_initial_)) == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-user_pool-initial_number], file("
        << file << ")." << std::endl;
      return false;
    }

    if(user_pool_node->Attribute("extend_number", (int *)(&information.user_pool_extend_)) == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-user_pool-extend_number], file("
        << file << ")." << std::endl;
      return false;
    }

    TiXmlElement *max_user_node = node_node->FirstChildElement("max_user");
    if(max_user_node == NULL) {
      std::cout << "Can't get node [gateway_server-node-max_user], file("
        << file << ")." << std::endl;
      return false;
    }

    if(max_user_node->Attribute("value", (int *)(&information.max_user_number_)) == NULL) {
      std::cout << "Can't get attribute [gateway_server-node-max_user-value], file("
        << file << ")."  << std::endl;
      return false;
    }

    if(this->gateway_servers_.find(information.id_) != this->gateway_servers_.end()) {
      std::cout << "gateway server has repeat id[" << information.id_ << "]." << std::endl;
      return false;
    }

    this->gateway_servers_.insert(std::make_pair(information.id_, information));

    node_node = node_node->NextSiblingElement("node");
  }

  return true;
}

bool ServerConfigure::LoadScheduleServer(TiXmlElement *configure_node, const std::string &file) {
  const char *str = NULL;

  // Schedule node.
  TiXmlElement *schedule_node = configure_node->FirstChildElement("schedule_server");
  if(schedule_node == NULL) {
    std::cout << "Can't get node [configure-schedule_server], file("
      << file << ")." << std::endl;
    return false;
  }

  // Host node.
  TiXmlElement *host_node = schedule_node->FirstChildElement("host");
  if(host_node == NULL) {
    std::cout << "Can't get node [schedule_server-node-host], file("
      << file << ")." << std::endl;
    return false;
  }

  str = host_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [schedule_server-node-host-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->schedule_server_.ip_ = str;

  if(host_node->Attribute("port", (int *)(&this->schedule_server_.port_)) == NULL) {
    std::cout << "Can't get attribute [schedule_server-node-host-port], file("
      << file << ")." << std::endl;
    return false;
  }

  // Log node.
  TiXmlElement *log_node = schedule_node->FirstChildElement("log");
  if(log_node == NULL) {
    std::cout << "Can't get node [schedule_server-node-log], file("
      << file << ")." << std::endl;
    return false;
  }

  str = log_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [schedule_server-node-log-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->schedule_server_.log_ = str;

  // Core node.
  TiXmlElement *core_node = schedule_node->FirstChildElement("core");
  if(core_node == NULL) {
    std::cout << "Can't get node [schedule_server-node-core], file("
      << file << ")." << std::endl;
    return false;
  }

  str = core_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [schedule_server-node-core-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->schedule_server_.core_ = str;

  // Coresh node.
  TiXmlElement *coresh_node = schedule_node->FirstChildElement("coresh");
  if(coresh_node == NULL) {
    std::cout << "Can't get node [schedule_server-node-coresh], file("
      << file << ")." << std::endl;
    return false;
  }

  str = coresh_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [schedule_server-node-coresh-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->schedule_server_.coresh_ = str;

  return true;
}

bool ServerConfigure::LoadSessionServer(TiXmlElement *configure_node, const std::string &file) {
  const char *str = NULL;

  // Session node.
  TiXmlElement *session_node = configure_node->FirstChildElement("session_server");
  if(session_node == NULL) {
    std::cout << "Can't get node [configure-session_server], file("
      << file << ")." << std::endl;
    return false;
  }

  // Host node.
  TiXmlElement *host_node = session_node->FirstChildElement("host");
  if(host_node == NULL) {
    std::cout << "Can't get node [session_server-host], file("
      << file << ")." << std::endl;
    return false;
  }

  str = host_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [session_server-host-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->session_server_.ip_ = str;

  if(host_node->Attribute("port", (int *)(&this->session_server_.port_)) == NULL) {
    std::cout << "Can't get attribute [session_server-host-port], file("
      << file << ")." << std::endl;
    return false;
  }

  // Admin host node.
  TiXmlElement *admin_host_node = session_node->FirstChildElement("admin_host");
  if(admin_host_node == NULL) {
    std::cout << "Can't get node [session_server-admin_host], file("
      << file << ")." << std::endl;
    return false;
  }

  str = admin_host_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [session_server-admin_host-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->session_server_.admin_ip_ = str;

  if(admin_host_node->Attribute("port", (int *)(&this->session_server_.admin_port_)) == NULL) {
    std::cout << "Can't get attribute [session_server-admin_host-port], file("
      << file << ")." << std::endl;
    return false;
  }

  // Log node.
  TiXmlElement *log_node = session_node->FirstChildElement("log");
  if(log_node == NULL) {
    std::cout << "Can't get node [session_server-log], file("
      << file << ")." << std::endl;
    return false;
  }

  str = log_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [session_server-log-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->session_server_.log_ = str;

  // Core node.
  TiXmlElement *core_node = session_node->FirstChildElement("core");
  if(core_node == NULL) {
    std::cout << "Can't get node [session_server-core], file("
      << file << ")." << std::endl;
    return false;
  }

  str = core_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [session_server-core-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->session_server_.core_ = str;

  // Coresh node.
  TiXmlElement *coresh_node = session_node->FirstChildElement("coresh");
  if(coresh_node == NULL) {
    std::cout << "Can't get node [session_server-coresh], file("
      << file << ")." << std::endl;
    return false;
  }

  str = coresh_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [session_server-coresh-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->session_server_.coresh_ = str;

  TiXmlElement *user_pool_node = session_node->FirstChildElement("user_pool");
  if(user_pool_node == NULL) {
    std::cout << "Can't get node [session_server-user_pool], file("
      << file << ")." << std::endl;
    return false;
  }

  if(user_pool_node->Attribute("initial_number",
        (int *)(&this->session_server_.user_pool_initial_)) == NULL) {
    std::cout << "Can't get attribute [session_server-user_pool-initial_number], file("
      << file << ")." << std::endl;
    return false;
  }

  if(user_pool_node->Attribute("extend_number",
        (int *)(&this->session_server_.user_pool_extend_)) == NULL) {
    std::cout << "Can't get attribute [session_server-user_pool-extend_number], file("
      << file << ")." << std::endl;
    return false;
  }

  TiXmlElement *max_user_node = session_node->FirstChildElement("max_user");
  if(max_user_node == NULL) {
    std::cout << "Can't get node [session_server-max_user], file("
      << file << ")." << std::endl;
    return false;
  }

  if(max_user_node->Attribute("value",
        (int *)(&this->session_server_.max_user_number_)) == NULL) {
    std::cout << "Can't get attribute [session_server-max_user-value], file("
      << file << ")."  << std::endl;
    return false;
  }

  return true;
}

}  // namespace global

