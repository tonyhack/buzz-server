#ifndef __GLOBAL__CONFIGURE__MAZE__CONFIGURE__H
#define __GLOBAL__CONFIGURE__MAZE__CONFIGURE__H

#include <map>
#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace global {

namespace configure {

class MazeGridBonusCell {
 public:
  struct BonusType {
    enum type {
      MIN = 1,
      
      // 资源
      RESOURCE = 1,

      // 道具 
      ITEM,

      // BUFF
      BUFF,

      // 增加行动点
      ADD_ACTION_POINT,

      // 减少行动点
      DECREASE_ACTION_POINT,

      // 倒退1
      MOVE_BACK1,

      // 倒退2
      MOVE_BACK2,

      // 回到起点
      MOVE_TO_START_GRID,

      MAX
    };
  };

  // 奖励ID
  core::int32 bonus_id_;

  // 几率(万分之)
  core::uint32 odds_;

  // 类型
  BonusType::type type_;

  // ID
  core::int32 id_;

  // 最小数量
  core::int32 min_number_;

  // 最大数量
  core::int32 max_number_;
};

class MazeGridCell {
 public:
  // 格子ID
  core::int32 id_;

  // 数量
  core::int32 number_;

  // 奖励集合
  std::vector<MazeGridBonusCell> bonus_set_;
};

class MazeConfigure : public core::Noncopyable {
 public:
  typedef std::map<core::int32, MazeGridCell> MazeGridMap;
  typedef __gnu_cxx::hash_map<core::uint64, MazeGridBonusCell *> MazeGridBonusIndexHashmap;

  MazeConfigure();
  ~MazeConfigure();

  bool LoadConfigure(const std::string &file);

  const MazeGridMap &GetAllMazeGrids() const { return this->maze_grids_; }
  const MazeGridCell *GetMazeGrid(core::int32 id) const;
  const MazeGridBonusCell *GetMazeGridBonus(core::int32 grid_id, core::int32 bonus_id) const;
  const MazeGridBonusCell *GetRandomMazeGridBonus(core::int32 grid_id, core::uint32 random) const;

 private:
  bool LoadMazeGrid(const std::string &file);
  bool LoadMazeGridBonus(const std::string &file);

  MazeGridMap maze_grids_;
  MazeGridBonusIndexHashmap maze_grid_bonus_index_;
};

}  // namespace configure

}  // namespace global

#endif  // __GLOBAL__CONFIGURE__MAZE__CONFIGURE__H

