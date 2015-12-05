#include "robot/config.h"

#include <cstddef>
#include <cstdlib>
#include <tinyxml2/tinyxml2.h>

#include "robot/logging.h"

namespace robot {

using tinyxml2::XML_NO_ERROR;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

BRICKRED_SINGLETON2_IMPL(Config)

Config::Config() {}
Config::~Config() {}

bool Config::load(const std::string &config_dir)
{
    config_dir_ = config_dir;
    std::string config_file = config_dir_ + "config.xml";

    XMLDocument doc;
    if (doc.LoadFile(config_file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", config_file.c_str());
        return false;
    }

    XMLElement *config_node = doc.FirstChildElement("config");
    if (NULL == config_node) {
        LOG_ERROR("/config not found");
        return false;
    }

    XMLElement *node = NULL;
    const char *attr = NULL;

    // login_server_addr
    node = config_node->FirstChildElement("login_server_addr");
    if (NULL == node) {
        LOG_ERROR("/config/login_server_addr not found");
        return false;
    }

    attr = node->Attribute("addr");
    if (NULL == attr) {
        LOG_ERROR("/config/login_server_addr[addr] not found");
        return false;
    }
    login_server_addr_.setIp(attr);

    attr = node->Attribute("port");
    if (NULL == attr) {
        LOG_ERROR("/config/login_server_addr[port] not found");
        return false;
    }
    login_server_addr_.setPort(::atoi(attr));

    // gateway_server_addr
    node = config_node->FirstChildElement("gateway_server_addr");
    if (NULL == node) {
        LOG_ERROR("/config/gateway_server_addr not found");
        return false;
    }

    attr = node->Attribute("addr");
    if (NULL == attr) {
        LOG_ERROR("/config/gateway_server_addr[addr] not found");
        return false;
    }
    gateway_server_addr_.setIp(attr);

    attr = node->Attribute("port");
    if (NULL == attr) {
        LOG_ERROR("/config/gateway_server_addr[port] not found");
        return false;
    }
    gateway_server_addr_.setPort(::atoi(attr));

    // actor
    node = config_node->FirstChildElement("actor");
    if (NULL == node) {
        LOG_ERROR("/config/actor not found");
        return false;
    }
    attr = node->Attribute("file");
    if (NULL == attr) {
        LOG_ERROR("/config/gateway_server_addr[port] not found");
        return false;
    }
    actor_file_ = config_dir_ + attr;

    // action
    node = config_node->FirstChildElement("action");
    if (NULL == node) {
        LOG_ERROR("/config/action not found");
        return false;
    }
    attr = node->Attribute("file");
    if (NULL == attr) {
        LOG_ERROR("/config/action[file] not found");
        return false;
    }
    action_file_ = config_dir_ + attr;

    // misc
    node = config_node->FirstChildElement("misc");
    if (NULL == node) {
        LOG_ERROR("/config/misc not found");
        return false;
    }
    attr = node->Attribute("file");
    if (NULL == attr) {
        LOG_ERROR("/config/misc[file] not found");
        return false;
    }
    misc_file_ = config_dir_ + attr;

    // scene
    node = config_node->FirstChildElement("scene");
    if (NULL == node) {
        LOG_ERROR("/config/scene not found");
        return false;
    }
    attr = node->Attribute("file");
    if (NULL == attr) {
        LOG_ERROR("/config/scene[file] not found");
        return false;
    }
    scene_file_ = config_dir_ + attr;

    // skill
    node = config_node->FirstChildElement("skill");
    if (NULL == node) {
        LOG_ERROR("/config/skill not found");
        return false;
    }
    attr = node->Attribute("file");
    if (NULL == attr) {
        LOG_ERROR("/config/skill[file] not found");
        return false;
    }
    skill_file_ = config_dir_ + attr;

    if (loadMiscConfig(misc_file_) == false) {
        LOG_ERROR("load misc config file(%s) failed",
                  misc_file_.c_str());
        return false;
    }
    if (loadSkillConfig(skill_file_) == false) {
        LOG_ERROR("load skill config file(%s) failed",
                  skill_file_.c_str());
        return false;
    }

    return true;
}

bool Config::loadMiscConfig(const std::string &file)
{
    XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", file.c_str());
        return false;
    }

    XMLElement *data_node = doc.FirstChildElement("data");
    if (NULL == data_node) {
        LOG_ERROR("/data not found");
        return false;
    }

    XMLElement *node = NULL;
    const char *attr = NULL;

    // default_skill
    node = data_node->FirstChildElement("default_skill");
    if (NULL == node) {
        LOG_ERROR("/data/default_skill not found");
        return false;
    }

    attr = node->Attribute("axe");
    if (NULL == attr) {
        LOG_ERROR("/data/default_skill[axe] not found");
        return false;
    }
    axe_melee_skill_id_ = ::atoi(attr);

    attr = node->Attribute("shoot");
    if (NULL == attr) {
        LOG_ERROR("/data/default_skill[shoot] not found");
        return false;
    }
    shoot_melee_skill_id_ = ::atoi(attr);

    attr = node->Attribute("magic");
    if (NULL == attr) {
        LOG_ERROR("/data/default_skill[magic] not found");
        return false;
    }
    magic_melee_skill_id_ = ::atoi(attr);

    return true;
}

bool Config::loadSkillConfig(const std::string &file)
{
    XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", file.c_str());
        return false;
    }

    XMLElement *data_node = doc.FirstChildElement("data");
    if (NULL == data_node) {
        LOG_ERROR("/data not found");
        return false;
    }

    const char *attr = NULL;
    size_t count = 0;

    // add idle skill
    SkillCell idle_cell;
    idle_cell.id_ = 0;
    idle_cell.level_ = 0;
    idle_cell.cooling_ = 0;
    idle_cell.distance_ = 0;
    skill_cells_[0] = idle_cell;

    for (XMLElement *cell_node = data_node->FirstChildElement("cell");
         cell_node != NULL;
         cell_node = cell_node->NextSiblingElement("cell")) {
        ++count;

        SkillCell cell;

        attr = cell_node->Attribute("id");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /data/cell[id] not found", count);
            return false;
        }
        cell.id_ = ::atoi(attr);

        attr = cell_node->Attribute("level");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /data/cell[level] not found", count);
            return false;
        }
        cell.level_ = ::atoi(attr);

        attr = cell_node->Attribute("cooling");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /data/cell[cooling] not found", count);
            return false;
        }
        cell.cooling_ = ::atoi(attr);

        attr = cell_node->Attribute("distance");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /data/cell[distance] not found", count);
            return false;
        }
        cell.distance_ = ::atoi(attr);

        attr = cell_node->Attribute("speed"); 
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /data/cell[speed] not found", count);
            return false;
        }
        cell.speed_ = ::atoi(attr);

        // check dumplicate
        if (getSkillCell(cell.id_, cell.level_) != NULL) {
            LOG_ERROR("count[%zd] dumplicate", count);
            return false;
        }

        int64_t index = ((uint64_t)cell.id_ << 32) + (uint64_t)cell.level_;
        skill_cells_.insert(std::make_pair(index, cell));
    }

    return true;
}

const SkillCell *Config::getSkillCell(int32_t id, int32_t level) const
{
    int64_t index = ((uint64_t)id << 32) + (uint64_t)level;

    SkillCellMap::const_iterator iter = skill_cells_.find(index);
    if (iter == skill_cells_.end()) {
        return NULL;
    }
    return &iter->second;
}

const SkillCell *Config::getIdleSkillCell() const
{
    return getSkillCell(0, 0);
}

} // namespace robot

