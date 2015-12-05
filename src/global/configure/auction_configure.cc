#include "global/configure/auction_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace global {

namespace configure {

AuctionConfigure::AuctionConfigure() {}
AuctionConfigure::~AuctionConfigure() {}

bool AuctionConfigure::LoadConfigure(const std::string &file) {
  const std::string &config_dir =
      global::ServerConfigureSingleton::GetInstance().GetConfigueDir();
  const char *node_attr = NULL;

  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 configure
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if (NULL == configure_node) {
    LOG_ERROR("Config file (%s) /configure not found.", file.c_str());
    return false;
  }

  // 加载节点 auction_time
  TiXmlElement *auction_time_node = configure_node->FirstChildElement("auction_time");
  if (NULL == auction_time_node) {
    LOG_ERROR("Config file (%s) /configure/auction_time not found.", file.c_str());
    return false;
  }
  node_attr = auction_time_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/auction_time[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadAuctionTime(config_dir + node_attr) == false) {
    LOG_ERROR("Load auction time config file failed.");
    return false;
  }

  return true;
}

bool AuctionConfigure::LoadAuctionTime(const std::string &file) {
  TiXmlDocument doc;

  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 data
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", file.c_str());
    return false;
  }

  // 加载节点集合 cell
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    AuctionTimeCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("time", (int *)&cell.time_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[time] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("fee", (int *)&cell.fee_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[fee] not found.", file.c_str());
      return false;
    }

    this->auction_times_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const AuctionTimeCell *AuctionConfigure::GetAuctionTime(core::int32 id) const {
  AuctionTimeHashmap::const_iterator iter = this->auction_times_.find(id);
  if (iter == this->auction_times_.end()) {
    return NULL;
  }
  return &iter->second;
}

}  // namespace configure

}  // namespace global

