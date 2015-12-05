#ifndef __GLOBAL__CONFIGURE__GUILD__CONFIGURE__H
#define __GLOBAL__CONFIGURE__GUILD__CONFIGURE__H

#include <set>
#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/item_types.h"

namespace global {

namespace configure {

class GuildCell {
 public:
  // 公会等级
  core::int32 level_;

  // 人数上限
  core::int32 max_members_;

  // 维护费用
  core::int32 maintenance_fee_;

  // 维护费用额度(拖欠超过该值军团将解散)
  core::int32 maintenance_fee_limit_;

  // 升级消耗金币(万)
  core::int32 spend_gold_;

  // 升级消耗光晶石数量
  core::int32 spend_light_crystal_;

  // 升级消耗暗晶石数量
  core::int32 spend_dark_crystal_;

  // 升级消耗圣晶石数量
  core::int32 spend_holy_crystal_;

  // 商店等级上限
  core::int32 max_shop_level_;

  // 学院等级上限
  core::int32 max_college_level_;

  // 军营等级上限
  core::int32 max_barrack_level_;
};

class GuildPositionCell {
 public:
  // 职位ID
  core::int32 id_;

  // 职位序号
  core::int32 seq_;

  // 人数上限
  core::int32 max_num_;

  // 权限-关闭申请
  bool authority_suppress_join_;

  // 权限-邀请成员加入
  bool authority_invite_;

  // 权限-审核新成员
  bool authority_audit_pending_member_;

  // 权限-离开公会
  bool authority_leave_guild_;

  // 权限-踢出成员
  bool authority_kick_member_;

  // 权限-职位任免
  bool authority_set_member_position_;

  // 权限-放弃职位
  bool authority_give_up_position_;

  // 权限-修改公告
  bool authority_edit_announcement_;

  // 权限-公会频道发言
  bool authority_chat_;

  // 权限-捐献
  bool authority_donate_;

  // 权限-升级公会设施
  bool authority_upgrade_facility_;

  // 权限-购买公会商店道具
  bool authority_shop_buy_item_;

  // 权限-学习公会技能
  bool authority_college_lean_skill_;

  // 权限-神像祈祷
  bool authority_idol_pray_;

  // 权限-解封副本
  bool authority_open_playing_;

  // 权限-发起副本挑战
  bool authority_create_playing_;
};

class GuildShopCell {
 public:
  // 商店等级
  core::int32 level_;

  // 升级消耗金币(万)
  core::int32 spend_gold_;
};

class GuildShopItemCell {
 public:
  // 道具ID
  core::uint32 item_id_;

  // 需要的公会商店等级
  core::int32 required_guild_shop_level_;

  // 需要的属性和
  core::int32 required_guild_attribute_sum_;

  // 消耗贡献值
  core::int32 spend_contribution_;

  // 消耗钻石
  core::int32 spend_freedom_dollars_;
};

class GuildCollegeCell {
 public:
  // 商店等级
  core::int32 level_;

  // 升级消耗金币(万)
  core::int32 spend_gold_;
};

class GuildSkillCell {
 public:
  struct AttrType {
    enum type {
      MIN = 0,

      PHYSIQUE = MIN,                         // 体质
      STRENGTH,                               // 力量
      INTELLIGENCE,                           // 智力
      AGILE,                                  // 敏捷
      SPIRIT,                                 // 精神

      MAX,
    };
  };

  // 技能ID
  core::int32 id_;

  // 技能等级
  core::int32 level_;

  // 需要的公会学院等级
  core::int32 required_guild_college_level_;

  // 属性类型
  AttrType::type attr_type_;

  // 属性值
  core::int32 attr_value_;

  // 消耗贡献值
  core::int32 spend_contribution_;
};

class GuildBuffCell {
 public:
  // 公会BUFF_ID
  core::int32 id_;

  // 对应BUFF表的ID
  core::int32 buff_id_;

  // 需要光属性
  core::int32 required_light_attribute_;

  // 需要暗属性
  core::int32 required_dark_attribute_;

  // 需要圣属性
  core::int32 required_holy_attribute_;
};

class GuildPrayCell {
 public:
  // 祈祷次数
  core::int32 pray_times_;

  // 消耗贡献值
  core::int32 spend_contribution_;
};

class GuildBarrackCell {
 public:
  // 军营等级
  core::int32 level_;

  // 升级消耗金币(万)
  core::int32 spend_gold_;
};

class GuildPlayingGroupCell {
 public:
  // 副本组ID
  core::int32 id_;

  // 解封需要公会等级
  core::int32 required_guild_level_;

  // 解封消耗公会金币(万)
  core::int32 spend_gold_;
};

class GuildPlayingCell {
 public:
  // 副本模板ID
  core::int32 playing_template_id_;

  // 副本组ID
  core::int32 group_id_;

  // 依赖副本完成
  core::int32 depend_;

  // 奖励
  std::vector<entity::AddItemFields> awards_;
};

class GuildConfigure : public core::Noncopyable {
 public:
  typedef __gnu_cxx::hash_map<core::int32, GuildCell> GuildHashmap;
  typedef __gnu_cxx::hash_map<core::int32, GuildPositionCell> GuildPositionHashmap;
  typedef std::set<core::int32> GuildLogoSet;
  typedef __gnu_cxx::hash_map<core::int32, GuildShopCell> GuildShopHashmap;
  typedef __gnu_cxx::hash_map<core::uint32, GuildShopItemCell> GuildShopItemHashmap;
  typedef __gnu_cxx::hash_map<core::int32, GuildCollegeCell> GuildCollegeHashmap;
  typedef __gnu_cxx::hash_map<core::uint64, GuildSkillCell> GuildSkillHashmap;
  typedef __gnu_cxx::hash_map<core::uint64, core::int32> GuildSkillMaxLevelHashmap;
  typedef __gnu_cxx::hash_map<core::int32, GuildBuffCell> GuildBuffHashmap;
  typedef __gnu_cxx::hash_map<core::int32, GuildPrayCell> GuildPrayHashmap;
  typedef __gnu_cxx::hash_map<core::int32, GuildBarrackCell> GuildBarrackHashmap;
  typedef __gnu_cxx::hash_map<core::int32, GuildPlayingGroupCell> GuildPlayingGroupHashmap;
  typedef __gnu_cxx::hash_map<core::int32, GuildPlayingCell> GuildPlayingHashmap;

  GuildConfigure();
  ~GuildConfigure();

  bool LoadConfigure(const std::string &file);

  const GuildCell *GetGuild(core::int32 level) const;
  const GuildPositionCell *GetGuildPosition(core::int32 id) const;
  inline const GuildPositionHashmap &GetAllGuildPosition() const { return this->guild_positions_; }
  bool CheckGuildLogoExist(core::int32 id) const;

  const GuildShopCell *GetGuildShop(core::int32 level) const;
  const GuildShopItemCell *GetGuildShopItem(core::uint32 item_id) const;

  const GuildCollegeCell *GetGuildCollege(core::int32 level) const;
  const GuildSkillCell *GetGuildSkill(core::int32 id, core::int32 level) const;
  core::int32 GetMaxSkillLevelInCollegeLevel(core::int32 college_level,
      core::int32 skill_id) const;

  const GuildBuffCell *GetGuildBuff(core::int32 id) const;
  inline const GuildBuffHashmap &GetAllGuildBuff() const { return this->guild_buffs_; }
  const GuildPrayCell *GetGuildPray(core::int32 pray_times) const;

  const GuildBarrackCell *GetGuildBarrack(core::int32 level) const;

  const GuildPlayingGroupCell *GetGuildPlayingGroup(core::int32 id) const;
  const GuildPlayingCell *GetGuildPlaying(core::int32 playing_template_id) const;

 private:
  bool LoadGuild(const std::string &file);
  bool LoadGuildPosition(const std::string &file);
  bool LoadGuildLogo(const std::string &file);
  bool LoadGuildShop(const std::string &file);
  bool LoadGuildShopItem(const std::string &file);
  bool LoadGuildCollege(const std::string &file);
  bool LoadGuildSkill(const std::string &file);
  bool LoadGuildBuff(const std::string &file);
  bool LoadGuildPray(const std::string &file);
  bool LoadGuildBarrack(const std::string &file);
  bool LoadGuildPlayingGroup(const std::string &file);
  bool LoadGuildPlaying(const std::string &file);

  GuildHashmap guilds_;
  GuildPositionHashmap guild_positions_;
  GuildLogoSet guild_logos_;
  GuildShopHashmap guild_shops_;
  GuildShopItemHashmap guild_shop_items_;
  GuildCollegeHashmap guild_colleges_;
  GuildSkillHashmap guild_skills_;
  GuildSkillMaxLevelHashmap guild_skill_max_levels_;
  GuildBuffHashmap guild_buffs_;
  GuildPrayHashmap guild_prays_;
  GuildBarrackHashmap guild_barracks_;
  GuildPlayingGroupHashmap guild_playing_groups_;
  GuildPlayingHashmap guild_playings_;
};

}  // namespace configure

}  // namespace global

#endif  // __GLOBAL__CONFIGURE__GUILD__CONFIGURE__H

