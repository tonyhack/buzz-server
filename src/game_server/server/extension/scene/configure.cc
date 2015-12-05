//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 17:48:58.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/scene/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace scene {

MapInfo::MapInfo() {}
MapInfo::~MapInfo() {}

const core::uint32 Configure::kGatherDistance_ = 5;

bool MapInfo::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) [地图信息配置] 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 map 结点标签
  TiXmlElement *map_node = doc.FirstChildElement("map");
  if(map_node == NULL) {
    global::LogError("%s:%d (%s) [地图信息配置] 找不到结点[map]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(map_node->Attribute("morning",(int*)(&this->daynight_.morning_interval_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-morning]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("daytime",(int*)(&this->daynight_.daytime_interval_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-daytime]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("evening",(int*)(&this->daynight_.evening_interval_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-evening]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("night",(int*)(&this->daynight_.night_interval_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-night]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("morning_aoi",(int*)(&this->daynight_.morning_aoi_coe_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-morning_aoi]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("daytime_aoi",(int*)(&this->daynight_.daytime_aoi_coe_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-daytime_aoi]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("evening_aoi",(int*)(&this->daynight_.evening_aoi_coe_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-evening_aoi]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("night_aoi",(int*)(&this->daynight_.night_aoi_coe_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[daynight-night_aoi]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("rookie_protect",(int*)(&this->rookie_protect_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[rookie_protect]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("ban_change_pk_mode",(int*)(&this->ban_change_pk_mode_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[ban_change_pk_mode]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("die_protect",(int*)(&this->die_protect_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[die_protect_]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("punish_for_pk",(int*)(&this->punish_for_pk_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[punish_for_pk_]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("revive_stay",(int*)(&this->revive_stay_)) == NULL) { 
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive_stay_]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;    
  }

  if(map_node->Attribute("zazen_add", (int *)(&this->zazen_coe_add_)) == NULL) {
    LOG_ERROR("找不到属性[zazen_add], 文件(%s)", file.c_str());
    return false;
  }

  int divide_screen = 0;
  this->divide_screen_ = false;
  if (map_node->Attribute("divide_screen", &divide_screen) == NULL || divide_screen != 0) {
    this->divide_screen_ = true;
  }

  // 加载 revive 结点标签
  TiXmlElement *revive_node = map_node->FirstChildElement("revive");
  while(revive_node) {
    ReviveAreaInfo revive;
    if(revive_node->Attribute("camp", (int *)(&revive.camp_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive-camp]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(this->revive_area_.find(revive.camp_) != this->revive_area_.end()) {
      LOG_ERROR("配置错误[%d]",revive.camp_);
      return false;
    }
    if(revive_node->Attribute("duration", (int *)(&revive.duration_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive-duration]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(revive_node->Attribute("width", (int *)(&revive.width_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive-width]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(revive_node->Attribute("height", (int *)(&revive.heigth_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive-height]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(revive_node->Attribute("x", (int *)(&revive.x_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive-x]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(revive_node->Attribute("y", (int *)(&revive.y_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive-y]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(revive_node->Attribute("map", (int *)(&revive.map_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[revive-map]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    this->revive_area_.insert(ReviveAreaMap::value_type(revive.camp_, revive));
    revive_node = revive_node->NextSiblingElement("revive");
  }

  // 加载 default 结点标签
  TiXmlElement *default_node = map_node->FirstChildElement("default");
  if(default_node  == NULL) {
    LOG_ERROR("本地图[%s]没有默认点", file.c_str());
    return false;
  }
  if(default_node->Attribute("x", (int *)(&this->default_area_.x_)) == NULL) {
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[default-camp]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(default_node->Attribute("y", (int *)(&this->default_area_.y_)) == NULL) {
    global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[default-duration]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  NpcInfo info;

  // 加载 npc 结点标签
  TiXmlElement *npc_node = map_node->FirstChildElement("npc");
  while(npc_node) {
    if(npc_node->Attribute("id", (int *)(&info.npc_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[npc-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    /*
    if(this->npcs_.find(id) != this->npcs_.end()) {
      global::LogError("%s:%d (%s) [地图信息配置] npc[%u]配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }
    */
    if(npc_node->Attribute("width", (int *)(&info.width_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[npc-width]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(npc_node->Attribute("height", (int *)(&info.heigth_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[npc-height]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(npc_node->Attribute("x", (int *)(&info.x_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[npc-x]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(npc_node->Attribute("y", (int *)(&info.y_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[npc-y]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(npc_node->Attribute("num", (int *)(&info.number_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[npc-num]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(npc_node->Attribute("reborn", (int *)(&info.reborn_time_)) == NULL) {
      global::LogError("%s:%d (%s) [地图信息配置] 找不到属性[npc-reborn]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    core::int32 temp_dir = entity::DirectionType::MAX;
    if(npc_node->Attribute("dir", (int *)(&temp_dir)) == NULL) {
      LOG_ERROR("[地图信息配置] 找不到属性[npc-dir], 文件[%s]", file.c_str());
      return false; 
    }
    if(temp_dir >= entity::DirectionType::MAX || temp_dir < entity::DirectionType::MIN) {
      LOG_ERROR("[地图信息配置] npc-dir 属性错误[%d]", temp_dir);
      return false;
    }
    info.dir_ = (entity::DirectionType::type)temp_dir;

    this->npcs_.push_back(info);
    npc_node = npc_node->NextSiblingElement("npc");
  }

  return true;
}

ServerMapConfigure::ServerMapConfigure() {}
ServerMapConfigure::~ServerMapConfigure() {}

bool ServerMapConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) [场景配置] 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点标签
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到结点[configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 map_block 结点标签
  TiXmlElement *map_block_node = configure_node->FirstChildElement("map_block");
  if(map_block_node == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到结点[configure-map_block]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 地图障碍配置文件
  const char *str = map_block_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到属性[map_block-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  this->map_block_file_ = global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + str;

  // 加载 server 结点标签
  TiXmlElement *server_node = configure_node->FirstChildElement("server");
  while(server_node) {
    // 服务器ID
    core::uint32 server = 0;
    if(server_node->Attribute("id", (int *)(&server)) == NULL) {
      global::LogError("%s:%d (%s) [场景配置] 找不到属性[server-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 服务器ID是否重复
    ServerMapMap::const_iterator iterator = this->maps_.find(server);
    if(iterator != this->maps_.find(server)) {
      global::LogError("%s:%d (%s) [场景配置] 服务器配置(%s)重复，server=%d",
          __FILE__, __LINE__, __FUNCTION__, file.c_str(), server);
      return false;
    }
    MapSet maps;
    // 加载 map 结点标签
    TiXmlElement *map_node = server_node->FirstChildElement("map");
    while(map_node) {
      // 地图ID
      core::uint32 map = 0;
      if(map_node->Attribute("id", (int *)(&map)) == NULL) {
        global::LogError("%s:%d (%s) [场景配置] 找不到属性[server-map-id]，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, file.c_str());
        return false;
      }
      // 地图ID是否重复
      MapSet::const_iterator iterator = maps.find(map);
      if(iterator != maps.end()) {
        global::LogError("%s:%d (%s) [场景配置] 地图配置(%s)重复，server=%d, map=%d",
            __FILE__, __LINE__, __FUNCTION__, file.c_str(), server, map);
        return false;
      }
      maps.insert(map);
      map_node = map_node->NextSiblingElement("map");
    }
    this->maps_.insert(std::make_pair(server, maps));
    server_node = server_node->NextSiblingElement("server");
  }

  {
    // 加载 mapinfo 结点标签
    TiXmlElement *mapinfo_node = configure_node->FirstChildElement("mapinfo");
    if(mapinfo_node == NULL) {
      global::LogError("%s:%d (%s) [场景配置] 找不到结点[configure-mapinfo]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    // 加载 map 结点
    TiXmlElement *map_node = mapinfo_node->FirstChildElement("map");
    while(map_node) {
      core::uint32 map = 0;
      if(map_node->Attribute("id", (int *)(&map)) == NULL) {
        global::LogError("%s:%d (%s) [场景配置] 找不到属性[mapinfo-map-id]，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, file.c_str());
        return false;
      }
      if(this->map_infos_.find(map) != this->map_infos_.end()) {
        global::LogError("%s:%d (%s) [场景配置] 地图信息[%u]配置重复，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, map, file.c_str());
        return false;
      }
      const char *str = map_node->Attribute("file");
      if(str == NULL) {
        global::LogError("%s:%d (%s) [场景配置] 找不到属性[mapinfo-map-file]，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, file.c_str());
        return false;
      }
      MapInfo info;
      if(info.LoadConfigure(global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
            str) == false) {
        global::LogError("%s:%d (%s) [场景配置] 加载地图信息配置文件[%s]失败，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, str, file.c_str());
        return false;
      }
      this->map_infos_.insert(std::make_pair(map, info));
      map_node = map_node->NextSiblingElement("map");
    }
  }

  return true;
}

MapBlockConfigure::MapBlockConfigure() {}
MapBlockConfigure::~MapBlockConfigure() {}

bool MapBlockConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    std::cout << "[地图配置] 加载配置文件(" << file << ")出错" << std::endl;
    return false;
  }
  // package node.
  TiXmlElement *package_node = doc.FirstChildElement("package");
  if(package_node == NULL) {
    std::cout << "[地图配置] 找不到结点[package], 文件("
      << file << ")" << std::endl;
    return false;
  }

  core::uint32 map = 0;
  const char *str = NULL;

  // map node.
  TiXmlElement *map_node = package_node->FirstChildElement("map");
  while(map_node) {
    if(map_node->Attribute("id", (int *)(&map)) == NULL) {
      std::cout << "[地图配置] 找不到属性 [map-id], 文件("
        << file << ")." << std::endl;
      return false;
    }
    if(this->map_blocks_.find(map) != this->map_blocks_.end()) {
      std::cout << "[地图配置] 地图ID(" << map << ")重复, 文件("
        << file << ")" << std::endl;
      return false;
    }
    str = map_node->Attribute("resource");
    if(str == NULL) {
      std::cout << "[地图配置] 找不到属性 [map-resource], 文件("
        << file << ")." << std::endl;
      return false;
    }

    if(this->Load(map, global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          str) == false) {
      std::cout << "[地图配置] 加载配置文件(" << str << ")失败"
        << std::endl;
      return false;
    }

    map_node = map_node->NextSiblingElement("map");
  }

  return true;
}

bool MapBlockConfigure::Load(core::uint32 map, const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    std::cout << "[地图配置] 加载配置文件(" << file << ")出错" << std::endl;
    return false;
  }
  // map node.
  TiXmlElement *map_node = doc.FirstChildElement("map");
  if(map_node == NULL) {
    std::cout << "[地图配置] 找不到结点[map], 文件("
      << file << ")" << std::endl;
    return false;
  }

  MapBlock configure;
/*
  if(map_node->Attribute("id", (int *)(&configure.id_)) == NULL) {
    std::cout << "[地图配置] 找不到属性 [map-id], 文件("
      << file << ")." << std::endl;
    return false;
  }

  if(map != configure.id_) {
    std::cout << "[地图配置] 地图id不一致, 文件("
      << file << ")." << std::endl;
    return false;
  }
*/
  int width = 0, height = 0;
  if(map_node->Attribute("width", (int *)(&width)) == NULL) {
    std::cout << "[地图配置] 找不到属性 [map-width]，文件("
      << file << ")." << std::endl;
    return false;
  }
  if(map_node->Attribute("height", (int *)(&height)) == NULL) {
    std::cout << "[地图配置] 找不到属性 [map-height]，文件("
      << file << ")." << std::endl;
    return false;
  }

  if(map_node->Attribute("gridW", (int *)(&configure.max_x_)) == NULL) {
    std::cout << "[地图配置] 找不到属性 [map-gridW], 文件("
      << file << ")." << std::endl;
    return false;
  }
  if(map_node->Attribute("gridH", (int *)(&configure.max_y_)) == NULL) {
    std::cout << "[地图配置] 找不到属性 [map-gridH], 文件("
      << file << ")." << std::endl;
    return false;
  }

  if(configure.max_x_ == 0 || configure.max_y_ == 0) {
    std::cout << "[地图配置] 属性 [map-gridH/gridH] 为 0, 文件("
      << file << ")." << std::endl;
    return false;
  }

  configure.max_x_ = width / configure.max_x_;
  configure.max_y_ = height / configure.max_y_;

  // grids node.
  TiXmlElement *grids_node = map_node->FirstChildElement("grids");
  if(grids_node == NULL) {
    std::cout << "[地图配置] 找不到结点[map-grids], 文件{"
      << file << ")" << std::endl;
    return false;
  }

  GridBlock block;

  // grid node.
  TiXmlElement *grid_node = grids_node->FirstChildElement("grid");
  while(grid_node) {
    /*
    if(grid_node->Attribute("x", (int *)(&block.x_)) == NULL) {
      std::cout << "[地图配置] 找不到属性 [map-grids-grid-x], 文件("
        << file << ")." << std::endl;
      return false;
    }
    if(grid_node->Attribute("y", (int *)(&block.y_)) == NULL) {
      std::cout << "[地图配置] 找不到属性 [map-grids-grid-y], 文件("
        << file << ")." << std::endl;
      return false;
    }
    */
    // 找到点集
    const char *position_set_str = grid_node->Attribute("contain");
    if(position_set_str == NULL) {
      std::cout << "[地图配置] 找不到属性 [map-grids-grid-contain], 文件("
        << file << ")." << std::endl;
      return false;
    }
    if(grid_node->Attribute("attribute", (int *)(&block.block_)) == NULL) {
      std::cout << "[地图配置] 找不到属性 [map-grids-grid-attribute], 文件("
        << file << ")." << std::endl;
      return false;
    }

    std::vector<std::string> pair;
    std::vector<core::int32> position;
    global::StringSplit(position_set_str, ",", pair);
    std::vector<std::string>::iterator iterator = pair.begin();
    for(; iterator != pair.end(); ++iterator) {
      global::StringSplit(iterator->c_str(), "_", position);
      if(position.size() != 2) {
        std::cout << "[地图配置] 属性 [map-grids-grid-contain] 错误, 文件("
          << file << ")." << std::endl;
        return false;
      }
      block.x_ = position[0];
      block.y_ = position[1];
      configure.blocks_.push_back(block);
    }

    grid_node = grid_node->NextSiblingElement("grid");
  }

  this->map_blocks_.insert(std::make_pair(map, configure));

  return true;
}

const MapBlock *MapBlockConfigure::GetMapBlock(core::uint32 map) const {
  MapBlockMap::const_iterator iterator = this->map_blocks_.find(map);
  if(iterator == this->map_blocks_.end()) {
    return NULL;
  }
  return &iterator->second;
}

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) [场景配置] 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点标签
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到结点[configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 gather 结点标签
  TiXmlElement *gather_node = configure_node->FirstChildElement("gather");
  if(gather_node == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到结点[configure-gather]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  const char *str = gather_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到属性[gather-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_gather_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + str) == false) {
    global::LogError("%s:%d (%s) [场景配置] 加载采集失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 world_jump 结点标签
  TiXmlElement *world_node = configure_node->FirstChildElement("world_jump");
  if(world_node == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到结点[configure-world_jump]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = world_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到属性[world_jump-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_world_jump_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + str) == false) {
    global::LogError("%s:%d (%s) [场景配置] 加载世界地图跳转失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 jump 结点标签
  TiXmlElement *jump_node = configure_node->FirstChildElement("jump");
  if(jump_node == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到结点[configure-jump]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = jump_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) [场景配置] 找不到属性[jump-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->jump_configure_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + str) == false) {
    global::LogError("%s:%d (%s) [场景配置] 加载跳转配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 consecutive_kill 节点标签
  TiXmlElement *consecutive_kill_node = configure_node->FirstChildElement("consecutive_kill");
  if(consecutive_kill_node == NULL) {
    LOG_ERROR("加载 consecutive_kill 配置失败");
    return false;
  }

  str = consecutive_kill_node->Attribute("file");
  if(str == NULL) {
    LOG_ERROR("consecutive_kill 配置找不到属性 file");
    return false;
  }
  if(this->consecutive_kill_configure_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + str) == false) {
    LOG_ERROR("加载文件(%s)失败", str);
    return false;
  }

  if(this->server_maps_.LoadConfigure(file) == false) {
    global::LogError("%s:%d (%s) [场景配置] 加载服务器地图配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->map_blocks_.LoadConfigure(this->server_maps_.map_block_file_) == false) {
    global::LogError("%s:%d (%s) [场景配置] 加载地图障碍配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  return true;
}

}  // namespace scene

}  // namespace server

}  // namespace game

