//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 15:00:21.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "test/chat/pressure_client/configure.h"

#include <iostream>

#include "core/tinyxml/tinyxml.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "coresh/variable_table.h"

namespace test {

namespace chat {

namespace pressure {

Configure ConfigureSingleton::instance_;

Configure::Configure() : thread_number_(0), chat_frequency_(0) {}
Configure::~Configure() {}

bool Configure::LoadFile(const char *file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file) == false) {
    std::cout << "Load file(" << file << ") error." << std::endl;
    return false;
  }

  const char *str = NULL;

  // Configure node.
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    std::cout << "Can't get node [configure], file(" << file << ")." << std::endl;
    return false;
  }

  // Pressure node.
  TiXmlElement *pressure_node = configure_node->FirstChildElement("pressure");
  if(pressure_node == NULL) {
    std::cout << "Can't get node [pressure], file(" << file << ")." << std::endl;
    return false;
  }

  str = pressure_node->Attribute("host");
  if(str == NULL) {
    std::cout << "Can't get attribute [configure-pressure-host], file("
      << file << ")." << std::endl;
    return false;
  }
  this->host_ = str;

  if(pressure_node->Attribute("port", (int *)(&this->port_)) == NULL) {
    std::cout << "Can't get attribute [configure-pressure-port], file("
      << file << ")." << std::endl;
    return false;
  }

  if(pressure_node->Attribute("thread_number", (int *)(&this->thread_number_)) == NULL) {
    std::cout << "Can't get attribute [configure-pressure-thread_number], file("
      << file << ")." << std::endl;
    return false;
  }

  str = pressure_node->Attribute("log_dir");
  if(str == NULL) {
    std::cout << "Can't get attribute [configure-pressure-log_dir], file("
      << file << ")." << std::endl;
    return false;
  }
  this->log_dir_ = str;

  str = pressure_node->Attribute("core_configure");
  if(str == NULL) {
    std::cout << "Can't get attribute [configure-pressure-core_configure], file("
      << file << ")." << std::endl;
    return false;
  }
  this->core_configure_ = str;
  coresh::VariableTableSingleton::GetInstance().SetCoreConfigure(this->core_configure_);


  // Chat node
  TiXmlElement *chat_node = configure_node->FirstChildElement("chat");
  if(chat_node == NULL) {
    std::cout << "Can't get node [chat], file(" << file << ")." << std::endl;
    return false;
  }

  if(chat_node->Attribute("chat_frequency", (int *)(&this->chat_frequency_)) == NULL) {
    std::cout << "Can't get attribute [configure-chat-chat_frequency]" << std::endl;
    return false;
  }

  TiXmlElement *node_node = chat_node->FirstChildElement("node");
  while(node_node) {
    str = node_node->Attribute("content");
    if(str == NULL) {
      std::cout << "Can't get attribute [configure-node-content]" << std::endl;
      return false;
    }
    this->chats_.push_back(std::string(str));

    node_node = node_node->NextSiblingElement("node");
  }


  // Coresh node.
  TiXmlElement *coresh_node = configure_node->FirstChildElement("coresh");
  if(coresh_node == NULL) {
    std::cout << "Can't get node [coresh], file(" << file << ")." << std::endl;
    return false;
  }

  size_t data = 0;

  if(coresh_node->Attribute("max_protocol_size", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-max_protocol_size], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetMaxProtocolSize(data);

  if(coresh_node->Attribute("max_message_size", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-max_message_size], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetMaxMessageSize(data);

  if(coresh_node->Attribute("message_queue_initial_size", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-message_queue_initial_size], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetMessageQueueInitialSize(data);

  if(coresh_node->Attribute("connection_pool_initial_number", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-connection_pool_initial_number], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetConnectionPoolInitialNumber(data);

  if(coresh_node->Attribute("connection_pool_extend_number", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-connection_pool_extend_number], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetConnectionPoolExtendNumber(data);

  if(coresh_node->Attribute("network_thread_number", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetNetworkThreadNumber(data);

  if(coresh_node->Attribute("compress_type", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-compress_type], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetCompressType((coresh::CompressType::Type)data);

  if(coresh_node->Attribute("encode_type", (int *)(&data)) == NULL) {
    std::cout << "Can't get attribute [configure-coresh-encode_type], file("
      << file << ")." << std::endl;
    return false;
  }
  coresh::VariableTableSingleton::GetInstance().SetEncodeType((coresh::EncodeType::Type)data);

  return true;
}

}  // namespace pressure

}  // namespace chat

}  // namespace test

