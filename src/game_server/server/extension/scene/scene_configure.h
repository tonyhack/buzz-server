//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-12 14:45:23.
// File name: scene_configure.h
//
// Description:
// Define scene configures.
//

#ifndef __GAME__SERVER__SCENE__SCENE__CONFIGURE__H
#define __GAME__SERVER__SCENE__SCENE__CONFIGURE__H

#include "entity/direction_types.h"

namespace game {

namespace server {

namespace scene {

class GridBlock {
 public:
  GridBlock() : x_(0), y_(0), block_(0) {}
  ~GridBlock() {}

  core::uint32 x_;
  core::uint32 y_;

  core::uint16 block_;
};

class MapBlock {
 public:
  typedef std::vector<GridBlock> GridBlockVector;

  core::uint32 id_;
  core::uint32 max_x_;
  core::uint32 max_y_;

  GridBlockVector blocks_;
};

class NpcInfo {
 public:
  NpcInfo() : npc_(0), width_(0), heigth_(0), x_(0), y_(0), number_(0) {}

  // NPC模板ID
  core::uint32 npc_;

  // 区域长宽
  core::uint32 width_;
  core::uint32 heigth_;
  // 区域左上坐标
  core::uint32 x_;
  core::uint32 y_;
  // 数量
  core::uint32 number_;
  // 重生时间
  core::uint32 reborn_time_;
  // npc 方向
  entity::DirectionType::type dir_;
};

class ReviveAreaInfo {
 public:
  ReviveAreaInfo() : duration_(0), width_(0), heigth_(0), x_(0), y_(0) {}

  // 复活时间
  core::uint32 duration_;
  // 复活区宽度
  core::uint32 width_;
  // 复活区长度
  core::uint32 heigth_;
  // 复活区左上坐标
  core::uint32 x_;
  core::uint32 y_;
  // 阵营 ID
  core::int32 camp_;
  // 地图 ID
  core::int32 map_;
};

class DefaultAreaInfo {
 public:
  DefaultAreaInfo() :x_(0), y_(0) {}
  ~DefaultAreaInfo() {};

  // 复活区左上坐标
  core::uint32 x_;
  core::uint32 y_;
};

class DayNightInfo {
  public:
  DayNightInfo() : morning_interval_(0), daytime_interval_(0),evening_interval_(0),
  night_interval_(0), morning_aoi_coe_(0), daytime_aoi_coe_(0), evening_aoi_coe_(0),
  night_aoi_coe_(0){}
  // 早晨间隔
  core::uint32 morning_interval_;
  // 白天间隔
  core::uint32 daytime_interval_;
  // 傍晚间隔
  core::uint32 evening_interval_;
  // 夜晚间隔
  core::uint32 night_interval_;

  // 早晨视野系数
  core::uint32 morning_aoi_coe_;
  // 白天视野系数
  core::uint32 daytime_aoi_coe_;
  // 傍晚视野系数
  core::uint32 evening_aoi_coe_;
  // 夜晚视野系数
  core::uint32 night_aoi_coe_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__CONFIGURE__H

