//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 15:14:19.
// File name: server_configure.cc
//
// Description:
// Define class ServerConfigure.
//

#include "login_server_informal/server/server_configure.h"

#include <iostream>

#include "core/tinyxml/tinyxml.h"

namespace login {

namespace server {

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

  // Mysql node.
  TiXmlElement *mysql_node = configure_node->FirstChildElement("mysql");
  if(mysql_node == NULL) {
    std::cout << "Can't get node [configure-mysql], file(" << file << ")." << std::endl;
    return false;
  }

  // Zone node.
  TiXmlElement *zone_node = mysql_node->FirstChildElement("zone");
  while(zone_node) {
    int zone = 0;
    if(zone_node->Attribute("id", &zone) == NULL) {
      std::cout << "Can't get attribute [mysql-zone-id], file("
        << file << ")." << std::endl;
      return false;
    }

    if(this->database_.databases_.find(zone) !=
        this->database_.databases_.end()) {
      std::cout << "database configure has repetitive zone id["
        << zone << "], file(" << file << ")." << std::endl;
      return false;
    }

    GameDatabaseInformation::DatabaseVector zone_dbs;
    // Node node.
    TiXmlElement *node_node = zone_node->FirstChildElement("node");
    while(node_node) {
      DatabaseInformation information;

      const char *str = node_node->Attribute("ip");
      if(str == NULL) {
        std::cout << "Can't get attribute [mysql-zone-node-ip], file("
          << file << ")." << std::endl;
        return false;
      }
      information.ip_ = str;

      if(node_node->Attribute("port", (int *)(&information.port_)) == NULL) {
        std::cout << "Can't get attribute [mysql-zone-node-port], file("
          << file << ")." << std::endl;
        return false;
      }

      str = node_node->Attribute("user");
      if(str == NULL) {
        std::cout << "Can't get attribute [mysql-zone-node-user], file("
          << file << ")." << std::endl;
        return false;
      }
      information.user_ = str;

      str = node_node->Attribute("passwd");
      if(str == NULL) {
        std::cout << "Can't get attribute [mysql-zone-node-passwd], file("
          << file << ")." << std::endl;
        return false;
      }
      information.passwd_ = str;

      str = node_node->Attribute("db");
      if(str == NULL) {
        std::cout << "Can't get attribute [mysql-zone-node-db], file("
          << file << ")." << std::endl;
        return false;
      }
      information.db_ = str;

      zone_dbs.push_back(information);

      node_node = node_node->NextSiblingElement("node");
    }
    this->database_.databases_.insert(std::make_pair(zone, zone_dbs));

    zone_node = zone_node->NextSiblingElement("zone");
  }

  // center node.
  TiXmlElement *center_node = mysql_node->FirstChildElement("center");
  if(center_node == NULL) {
    std::cout << "Can't get attribute [mysql-center], file("
      << file << ")." << std::endl;
    return false;
  }

  const char *str = center_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [mysql-center-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->database_.center_database_.ip_ = str;

  if(center_node->Attribute("port",
        (int *)(&this->database_.center_database_.port_)) == NULL) {
    std::cout << "Can't get attribute [mysql-center-port], file("
      << file << ")." << std::endl;
    return false;
  }

  str = center_node->Attribute("user");
  if(str == NULL) {
    std::cout << "Can't get attribute [mysql-center-user], file("
      << file << ")." << std::endl;
    return false;
  }
  this->database_.center_database_.user_ = str;

  str = center_node->Attribute("passwd");
  if(str == NULL) {
    std::cout << "Can't get attribute [mysql-center-passwd], file("
      << file << ")." << std::endl;
    return false;
  }
  this->database_.center_database_.passwd_ = str;

  str = center_node->Attribute("db");
  if(str == NULL) {
    std::cout << "Can't get attribute [mysql-center-db], file("
      << file << ")." << std::endl;
    return false;
  }
  this->database_.center_database_.db_ = str;

  // Procedure node.
  TiXmlElement *procedure_node = mysql_node->FirstChildElement("procedure");
  while(procedure_node) {
    int type = 0;
    if(procedure_node->Attribute("type", (int *)(&type)) == NULL) {
      std::cout << "Can't get attribute [mysql-procedure-type], file("
        << file << ")." << std::endl;
      return false;
    }
    str = procedure_node->Attribute("name");
    if(str == NULL) {
      std::cout << "Can't get attribute [mysql-procedure-name], file("
        << file << ")." << std::endl;
      return false;
    }
    if(this->database_.InsertProcedure(type, str) == false) {
      std::cout << "Procedure [type=" << type << ", name=" << str
        << "] repeat, file(" << file << ")." << std::endl;
      return false;
    }
    procedure_node = procedure_node->NextSiblingElement("procedure");
  }

  // Login node.
  TiXmlElement *login_node = configure_node->FirstChildElement("login_server");
  if(login_node == NULL) {
    std::cout << "Can't get node [configure-login_server], file("
        << file << ")." << std::endl;
    return false;
  }

  // Host node.
  TiXmlElement *host_node = login_node->FirstChildElement("host");
  if(host_node == NULL) {
    std::cout << "Can't get node [login_server-host], file("
      << file << ")." << std::endl;
    return false;
  }

  str = host_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [login_server-host-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->login_.ip_ = str;

  if(host_node->Attribute("port", (int *)(&this->login_.port_)) == NULL) {
    std::cout << "Can't get attribute [login_server-host-port], file("
      << file << ")." << std::endl;
    return false;
  }

  // Gateway node.
  TiXmlElement *gateway_node = login_node->FirstChildElement("gateway");
  if(gateway_node == NULL) {
    std::cout << "Can't get node [login_server-gateway], file("
      << file << ")." << std::endl;
    return false;
  }

  str = gateway_node->Attribute("ip");
  if(str == NULL) {
    std::cout << "Can't get attribute [login_server-gateway-ip], file("
      << file << ")." << std::endl;
    return false;
  }
  this->gateway_.ip_ = str;

  if(gateway_node->Attribute("port", (int *)(&this->gateway_.port_)) == NULL) {
    std::cout << "Can't get attribute [login_server-gateway-port], file("
      << file << ")." << std::endl;
    return false;
  }

  // Log node.
  TiXmlElement *log_node = login_node->FirstChildElement("log");
  if(log_node == NULL) {
    std::cout << "Can't get node [login_server-log], file("
      << file << ")." << std::endl;
    return false;
  }

  str = log_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [login_server-log-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->log_ = str;

  // Core node.
  TiXmlElement *core_node = login_node->FirstChildElement("core");
  if(core_node == NULL) {
    std::cout << "Can't get node [login_server-core], file("
      << file << ")." << std::endl;
    return false;
  }

  str = core_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [login_server-core-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->core_ = str;

  // Dirty word node.
  TiXmlElement *dirty_word_node = login_node->FirstChildElement("dirty_word");
  if(dirty_word_node == NULL) {
    std::cout << "Can't get node [login_server-dirty], file("
      << file << ")." << std::endl;
    return false;
  }

  str = dirty_word_node->Attribute("value");
  if(str == NULL) {
    std::cout << "Can't get attribute [login_server-dirty-value], file("
      << file << ")." << std::endl;
    return false;
  }
  this->dirty_word_file_ = str;

  return true;
}

}  // namespace server

}  // namespace login

