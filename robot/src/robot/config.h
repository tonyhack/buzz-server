#ifndef BUZZ_ROBOT_ROBOT_CONFIG_H
#define BUZZ_ROBOT_ROBOT_CONFIG_H

#include <stdint.h>
#include <string>
#include <ext/hash_map>
#include <brickred/class_util.h>
#include <brickred/socket_address.h>

namespace robot {

class SkillCell {
public:
    int32_t id_;
    int32_t level_;
    int32_t cooling_;
    int32_t distance_; 
    int32_t speed_;
};

class Config {
public:
    typedef __gnu_cxx::hash_map<uint64_t, SkillCell> SkillCellMap;

    bool load(const std::string &config_dir);

public:
    std::string config_dir_;
    brickred::SocketAddress login_server_addr_;
    brickred::SocketAddress gateway_server_addr_;
    std::string actor_file_;
    std::string action_file_;
    std::string misc_file_;
    std::string scene_file_;
    std::string skill_file_;

    // misc config
    int32_t axe_melee_skill_id_;
    int32_t shoot_melee_skill_id_;
    int32_t magic_melee_skill_id_;

    // skill
    const SkillCell *getSkillCell(int32_t id, int32_t level) const;
    const SkillCell *getIdleSkillCell() const;

private:
    BRICKRED_SINGLETON2(Config)

    bool loadMiscConfig(const std::string &file);
    bool loadSkillConfig(const std::string &file);

    SkillCellMap skill_cells_;
};

} // namespace robot

#endif

