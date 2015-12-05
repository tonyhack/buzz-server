#ifndef __GLOBAL__CONFIGURE__SOCIAL__CONFIGURE__H
#define __GLOBAL__CONFIGURE__SOCIAL__CONFIGURE__H

#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace global {

namespace configure {

class HomelandTempleCell {
 public:
  // 神殿等级
  core::int32 level_;

  // 升级消耗金币
  core::int32 upgrade_spend_gold_;

  // 升级冷却时间(单位s)
  core::int32 upgrade_cooling_;

  // 每天最大征收次数
  core::int32 max_harvest_times_;

  // 征收得到金币
  core::int32 harvest_gold_reward_;

  // 征收冷却时间
  core::int32 harvest_cooling_;

  // 每天最大强征次数
  core::int32 max_force_harvest_times_;

  // VIP增加的强征次数
  core::int32 vip_bonus_force_harvest_times_;

  // 强征得到金币
  core::int32 force_harvest_gold_reward_;

  // 强征消耗钻石下限
  core::int32 force_harvest_min_spend_freedom_dollars_;

  // 强征消耗钻石上限
  core::int32 force_harvest_max_spend_freedom_dollars_;

  // 每次强征消耗钻石增量
  core::int32 force_harvest_step_spend_freedom_dollars_;
};

class HomelandGoldmineCell {
 public:
  // 金矿等级
  core::int32 level_;

  // 升级消耗金币
  core::int32 upgrade_spend_gold_;

  // 升级冷却时间(单位s)
  core::int32 upgrade_cooling_;

  // 收获得到金币
  core::int32 harvest_gold_reward_;

  // 收获冷却时间
  core::int32 harvest_cooling_;

  // 最大被掠夺次数
  core::int32 max_robbed_times_;

  // 小胜被掠夺损失金币比例(百分之)
  core::int32 tiny_win_gold_loss_rate_;

  // 大胜被掠夺损失金币比例(百分之)
  core::int32 great_win_gold_loss_rate_;

  // 完胜被掠夺损失金币比例(百分之)
  core::int32 perfect_win_gold_loss_rate_;
};

class HomelandPoolCell {
 public:
  // 神秘之源等级
  core::int32 level_;

  // 升级消耗金币
  core::int32 upgrade_spend_gold_;

  // 升级冷却时间
  core::int32 upgrade_cooling_;

  // 每次累计获得经验
  core::int32 accumulate_exp_reward_;

  // 每次累计获得灵气
  core::int32 accumulate_nimbus_reward_;

  // 每次累计花费时间(单位s)
  core::int32 accumulate_spend_time_;

  // 最大累计次数
  core::int32 max_accumulate_times_;

  // 双倍领取消耗道具ID
  core::uint32 double_harvest_spend_item_id_;

  // 双倍领取消耗道具数量
  core::int32 double_harvest_spend_item_count_;
};

class HomelandChurchBonusCell {
 public:
  // 奖励ID
  core::int32 bonus_id_;

  // 几率(万分之)
  core::uint32 odds_;

  // 奖励道具ID
  core::uint32 item_id_;

  // 奖励道具数量
  core::int32 item_count_;
};

class HomelandChurchChestCell {
 public:
  // 宝箱ID
  core::int32 id_;

  // 祈福几率(万分之)
  core::uint32 pray_odds_;

  // 刷新几率(万分之)
  core::uint32 refresh_odds_;

  // 奖励集合
  std::vector<HomelandChurchBonusCell> bonus_set_;
};

class HomelandTreeCell {
 public:
  // 神树等级
  core::int32 level_;

  // 升级需要收获次数
  core::int32 upgrade_required_harvest_times_;

  // 进入生长期需要完成度
  core::int32 growing_required_water_times_;

  // 自己浇水冷却时间(单位s)
  core::int32 water_cooling_;

  // 生长冷却时间(单位s)
  core::int32 growing_cooling_;

  // 收获得到礼券
  core::int32 harvest_restrict_dollars_reward_;

  // 最大被抢收次数
  core::int32 max_stolen_times_;

  // 每次被抢收损失比例(百分之)
  core::int32 stolen_loss_rate_;
};

class HomelandTowerCell {
 public:
  // 炮塔等级
  core::int32 level_;

  // 升级消耗金币
  core::int32 upgrade_spend_gold_;

  // 升级冷却时间(单位s)
  core::int32 upgrade_cooling_;

  // 攻击力
  core::int32 attack_;
};

class HomelandStrongholdCell {
 public:
  // 军团据点等级
  core::int32 level_;

  // 升级消耗金币
  core::int32 upgrade_spend_gold_;

  // 升级冷却时间(单位s)
  core::int32 upgrade_cooling_;

  // 最大支援栏位数量
  core::int32 max_support_slot_capacity_;

  // 申请支援冷却时间
  core::int32 call_support_cooling_;
};

class HomelandSoulCell {
 public:
  struct SpecialIDType {
    enum type {
      MIN = 1,

      // 战士玩家
      AXE_ACTOR = 1,

      // 弓手玩家
      SHOOT_ACTOR,

      // 法师玩家
      MAGIC_ACTOR,

      MAX
    };
  };

  struct SoulType {
    enum type {
      MIN = 0, 

      // 近战
      AXE = 0,

      // 远程
      SHOOT,

      // 法术
      MAGIC,

      MAX
    };
  };

  // 英灵ID
  core::int32 id_;

  // 职业类型
  SoulType::type type_;

  // 基础生命
  core::int32 hp_base_;

  // 生命成长值
  core::int32 hp_growth_;

  // 基础攻击
  core::int32 attack_base_;

  // 攻击成长值
  core::int32 attack_growth_;

  // 闪避率(万分之)
  core::int32 dodge_percent_;

  // 暴击率(万分之)
  core::int32 crit_percent_;

  // 暴击倍率(万分之)
  core::int32 crit_effect_;

  // 特殊技能发动几率
  core::int32 special_skill_odds_;

  // 特殊技能伤害系数
  core::int32 special_skill_hurt_coe_;
};

class SocialConfigure : public core::Noncopyable {
 public:
  typedef __gnu_cxx::hash_map<core::int32, HomelandTempleCell> HomelandTempleHashmap;
  typedef __gnu_cxx::hash_map<core::int32, HomelandGoldmineCell> HomelandGoldmineHashmap;
  typedef __gnu_cxx::hash_map<core::int32, HomelandPoolCell> HomelandPoolHashmap;
  typedef std::vector<HomelandChurchChestCell> HomelandChurchChestVector;
  typedef __gnu_cxx::hash_map<core::int32, size_t> HomelandChurchChestIndexHashmap;
  typedef __gnu_cxx::hash_map<core::uint64, HomelandChurchBonusCell *> HomelandChurchBonusIndexHashmap;
  typedef __gnu_cxx::hash_map<core::int32, HomelandTreeCell> HomelandTreeHashmap;
  typedef __gnu_cxx::hash_map<core::int32, HomelandTowerCell> HomelandTowerHashmap;
  typedef __gnu_cxx::hash_map<core::int32, HomelandStrongholdCell> HomelandStrongholdHashmap;
  typedef __gnu_cxx::hash_map<core::int32, HomelandSoulCell> HomelandSoulHashmap;

  SocialConfigure();
  ~SocialConfigure();

  bool LoadConfigure(const std::string &file);

  // 家园神殿
  const HomelandTempleCell *GetHomelandTemple(core::int32 level) const;
  // 家园金矿
  const HomelandGoldmineCell *GetHomelandGoldmine(core::int32 level) const;
  // 家园神秘之源
  const HomelandPoolCell *GetHomelandPool(core::int32 level) const;
  // 家园祈福堂
  const HomelandChurchChestCell *GetHomelandChurchChest(core::int32 id) const;
  const HomelandChurchChestCell *GetLastHomelandChurchChest() const;
  const HomelandChurchChestCell *GetRandomHomelandChurchChestByPrayOdds(core::uint32 random) const;
  const HomelandChurchChestCell *GetRandomHomelandChurchChestByRefreshOdds(core::uint32 random) const;
  const HomelandChurchBonusCell *GetHomelandChurchBonus(core::int32 chest_id,
      core::int32 bonus_id) const;
  const HomelandChurchBonusCell *GetRandomHomelandChurchBonus(core::int32 chest_id,
      core::uint32 random) const;
  // 家园神树
  const HomelandTreeCell *GetHomelandTree(core::int32 level) const;
  // 家园炮塔
  const HomelandTowerCell *GetHomelandTower(core::int32 level) const;
  // 家园军团据点
  const HomelandStrongholdCell *GetHomelandStronghold(core::int32 level) const;
  // 家园英灵
  const HomelandSoulCell *GetHomelandSoul(core::int32 id) const;

 private:
  bool LoadHomelandTemple(const std::string &file);
  bool LoadHomelandGoldmine(const std::string &file);
  bool LoadHomelandPool(const std::string &file);
  bool LoadHomelandChurchChest(const std::string &file);
  bool LoadHomelandChurchBonus(const std::string &file);
  bool LoadHomelandTree(const std::string &file);
  bool LoadHomelandTower(const std::string &file);
  bool LoadHomelandStronghold(const std::string &file);
  bool LoadHomelandSoul(const std::string &file);

  HomelandTempleHashmap homeland_temples_;
  HomelandGoldmineHashmap homeland_goldmines_;
  HomelandPoolHashmap homeland_pools_;
  HomelandChurchChestVector homeland_church_chests_store_;
  HomelandChurchChestIndexHashmap homeland_church_chests_index_;
  HomelandChurchBonusIndexHashmap homeland_church_bonus_index_;
  HomelandTreeHashmap homeland_trees_;
  HomelandTowerHashmap homeland_towers_;
  HomelandStrongholdHashmap homeland_strongholds_;
  HomelandSoulHashmap homeland_souls_;
};

}  // namespace configure

}  // namespace global

#endif  // __GLOBAL__CONFIGURE__SOCIAL__CONFIGURE__H

