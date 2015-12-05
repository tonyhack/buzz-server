namespace cpp entity
namespace py ccentity.maze
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 地宫格子类型定义(只定义了封印格子和通路格子两个特殊值, 其余值见表)
enum MazeGridType {
  MIN = 0,

  UNEXPLORED = 0,                     // 封印格子
  NORMAL,                             // 通路格子

  MAX,
}



////////////////////////////////////////////////
// 地宫移动方向类型定义
enum MazeDirectionType {
  MIN = 0,

  EAST = 0,                           // 东
  WEST,                               // 西
  SOUTH,                              // 南
  NORTH,                              // 北

  MAX,
}

