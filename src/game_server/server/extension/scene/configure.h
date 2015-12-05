//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 17:26:22.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__SCENE__CONFIGURE__H
#define __GAME__SERVER__SCENE__CONFIGURE__H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "core/base/types.h"
#include "game_server/server/extension/scene/jump_configure.h"
#include "game_server/server/extension/scene/scene_configure.h"
#include "game_server/server/extension/scene/load_consecutive_kill_configure.h"
#include "game_server/server/extension/scene/load_gather_configure.h"
#include "game_server/server/extension/scene/load_world_jump_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class MapInfo {
 public:
  // typedef std::map<core::uint32, NpcInfo> NpcMap;
  typedef std::vector<NpcInfo> NpcVector;
  typedef std::map<core::int32, ReviveAreaInfo> ReviveAreaMap;

  MapInfo();
  ~MapInfo();

  bool LoadConfigure(const std::string &file);

  /*
  inline const NpcMap &GetNpcs() const {
    return this->npcs_;
  }
  */
  inline const NpcVector &GetNpcs() const {
    return this->npcs_;
  }
  inline const ReviveAreaInfo *GetReviveArea(core::int32 camp) const {
    ReviveAreaMap::const_iterator itr = this->revive_area_.find(camp);
    if(itr != this->revive_area_.end()) {
      return &itr->second;
    }
    itr = this->revive_area_.find(0);
    if(itr != this->revive_area_.end()) {
      return &itr->second;
    }
    return NULL;
  }
  inline const DefaultAreaInfo *GetDefaultArea() const {
    return &this->default_area_;
  }
  /*
  inline const NpcInfo *GetNpc(core::uint32 id) const {
    NpcMap::const_iterator iterator = this->npcs_.find(id);
    if(iterator != this->npcs_.end()) {
      return &iterator->second;
    }
    return NULL;
  }
  */
  inline const DayNightInfo &GetDayNight() const {
    return this->daynight_;
  }
  // Npc配置
  // NpcMap npcs_;
  NpcVector npcs_;
  // 复活区配置
  ReviveAreaMap revive_area_;
  // 默认点
  DefaultAreaInfo default_area_;
  // 昼夜配置
  DayNightInfo daynight_;

  // 是否新手保护
  bool rookie_protect_;
  // 是否禁止设置PK模式
  bool ban_change_pk_mode_;
  // 是否死亡保护
  bool die_protect_;
  // 是否PK惩罚
  bool punish_for_pk_;
  // 是否可以原地复活
  bool revive_stay_;
  // 是否需要分屏
  bool divide_screen_;
  // 打坐额外奖励
  core::int32 zazen_coe_add_;
};


class ServerMapConfigure {
 public:
  typedef std::set<core::uint16> MapSet;
  typedef std::map<core::uint32, MapSet> ServerMapMap;

  typedef std::map<core::uint32, MapInfo> MapInfoMap;
/*
  typedef std::set<core::uint16> Maps;
  typedef std::map<core::uint16, Maps> ZoneMaps;
  typedef std::map<core::uint32, ZoneMaps> ServerZoneMaps;

  typedef std::map<core::uint32, MapInfo> MapinfoMap;
*/
  ServerMapConfigure();
  ~ServerMapConfigure();

  bool LoadConfigure(const std::string &file);

  inline const MapSet *GetServerMaps(core::uint32 server) {
    ServerMapMap::const_iterator iterator = this->maps_.find(server);
    if(iterator != this->maps_.end()) {
      return &iterator->second;
    }
    return NULL;
  }
/*
  // 得到服务器的区地图
  inline const ZoneMaps *GetServerMaps(core::uint32 server) {
    ServerZoneMaps::const_iterator iterator = this->maps_.find(server);
    if(iterator != this->maps_.end()) {
      return &iterator->second;
    }
    return NULL;
  }
*/
  // 得到地图配置数据
  inline const MapInfo *GetMapInfo(core::uint32 map) const {
    MapInfoMap::const_iterator iterator = this->map_infos_.find(map);
    if(iterator != this->map_infos_.end()) {
      return &iterator->second;
    }
    return NULL;
  }

  // 以服务器ID为键的地衅配置表
  ServerMapMap maps_;

  // 地图障碍信息集合配置文件
  std::string map_block_file_;

  // 地图信息配置
  MapInfoMap map_infos_;
/*
  // 以服务器ID为键值的地图配置表(支持不同的区配置在同一个游戏服务器中)
  ServerZoneMaps maps_;
  // 地图障碍信息集合配置文件
  std::string map_block_file_;

  // 地图信息配置
  MapinfoMap map_infos_;
  */
};

class MapBlockConfigure {
  typedef std::map<core::uint32, MapBlock> MapBlockMap;

 public:
  MapBlockConfigure();
  ~MapBlockConfigure();

  bool LoadConfigure(const std::string &file);
  bool Load(core::uint32 map, const std::string &map_file);

  const MapBlock *GetMapBlock(core::uint32 map) const;

  MapBlockMap map_blocks_;
};

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  // 得到地图障碍信息
  inline const MapBlock *GetMapBlock(core::uint32 map) const {
    return this->map_blocks_.GetMapBlock(map);
  }
  // 得到服务器地图配置
  inline const ServerMapConfigure *GetServerMapConfigure() const {
    return &this->server_maps_;
  }
  inline const ServerMapConfigure::MapSet *GetServerMaps(core::uint32 server) {
    return this->server_maps_.GetServerMaps(server);
  }

  inline const JumpCell *GetJump(core::uint32 npc) const {
    return this->jump_configure_.GetJump(npc);
  }
  
  inline const GatherCell *GetGatherCell(core::uint32 npc) const {
    return this->load_gather_.GetCell((core::int32)npc);
  }

  inline const WorldJumpCell *GetWorldJumpCell(core::uint32 map_id) const {
    return this->load_world_jump_.GetJumpCell(map_id);
  }

  inline const ConsecutiveKillCell *GetNearCell(core::int32 kill_num) const {
    return this->consecutive_kill_configure_.GetNearCell(kill_num);
  }
  inline const ConsecutiveKillCell *GetCell(core::int32 kill_num) const {
    return this->consecutive_kill_configure_.GetCell(kill_num);
  }

  // NPC 与玩家最大采集距离
  static const core::uint32 kGatherDistance_;
private:
  Configure() {}
  ~Configure() {}

  ServerMapConfigure server_maps_;
  MapBlockConfigure map_blocks_;
  LoadGatherConfigure load_gather_;

  JumpConfigure jump_configure_;
  LoadWorldJumpConfigure load_world_jump_;
  LoadConsecutiveKillConfigure consecutive_kill_configure_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__CONFIGURE__H

