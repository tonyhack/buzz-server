//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 18:14:58.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "test/chat/client/configure.h"

#include <iostream>

#include "core/tinyxml/tinyxml.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "coresh/variable_table.h"

namespace test {

namespace chat {

namespace client {

Configure::Configure() {}
Configure::~Configure() {}

bool Configure::Load(const char *file) {
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

  // Server node.
  TiXmlElement *server_node = configure_node->FirstChildElement("server");
  if(server_node == NULL) {
    std::cout << "Can't get node [server], file(" << file << ")." << std::endl;
    return false;
  }

  str = server_node->Attribute("host");
  if(str == NULL) {
    std::cout << "Can't get attribute [configure-server-host], file("
      << file << ")." << std::endl;
    return false;
  }
  this->host_ = str;

  if(server_node->Attribute("port", (int *)(&this->port_)) == NULL) {
    std::cout << "Can't get attribute [configure-server-port], file("
      << file << ")." << std::endl;
    return false;
  }

  if(server_node->Attribute("thread_number", (int *)(&this->thread_number_)) == NULL) {
    std::cout << "Can't get attribute [configure-server-thread_number], file("
      << file << ")." << std::endl;
    return false;
  }

  str = server_node->Attribute("log_dir");
  if(str == NULL) {
    std::cout << "Can't get attribute [configure-server-log_dir], file("
      << file << ")." << std::endl;
    return false;
  }
  this->log_dir_ = str;

  str = server_node->Attribute("core_configure");
  if(str == NULL) {
    std::cout << "Can't get attribute [configure-server-core_configure], file("
      << file << ")." << std::endl;
    return false;
  }
  this->core_configure_ = str;
  coresh::VariableTableSingleton::GetInstance().SetCoreConfigure(this->core_configure_);


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

}  // client

}  // chat

}  // test

