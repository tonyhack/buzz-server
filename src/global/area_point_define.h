//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-13 15:37:22.
// File name: area_point_define.h
//
// Description:
// Define class AreaPointDefine.
//

#ifndef __GLOBAL__AREA__POINT__DEFINE__H
#define __GLOBAL__AREA__POINT__DEFINE__H

#include <cstddef>

#include "core/base/types.h"
#include "entity/direction_types.h"

namespace global {

struct OffsetPoint {
  int offet_x_;
  int offet_y_;
};

struct RingSize {
  enum {
    RADIUS_0 = 1,
    RADIUS_1 = 9,
    RADIUS_2 = 21,
    RADIUS_3 = 37,
    RADIUS_4 = 69,
    RADIUS_5 = 97,
    RADIUS_6 = 129, 
    RADIUS_7 = 165, 
    RADIUS_8 = 205, 
    RADIUS_9 = 249, 
    RADIUS_10 = 329,
    RADIUS_11 = 385,
    RADIUS_12 = 445,
  };
};

struct RingArrayIndex {
  enum {
    INDEX_0 = 0,
    INDEX_1 = 1,
    INDEX_2 = 2,
    INDEX_3 = 3,
    INDEX_4 = 4,
    INDEX_5 = 5,
    INDEX_6 = 6,
    INDEX_7 = 7,
    INDEX_8 = 8,
    INDEX_9 = 9,
    INDEX_10 = 10,
    INDEX_11 = 11,
    INDEX_12 = 12,
    INDEX_MAX = 13,
  };
};

struct SectorQuarterLength {
  enum {
    SECTOR_0 = 1,
    SECTOR_1 = 4,
    SECTOR_2 = 8,
    SECTOR_3 = 13,
    SECTOR_4 = 22,
    SECTOR_5 = 30,
    SECTOR_6 = 39,
    SECTOR_7 = 49,
    SECTOR_8 = 60,
    SECTOR_9 = 72,
    SECTOR_10 = 93,
    SECTOR_11 = 108,
    SECTOR_12 = 124,
    SECTOR_MAX,
  };
};

struct SectorQuarterArrayIndex {
  enum {
    INDEX_0 = 0,
    INDEX_1 = 1,
    INDEX_2 = 2,
    INDEX_3 = 3,
    INDEX_4 = 4,
    INDEX_5 = 5,
    INDEX_6 = 6,
    INDEX_7 = 7,
    INDEX_8 = 8,
    INDEX_9 = 9,
    INDEX_10 = 10,
    INDEX_11 = 11,
    INDEX_12 = 12,
    INDEX_MAX = 13,
  };
};

struct SectorHalfLength {
  enum {
    SECTOR_0 = 1,
    SECTOR_1 = 6,
    SECTOR_2 = 13,
    SECTOR_3 = 22,
    SECTOR_4 = 39,
    SECTOR_5 = 54,
    SECTOR_6 = 71,
    SECTOR_7 = 97,
    SECTOR_8 = 111,
    SECTOR_9 = 134,
    SECTOR_10 = 175,
    SECTOR_11 = 204,
    SECTOR_12 = 235,
    SECTOR_MAX,
  };
};

struct SectorHalfArrayIndex {
  enum {
    INDEX_0 = 0,
    INDEX_1 = 1,
    INDEX_2 = 2,
    INDEX_3 = 3,
    INDEX_4 = 4,
    INDEX_5 = 5,
    INDEX_6 = 6,
    INDEX_7 = 7,
    INDEX_8 = 8,
    INDEX_9 = 9,
    INDEX_10 = 10,
    INDEX_11 = 11,
    INDEX_12 = 12,
    INDEX_MAX = 13,
  };
};

struct RayLength {
  enum {
    RAY_1 = 25,
    RAY_2 = 25,
    RAY_3 = 25,
    RAY_4 = 25,
    RAY_5 = 25,
    RAY_6 = 25,
    RAY_7 = 24,
    RAY_8 = 24,
    RAY_9 = 24,
    RAY_10 = 24,
    RAY_11 = 23,
    RAY_12 = 23,
    RAY_13 = 22,
    RAY_14 = 22,
    RAY_15 = 22,
    RAY_16 = 21,
    RAY_17 = 21,
    RAY_18 = 20,
    RAY_19 = 20,
    RAY_20 = 19,
    RAY_21 = 19,
    RAY_22 = 18,
  };
};
struct RayArrayIndex {
  enum {
    INDEX_1 = 0,
    INDEX_2 = 1,
    INDEX_3 = 2,
    INDEX_4 = 3,
    INDEX_5 = 4,
    INDEX_6 = 5,
    INDEX_7 = 6,
    INDEX_8 = 7,
    INDEX_9 = 8,
    INDEX_10 = 9,
    INDEX_11 = 10,
    INDEX_12 = 11,
    INDEX_13 = 12,
    INDEX_14 = 13,
    INDEX_15 = 14,
    INDEX_16 = 15,
    INDEX_17 = 16,
    INDEX_18 = 17,
    INDEX_19 = 18,
    INDEX_20 = 19,
    INDEX_21 = 20,
    INDEX_22 = 21,
    INDEX_MAX = 22,
  };
};

class AreaPointDefine {
 public:
  // 315度到360度，长度为25的射线
  static const OffsetPoint kRayArray1_[RayLength::RAY_1];
  static const OffsetPoint kRayArray2_[RayLength::RAY_2];
  static const OffsetPoint kRayArray3_[RayLength::RAY_3];
  static const OffsetPoint kRayArray4_[RayLength::RAY_4];
  static const OffsetPoint kRayArray5_[RayLength::RAY_5];
  static const OffsetPoint kRayArray6_[RayLength::RAY_6];
  static const OffsetPoint kRayArray7_[RayLength::RAY_7];
  static const OffsetPoint kRayArray8_[RayLength::RAY_8];
  static const OffsetPoint kRayArray9_[RayLength::RAY_9];
  static const OffsetPoint kRayArray10_[RayLength::RAY_10];
  static const OffsetPoint kRayArray11_[RayLength::RAY_11];
  static const OffsetPoint kRayArray12_[RayLength::RAY_12];
  static const OffsetPoint kRayArray13_[RayLength::RAY_13];
  static const OffsetPoint kRayArray14_[RayLength::RAY_14];
  static const OffsetPoint kRayArray15_[RayLength::RAY_15];
  static const OffsetPoint kRayArray16_[RayLength::RAY_16];
  static const OffsetPoint kRayArray17_[RayLength::RAY_17];
  static const OffsetPoint kRayArray18_[RayLength::RAY_18];
  static const OffsetPoint kRayArray19_[RayLength::RAY_19];
  static const OffsetPoint kRayArray20_[RayLength::RAY_20];
  static const OffsetPoint kRayArray21_[RayLength::RAY_21];
  static const OffsetPoint kRayArray22_[RayLength::RAY_22];

  static const OffsetPoint *kRayArrays_[RayArrayIndex::INDEX_MAX];
  static const size_t kRayArraysSize_[RayArrayIndex::INDEX_MAX];

  static const OffsetPoint kRingArray0_[RingSize::RADIUS_0];
  static const OffsetPoint kRingArray1_[RingSize::RADIUS_1];
  static const OffsetPoint kRingArray2_[RingSize::RADIUS_2];
  static const OffsetPoint kRingArray3_[RingSize::RADIUS_3];
  static const OffsetPoint kRingArray4_[RingSize::RADIUS_4];
  static const OffsetPoint kRingArray5_[RingSize::RADIUS_5];
  static const OffsetPoint kRingArray6_[RingSize::RADIUS_6];
  static const OffsetPoint kRingArray7_[RingSize::RADIUS_7];
  static const OffsetPoint kRingArray8_[RingSize::RADIUS_8];
  static const OffsetPoint kRingArray9_[RingSize::RADIUS_9];
  static const OffsetPoint kRingArray10_[RingSize::RADIUS_10];
  static const OffsetPoint kRingArray11_[RingSize::RADIUS_11];
  static const OffsetPoint kRingArray12_[RingSize::RADIUS_12];

  static const OffsetPoint *kRingArrays_[RingArrayIndex::INDEX_MAX];
  static const size_t kRingArraysSize_[RingArrayIndex::INDEX_MAX];

  static OffsetPoint kSectorQuarterArray0_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray1_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray2_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray3_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray4_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray5_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray6_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray7_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray8_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray9_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray10_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray11_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];
  static OffsetPoint kSectorQuarterArray12_[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];

  typedef OffsetPoint (*SectorQuarterArray)[entity::DirectionType::MAX][SectorQuarterLength::SECTOR_MAX];

  static SectorQuarterArray kSectorQuarterArrays_[SectorQuarterArrayIndex::INDEX_MAX];
  static const size_t kSectorQuarterArraysSize_[SectorQuarterArrayIndex::INDEX_MAX][entity::DirectionType::MAX];

  static OffsetPoint kSectorHalfArray0_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray1_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray2_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray3_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray4_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray5_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray6_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray7_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray8_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray9_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray10_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray11_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];
  static OffsetPoint kSectorHalfArray12_[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];

  typedef OffsetPoint (*SectorHalfArray)[entity::DirectionType::MAX][SectorHalfLength::SECTOR_MAX];

  static SectorHalfArray kSectorHalfArrays_[SectorHalfArrayIndex::INDEX_MAX];
  static const size_t kSectorHalfArraysSize_[SectorHalfArrayIndex::INDEX_MAX][entity::DirectionType::MAX];
};

}  // namespace global

#endif  // __GLOBAL__AREA__POINT__DEFINE__H

