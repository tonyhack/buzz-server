//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:35:56.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "core/configure.h"

#include "core/base/logging.h"

namespace core {

pthread_once_t Configure::once_ = PTHREAD_ONCE_INIT;
std::string Configure::file_;
TCPConnectionConfig Configure::tcp_connection_;
ConnectorConfig Configure::connector_;
TimerConfig Configure::timer_;
Mutex Configure::mutex_;
bool Configure::loaded_ = false;

void Configure::LoadConfig() {
  if(Configure::file_.empty() == true) {
    CoreLog(ERROR, "%s:%d (%s) File name empty.",
           __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // Load Configure::file_.
  TiXmlDocument doc;
  if(doc.LoadFile(file_.c_str()) == false) {
    CoreLog(ERROR, "%s:%d (%s) Load Configure::file_(%s) error.",
           __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
    return ;
  }

  // config node.
  TiXmlElement *config_node = doc.FirstChildElement("config");
  if(config_node == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Can't get node [config], Configure::file_(%s).",
           __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
    return ;
  }

  // tcp_connection node.
  TiXmlElement *tcp_connection_node = config_node->FirstChildElement("tcp_connection");
  if(tcp_connection_node == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Can't get node [tcp_connection], Configure::file_(%s).",
           __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
    return ;

  } else {
    // buffer node.
    TiXmlElement *buffer_node = tcp_connection_node->FirstChildElement("buffer");
    if(buffer_node == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Can't get node [tcp_connection-buffer], Configure::file_(%s).",
             __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
      return ;
    }
    if(buffer_node->Attribute("unit_size", &Configure::tcp_connection_.buffer_unit_size_) == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Can't get attribute [tcp_connection-buffer-unit_size], Configure::file_(%s).",
             __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
      return ;
    }
    if(buffer_node->Attribute("init_size", &Configure::tcp_connection_.buffer_init_size_) == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Can't get attribute [tcp_connection-buffer-buffer_init_size], Configure::file_(%s).",
             __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
      return ;
    }

#ifdef _CORE_DEBUG
      CoreLog(INFO, "<buffer unit_size=\"%d\", init_size\"%d\"/>",
             Configure::tcp_connection_.buffer_unit_size_, Configure::tcp_connection_.buffer_init_size_);
#endif

    // bound_queue node.
    TiXmlElement *bound_queue_node = tcp_connection_node->FirstChildElement("bound_queue");
    if(bound_queue_node == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Can't get node [tcp_connection-bound_queue], Configure::file_(%s).",
             __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
      return ;
    }
    if(bound_queue_node->Attribute("size", &Configure::tcp_connection_.bound_queue_size_) == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Can't get attribute [tcp_connection-bound_queue_size], Configure::file_(%s).",
             __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
      return ;
    }

#ifdef _CORE_DEBUG
      CoreLog(INFO, "<bound_queue size=\"%d\"/>", Configure::tcp_connection_.bound_queue_size_);
#endif

  }

  // connector node.
  TiXmlElement *connector_node = config_node->FirstChildElement("connector");
  if(connector_node == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Can't get node [connector], Configure::file_(%s).",
           __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
    return;
  } else {
    if(connector_node->Attribute("retry_msec", &Configure::connector_.retry_msec_) == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Can't get attribute [connector_-retry_msec], Configure::file_(%s).",
             __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
      return ;
    }
#ifdef _CORE_DEBUG
    CoreLog(INFO, "<connector retry_msec=\"%d\"/>", Configure::connector_.retry_msec_);
#endif
  }

  // timing_wheel node.
  TiXmlElement *timing_wheel_node = config_node->FirstChildElement("timing_wheel");
  if(timing_wheel_node == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Can't get node [timing_wheel], Configure::file_(%s).",
           __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
    return ;
  } else {
    if(timing_wheel_node->Attribute("tick_msec", &Configure::timer_.unit_interval_msec_) == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Can't get attribute [timing_wheel-tick_msec], Configure::file_(%s).",
             __FILE__, __LINE__, __FUNCTION__, Configure::file_.c_str());
      return ;
    }
#ifdef _CORE_DEBUG
    CoreLog(INFO, "<timing_wheel tick_msec=\"%d\"/>", Configure::timer_.unit_interval_msec_);
#endif

  }

  Configure::loaded_ = true;
}

}  // namespace core
