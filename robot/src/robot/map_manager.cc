#include "robot/map_manager.h"

#include <cstddef>
#include <cstdlib>
#include <map>
#include <ext/hash_map>
#include <tinyxml2/tinyxml2.h>

#include "robot/logging.h"
#include "robot/config.h"
#include "robot/position.h"
#include "robot/map.h"

namespace robot {

using tinyxml2::XML_NO_ERROR;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using brickred::UniquePtr;

class MapManager::Impl {
public:
    typedef __gnu_cxx::hash_map<int, Map *> MapMap;

    Impl();
    ~Impl();

    bool loadMaps(const std::string &file);
    Map *getMap(int map_id);

private:
    MapMap maps_;
};

MapManager::Impl::Impl()
{
}

MapManager::Impl::~Impl()
{
    for (MapMap::iterator iter = maps_.begin();
         iter != maps_.end(); ++iter) {
        delete iter->second;
    }
}

static bool loadMapBlockFiles(const std::string &file,
    std::map<int, std::string> *map_block_files)
{
    XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", file.c_str());
        return false;
    }

    XMLElement *package_node = doc.FirstChildElement("package");
    if (NULL == package_node) {
        LOG_ERROR("/package not found");
        return false;
    }

    const char *attr = NULL;
    size_t count = 0;

    for (XMLElement *map_node = package_node->FirstChildElement("map");
         map_node != NULL;
         map_node = map_node->NextSiblingElement("map")) {
        ++count;

        attr = map_node->Attribute("id");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /package/map[id] not found", count);
            return false;
        }
        int id = ::atoi(attr);

        attr = map_node->Attribute("resource");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /package/map[resource] not found", count);
            return false;
        }
        std::string file = Config::getInstance()->config_dir_ + attr;

        (*map_block_files)[id] = file;
    }

    return true;
}

bool MapManager::Impl::loadMaps(const std::string &file)
{
    XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", file.c_str());
        return false;
    }

    XMLElement *configure_node = doc.FirstChildElement("configure");
    if (NULL == configure_node) {
        LOG_ERROR("/configure not found");
        return false;
    }

    std::map<int, std::string> map_info_files;
    std::map<int, std::string> map_block_files;
    const char *attr = NULL;

    // load map info
    {
        XMLElement *map_info_node =
            configure_node->FirstChildElement("mapinfo");
        if (NULL == map_info_node) {
            LOG_ERROR("/configure/mapinfo not found");
            return false;
        }

        size_t count = 0;
        for (XMLElement *map_node = map_info_node->FirstChildElement("map");
             map_node != NULL;
             map_node = map_node->NextSiblingElement("map")) {
            ++count;

            attr = map_node->Attribute("id");
            if (NULL == attr) {
                LOG_ERROR("count[%zd] /configure/mapinfo/map[id]"
                          " not found", count);
                return false;
            }
            int id = ::atoi(attr);

            attr = map_node->Attribute("file");
            if (NULL == attr) {
                LOG_ERROR("count[%zd] /configure/mapinfo/map[file]"
                          " not found", count);
                return false;
            }
            std::string file = Config::getInstance()->config_dir_ + attr;

            map_info_files[id] = file;
        }
    }

    // load map block
    {
        XMLElement *map_block_node =
            configure_node->FirstChildElement("map_block");
        if (NULL == map_block_node) {
            LOG_ERROR("/configure/map_block not found");
            return false;
        }

        attr = map_block_node->Attribute("file");
        if (NULL == attr) {
            LOG_ERROR("/configure/map_block[file] not found");
            return false;
        }
        std::string block_file =
            Config::getInstance()->config_dir_ + attr;

        if (loadMapBlockFiles(block_file, &map_block_files) == false) {
            LOG_ERROR("load map block files from config file(%s) failed",
                      block_file.c_str());
            return false;
        }
    }

    for (std::map<int, std::string>::iterator iter = map_info_files.begin();
         iter != map_info_files.end(); ++iter) {
        int map_id = iter->first;

        UniquePtr<Map> map(new Map(map_id));
        if (map->load(iter->second, map_block_files[map_id]) == false) {
            LOG_ERROR("create map(%d) ailed", map_id);
            return false;
        }

        maps_.insert(std::make_pair(map_id, map.get()));
        map.release();
    }

    return true;
}

Map *MapManager::Impl::getMap(int map_id)
{
    MapMap::iterator iter = maps_.find(map_id);
    if (maps_.end() == iter) {
        return NULL;
    }
    return iter->second;
}

///////////////////////////////////////////////////////////////////////////////
MapManager::MapManager() :
    pimpl_(new Impl())
{
}

MapManager::~MapManager()
{
}

bool MapManager::loadMaps(const std::string &file)
{
    return pimpl_->loadMaps(file);
}

Map *MapManager::getMap(int map_id)
{
    return pimpl_->getMap(map_id);
}

} // namespace robot

