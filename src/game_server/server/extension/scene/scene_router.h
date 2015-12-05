//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-26 10:38:30.
// File name: scene_router.h
//
// Description:
// Define class SceneRouter.
//
// 算法说明：
//     该寻路用于服务器端怪物寻路，按从源点到目标点的直线(有可能是
// 折线，因为八方向原因)尝试逼近目标点，如果中间遇到阻档点，则开始
// 绕转(先顺时转，再逆时转)，如果绕转到直线(有可能是折线，八方向原
// 因)，则停止绕转，如果寻路路径超过了最大寻路点个数时，再从目标点
// 到源点按上述方向寻一次，如果再失败，反回一个能寻到的最近的点。
//     该算法的寻路结果并不是最短路径，也在某些复杂情况下会导致连通
// 点的寻路失败，但这都不是关键，关键是效率高，怪物寻路没有必要那么
// 精确。
//

#ifndef __GAME__SERVER__SCENE__SCENE__ROUTER__H
#define __GAME__SERVER__SCENE__SCENE__ROUTER__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/constants_constants.h"
#include "entity/direction_types.h"

namespace game {

namespace server {

namespace scene {

// 坐标点
class RouterCoord {
 public:
  RouterCoord() : x_(0), y_(0) {}

  core::uint32 x_;
  core::uint32 y_;
};

// 绕转点
class AroundPoint {
 public:
  AroundPoint() : dir_(entity::DirectionType::MIN) {}

  RouterCoord coord_;           // 坐标点
  entity::DirectionType::type dir_;         // 方向
};

template <typename Scene>
class SceneRouter : public core::Noncopyable {
 public:
  SceneRouter() {}
  ~SceneRouter() {}

  bool Initialize();
  void Finalize();

  inline void SetScene(Scene *scene) {
    this->scene_ = scene;
  }

  // 获取寻路结果
  inline const entity::DirectionType::type *GetPaths(size_t &size) const {
    size = this->path_size_;
    return this->paths_;
  }

  // 检测是否需要反转
  inline bool CheckReverse() const {
    return this->reverse_;
  }

  // 方向取反
  inline entity::DirectionType::type ReverseDirection(
      entity::DirectionType::type dir) const {
    if(dir < entity::DirectionType::MIN || dir > entity::DirectionType::NONE) {
      return entity::DirectionType::MAX;
    }

    static entity::DirectionType::type reverse_dir[8] = {
      entity::DirectionType::WEST,
      entity::DirectionType::EAST,
      entity::DirectionType::NORTH,
      entity::DirectionType::SOUTH,
      entity::DirectionType::SOUTHWEST,
      entity::DirectionType::SOUTHEAST,
      entity::DirectionType::NORTHWEST,
      entity::DirectionType::NORTHEAST,
    };

    return reverse_dir[dir];
  };

  // 寻路接口
  bool Router(const RouterCoord &src, const RouterCoord &dest, size_t max_path_size,
      core::int32 dest_radius);

 private:
  // 寻路算法
  bool RouterRoad(const RouterCoord &src, const RouterCoord &dest);

  // 向终点靠近(-1:出错, 0:靠近失败, 1:靠近成功)
  int Approach(const RouterCoord &coord, const RouterCoord &dest);

  // 绕转
  bool Around(const RouterCoord &coord, entity::DirectionType::type dir,
      const RouterCoord &src, const RouterCoord &dest, bool clockwise);

  // 获取移动方向
  entity::DirectionType::type GetDirection(const RouterCoord &src,
      const RouterCoord &dest);

  // 得到下一步的坐标
  void NextCoord(entity::DirectionType::type dir, const RouterCoord &coord,
      RouterCoord &next);

  // 转向
  entity::DirectionType::type TurnDirection(entity::DirectionType::type dir,
      bool clockwise = true);

  // 是否在直路上
  bool CheckStraight(const RouterCoord &src, const RouterCoord &coord,
      const RouterCoord &dest);

  // 是否在直接射线上
  bool CheckInRay(const RouterCoord &src, const RouterCoord &coord,
      const RouterCoord &dest);

  // 最大路径
  static const size_t kMaxPathSize_ = 128;

  // 最大路径个数，不能大于kMaxPathSize_
  size_t max_path_;

  // 路径
  entity::DirectionType::type paths_[kMaxPathSize_];

  // 路径长度
  size_t path_size_;

  // 绕转点信息
  AroundPoint around1_;
  AroundPoint around2_;

  // 起点/终点
  RouterCoord src_;
  RouterCoord dest_;

  // 最近点
  RouterCoord nearest_;

  // 所在场景
  Scene *scene_;

  // 寻路结果是否需要反转
  bool reverse_;
};

template <typename Scene>
bool SceneRouter<Scene>::Initialize() {
  this->max_path_ = 0;
  this->path_size_ = 0;
  this->src_.x_ = 0;
  this->src_.y_ = 0;
  this->dest_.x_ = 0;
  this->dest_.y_ = 0;
  this->nearest_.x_ = 0;
  this->nearest_.y_ = 0;
  this->scene_ = NULL;
  this->reverse_ = false;

  return true;
}

template <typename Scene>
void SceneRouter<Scene>::Finalize() {
}

template <typename Scene>
bool SceneRouter<Scene>::Router(const RouterCoord &src, const RouterCoord &dest,
    size_t max_path_size, core::int32 dest_radius) {
  RouterCoord dest_coord = dest;

  if(this->scene_->CheckNpcWalkBlock(dest_coord.x_, dest_coord.y_) == true &&
      this->scene_->GetNearNpcWalkPosition(dest_coord.x_, dest_coord.y_, dest_radius) == false) {
    return false;
  }

  // 先把路径清空
  this->path_size_ = 0;

  // 最大寻路长度超过最大长度
  if(max_path_size > kMaxPathSize_) {
    return false;
  }

  this->max_path_ = max_path_size;

  // 第一次，从源点到目标点
  if(this->RouterRoad(src, dest_coord) == true) {
    this->reverse_ = false;
    return true;
  }

  this->path_size_ = 0;

  // 第二次，从目标点到源点
  if(this->RouterRoad(dest_coord, src) == true) {
    this->reverse_ = true;
    return true;
  }

  return false;
}

template <typename Scene>
bool SceneRouter<Scene>::RouterRoad(const RouterCoord &src, const RouterCoord &dest) {
  this->src_ = src;
  this->dest_ = dest;
  this->path_size_ = 0;
  this->nearest_ = src;

  RouterCoord current_coord = src;

  while(true) {
    // 试着向目标逼近
    int ret = this->Approach(current_coord, dest);
    if(ret >= 1) {
      // 成功到达终点直接返回成功
      return true;
    } else if(ret <= -1) {
      // 失败后直接返回失败
      return false;
    } else {
      bool around = false;
      core::uint32 path_size = this->path_size_;
      // 逼近中遇到了阻档点，开始顺时绕转转
      if(this->Around(this->around1_.coord_, this->around1_.dir_,
            src, dest, false) == false) {
        this->path_size_ = path_size;
        // 顺时绕转失败，逆时绕转
        around = this->Around(this->around2_.coord_, this->around2_.dir_,
            src, dest, true);
        if(around == false) {
          // 烧转失败
          this->path_size_ = path_size;
          return false;
        }
      }
      // 绕转成功，更新为当前点为找到的最近点
      current_coord = this->nearest_;
    }
  }
}

template <typename Scene>
int SceneRouter<Scene>::Approach(const RouterCoord &coord, const RouterCoord &dest) {
  if(this->scene_ == NULL) {
    return -1;
  }

  // 如果寻到了终点，直接返回成功
  if(coord.x_ == dest.x_ && coord.y_ == dest.y_) {
    return 1;
  }

  core::uint32 current_dir = entity::DirectionType::EAST;

  RouterCoord current_coord = coord;
  RouterCoord next_coord;

  while(this->path_size_ < this->max_path_) {
    // 获取移动方向
    current_dir = GetDirection(current_coord, dest);
    // 获取移动方向上的下个点
    this->NextCoord((entity::DirectionType::type)current_dir, current_coord, next_coord);

    // 下个点是否为阻档
    if(this->scene_->CheckNpcWalkBlock(next_coord.x_, next_coord.y_) == true) {
      // 遇到了阻挡，需要绕转，增加一个顺时针绕转点
      this->around1_.coord_.x_ = current_coord.x_;
      this->around1_.coord_.y_ = current_coord.y_;
      // 顺时绕转点要把方向设置为向逆时指转45度方向
      this->around1_.dir_ = this->TurnDirection((entity::DirectionType::type)current_dir, true);

      // 再增加一个逆时针烧转点
      this->around2_.coord_.x_ = current_coord.x_;
      this->around2_.coord_.y_ = current_coord.y_;
      // 逆时绕转点要把方向设置为向顺时指转45度方向
      this->around2_.dir_ = this->TurnDirection((entity::DirectionType::type)current_dir, false);

      return 0;
    }

    // 记录到下一个点的移动方向
    this->paths_[this->path_size_++] = (entity::DirectionType::type)current_dir;

    // 如果寻到了终点，直接返回成功
    if(next_coord.x_ == dest.x_ && next_coord.y_ == dest.y_) {
      return 1;
    }

    // 记录最接近终点的点
    this->nearest_ = current_coord;
    // 更新当前点
    current_coord = next_coord;
  }

  return -1;
}

template <typename Scene>
bool SceneRouter<Scene>::Around(const RouterCoord &coord, entity::DirectionType::type dir,
    const RouterCoord &src, const RouterCoord &dest, bool clockwise) {
  if(this->scene_ == NULL) {
    return false;
  }

  RouterCoord current_coord = coord;
  RouterCoord next_coord;
  RouterCoord around_coord;

  entity::DirectionType::type around_dir;

  core::uint32 turn_count = 0;

  while(this->path_size_ < this->max_path_) {
    // 获取移动方向上的下一个点
    this->NextCoord(dir, current_coord, next_coord);

    if(this->scene_->CheckNpcWalkBlock(next_coord.x_, next_coord.y_) == true) {
      // 如果下一个点为阻档点，向反方向转向一次
      dir = this->TurnDirection(dir, !clockwise);
      // 如果连续转了两次向都为阻档点，则失败
      if(++turn_count > 2) {
        return false;
      }
      continue;
    }

    turn_count = 0;

    // 记录移动到下个点的方向
    this->paths_[this->path_size_++] = dir;

    current_coord = next_coord;

    // 如果当前点已经回到直线上，停止绕转
    if(this->CheckStraight(coord, current_coord, dest) == true) {
      this->nearest_ = current_coord;
      return true;
    }

    // 尝试绕转
    around_dir = this->TurnDirection(dir, clockwise);
    this->NextCoord(around_dir, current_coord, around_coord);
    if(this->scene_->CheckNpcWalkBlock(around_coord.x_, around_coord.y_) == false) {
      dir = around_dir;
    }
  }

  return false;
}

template <typename Scene>
entity::DirectionType::type SceneRouter<Scene>::GetDirection(const RouterCoord &src,
    const RouterCoord &dest) {
  static core::uint32 dir_table[3][3] = {
    { entity::DirectionType::NORTHWEST, entity::DirectionType::NORTH,
      entity::DirectionType::NORTHEAST },
    { entity::DirectionType::WEST, entity::DirectionType::MAX,
      entity::DirectionType::EAST },
    { entity::DirectionType::SOUTHWEST, entity::DirectionType::SOUTH,
      entity::DirectionType::SOUTHEAST }
  };

  core::uint32 x = dest.x_ > src.x_ ? 2 : (dest.x_ == src.x_ ? 1 : 0);
  core::uint32 y = dest.y_ > src.y_ ? 2 : (dest.y_ == src.y_ ? 1 : 0);

  return (entity::DirectionType::type)dir_table[y][x];
}

template <typename Scene>
void SceneRouter<Scene>::NextCoord(entity::DirectionType::type dir,
    const RouterCoord &coord, RouterCoord &next) {
  if(dir < entity::DirectionType::MIN || dir > entity::DirectionType::NONE) {
    return ;
  }
  next.x_ = coord.x_ + entity::g_constants_constants.kDirectionOffsetX[dir];
  next.y_ = coord.y_ + entity::g_constants_constants.kDirectionOffsetY[dir];
}

template <typename Scene>
entity::DirectionType::type SceneRouter<Scene>::TurnDirection(entity::DirectionType::type dir,
    bool clockwise) {
  if(dir < entity::DirectionType::MIN || dir > entity::DirectionType::NONE) {
    return entity::DirectionType::MAX;
  }

  static entity::DirectionType::type next_dir1[8] = {
    entity::DirectionType::SOUTH,
    entity::DirectionType::NORTH,
    entity::DirectionType::WEST,
    entity::DirectionType::EAST,
    entity::DirectionType::EAST,
    entity::DirectionType::NORTH,
    entity::DirectionType::SOUTH,
    entity::DirectionType::WEST,
  };

  static entity::DirectionType::type next_dir2[8] = {
    entity::DirectionType::NORTH,
    entity::DirectionType::SOUTH,
    entity::DirectionType::EAST,
    entity::DirectionType::WEST,
    entity::DirectionType::NORTH,
    entity::DirectionType::WEST,
    entity::DirectionType::EAST,
    entity::DirectionType::SOUTH,
  };

  if(clockwise) {
    return next_dir1[dir];
  } else {
    return next_dir2[dir];
  }
}

template <typename Scene>
bool SceneRouter<Scene>::CheckStraight(const RouterCoord &src, const RouterCoord &coord,
    const RouterCoord &dest) {
  // 是否在射线上
  if(this->CheckInRay(src, coord, dest) == true) {
    return true;
  }

  // 检测是否在有折线(两条)上
  int offsetx = (int)dest.x_ - (int)src.x_;
  int offsety = (int)dest.y_ - (int)src.y_;

  int abs_offsetx = (abs(offsetx));
  int abs_offsety = (abs(offsety));

  RouterCoord middle;
  if(abs_offsetx < abs_offsety) {
    middle.x_ = dest.x_;
    middle.y_ = src.y_ + (offsety > 0 ? abs_offsetx : -abs_offsetx);
  } else {
    middle.y_ = dest.y_;
    middle.x_ = src.x_ + (offsetx > 0 ? abs_offsety : -abs_offsety);
  }

  if(middle.x_ == coord.x_ && middle.y_ == coord.y_) {
    return true;
  }

  return this->CheckInRay(src, coord, middle) ||
    this->CheckInRay(middle, coord, dest);
}

template <typename Scene>
bool SceneRouter<Scene>::CheckInRay(const RouterCoord &src, const RouterCoord &coord,
    const RouterCoord &dest) {
  int offsetx = (int)dest.x_ - (int)src.x_;
  int offsety = (int)dest.y_ - (int)src.y_;

  if(offsetx == 0 && offsety == 0) {
    return false;
  }

  // 在90度射线上
  if(offsetx == 0) {
    // 方向是否相同
    return (dest.x_ == coord.x_) && ((coord.y_ > src.y_) == (dest.y_ > src.y_));
  }

  // 在0度射线上
  if(offsety == 0) {
    // 方向是否相同
    return (dest.y_ == coord.y_) && ((coord.x_ > src.x_) == (dest.x_ > src.x_));
  }

  int abs_offsetx = (abs(offsetx));
  int abs_offsety = (abs(offsety));

  // 在45度射线上
  if(abs_offsetx == abs_offsety) {
    // 在45度线上
    if(abs(coord.x_ - src.x_) != abs(coord.y_ - src.y_)) {
      return false;
    }
    // 方向也必须相同
    return ((dest.x_ > src.x_) == (coord.x_ > src.x_)) &&
      ((dest.y_ > src.y_) == (coord.y_ > src.y_));
  }

  return false;
}

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__ROUTER__H

