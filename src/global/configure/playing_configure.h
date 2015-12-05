//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 18:03:19.
// File name: playing_configure.h
//
// Description:
// Define class PlayingConfigure.
//

#ifndef __GLOBAL__PLAYING__CONFIGURE__H
#define __GLOBAL__PLAYING__CONFIGURE__H

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/playing_types.h"

namespace global {

namespace configure {

class PlayingAward {
 public:
  PlayingAward() {}
  ~PlayingAward() {}

  entity::ResourceType::type type_;
  core::int32 id_;
  core::int32 number_;
};

class PlayingCell {
  typedef std::vector<core::int32> PositionVector;

 public:
  typedef std::vector<PlayingAward> AwardVector;

  PlayingCell() {}
  ~PlayingCell() {}

  core::int32 id_;
  core::int32 group_;

  // 副本依赖关系
  core::int32 depend_;

  // entity::PlayingType::type type_;
  // 阵营类型
  entity::PlayingCampType::type camp_type_;
  // 阵营数量
  core::int32 camp_number_;
  core::int32 map_;

  // 自动进行类型
  entity::PlayingAutoType::type auto_type_;
  core::int32 auto_spend_freedom_dollars_;

  // 最小等级和最大等级
  core::int32 min_level_;
  core::int32 max_level_;

  // 周期次数
  entity::PlayingPeriodType::type period_type_;
  core::int32 period_count_;

  // 周期中的天
  core::int32 period_day_;

  // 周周期中某天的时间段
  core::int32 period_begin_time_;

  // 最小玩家数量和最大玩家数量
  core::int32 min_actor_;
  core::int32 max_actor_;

  // 进入坐标
  PositionVector enter_posx_;
  PositionVector enter_posy_;

  // 退出地图和坐标
  PositionVector exit_map_;
  PositionVector exit_posx_;
  PositionVector exit_posy_;

  // 首通奖励
  AwardVector first_pass_awards_;

  // 购买奖励
  AwardVector paid_awards_;
  // 购买奖励次数(周期内)
  core::int32 paid_award_period_count_;
  // 购买奖励消耗
  core::int32 paid_award_spend_freedom_dollars_;

  inline bool GetEnterPosition(core::int32 camp, core::int32 &x,
      core::int32 &y) const {
    if((size_t)camp >= this->enter_posx_.size() ||
        (size_t)camp >= this->enter_posy_.size()) {
      return false;
    }
    x = this->enter_posx_[camp];
    y = this->enter_posy_[camp];
    return true;
  }
  inline bool GetExitPosition(core::int32 camp, core::int32 &map,
      core::int32 &x, core::int32 &y) const {
    if((size_t)camp >= this->exit_map_.size() ||
        (size_t)camp >= this->exit_posx_.size() ||
        (size_t)camp >= this->exit_posy_.size()) {
      return false;
    }
    map = this->exit_map_[camp];
    x = this->exit_posx_[camp];
    y = this->exit_posy_[camp];
    return true;
  }
};

class PlayingConfigure : public core::Noncopyable {
 public:
  typedef std::map<core::int32, PlayingCell> PlayingMap;

  PlayingConfigure();
  ~PlayingConfigure();

  bool LoadConfigure(const std::string &file);

  bool LoadPlayingConfigure(const std::string &file);

  const PlayingCell *GetPlaying(core::int32 id) const;

  const std::string &GetPlayingFile() const {
    return this->playing_file_;
  }

  const PlayingMap &GetPlayings() const {
    return this->playings_;
  }

  core::int32 playing_pool_initial_;
  core::int32 playing_pool_extend_;

  // 副本销毁检测时间
  static const core::uint32 kPlayingDestoryCheckMsec_ = 30 * 1000;
  static const core::uint32 kPlayingDestoryTimeout_ = 30;
  static const core::int32 kTeamMaxMembers_ = 5;

 private:
  PlayingMap playings_;
  std::string playing_file_;
};

}  // namespace configure

}  // namespace global

#endif  // __GLOBAL__PLAYING__CONFIGURE__H

