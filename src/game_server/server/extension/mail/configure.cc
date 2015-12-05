//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-03 17:47:53.
// File name: load_mail_configure.cc
//
// Description:
//

#include "game_server/server/extension/mail/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace mail {

Configure::Configure() {}
Configure::~Configure() {}

bool Configure::LoadMailConfigure(const std::string &file) {
  TiXmlElement *data_node = NULL;
  TiXmlElement *cell_node = NULL; 

  // 加载 configure.xml
  TiXmlDocument mail_doc;
  if (mail_doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed", file.c_str());
    return false;
  }
  // --加载节点 data 
  data_node = mail_doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", file.c_str());
    return false;
  }

  cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    MailCell cell;

    if (cell_node->Attribute("mail_type", (int *)&cell.mail_type_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("expiry_time", (int *)&cell.expiry_time_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }

    if (cell_node->Attribute("context_max", (int *)&cell.context_max_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[exp] not found.", file.c_str());
      return false;
    }
    mails_.push_back(cell);
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool Configure::LoadConfigure(const std::string & file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s)出错",
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

  // 加载 mail_pool 结点
  TiXmlElement *mail_pool_node = configure_node->FirstChildElement("mail_pool");
  if(mail_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-item_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(mail_pool_node->Attribute("initial_number",
        (int *)(&this->mail_init_num_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [item_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(mail_pool_node->Attribute("extend_number",
        (int *)(&this->mail_extend_num_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [item_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 mail_configure 结点
  TiXmlElement *mail_node = configure_node->FirstChildElement("mail_configure");
  if(mail_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [mail_configure，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *str = mail_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [mail_configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(LoadMailConfigure(global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
       str) == false) {
    LOG_ERROR("加载 邮件配置失败!");
    return false;
  }

  return true;
}

const MailCell *Configure::GetMailCell(entity::MailType::type type) const {
  if(entity::MailType::GENERAL >= type && entity::MailType::SYSTEM <= type
          && entity::MailType::GENERAL == mails_.size()) {
    return &mails_[(size_t)type - 1];
  }
  return NULL;
}

core::int32 Configure::GetInitSize() const {
  return mail_init_num_;
}

core::int32 Configure::GetExtendSize() const {
 return mail_extend_num_;
}

} // game

} // server

} // game

