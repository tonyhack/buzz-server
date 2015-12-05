#ifndef __GLOBAL__CONFIGURE__CONFIGURE__H
#define __GLOBAL__CONFIGURE__CONFIGURE__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/configure/misc_configure.h"
#include "global/configure/social_configure.h"
#include "global/configure/playing_configure.h"
#include "global/configure/guild_configure.h"
#include "global/configure/load_world_boss_configure.h"
#include "global/configure/load_vip_configure.h"
#include "global/configure/auction_configure.h"
#include "global/configure/maze_configure.h"

namespace global {

namespace configure {

class Configure : public core::Noncopyable {
 public:
  Configure();
  ~Configure();

  bool LoadConfigure(const std::string &file);

  inline const std::string &GetDirtyWordFile() { return this->dirty_word_file_; }

  inline const MiscConfigure *GetMiscConfigure() const {
    return &this->misc_configure_;
  }
  inline const SocialConfigure *GetSocialConfigure() const {
    return &this->social_configure_;
  }
  inline const GuildConfigure *GetGuildConfigure() const {
    return &this->guild_configure_;
  }
  inline const PlayingConfigure *GetPlayingConfigure() const {
    return &this->playing_configure_;
  }
  inline const PlayingCell *GetPlayingCell(core::int32 id) const {
    return this->playing_configure_.GetPlaying(id);
  }
  inline const std::string &GetPlayingConfigureFile() const {
    return this->playing_configure_.GetPlayingFile();
  }
  inline const LoadWorldBossConfigure *GetWorldBossConfigure() const {
    return &this->boss_configure_;
  }
  inline const LoadVipConfigure *GetVipConfigure() const {
    return &this->vip_configure_;
  }
  inline const AuctionConfigure *GetAuctionConfigure() const {
    return &this->auction_configure_;
  }
  inline const MazeConfigure *GetMazeConfigure() const {
    return &this->maze_configure_;
  }

 private:
  std::string dirty_word_file_;

  MiscConfigure misc_configure_;
  SocialConfigure social_configure_;
  PlayingConfigure playing_configure_;
  GuildConfigure guild_configure_;
  LoadWorldBossConfigure boss_configure_;
  LoadVipConfigure vip_configure_;
  AuctionConfigure auction_configure_;
  MazeConfigure maze_configure_;
};

class ConfigureSingleton : public core::Noncopyable {
 public:
  ConfigureSingleton() {}
  ~ConfigureSingleton() {}

  static inline Configure &GetInstance() {
    return ConfigureSingleton::instance_;
  }

 private:
  static Configure instance_;
};

}  // namespace configure

}  // namespace global

#define MISC_CONF() \
  global::configure::ConfigureSingleton::GetInstance().GetMiscConfigure()

#define SOCIAL_CONF() \
  global::configure::ConfigureSingleton::GetInstance().GetSocialConfigure()

#define GUILD_CONF() \
  global::configure::ConfigureSingleton::GetInstance().GetGuildConfigure()

#define WORLD_BOSS_CONF() \
  global::configure::ConfigureSingleton::GetInstance().GetWorldBossConfigure()

#define VIP_CONF() \
  global::configure::ConfigureSingleton::GetInstance().GetVipConfigure()

#define AUCTION_CONF() \
  global::configure::ConfigureSingleton::GetInstance().GetAuctionConfigure()

#define MAZE_CONF() \
  global::configure::ConfigureSingleton::GetInstance().GetMazeConfigure()

#endif  // __GLOBAL__CONFIGURE__CONFIGURE__H

